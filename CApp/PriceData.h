#pragma once

typedef struct _DateTimePrice {
	int day;
	int month;
	int year;
	int hourStart;
	int hourEnd;
	double price;
} _DateTimePrice;

extern int GetHourPrice(int startDay, int startMonth, int endDay, int endMonth, _DateTimePrice** dateTimePrice, int* sizeOfStruct);
