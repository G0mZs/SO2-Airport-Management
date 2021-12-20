
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.c"


void RegistryAvioes() {

    HKEY key;
    DWORD result, size;
    data dados;
 
    dados.max_avioes = 10;



    if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SO2\\Avioes"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &result) != ERROR_SUCCESS) {
        errorGui(TEXT("Não é possivel criar chave no Registry. Pode afetar os dados do control"));
        return;
    }
    else {

        if (result == REG_CREATED_NEW_KEY)
        {
           
            RegSetValueEx(key, TEXT("Numero de Avioes"), 0, REG_DWORD, (LPBYTE)&dados.max_avioes, sizeof(DWORD));
           
        }
        else if (result == REG_OPENED_EXISTING_KEY) {

            size = sizeof(dados.max_avioes);
            RegQueryValueEx(key, TEXT("Numero de Avioes"), NULL, NULL, (LPBYTE)&dados.max_avioes, &size);
           

        }
    }



    RegCloseKey(key);


    return 0;
}
   
void RegistryAeroportos() {

    HKEY key;
    DWORD result, size;
    data dados;
    dados.max_aeroportos = 5;
 



    if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SO2\\Aeroportos"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &result) != ERROR_SUCCESS) {
        errorGui(TEXT("Não é possivel criar chave no Registry. Pode afetar os dados do control"));
        return;
    }
    else {

        if (result == REG_CREATED_NEW_KEY)
        {
            
            RegSetValueEx(key, TEXT("Numero de Aeroportos"), 0, REG_DWORD, (LPBYTE)&dados.max_aeroportos, sizeof(DWORD));
           
        }
        else if (result == REG_OPENED_EXISTING_KEY) {

            size = sizeof(dados.max_aeroportos);
            RegQueryValueEx(key, TEXT("Numero de Aeroportos"), NULL, NULL, (LPBYTE)&dados.max_aeroportos, &size);
            

        }
    }



    RegCloseKey(key);


    return 0;
}

int getRegistryValueAeroportos() {

    HKEY key;
    DWORD result, size;
    data dados;


    if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SO2\\Aeroportos"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &result) != ERROR_SUCCESS) {
        errorGui(TEXT("Não é possivel criar chave no Registry. Pode afetar os dados do control"));
        return;
    }
    else {

        if (result == REG_OPENED_EXISTING_KEY) {

            size = sizeof(dados.max_aeroportos);
            RegQueryValueEx(key, TEXT("Numero de Aeroportos"), NULL, NULL, (LPBYTE)&dados.max_aeroportos, &size);
            

        }
    }


    return dados.max_aeroportos;
    RegCloseKey(key);
}

int getRegistryValueAvioes() {

    HKEY key;
    DWORD result, size;
    data dados;


    if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\SO2\\Avioes"), 0, NULL, 0, KEY_ALL_ACCESS, NULL, &key, &result) != ERROR_SUCCESS) {
        errorGui(TEXT("Não é possivel criar chave no Registry. Pode afetar os dados do control"));
        return;
    }
    else {

      
        if (result == REG_OPENED_EXISTING_KEY) {

            size = sizeof(dados.max_avioes);
            RegQueryValueEx(key, TEXT("Numero de Avioes"), NULL, NULL, (LPBYTE)&dados.max_avioes, &size);
            

        }
    }

    return dados.max_avioes;
    RegCloseKey(key);
}