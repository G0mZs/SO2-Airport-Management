#pragma once

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"

#ifndef EMBARQUE_H
#define EMBARQUE_h


DWORD WINAPI embarque(LPVOID lpParam);
void embarca(passageiros* passag);
void PassagDisconnect(passageiros* passag);





#endif