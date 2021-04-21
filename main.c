/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "project.h" 
#include <stdio.h> // not sure if needed
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // needed for timed display
#include <math.h>

#define M 7 // number of measurements

int compare(const void* a, const void* b) {
	// needed to determine median of measurements
	return (*(int*)a - *(int*)b);
}

CY_ISR(switchState) {
	// 1 if cm
	// link to LED
    int old = EEPROM_1_ReadByte(0);
    int new = 1 - old;
    
	EEPROM_1_WriteByte(new, 0);
    
    
}

CY_ISR(distanceMeter) {
	float readings[M] = { 0 };
	float dist;

	int unit[] = { 1000, 100, 10, 1 };
	int indA[] = { 0, 1, 0, 0, 1, 0, 0, 0, 0, 0 };
	int indB[] = { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 };
	int indC[] = { 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 };
	int indD[] = { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0 };
	int indE[] = { 0, 1, 0, 1, 1, 1, 0, 1, 0, 0 };
	int indF[] = { 0, 1, 1, 1, 0, 0, 0, 1, 0, 0 };
	int indG[] = { 1, 1, 0, 0, 0, 0, 0, 1, 0, 0 };
    
    int indDIG1[] = {0, 1, 1, 1};
    int indDIG2[] = {1, 0, 1, 1};
    int indDIG3[] = {1, 1, 0, 1};
    int indDIG4[] = {1, 1, 1, 0};
    

	time_t start, now;
	double elapsed = 0;

	buzzer_Write(~buzzer_Read()); // toggles buzzer for 200ms
	CyDelay(200);
	buzzer_Write(~buzzer_Read());
	

	for (int i = 0; i < M; i++) {
		//readings[i] = ;//PUT RETURN VALUE HERE
	}
	qsort(readings, M, sizeof(float), compare);
	dist = readings[M/2-1]; // take median

	// CONVERT MEASUREMENT TO DISTANCE
    dist = 1234;
	time(&start);

	while (elapsed < 2) {
		for (int u = 0; u < 4; u++) {
			int count = 0;
			while (dist >= unit[u]) {
				dist = dist - unit[u];
				count++;
			}
            // turn on correct digit 
            DIG1_Write(indDIG1[u]);
            DIG2_Write(indDIG2[u]);
            DIG3_Write(indDIG3[u]);
            DIG4_Write(indDIG4[u]);
            
			// outputs to corresponding segments
			segA_Write(indA[count]);
			segB_Write(indB[count]);
			segC_Write(indC[count]);
			segD_Write(indD[count]);
			segE_Write(indE[count]);
			segF_Write(indF[count]);
			segG_Write(indG[count]);

			// accurate to one dp
			if (dist != (int)dist && u == sizeof(unit) / sizeof(unit[0]) - 2) { segDP_Write(1); }
			else { segDP_Write(0); }
		}
		elapsed = difftime(time(&now), start);
	}
}

int main(void) {
	CyGlobalIntEnable;
	isr_1_ClearPending(); // cancel pending interrupts
	isr_2_ClearPending();
    
	// read from eeprom
    EEPROM_1_UpdateTemperature(); 
    //CySetFlashEEBuffer();
	EEPROM_1_Start();



	isr_1_StartEx(switchState);
	isr_2_StartEx(distanceMeter);
	

	// standby sequence
	// turn each digit in dsplay on for 1s
    int indDIG1[] = {0, 1, 1, 1};
    int indDIG2[] = {1, 0, 1, 1};
    int indDIG3[] = {1, 1, 0, 1};
    int indDIG4[] = {1, 1, 1, 0};
    
    for(int i = 0; i < 4; i++) {
        
        DIG1_Write(indDIG1[i]);
        DIG2_Write(indDIG2[i]);
        DIG3_Write(indDIG3[i]);
        DIG4_Write(indDIG4[i]);
        
        segA_Write(0);
        segB_Write(0);
        segC_Write(0);
        segD_Write(0);
        segE_Write(0);
        segF_Write(0);
        segG_Write(0);
        
        CyDelay(1000); // hold for 1s
    }
    
    // display group number for 1s
        // read group number from eeprom
    
    DIG1_Write(1); // only outputs to one digit
    DIG2_Write(1);
    DIG3_Write(1);
    DIG4_Write(0);
    
    // display metric status for 1s
    if(EEPROM_1_ReadByte(0) == 1) {        
        segG_Write(1); // F E D A
        segF_Write(0);
        segE_Write(0);
        segD_Write(0);
        segC_Write(1);
        segB_Write(1);
        segA_Write(0);
        segDP_Write(1);
    } else {
        segG_Write(1); // F E D A
        segF_Write(0);
        segE_Write(0);
        segD_Write(1);
        segC_Write(1);
        segB_Write(1);
        segA_Write(1);
        segDP_Write(1);
    }
    
    CyDelay(1000); // display for 1s
    
    // segDP flashing at 1Hz
    segA_Write(1);
    segB_Write(1);
    segC_Write(1);
    segD_Write(1);
    segE_Write(1);
    segF_Write(1);
    segG_Write(1);
    
	for (;;) {
        segDP_Write(~segDP_Read());
		CyDelay(500);
    }
}