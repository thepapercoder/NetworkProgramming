// WSAAsyncSelect_Telnet.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "WSAAsyncSelect_Telnet.h"
#include <WinSock2.h>
#include <stdio.h>

#define MAX_LOADSTRING 100
#define WM_ACCEPT	WM_USER + 1
#define WM_RECV		WM_USER + 2
// Global Variables:
SOCKET* client_sockets = NULL;
int nclient = 0;
bool* client_login = NULL;

HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_WSAASYNCSELECT_TELNET, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WSAASYNCSELECT_TELNET));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WSAASYNCSELECT_TELNET));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_WSAASYNCSELECT_TELNET);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	WSADATA DATA;
	SOCKET s;
	SOCKADDR_IN saddr;
	SOCKADDR_IN caddr;
	int clen = sizeof(caddr);
	SOCKET c;
	char buffer[1024];
	char* welcome = "Hay gui thong tin dang nhap dang: <user> <psw>\n";
	char* login = "Dang nhap thanh cong. Hay gui lenh de thuc thi\n";
	char* invalid = "Sai cu phap dang nhap!\n";
	char line[1024];
	FILE* f = NULL;
	int i = 0;
	switch (message)
	{
	case WM_RECV:
		c = (SOCKET)wParam;
		for (i = 0; i < nclient; i++)
		{
			if (client_sockets[i] == c)
			{
				break;
			}
		}
		memset(buffer, 0, sizeof(buffer));
		recv(c, buffer, sizeof(buffer), 0);
		while (buffer[strlen(buffer) - 1] == '\n' || buffer[strlen(buffer) - 1] == '\r')
		{
			buffer[strlen(buffer) - 1] = '\0';
		}

		if (client_login[i] == false)
		{

			f = fopen("C:\\Temp\\user_psw.txt", "rt");
			while (!feof(f))
			{
				memset(line, 0, sizeof(line));
				fgets(line, sizeof(line), f);
				while (line[strlen(line) - 1] == '\n' || line[strlen(line) - 1] == '\r')
				{
					line[strlen(line) - 1] = '\0';
				}
				if (strcmp(line, buffer) == 0)
				{
					send(c, login, strlen(login), 0);
					client_login[i] = true;
					break;
				}
			}
			fclose(f);
			if (client_login[i] == false)
			{
				send(c, invalid, strlen(invalid), 0);
			}
		}
		else
		{
			char command[1024];
			char filename[1024];
			memset(command, 0, sizeof(command));
			sprintf(command, "%s > C:\\Temp\\%d.txt", buffer, c);
			system(command);
			memset(filename, sizeof(filename), 0);
			sprintf(filename, "C:\\Temp\\%d.txt", c);
			f = fopen(filename, "rt");
			while (!feof(f))
			{
				memset(line, 0, sizeof(line));
				fgets(line, sizeof(line), f);
				send(c, line, strlen(line), 0);
			}
			fclose(f);
		}
		break;
	case WM_ACCEPT:
		s = (SOCKET)wParam;
		c = accept(s, (sockaddr*)&caddr, &clen);
		send(c, welcome, strlen(welcome), 0);
		client_sockets = (SOCKET*)realloc(client_sockets, (nclient + 1) * sizeof(SOCKET));
		client_login = (bool*)realloc(client_login, (nclient + 1) * sizeof(bool));
		client_sockets[nclient] = c;
		client_login[nclient] = false;
		nclient += 1;
		WSAAsyncSelect(c, hWnd, WM_RECV, FD_READ);
		break;
	case WM_CREATE:
		WSAStartup(MAKEWORD(2, 2), &DATA);
		s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		saddr.sin_family = AF_INET;
		saddr.sin_port = htons(8888);
		saddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
		bind(s, (sockaddr*)&saddr, sizeof(saddr));
		listen(s, 10);
		WSAAsyncSelect(s, hWnd, WM_ACCEPT, FD_ACCEPT);
		break;
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
