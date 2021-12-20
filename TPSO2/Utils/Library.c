#include "Library.h"


VOID writeDataToSharedMemory(LPVOID sharedMemory, LPVOID data, SIZE_T size,
    HANDLE hMutex, HANDLE hEvent) {
    WaitForSingleObject(hMutex, INFINITE);
    CopyMemory(sharedMemory, data, size);
    ReleaseMutex(hMutex);
    if (!SetEvent(hEvent)) {
        error(TEXT("Failed to notify about data sent"));
    }
}

VOID readDataFromSharedMemory(LPVOID sharedMemory, LPVOID data, SIZE_T size,
    HANDLE* hMutex) {
    WaitForSingleObject(*hMutex, INFINITE);
    CopyMemory(data, sharedMemory, size);
    ReleaseMutex(*hMutex);
}

BOOL isControlRunning() {
    HANDLE m_hStartEvent = CreateEventW(NULL, FALSE, FALSE, CONTROL_RUNNING_EVENT);

    if (m_hStartEvent == NULL) {
        CloseHandle(m_hStartEvent);
        return TRUE;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {

        CloseHandle(m_hStartEvent);
        m_hStartEvent = NULL;
        return TRUE;
    }
    return FALSE;
}

VOID errorGui(LPCWSTR text) {
    MessageBox(NULL, text, TEXT("Error"), MB_OK | MB_ICONERROR);
}

VOID error(LPCWSTR text, ...) {
    TCHAR msg[1024];
    va_list argptr;
    va_start(argptr, text);
    _stprintf_s(msg, TEXT("[ERRO] %s (%d)\n"), text, GetLastError());
    _vftprintf(stderr, msg, argptr);
    va_end(argptr);
}

BOOL initMessageZone(dados_mensagem* messageData) {
    if (!initMemAndSync(&messageData->hMapFile, MESSAGES_SHARED_MEMORY_NAME,
        &messageData->hMutex, MESSAGES_MUTEX_NAME)) {
        return FALSE;
    }

    messageData->sharedMessage = (mensagem*)MapViewOfFile(
        messageData->hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(mensagem));

    if (messageData->sharedMessage == NULL) {
        error(TEXT("A mapear a memória partilhada"));
        return FALSE;
    }

    return TRUE;
}


BOOL initMemAndSync(HANDLE* hMapFile, const LPCWSTR sharedMemoryName, HANDLE* hMutex, LPCWSTR mutexName) {

    *hMapFile = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
        sizeof(data), sharedMemoryName);
    if (*hMapFile == NULL) {
        error(TEXT("A Inicializar a memória partilhada"));
        return FALSE;
    }
   
    
    *hMutex = CreateMutex(NULL, FALSE, mutexName);
    if (*hMutex == NULL) {
        error(TEXT("A criar o Mutex"));
        return FALSE;
    }

    return TRUE;
}

BOOL readDataFromPipe(HANDLE hPipe, LPVOID data, SIZE_T size) {
    DWORD nBytes;
    BOOL fSuccess;

    if (hPipe == NULL) {
        return FALSE;
    }

    fSuccess = ReadFile(hPipe, data, size, &nBytes, NULL);
    if (!fSuccess || !nBytes) {
        return FALSE;
    }

    return TRUE;
}

BOOL writeDataToPipe(HANDLE hPipe, LPVOID data, SIZE_T size) {
    DWORD nBytes;
    BOOL fSuccess;

    if (hPipe == NULL) {
        return FALSE;
    }

    if (data == NULL) {
        return FALSE;
    }

    fSuccess = WriteFile(hPipe, data, size, &nBytes, NULL);
    if (!fSuccess || !nBytes) {
        return FALSE;
    }

    return TRUE;
}

BOOL writeDataToPipeAsync(HANDLE hPipe, HANDLE hEvent, LPVOID data,SIZE_T size) {
    OVERLAPPED OverlWr = { 0 };
    DWORD nBytes;

    if (hEvent == NULL) {
        errorGui(TEXT("Write event is NULL!"));
        return FALSE;
    }

    if (hPipe == NULL) {
        return FALSE;
    }

    if (data == NULL) {
        errorGui(TEXT("Data is NULL"));
    }

    ZeroMemory(&OverlWr, sizeof(OverlWr));
    ResetEvent(hEvent);
    OverlWr.hEvent = hEvent;

    WriteFile(hPipe, data, size, &nBytes, &OverlWr);

    WaitForSingleObject(hEvent, INFINITE);

    GetOverlappedResult(hPipe, &OverlWr, &nBytes, FALSE);

    if (nBytes < size) {
        errorGui(TEXT("Can't write file!"));
        return FALSE;
    }

    return TRUE;
}

BOOL readDataFromPipeAsync(HANDLE hPipe, HANDLE hEvent, LPVOID data, SIZE_T size) {
    OVERLAPPED OverlRd = { 0 };
    DWORD nBytes = 0;

    ZeroMemory(&OverlRd, sizeof(OverlRd));
    ResetEvent(hEvent);
    OverlRd.hEvent = hEvent;

    ReadFile(hPipe, data, size, &nBytes, &OverlRd);

    WaitForSingleObject(hEvent, INFINITE);

    GetOverlappedResult(hPipe, &OverlRd, &nBytes, FALSE);
    if (nBytes < size) {
        return FALSE;
    }

    return TRUE;
}