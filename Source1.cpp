// eeprom needs to store
	// calibration data (done once)
	// cm/in state (linked to LED)
	// group number	

// if switch pressed (isr module)
		// set to rising edge (runs when button pushed)
		// assign priority 0
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

#include "project.h" // not sure if needed, included in all other modules
//#include <stdio.h> // not sure if needed
#include <stdlib.h>
#include <time.h> // needed for timed display

int compare(const void* a, const void* b) {
	// needed to determine median of measurements
	return (*(int*)a - *(int*)b);
}


CY_ISR(distanceMeter) {
	float readings[7] = { 0 };
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

	// BEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEP

	for (int i = 0; i < sizeof(readings) / sizeof(readings[0]); i++) {
		readings[i] = ;//PUT RETURN VALUE HERE
	}
	qsort(readings, (sizeof(readings) / sizeof(readings[0])), sizeof(float), compare);
	dist = readings[2]; // take median

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

	// read from eeprom

	isr_1_StartEx(distanceMeter);

	// standby sequence
		// segDP flashing at 1Hz
	for (;;) {
		segDP = ~segDP;
		CyDelay(500);
	}
}

//------------------------------------------------------------------------------------------------------------------------