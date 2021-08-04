#include <stdlib.h>
#include <time.h>
#include "Prices.h"
#include "ReturnErrors.h"
#include "TextStrings.h"
#include "ReturnErrors.h"
#include "FileHandler.h"
#include "PriceData.h"

enum states {
	SELECTION = 0,/*Must be first!*/
	TODAY = 1,
	TOMORROW = 2,
	HISTORICAL = 3,
	BACK = 9,
};

void PrintOutPriceData(_DateTimePrice* prices, int cnt);
int Today(void);
int Tomorrow(void);
int Historical(void);

int Price(void) {

	int returnCode = UNKNOWN_ERROR, run = 1, state = 0, errorCode = OK;

	while (run) {
		errorCode = OK;
		switch (state) {
			case SELECTION: {

				printf("%s\n%s\n", GetTextString(SELECT_A_NUMBER), GetTextString(PRICES_MENU));
				if ((errorCode = GetIntegerFromStdin(&state)) != OK) {
					state = SELECTION;
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				break;
			}
			case TODAY: {

				if ((errorCode = Today()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				state = SELECTION;
				break;
			}
			case TOMORROW: {

				if ((errorCode = Tomorrow()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				state = SELECTION;
				break;
			}
			case HISTORICAL: {

				if ((errorCode = Historical()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}
				state = SELECTION;
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

	return returnCode;
}

int Today(void) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	_DateTimePrice* callPrices = NULL;
	size_t structSize = 0;
	int errorCode = UNKNOWN_ERROR;

	if ((errorCode = GetHourPrice(tm.tm_mday, tm.tm_mon + 1, tm.tm_mday, tm.tm_mon + 1, &callPrices, &structSize)) == OK) {
		PrintOutPriceData(callPrices, structSize);
	}

	free(callPrices);
	return errorCode;
}

int Tomorrow(void) {
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	_DateTimePrice* callPrices = NULL;
	size_t structSize = 0;
	int errorCode = UNKNOWN_ERROR;

	if ((errorCode = GetHourPrice(tm.tm_mday + 1, tm.tm_mon + 1, tm.tm_mday + 1, tm.tm_mon + 1, &callPrices, &structSize)) == OK) {
		PrintOutPriceData(callPrices, structSize);
	}

	free(callPrices);
	return errorCode;
}

int Historical(void) {
	int startDay = 0, startMonth = 0, endDay = 0, endMonth = 0;
	_DateTimePrice* callPrices = NULL;
	size_t structSize = 0;
	int errorCode = UNKNOWN_ERROR;

	printf("%s\n", GetTextString(DATE_INPUT));
	scanf_s("%d:%d %d:%d", &startDay, &startMonth, &endDay, &endMonth);

	if ((errorCode = GetHourPrice(startDay, startMonth, endDay, endMonth, &callPrices, &structSize)) == OK) {
		PrintOutPriceData(callPrices, structSize);
	}

	free(callPrices);
	return errorCode;
}

void PrintOutPriceData(_DateTimePrice* prices, int cnt) {
	int i;

	for (i = 0; i < cnt; i++) {
		printf("|Date: %2d-%2d-%4d | Hour: %2d-%2d | Price: %0.2lf DKK / KwH|\n", prices[i].day, prices[i].month, prices[i].year, prices[i].hourStart, prices[i].hourEnd, prices[i].price/1000);
	}
}