#include <wincodec.h>
#include <tchar.h>
#include "ControlDlgProc.h"
#include "Utils.h"
#include "ImageBlender.h"

#define FILENAME_SIZE MAX_PATH

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
	DWORD dwError = 0;
	TCHAR szFileName[FILENAME_SIZE] = { 0 };
	if (!OpenFileDlg(hwndOpener, szFilter, szFileName, FILENAME_SIZE) && (dwError == CommDlgExtendedError()) != 0) {
		_ftprintf(stderr, TEXT("Error: %s\n"), CommDlgExtendedErrorToString(dwError));
	} else {
		SetWindowText(hwndText, szFileName);
	}
}

///<summary>Checks whether the given file is a valid image file.</summary>
///<param name="szFile">The path to the file to check.</param>
///<returns>Whether the file is valid.</summary>
BOOL IsValidImageFile(LPTSTR szFile) {
	TCHAR szFileExt[8] = { 0 };   // The file extension of the actual file
	DWORD dwExtSize;              // Size of the file extension
	TCHAR szFilterExt[8] = { 0 }; // The current filter extension being compared
	BOOL  bIsValidExt = FALSE;

	TCHAR* curChar;
	TCHAR period = TEXT('.');
	TCHAR* lastPeriod = NULL;
	for (curChar = szFile; *curChar != 0; curChar++) {
		if (*curChar == period) lastPeriod = curChar;
	}

	// No file extension? Definitely not valid then
	if (lastPeriod == NULL) return FALSE;

	// Extension size of 0 or greater than 7? Never heard of that format...
	dwExtSize = (DWORD)(curChar - lastPeriod - 1);
	if (!dwExtSize || dwExtSize > 7) {
		return FALSE;
	}

	// Save the extension and convert it to uppercase for comparison with our filter
	_tcsnccpy_s(szFileExt, lastPeriod + 1, dwExtSize + 1); // + 1 will copy the null terminator too
	for (curChar = szFileExt; *curChar != 0; curChar++) {
		*curChar = _totupper(*curChar);
	}
	
	// Yep, I'm re-using variables
	// Skip the description part of our filter to get to the actual extensions
	for (curChar = szFilter; *curChar != 0; curChar++);
	curChar += 3; // Skip to first character of the first extension
	lastPeriod = curChar - 1;

	// Compare each valid file extension against the one for our image
	for (; *curChar != 0; curChar++) {
		if (*curChar == TEXT(';')) {
			dwExtSize = (DWORD)(curChar - lastPeriod - 1);
			_tcsnccpy_s(szFilterExt, lastPeriod + 1, dwExtSize);
			szFilterExt[dwExtSize + 1] = 0;
			
			if (_tccmp(szFileExt, szFilterExt) == 0) {
				bIsValidExt = TRUE;
				break;
			}

			curChar += 3; // Skip the "*." after the semicolon in the list
			lastPeriod = curChar - 1;
		}
	}

	// Final comparison if there's no trailing semicolon
	if (*(curChar - 1) != TEXT(';') && !bIsValidExt) {
		dwExtSize = (DWORD)(curChar - lastPeriod - 1);
		_tcsnccpy_s(szFilterExt, lastPeriod + 1, dwExtSize);
		szFilterExt[dwExtSize + 1] = 0;
		if (_tccmp(szFileExt, szFilterExt) == 0) {
			bIsValidExt = TRUE;
		}
	}

	if (!bIsValidExt) return FALSE;

	// Finally, check if the file actually exists
	DWORD dwAttrib = GetFileAttributes(szFile);
	return dwAttrib != INVALID_FILE_ATTRIBUTES;
}

INT_PTR CALLBACK ControlDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// These will be garbage in WM_INITDIALOG, but we're not using them there anyway, so who cares
	LPBLENDSETTINGS settings = (LPBLENDSETTINGS)GetWindowLongPtr(hDlg, GWLP_USERDATA);
	LPBLENDRESULT results    = (LPBLENDRESULT)(settings + 1);
	HWND *imgWindows = (HWND*)(results + 1);

	switch (uMsg)
	{
	case WM_INITDIALOG: {
		// Check the serial radio button by default
		HWND hwndSerialRadioBtn = GetDlgItem(hDlg, IDC_RADIO_SERIAL);
		SendMessage(hwndSerialRadioBtn, BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
		
		// Allocate space for BLENDSETTINGS, BLENDRESULT and the HWNDs to display the images (one for the image, one for the kernel, and one for the blend result)
		LPVOID userData = (LPVOID)malloc(sizeof(BLENDSETTINGS) + sizeof(BLENDRESULT) + sizeof(HWND) * 3);

		// Allocate space for the file names
		LPBLENDSETTINGS blendSettings = (LPBLENDSETTINGS)userData;
		blendSettings->lpszImageFile = (TCHAR*)malloc(sizeof(TCHAR) * FILENAME_SIZE);
		blendSettings->lpszKernelFile = (TCHAR*)malloc(sizeof(TCHAR) * FILENAME_SIZE);

		LPBLENDRESULT blendResult = (LPBLENDRESULT)(blendSettings + 1);
		blendResult->hwndNotifyWindow = hDlg;
		blendResult->pixels[BLENDRESULT_IMAGE] = NULL;
		blendResult->pixels[BLENDRESULT_KERNEL] = NULL;
		blendResult->pixels[BLENDRESULT_BLENDED] = NULL;

		// Initialise the display windows
		HWND *displayWindows = (HWND*)(blendResult + 1);
		displayWindows[0] = (HWND)INVALID_HANDLE_VALUE;
		displayWindows[1] = (HWND)INVALID_HANDLE_VALUE;
		displayWindows[2] = (HWND)INVALID_HANDLE_VALUE;

		SetWindowLongPtr(hDlg, GWLP_USERDATA, (LONG_PTR)userData);

		return TRUE;
	}
	case WM_BLENDFINISHED: {
		DWORD dwError = wParam;
		switch (dwError) {
		case BLEND_GENERAL_FAILURE:
			MessageBox(NULL, TEXT("The images could not be blended for some reason. Please try again later."), TEXT("General Failure"), MB_ICONERROR);
			return TRUE;
		case BLEND_KERNEL_2LARGE:
			MessageBox(NULL, TEXT("The kernel image was too large. Its maximum dimensions are those of the image to blend. Please select another kernel."),
				TEXT("Kernel Too Large"), MB_ICONERROR);
			return TRUE;
		}

		// Set the amount of time taken
		HWND hwndTimeMessage = GetDlgItem(hDlg, IDC_PROC_TIME_TXT);
		TCHAR buf[128] = { 0 };
		_sntprintf_s(buf, _countof(buf), TEXT("Processing Time (ms): %.3f"), results->processTime);
		SetWindowText(hwndTimeMessage, buf);

		// Re-enable the start button
		EnableWindow(GetDlgItem(hDlg, IDSTART), TRUE);
		return TRUE;
	}

	case WM_CLOSE: /* there are more things to go here, */
		DestroyWindow(hDlg);
		
	case WM_DESTROY:
		// TODO: Destroy the child windows and tell them to free any memory they have

		// Free the settings object
		free(settings->lpszImageFile);
		free(settings->lpszKernelFile);
		
		// Free the results object
		//free(results->imageBitmap)
		//free(results->kernelBitmap)
		//free(results->blendedBitmap)
		//free(results);

		// settings is actually the pointer to the whole buffer, so we need to free it last
		free(settings);
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

		case IDSTART: {
			TCHAR szBlendFactor[64] = { 0 };
			BYTE blendType;
			BOOL bIsSerial = SendMessage(GetDlgItem(hDlg, IDC_RADIO_SERIAL), BM_GETCHECK, 0, 0) == BST_CHECKED;
			BOOL bIsMMX = SendMessage(GetDlgItem(hDlg, IDC_RADIO_MMX), BM_GETCHECK, 0, 0) == BST_CHECKED;
			
			// Try to get the blend factor
			GetWindowText(GetDlgItem(hDlg, IDC_BLEND), szBlendFactor, 64);
			if (_stscanf_s(szBlendFactor, TEXT("%lf"), &settings->blendFactor) != 1 || settings->blendFactor < 0 || settings->blendFactor > 1) {
				MessageBox(hDlg, TEXT("Blend factor must be between 0 and 1."), TEXT("Invalid Blend Factor"), MB_ICONERROR);
				return TRUE;
			}

			// Set the blend procedure
			if (bIsSerial)   settings->blendType = BLEND_SERIAL;
			else if (bIsMMX) settings->blendType = BLEND_MMX;
			else             settings->blendType = BLEND_SSE;

			// Get the file names and check to make sure they're valid
			GetWindowText(GetDlgItem(hDlg, IDC_IMAGE_EDIT), settings->lpszImageFile, FILENAME_SIZE);
			GetWindowText(GetDlgItem(hDlg, IDC_KERNEL_EDIT), settings->lpszKernelFile, FILENAME_SIZE);

			if (!IsValidImageFile(settings->lpszImageFile)) {
				MessageBox(hDlg, TEXT("Invalid image file. Please select an existing file file with one of the specified extensions."), TEXT("Invalid Image File"), MB_ICONERROR);
			} else if (!IsValidImageFile(settings->lpszKernelFile)) {
				MessageBox(hDlg, TEXT("Invalid kernel image file. Please select an existing file with one of the specified extensions."), TEXT("Invalid Kernel Image File"), MB_ICONERROR);
			}

			HWND startBtnHwnd = (HWND)lParam;
			EnableWindow(GetDlgItem(hDlg, IDSTART), FALSE);

			BlendImages(settings, results);

			// Initialize COM
			//CoInitialize(NULL);

			//// The factory pointer
			//IWICImagingFactory *pFactory = NULL;

			//// Create the COM imaging factory
			//HRESULT hr = CoCreateInstance(
			//	CLSID_WICImagingFactory,
			//	NULL,
			//	CLSCTX_INPROC_SERVER,
			//	IID_PPV_ARGS(&pFactory)
			//	);

			return TRUE;
		}
		}
	default:
		return FALSE;
	}
}