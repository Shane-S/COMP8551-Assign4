#include <windows.h>
#include <cstdio>
#include "ControlDlgProc.h"
#include "resource.h"
#include "Utils.h"
#include "ImageWindow.h"
#include "ImageBlender.h"
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
	WNDCLASSEX imageWndClass;
	imageWndClass.style       = CS_HREDRAW | CS_VREDRAW;
	imageWndClass.lpfnWndProc = ImageWndProc;
	imageWndClass.cbClsExtra  = 0;
	imageWndClass.cbWndExtra = sizeof(OFFSCREENBUFFER);
	imageWndClass.cbSize = sizeof(WNDCLASSEX);
	imageWndClass.hInstance = hInstance;
	imageWndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	imageWndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	imageWndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	imageWndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	imageWndClass.lpszMenuName = NULL;
	imageWndClass.lpszClassName = szImgWindowClassName;

	if (!RegisterClassEx(&imageWndClass)) {
		DWORD dwError = GetLastError();
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
}
