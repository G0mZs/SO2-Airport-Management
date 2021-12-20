
#include "structs.h"

#ifndef LIBRARY_H
#define LIBRARY_H

#define CONTROL_RUNNING_EVENT TEXT("Global\\Control_running")

#define MESSAGES_MUTEX_NAME TEXT("sistema_messages_mutex")
#define MESSAGES_SHARED_MEMORY_NAME TEXT("sistema_messages_shared_memory")

//#define PIPE_GAME_NAME TEXT("\\\\.\\pipe\\phoenix_pipe_game")
//#define PIPE_MESSAGE_NAME TEXT("\\\\.\\pipe\\phoenix_pipe_message")

#define MESSAGE_CONTROL_UPDATE_EVENT TEXT("Global\\control_message_update_event")
#define MESSAGE_AVIAO_UPDATE_EVENT  TEXT("Global\\Avioes_message_update_event_%d")

#define PIPE_MESSAGE_NAME TEXT("\\\\.\\pipe\\system_pipe_message")

#define SYSTEM_UPDATE_EVENT TEXT("Global\\systemdata_update_event")

#define CONTROL_PASSAG_MUTEX TEXT("sistema_control_passageiros_mutex")



    VOID writeDataToSharedMemory(LPVOID sharedMemory,
        LPVOID data, SIZE_T size,
        HANDLE hMutex, HANDLE hEvent);

    VOID readDataFromSharedMemory(LPVOID sharedMemory,
        LPVOID data, SIZE_T size,
        HANDLE* hMutex);

    BOOL isControlRunning();

    VOID errorGui(LPCWSTR text);

    VOID error(LPCWSTR text, ...);

    BOOL initMessageZone(dados_mensagem* messageData);

    BOOL initMemAndSync(HANDLE* hMapFile, const LPCWSTR sharedMemoryName, HANDLE* hMutex, LPCWSTR mutexName);

    BOOL readDataFromPipe(HANDLE hPipe, LPVOID data, SIZE_T size);

    BOOL writeDataToPipe(HANDLE hPipe, LPVOID data, SIZE_T size);

    BOOL writeDataToPipeAsync(HANDLE hPipe, HANDLE hEvent, LPVOID data, SIZE_T size);

    BOOL readDataFromPipeAsync(HANDLE hPipe, HANDLE hEvent, LPVOID data, SIZE_T size);



#endif