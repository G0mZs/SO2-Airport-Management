#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"
#include "Passageiros.h"

#ifndef AEROPORTOS_H
#define AEROPORTOS_H

void procuraDestino(data* dados, mensagem message);
//void embarca(data* dados, mensagem message);
int procuraPessoa(aeroporto array, mensagem message);
void listaDados(data* dados);
void listaAeroporto(data* dados);
void criaAeroporto(data* dados);
void cria(data* data, TCHAR nome[50], int x, int y);
void insereAviao(data* data, mensagem message, int pos);
int procuraAeroportoInicial(data* data, mensagem message);
void inserirFalhado(data* data, mensagem message);
void inserirDestino(data* dados, mensagem message);
int averiguaCoordenadas(data* dados, int x, int y);
void suspendeAvioes(data* dados, mensagem message);
void voa(data* data, mensagem message,int pos);
void updateCoordenadas(data* dados, mensagem message);
void chegada(data* data, mensagem message, int pos);
int procuraAeroportoDestino(data* dados, mensagem message);
void addPassToFlying(int id, TCHAR nome[50], TCHAR destino[30], TCHAR origem[30], int index);
void disconnectAll(data* dados);
void removePlane(data* dados, mensagem msg);
void embarcaPass(data* data, mensagem msg, int id, TCHAR nome[50], TCHAR destino[50], TCHAR origem[50], HANDLE pipeM, int posAeroporto, int posAviao);
int getIndexAviao(data* dados, mensagem msg);

#endif
