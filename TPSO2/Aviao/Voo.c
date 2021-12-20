#include "Voo.h"

DWORD WINAPI Voar(LPVOID lpParam) {

    data* dados = (data*)lpParam;
    int x = 0, y = 0, res, contador = 0;

    dados_mensagem messageData;

    messageData.STOP = FALSE;

    mensagem msg = dados->messageData->message;


    //while principal, quando chegar ao seu destino atraves da função da dll ele sai deste while.

    while (!messageData.STOP) {

        //Analisar a velocidade do aviao; Pede a funcao x vezes por segundo
        while (contador <= dados->messageData->message.infoAvioes.velocidade)
        {
            //Obtem nova posição

            res = move(dados->messageData->message.infoAvioes.posicaoX, dados->messageData->message.infoAvioes.posicaoY, dados->messageData->message.infoAvioes.posicaoXAeroporto_destino, dados->messageData->message.infoAvioes.posicaoYAeroporto_destino, &x, &y);
            
            if (res == 0) {

                //Mensagem para control a dizer que chegou ao destino e no control meter o aviao que estava em voo no aeroporto que era o destino do aviao

                msg.cmd = ARRIVAL;
                msg.infoAvioes.evento = dados->messageData->message.infoAvioes.evento;
                writeDataToSharedMemory(dados->messageData->sharedMessage, &msg, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);

                //Para poder sair dos dois whiles
                messageData.STOP = TRUE;
                contador = dados->messageData->message.infoAvioes.velocidade;
            }
            else if (res == 1) {

                msg.cmd = UPDATECOORDS;
                msg.infoAvioes.proximoX = x;
                msg.infoAvioes.proximoY = y;
               

                writeDataToSharedMemory(dados->messageData->sharedMessage, &msg, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->controlMessageUpdateEvent);

                Sleep(100);
            }
            else {
                errorGui(TEXT("Erro na dll"));
            }
            contador++;
        }

        //Reseta o contador e espera 1 segundo ate poder pedir a posição seguinte

        contador = 0;
        Sleep(1000);
    }
    dados->messageData->message.infoAvioes.comandos = TRUE;

    return 0;
}