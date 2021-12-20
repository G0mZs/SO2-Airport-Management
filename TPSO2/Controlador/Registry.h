#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include "../Utils/Structs.h"


#ifndef REGISTRY_H
#define REGISTRY_H


	void RegistryAeroportos();
	void RegistryAvioes();
	int getRegistryValueAeroportos();
	int getRegistryValueAvioes();
	//LPBYTE getRegistryValue();


#endif