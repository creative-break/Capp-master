#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "stdinHelper.h"
#include "Devices.h"
#include "ReturnErrors.h"
#include "TextStrings.h"
#include "FileHandler.h"
#include "PriceData.h"

#define INT_MAX_CHAR 10
#define ARRAY_SIZE 10
#define DEVICE_FILE "devices.ini"
#define MAX_DEVICE_NAME 50

enum state {
	SELECTION = 0,
	LISTE_ALL_DEVICES = 1,
	CREATE_DEVICE = 1,
	GET_BEST_PRICE = 2,
	DELETE_A_DEVICE = 3,
	REGISTER = 1,
	EXISTING = 2,
	BACK = 9,
};

typedef struct Devices {
	char deviceName[MAX_DEVICE_NAME];
	double kwh;
} devices;

typedef struct _BestTime {
	char timeStamp[30];
	double price;
}BestTime;

devices* existingDevices = NULL;
int deviceCounter = 0;

int ExistingDevices(void);
int ChooseDevice(int* selectedDevice);
int RegisterDevice(void);
int SaveCfg(devices deviceList[], int deviceCount);
int LoadCfg(devices** deviceList, int* counter);
int GetBestTime(BestTime* bestTimeToStart, int runTimeInMinutes);

/*
 * Function: Devices
 * -------------------
 * Menu that handles the device menu selection 
 *
 * parameters: none
 *
 * returns: error code reflecting the execution status
 */
int Devices(void) {
	int returnCode = UNKNOWN_ERROR, run = 1, state = 0, errorCode = OK;
	
	LoadCfg(&existingDevices, &deviceCounter);

	while (run) {
		errorCode = OK;
		switch (state) {
			case SELECTION: {

				printf("%s\n%s\n", GetTextString(SELECT_A_NUMBER), GetTextString(DEVICE_MENU));
				if ((errorCode = GetIntegerFromStdin(&state)) != OK) {
					state = SELECTION;
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				break;
			}
			case REGISTER: {

				if ((errorCode = RegisterDevice()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				state = SELECTION;
				break;
			}
			case EXISTING: {

				if (deviceCounter) {
					if (ExistingDevices() != OK) {
						printf("%s\n", GetErrorCodeString(EXISTING_FAILED));
					}	
				} 
				else { 
					printf("%s\n", GetTextString(NO_DEVICES)); 
				}

				state = SELECTION;
				break;
			}
			case BACK: {

				run = 0;
				returnCode = OK;
				break;
			}
			default: {

				state = SELECTION;
				printf("%s\n", GetTextString(INVALID_SELECTION));
				break;
			}
		}
	}
	free(existingDevices);
	return returnCode;
}

/*
* Function: Existing
* --------------------------
* Menu that handles existing devices
*
* parameters: none
*
* returns: error code reflecting the execution status
*/
int ExistingDevices(void) {
	int state = SELECTION, keepAlive = 1, i = 0, selectedDevice = 0, minToRun = 0, returnCode = OK;

	while (keepAlive) {

		switch (state) {
			case SELECTION: {

				printf("%s\n", GetTextString(EXISTING_MENU));
				if ((returnCode = GetIntegerFromStdin(&state)) != OK) {

				}

				break;
			}
			case LISTE_ALL_DEVICES: {

				for (i = 0; i < deviceCounter; i++) {
					printf("|Device nr: %d | Name: %s | kWh: %2.2lf|\n", i + 1, existingDevices[i].deviceName, existingDevices[i].kwh);
				}

				state = SELECTION;
				break;
			}
			case GET_BEST_PRICE: {
					
				if ((returnCode = ChooseDevice(&selectedDevice)) == OK) {
					printf("%s\n", GetTextString(GET_RUN_TIME_IN_MINUTES));
					if ((returnCode = GetIntegerFromStdin(&minToRun)) == OK) {
						BestTime bt;
						if ((returnCode = GetBestTime(&bt, minToRun)) == OK) {
							printf("|Start: %s | price kWh DKK: %2.3lf | cost: %2.3lf DKK|\n", bt.timeStamp, bt.price, existingDevices[selectedDevice - 1].kwh * bt.price);
						}
					}
				}

				if (returnCode != OK) {
					printf("%s\n", GetErrorCodeString(returnCode));
				}

				state = SELECTION;
				break;
			}
			case DELETE_A_DEVICE: {
				if ((returnCode = ChooseDevice(&selectedDevice)) == OK) {

					for (i = selectedDevice; i < deviceCounter; i++) {
						existingDevices[i - 1] = existingDevices[i];
					}
					deviceCounter--;

					if (deviceCounter == 0) {
						free(existingDevices);
					}
					else {
						devices* d = realloc(existingDevices, deviceCounter * sizeof(devices));
						if (d != NULL) {
							existingDevices = d;
							printf("%s\n", GetTextString(SUCCESFULLY_DELETED));
						}
						else {
							returnCode = ALLOCATING_MEMORY_FAILED;
						}
					}

					SaveCfg(existingDevices, deviceCounter);
				}

				if (returnCode != OK) {
					printf("%s\n", GetTextString(returnCode));
				}
	
				state = BACK;
				break;
			}
			case BACK: {

				keepAlive = 0;
				break;
			}
			default: {

				printf("%s\n", GetTextString(INVALID_SELECTION));
				state = SELECTION;
				break;
			}
		}
	}
	return returnCode;
}

/*
* Function: ChooseDevice
* --------------------------
* Takes user input and and checks if device exists
*
* parameters: selectedDevice
*
* returns: error code reflecting user input
*/
int ChooseDevice(int* selectedDevice) {
	int returnCode = UNKNOWN_ERROR;

	printf("%s\n", GetTextString(ENTER_DEVICE_NUMBER));
	returnCode = GetIntegerFromStdin(selectedDevice);
	if (*selectedDevice < 0 || *selectedDevice > deviceCounter) {
		returnCode = SELECTED_OUT_OF_BOUND;		
	}

	return returnCode;
}

/*
* Function: RegisterDevice
* --------------------------
* Menu that handles device registration
* 
* parameters: none 
*
* returns: error code reflecting the execution status
*/
int RegisterDevice(void) {
	int state = 0, keepAlive = 1, errorCode = OK;
	devices newdevice;

	while (keepAlive) {

		switch (state) {
			case SELECTION: {

				printf("%s\n", GetTextString(SELECT_REGISTER));

				if (scanf("%d", &state) == 0) {
					state = SELECTION;
					ClearStdinBuffer();
					printf("%s\n", GetTextString(ENTERED_VALUE_WAS_NOT_A_NUMBER));
				}

				break;
			}
			case CREATE_DEVICE: {

				printf("%s\n", GetTextString(ENTER_NAME_OF_DEVICE));

				if ((errorCode = GetStringFromStdin(newdevice.deviceName, MAX_DEVICE_NAME)) == OK) {
					printf("%s\n", GetTextString(ENTER_POWER_USAGE_OF_DEVICE));

					if ((errorCode = GetDoubleFromStdin(&newdevice.kwh)) != OK) {
						printf("%s\n", GetErrorCodeString(errorCode));

						state = SELECTION;
					}
					else {
						deviceCounter++;

						devices* d =  realloc(existingDevices, deviceCounter * sizeof(devices));
						if (d != NULL) {
							existingDevices = d;
							strcpy(existingDevices[deviceCounter - 1].deviceName, newdevice.deviceName);
							existingDevices[deviceCounter - 1].kwh = newdevice.kwh;
							SaveCfg(existingDevices, deviceCounter);
							printf("%s\n", GetTextString(DEVICE_SAVED_SUCCESSFULLY));
						}
						else {
							printf("%s\n", GetErrorCodeString(ALLOCATING_MEMORY_FAILED));
						}
							
						state = SELECTION;	
					}
				}
				else {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				break;
			}
			case BACK: {

				keepAlive = 0;
				break;
			}
			default: {

				state = SELECTION;
				printf("%s\n", GetTextString(INVALID_SELECTION));
				break;
			}
		}
	}
	return 0;
}

int SaveCfg(devices deviceList[], int deviceCount) {
	char* device;
	char ckwh[MAX_DEVICE_NAME];
	int returnCode = OK;

	if (deviceCount > 0) {
		device = calloc(deviceCount, MAX_DEVICE_NAME + INT_MAX_CHAR);

		if (device != NULL) {
			for (int i = 0; i < deviceCount; i++) {
				printf("device :%s %lf\n", deviceList[i].deviceName, deviceList[i].kwh);
				strcat(device, deviceList[i].deviceName);
				strcat(device, ";");
				sprintf(ckwh, "%lf", deviceList[i].kwh);
				strcat(device, ckwh);
				strcat(device, "\n");

			}
			/*printf("string: %s\n", device);*/
			returnCode = SaveToFile(device, strlen(device), DEVICE_FILE);
		}
		else {
			returnCode = UNKNOWN_ERROR;
		}
	}
	else {
		device = calloc(1, MAX_DEVICE_NAME + INT_MAX_CHAR);
		strcpy(device, "\0");
		returnCode = SaveToFile(device, strlen(device), DEVICE_FILE);
	}

	free(device);
	return returnCode;
}

int LoadCfg(devices** deviceList, int* counter) {
	int returnCode = UNKNOWN_ERROR, fileHeight = 0, i = 0, cnt = 0;
	char** loadedFileArray = NULL;
	char delim[] = ";";
	char* temp;

	if ((returnCode = LoadFile(DEVICE_FILE, &loadedFileArray, &fileHeight)) == OK) {
		*deviceList = calloc(fileHeight, sizeof(devices));

		for (i = 0; i < fileHeight; i++) {
			if (strlen(loadedFileArray[i]) > 1){
				temp = strtok(loadedFileArray[i], delim);

				if (temp != NULL) {
					strcpy((*deviceList + i)->deviceName, temp);
						temp = strtok(NULL, delim);
						if (temp != NULL) {
							(*deviceList + i)->kwh = atof(temp);
							cnt++;
						}
						else {
							returnCode = UNABLE_TO_DECODE_DEVICE_CONFIG;
						}
				}
				else {
					returnCode = UNABLE_TO_DECODE_DEVICE_CONFIG;
				}
			}
		}
	}

	*counter = cnt;
	return returnCode;
}

int GetBestTime(BestTime* bestTimeToStart, int runTimeInMinutes){
	int errorCode = UNKNOWN_ERROR, i = 0, minLeft = 0, hourOffset = 0, startMin = 0, hourEnd = 0;
	int foundStartHour = 0, foundStartMin = 0, minCalc = 0;
	double price = 0, newPrice = 0;

	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	_DateTimePrice* hourPrices = NULL;
	size_t structSize = 0;

	if ((errorCode = GetHourPrice(tm.tm_mday, tm.tm_mon+1, tm.tm_mday + 1, tm.tm_mon+1, &hourPrices, &structSize)) == OK) {
		for (i = tm.tm_hour; i < structSize; i++) {
			hourPrices[i].price = hourPrices[i].price / 60000; /*convert mwh to kwh then hour to minut*/
		}

		startMin = (tm.tm_min + 5) % 60; /*find start minute*/
		hourOffset = tm.tm_min + 5 >= 60 ? 1 : 0; // hour roll over

		for (i = 0; i < runTimeInMinutes; i++) {
			/*printf("Hour: %2d min: %2d\n", tm.tm_hour + hourOffset + ((i + startMin) / 60), (i + startMin) % 60);
			printf("Price: %2.8lf\n", hourPrices[tm.tm_hour + hourOffset + ((i + startMin) / 60)].price);*/
			price += hourPrices[tm.tm_hour + hourOffset + ((i + startMin) / 60)].price;
		}

		//printf("Price: %2.5lf\n\n\n", price);
		hourEnd = tm.tm_hour + hourOffset + runTimeInMinutes / 60;
		minLeft = 2880 - ((tm.tm_hour + hourOffset) * 60 + runTimeInMinutes);
		newPrice = price;
		minCalc = startMin;
		for (i = 0; i < minLeft; i++) {
			minCalc =+ i;
			/*printf("Start: %2d:%2d - end: %2d:%2d\n", tm.tm_hour + hourOffset + ((minCalc) / 60), minCalc % 60, hourEnd + (minCalc / 60), (minCalc + runTimeInMinutes) % 60);
			printf("Subtract: %2.7lf, add:  %2.7lf, price: %2.7lf\n", hourPrices[tm.tm_hour + (minCalc / 60)].price, hourPrices[hourOffset + (minCalc / 60)].price, price);*/

			newPrice -= hourPrices[tm.tm_hour + hourOffset + (minCalc / 60)].price;
			newPrice += hourPrices[hourEnd + (minCalc  / 60)].price;

			if (newPrice < price) {
				foundStartMin = ((minCalc + 1) % 60);
				foundStartHour = tm.tm_hour + hourOffset + ((minCalc + 1) / 60);
				price = newPrice;
				//printf("time: %2d:%d -  %2.4lf\n", foundStartHour, foundStartMin, price);
			}
		}

		if (foundStartHour > 23) {
			foundStartHour -= 24;
			if (tm.tm_mday + 1 > 31) {
				if (tm.tm_mon + 1 > 11) {
					tm.tm_mon = 1;
					tm.tm_mday = 1;
					tm.tm_year++;
				}
				else {
					tm.tm_mon++;
					tm.tm_mday = 1;
				}
			}
			else {
				tm.tm_mday++;
			}
		}
		//printf("price: %2.7lf\n", price);
		bestTimeToStart->price = price;
		sprintf(bestTimeToStart->timeStamp, "%2d-%2d-%4d : %2d:%2d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, foundStartHour, foundStartMin);
		free(hourPrices);
	}
	return errorCode;
}