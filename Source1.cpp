// PINS TO HAVE
/*
Buzzer - connected to buzzer pin on keypad
metric - toggle switch for units (in/cm), on keypad
segA -> segG, segDP - writing to seven segment display
*/


// eeprom needs to store
	// calibration data (done once)
	// cm/in state (linked to LED)
	// group number

// if switch pressed (isr module)
		// set to rising edge (runs when button pushed)
		// assign priority 1
	// interrupt

	// beep 200ms
	// take measure
		// 7 measurements
		// median

	// convert to distance
		// check eeprom units state (in, cm)
			// LED ON for cm, OFF for in
		// math, convert


//-----------------------------------------------------------------------------------------------------------------------

#include "project.h" 
#include <stdio.h> // not sure if needed
#include <stdlib.h>
#include <time.h> // needed for timed display
#include <math.h>

#define M 7 // number of measurements

int compare(const void* a, const void* b) {
	// needed to determine median of measurements
	return (*(int*)a - *(int*)b);
}


	if (metric_Read()) { EEPROM_WriteByte(1, 0); }
	else { EEPROM_WriteByte(0, 0); }
}

CY_ISR(distanceMeter) {
	float readings[M] = { 0 };
	float dist;

	int unit[] = { 1000, 100, 10, 1 };
	int broken[4] = { 0 };
	int segA, segB, segC, segD, segE, segF, segG, segDP;
	int indA[] = { 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 };
	int indB[] = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 };
	int indC[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	int indD[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };
	int indE[] = { 0, 1, 0, 1, 1, 1, 0, 1, 0, 0 };
	int indF[] = { 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };
	int indG[] = { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0 };

	time_t start, now;
	double elapsed = 0;

	Buzzer_Write(1);
	CyDelay(200);
	Buzzer_Write(0);
	

	for (int i = 0; i < M; i++) {
		readings[i] = ;//PUT RETURN VALUE HERE
	}
	qsort(readings, M, sizeof(float), compare);
	dist = readings[M/2-1]; // take median

	// CONVERT MEASUREMENT TO DISTANCE

	time(&start);

	while (elapsed < 2) {
		for (int u = 0; u < sizeof(unit) / sizeof(unit[0]); u++) {
			int count = 0;
			while (dist >= unit[u]) {
				dist = dist - unit[u];
				count++;
			}
			// outputs to corresponding segments
			segA = indA[count];
			segB = indB[count];
			segC = indC[count];
			segD = indD[count];
			segE = indE[count];
			segF = indF[count];
			segG = indG[count];

			// accurate to one dp
			if (dist != (int)dist && u == sizeof(unit) / sizeof(unit[0]) - 2) { segDP = 1; }
			else { segDP = 0; }
		}
		elapsed = difftime(time(&now), start);
	}
}

int main(void) {
	CyGlobalIntEnable;
	isr_1_ClearPending; // cancel pending interrupts
	isr_2_ClearPending;

	// read from eeprom
	EEPROM_Start();

	isr_1_StartEx(distanceMeter);

	// standby sequence
		// segDP flashing at 1Hz
	for (;;) {
		segDP = ~segDP;
		CyDelay(500);
	}

	/* calibration
	cal_result = empty array for each calibration value
	distance = 0;

	for (loop through each calibration value)
		calibration(i) = Read Calibration values from EEPROM
	end

	for (loop through each calibration value)
		while(distance < 0.9*calibration(i) OR distance > 1.1*calibration(i))
			calib_volt = read VDAC value
			if calib_volt > 4080
				break
			calib_volt = calib_volt + 100 (mV)
			VDAC write(calib_volt)
			distance = get distance measurement (median)
		cal_result = calib_volt
	for (loop through calibration results)
		cal = cal + cal_result(i)
	cal = cal/6
	VDAC write(cal)

	*/
}

//------------------------------------------------------------------------------------------------------------------------