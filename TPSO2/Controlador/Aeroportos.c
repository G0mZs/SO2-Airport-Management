#include "Aeroportos.h"

int procuraAeroportoInicial(data* data, mensagem message) {

	for (int i = 0; i < data->max_aeroportos; i++) {
		if (_tcscmp(data->aeroportos[i].nome, message.infoAvioes.aeroporto_inicial) == 0) {
			return i;
		}
	}

	return 10;


}

void voa(data* data, mensagem message,int pos) {

	int position = -1;

	for (int i = 0; i < data->max_avioes; i++) {
		if (data->arrayAvioes[i].empty == TRUE) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		errorGui(TEXT("O Numero de avioes está no máximo!!Espere que alguns acabem o voo"));
		return;
	}

	data->arrayAvioes[position].empty = FALSE;
	data->arrayAvioes[position].id = message.infoAvioes.id;
	data->arrayAvioes[position].capacidade = message.infoAvioes.capacidade;
	data->arrayAvioes[position].posicaoX = message.infoAvioes.posicaoX;
	data->arrayAvioes[position].posicaoY = message.infoAvioes.posicaoY;
	_tcscpy_s(data->arrayAvioes[position].aeroporto_destino, _countof(data->arrayAvioes[position].aeroporto_destino),message.infoAvioes.aeroporto_destino);
	data->arrayAvioes[position].posicaoXAeroporto_destino = message.infoAvioes.posicaoXAeroporto_destino;
	data->arrayAvioes[position].posicaoYAeroporto_destino = message.infoAvioes.posicaoYAeroporto_destino;
	
	

	for (int i = 0; i < data->max_avioes; i++) {
		for (int j = 0; j < MAX_PASSAGEIROS; j++) {
			if (data->aeroportos[pos].arrayAvioes[i].id == message.infoAvioes.id) {
				data->arrayAvioes[position].evento = data->aeroportos[pos].arrayAvioes[i].evento;
				if (data->aeroportos[pos].arrayAvioes[i].passagAviao[j].empty == FALSE) {
					addPassToFlying(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].id, data->aeroportos[pos].arrayAvioes[i].passagAviao[j].name, data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_destino, data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_origem, data->aeroportos[pos].arrayAvioes[i].passagAviao[j].hPipeMessage,position,data);
				}
			}
		}
	}

	

	



	for (int i = 0; i < data->max_avioes; i++) {
		if (data->aeroportos[pos].arrayAvioes[i].id == message.infoAvioes.id) {
			data->aeroportos[pos].arrayAvioes[i].id = 0;
			data->aeroportos[pos].arrayAvioes[i].empty = TRUE;
			_tcscpy_s(data->aeroportos[pos].arrayAvioes[i].aeroporto_destino,_countof(data->aeroportos[pos].arrayAvioes[i].aeroporto_destino),TEXT(""));
			_tcscpy_s(data->aeroportos[pos].arrayAvioes[i].aeroporto_inicial, _countof(data->aeroportos[pos].arrayAvioes[i].aeroporto_inicial),TEXT(""));
			data->aeroportos[pos].arrayAvioes[i].posicaoX = 0;
			data->aeroportos[pos].arrayAvioes[i].posicaoY = 0;
			data->aeroportos[pos].arrayAvioes[i].posicaoXAeroporto_destino = 0;
			data->aeroportos[pos].arrayAvioes[i].posicaoYAeroporto_destino = 0;
			data->aeroportos[pos].arrayAvioes[i].proximoX = 0;
			data->aeroportos[pos].arrayAvioes[i].proximoY = 0;
			for (int j = 0; j < MAX_PASSAGEIROS; j++) {
				data->aeroportos[pos].arrayAvioes[i].passagAviao[j].empty = TRUE;
				data->aeroportos[pos].arrayAvioes[i].passagAviao[j].id = 0;
				data->aeroportos[pos].arrayAvioes[i].passagAviao[j].hPipeMessage = NULL;
				_tcscpy_s(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].name, _countof(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].name), TEXT(""));
				_tcscpy_s(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_destino, _countof(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_destino), TEXT(""));
				_tcscpy_s(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_origem, _countof(data->aeroportos[pos].arrayAvioes[i].passagAviao[j].aeroporto_origem), TEXT(""));
				
			}
		
			
		}
	}

	mensagem msg;
	msg.cmd = NOCOMMANDS;

	data->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
	if (data->messageData->aviaoMessageUpdateEvent == NULL) {
		error(TEXT("OpenEvent failed"));
		return FALSE;
	}
	writeDataToSharedMemory(data->messageData->sharedMessage, &msg, sizeof(mensagem), data->messageData->hMutex, data->messageData->aviaoMessageUpdateEvent);



}

void addPassToFlying(int id, TCHAR nome[50], TCHAR destino[30], TCHAR origem[30], HANDLE hpipemessag, int index,data* data) {

	int position = -1;

	for (int i = 0; i < MAX_PASSAGEIROS; i++) {
		if (data->arrayAvioes[index].passagAviao[i].empty == TRUE) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		return;
	}

	data->arrayAvioes[index].passagAviao[position].empty = FALSE;
	data->arrayAvioes[index].passagAviao[position].id = id;
	data->arrayAvioes[index].passagAviao[position].hPipeMessage = hpipemessag;
	_tcscpy_s(data->arrayAvioes[index].passagAviao[position].name, _countof(data->arrayAvioes[index].passagAviao[position].name), nome);
	_tcscpy_s(data->arrayAvioes[index].passagAviao[position].aeroporto_destino, _countof(data->arrayAvioes[index].passagAviao[position].aeroporto_destino), destino);
	_tcscpy_s(data->arrayAvioes[index].passagAviao[position].aeroporto_origem, _countof(data->arrayAvioes[index].passagAviao[position].aeroporto_origem), origem);
	mensagem mens;
	mens.cmd = VOAR;
	writeDataToPipeAsync(data->arrayAvioes[index].passagAviao[position].hPipeMessage, data->hEvent, &mens, sizeof(mens));
}

void insereAviao(data* data, mensagem message,int pos) {

	mensagem msg;

		int position = -1;
		//Falta restrição do maximo de avioes(10). A ideia é dividir os avioes pelos aeroportos porque so podem estar 10 em voo ou seja  em cada aeroporto so pode estar 2
		for (int i = 0; i < data->max_avioes; i++) {
			if (data->aeroportos[pos].arrayAvioes[i].empty) {
				position = i;
				break;
			}
		}

		if (position == -1) {
			errorGui(TEXT("Aeroportos no máximo!!Não é possivel acrescentar mais"));
			return;
		}

		data->aeroportos[pos].arrayAvioes[position].empty = FALSE;
		data->aeroportos[pos].arrayAvioes[position].id = message.infoAvioes.id;
		data->aeroportos[pos].arrayAvioes[position].capacidade = message.infoAvioes.capacidade;
		data->aeroportos[pos].arrayAvioes[position].posicaoX = data->aeroportos[pos].posicaoX;
		data->aeroportos[pos].arrayAvioes[position].posicaoY = data->aeroportos[pos].posicaoY;
		_tcscpy_s(data->aeroportos[pos].arrayAvioes[position].aeroporto_destino, _countof(data->aeroportos[pos].arrayAvioes[position].aeroporto_destino), TEXT(""));
		msg.infoAvioes.posicaoX = data->aeroportos[pos].posicaoX;
		msg.infoAvioes.posicaoY = data->aeroportos[pos].posicaoY;
		msg.infoAvioes.id = data->aeroportos[pos].arrayAvioes[position].id;
		msg.cmd = ENTERSUCESS;

		data->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
		data->aeroportos[pos].arrayAvioes[position].evento = data->messageData->aviaoMessageUpdateEvent;
		msg.infoAvioes.evento = data->messageData->aviaoMessageUpdateEvent;
		if (data->messageData->aviaoMessageUpdateEvent == NULL) {
			error(TEXT("OpenEvent failed"));
			return FALSE;
		}
		writeDataToSharedMemory(data->messageData->sharedMessage, &msg, sizeof(mensagem), data->messageData->hMutex, data->messageData->aviaoMessageUpdateEvent);
	
	
}


void inserirFalhado(data* data, mensagem message) {

	data->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
	if (data->messageData->aviaoMessageUpdateEvent == NULL) {
		error(TEXT("OpenEvent failed"));
		return FALSE;
	}

	message.cmd = ENTERFAILED;
	writeDataToSharedMemory(data->messageData->sharedMessage, &message, sizeof(mensagem), data->messageData->hMutex, data->messageData->aviaoMessageUpdateEvent);
}

void procuraDestino(data *dados,mensagem message) {

	dados->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
	if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
		error(TEXT("OpenEvent failed"));
		return FALSE;
	}

	int contador = 0;

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (_tcscmp(message.infoAvioes.aeroporto_destino, dados->aeroportos[i].nome) == 0) {
			message.infoAvioes.posicaoXAeroporto_destino = dados->aeroportos[i].posicaoX;
			message.infoAvioes.posicaoYAeroporto_destino = dados->aeroportos[i].posicaoY;
			inserirDestino(dados, message);
			message.cmd = DESTINYSUCCESS;
			writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);
			contador++;
			return;
		}
		
	}
	

	message.cmd = DESTINYFAILED;
	writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);
}

void inserirDestino(data* dados, mensagem message) {
	

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (_tcscmp(dados->aeroportos[i].nome, message.infoAvioes.aeroporto_inicial) == 0) {
			for (int j = 0; j < dados->max_avioes; j++) {
				if (message.infoAvioes.id == dados->aeroportos[i].arrayAvioes[j].id) {
					_tcscpy_s(dados->aeroportos[i].arrayAvioes[j].aeroporto_destino, _countof(dados->aeroportos[i].arrayAvioes[j].aeroporto_destino), message.infoAvioes.aeroporto_destino);
				}

			}
		}
	}
}

int embarca(data* dados, mensagem message) {

	int posAviao = getIndexAviao(dados, message);
	int pos = posAviao;


	for (int i = 0; i < dados->max_aeroportos; i++) {
		for (int j = 0; j < MAX_PASSAGEIROS; j++) {
			if (_tcscmp(dados->aeroportos[i].Passag[j].aeroporto_origem, message.infoAvioes.aeroporto_inicial) == 0 && _tcscmp(dados->aeroportos[i].Passag[j].aeroporto_destino, message.infoAvioes.aeroporto_destino) == 0) {

				embarcaPass(dados, message, dados->aeroportos[i].Passag[j].id, dados->aeroportos[i].Passag[j].name, dados->aeroportos[i].Passag[j].aeroporto_destino, dados->aeroportos[i].Passag[j].aeroporto_origem, dados->aeroportos[i].Passag[j].hPipeMessage, i, pos);
				dados->aeroportos[i].Passag[j].id = 0;
				dados->aeroportos[i].Passag[j].empty = TRUE;
				dados->aeroportos[i].Passag[j].hPipeMessage = NULL;
				_tcscpy_s(dados->aeroportos[i].Passag[j].name, _countof(dados->aeroportos[i].Passag[j].name), TEXT(""));
				_tcscpy_s(dados->aeroportos[i].Passag[j].aeroporto_destino, _countof(dados->aeroportos[i].Passag[j].aeroporto_destino), TEXT(""));
				_tcscpy_s(dados->aeroportos[i].Passag[j].aeroporto_origem, _countof(dados->aeroportos[i].Passag[j].aeroporto_origem), TEXT(""));
				
				
				
			}
		}
	}

	
	
}

void embarcaPass(data* data, mensagem msg,int id, TCHAR nome[50],TCHAR destino[50],TCHAR origem[50],HANDLE pipeM,int posAeroporto,int posAviao) {

	int position = -1;

	for (int i = 0; i < MAX_PASSAGEIROS; i++) {
		if (data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[i].empty == TRUE) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		return;
	}

	data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].empty = FALSE;
	data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].id = id;
	data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].hPipeMessage = pipeM;
	_tcscpy_s(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].name, _countof(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].name), nome);
	_tcscpy_s(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_destino, _countof(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_destino), destino);
	_tcscpy_s(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_origem, _countof(data->aeroportos[posAeroporto].arrayAvioes[posAviao].passagAviao[position].aeroporto_origem), origem);
}

int getIndexAviao(data* dados, mensagem msg) {
	
	for (int i = 0; i < dados->max_aeroportos; i++) {
		for (int j = 0; j < dados->max_avioes; j++) {
			if (dados->aeroportos[i].arrayAvioes[j].id == msg.infoAvioes.id) {
				return j;
			}
		}
	}

	return -1;
	
}

void listaDados(data* dados) {

	_tprintf(TEXT("\n\nDados:\n"));

	listaAeroporto(dados);
	

}

void listaAeroporto(data* dados) {

	

	system("cls");
	
	for (int i = 0; i < dados->max_aeroportos; i++) {
		if(dados->aeroportos[i].vazio == FALSE){
			_tprintf(TEXT("Aeroporto de %s || Coordenadas [X:%d,Y:%d]\n"), dados->aeroportos[i].nome, dados->aeroportos[i].posicaoX, dados->aeroportos[i].posicaoY);
			for (int k = 0; k < MAX_PASSAGEIROS; k++) {
				if (dados->aeroportos[i].Passag[k].empty == FALSE) {
					_tprintf(TEXT("Id do Passageiro: %d || Nome: %s  || Destino:  %s  || Origem:  %s\n\n"), dados->aeroportos[i].Passag[k].id, dados->aeroportos[i].Passag[k].name, dados->aeroportos[i].Passag[k].aeroporto_destino, dados->aeroportos[i].Passag[k].aeroporto_origem);
				}
				//Bug nao esta a mostrar o nome nao sei porque.
				
			}
			for (int j = 0; j < dados->max_avioes; j++) {
				if (dados->aeroportos[i].arrayAvioes[j].empty == FALSE) {
					_tprintf(TEXT("\n Aviao %d || Coordenadas [X:%d,Y:%d] || Capacidade[%d] || Destino: %s\n\n"), dados->aeroportos[i].arrayAvioes[j].id, dados->aeroportos[i].arrayAvioes[j].posicaoX, dados->aeroportos[i].arrayAvioes[j].posicaoY, dados->aeroportos[i].arrayAvioes[j].capacidade, dados->aeroportos[i].arrayAvioes[j].aeroporto_destino);
					for (int k = 0; k < dados->aeroportos[i].arrayAvioes[j].capacidade; k++) {
						if (dados->aeroportos[i].arrayAvioes[j].passagAviao[k].empty == FALSE) {
							_tprintf(TEXT("\n  Id do Passageiro: %d || Nome: %s  || Destino:  %s  || Origem:  %s\n"), dados->aeroportos[i].arrayAvioes[j].passagAviao[k].id, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].name, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_destino, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_origem);
						}
					}
				}
			}
		}
	}

	_tprintf(TEXT("\nAviões em Voo:\n\n"));
	for (int i = 0; i < dados->max_avioes; i++) {
		if (dados->arrayAvioes[i].empty == FALSE) {
			_tprintf(TEXT(" Aviao %d || Coordenadas [X:%d,Y:%d] || Capacidade[%d] || Destino: %s\n"), dados->arrayAvioes[i].id, dados->arrayAvioes[i].posicaoX, dados->arrayAvioes[i].posicaoY, dados->arrayAvioes[i].capacidade, dados->arrayAvioes[i].aeroporto_destino);
		}

		for (int k = 0; k < dados->arrayAvioes[i].capacidade; k++) {
			if (dados->arrayAvioes[i].passagAviao[k].empty == FALSE) {
				_tprintf(TEXT("\n  Id do Passageiro: %d || Nome: %s  || Destino:  %s  || Origem:  %s\n"), dados->arrayAvioes[i].passagAviao[k].id, dados->arrayAvioes[i].passagAviao[k].name, dados->arrayAvioes[i].passagAviao[k].aeroporto_destino, dados->arrayAvioes[i].passagAviao[k].aeroporto_origem);
			}
		}
	}

	
}

void criaAeroporto(data* dados) {

	TCHAR nome[50], x1[10], y1[10];
	int x = 0, y = 0;

	_tprintf(TEXT("Introduza o nome do Aeroporto que pretende criar:"));
	_fgetts(nome, sizeof(nome), stdin);
	nome[_tcslen(nome) - 1] = '\0';

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (_tcscmp(dados->aeroportos[i].nome, nome) == 0) {
			errorGui(TEXT("Já existe um aeroporto com esse nome!!"));
			return;
		}
	}

	do {

		_tprintf(TEXT("\nIntroduza a coordenada X do Aeroporto:"));
		_fgetts(x1, sizeof(x1), stdin);
		
		x = _wtoi(x1);

		_tprintf(TEXT("\nIntroduza a coordenada Y do Aeroporto:"));
		_fgetts(y1, sizeof(y1), stdin);
	
		y = _wtoi(y1);


		if (y > 1000 || y < 0 || x < 0 || x > 1000) {
			_tprintf(TEXT("\nAmbas as coordenadas têm de estar no intervalo [0,1000]"));
		}
		if (averiguaCoordenadas(dados, x, y) == 0) {
			_tprintf(TEXT("\nAmbas as coordenadas têm de possuir uma distância de 10 posições com outros Aeroportos"));
		}

	} while (y > 1000 || y < 0 || x < 0 || x > 1000 || averiguaCoordenadas(dados,x,y) == 0);

	cria(dados, nome, x, y);
}

int averiguaCoordenadas(data* dados, int x, int y) {

	int contador = 0;

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (dados->aeroportos[i].vazio == FALSE) {
			if (x > dados->aeroportos[i].posicaoX + 10 || x < dados->aeroportos[i].posicaoX - 10 && y > dados->aeroportos[i].posicaoY + 10 || y < dados->aeroportos[i].posicaoY - 10) {
				contador++;
			}
		}
	}

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (dados->aeroportos[i].vazio == TRUE) {
			if (x > dados->aeroportos[i].posicaoX  || x < dados->aeroportos[i].posicaoX  && y > dados->aeroportos[i].posicaoY || y < dados->aeroportos[i].posicaoY) {
				contador++;
			}
		}
	}

	if (contador == dados->max_aeroportos) {
		return 1;
	}
	else {
		return 0;
	}

}



void cria(data* data, TCHAR nome[50], int x,int y) {
	

	int position = -1;

	for (int i = 0; i < MAX_AEROPORTOS; i++) {
		if (data->aeroportos[i].vazio) {
			position = i;
			break;
		}
	}

	if (position == -1) {
		errorGui(TEXT("Aeroportos no máximo!!Não é possivel acrescentar mais"));
		return;
	}

	data->aeroportos[position].vazio = FALSE;
	_tcscpy_s(data->aeroportos[position].nome,_countof(data->aeroportos[position].nome),nome);
	data->aeroportos[position].posicaoX = x;
	data->aeroportos[position].posicaoY = y;
	 
		for (int j = 0; j < MAX_PASSAGEIROS; j++) {
			data->aeroportos[position].Passag[j].empty = TRUE;
		}
	

	
}

void suspendeAvioes(data* dados, mensagem message) {

	dados->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
	if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
		error(TEXT("OpenEvent failed"));
		return FALSE;
	}

	message.cmd = PLANESUSPEND;
	writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);
}

void updateCoordenadas(data* dados, mensagem message) {

	int contador = 0;

	for (int i = 0; i < dados->max_avioes; i++) {
		if(dados->arrayAvioes->empty == FALSE){
			if (message.infoAvioes.proximoX == dados->arrayAvioes[i].posicaoX && message.infoAvioes.proximoY == dados->arrayAvioes[i].posicaoY) {
				contador++;
			}
		}
	}


	if (contador > 0) {

		for (int i = 0; i < dados->max_avioes; i++) {
			if (message.infoAvioes.id == dados->arrayAvioes[i].id) {
				dados->arrayAvioes[i].posicaoX = message.infoAvioes.proximoX + 1;
				dados->arrayAvioes[i].posicaoY = message.infoAvioes.proximoY + 1;
			}
		}

		dados->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
		if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
			error(TEXT("OpenEvent failed"));
			return FALSE;
		}

		message.cmd = AVOIDCOLISIONS;
		writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);
	}
	else if (contador == 0) {

		for (int i = 0; i < dados->max_avioes; i++) {
			if (message.infoAvioes.id == dados->arrayAvioes[i].id) {
				dados->arrayAvioes[i].posicaoX = message.infoAvioes.proximoX;
				dados->arrayAvioes[i].posicaoY = message.infoAvioes.proximoY;

				dados->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
				if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
					error(TEXT("OpenEvent failed"));
					return FALSE;
				}

				message.cmd = UPDATESUCESS;
				writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);

				for (int j = 0; j < MAX_PASSAGEIROS; j++) {
					if (dados->arrayAvioes[i].passagAviao[j].empty == FALSE) {
						mensagem mens;
						mens.cmd = COORDSPASS;
						mens.infoAvioes.posicaoX = dados->arrayAvioes[i].posicaoX;
						mens.infoAvioes.posicaoY = dados->arrayAvioes[i].posicaoY;
						writeDataToPipeAsync(dados->arrayAvioes[i].passagAviao[j].hPipeMessage, dados->hEvent, &mens, sizeof(mens));
					}
				}
			}
		}
	}
}

void chegada(data* data, mensagem message, int pos) {

	mensagem msg;

	int position = -1;
	//Falta restrição do maximo de avioes(10). A ideia é dividir os avioes pelos aeroportos porque so podem estar 10 em voo ou seja  em cada aeroporto so pode estar 2
	for (int i = 0; i < data->max_avioes; i++) {
		if (data->aeroportos[pos].arrayAvioes[i].empty) {
			position = i;
			break;
		}
	}
	
	if (position == -1) {
		errorGui(TEXT("Aeroportos no máximo!!Não é possivel acrescentar mais"));
		return;
	}
	

	data->aeroportos[pos].arrayAvioes[position].empty = FALSE;
	data->aeroportos[pos].arrayAvioes[position].id = message.infoAvioes.id;
	data->aeroportos[pos].arrayAvioes[position].capacidade = message.infoAvioes.capacidade;
	data->aeroportos[pos].arrayAvioes[position].posicaoX = data->aeroportos[pos].posicaoX;
	data->aeroportos[pos].arrayAvioes[position].posicaoY = data->aeroportos[pos].posicaoY;
	data->aeroportos[pos].arrayAvioes[position].posicaoY = message.infoAvioes.evento;
	_tcscpy_s(data->aeroportos[pos].arrayAvioes[position].aeroporto_destino, _countof(data->aeroportos[pos].arrayAvioes[position].aeroporto_destino), TEXT(""));
	//Pessoas
	data->aeroportos[pos].arrayAvioes[position].evento = message.infoAvioes.evento;

	_tcscpy_s(msg.infoAvioes.aeroporto_inicial, _countof(msg.infoAvioes.aeroporto_inicial), data->aeroportos[pos].nome);
	msg.infoAvioes.posicaoX = data->aeroportos[pos].posicaoX;
	msg.infoAvioes.posicaoY = data->aeroportos[pos].posicaoY;

	msg.cmd = ARRIVALSUCESS;

	data->messageData->aviaoMessageUpdateEvent = OpenEventW(EVENT_ALL_ACCESS, FALSE, message.text);
	if (data->messageData->aviaoMessageUpdateEvent == NULL) {
		error(TEXT("OpenEvent failed"));
		return FALSE;
	}
	writeDataToSharedMemory(data->messageData->sharedMessage, &msg, sizeof(mensagem), data->messageData->hMutex, data->messageData->aviaoMessageUpdateEvent);

	for (int i = 0; i < data->max_avioes; i++) {
		for (int j = 0; j < MAX_PASSAGEIROS; j++) {
			if (message.infoAvioes.id == data->arrayAvioes[i].id) {
				if (data->arrayAvioes[i].passagAviao[j].empty == FALSE) {
					mensagem mens;
					mens.cmd = ARRIVALPASS;
					writeDataToPipeAsync(data->arrayAvioes[i].passagAviao[j].hPipeMessage, data->hEvent, &mens, sizeof(mens));
				}
			}
		}
		
	}

	for (int i = 0; i < data->max_avioes; i++) {
		if (message.infoAvioes.id == data->arrayAvioes[i].id) {
			data->arrayAvioes[i].empty = TRUE;
			data->arrayAvioes[i].id = 0;
			_tcscpy_s(data->arrayAvioes[i].aeroporto_destino, _countof(data->arrayAvioes[i].aeroporto_destino), TEXT(""));
			_tcscpy_s(data->arrayAvioes[i].aeroporto_inicial, _countof(data->arrayAvioes[i].aeroporto_inicial), TEXT(""));
			data->arrayAvioes[i].posicaoX = 0;
			data->arrayAvioes[i].posicaoY = 0;
			data->arrayAvioes[i].posicaoXAeroporto_destino = 0;
			data->arrayAvioes[i].posicaoYAeroporto_destino = 0;
			data->arrayAvioes[i].proximoX = 0;
			data->arrayAvioes[i].proximoY = 0;
			for (int j = 0; j < MAX_PASSAGEIROS; j++) {
				data->arrayAvioes[i].passagAviao[j].empty = TRUE;
				data->arrayAvioes[i].passagAviao[j].id = 0;
				_tcscpy_s(data->arrayAvioes[i].passagAviao[j].name, _countof(data->arrayAvioes[i].passagAviao[j].name), TEXT(""));
				_tcscpy_s(data->arrayAvioes[i].passagAviao[j].aeroporto_destino, _countof(data->arrayAvioes[i].passagAviao[j].aeroporto_destino), TEXT(""));
				_tcscpy_s(data->arrayAvioes[i].passagAviao[j].aeroporto_origem, _countof(data->arrayAvioes[i].passagAviao[j].aeroporto_origem), TEXT(""));
			}
			
		}
	}
}

int procuraAeroportoDestino(data* dados, mensagem message) {

	for (int i = 0; i < dados->max_aeroportos; i++) {
		if (_tcscmp(dados->aeroportos[i].nome, message.infoAvioes.aeroporto_destino) == 0) {
			return i;
		}
	}

	return 10;
}


void disconnectAll(data* dados) {

	mensagem message;

	

	for (int i = 0; i < dados->max_avioes; i++) {
		if (dados->arrayAvioes[i].empty == FALSE) {
			dados->messageData->aviaoMessageUpdateEvent =  dados->arrayAvioes[i].evento;
			if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
				error(TEXT("OpenEvent failed"));
				return FALSE;
			}
			
			message.cmd = CONTROL_DISCONNECT;
			writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);

			
		}
	}


	for (int i = 0; i < dados->max_aeroportos; i++) {
		for (int j = 0; j < dados->max_avioes; j++) {
			if (dados->aeroportos[i].arrayAvioes[j].empty == FALSE) {
				dados->messageData->aviaoMessageUpdateEvent = dados->aeroportos[i].arrayAvioes[j].evento;
				if (dados->messageData->aviaoMessageUpdateEvent == NULL) {
					error(TEXT("OpenEvent failed"));
					return FALSE;
				}
				
				message.cmd = CONTROL_DISCONNECT;
				writeDataToSharedMemory(dados->messageData->sharedMessage, &message, sizeof(mensagem), dados->messageData->hMutex, dados->messageData->aviaoMessageUpdateEvent);

				
			}
		}
	}
	

	for (int i = 0; i < MAX_PASSAGEIROS;i++) {
		if (dados->pass[i].empty == FALSE) {
			message.cmd = CONTROL_DISCONNECT;
			writeDataToPipeAsync(dados->pass[i].hPipeMessage, dados->hEvent, &message, sizeof(message));
		}
	}
	
}

void removePlane(data* dados, mensagem msg) {

	for (int i = 0; i < dados->max_aeroportos; i++) {
		for (int j = 0; j < dados->max_avioes; j++) {
			if (dados->aeroportos[i].arrayAvioes[j].id == msg.infoAvioes.id) {
				dados->aeroportos[i].arrayAvioes[j].id = 0;
				dados->aeroportos[i].arrayAvioes[j].empty = TRUE;
				_tcscpy_s(dados->aeroportos[i].arrayAvioes[j].aeroporto_destino, _countof(dados->aeroportos[i].arrayAvioes[j].aeroporto_destino), TEXT(""));
				_tcscpy_s(dados->aeroportos[i].arrayAvioes[j].aeroporto_inicial, _countof(dados->aeroportos[i].arrayAvioes[j].aeroporto_inicial), TEXT(""));
				dados->aeroportos[i].arrayAvioes[j].posicaoX = 0;
				dados->aeroportos[i].arrayAvioes[j].posicaoY = 0;
				dados->aeroportos[i].arrayAvioes[j].posicaoXAeroporto_destino = 0;
				dados->aeroportos[i].arrayAvioes[j].posicaoYAeroporto_destino = 0;
				dados->aeroportos[i].arrayAvioes[j].proximoX = 0;
				dados->aeroportos[i].arrayAvioes[j].proximoY = 0;

				for (int k = 0; k < MAX_PASSAGEIROS; k++) {
					if(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].empty == FALSE){
						
						addPassToAirport(dados->aeroportos[i].arrayAvioes[j].passagAviao[k].id, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_destino, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].aeroporto_origem, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].name, i, dados, dados->aeroportos[i].arrayAvioes[j].passagAviao[k].hPipeMessage);

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
	}


	for (int j = 0; j < dados->max_avioes; j++) {
		if (dados->arrayAvioes[j].id == msg.infoAvioes.id) {
			dados->arrayAvioes[j].id = 0;
			dados->arrayAvioes[j].empty = TRUE;
			_tcscpy_s(dados->arrayAvioes[j].aeroporto_destino, _countof(dados->arrayAvioes[j].aeroporto_destino), TEXT(""));
			_tcscpy_s(dados->arrayAvioes[j].aeroporto_inicial, _countof(dados->arrayAvioes[j].aeroporto_inicial), TEXT(""));
			dados->arrayAvioes[j].posicaoX = 0;
			dados->arrayAvioes[j].posicaoY = 0;
			dados->arrayAvioes[j].posicaoXAeroporto_destino = 0;
			dados->arrayAvioes[j].posicaoYAeroporto_destino = 0;
			dados->arrayAvioes[j].proximoX = 0;
			dados->arrayAvioes[j].proximoY = 0;

			for (int k = 0; k < MAX_PASSAGEIROS; k++) {
				if (dados->arrayAvioes[j].passagAviao[k].empty == FALSE) {

					mensagem message;
					message.cmd = PLANE_DISCONNECT;
					writeDataToPipeAsync(dados->arrayAvioes[j].passagAviao[k].hPipeMessage, dados->hEvent, &message, sizeof(message));

					dados->arrayAvioes[j].passagAviao[k].empty = TRUE;
					dados->arrayAvioes[j].passagAviao[k].id = 0;
					dados->arrayAvioes[j].passagAviao[k].hPipeMessage = NULL;
					_tcscpy_s(dados->arrayAvioes[j].passagAviao[k].name, _countof(dados->arrayAvioes[j].passagAviao[k].name), TEXT(""));
					_tcscpy_s(dados->arrayAvioes[j].passagAviao[k].aeroporto_destino, _countof(dados->arrayAvioes[j].passagAviao[k].aeroporto_destino), TEXT(""));
					_tcscpy_s(dados->arrayAvioes[j].passagAviao[k].aeroporto_origem, _countof(dados->arrayAvioes[j].passagAviao[k].aeroporto_origem), TEXT(""));
					
				}
			}
		}

	}
	
}