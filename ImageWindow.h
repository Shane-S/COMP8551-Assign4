#include <Windows.h>

// Sent to the parent window to notify that the image window has closed
#define WM_IMGWINDOWCLOSED (WM_APP + 1)

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

extern const TCHAR* szImgWindowClassName;