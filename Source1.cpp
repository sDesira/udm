// test
// test2 by Nick


// calibrate
	// save values to eeprom


// if switch pressed
	// beep 200ms
	// take measure
		// 7 measurements
		// median

	// convert to distance
		// check eeprom state
		// math, convert
		// display 2s

	// decimal to 7 ssegment display
// turn off


// this breaks value into array corresponding to units - first step of converting to 7 seg display. 
float dist = 325.4; // test value, would be calculated distance of object
int unit[4] = { 1000, 100, 10, 1 };
int segA, segB, segC, segD, segE, segF, segG, segDP;
int indA[] = {0, 1, 0, 0, 1, 0, 0, 0, 0, 0};
int indB[] = {0, 0, 0, 0, 0, 1, 1, 0, 0, 0};
int indC[] = {0, 0, 1, 0, 0, 0, 0, 0, 0, 0};
int indD[] = {0, 1, 0, 0, 1, 0, 0, 1, 0, 0};
int indE[] = {0, 1, 0, 1, 1, 1, 0, 1, 0, 0};
int indF[] = {0, 1, 1, 1, 0, 0, 0, 1, 0, 0};
int indG[] = {1, 1, 0, 0, 0, 0, 0, 1, 0, 0};

for (int u = 0; u < (sizeof(unit)/sizeof(unit[0]); u++) {
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
	if ((value != (int)value) && (u == (sizeof(unit) / sizeof(unit[0]) - 2))) { segDP = 1; }
	else { segDP = 0; }
}