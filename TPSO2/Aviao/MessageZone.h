#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"


#ifndef MESSAGEZONE_H
#define MESSAGEZONE_H



DWORD WINAPI receiveMessagesFromControl(LPVOID lpParam);


#endif