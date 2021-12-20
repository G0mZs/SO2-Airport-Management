#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"



#ifndef MENSAGENS_H
#define MENSAGENS_H

void recebeComandos(data* data, mensagem message);
DWORD WINAPI receiveMessagesFromAvioes(LPVOID lpParam);






#endif