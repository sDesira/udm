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

int compare(const void* a, const void* b) {
	// needed to determine median of measurements
	return (*(int*)a - *(int*)b);
}

CY_ISR(switchStates) {
	// 1 if cm
	// link to LED
	metric_Write = (~metric_Read());
	EEPROM_WriteByte(metric, 0);
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
			segA_Write = indA[count];
			segB_Write = indB[count];
			segC_Write = indC[count];
			segD_Write = indD[count];
			segE_Write = indE[count];
			segF_Write = indF[count];
			segG_Write = indG[count];

			// accurate to one dp
			if (dist != (int)dist && u == sizeof(unit) / sizeof(unit[0]) - 2) { segDP = 1; }
			else { segDP_Write = 0; }
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

	metric_Write = EEPROM_ReadByte(0);

	isr_1_StartEx(switchState);
	isr_2_StartEx(distanceMeter);

	

	// standby sequence
		// segDP flashing at 1Hz
	for (;;) {
		segDP_Write = (~segDP_Read()); //!!
		CyDelay(500);
	}
}

//------------------------------------------------------------------------------------------------------------------------