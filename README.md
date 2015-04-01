Pi Camera PanTilt in C
=================================

C Code for turning a Pi Camera into a Pan Tilt stills camera. It uses: 
* Adafruit Mini PanTilt kit (http://www.adafruit.com/product/1967);
* pca9685 LED/servo board;
* Raspberry Pi B;
* Raspberry Pi Camera and long ribbon;
* wiringPi library for I2C bus access.

#Usage
The compiled code can be run from the commandline using:
```
sudo ./picampantilt [OPTION] [arg1] [arg2] [OPTION] [arg1] [arg2]
```
This way multiple actions can be carried out on a single line. The possible OPTIONS are:
|--move|-m|Pan to arg1 degrees and tilt to arg2 degrees|
|--pan|-p|Pan to arg1 degrees|
|--tilt|-t|Tilt to arg1 degrees|
|--snapshot|-s|If arg1=capture, then capture image and name arg2|
|||If arg2=patrol3, then capture images at 90, 180, and 270 degrees|
|||If arg2=patrol5, then capture images at 90, 135, 180, 225 and 270 degrees|

Compile using:
```
gcc -Wall -lwiringPi -o picampantilt picampantilt.c pca9685.c
```
