#include <stdio.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"
#include "Registry.h"
#include "control.h"
#include "Mensagens.h"
#include "Aeroportos.h"
#include "Passageiros.h"
#include <Mmsystem.h>
#include <process.h>
#include "resource.h"
#include "Interface.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow) {

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine); 
	
	data dados;

	dados_mensagem messageData;
	TCHAR comando[50];

	DWORD threadRecebeMensagensDoControl;
	HANDLE hthreadrecebeMensagensDoAviao;

	DWORD threadReceiveDataFromPassagId;
	HANDLE hThreadReceiveDataFromPassag;

#ifdef UNICODE
	_setmode(_fileno(stdin), _O_WTEXT);
	_setmode(_fileno(stdout), _O_WTEXT);
#endif

	if (isControlRunning()) {
		errorGui(TEXT("Ja existe um controlador Aéreo ativo!!"));
		return FALSE;
	}

	dados.suspenso = FALSE;
	dados.messageData = &messageData;
	HANDLE hMutexClients;

	hMutexClients = CreateMutex(NULL, FALSE, CONTROL_PASSAG_MUTEX);
	if (hMutexClients == NULL) {
		error(TEXT("Creating clients mutex"));
		return FALSE;
	}
	

	if (!initMessageZone(&messageData)) {
		errorGui(TEXT("Não é possivel conectar os dados da mensagem ao servidor. A sair..."));
		return FALSE;
	}


	messageData.controlMessageUpdateEvent =	CreateEventW(NULL, FALSE, FALSE, MESSAGE_CONTROL_UPDATE_EVENT);
	

	if (messageData.controlMessageUpdateEvent == NULL) {
		errorGui(TEXT("Erro ao criar evento!!"));
		return FALSE;
	}




	hthreadrecebeMensagensDoAviao = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receiveMessagesFromAvioes,&dados, 0, &threadRecebeMensagensDoControl);
	if (hthreadrecebeMensagensDoAviao == NULL) {
		errorGui(TEXT("Erro ao criar a thread!!"));
		return FALSE;
	}
	

	
	
	hThreadReceiveDataFromPassag = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)managePassageiros, &dados, 0,&threadReceiveDataFromPassagId);
	if (hThreadReceiveDataFromPassag == NULL) {
		error(TEXT("Creating thread to manage clients"));
		return -1;
	}


	
	RegistryAvioes();
	RegistryAeroportos();
	
	dados.max_aeroportos = getRegistryValueAeroportos();
	dados.max_avioes = getRegistryValueAvioes();

	for (int i = 0; i < dados.max_aeroportos; i++) {
		dados.aeroportos[i].vazio = TRUE;
		for (int j = 0; j < dados.max_avioes; j++) {
			dados.aeroportos[i].arrayAvioes[j].empty = TRUE;
			dados.aeroportos[i].arrayAvioes[j].posicaoX = dados.aeroportos[i].posicaoX;
			dados.aeroportos[i].arrayAvioes[j].posicaoY = dados.aeroportos[i].posicaoY;
			dados.aeroportos[i].posicaoY = 0;
			dados.aeroportos[i].posicaoX = 0;
			_tcscpy_s(dados.aeroportos[i].arrayAvioes[j].aeroporto_destino, sizeof(dados.aeroportos[i].arrayAvioes[j].aeroporto_destino), TEXT(""));
		}
	}

	for (int i = 0; i < dados.max_avioes; i++) {
		dados.arrayAvioes[i].empty = TRUE;
	}

	for (int i = 0; i < dados.max_aeroportos; i++) {
	

	  dados.pass[i].empty = TRUE;

	  for (int j = 0; j < MAX_PASSAGEIROS; j++) {
		  dados.aeroportos[i].Passag[j].empty = TRUE;
	  }
	}

	for (int i = 0; i < dados.max_aeroportos; i++) {
		for (int j = 0; j < dados.max_avioes; j++) {
			for(int k = 0; k < MAX_PASSAGEIROS; k++){
				dados.aeroportos[i].arrayAvioes[j].passagAviao[k].empty = TRUE;
			}
		}
	}

	for (int j = 0; j < dados.max_avioes; j++) {
		for (int k = 0; k < MAX_PASSAGEIROS; k++) {
			dados.arrayAvioes[j].passagAviao[k].empty = TRUE;
		}
	}

	


	
	/*system("cls");
	_tprintf(TEXT("Controlador Aéreo Iniciado !!\n\n"));
	_tprintf(TEXT("Numero máximo de aeroportos: %d\n"),dados.max_aeroportos);
	_tprintf(TEXT("Numero máximo de avioes: %d\n\n"),dados.max_avioes);
	
	do {
		_tprintf(TEXT("Comando:"));
		_fgetts(comando, sizeof(comando), stdin);
		comando[_tcslen(comando) - 1] = '\0';


		if (_tcscmp(comando, TEXT("cria")) == 0) {

			criaAeroporto(&dados);

		}
		else if (_tcscmp(comando, TEXT("lista")) == 0) {

			listaDados(&dados);

		}
		else if (_tcscmp(comando, TEXT("ativa")) == 0) {

			dados.suspenso = FALSE;

		}
		else if (_tcscmp(comando, TEXT("suspende")) == 0) {

			dados.suspenso = TRUE;

		}

		
	} while (_tcscmp(comando,TEXT("encerra")) != 0);

	disconnectAll(&dados);
	*/

	if (!MyRegisterClass(hInstance)) {
		return 0;
	}

	

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow,&dados)) {
		return FALSE;
	}

	SetWindowLongPtr(dados.hWnd, 0, (LONG_PTR)&dados);
	

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CONTROL));


	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) {
		if (msg.message == WM_DESTROY) {
			break;
		}

		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	CloseHandle(dados.messageData->hMapFile);
	CloseHandle(dados.messageData->hMutex);
	CloseHandle(dados.messageData->aviaoMessageUpdateEvent);
	CloseHandle(dados.messageData->controlMessageUpdateEvent);
	CloseHandle(hthreadrecebeMensagensDoAviao);


	UnmapViewOfFile(dados.messageData->sharedMessage);
	CloseHandle(hMutexClients);

	return (int)msg.wParam;
}





