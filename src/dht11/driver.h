#ifndef DHT11_DRIVER_H
#define DHT11_DRIVER_H

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <pico/stdlib.h>
#include <hardware/timer.h>


typedef struct dht11_data_t
{
    uint8_t integral_RH;
    uint8_t decimal_RH;
    uint8_t integral_T;
    uint8_t decimal_T;
    uint8_t checksum;
    bool is_valid;
}
dht11_data_t;


bool dht11_is_valid_data(const dht11_data_t* pdata);
void dht11_request_data(const uint dht_pin);
dht11_data_t* dht11_sensor_data();


#endif //DHT11_DRIVER_H