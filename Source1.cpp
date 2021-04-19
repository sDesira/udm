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
		// display 2s

	// back to standby

//-----------------------------------------------------------------------------------------------------------------------

// code for isr module 
CY_ISR(distanceMeter) {

}

int main(void) {
	CyGlobalIntEnable;
	isr_1_ClearPending; // cancel pending interrupts
	// read from eeprom

	isr_1_StartEx(distanceMeter);

	// start standby sequence
		// segDP flashing at 1Hz
	for (;;) {
		segDP = ~segDP;
		CyDelay(500);
	}
}

//------------------------------------------------------------------------------------------------------------------------


// converts decimal value (<10,000) into 7 segment display
// use array to store values by unit and put segment displays in separate loop which iterates for 2s
float dist = 7325.4; // test value, would be calculated distance of object
int unit[] = { 1000, 100, 10, 1 };
int broken[4] = { 0 };
int segA, segB, segC, segD, segE, segF, segG, segDP;
int indA[] = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0};
int indB[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
int indC[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
int indD[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0};
int indE[] = {0, 1, 0, 1, 1, 1, 0, 1, 0, 0};
int indF[] = {0, 1, 1, 1, 0, 0, 0, 1, 0, 0};
int indG[] = {1, 1, 0, 0, 0, 0, 0, 1, 0, 0};

for (int u = 0; u < sizeof(unit)/sizeof(unit[0]); u++) {
	int count = 0;
	while (dist >= unit[u]) {
		dist = dist - unit[u];
		count++;
	}
	segA = indA[count];
	segB = indB[count];
	segC = indC[count];
	segD = indD[count];
	segE = indE[count];
	segF = indF[count];
	segG = indG[count];

	// accurate to one dp
	// used sizeof function to avoid hardcoding
	if (dist != (int)dist  &&  u == sizeof(unit) / sizeof(unit[0]) - 2) { segDP = 1; }
	else { segDP = 0; }
}