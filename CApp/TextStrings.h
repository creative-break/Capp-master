#pragma once


extern char* GetTextString(int value);

/*Added new item here must also be added the char *array in the c file to work.*/
enum Text_Strings {
	/* 0 */ ENTERED_VALUE_WAS_NOT_A_NUMBER,
	/* 1 */ UNKNOWN_STRING_REQUEST,
	/* 2 */ INVALID_SELECTION,
	/* 3 */ SELECT_A_NUMBER,
	/* 4 */ EXISTING_MENU,
	/* 5 */ ENTER_NAME_OF_DEVICE,
	/* 6 */ ENTER_POWER_USAGE_OF_DEVICE,
	/* 7 */ ENTER_WANTED_DEVICE,
	/* 8 */ COULD_NOT_DELETE_DEVICE,
	/* 9 */ SUCCESFULLY_DELETED,
	/* 10 */ NO_DEVICE_TO_GET_PRICE_ON,
	/* 11 */ DEVICE_SAVED_SUCCESSFULLY,
	/* 12 */ SELECT_REGISTER,
	/* 13 */ DATE_INPUT,
	/* 14 */ PRICES_MENU,
	/* 15 */ DEVICE_MENU,
	/* 16 */ MAIN_MENU,
	/* 17 */ GET_RUN_TIME_IN_MINUTES,
	/* 18 */ NO_DEVICES,
	/* 19 */ ENTER_DEVICE_NUMBER,
	/*insert string here*/
	/* 20 */ TOTAL_STRINGS/*MUST BE LAST!!*/
};