#include <stdio.h>
#include <string.h>
#include "SO2_TP_DLL_2021.h";
#include "../Utils/Structs.h"
#include "../Utils/Library.c"
#include "MessageZone.h"
#include "aviao.h"
#include "ComandosAviao.h"
#include "Voo.h";



int _tmain(int argc, _TCHAR* argv[]) {

    dados_mensagem messageData;

    data dados;
	TCHAR comando[100];
	const TCHAR aviaofile[50];

    HANDLE hThreadReceiveMessagesFromControl;
    DWORD threadReceiveMessagesFromControl;

	HANDLE hThreadvoo;
	DWORD threadVoo;

    dados.messageData = &messageData;


#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	_stprintf_s(aviaofile, _countof(aviaofile), MESSAGE_AVIAO_UPDATE_EVENT, GetCurrentProcessId());


	if (!isControlRunning()) {
		errorGui(TEXT("Não existe um Controlador Aéreo ativo. Inicie um primeiro!!"));
		return FALSE;
	}


    if (!initMessageZone(&messageData)) {
        error(TEXT("Can't connect message data with control. Exiting..."));
        system("pause");
    }

    messageData.controlMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, MESSAGE_CONTROL_UPDATE_EVENT);
    if (messageData.controlMessageUpdateEvent == NULL) {
        error(TEXT("OpenEvent failed"));
        return FALSE;
    }
	
	messageData.aviaoMessageUpdateEvent = CreateEventW(NULL, FALSE, FALSE, aviaofile);


	if (messageData.aviaoMessageUpdateEvent == NULL) {
		errorGui(TEXT("Erro ao criar evento!!"));
		return FALSE;
	}
	
    hThreadReceiveMessagesFromControl = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receiveMessagesFromControl, &dados, 0, &threadReceiveMessagesFromControl);
    if (hThreadReceiveMessagesFromControl == NULL) {
        error(TEXT("Creating thread to receive data from control"));
        return -1;
    }
	
	_tcscpy_s(messageData.message.text, _countof(messageData.message.text), aviaofile);

	system("cls"); 

	dados.messageData->message.infoAvioes.id = GetCurrentProcessId();
	dados.messageData->message.infoAvioes.capacidade = _wtoi(argv[1]);
	dados.messageData->message.infoAvioes.velocidade = _wtoi(argv[2]);
	_tcscpy_s(dados.messageData->message.infoAvioes.aeroporto_inicial, _countof(dados.messageData->message.infoAvioes.aeroporto_inicial), argv[3]);
	dados.messageData->message.infoAvioes.empty = TRUE;

	enterSystem(&dados);

	_tprintf(TEXT("Capacidade deste Avião: %d \nVelocidade: %d\nAeroporto Inicial: %s \n\n"), dados.messageData->message.infoAvioes.capacidade, dados.messageData->message.infoAvioes.velocidade, dados.messageData->message.infoAvioes.aeroporto_inicial);

	

	do {
		_tprintf(TEXT("Comando:"));
		_fgetts(comando, _countof(comando), stdin);
		comando[_tcslen(comando) - 1] = '\0';

		//Meter a parte de quando o avião está em voo seja impossivel de usar qualquer comando

		if (_tcscmp(comando, TEXT("destino")) == 0) {

			if (dados.messageData->message.infoAvioes.comandos == FALSE) {
				errorGui(TEXT("Não pode usar comandos enquanto o avião está em vôo!!"));
			}
			else {

				destino(&dados);
			}

		}
		else if (_tcscmp(comando, TEXT("embarcar")) == 0) {

			if (dados.messageData->message.infoAvioes.comandos == FALSE) {
				errorGui(TEXT("Não pode usar comandos enquanto o avião está em vôo!!"));
			}
			else {

				embarcaPassageiros(&dados);
			}
			
		}
		else if (_tcscmp(comando, TEXT("viajar")) == 0) {

			if (dados.messageData->message.infoAvioes.comandos == FALSE) {
				errorGui(TEXT("Não pode usar comandos enquanto o avião está em vôo!!"));
			}
			else {

				viajar(&dados);

				hThreadvoo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Voar, &dados, 0, &threadVoo);
				if (hThreadvoo == NULL) {
					error(TEXT("Erro a criar thread para o avião voar"));
					return -1;
				}
				
			}
		}



	} while (_tcscmp(comando, TEXT("termina")) != 0);

	disconnectPlane(&dados);

	CloseHandle(dados.messageData->hMapFile);
	CloseHandle(dados.messageData->hMutex);
	CloseHandle(dados.messageData->aviaoMessageUpdateEvent);
	CloseHandle(dados.messageData->controlMessageUpdateEvent);
	CloseHandle(hThreadReceiveMessagesFromControl);


	UnmapViewOfFile(dados.messageData->sharedMessage);
	return 0;
	
}