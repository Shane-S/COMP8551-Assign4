#include "ControlDlgProc.h"
#include "Utils.h"
#include <tchar.h>

#define FILENAME_SIZE 8192

// Acceptable image file formats
static const LPTSTR szFilter = TEXT("Image files (*.BMP; *.GIF; *.ICO; *.JPEG; *.JPG; *.PNG;*.TIFF)\0*.BMP;*.GIF;*.ICO;*.JPEG;*.JPG;*.PNG;*.TIFF\0\0");

///<summary>Opens a file chooser dialog with the specified filter and stores the resulting file name in szFileName.</summary>
///
///<param name="hwndOwner">A handle to the window that owns this file chooser dialog.</param>
///<param name="filters">The set of filters that will limit file choices. Pass NULL to allow any files to be chosen.</param>
///<param name="szFilename">Stores the name of the selected file, if any. This must be allocated before calling the function.</param>
///<param name="dwBufSize">The size (in characters) of the szFileName buffer.</param>
///<return>Whether getting a file name succeeded. Call CommDlgExtendedError to determine whether an error occurred or if the user just cancelled.</return>
BOOL OpenFileDlg(HWND hwndOwner, const LPTSTR filters, TCHAR *szFileName, DWORD dwBufSize)
{
	OPENFILENAME	ofn;
	DWORD			dwFlags = OFN_EXPLORER | OFN_FORCESHOWHIDDEN | OFN_NONETWORKBUTTON | OFN_PATHMUSTEXIST;
	TCHAR			*szText;

	dwFlags |= OFN_PATHMUSTEXIST;
	szText = TEXT("Select the image to blend.");

	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwndOwner;
	ofn.hInstance = NULL;
	ofn.Flags = dwFlags;
	ofn.lpstrFilter = filters;
	ofn.lpstrCustomFilter = NULL;
	ofn.nMaxCustFilter = 0;
	ofn.nFilterIndex = 0;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = dwBufSize;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = NULL;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrTitle = szText;
	ofn.lpstrDefExt = NULL;
	ofn.lCustData = 0;
	ofn.lpfnHook = NULL;
	ofn.lpTemplateName = NULL;
	ofn.FlagsEx = 0;

	return GetOpenFileName(&ofn);
}

///<summary>Picks a file and sets the specified textbox's text to the file path.</summary>
///<param name="hwndOpener">The HWND of the window that opened/owns the dialog box.</param>
///<param name="hwndText">The HWND of the textbox control that will display the name.</param>
VOID ChooseFile(HWND hwndOpener, HWND hwndText) {
	TCHAR szFileName[FILENAME_SIZE] = { 0 };
	DWORD dwError = 0;
	if (!OpenFileDlg(hwndOpener, szFilter, szFileName, FILENAME_SIZE) && (dwError == CommDlgExtendedError()) != 0) {
		_ftprintf(stderr, TEXT("Error: %s\n"), CommDlgExtendedErrorToString(dwError));
	} else {
		SetWindowText(hwndText, szFileName);
	}
}

INT_PTR CALLBACK ControlDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Acceptable file formats for the images
	switch (uMsg)
	{
	case WM_CLOSE: /* there are more things to go here, */
		DestroyWindow(hDlg);

	case WM_DESTROY:
		PostQuitMessage(0);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_IMG_BROWSE:
			ChooseFile(hDlg, GetDlgItem(hDlg, IDC_IMAGE_EDIT));
			return TRUE;
		case IDC_KERNEL_BROWSE:
			ChooseFile(hDlg, GetDlgItem(hDlg, IDC_KERNEL_EDIT));
			return TRUE;
		}
	default:
		return FALSE;
	}
}