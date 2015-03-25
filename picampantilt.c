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
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <math.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include "picampantilt.h"
#include "pca9685.h"


int main(int argc, char *argv[]){
	int i;
	uint16_t fd = 0;
	fd = wiringPiI2CSetup(PCA9685_ADDR);
	setUpDevice(fd);

	if (argc > 1) {
		for (i=1; i<argc; i++) {
			if ((strcmp(argv[i],"-m")==0)||(strcmp(argv[i],"--move")==0)){
				pan(fd, atoi(argv[i+1]));
				tilt(fd,atoi(argv[i+2]));
				i+=2;
			} else if ((strcmp(argv[i],"-p")==0)||(strcmp(argv[i],"--pan")==0)){
				pan(fd, atoi(argv[i+1]));
				i+=1;
			} else if ((strcmp(argv[i],"-t")==0)||(strcmp(argv[i],"--tilt")==0)){
				tilt(fd,atoi(argv[i+1]));
				i+=1;
			} else if ((strcmp(argv[i],"-s")==0)||(strcmp(argv[i],"--snapshot")==0)){
				cameraTask("capture","south.jpg");
			}
		}
	}else{
		printf("Test");
	}
// Pause to allow servos to move to position
	secondSleep(1);
// Stop the pca9685 driving the servos
	sleepOn(fd);
	return 0;
}


void cameraTask(char *task, char *argv) {
//void cameraTask(int task) {
//	int x;
	printf("Test %s %s\n",task,argv);

}


void secondSleep(float seconds) {
	usleep((long) (seconds*1000000.0));
}


void fileCopy(char *sourceFileFull, char *targetFileFull) {
	char buffer[100];
	char bufferBash[100];
	char fileFull[25];
	char *fileName;
	char *fileExt;  
	time_t now;
	struct tm * timeinfo;

// write storage by date directories
	now = time (NULL);
	time ( &now);
	timeinfo = localtime ( &now);
	int written = sprintf(buffer, "mkdir -p %s", IMAGEPATH);
	strftime(buffer+written, 100-written, "%Y", timeinfo);
	system(buffer);
	strftime(buffer+written, 100-written, "%Y/%Y-%m", timeinfo);
	system(buffer);

// copy source to IMAGEPATH with new target filename
	strcpy(buffer, "");
	snprintf(buffer,sizeof buffer, "cp %s %s%s",sourceFileFull,IMAGEPATH,targetFileFull);
	system(buffer);
	strcpy(fileFull,targetFileFull);
	fileName = strtok(fileFull,".");
	fileExt = strtok(NULL,".");

// copy source to history with timestamped filename
	strcpy(buffer, "");
	strftime(buffer,sizeof buffer, "cp %%s %%s%Y/%Y-%m/%%s-%Y%m%d-%H%M%S.%%s", timeinfo);
	snprintf(bufferBash,sizeof bufferBash, buffer,sourceFileFull,IMAGEPATH,fileName,fileExt);
	system(bufferBash);

   return;
}


char * fileDateStamp() {
	static char buf[150];
   time_t curtime;
   struct tm *loc_time;

   //Getting current time of system
   curtime = time (NULL);

   // Converting current time to local time
   loc_time = localtime (&curtime);

   strftime (buf, 150, "%Y%m%d%H%M", loc_time);
//	printf("%s \n",buf);
	char *x;
	x = (char*) &buf;
	return x;
}

char *replace_str2(const char *str, const char *old, const char *new)
{
	char *ret, *r;
	const char *p, *q;
	size_t oldlen = strlen(old);
	size_t count, retlen, newlen = strlen(new);
	int samesize = (oldlen == newlen);

	if (!samesize) {
		for (count = 0, p = str; (q = strstr(p, old)) != NULL; p = q + oldlen)
			count++;
		/* This is undefined if p - str > PTRDIFF_MAX */
		retlen = p - str + strlen(p) + count * (newlen - oldlen);
	} else
		retlen = strlen(str);

	if ((ret = malloc(retlen + 1)) == NULL)
		return NULL;

	r = ret, p = str;
	while (1) {
		/* If the old and new strings are different lengths - in other
		 * words we have already iterated through with strstr above,
		 * and thus we know how many times we need to call it - then we
		 * can avoid the final (potentially lengthy) call to strstr,
		 * which we already know is going to return NULL, by
		 * decrementing and checking count.
		 */
		if (!samesize && !count--)
			break;
		/* Otherwise i.e. when the old and new strings are the same
		 * length, and we don't know how many times to call strstr,
		 * we must check for a NULL return here (we check it in any
		 * event, to avoid further conditions, and because there's
		 * no harm done with the check even when the old and new
		 * strings are different lengths).
		 */
		if ((q = strstr(p, old)) == NULL)
			break;
		/* This is undefined if q - p > PTRDIFF_MAX */
		ptrdiff_t l = q - p;
		memcpy(r, p, l);
		r += l;
		memcpy(r, new, newlen);
		r += newlen;
		p = q + oldlen;
	}
	strcpy(r, p);

	return ret;
}
