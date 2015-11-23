#ifndef CONTROL_DLG_PROC_H
#define CONTROL_DLG_PROC_H

#include <Windows.h>
#include "Utils.h"
#include "resource.h"

#define GWLP_BLENDSETTINGS_OFF 0
#define GWLP_BLENDRESULT_OFF   sizeof(BLENDSETTINGS)

INT_PTR CALLBACK ControlDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
#endif