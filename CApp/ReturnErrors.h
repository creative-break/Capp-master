#pragma once

/*Added new item here must also be added the char *array in the c file to work.*/
enum ReturnValues {
	/* 0 */ OK,
	/* 1 */ UNABLE_TO_CLEAR_STDIN,
	/* 2 */ UNKNOWN_ERROR,
	/* 3 */ INPUT_WAS_NOT_A_NUMBER,
	/* 4 */ CANNOT_SAVE_TO_FILE,
	/* 5 */ EXISTING_FAILED,
	/* 6 */ CANNOT_OPEN_FILE,
	/* 7 */ UNABLE_TO_DECODE_DEVICE_CONFIG,
	/* 8 */ ALLOCATING_MEMORY_FAILED,
	/* 9 */ SELECTED_OUT_OF_BOUND,
	/*insert errors here*/
	/* 10 */ TOTAL_RETURN_VALUES /*MUST BE LAST*/
};

extern char* GetErrorCodeString(int value);