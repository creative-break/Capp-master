#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "PriceData.h"
#include "ReturnErrors.h"
#include "FileHandler.h"


int GetHourPrice(int startDay, int startMonth, int endDay, int endMonth, _DateTimePrice** dateTimePrice, int* sizeOfStruct) {
	int count = 0, fileHeight = 0, i = 0, x = 0, errorCode = UNKNOWN_ERROR;
	char** loadedFileArray = NULL;
	char* temp;
	char delim[] = ";-";
	double data[6] = { 0 }; 
	
	if ((errorCode = LoadFile("elspot-prices_2018_hourly_dkk.csv", &loadedFileArray, &fileHeight)) == OK) {
		_DateTimePrice* builder = calloc(fileHeight, sizeof(_DateTimePrice));


		/* Loop though though all lines in the data */
		for (i = 0; i < fileHeight; i++) {
			temp = strtok(loadedFileArray[i], delim); /* Gets char* to first substring */

			for (x = 0; x < 6; x++) { /* Loop though the string to find alle substrings seperated with the specified delimiters ";" and "-" */

				if (temp != NULL) {
					data[x] = atof(temp);
				}
				else { 

					break; /* At the end of the string */
				}

				temp = strtok(NULL, delim); /* Move char* to next substring*/
			}

			/* If price data is within the request timespan then put the data into builder for later use */

			if (data[0] >= startDay && data[1] >= startMonth && data[0] <= endDay && data[1] <= endMonth) {
				builder[count].day = data[0];
				builder[count].month = data[1];
				builder[count].year = data[2];
				builder[count].hourStart = data[3];
				builder[count].hourEnd = data[4];
				builder[count].price = data[5];
				count++;
			}
		}


		*dateTimePrice = calloc(count, sizeof(_DateTimePrice)); /* Allocate memory for the result */

		/* Copy the result from builder to the locating caller has pass with */

		for (i = 0; i < count; i++) {
			(*dateTimePrice + i)->day = builder[i].day;
			(*dateTimePrice + i)->month = builder[i].month;
			(*dateTimePrice + i)->year = builder[i].year;
			(*dateTimePrice + i)->hourStart = builder[i].hourStart;
			(*dateTimePrice + i)->hourEnd = builder[i].hourEnd;
			(*dateTimePrice + i)->price = builder[i].price;
		}
		free(builder);
	}

	*sizeOfStruct = count;
	return errorCode;
}