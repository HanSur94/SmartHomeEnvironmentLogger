# SmartHomeEnvironmentLogger
This project is intended to create a custom smart home device that can be used in combination with the Home Assistant home automation system or together with the Blynk Framework. The following sections will provide you with more detailed informations to the device.

### Table of contents
* [Introduction](#Introduction)
* [Electrical Components](#Electrical Components)
* [Home Assistant Configuration](#Home Assistant Configuration)
* [Arduino & Blynk Configuration](#Arduino & Blynk Configuration)
* [Sensor Housing](#Sensor Housing)
* [Examples](#Examples)
* [Bill of Materials](#Bill of Materials)
* [Built With](#built-with)
* [Authors](#authors)

## Introduction

The device is capable of measuring the following metrics like temperature, relative humidity, air pressure, CO2 concentration, TVOC concentration, UVA/UVB as well as the light level. Those metrics can then be used for further smart home automations or just for monitoring your indor environments.

## Electrical Components

Electrical components and their interconnection used for zthis project can be found in the following schematic. The operating brain of the smart home sytsem is a ordinary ESP8266 Chip on the Node MCU v3 development board. The Sensors used inside the device can be seen in the following table with their measured quantities.

| Board Name | Sensor Name  | Metric | Units |
|-----------|-----------|-----------|-----------|
| DEBO Sens BME860  | Bosch BME860 | Temperature, Humidity, Air Pressure | °C, %, mPas   |
| GUVA-S12SD        | GUVA-S12SD   | UVA / UVB                           | UV Index               |
| DEBO BH1750       | BH 1750      | Luminous flux                       | lm                     |
| DEBO Sens CCS811  | CCS811       | CO2, TVOC                           | ppm, ppb               | 

For usage without the need of a internet connection 5 RGB leds of the Type W2812B are used in order to disply a color code. This color code identifies the sensor and if predefined boundary values where exceeded. A Blue color is shown for values that are smaller than a predefined minimum, a red color is shown if a sensor value is higehr than a maximum pe defined value and a green color is shown if a sensor values is its defined beoundary. The position of the light indicates the actual sensor metric. This setup can be customized by any user in the Arduino code.

The connection between all parts can be found here:

![alt_text](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Schematic_SmartEnvironmentSensor_2022-12-31.pdf)

## Home Assistant Configuration

The Home Assistant configuration can be seen here:

```yaml

esphome:
  name: smartenvlogger

esp8266:
  framework:
    version: 2.7.4
  board: nodemcuv2

wifi:
  ssid: ""
  password: ""

# Enable logging
logger:

# Enable Home Assistant API
api:
  encryption:
    key: ""

ota:
  password: ""

captive_portal:

i2c:
  sda: GPIO5
  scl: GPIO4
  scan: True

light:
  - platform: fastled_clockless
    chipset: WS2811
    pin: D5
    num_leds: 5
    rgb_order: GRB
    name: "SmartEnvLoggerLight"
    id: smartenvloggerlight

sensor:
  - platform: adc
    pin: A0
    name: "Living Room UV-A/UV-B"
    device_class: illuminance
    update_interval: 5s

  - platform: bh1750
    name: "BH1750 Illuminance"
    address: 0x23
    update_interval: 5s

  - platform: bme680
    temperature:
      name: "BME680 Temperature"
      oversampling: 16x
      device_class: temperature
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

  - platform: ccs811
    eco2:
      name: "CCS811 eCO2 Value"
      device_class: carbon_dioxide
    tvoc:
      name: "CCS811 Total Volatile Organic Compound"
      #device_class: volatile_organic_compounds 
    address: 0x5A
    update_interval: 5s
      
  - platform: template
    name: "BME680 IAQ"
    id: IAQ
    unit_of_measurement: IAQ
    device_class: aqi
    lambda: 'return log(id(g_res).state) + 0.04 *  id(hum).state;'

```

## Arduino & Blynk Configuration

The Arduino code using the Blynk library can be found here:
(https:)



## Built With

* [Blynk](https://blynk.io) - Free Version
* [Home Asssitant](https://www.home-assistant.io) - Version: 2022.12.8
* [Arduino](https://www.arduino.cc) - Version: 1.8.13

## Authors

* **HanSur94** - [HanSur94](https://github.com/HanSur94)

