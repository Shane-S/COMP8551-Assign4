#include <Windows.h>

// Sent to the parent window to notify that the image window has closed
#define WM_IMGWINDOWCLOSED (WM_APP + 1)

#define IMG_GWLP_OFFSCREENBUFFER (0)

///<summary></summary>
BOOL InitImageWindow(HWND* hwnd, HWND parent, HINSTANCE hInstance, TCHAR* caption);

LRESULT CALLBACK ImageWndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
extern const TCHAR* szImgWindowClassName;