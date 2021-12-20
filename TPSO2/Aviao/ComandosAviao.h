#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"

#ifndef COMANDOSAVIAO_H
#define COMANDOSAVIAO_H

void proximoDestino(data *dadosAviao);
void destino(data* dados);
void embarcaPassageiros(data* dados);
void enterSystem(data* dados);
void viajar(data* dados);
void disconnectPlane(data* dados);






#endif // !COMANDOSAVIAO_H
