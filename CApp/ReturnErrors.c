#include <stdlib.h>
#include "ReturnErrors.h"


char* errorStrings[TOTAL_RETURN_VALUES] = {
	"OK",
	"Unable to clear stdin buffer",
	"Unkowned error",
	"Entered value was not a number",
	"Cannot save the file",
	"Existing failed",
    "File not found",
	"Could not convert string to struct",
	"Allocating memory failed",
	"Selected number out of bound",
};

char* GetErrorCodeString(int value) {

	if (value >= 0 && value < TOTAL_RETURN_VALUES) {
		return errorStrings[value];
	}
	else {
		return errorStrings[UNKNOWN_ERROR];
	}
}