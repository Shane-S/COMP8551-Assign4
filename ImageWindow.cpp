#include <Windows.h>
#include "ImageWindow.h"
#include "ImageBlender.h"

const TCHAR* szImgWindowClassName = TEXT("Image");

BOOL InitImageWindow(HWND* hwnd, HWND parent, HINSTANCE hInstance, TCHAR* caption) {
	DWORD dwStyle = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU);
	*hwnd = CreateWindow(szImgWindowClassName, // window class name
						 caption,              // window caption
						 dwStyle,              // window style
						 0,                    // initial x position
						 0,                    // initial y position
						 200,                  // initial x size
						 200,                  // initial y size
						 parent,               // parent window handle
						 NULL,                 // window menu handle
						 hInstance,            // program instance handle
						 NULL);                // creation parameters
	if (*hwnd == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	// Null out the pointer so that we can realloc it later
	DWORD dwError;
	LPOFFSCREENBUFFER info = (LPOFFSCREENBUFFER)calloc(1, sizeof(OFFSCREENBUFFER));
	if (SetWindowLongPtr(*hwnd, IMG_GWLP_OFFSCREENBUFFER, (LONG_PTR)info) == 0 && (dwError = GetLastError() != 0)) {
		free(info);
		DestroyWindow(*hwnd);
		return FALSE;
	}
	return TRUE;
}

static void Win32DisplayBufferInWindow(HDC deviceContext,
	LPOFFSCREENBUFFER buffer,
	int windowWidth, int windowHeight, int x, int y, int width, int height)
{
	// TODO: Aspect ratio correction
	DWORD dwError = StretchDIBits(deviceContext,
		0, 0, windowWidth, windowHeight,
		0, 0, buffer->width, buffer->height,
		buffer->pixels,
		&buffer->info,
		DIB_RGB_COLORS, SRCCOPY);
}

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT     ps;
	RECT            rect;

	LPOFFSCREENBUFFER buf = (LPOFFSCREENBUFFER)GetWindowLongPtr(hwnd, IMG_GWLP_OFFSCREENBUFFER);

	switch (message)
	{
	case WM_CREATE:
		return 0;

	case WM_PAINT: {
	
		// Render the image to a GDI device context
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		int x = ps.rcPaint.left;
		int y = ps.rcPaint.top;
		int height = ps.rcPaint.bottom - ps.rcPaint.top;
		int width = ps.rcPaint.right - ps.rcPaint.left;

		DWORD winWidth = rect.right - rect.left;
		DWORD winHeight = rect.bottom - rect.top;

		Win32DisplayBufferInWindow(hdc, buf, winWidth, winHeight, x, y, width, height);
		EndPaint(hwnd, &ps);
		return 0;
	}
	
	case WM_CLOSE:
		SendMessage((HWND)GetWindowLongPtr(hwnd, GWLP_HWNDPARENT), WM_IMGWINDOWCLOSED, 0, 0);
		DestroyWindow(hwnd);
		return 0;

	case WM_DESTROY:
		// Send destroy message to other windows or notify them somehow (SetClassLongPtr, maybe?)
		// and also notify the dialog box
		free(buf->pixels);
		free(buf);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}