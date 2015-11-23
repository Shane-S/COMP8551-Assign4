#include <windows.h>
#include <cstdio>
#include "ControlDlgProc.h"
#include "resource.h"
#include "Utils.h"
#include "ImageWindow.h"
#include "WICImagingFactory.h"

LRESULT CALLBACK NameWndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK SetWndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR szCmdLine, int iCmdShow)
{
	// We're using a dialog as the main window for convenience in creating the UI
	HWND hDlg;
	hDlg = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, ControlDlgProc, 0);
	ShowWindow(hDlg, iCmdShow);

	// Register the window class for windows which will contain images
	WNDCLASS imageWndClass;
	imageWndClass.style       = CS_HREDRAW | CS_VREDRAW;
	imageWndClass.lpfnWndProc = ImageWndProc;
	imageWndClass.cbClsExtra  = sizeof(HWND);
	imageWndClass.cbWndExtra = 0;
	imageWndClass.hInstance = hInstance;
	imageWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	imageWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	imageWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	imageWndClass.lpszMenuName = NULL;
	imageWndClass.lpszClassName = szImgWindowClassName;

	if (!RegisterClass(&imageWndClass)) {
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szImgWindowClassName, MB_ICONERROR);
		return 0;
	}

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
	return msg.wParam;

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
