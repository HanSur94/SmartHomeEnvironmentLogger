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
| DEBO Sens BME860  | Bosch BME860 | Temperature, Humidity, Air Pressure | °C, rel.Humid., mPas   |
| GUVA-S12SD        | GUVA-S12SD   | UVA / UVB                           | UV Index               |
| DEBO BH1750       | BH 1750      | Luminous flux                       | lm                     |
| DEBO Sens CCS811  | CCS811       | CO2, TVOC                           | ppm, ppb               | 


The connection between all parts can be found here:

![Schematic](https://github.com/HanSur94/SmartHomeEnvironmentLogger/blob/main/Schematic_SmartEnvironmentSensor_2022-12-31.pdf)

---------

![alt text]()

## Built With

* [Blynk](https://blynk.io) - Free Version
* [Home Asssitant](https://www.home-assistant.io) - Version: 2022.12.8
* [Arduino](https://www.arduino.cc) - Version: 1.8.13

## Authors

* **HanSur94** - [HanSur94](https://github.com/HanSur94)

