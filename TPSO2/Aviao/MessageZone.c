#include "../Passageiros/passag.h"
#include "MessageZone.h"
#include "../Utils/Library.h"


DWORD WINAPI receiveMessagesFromControl(LPVOID lpParam) {
	data* dados = (data*)lpParam;
	dados_mensagem* messageData = dados->messageData;
	mensagem message;

	DWORD dwWaitResult;

	messageData->STOP = FALSE;

	while (!messageData->STOP) {
		dwWaitResult = WaitForSingleObject(messageData->aviaoMessageUpdateEvent, INFINITE);

		if (dwWaitResult == WAIT_OBJECT_0) {
			readDataFromSharedMemory(messageData->sharedMessage, &message, sizeof(mensagem), &messageData->hMutex);

			switch (message.cmd) {
			case PLANESUSPEND:
				errorGui(TEXT("A entrada de aviões foi suspensa pelo controlador aéreo !! Tente novamente mais tarde..."));
				exit(0);
				break;
			case ENTERFAILED:
				//Meter um comando caso aeroporto ja esteja cheio
				errorGui(TEXT("Não foi possivel registar o avião porque possui um aeroporto inicial inexistente"));
				exit(0);
				break;
			case ENTERSUCESS:
				dados->messageData->message.infoAvioes.id = message.infoAvioes.id;
				dados->messageData->message.infoAvioes.posicaoX = message.infoAvioes.posicaoX;
				dados->messageData->message.infoAvioes.posicaoY = message.infoAvioes.posicaoY;
				dados->messageData->message.infoAvioes.evento = message.infoAvioes.evento;
				break;
			case DESTINYFAILED:
				errorGui(TEXT("\nO Aeroporto Destino que introduziu não existe!! Introduza um correto...\n"));
				_tcscpy_s(dados->messageData->message.infoAvioes.aeroporto_destino, _countof(dados->messageData->message.infoAvioes.aeroporto_destino),TEXT(""));
				break;
			case DESTINYSUCCESS:
				MessageBox(NULL, TEXT("DESTINO INSERIDO COM SUCESSO"), TEXT("Information"),MB_OK | MB_ICONINFORMATION);
				dados->messageData->message.infoAvioes.posicaoXAeroporto_destino = message.infoAvioes.posicaoXAeroporto_destino;
				dados->messageData->message.infoAvioes.posicaoYAeroporto_destino = message.infoAvioes.posicaoYAeroporto_destino;
				break;
			case AVOIDCOLISIONS:
				dados->messageData->message.infoAvioes.posicaoX = message.infoAvioes.proximoX + 1;
				dados->messageData->message.infoAvioes.posicaoY = message.infoAvioes.proximoY + 1;
				break;
			case UPDATESUCESS:
				dados->messageData->message.infoAvioes.posicaoX = message.infoAvioes.proximoX;
				dados->messageData->message.infoAvioes.posicaoY = message.infoAvioes.proximoY;
				break;
			case ARRIVALSUCESS:
				_tcscpy_s(dados->messageData->message.infoAvioes.aeroporto_inicial, _countof(dados->messageData->message.infoAvioes.aeroporto_inicial), message.infoAvioes.aeroporto_inicial);
				_tprintf(TEXT("\nChegou ao seu destino !!"));
				break;
			case NOCOMMANDS:
				dados->messageData->message.infoAvioes.comandos = FALSE;
				break;
			case CONTROL_DISCONNECT:
				errorGui(TEXT("O Control desconectou-se.Encerrando..."));
				exit(0);
				break;
			default:
			   
				break;


			}
		}
	}
}