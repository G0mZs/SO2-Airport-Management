#pragma once

#include <stdio.h>
#include "../Utils/Structs.h"
#include "../Utils/Library.h"
#include "Registry.h"
#include "control.h"
#include "Mensagens.h"
#include "Aeroportos.h"
#include "Passageiros.h"
#include <Mmsystem.h>
#include <process.h>
#include "resource.h"
#include "resource1.h"


#ifndef INTERFACE_H
#define INTERFACE_H

BOOL CALLBACK Airport(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, data* dados);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
ATOM MyRegisterClass(HINSTANCE hInstance);




#endif

