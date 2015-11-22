#include <Windows.h>

LRESULT CALLBACK NameWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC             hdc;
	PAINTSTRUCT     ps;
	RECT            rect;
	int             count = 0;

	switch (message)
	{
	case WM_CREATE:
		//PlaySound(TEXT("hellowin.wav"), NULL, SND_FILENAME | SND_ASYNC);

		if ((int)((CREATESTRUCT *)lParam)->lpCreateParams != 1)
		{
			count++;
			SetClassLongPtr(hwnd, 0, (long)count);
		}

		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);

		DrawText(hdc, TEXT("Shane Spoor"), -1, &rect,
			DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint(hwnd, &ps);
		return 0;

	case WM_DESTROY:
		count = GetClassLongPtr(hwnd, 0);
		count--;
		if (count >= 0)
		{
			SetClassLongPtr(hwnd, 0, (long)count);
		}
		else
		{
			PostQuitMessage(0);
		}
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}