# Raspberry Pi Pico RDE Firmware

## Open BMC image build (Raspberry Pi)

### Build Open BMC image

Clone Open BMC distro
```bash
git clone https://github.com/openbmc/openbmc.git
cd openbmc
```

Create default layer.conf file (required for custom TEMPLATECONF). Filepath should be as follows: $TEMPLATECONF/../../layer.conf
```bash
cat << EOF > meta-evb/meta-evb-raspberrypi/conf/layer.conf
# We have a conf and classes directory, add to BBPATH
BBPATH .= ":${LAYERDIR}"

BBFILE_COLLECTIONS += "evb-evb-raspberrypi"
BBFILE_PATTERN_evb-raspberrypi = ""
BBFILE_PATTERN_IGNORE_EMPTY_evb-raspberrypi =  "1"
LAYERSERIES_COMPAT_evb-raspberrypi = "nanbield scarthgap"
EOF
```

Set TEMPLATECONF for Raspberrypi
```bash
export TEMPLATECONF=meta-evb/meta-evb-raspberrypi/conf/templates/default
echo $TEMPLATECONF
```

Setup Open BMC build environment
```bash
. openbmc-env
```

Configure bblayers.conf
```bash
cat << EOF >> conf/bblayers.conf

include conf/distro/include/pldm.inc
EOF
```

Configure local.conf
```bash
cat << EOF >> conf/local.conf

# Additional Build Options
ENABLE_SPI_BUS = "1"
ENABLE_W1 = "1"
ENABLE_I2C = "1"
ENABLE_UART = "1"

KERNEL_MODULE_AUTOLOAD:rpi += "i2c-dev i2c-bcm2708"

IMAGE_FSTYPES += " rpi-sdimg"

FLASH_SIZE = "262144"
FLASH_MANIFEST_OFFSET:flash-262144 ?= "888"
FLASH_UBOOT_ENV_OFFSET:flash-262144 ?= "896"
FLASH_KERNEL_OFFSET:flash-262144 ?= "1024"
FLASH_ROFS_OFFSET:flash-262144 ?= "21504"
FLASH_RWFS_OFFSET:flash-262144 ?= "98304"
FLASH_UBI_RWFS_SIZE:flash-262144 ?= "45600"
FLASH_UBI_RWFS_TXT_SIZE:flash-262144 ?= "32MiB"
EOF
```

Run build
```bash
bitbake obmc-phosphor-image
```

## Flash Raspberry Pi SD-card

<i>For WSL users:</i>

Flash your SD-card using [Balena Etcher](https://etcher.balena.io/#download-etcher). The image is located in build directory at ```tmp/deploy/images/raspberrypi/obmc-phosphor-image-raspberrypi-DATETIME.rpi-sdimg```. ```DATETIME``` is in format ```%YYYY%MM%DD%hh%mm%ss``` (without spaces). For example, ```obmc-phosphor-image-raspberrypi-20240321044920.rpi-sdimg```.

After flashing, insert the SD-card and plug an Ethernet cable into the board.

## Connect by SSH
First of all, find out what is the IP-address of Raspberry Pi board in your local network. After that connect to the board by SSH.
```bash
ssh root@BMC_IP
```

For example
```bash
ssh root@192.168.0.107
```

The password for login is below (default Open BMC password)
```bash
0penBmc
```

## Disable kernel serial console
If the board has only 1 uart peripheral, you must ensure that it is not used for serial terminal. 

Usually cmdline.txt is located at /boot directory. If it is, open this file in editor.
```bash
vi /boot/cmdline.txt
```

Otherwise mount the partition, where it is located, and open cmdline txt.
```bash
mkdir /mnt/c
mount /dev/mmcblk0p1 /mnt/c
vi /mnt/c/cmdline.txt
```

Replace ```console=serial0,115200``` by ```console=null,115200```. It will redirect kernel debug output into <b>/dev/null</b> instead of <b>/dev/serial0</b>. After that, save and exit (in vi editor <b>SHIFT + I</b> => <b>wq</b> => <b>ENTER</b>).

To apply changes reboot the board
```bash
reboot
```

## Enable MCTP and PLDM daemons verbosity
To enable <b>mctpd</b> (MCTP daemon) verbosity edit the service config file
```bash
vi /lib/systemd/system/pldmd.service
```
Append ```ExecStart=/usr/sbin/mctpd``` with ```-v``` option.

Same with <b>pldmd</b> (PLDM daemon)
```bash
vi /lib/systemd/system/pldmd.service
```
and append ```ExecStart=/usr/bin/pldmd``` line with ```--verbose``` option.

To apply changes reload systemctl config files.
```bash
systemctl daemon-reload
```

To check service logs enter
```bash
journalctl -u mctpd -b
```

or 

```bash
journalctl -u pldmd -b
```

## MCTP endpoint setup

### Link the device file (serial binding only)

<b>mctp</b> tool is used for primary endoint setup.

<i>Serial binding:</i>

Link any available serial character device. ```/dev/serialX``` is a common serial device name, where X is its number. For example, ```/dev/serial0```, ```/dev/serial1``` etc. Usually it is a softlink to ```/dev/ttyAMAX``` device, where X is also its number. ```ttyAMA``` character devices are initially UART interfaces.

```bash
mctp link serial /dev/serial0
```

Keep the tab with running mctp tool opened while you this device is used. mctp tool just keeps the device file opened while it's used. Following actions do in another tab.

### Set the interface up

After linking device file check the available MCTP network devices
```bash
mctp link
```

Following output sholud appear:
```
dev lo index 1 address 0x00:00:00:00:00:00 net 1 mtu 65536 up
dev mctpserial0 index 3 address 0x(no-addr) net 1 mtu 68 down
```

The first one is a loopback interface, the second is a serial one. Each network device usually have following name format: "mctp" +  bound device name. For exemple, /dev/i2c3 character device will be referred to as mctpi2c3.

Now, set the appeared interface up
```bash
mctp link set mctpserial0 up
```

### Assign MCTP EID to the bus owner

After setting interface up, assign an available MCTP address to it. For example:
```bash
mctp address add 0x08 dev mctpserial0
```

```mctpserial0``` is a network device name, ```0x08``` is an MCTP address of this BMC device. You can choose any available and non-reserved.

### Assign MCTP EID to the bus owner
Before following actions, MCTP responder (i.e. RDE device) must be switched on and connected with requester by peripherals. It should be done by <b>mctpd</b> daemon using DBus interface.
``` bash
busctl introspect xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp
```

Interface overview
```
NAME                                TYPE      SIGNATURE  RESULT/VALUE  FLAGS
au.com.CodeConstruct.MCTP           interface -          -             -
.AssignEndpoint                     method    say        yisb          -
.AssignEndpointStatic               method    sayy       yisb          -
.LearnEndpoint                      method    say        yisb          -
.SetupEndpoint                      method    say        yisb          -
```


To check the responder EID, call LearnEndpoint method. 
```bash
busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP LearnEndpoint say mctpserial0 0
```

```LearnEndpoint``` takes network device name (s or UTF-8 string) and binding address (ay or byte array) as parameters. All serial devices has no address. Each I2C device use its slave address as device address. For example, ```1 0x24```. 1 is byte array length, 0x24 is array data. For serial devices array length is 0 and no data provided.

If the responder EID is 0, new EID should be assigned.
```bash
busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP SetupEndpoint say mctpserial0 0
```
On success returns following values (example):
```yisb 9 1 "/xyz/openbmc_project/mctp/1/9" true```
where yisb - signature, 9 (y or byte) - EID, 1 (i or integer) - network ID, "/xyz/openbmc_project/mctp/1/9" (s or UTF-8 string) - D-Bus object path, true (b or boolean) - is EID already known.


```SetupEndpoint``` assignes a dynamic EID to the responder. BMC device will assign the least number available. Parameters are the same as in ```LearnEndpoint``` function. If endoint is already recognized by BMC device, no action will be performed.


To assign new EID to existing endpoint use ```AssignEndpoint```:
```bash
busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP AssignEndpoint say mctpserial0 0 0x20
```
This call will assign 0x20 as new endpoint. Parameters are the same as in ```SetupEndpoint```, but with the EID as the last parameter. To assign static endpoint use ```AssignEndpointStatic``` (may not be supported by MCTP responder due to specification). 
```bash
busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP AssignEndpointStatic say mctpserial0 0 0x20
```
The same as ```AssignEndpoint``` call, but the endpoint is not dynamic. Functions response parameters in ```SetupEndpoint```, ```AssignEndpoint``` and ```AssignEndpointStatic``` are the same. First three response parameters are the same as in ```SetupEndpoint``` (EID, network ID, object path). The fourth parameter (boolean) tells if a new EID was assigned.


After EID assignment, add route to assigned address. For example:
```bash
mctp route add 0x09 via mctpserial0
```
where 0x09 - responder EID, mctpserial0 - bus owner interface name.

Then, check if all required routes are available:
```
mctp route
```

Example output:
```
eid min 9 max 9 net 1 dev mctpserial0 mtu 0
eid min 8 max 8 net 1 dev mctpserial0 mtu 0
```
On success, two or more routes will appear - the first line is a responder route, the second one belongs to the bus owner.

After successful setup, introspect the EID D-Bus object
```bash
busctl introspect xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp/1/9
```

Example output:
```
NAME                                TYPE      SIGNATURE RESULT/VALUE                           FLAGS
au.com.CodeConstruct.MCTP.Endpoint  interface -         -                                      -
.Remove                             method    -         -                                      -
.SetMTU                             method    u         -                                      -
org.freedesktop.DBus.Introspectable interface -         -                                      -
.Introspect                         method    -         s                                      -
org.freedesktop.DBus.Peer           interface -         -                                      -
.GetMachineId                       method    -         s                                      -
.Ping                               method    -         -                                      -
org.freedesktop.DBus.Properties     interface -         -                                      -
.Get                                method    ss        v                                      -
.GetAll                             method    s         a{sv}                                  -
.Set                                method    ssv       -                                      -
.PropertiesChanged                  signal    sa{sv}as  -                                      -
xyz.openbmc_project.Common.UUID     interface -         -                                      -
.UUID                               property  s         "9a2d7206-cc60-8c87-cb46-0267e519c1e4" const
xyz.openbmc_project.MCTP.Endpoint   interface -         -                                      -
.EID                                property  y         9                                      const
.NetworkId                          property  i         1                                      const
.SupportedMessageTypes              property  ay        2 0 1                                  const
```
This function sends ```Get endpoint UUID``` and ```Get message type support``` commands to the MCTP responder. After that MCTP endpoint is ready to use.


To perform PLDM endpoint discovery, restart ```pldmd``` daemon:
```bash
systemctl restart pldmd
```

After that, check the daemon logs:
```bash
journalctl -u pldmd -b
```

If there is an MCTP endpoint, that supports PLDM messaging, BMC device will send following request messages:
- Get PLDM commands \(type 0\)
- Get PLDM version \(type 0\)
- Get PDR \(type 2\)

<i><u>Note</u>: Unfortunately, there is no RDE requester support in Open BMC (this feature is in development). Therefore, all RDE requests will be done manually using ```pldmtool```</i>


## Quick MCTP + PLDM endpoint setup
### Full setup
<u>Tab 1</u>
```bash
mctp link serial /dev/serial0
```

<u>Tab 2</u>
```bash
mctp link set mctpserial0 up
mctp address add 0x08 dev mctpserial0

busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP LearnEndpoint say mctpserial0 0

busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP SetupEndpoint say mctpserial0 0

mctp route add 0x09 via mctpserial0

busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP LearnEndpoint say mctpserial0 0

busctl introspect xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp/1/9

systemctl restart pldmd
```


### Responder restart
```bash
busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP LearnEndpoint say mctpserial0 0

busctl call xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp au.com.CodeConstruct.MCTP SetupEndpoint say mctpserial0 0

busctl introspect xyz.openbmc_project.MCTP /xyz/openbmc_project/mctp/1/9
```

### Requester restart
<i>The same as full setup, but without ```SetupEndpoint``` D-Bus call</i>