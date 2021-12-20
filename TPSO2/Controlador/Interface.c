
#include "Interface.h"
int x, y;
HDC hdc = NULL, auxDC = NULL;
HBRUSH bg = NULL;
HBITMAP auxBM = NULL;
int nX = 0, nY = 0;
HBITMAP hBitmap;
HBITMAP hbackground;
HBITMAP haviao;
data* Dados;

INT_PTR CALLBACK Airport(HWND hWnd, UINT messg, WPARAM wParam, LPARAM lParam) {

	data* dados = (data*)GetWindowLongPtr(hWnd, GWL_USERDATA);
	TCHAR text[50];
	BOOL fSuccess;
	int x, y;
	

	switch (messg) {
	case WM_INITDIALOG:
		SetWindowLongPtr(hWnd, GWL_USERDATA, lParam);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
			case IDOK:
				GetDlgItemText(hWnd, IDC_EDIT2, text, 50);
				for (int i = 0; i < Dados->max_aeroportos; i++) {
					break;
					if (_tcscmp(Dados->aeroportos[i].nome, text) == 0) {
						MessageBox(hWnd, TEXT("Já existe um aeroporto com esse nome"),
							TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
						break;
					}
				}
				if (_tcslen(text) == 0) {
					MessageBox(hWnd, TEXT("O nome do aeroporto não pode ser nulo"),
						TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
					break;
				}
				x = GetDlgItemInt(hWnd, IDC_EDIT4, &fSuccess, TRUE);
				if (x < 0 || x > 1000) {
					MessageBox(hWnd, TEXT("A coordenada X deve situar-se no intervalo [0,1000]"),
						TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
					break;
				}
				y = GetDlgItemInt(hWnd, IDC_EDIT3, &fSuccess, TRUE);
				if (y < 0 || y > 1000) {
					MessageBox(hWnd, TEXT("A coordenada Y deve situar-se no intervalo [0,1000]"),
						TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
					break;

				}
				if (x == NULL || y == NULL) {
					MessageBox(hWnd, TEXT("As coordenadas não podem ser nulas!"),
						TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
					break;
				}
				if (averiguaCoordenadas(Dados, x, y) == 0) {
					MessageBox(hWnd, TEXT("Ambas as coordenadas têm de possuir uma distância de 10 posições com outros Aeroportos"),
						TEXT("Missing fields"), MB_OK | MB_ICONINFORMATION);
					break;
				}
				
				
				
				cria(Dados, text, x, y);
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
			case IDCANCEL:
				EndDialog(hWnd, LOWORD(wParam));
				return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow, data* dados) {
	HINSTANCE hInst = hInstance; // Store instance handle in our global variable

	dados->hInst = hInstance;
	Dados = dados;

	HWND hWnd = CreateWindow(TEXT("Aeroportos") , TEXT("So2"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 1024, HWND_DESKTOP, NULL, hInstance, NULL);

	if (!hWnd) {
		
		return FALSE;
	}
	

	dados->hWnd = hWnd;

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam) {

	data* dados = (data*)GetWindowLongPtr(hWnd, 0);
	HMENU hMainMenu;
	static BITMAP bmAeroporto;
	static HDC hdcAeroporto;
	static BITMAP bmbackground;
	static HDC hdcbackground;
	static BITMAP bmAviao;
	static HDC hdcAviao;
	int x = 0, y = 0;
	TCHAR text[40];
	HWND d;

	SetWindowLongPtr(hWnd, 0, (LONG_PTR)&dados);
	switch (message) {
	case WM_CREATE:

		
		
		hBitmap = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP5), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(hBitmap, sizeof(bmAeroporto), &bmAeroporto);
		hdcAeroporto = CreateCompatibleDC(hdc);
		SelectObject(hdcAeroporto, hBitmap);
		ReleaseDC(hWnd, hdc);
		

		hbackground = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP4), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(hbackground, sizeof(bmbackground), &bmbackground);
		hdcbackground = CreateCompatibleDC(hdc);
		SelectObject(hdcbackground, hbackground);
		ReleaseDC(hWnd, hdc);

		haviao = (HBITMAP)LoadImage((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDB_BITMAP10), IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
		hdc = GetDC(hWnd);
		GetObject(haviao, sizeof(bmAviao), &bmAviao);
		hdcAviao = CreateCompatibleDC(hdc);
		SelectObject(hdcAviao, haviao);
		ReleaseDC(hWnd, hdc);

		bg = CreateSolidBrush(RGB(255, 128, 128));
		nX = GetSystemMetrics(SM_CXSCREEN);
		nY = GetSystemMetrics(SM_CYSCREEN);

		// PREPARA 'BITMAP' E ASSOCIA A UM 'DC' EM MEMORIA...
		hdc = GetDC(hWnd);
		auxDC = CreateCompatibleDC(hdc);
		auxBM = CreateCompatibleBitmap(hdc, nX, nY);
		SelectObject(auxDC, auxBM);
		SelectObject(auxDC, GetStockObject(BLACK_BRUSH));
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
		ReleaseDC(hWnd, hdc);

		SetTextColor(auxDC, RGB(255, 255, 255));
		SetBkColor(auxDC, RGB(0, 0, 0));
		hMainMenu = LoadMenu((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDR_MENU1));
		SetMenu(hWnd, hMainMenu);
		break;
	case WM_COMMAND: {
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId) {
		case ID_FILE_AEROPORTO:
			MessageBeep(MB_OK);
			DialogBoxParam((HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), MAKEINTRESOURCE(IDD_DIALOG10), hWnd, Airport, (LPARAM)dados);
			break;
		case ID_FILE_ATIVARAVIAO1:
			MessageBeep(MB_OK);
			MessageBox(NULL, TEXT("Aviões Ativos"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
			Dados->suspenso = FALSE;
			break;
		case ID_FILE_SUSPENDERAVIAO1:
			MessageBeep(MB_OK);
			MessageBox(NULL, TEXT("Aviões Suspensos"), TEXT("Information"), MB_OK | MB_ICONINFORMATION);
			Dados->suspenso = TRUE;
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	 break;
	case WM_CLOSE:
		if (MessageBox(hWnd, TEXT("Exit?"), TEXT("Exit"), MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) == IDYES) {
			DestroyWindow(hWnd);
		}
		break;
	case WM_MOUSEMOVE:
		InvalidateRect(hWnd, NULL, TRUE); 
		UpdateWindow(hWnd);
		break;
	case WM_ERASEBKGND:
		return TRUE;
	case WM_PAINT:
		PAINTSTRUCT ps;
		PatBlt(auxDC, 0, 0, nX, nY, PATCOPY);
		SetStretchBltMode(auxDC, BLACKONWHITE);
		StretchBlt(auxDC,0, 0, 1280, 1024, hdcbackground, 0, 0, bmbackground.bmWidth, bmbackground.bmHeight, SRCCOPY);
		
		for (int i = 0; i < Dados->max_aeroportos; i++) {
			if (!Dados->aeroportos[i].vazio) {
				StretchBlt(auxDC, Dados->aeroportos[i].posicaoX,Dados->aeroportos[i].posicaoY, 35, 35,hdcAeroporto, 0, 0, bmAeroporto.bmWidth, bmAeroporto.bmHeight, SRCCOPY);
		
				
			}
		}

		for (int i = 0; i < Dados->max_avioes; i++) {
			if (!Dados->arrayAvioes[i].empty) {
				StretchBlt(auxDC, Dados->arrayAvioes[i].posicaoX, Dados->arrayAvioes[i].posicaoY, 25, 25, hdcAviao, 0, 0, bmAviao.bmWidth, bmAviao.bmHeight, SRCCOPY);
				
			}
		}

		
		hdc = BeginPaint(hWnd, &ps);
	
		BitBlt(hdc, 0, 0, nX, nY, auxDC, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);

		UpdateWindow(hWnd);
		SendMessage(hWnd, WM_DISPLAYCHANGE, NULL, NULL);
		break;
	case WM_DESTROY:
		disconnectAll(Dados);
		MessageBox(NULL, TEXT("A encerrar o sistema"), TEXT("Exit"), MB_OK | MB_ICONINFORMATION);
		DeleteObject(hBitmap);
		DeleteObject(hbackground);
		DeleteObject(haviao);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
	WNDCLASSEXW wcex;

	TCHAR nome[100] = TEXT("Aeroportos");

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.hInstance = hInstance;
	wcex.style = CS_DBLCLKS | CS_PARENTDC;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = sizeof(data*);
	
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICONAVIAO));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = CreateSolidBrush(0x00000000);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_CONTROL);
	wcex.lpszClassName = nome;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICONAVIAO));

	return RegisterClassExW(&wcex);
}


