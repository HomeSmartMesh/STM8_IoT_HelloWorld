# See these boards in action [IoT_Frameworks (Sensor/Server/Client)](https://github.com/wassfila/IoT_Frameworks)

# Hello world samples for the [STM8 IoT Boards](https://github.com/wassfila/STM8_IoT_Boards)
This repository contains incremented projects that gradually cover hello world samples for :
 - Radio transceiver module
 - Weather sensors : temperature, humidity, air quality.
 - RGB Leds
 - Gyro / Accel
 - Ambient light and user gesture

# Samples Overview
## 01_HelloLed
Check if your compilation and flashing chain is running, the led should blink

## 02_HelloUart
Check if your USB to UART connection is correctly configured with the right com port and speed

## 02_HelloRF_NordicAPI
Check if your SPI connection to the nRF24L01+ is properly connected, it reads registers and displays them with their names and values on the UART console.
Note that the register names were declared for easier debug through the console, the default values can be cross checked with the datasheet.

## 05_Hello_RGBLeds:
C functions with nops to set a fixed value of RGB colors to some LEDs. The RGB values here can only be set on compile time.

## 06_Hello_RGBLeds_24BitsParam
Assembler function that reads any RGB values list from an array in memory and serializes the data to the LEDs. The RGB values here can be changed on run time.

## 07_Hello_Temperature
API draft for the DS18B20 to read the temperature. Note that there are two modes, the bus mode and the single sensor read.
The _Single_ functions are to be used when a single sensor is connected. They are to be used to identify the sensor ID before using it in a bus or when a single sensor is used.

# Licensing
- By default and everywhere when nothing is mentionned : Sources are free to use for commercial and private purpose, share, copy partially or completely.
- Unless a specific example is taken from another library where a different licensing model is explicitely mentionned : Due to switching the 'real' working repository to guthub, the Licensing is indeed more complex now as it is on a project subdirectory basis. Such snapshot libraries are not used for the purpose of this project but are here as a matter of comparision and criticism, which means that the final usage of such imported library depends on the purpose the end user will use them for.
