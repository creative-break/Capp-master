#pragma once

extern int SaveToFile(char* buffer, int lenght, char fileName[]);

extern int LoadFile(char InputFileName[], char*** LoadedFileArray, int* AmountOfStrings);