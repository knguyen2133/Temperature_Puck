## Example Summary

Sample application to read Pressure, Temperature and Humidity from the BME280 sensor.

## Peripherals Exercised

* `Board_I2C0` - I2C used to communicate with BME280 Sensor

## Resources & Jumper Settings

> If you're using an IDE (such as CCS or IAR), please refer to Board.html in your project
directory for resources used and board-specific jumper settings. Otherwise, you can find
Board.html in the directory &lt;PLUGIN_INSTALL_DIR&gt;/source/ti/boards/&lt;BOARD&gt;.

## Example Usage

* Connect the BOOSTXL-SENSORS BoosterPack to the LaunchPad before powering the hardware.

* Open a serial session (e.g. [`PuTTY`](http://www.putty.org/ "PuTTY's Homepage"),teraterm, CCS terminal, etc.) to the appropriate COM port.
    * The COM port can be determined via Device Manager in Windows or via `ls /dev/tty*` in Linux.

The connection will have the following settings:
```
    Baud-rate:     115200
    Data bits:          8
    Stop bits:          1
    Parity:          None
    Flow Control:    None
```
* Run the example.

* The example collects the samples for humidity, temperature and pressure from BME280 driver and displays it on serial console as shown below.
```

Starting the BME280 sensor example...


I2C Initialized!

70 KPa(Pressure), 22 DegC(Temp), 59 %RH(Humidity)

90 KPa(Pressure), 26 DegC(Temp), 58 %RH(Humidity)

90 KPa(Pressure), 26 DegC(Temp), 58 %RH(Humidity)

```

## Application Design Details

This application uses 1 thread:

`mainThread` - performs the following actions:

1. Opens and initializes an I2C Driver.

2. Initialize the BME280 sensor and set readout template.

3. Periodically read the temperature, humidity and pressure reading from the BME280 sensor
