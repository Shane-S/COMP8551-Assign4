#ifndef CONTROL_DLG_PROC_H
#define CONTROL_DLG_PROC_H

#include <Windows.h>
#include "Utils.h"
#include "resource.h"

#define BLEND_SERIAL 0
#define BLEND_MMX    1
#define BLEND_SSE    2

typedef struct _BLENDSETTINGS {
	TCHAR* szImageFile;  // The main image file
	TCHAR* szKernelFile; // The kernel image file
	double blendFactor;  // The blend factor (between 0 and 1)
	BYTE   blendType;    // How to perform the blending
} BLENDSETTINGS, *LPBLENDSETTINGS;

INT_PTR CALLBACK ControlDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif