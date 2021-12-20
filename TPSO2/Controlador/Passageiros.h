#pragma once

#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"

#ifndef PASSAGEIROS_H
#define PASSAGEIROS_H


DWORD WINAPI managePassageiros(LPVOID lpParam);
DWORD WINAPI manageClient(LPVOID lpParam);
int addClient(data* data, int id, HANDLE hPipeMessag);
int averiguaAeroportos(TCHAR aeroporto_origem[30], TCHAR aeroporto_destino[30], data* data);
int addPassToAirport(int id, TCHAR aeroporto_destino[30], TCHAR aeroporto_origem[30], TCHAR nome[50], int index, data* dados, HANDLE hpipeMsg);
void verificaAvioes(data* dados, mensagem msg);
void embarcaPassageiros(data* dados, mensagem msg, int posAeroporto, int posAviao);
int getAeroportoIndex(data* dados, mensagem msg);
int getPassIndex(data* dados, mensagem msg);
void removePassag(data* dados, mensagem msg);
#endif // !PASSAGEIROS_H

