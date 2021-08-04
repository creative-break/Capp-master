#include "TextStrings.h"


char* stringArray[TOTAL_STRINGS ] = {
	"Entered value was not a number",
	"Requested string could not be resolved",
	"Invalid selection",
	"Please enter a number to select following:",
	"Select option: \n1. List devices \n2. Get best price \n3. Delete a device \n9. Back",
	"Please enter the name of your device",
	"Please enter the kWh usage of your device",
	"Enter device to use",
	"Device could not be deleted",
	"Device was succesfully deleted",
	"Could not get best price",
	"Device was saved successfully",
	"Select option: \n1. Register new device \n9. Back",
	"Please enter two dates in the following format: 'DD:MM DD:MM' starting with the oldest date",
	"1. Today\n2. Tomorrow\n3. History\n9. Back",
	"1. Register a new device\n2. Select an existing device\n9. Exit to main menu",
	"1. Price\n2. Devices\n3. About\n4. Exit program",
	"Enter run time for the device in minutes",
	"No existing devices found, please use register to create a new devices",
	"Enter device number",
};


char* GetTextString(int value) {

	if (value >= 0 && value < TOTAL_STRINGS) {
		return stringArray[value];
	}
	else {
		return stringArray[UNKNOWN_STRING_REQUEST];
	}
}