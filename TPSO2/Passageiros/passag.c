#include <stdio.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.c"
#include "Comunicacao.h";
#include "embarque.h";



int _tmain(int argc,_TCHAR* argv[]) {

	dados_mensagem msg;
	passageiros passag;
	data dados;

	BOOL started = FALSE;
	

	HANDLE hThreadMessageReceiver;
	DWORD threadMessageReceiverId = 0;

	HANDLE hThreadEmbarque;
	DWORD threadEmbarque;

	dados.messageData = &msg;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	if (!isControlRunning()){
		errorGui(TEXT("Não existe um Controlador Aéreo ativo. Inicie um primeiro!!"));
		return FALSE;
	}

	passag.logged = FALSE;

	if (!connectPipes(&passag)) {
		return FALSE;
	}

	passag.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	passag.threadContinue = TRUE;

	hThreadMessageReceiver = CreateThread(NULL, 0, messageReceiver, &passag, 0,
		&threadMessageReceiverId);
	if (hThreadMessageReceiver == NULL) {
		errorGui(TEXT("A criar a thread que recebe mensagens"));
		return FALSE;
	}


	
	_tcscpy_s(dados.messageData->message.infoPassag.name, _countof(dados.messageData->message.infoPassag.name),argv[1]);
	_tcscpy_s(dados.messageData->message.infoPassag.aeroporto_origem, _countof(dados.messageData->message.infoPassag.aeroporto_origem), argv[2]);
	_tcscpy_s(dados.messageData->message.infoPassag.aeroporto_destino, _countof(dados.messageData->message.infoPassag.aeroporto_destino), argv[3]);

	if (argv[4] != NULL) {
		passag.tempo_embarque = _wtoi(argv[4]);
		dados.messageData->message.infoPassag.tempo_embarque = _wtoi(argv[4]);
	}
	else {
		passag.tempo_embarque = 0;
		dados.messageData->message.infoPassag.tempo_embarque = 0;
	}

	passag.embarque = FALSE;
	
	dados.messageData->message.cmd = ENTER;
	writeDataToPipeAsync(passag.hPipeMessage,passag.hEvent, &dados.messageData->message, sizeof(dados.messageData->message));

	Sleep(3000);

	hThreadEmbarque = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)embarque, &passag, 0,&threadEmbarque);
	if (hThreadEmbarque == NULL) {
		errorGui(TEXT("A criar a thread que embarca"));
		return FALSE;
	}

	TCHAR comando[50];

	do {
		_tprintf(TEXT("Comando:"));
		_fgetts(comando, _countof(comando), stdin);
		comando[_tcslen(comando) - 1] = '\0';

		if (passag.readerAlive == FALSE)
		{
			break;
		}
	
	} while (_tcscmp(comando, TEXT("encerra")));

	mensagem message;
	message.infoPassag.id = passag.id;
	message.cmd = PASSAG_DISCONNECT;
	PassagDisconnect(&passag, message);

	CloseHandle(hThreadMessageReceiver);
	CloseHandle(hThreadEmbarque);
}

