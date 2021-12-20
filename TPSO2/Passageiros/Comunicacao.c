#include "Comunicacao.h"


BOOL connectPipes(passageiros* passag) {
    DWORD dwMode;
    BOOL fSuccess;

    while (TRUE) {
        passag->hPipeMessage =
            CreateFile(PIPE_MESSAGE_NAME, GENERIC_READ | GENERIC_WRITE,
                0 | FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING,
                0 | FILE_FLAG_OVERLAPPED, NULL);

        if (passag->hPipeMessage != INVALID_HANDLE_VALUE) {
            break;
        }

        if (GetLastError() != ERROR_PIPE_BUSY) {
            errorGui(TEXT("Can't create file"));
            return FALSE;
        }

        /**
         * Wait 30 sec
         */
        if (!WaitNamedPipe(PIPE_MESSAGE_NAME, 30000)) {
            errorGui(TEXT("Timeout! Exiting..."));
            return FALSE;
        }
    }

    /**
     * Connected! Change pipe mode to read
     */
    dwMode = PIPE_READMODE_MESSAGE;
    fSuccess = SetNamedPipeHandleState(passag->hPipeMessage, &dwMode, NULL, NULL);

    if (!fSuccess) {
        errorGui(TEXT("Can't set message named pipe handle state"));
        return FALSE;
    }

    return TRUE;
}

DWORD WINAPI messageReceiver(LPVOID lpParam) {
    passageiros* passag = (passageiros*)lpParam;

    DWORD nBytes = 0;
    BOOL fSuccess = FALSE;

    mensagem message;

    HANDLE readReady;
    OVERLAPPED OverlRr = { 0 };

    if (passag->hPipeMessage == NULL) {
        errorGui(TEXT("Message pipe is NULL"));
        return FALSE;
    }

    readReady = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (readReady == NULL) {
        errorGui(TEXT("Can't create reader event!"));
        return FALSE;
    }

    passag->readerAlive = TRUE;

    while (passag->threadContinue) {
        fSuccess = readDataFromPipeAsync(passag->hPipeMessage, readReady, &message,
            sizeof(mensagem));

        if (!fSuccess) {
            if (GetLastError() == ERROR_BROKEN_PIPE) {
                errorGui(TEXT("Gateway disconnected! Can't obtain data!"));
            }
            else {
                errorGui(TEXT("Can't read message data"));
            }
            break;
        }

        if (!handleCommand(passag, message)) {
            break;
        }
    }

    passag->readerAlive = FALSE;

    return TRUE;
}

BOOL handleCommand(passageiros* client, mensagem message) {

    switch (message.cmd) {
        
         case ENTERFAILED:
             errorGui(TEXT("Não existem os aeroportos deste passageiro"));
             exit(0);
             break;
         case ENTERSUCESS:
             client->id = message.infoPassag.id;
             client->logged = TRUE;
             MessageBox(NULL, TEXT("Entrou no Sistema"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
             break;
         case EMBARQUESUCESS:
             client->embarque = TRUE;
             MessageBox(NULL, TEXT("Foi Embarcado"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
             break;
         case VOAR:
             _tprintf(TEXT("\nO seu avião começou a voar !!"));
             break;
         case COORDSPASS:
             _tprintf(TEXT("\n Está nas cordenadas[%d,%d] "), message.infoAvioes.posicaoX, message.infoAvioes.posicaoY);
             break;
         case CONTROL_DISCONNECT:
             _tprintf(TEXT("O Control desconectou-se!! Encerrando..."));
              exit(0);
              break;
         case PLANE_DISCONNECT:
             _tprintf(TEXT("\n\nO Avião teve um acidente!!"));
             mensagem msg;
             msg.cmd = DISCONNECT;
             msg.infoPassag.id = client->id;
             writeDataToPipeAsync(client->hPipeMessage, client->hEvent, &msg, sizeof(msg));
             exit(0);
             break;
         case ARRIVALPASS:
             _tprintf(TEXT("\n\nChegou ao seu destino !! Encerrando..."));
             return FALSE;
         }
        
        return TRUE;
   }



