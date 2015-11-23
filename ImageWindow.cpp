#include <Windows.h>

const TCHAR* szImgWindowClassName = TEXT("Image");

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT     ps;
	RECT            rect;

	switch (message)
	{
	case WM_CREATE:
		// Cast CREATESTRUCT to bitmap
		// Save bitmap as class long ptr
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		// Send destroy message to other windows or notify them somehow (SetClassLongPtr, maybe?)
		// and also notify the dialog box
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}