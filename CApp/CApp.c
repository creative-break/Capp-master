/*
* gruppe: a400B
*/

#include <stdio.h>
#include <stdlib.h>
#include "stdinHelper.h"
#include "TextStrings.h"
#include "ReturnErrors.h"
#include "Devices.h"
#include "Prices.h"

enum states {
	SELECTION = 0,/*Must be first!*/
	PRICE = 1,
	DEVICES = 2,
	ABOUT = 3,
	EXIT = 4,
	TODAY = 1,
	TOMORROW = 2,
	HISTORICAL = 3,
	BACK = 9,
};

int main(void) {
	int state = SELECTION, i = 0, run = 1, returnValue = EXIT_FAILURE, errorCode = 0;
	
	printf("*****************************************************\n");
	printf("************Welcome to Energy advisor 1.0************\n");
	printf("*****************************************************\n");
	printf("*****************************************************\n\n");

	while (run) {
		errorCode = OK;

		switch (state) {
			case SELECTION: {

				printf("%s\n%s\n", GetTextString(SELECT_A_NUMBER), GetTextString(MAIN_MENU));

				if ((errorCode = GetIntegerFromStdin(&state)) != OK) {
					state = SELECTION;
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				break;
			}
			case PRICE: {

				if ((errorCode = Price()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				state = SELECTION;
				break;
			}
			case DEVICES: {

				if ((errorCode = Devices()) != OK) {
					printf("%s\n", GetErrorCodeString(errorCode));
				}

				state = SELECTION;
				break;
			}
			case ABOUT: {

				printf("This program was developed by group A400B for first semester group project P1 2019.\n"
					"The project is about Energy price awareness\n"
					"Created by:\n"
					"Martin Lindblom Hansen\n"
					"Rune Bohnstedt\n"
					"Kim Helstrup Pedersen\n"
					"Micklas Skov\n"
					"Claes Mortensen\n"
					"Lars Christensen\n"
					"Frederik L. Jakobsen\n\n");

				state = SELECTION;
				break;
			}
			case EXIT: {

				run = 0;
				returnValue = EXIT_SUCCESS;
				break;
			}
			default: {

				printf("%s\n", GetTextString(INVALID_SELECTION));
				state = SELECTION;
				break;
			}
		}
	}
	return returnValue;
}
