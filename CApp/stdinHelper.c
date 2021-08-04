#include <stdio.h>
#include <string.h>
#include "ReturnErrors.h"

#define CLEAR_ATTEMPS 1000

/*
 *  Function:  ClearStdinBuffer
 * --------------------
 *  Summary: Clears the Standard input buffer
 * --------------------
 *  Returns: Error code
 */
int ClearStdinBuffer(void) {
	int attempts = CLEAR_ATTEMPS;
	int returnCode = UNABLE_TO_CLEAR_STDIN;
	int c;

	while (attempts) {
		c = fgetc(stdin);

		if (c == '\n' || c == EOF) {
			returnCode = OK;
			break;
		}

		--attempts;
	}
	return returnCode;
}

int GetIntegerFromStdin(int* Variable) {
	int returnCode = OK;

	if (scanf("%d", Variable) == 0) {
		returnCode = INPUT_WAS_NOT_A_NUMBER;
	}

	ClearStdinBuffer();

	return returnCode;
}

int GetDoubleFromStdin(double* Variable) {
	int returnCode = OK;

	if (scanf("%lf", Variable) == 0) {
		returnCode = INPUT_WAS_NOT_A_NUMBER;
	}

	ClearStdinBuffer();

	return returnCode;
}

int GetStringFromStdin(char *string, int maxLength) {
	int returnCode = UNKNOWN_ERROR;

	scanf_s(" %s", string, maxLength);
	ClearStdinBuffer();

	returnCode = OK;

	return returnCode;
}