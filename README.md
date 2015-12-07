# STM8_IoT_HelloWorld
First versions of the IoT Node Hello world samples
The IoT node is an STM8 based low cost development platform.
It is an initiative for an open product = open hardware + open source project.
find all the details in this link :
http://www.homesmartmesh.com/mediawiki/index.php/IoT_Node
This project will be incremented gradually to cover hollohello world samples for :
 - Radio transceiver module
 - Weather sensors : temperature, humidity, air quality.
 - RGB Leds
 - Gyro / Accel
 - Ambient light and user gesture

This IoT Node is part of a bigger project that extand the collaboration to a complete IoT environment.
It includes  interfaces to existing products and server applications for merging everything together.
The "Home Smart Mesh" concept is described in the main wiki page
http://www.homesmartmesh.com/mediawiki/index.php/Main_Page

Samples brief description (find more on the wiki page) :
01_HelloLed:	Check if your compilation and flashing chain is running, the led should blink
02_HelloUart:	Check if your USB to UART connection is correctly configured with the right com port and speed
02_HelloRF_NordicAPI:	Check if your SPI connection to the nRF24L01+ is properly connected, it reads registers and displays them with their names and values on the UART console.
Note that the register names were declared for easier debug through the console, the default values can be cross checked with the datasheet.

05_Hello_RGBLeds:	C functions with nops to set a fixed value of RGB colors to some LEDs. The RGB values here can only be set on compile time.
06_Hello_RGBLeds_24BitsParam:	Assembler function that reads any RGB values list from an array in memory and serializes the data to the LEDs. The RGB values here can be changed on run time.
07_Hello_Temperature:	API draft for the DS18B20 to read the temperature. Note that there are two modes, the bus mode and the single sensor read.
The _Single_ functions are to be used when a single sensor is connected. They are to be used to identify the sensor ID before using it in a bus or when a single sensor is used.
