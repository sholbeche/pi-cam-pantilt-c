Pi Camera PanTilt in C
=================================

C Code for turning a Pi Camera into a Pan Tilt stills camera. It uses: 
* Adafruit Mini PanTilt kit (http://www.adafruit.com/product/1967);
* pca9685 LED/servo board;

Dependent on wiringPi for accessing I2C bus

Compile using:
gcc -Wall -lwiringPi -o picampantilt picampantilt.c
