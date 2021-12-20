#include <stdio.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"
#include "Aeroportos.h"

void recebeComandos(data* data, mensagem message) {

    switch (message.cmd) {
    case ENTER:
        if (data->suspenso == TRUE) {
            suspendeAvioes(data, message);
        }
        else {
            if (procuraAeroportoInicial(data, message) != 10) {
                insereAviao(data, message, procuraAeroportoInicial(data, message));
            }
            else {
                inserirFalhado(data, message);
            }
        }

        break;
    case DESTINO:
        procuraDestino(data, message);
        break;
    case FLY:
        voa(data, message,procuraAeroportoInicial(data,message));
        break;
    case EMBARCA:
        embarca(data, message);
        break;
    case UPDATECOORDS:
        updateCoordenadas(data, message);
        break;
    case ARRIVAL:
        chegada(data, message, procuraAeroportoDestino(data, message));
        break;
    case PLANE_DISCONNECT:
        removePlane(data, message);
        break;
    case EXIT:
        MessageBox(NULL, TEXT("EXIT REQUESTED"), TEXT("Exit"),
            MB_OK | MB_ICONINFORMATION);
        //SendMessage(data->hWnd, WM_DESTROY, 0, 0);
        break;
    }
}

DWORD WINAPI receiveMessagesFromAvioes(LPVOID lpParam) {
    data* dados = (data*)lpParam;
    

    dados_mensagem* messageData = dados->messageData;

    DWORD dwWaitResult;

    messageData->STOP = FALSE;

    while (!messageData->STOP) {
        
        dwWaitResult = WaitForSingleObject(messageData->controlMessageUpdateEvent, INFINITE);

        if (dwWaitResult == WAIT_OBJECT_0) {
            readDataFromSharedMemory(messageData->sharedMessage,
                &messageData->message, sizeof(mensagem),
                &messageData->hMutex);
            recebeComandos(dados, messageData->message);
           
        }
    }
    return 0;
}

