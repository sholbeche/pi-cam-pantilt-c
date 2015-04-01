// C Code for making a pan tilt static web cam using:
// - Adafruit Mini PanTilt kit (http://www.adafruit.com/product/1967);
// - pca9685 LED/Servo board;
// - Raspberry Pi B 
//
// Original Python version https://github.com/pimoroni/PanTiltFacetracker/
// Dependent on wiringPi for accessing I2C bus
//
// Compile using:
// gcc -Wall -lwiringPi -o pca9685 pca9685.c

#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "pca9685.h"


int main_pca9685(){
	uint16_t fd = 0;
	fd = wiringPiI2CSetup(PCA9685_ADDR);
	setUpDevice(fd);

// Example drive both pan and tilt through 90 deg. 
// Pan from 90(E) to 180(S). Tilt from 90(vert) to 0(hori)
	int idx;
	for(idx=0; idx < 90; idx++){
		pan(fd,idx+90);
		tilt(fd,90-idx);
		usleep(125000);
	}

// Stop the pca9685 driving the servos
	sleepOn(fd);
	return 0;
}


uint8_t setUpDevice(uint16_t fd){
	setAllPWM(fd,0,0);
	wiringPiI2CWriteReg8(fd,MODE2,OUTDRV);
	wiringPiI2CWriteReg8(fd,MODE1,ALLCALL);
	delay(0.005);
	uint8_t mode1=wiringPiI2CReadReg8(fd,MODE1);
	mode1=mode1 & SLEEP;
	wiringPiI2CWriteReg8(fd,MODE1,mode1);
	delay(0.005);
	setPWMFreq(fd,50);
	return 0;
}


uint8_t pan(uint16_t fd, int16_t deg){
	move(fd, PAN, convAxis(PAN,deg));
	return 0;
}


uint8_t tilt(uint16_t fd, int16_t deg){
	move(fd, TILT, convAxis(TILT,deg));
	return 0;
}


uint8_t move(uint16_t fd, uint8_t s, int16_t deg){
// Calculate pulse length
//	float pwm = 570.0 + ((deg/180.0) * 1700.0);
	float pwm = PULSEMIN + ((deg/180.0) * PULSEINC);
	pwm = (4096.0/20000.0) * pwm;
//	printf("deg:%d pwm:%f\n",deg,pwm);
	setPWM(fd, s, 0, (int) pwm);
	return 0;
}


// Shift North 0 deg/South 180 deg to anticlockwise servo 180 deg pan
// Shift Up 90 deg/Hori 0 deg to top down servo 145 deg tilt
// Includes offset and scale constants to correct pwm calculation for specific servos
int16_t convAxis(uint8_t s, int16_t deg){
	if (!s) {
		if ((deg>PANMAX) || (deg<PANMIN)){
			deg = (PANMAX-PANMIN)/2; //Out of Range then go to centre
		}
		deg = PANOFFSET+((270-deg)*PANSCALE);
	} else {
		if ((deg>TILTMAX) || (deg<TILTMIN)){
			deg = TILTOFFSET+((90-TILTMIN)*TILTSCALE); // OoR go to minimum
		}
		deg = TILTOFFSET+((90-deg)*TILTSCALE);
	}
	return deg;
}


uint8_t setPWMFreq(uint16_t fd, uint16_t freq){
	float prescaleval = CLOCKFREQ;
	prescaleval = prescaleval/4096.0;
	prescaleval = prescaleval/freq;
	prescaleval = prescaleval - 1.0;
	float prescale = (prescaleval + 0.5);
	uint8_t oldmode = wiringPiI2CReadReg8(fd,MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10;
	wiringPiI2CWriteReg8(fd,MODE1, newmode);
	wiringPiI2CWriteReg8(fd,PRESCALE, (int)prescale);
	wiringPiI2CWriteReg8(fd,MODE1, oldmode);
	delay(0.005);
	wiringPiI2CWriteReg8(fd,MODE1, oldmode | 0x80);
	return 0;
}


uint8_t setPWM(uint16_t fd, uint8_t channel, uint16_t on, uint16_t off){
//	printf("SetPWM:%d",off);
	wiringPiI2CWriteReg8(fd, LED0_ON_L+4*channel, on & 0xFF);
	wiringPiI2CWriteReg8(fd, LED0_ON_H+4*channel, on >> 8);
	wiringPiI2CWriteReg8(fd, LED0_OFF_L+4*channel, off & 0xFF);
	wiringPiI2CWriteReg8(fd, LED0_OFF_H+4*channel, off >> 8);
	return 0;
}


uint8_t setAllPWM(uint16_t fd, uint16_t on, uint16_t off){
	wiringPiI2CWriteReg8(fd, ALL_LED_ON_L, on & 0xFF);
	wiringPiI2CWriteReg8(fd, ALL_LED_ON_H, on >> 8);
	wiringPiI2CWriteReg8(fd, ALL_LED_OFF_L, off & 0xFF);
	wiringPiI2CWriteReg8(fd, ALL_LED_OFF_H, off >> 8);
	return 0;
}


uint8_t sleepOn(uint16_t fd){
	uint8_t oldmode = wiringPiI2CReadReg8(fd,MODE1);
	uint8_t newmode = (oldmode & 0x7F) | 0x10;
	wiringPiI2CWriteReg8(fd,MODE1, newmode);
	return 0;
}
