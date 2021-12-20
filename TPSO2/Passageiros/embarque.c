#include "embarque.h"


DWORD WINAPI embarque(LPVOID lpParam) {
	passageiros* passag = (passageiros*)lpParam;
	mensagem msg;
	msg.infoPassag.id = passag->id;
	
	

	if (passag->tempo_embarque == 0) {
		while (passag->embarque == FALSE) {
			embarca(passag,msg);
			if (passag->embarque == TRUE) {
				break;
			}
		}
	}
	else {
		
		
		LARGE_INTEGER time;
		time.QuadPart = (LONGLONG)-(passag->tempo_embarque) * 10000000;
		HANDLE timer = CreateWaitableTimer(NULL, TRUE, NULL);
		SetWaitableTimer(timer, &time, 0, NULL, NULL, 0);


		WaitForSingleObject(timer, INFINITE);
		embarca(passag,msg);
		Sleep(3000);

	
		if (passag->embarque == FALSE) {
			errorGui(TEXT("Não lhe foi atribuido um avião no tempo de espera.Saindo..."));
			PassagDisconnect(passag,msg);
			exit(0);
		}

		
		
	}
}

void embarca(passageiros* passag,mensagem msg) {

	
	msg.cmd = EMBARQUE;
	writeDataToPipeAsync(passag->hPipeMessage, passag->hEvent, &msg, sizeof(msg));
}

void PassagDisconnect(passageiros* passag,mensagem msg) {
	
	msg.cmd = PASSAG_DISCONNECT;
	msg.infoPassag.id = passag->id;
	writeDataToPipeAsync(passag->hPipeMessage, passag->hEvent, &msg, sizeof(msg));
}

