# Glucose Monitoring System

Non-invasive Glucose detector and monitoring System based on ESP32.

## Dependencies
Make sure to download and install these library on your Arduino IDE

 - [NTPClient](https://github.com/arduino-libraries/NTPClient)
 - [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
 - [Firebase ESP32 Client](https://github.com/mobizt/Firebase-ESP32)
 - [MAX30100](https://github.com/oxullo/Arduino-MAX30100)

There are several modification with both of this library that you can copy from [here](https://github.com/aliffathoni/Glucose-Monitoring-System/tree/main/extras) and paste it on your saved library.

## Features

- Offline Mode
- One Button Control
- Firebase Ready
- Sleep Mode

## Installation

Since there are several images and icons appear on LCD make sure to set your partition sceme to huge app with no ota so you can use all ~4MB of your ESP32 Flash Memory.

#### PlatformIO

On [platformio.ini](/platformio.ini) add this as build flags :
```.
    board_build.partitions = huge_app.csv
```
    
#### Arduino IDE

Go to tools and change your board partition scheme to this :
![partition scheme](/extras/Images/partition_scheme.jpeg)
    
## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE) file for details.