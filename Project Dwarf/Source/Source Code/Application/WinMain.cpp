// Source.cpp : Defines the entry point for the application.
//

#define VLD_ON
#ifndef VLD_ON
#include <vld.h>
#endif

#include <ctime>

#include "../Application/stdafx.h"
#include "WinMain.h"
#include "../Core/Core.h"
#include "../Utilities/TimeManager.h"

#define MAX_LOADSTRING 100
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768
#define FPS

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
HWND hWnd;										// the main window
bool Running = true;							//bool for game loop
Core * pCoreApp = nullptr;
wstring fps_string;
TITLEBARINFO title_info;


// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


#ifdef FPS
void				FPSCounter(HINSTANCE hInstance);
#define FPS_FUNC(x) FPSCounter(x)
#else
#define FPS_FUNC(x) 
#endif


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	//Seed Rand
	srand(UINT(time(nullptr)));
	MSG msg;
	msg.hwnd = hWnd;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SOURCE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SOURCE));

	pCoreApp = new Core;
	if (!pCoreApp->Initialize(hWnd, hInst, WINDOW_WIDTH, WINDOW_HEIGHT, true))
	{
		pCoreApp->Shutdown();
		delete pCoreApp;
		pCoreApp = nullptr;

		//Initialize did not work properly
		msg.wParam = -1;
		Running = false;
	}

	// Main message loop:
	while (Running)
	{
		//if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		if (PeekMessageW(&msg, hWnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (pCoreApp)
		{
			pCoreApp->Update();
			FPS_FUNC(hInst);
		}
	}

	SAFE_SHUTDOWN(pCoreApp);

	return (int)msg.wParam;
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

	wcex.style = NULL;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SOURCE));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_SOURCE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
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
	case WM_GETTITLEBARINFOEX:
		GetTitleBarInfo(hWnd, &title_info);
		break;
	case WM_SETTEXT:
		//SetWindowText(hWnd, fps_string.c_str());
		DefWindowProc(hWnd, WM_SETTEXT, wParam, lParam);
		break;
	case WM_CLOSE:
		Running = false;
		DestroyWindow(hWnd);
		break;
	case WM_NULL:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		//// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_QUIT:
		Running = false;
		SAFE_SHUTDOWN(pCoreApp);
		SendMessage(hWnd, WM_CLOSE, NULL, NULL);
		break;
	case WM_DESTROY:
		Running = false;
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

#ifdef FPS
//FPS Counter
void FPSCounter(HINSTANCE hInstance)
{
	static int frames = 0;
	static double starttime = CTimeManager::GetElapsedTimeD();
	static float fps = 0.0f;

	if (CTimeManager::GetElapsedTimeD() - starttime > 0.25 && frames > 10)
	{
		fps_string.clear();
		fps_string.reserve(MAX_LOADSTRING);
		fps_string = szTitle;
		fps = float((double)frames / double(CTimeManager::GetElapsedTimeD() - starttime));
		starttime = CTimeManager::GetElapsedTimeD();
		frames = 0;
		fps_string += L" FPS: ";
		wchar_t temp[32];
		_snwprintf_s(temp, 32u, L"%f", fps);
		fps_string += temp;
		SendMessage(hWnd, WM_SETTEXT, NULL, (LPARAM)fps_string.data());
	}


	frames++;
}
#endif // !FPS

