#include <windows.h>
#include "ControlDlgProc.h"
#include "resource.h"
#include "Utils.h"

LRESULT CALLBACK NameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SetWndProc(HWND, UINT, WPARAM, LPARAM);

/**
 * Initialises the class members.
 *
 * @param wndClass    Pointer to the WNDCLASS to be initialised.
 * @param hInstnace   Handle to the current window.
 * @param lpfnWndProc Handle to the class's WndProc function.
 * @param szAppName   The name for this class.
 */
void initClass(WNDCLASS* wndClass, WNDPROC lpfnWndProc, HINSTANCE hInstance, TCHAR szAppName[])
{
	wndClass->style = CS_HREDRAW | CS_VREDRAW;
	wndClass->lpfnWndProc = lpfnWndProc;
	wndClass->cbClsExtra = sizeof(int);
	wndClass->cbWndExtra = 0;
	wndClass->hInstance = hInstance;
	wndClass->hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndClass->hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass->hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndClass->lpszMenuName = NULL;
	wndClass->lpszClassName = szAppName;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// We're using a dialog as the main window for convenience in creating the UI
	HWND hDlg;
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, ControlDlgProc, 0);
	ShowWindow(hDlg, iCmdShow);

	BOOL ret;
	MSG msg;
	while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
		if (ret == -1) /* error found */
			return -1;

		if (!IsDialogMessage(hDlg, &msg)) {
			TranslateMessage(&msg); /* translate virtual-key messages */
			DispatchMessage(&msg); /* send it to dialog procedure */
		}
	}
	//static TCHAR szNameClassAppName[] = TEXT("Name");
	//static TCHAR szSetClassAppName[] = TEXT("Set");
	//HWND         hwnd;
	//HWND         hwnd2;
	//HWND         hwnd3;
	//HWND         hwnd4;
	//MSG          msg;
	//WNDCLASS     nameWndClass;
	//WNDCLASS     setWndClass;

	//initClass(&nameWndClass, NameWndProc, hInstance, szNameClassAppName);
	//initClass(&setWndClass, SetWndProc, hInstance, szSetClassAppName);

	//if (!RegisterClass(&nameWndClass))
	//{
	//	MessageBox(NULL, TEXT("This program requires Windows NT!"),
	//		szNameClassAppName, MB_ICONERROR);
	//	return 0;
	//}

	//if (!RegisterClass(&setWndClass))
	//{
	//	MessageBox(NULL, TEXT("This program requires Windows NT!"),
	//		szSetClassAppName, MB_ICONERROR);
	//	return 0;
	//}

	//hwnd = CreateWindow(szNameClassAppName, // window class name
	//	TEXT("Shane Spoor"),       // window caption
	//	WS_OVERLAPPEDWINDOW,        // window style
	//	0,                          // initial x position
	//	0,                          // initial y position
	//	200,                        // initial x size
	//	200,                        // initial y size
	//	NULL,                       // parent window handle
	//	NULL,                       // window menu handle
	//	hInstance,                  // program instance handle
	//	NULL);                      // creation parameters

	//hwnd2 = CreateWindow(szNameClassAppName, // window class name
	//	TEXT("Shane Spoor"),       // window caption
	//	WS_OVERLAPPEDWINDOW,        // window style
	//	200,                        // initial x position
	//	0,                          // initial y position
	//	200,                        // initial x size
	//	200,                        // initial y size
	//	NULL,                       // parent window handle
	//	NULL,                       // window menu handle
	//	hInstance,                  // program instance handle
	//	NULL);                      // creation parameters

	//hwnd3 = CreateWindow(szSetClassAppName, // window class name
	//	TEXT("Set 3O"),            // window caption
	//	WS_OVERLAPPEDWINDOW,        // window style
	//	0,                          // initial x position
	//	200,                        // initial y position
	//	200,                        // initial x size
	//	200,                        // initial y size
	//	NULL,                       // parent window handle
	//	NULL,                       // window menu handle
	//	hInstance,                  // program instance handle
	//	NULL);                      // creation parameters

	//hwnd4 = CreateWindow(szSetClassAppName, // window class name
	//	TEXT("Set 3O"),            // window caption
	//	WS_OVERLAPPEDWINDOW,        // window style
	//	200,                        // initial x position
	//	200,                        // initial y position
	//	200,                        // initial x size
	//	200,                        // initial y size
	//	NULL,                       // parent window handle
	//	NULL,                       // window menu handle
	//	hInstance,                  // program instance handle
	//	NULL);                      // creation parameters

	//ShowWindow(hwnd, iCmdShow);
	//UpdateWindow(hwnd);

	//ShowWindow(hwnd2, iCmdShow);
	//UpdateWindow(hwnd2);

	//ShowWindow(hwnd3, iCmdShow);
	//UpdateWindow(hwnd3);

	//ShowWindow(hwnd4, iCmdShow);
	//UpdateWindow(hwnd4);

	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	TranslateMessage(&msg);
	//	DispatchMessage(&msg);
	//}

	//return msg.wParam;
}
