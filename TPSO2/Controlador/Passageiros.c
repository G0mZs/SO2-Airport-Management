#include "Passageiros.h"

DWORD WINAPI managePassageiros(LPVOID lpParam) {
    data* dados = (data*)lpParam;

    if (dados == NULL) {
        error(TEXT("Can't receive data"));
        return FALSE;
    }

    BOOL fConnectedGame = FALSE;
    BOOL fConnectedMessage = FALSE;
    DWORD dwThreadId = 0;

    HANDLE hPipeMessage = INVALID_HANDLE_VALUE;

    HANDLE hThread = NULL;

    dados->hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (dados->hEvent == NULL) {
        error(TEXT("Can't create write event"));
    }

    while (TRUE) {
        hPipeMessage = CreateNamedPipe(
            PIPE_MESSAGE_NAME, PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED,
            PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
            PIPE_UNLIMITED_INSTANCES, sizeof(mensagem), sizeof(mensagem), 5000, NULL);

     

        if (hPipeMessage == INVALID_HANDLE_VALUE) {
            error(TEXT("Failed creating named pipe"));
            return FALSE;
        }

      

        /**
         * != 0 means success
         * ERROR_PIPE_CONNECTED means success too
         */
        fConnectedMessage = ConnectNamedPipe(hPipeMessage, NULL)
            ? TRUE
            : (GetLastError() == ERROR_PIPE_CONNECTED);

     

        if (fConnectedMessage) {
            dados->tmpPipeMessage = hPipeMessage;

            hThread = CreateThread(NULL, 0, manageClient, dados, 0, &dwThreadId);

            if (hThread == NULL) {
                error(TEXT("Failed to create thread for client"));
                return FALSE;
            }
            else {
                CloseHandle(hThread);
            }
        }
        else {
            CloseHandle(hPipeMessage);
         
        }
    }
    return TRUE;
}

DWORD WINAPI manageClient(LPVOID lpParam) {
   
        data* dados = (data*)lpParam;

        HANDLE readReady;
        OVERLAPPED OverlRd = { 0 };

        if (dados == NULL) {
            error(TEXT("Can't receive data"));
            return FALSE;
        }

        int id = GetCurrentThreadId();
        int index;
        int position = addClient(dados, id, dados->tmpPipeMessage);
        int pos;

        if (position == -1) {
            error(TEXT("Can't find a valid slot"));
            return FALSE;
        }

        DWORD nBytes = 0;
        BOOL fSuccess = FALSE;

        mensagem message;

        if (dados->pass[position].hPipeMessage == NULL) {
            error(TEXT("Can't access client pipe"));
            return FALSE;
        }

        readReady = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (readReady == NULL) {
            error(TEXT("Can't create read event!"));
            return FALSE;
        }

        while (TRUE) {
            fSuccess = readDataFromPipeAsync(dados->pass[position].hPipeMessage, readReady, &message, sizeof(message));

            if (!fSuccess) {
                if (GetLastError() == ERROR_BROKEN_PIPE) {
                    error(TEXT("Client %d disconnected! Removing him..."), id);
                }
                else {
                    error(TEXT("Can't read message data"));
                }
                break;
            }

             
            if (message.cmd == ENTER) {
                index = averiguaAeroportos(message.infoPassag.aeroporto_origem, message.infoPassag.aeroporto_destino, dados);
                if (index == -1) {
                    mensagem msg;
                    msg.cmd = ENTERFAILED;
                    writeDataToPipeAsync(dados->pass[position].hPipeMessage, dados->hEvent, &msg, sizeof(msg));
                    break;
                }
                else {
                    mensagem msg;
                    msg.cmd = ENTERSUCESS;
                    msg.infoPassag.id = id;
                    writeDataToPipeAsync(dados->pass[position].hPipeMessage, dados->hEvent, &msg, sizeof(msg));
                    pos = addPassToAirport(id,message.infoPassag.aeroporto_destino,message.infoPassag.aeroporto_origem,message.infoPassag.name,index,dados, dados->pass[position].hPipeMessage);
                    
                }
            }
            if (message.cmd == EMBARQUE) {
                verificaAvioes(dados, message);
            }
            if (message.cmd == PASSAG_DISCONNECT) {
                removePassag(dados, message);
                break;
            }
            if (message.cmd == DISCONNECT) {
                removePassag(dados, message);
                break;
            }
             
 
        }

        FlushFileBuffers(dados->pass[position].hPipeMessage);
        DisconnectNamedPipe(dados->pass[position].hPipeMessage);
        CloseHandle(dados->pass[position].hPipeMessage);
         

        return TRUE;
   }


int addClient(data* dados, int id, HANDLE hPipeMessag) {
   

    HANDLE hMutexClient;

    hMutexClient = OpenMutex(MUTEX_ALL_ACCESS, FALSE, CONTROL_PASSAG_MUTEX);


    WaitForSingleObject(hMutexClient, INFINITE);

    int position = -1;

    for (int i = 0; i < MAX_PASSAGEIROS; i++) {
        if (dados->pass[i].empty == TRUE) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        
        ReleaseMutex(hMutexClient);
        return -1;
    }

    Sleep(1000);
 
    dados->pass[position].id = id;
    dados->pass[position].hPipeMessage = hPipeMessag;
    
    dados->pass[position].empty = FALSE;


    ReleaseMutex(hMutexClient);

    return position;
}

int averiguaAeroportos(TCHAR aeroporto_origem[30], TCHAR aeroporto_destino[30],data *data) {

    int contador = 0;
    int indice;

    for (int i = 0; i < data->max_aeroportos; i++) {
        if (_tcscmp(data->aeroportos[i].nome, aeroporto_origem) == 0) {
            contador++;
            indice = i;
        }
    }

    for (int i = 0; i < data->max_aeroportos; i++) {
        if (_tcscmp(data->aeroportos[i].nome, aeroporto_destino) == 0) {
            contador++;
        }
    }

    if (contador == 2) {
        return indice;
    }
    else
        return -1;
}

int addPassToAirport(int id,TCHAR aeroporto_destino[30],TCHAR aeroporto_origem[30],TCHAR nome[50],int index,data* dados, HANDLE hpipeMsg) {
   
    int position = -1;

    HANDLE hMutexClient;
    hMutexClient = OpenMutex(MUTEX_ALL_ACCESS, FALSE, CONTROL_PASSAG_MUTEX);

    WaitForSingleObject(hMutexClient, INFINITE);

    for (int i = 0; i < MAX_PASSAGEIROS; i++) {
        if (dados->aeroportos[index].Passag[i].empty == TRUE) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        ReleaseMutex(hMutexClient);
        return -1;
    }

  
    dados->aeroportos[index].Passag[position].empty = FALSE;
    dados->aeroportos[index].Passag[position].id = id;
    dados->aeroportos[index].Passag[position].hPipeMessage = hpipeMsg;
    _tcscpy_s(dados->aeroportos[index].Passag[position].name, _countof(dados->aeroportos[index].Passag[position].name), nome);
    _tcscpy_s(dados->aeroportos[index].Passag[position].aeroporto_destino, _countof(dados->aeroportos[index].Passag[position].aeroporto_destino), aeroporto_destino);
    _tcscpy_s(dados->aeroportos[index].Passag[position].aeroporto_origem, _countof(dados->aeroportos[index].Passag[position].aeroporto_origem), aeroporto_origem);

    ReleaseMutex(hMutexClient);
    return position;
}

void verificaAvioes(data* dados, mensagem msg) {

    int position = getAeroportoIndex(dados,msg);
    int pos = position;

    int posPass = getPassIndex(dados, msg);
    int pos1 = posPass;

   

    for (int i = 0; i < dados->max_avioes; i++) {
        if (_tcscmp(dados->aeroportos[pos].arrayAvioes[i].aeroporto_destino,dados->aeroportos[pos].Passag[pos1].aeroporto_destino) == 0) {
            embarcaPassageiros(dados, msg, position, i,pos1);
        }
    }
}

void embarcaPassageiros(data* dados, mensagem msg, int posAeroporto, int posAviao, int posPass) {

    //enviar mensagem com embarque sucesso e meter no outro programa passag.embarque == TRUE, adicionar o passageiro ao aviao.

    int position = -1;
    

    for (int i = 0; i < MAX_PASSAGEIROS; i++) {
        if (dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[i].empty == TRUE) {
            position = i;
            break;
        }
    }

    if (position == -1) {
        return -1;
    }

    dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].empty = FALSE;
    dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].id = dados->aeroportos[posAeroporto].Passag[posPass].id;
    dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].hPipeMessage = dados->aeroportos[posAeroporto].Passag[posPass].hPipeMessage;
    _tcscpy_s(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].name, _countof(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].name), dados->aeroportos[posAeroporto].Passag[posPass].name);
    _tcscpy_s(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_destino, _countof(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_destino), dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_destino);
    _tcscpy_s(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_origem, _countof(dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_origem), dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_origem);
    dados->aeroportos[posAeroporto].Passag[posPass].id = 0;
    dados->aeroportos[posAeroporto].Passag[posPass].empty = TRUE;
    _tcscpy_s(dados->aeroportos[posAeroporto].Passag[posPass].name, _countof(dados->aeroportos[posAeroporto].Passag[posPass].name), TEXT(""));
    _tcscpy_s(dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_destino, _countof(dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_destino), TEXT(""));
    _tcscpy_s(dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_origem, _countof(dados->aeroportos[posAeroporto].Passag[posPass].aeroporto_origem), TEXT(""));

    mensagem message;

    for (int i = 0; i < MAX_PASSAGEIROS; i++) {
        if (dados->pass[i].id == dados->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].id) {
            message.cmd = EMBARQUESUCESS;
            writeDataToPipeAsync(dados->pass[i].hPipeMessage, dados->hEvent, &message, sizeof(message));
        }
    }

}

int getAeroportoIndex(data* dados,mensagem msg) {

    int posPass = 0;

    for (int i = 0; i < dados->max_aeroportos; i++) {
        for (int j = 0; j < MAX_PASSAGEIROS; j++) {
            if (dados->aeroportos[i].Passag[j].id == msg.infoPassag.id) {
                posPass = j;
            }
        }
    }

    int position = -1;

    for (int i = 0; i < dados->max_aeroportos; i++) {
        if (_tcscmp(dados->aeroportos[i].nome, dados->aeroportos[i].Passag[posPass].aeroporto_origem) == 0) {
            position = i;
            return position;
        }
    }

    return position;
}

int getPassIndex(data* dados,mensagem msg) {

    int posPass = 0;

    for (int i = 0; i < dados->max_aeroportos; i++) {
        for (int j = 0; j < MAX_PASSAGEIROS; j++) {
            if (dados->aeroportos[i].Passag[j].id == msg.infoPassag.id) {
                posPass = j;
            }
        }
    }
    
    int pos = posPass;

    return pos;
}

void removePassag(data* dados, mensagem msg) {

    for (int i = 0; i < dados->max_aeroportos; i++) {
        for (int j = 0; j < MAX_PASSAGEIROS; j++) {
            if (dados->aeroportos[i].Passag[j].id == msg.infoPassag.id) {
                dados->aeroportos[i].Passag[j].empty = TRUE;
                dados->aeroportos[i].Passag[j].id = 0;
                dados->aeroportos[i].Passag[j].hPipeMessage = NULL;
                _tcscpy_s(dados->aeroportos[i].Passag[j].name, _countof(dados->aeroportos[i].Passag[j].name), TEXT(""));
                _tcscpy_s(dados->aeroportos[i].Passag[j].aeroporto_destino, _countof(dados->aeroportos[i].Passag[j].aeroporto_destino), TEXT(""));
                _tcscpy_s(dados->aeroportos[i].Passag[j].aeroporto_origem, _countof(dados->aeroportos[i].Passag[j].aeroporto_origem), TEXT(""));
            }
        }
    }

    for (int i = 0; i < dados->max_aeroportos; i++) {
        for (int j = 0; j < dados->max_avioes; j++) {
            for (int k = 0; k < MAX_PASSAGEIROS; k++) {
                if (dados->aeroportos[i].arrayAvioes[j].passagAviao[k].id == msg.infoPassag.id) {
                    dados->aeroportos[i].arrayAvioes[j].passagAviao[k].empty = TRUE;
                    dados->aeroportos[i].arrayAvioes[j].passagAviao[k].id = 0;
                    dados->aeroportos[i].arrayAvioes[j].passagAviao[k].hPipeMessage = NULL;
                    _tcscpy_s(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].name, _countof(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].name), TEXT(""));
                    _tcscpy_s(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_destino, _countof(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_destino), TEXT(""));
                    _tcscpy_s(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_origem, _countof(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_origem), TEXT(""));
                }
            }
        }
    }

    for (int i = 0; i < dados->max_avioes; i++) {
        for (int j = 0; j < MAX_PASSAGEIROS; j++) {
            if (dados->arrayAvioes[i].passagAviao[j].id == msg.infoPassag.id) {
                dados->arrayAvioes[i].passagAviao[j].empty = TRUE;
                dados->arrayAvioes[i].passagAviao[j].id = 0;
                dados->arrayAvioes[i].passagAviao[j].hPipeMessage = NULL;
                _tcscpy_s(dados->arrayAvioes[i].passagAviao[j].name, _countof(dados->arrayAvioes[i].passagAviao[j].name), TEXT(""));
                _tcscpy_s(dados->arrayAvioes[i].passagAviao[j].aeroporto_destino, _countof(dados->arrayAvioes[i].passagAviao[j].aeroporto_destino), TEXT(""));
                _tcscpy_s(dados->arrayAvioes[i].passagAviao[j].aeroporto_origem, _countof(dados->arrayAvioes[i].passagAviao[j].aeroporto_origem), TEXT(""));
            }
        }
    }


    for (int i = 0; i < MAX_PASSAGEIROS; i++) {
        if (dados->pass[i].id == msg.infoPassag.id) {
            dados->pass[i].empty = TRUE;
            dados->pass[i].id = 0;
            dados->pass[i].hPipeMessage = NULL;
            _tcscpy_s(dados->pass[i].name, _countof(dados->pass[i].name), TEXT(""));
            _tcscpy_s(dados->pass[i].aeroporto_destino, _countof(dados->pass[i].aeroporto_destino), TEXT(""));
            _tcscpy_s(dados->pass[i].aeroporto_origem, _countof(dados->pass[i].aeroporto_origem), TEXT(""));
        }
    }
}