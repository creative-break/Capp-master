#include <stdio.h>
#include <stdlib.h>
#include "ReturnErrors.h"
#include "FileHandler.h"

int FindWidthAndLengthOfFile(char _InputFileName[], int* Width, int* Length);
int LoadFileToStringArray(char* _LoadedFile[], char _InputFileName[], int _Width);

int SaveToFile(char* buffer, int lenght, char fileName[]) {
	int i = 0;
	FILE* outputFile;

	outputFile = fopen(fileName, "w");
	if (outputFile == NULL) {
		return CANNOT_SAVE_TO_FILE;
	}
	else {
		for (i = 0; i < lenght; i++) {
			fputc(buffer[i], outputFile);
		}
	}

	fclose(outputFile);

	return OK;
}

int LoadFile(char InputFileName[], char*** LoadedFileArray, int* AmountOfStrings) {
	int Width = 0, Height = 0, Index = 0;
	char** LoadedFile;

	if (FindWidthAndLengthOfFile(InputFileName, &Width, &Height) == CANNOT_OPEN_FILE) {
		return CANNOT_OPEN_FILE;
	}

	LoadedFile = calloc(Height, sizeof(char**));
	if (LoadedFile != NULL) {
		for (int i = 0; i < Height; i++) {
			LoadedFile[i] = calloc(Width, sizeof(char*));
		}
	}

	if (LoadedFile != NULL){
		LoadFileToStringArray(LoadedFile, InputFileName, Width);
	}

	*LoadedFileArray = LoadedFile;
	*AmountOfStrings = Height;

	return OK;
}

int FindWidthAndLengthOfFile(char _InputFileName[], int* Width, int* Length) {
	FILE* fp;
	char Buffer[128];
	int CurrentStringLength = 0, CurrentFileLength = 0;

	fopen_s(&fp, _InputFileName, "r");

	if (fp == NULL) {
		return CANNOT_OPEN_FILE;
	}

	while (!feof(fp)) {
		fgets(Buffer, 128, fp);
		CurrentStringLength = strlen(Buffer);

		if (CurrentStringLength > * Width) {
			*Width = CurrentStringLength;
		}

		CurrentFileLength++;
	}
	*Length = CurrentFileLength + 1;

	fclose(fp);

	return OK;
}

int LoadFileToStringArray(char* _LoadedFile[], char _InputFileName[], int _Width) {
	FILE* fp;
	int Index = 0;

	fopen_s(&fp, _InputFileName, "r");

	if (fp == NULL) {
		return CANNOT_OPEN_FILE;
	}

	while (!feof(fp)) {
		fgets(_LoadedFile[Index], _Width, fp);

		/*printf(" %s", _LoadedFile[Index]);*/

		Index++;
	}

	fclose(fp);

	return OK;
}