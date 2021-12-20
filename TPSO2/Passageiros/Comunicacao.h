#pragma once

#include <Windows.h>
#include "../Utils/Library.h"
#include "../Utils/Structs.h"
#include <Mmsystem.h>
#include <process.h>

#ifndef COMUNICACAO_H
#define COMUNICACAO_H

BOOL connectPipes(passageiros* passag);

DWORD WINAPI messageReceiver(LPVOID lpParam);

BOOL handleCommand(passageiros* client, mensagem message);

BOOL joinGame(passageiros* passag);


#endif // COMUNICACAO_H
