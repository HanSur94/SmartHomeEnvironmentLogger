


![](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Pasted%20image%2020230129214140.png)

# Smart Home Environment Logger

This project is intended to create a custom smart home device that can be used in combination with the Home Assistant home automation system or together with the Blynk Framework. The following sections will provide you with more detailed informations to the device.

### Table of contents
* [Introduction](#Introduction)
* [Electrical Components](#Electrical-Components)
* [Printed Circiut Board](#Printed-Circiut-Board)
* [Home Assistant Configuration](#Home-Assistant-Configuration)
* [Arduino & Blynk Configuration](#Arduino-&-Blynk-Configuration)
* [Sensor Housing](#Sensor-Housing)
* [Examples](#Examples)
* [Bill of Materials (BOM)](# Bill-of-Materials-(BOM))
* [Built With](#built-with)
* [Authors](#authors)

## Introduction

The device is capable of measuring the following metrics like temperature, relative humidity, air pressure, CO2 concentration, TVOC concentration, UVA/UVB as well as the light level. Those metrics can then be used for further smart home automations or just for monitoring your indor environments.

## Electrical Components

Electrical components and their interconnection used for zthis project can be found in the following schematic. The operating brain of the smart home sytsem is a ordinary ESP8266 Chip on the Node MCU v3 development board. The Sensors used inside the device can be seen in the following table with their measured quantities.

| Board Name       | Sensor Name  | Metric                              | Units       |
| ---------------- | ------------ | ----------------------------------- | ----------- |
| DEBO Sens BME860 | Bosch BME860 | Temperature, Humidity, Air Pressure | °C, %, mPas |
| GUVA-S12SD       | GUVA-S12SD   | UVA / UVB                           | UV Index    |
| DEBO BH1750      | BH 1750      | Luminous flux                       | lm          |
| DEBO Sens CCS811 | CCS811       | CO2, TVOC                           | ppm, ppb    |

For usage without the need of a internet connection 5 RGB leds of the Type W2812B are used in order to disply a color code. This color code identifies the sensor and if predefined boundary values where exceeded. A Blue color is shown for values that are smaller than a predefined minimum, a red color is shown if a sensor value is higehr than a maximum pe defined value and a green color is shown if a sensor values is its defined beoundary. The position of the light indicates the actual sensor metric. This setup can be customized by any user in the Arduino code.

The connection between all parts can be found here:

![[Schematic_SmartEnvironmentSensor_2022-12-31.pdf]]


## Printed Circiut Board

![[PCB_PCB_PCB_SmartEnvironmentSensor_2022-12-31_2023-01-29.pdf]]

## Home Assistant Configuration

The Home Assistant configuration can be seen here:

```yaml

esphome:
  name: smartenvlogger

esp8266:
	# use a specific framework for the use of the fast led lib
  framework:
    version: 2.7.4
  board: nodemcuv2

# wifi credentials
wifi:
  ssid: "*******"
  password: "*******"

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: "*******"

ota:
  password: "*******"

captive_portal:

# setup i2c ports
i2c:
  sda: GPIO5
  scl: GPIO4
  scan: True

# setup a fast led component for the indication lights
# indication lights are still not working here.....
light:
  - platform: fastled_clockless
    chipset: WS2811
    pin: D5
    num_leds: 5
    rgb_order: GRB
    name: "SmartEnvLoggerLight"
    id: smartenvloggerlight

# setup the sensors
sensor:

	# setup the UVA/UVB sensors
  - platform: adc
    pin: A0
    name: "Living Room UV-A/UV-B"
    device_class: illuminance
    update_interval: 5s

	# setup the illuminace sensor
  - platform: bh1750
    name: "BH1750 Illuminance"
    address: 0x23
    update_interval: 5s

	# setup the bosch bme multisensor
  - platform: bme680
    temperature:
      name: "BME680 Temperature"
      oversampling: 16x
      device_class: temperature
      # setup an offset if the temperature of the sensor seems a bit off
      filters:
        - offset: -3.0

    pressure:
      name: "BME680 Pressure"
      oversampling: 16x
      device_class: pressure

    humidity:
      name: "BME680 Humidity" 
      oversampling: 16x
      id: hum
      device_class: humidity

    gas_resistance:
      name: "BME680 Gas Resistance"
      id: g_res
    address: 0x77
    update_interval: 5s

	# setup the ccs811 CO2 and TVOC sensor
  - platform: ccs811
    eco2:
      name: "CCS811 eCO2 Value"
      device_class: carbon_dioxide
    tvoc:
      name: "CCS811 Total Volatile Organic Compound"
      #device_class: volatile_organic_compounds 
    address: 0x5A
    update_interval: 5s

	# setup another sensor called Indoor Air Quality (IAQ) which is computed via the gas resistance and humidity
  - platform: template
    name: "BME680 IAQ"
    id: IAQ
    unit_of_measurement: IAQ
    device_class: aqi
    lambda: 'return log(id(g_res).state) + 0.04 *  id(hum).state;'

```


## Sensor Housing

The sensor housing was designed in order to fit the PCB with the soldered electrical components. Furthermore, openings into the gousing where incorperated in order to guarantee passage for fresh air to the sensors. Additionally, openings fo a Reset Button, DC Power Jack and for battery on/off switch where added.

For a more versatile positioning of the Smart Home Sensor additional sideways stand can be used.
![](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Pasted%20image%2020230129201735.png)
Full sensor housing wth ventilation openings. Furthermore two openings on the upper ending of the top cover can be seen where the BH1750 and GUVA-S12SD sensors are attached to. The cut out rectangle is used to mark the area for the indication LEDs.

![](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Pasted%image%20230129201644.png)
The sensor housing with its embedded electronics as well as buttons on the back side. 

![](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Pasted%20image%2020230129201722.png)
The sensor housing with its embedded electronics as well as buttons on the front side. 

## Arduino & Blynk Configuration

The Arduino code using the Blynk library can be found here:
[Arduino Code for Blynk](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Arduino/SmartEnvlogger.ino)

## Examples

#### Usage with Blynk

Here we can see how the Smart home sensor can be used within the Blynk Web Dashboard to show all the sensor Data:

![[BlynkConfiguration_1.png]]
Dashboard view of the Blynk web edition.

The configuration for  the Blynk datastreams can be found here:

![[BlynkConfiguration_3.png]]
All the configured datastreams for the web dashboard. The configuration should match the Arduino code where the virtual Blynk channels are set up. For further informations see the Blynk configuration.

#### Usage within Home Assistant

A example view of the dashboard in Home Assistant can be ound here:

![[Pasted image 20230129203700.png]]
Graphical view and list view of the used sensors from the Smart Envionment Logger in Home Assistant web dashboard.

#### Usage with Apple HomeKit

Using the Apple HomeKit integration for Home Assistant enables the usage of the sensors on all of your Aplle devices. An example of the Smart Home Environment Logger Sensors in Apple Home can be seen here:

![[Pasted image 20230129204024.png]]
Here you can see the sensors listed in the Apple Home App.

![[Pasted image 20230129204016.png]]
Further Automations are possible with the used sensors.


## Bill of Materials (BOM)

#### Electrical Components

| Material                | Quantity | Comment                                                   |
| ----------------------- | -------- | --------------------------------------------------------- |
| DEBO Sens BME860        | 1x       |                                                           |
| GUVA-S12SD              | 1x       |                                                           |
| DEBO BH1750             | 1x       |                                                           |
| DEBO Sens CCS811        | 1x       |                                                           |
| ESP Home Node MCU v3    | 1x       |                                                           |
| Printed Circiut Board   | 1x       | Design is explained                                       |
| WS2812B                 | 5x       |                                                           |
| DC005-2.0mm             | 1x       | Not needed if powered via micro usb of the esp controller |
| Switch Battery          | 1x       |                                                           |
| Switch Ext Power        | 1x       |                                                           |
| Resistor 10k            | 1x       |                                                           |
| TP4056 board            | 1x       | Needed if battery powered                                                          |
| MT3608 board            | 1x       | Needed if battery powered                                                          |
| Li-Ion Battery & Holder | 1x       | Needed if battery powered                                                          |

#### 3D Printed components

| Material       | Quantity | Comment                         |
| -------------- | -------- | ------------------------------- |
| Top Cover      | 1x       |                                 |
| Case Base      | 1x       |                                 |
| LED Holder     | 1x       |                                 |
| DC Jack Cover  | 1x       | only needed if DC Jack not used |
| Sideways Stand | 1x       | accessory                       |

#### Other parts
˘
| Material  | Quantity | Comment |
| --------- | -------- | ------- |
| M2 Screws | 6x       | For mounting the PCB        |
 


## Built With

* [Blynk](https://blynk.io) - Free Version
* [Home Asssitant](https://www.home-assistant.io) - Version: 2022.12.8
* [ESP-Home](https://esphome.io) - Version: 2022.12.8
* [Arduino](https://www.arduino.cc) - Version: 1.8.13

## Authors

* **HanSur94** - [HanSur94](https://github.com/HanSur94)
