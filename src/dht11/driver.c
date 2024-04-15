#include "driver.h"

#define GPIO_HIGH true
#define GPIO_LOW false
#define DHT11_LOW_REQUEST_MS 20
#define DHT11_HIGH_REQUEST_US 20
#define DHT11_ZERO_BIT_LIMIT_US 53
#define DHT11_DATA_BITS_COUNT 40


typedef enum dht11_fsm_state_t
{
    DHT11_FSM_IDLE,
    DHT11_FSM_STARTED,
    DHT11_FSM_LOW_REQUEST_SENT,
    DHT11_FSM_HIGH_REQUEST_SENT,
    DHT11_FSM_LOW_RESPONSE_RECEIVED,
    DHT11_FSM_HIGH_RESPONSE_RECEIVED,
    DHT11_FSM_DATA_RECEIVED,
    DHT11_FSM_DATA_CHECKED
}
dht11_fsm_state_t;

typedef enum dht11_fsm_trigger_t
{
    DHT11_FSM_TRIGGER_START,
    DHT11_FSM_TRIGGER_LOW_REQUEST_START,
    DHT11_FSM_TRIGGER_LOW_REQUEST_END,
    DHT11_FSM_TRIGGER_HIGH_REQUEST_START,
    DHT11_FSM_TRIGGER_HIGH_REQUEST_END,
    DHT11_FSM_TRIGGER_EDGE_FALL,
    DHT11_FSM_TRIGGER_EDGE_RISE,
    DHT11_FSM_TRIGGER_STOP,
}
dht11_fsm_trigger_t;


typedef struct dht11_state_t
{
    dht11_data_t sensor_data;
    uint received_bits;
    uint64_t last_signal_time;
    dht11_fsm_state_t fsm;
    uint dht_pin;
}
dht11_state_t;


static void dht11_set_bit(const uint bit_value);

static int64_t dht11_low_request_alarm_callback(alarm_id_t id, void *user_data);
static int64_t dht11_high_request_alarm_callback(alarm_id_t id, void *user_data);
static void dht11_irq_callback(uint gpio, uint32_t event_mask);

static void dht11_sfm_start(dht11_fsm_trigger_t trigger);
static void dht11_sfm_send_low_request(dht11_fsm_trigger_t trigger);
static void dht11_sfm_send_high_request(dht11_fsm_trigger_t trigger);
static void dht11_sfm_receive_low_response(dht11_fsm_trigger_t trigger);
static void dht11_sfm_receive_high_response(dht11_fsm_trigger_t trigger);
static void dht11_sfm_receive_bit_value(dht11_fsm_trigger_t trigger);
static void dht11_sfm_check_data(dht11_fsm_trigger_t trigger);
static void dht11_sfm_stop(dht11_fsm_trigger_t trigger);

static void dht11_sfm_routine(dht11_fsm_trigger_t trigger);


dht11_state_t dht11_state;


bool dht11_is_valid_data(const dht11_data_t* pdata)
{
    return (
        pdata->integral_RH + 
        pdata->decimal_RH + 
        pdata->integral_T + 
        pdata->decimal_T
    ) == pdata->checksum;
}


dht11_data_t* dht11_sensor_data()
{
    return &(dht11_state.sensor_data);
}


void dht11_request_data(const uint dht_pin)
{
    dht11_state.dht_pin = dht_pin;
    dht11_sfm_routine(DHT11_FSM_TRIGGER_START);
}


void dht11_set_bit(const uint bit_value)
{
    uint value_no = dht11_state.received_bits >> 3;

    switch (value_no)
    {
        case 0:
            dht11_state.sensor_data.integral_RH <<= 1;
            dht11_state.sensor_data.integral_RH |= bit_value;
            break;
        case 1:
            dht11_state.sensor_data.decimal_RH <<= 1;
            dht11_state.sensor_data.decimal_RH |= bit_value;
            break;
        case 2:
            dht11_state.sensor_data.integral_T <<= 1;
            dht11_state.sensor_data.integral_T |= bit_value;
            break;
        case 3:
            dht11_state.sensor_data.decimal_T <<= 1;
            dht11_state.sensor_data.decimal_T |= bit_value;
            break;
        case 4:
            dht11_state.sensor_data.checksum <<= 1;
            dht11_state.sensor_data.checksum |= bit_value;
            break;
        default:
            break;
    }
}


int64_t dht11_low_request_alarm_callback(alarm_id_t id, void *user_data)
{
    dht11_sfm_routine(DHT11_FSM_TRIGGER_LOW_REQUEST_END);   
    return 0;
}

int64_t dht11_high_request_alarm_callback(alarm_id_t id, void *user_data)
{
    dht11_sfm_routine(DHT11_FSM_TRIGGER_HIGH_REQUEST_END);
    return 0;
}

void dht11_irq_callback(uint gpio, uint32_t event_mask)
{
    if(gpio_get(dht11_state.dht_pin) == GPIO_HIGH)
    {
        dht11_sfm_routine(DHT11_FSM_TRIGGER_EDGE_RISE);      
    }
    else
    {
        dht11_sfm_routine(DHT11_FSM_TRIGGER_EDGE_FALL);
    }
}


void dht11_sfm_start(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_START)
    {
        dht11_state.fsm = DHT11_FSM_STARTED;
        dht11_state.received_bits = 0;
        dht11_state.last_signal_time = 0;
        memset(&dht11_state.sensor_data, 0, sizeof(dht11_data_t));

        gpio_set_irq_enabled_with_callback(
            dht11_state.dht_pin, 
            GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
            true,
            dht11_irq_callback
        );

        dht11_sfm_routine(DHT11_FSM_TRIGGER_LOW_REQUEST_START);
    }
}

void dht11_sfm_send_low_request(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_LOW_REQUEST_START)
    {
        gpio_set_dir(dht11_state.dht_pin, GPIO_OUT);
        gpio_put(dht11_state.dht_pin, GPIO_LOW);

        add_alarm_in_ms(DHT11_LOW_REQUEST_MS, dht11_low_request_alarm_callback, NULL, true);
    }
    else 
    if(trigger == DHT11_FSM_TRIGGER_LOW_REQUEST_END)
    {
        dht11_state.fsm = DHT11_FSM_LOW_REQUEST_SENT;

        dht11_sfm_routine(DHT11_FSM_TRIGGER_HIGH_REQUEST_START);
    }
}

void dht11_sfm_send_high_request(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_HIGH_REQUEST_START)
    {
        gpio_put(dht11_state.dht_pin, GPIO_HIGH);
        add_alarm_in_us(DHT11_HIGH_REQUEST_US, dht11_high_request_alarm_callback, NULL, true);
    }
    else 
    if(trigger == DHT11_FSM_TRIGGER_HIGH_REQUEST_END)
    {
        gpio_set_dir(dht11_state.dht_pin, GPIO_IN);

        dht11_state.fsm = DHT11_FSM_HIGH_REQUEST_SENT;
    }
}

void dht11_sfm_receive_low_response(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_EDGE_RISE)
    {
        dht11_state.fsm = DHT11_FSM_LOW_RESPONSE_RECEIVED;
    }
}

void dht11_sfm_receive_high_response(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_EDGE_FALL)
    {
        dht11_state.fsm = DHT11_FSM_HIGH_RESPONSE_RECEIVED;
    }
}

void dht11_sfm_receive_bit_value(dht11_fsm_trigger_t trigger)
{
    uint64_t current_time = time_us_64();

    if(trigger == DHT11_FSM_TRIGGER_EDGE_FALL)
    {
        uint64_t signal_time = current_time - dht11_state.last_signal_time;
        uint bit_value = (signal_time > DHT11_ZERO_BIT_LIMIT_US) ? 0b1 : 0b0;

        dht11_set_bit(bit_value);
        ++dht11_state.received_bits;

        if(dht11_state.received_bits == DHT11_DATA_BITS_COUNT)
        {
            dht11_state.fsm = DHT11_FSM_DATA_RECEIVED;
        }
    }

    dht11_state.last_signal_time = current_time;
}

void dht11_sfm_check_data(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_EDGE_RISE)
    {
        dht11_state.sensor_data.is_valid = dht11_is_valid_data(&dht11_state.sensor_data);
        dht11_state.fsm = DHT11_FSM_DATA_CHECKED;

        dht11_sfm_routine(DHT11_FSM_TRIGGER_STOP);
    }
}

void dht11_sfm_stop(dht11_fsm_trigger_t trigger)
{
    if(trigger == DHT11_FSM_TRIGGER_STOP)
    {
        gpio_set_irq_enabled(
            dht11_state.dht_pin, 
            GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE,
            false
        );

        dht11_state.last_signal_time = 0;
        dht11_state.fsm = DHT11_FSM_IDLE;
    }
}


void dht11_sfm_routine(dht11_fsm_trigger_t trigger)
{
    switch (dht11_state.fsm)
    {
    case DHT11_FSM_IDLE:
        dht11_sfm_start(trigger);
        break;
    case DHT11_FSM_STARTED:
        dht11_sfm_send_low_request(trigger);
        break;
    case DHT11_FSM_LOW_REQUEST_SENT:
        dht11_sfm_send_high_request(trigger);
        break;
    case DHT11_FSM_HIGH_REQUEST_SENT:
        dht11_sfm_receive_low_response(trigger);
        break;
    case DHT11_FSM_LOW_RESPONSE_RECEIVED:
        dht11_sfm_receive_high_response(trigger);
        break;
    case DHT11_FSM_HIGH_RESPONSE_RECEIVED:
        dht11_sfm_receive_bit_value(trigger);
        break;
    case DHT11_FSM_DATA_RECEIVED:
        dht11_sfm_check_data(trigger);
        break;
    case DHT11_FSM_DATA_CHECKED:
        dht11_sfm_stop(trigger);
        break;
    default:
        break;
    }
}