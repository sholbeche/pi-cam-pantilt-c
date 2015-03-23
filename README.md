PCA9685 PanTilt Servo driver in C
=================================

C Code for driving Adafruit Mini PanTilt kit (http://www.adafruit.com/product/1967
via a pca9685 board from a Raspberry Pi B 
Servos connected to channels 0(Pan) and 1(Tilt) on pca9685

Original Python version https://github.com/pimoroni/PanTiltFacetracker/
Dependent on wiringPi for accessing I2C bus

Compile using:
gcc -Wall -lwiringPi -o pca9685 pca9685.c
