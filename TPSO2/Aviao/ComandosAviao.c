#include "ComandosAviao.h"

//Funções relativas ao comando destino

void proximoDestino(data* dadosAviao) {

	mensagem Message;
	dadosAviao->messageData->message.cmd = DESTINO;

	Message = dadosAviao->messageData->message;
	
	writeDataToSharedMemory(dadosAviao->messageData->sharedMessage, &Message,sizeof(mensagem), dadosAviao->messageData->hMutex,dadosAviao->messageData->controlMessageUpdateEvent);
}

void destino(data* dados) {

	_tprintf(TEXT("\nIntroduza o proximo destino:"));
	_fgetts(dados->messageData->message.infoAvioes.aeroporto_destino, sizeof(dados->messageData->message.infoAvioes.aeroporto_destino), stdin);
	dados->messageData->message.infoAvioes.aeroporto_destino[_tcslen(dados->messageData->message.infoAvioes.aeroporto_destino) - 1] = '\0';

	if (_tcscmp(dados->messageData->message.infoAvioes.aeroporto_destino, dados->messageData->message.infoAvioes.aeroporto_inicial) == 0) {
		errorGui(TEXT("\nJá está neste destino!!\n"));
		return;
	}
	else {
		proximoDestino(dados);
	}

}

//Funções relativas ao comando embarcar

void embarcaPassageiros(data* dados) {

	mensagem Message;

	_tprintf(TEXT("\nEmbarcando Passageiros...\n"));
	
	dados->messageData->message.cmd = EMBARCA;
	Message = dados->messageData->message;

	writeDataToSharedMemory(dados->messageData->sharedMessage, &Message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);
}

void enterSystem(data* dados) {

	mensagem msg;

	dados->messageData->message.cmd = ENTER;
	msg = dados->messageData->message;

	writeDataToSharedMemory(dados->messageData->sharedMessage, &msg, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);
}

void viajar(data* dados) {

	if (_tcscmp(dados->messageData->message.infoAvioes.aeroporto_destino, TEXT("")) == 0) {
		errorGui(TEXT("Introduza um destino primeiro !!"));
		return;
	}
	else {

		mensagem msg;


		dados->messageData->message.cmd = FLY;
		msg = dados->messageData->message;
		msg.infoAvioes.evento = dados->messageData->aviaoMessageUpdateEvent;

		writeDataToSharedMemory(dados->messageData->sharedMessage, &msg, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);



	}




}

void disconnectPlane(data* dados) {
	mensagem msg;

	msg.infoAvioes.id = dados->messageData->message.infoAvioes.id;
	msg.cmd = PLANE_DISCONNECT;
	writeDataToSharedMemory(dados->messageData->sharedMessage, &msg, sizeof(msg), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);


}