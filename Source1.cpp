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
int value = 3254; // test value, would be calculated distance of object
int unit[4] = { 1000, 100, 10, 1 };
int broken[4] = { 0 };

for (int u = 0; u < 4; u++) {
	check++;
	int count = 0;
	while (value >= unit[u]) {
		value = value - unit[u];
		count++;
	}
	broken[u] = count;
}