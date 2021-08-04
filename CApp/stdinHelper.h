#pragma once


/*
 *  Function:  ClearStdinBuffer
 * --------------------
 *  Summary: Clears the Standard input buffer
 * --------------------
 *  Returns: Error code
 */
extern int ClearStdinBuffer(void);

extern int GetIntegerFromStdin(int* Variable);

extern int GetStringFromStdin(char* string, int maxLength);

extern int GetDoubleFromStdin(double* Variable);