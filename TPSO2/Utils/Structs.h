#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>

#ifndef STRUCTS_H
#define STRUCTS_H
#define MAX_PASSAGEIROS 10//Alterar esta e proxima linha
#define MAX_AVIOES 10
#define MAX_AEROPORTOS 5

typedef enum {
	ENTER,
	ENTERFAILED,
	ENTERSUCESS,
	PLANESUSPEND,
	DESTINO,
	DESTINYFAILED,
	DESTINYSUCCESS,
	FLY,
	UPDATECOORDS,
	UPDATESUCESS,
	AVOIDCOLISIONS,
	COORDSPASS,
	ARRIVAL,
	ARRIVALPASS,
	ARRIVALSUCESS,
	NOCOMMANDS,
	EMBARCA,
	EMBARQUE,
	EMBARQUESUCESS,
	PASSAG_DISCONNECT,
	CONTROL_DISCONNECT,
	PLANE_DISCONNECT,
	DISCONNECT,
	VOAR,
	EXIT
} Command;

typedef struct PASSAGEIROS{

	int id;
	TCHAR name[50];
	int tempo_embarque;
	HANDLE hPipeGame;
	HANDLE hPipeMessage;
	HANDLE hEvent;
	BOOL threadContinue;
	BOOL readerAlive;
	BOOL logged;
	TCHAR aeroporto_origem[30];
	TCHAR aeroporto_destino[30];
	int empty;
	BOOL embarque;



}passageiros;


typedef struct AVIOES {

	int id;
	int capacidade;
	int posicaoX;
	int posicaoY;
	int proximoX;
	int proximoY;
	int velocidade;
	TCHAR aeroporto_inicial[30];
	TCHAR aeroporto_destino[30];
	int posicaoXAeroporto_destino;
	int posicaoYAeroporto_destino;
	passageiros passagAviao[20];
	int empty;
	BOOL deslocamento;
	BOOL comandos;
	HANDLE evento;
	HWND hWnd;

}avioes;

typedef struct AEROPORTO {

	TCHAR nome[30];
	int posicaoX;
	int posicaoY;
	avioes arrayAvioes[MAX_AVIOES];
	BOOL vazio;
	HWND hWnd;
	passageiros Passag[MAX_PASSAGEIROS];

}aeroporto;

typedef struct MENSAGEM {

	//Comandos cmd
	TCHAR text[80];
	avioes infoAvioes;
	passageiros infoPassag;
	Command cmd;
	TCHAR evento[50];


}mensagem;

typedef struct DADOSMENSAGEM{

	mensagem* sharedMessage;
	mensagem message;
	HANDLE hMapFile;
	HANDLE hMutex;
	BOOL STOP;
	HANDLE controlMessageUpdateEvent;
	HANDLE aviaoMessageUpdateEvent;

}dados_mensagem;

typedef struct DATA {

	int max_aeroportos;
	int max_avioes;
	dados_mensagem *messageData;
	aeroporto aeroportos[MAX_AEROPORTOS];
	int capacidade_aeroporto;
	avioes arrayAvioes[MAX_AVIOES];
	BOOL suspenso;
	HANDLE tmpPipeMessage;
	HANDLE tmpPipeGame;
	HANDLE hEvent;
	passageiros pass[MAX_PASSAGEIROS];
	HWND hWnd;
	HINSTANCE hInst;

}data;


#endif
