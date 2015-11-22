#include "Utils.h"

const TCHAR* CommDlgExtendedErrorToString(DWORD dwErrorCode) {
	switch (dwErrorCode){
		// Common dialog box error codes
	case CDERR_DIALOGFAILURE: return TEXT("The dialog box could not be created");
	case CDERR_FINDRESFAILURE: return TEXT("The common dialog box function failed to find a specified resource.");
	case CDERR_INITIALIZATION: return TEXT("The common dialog box function failed during initialization.");
	case CDERR_LOADRESFAILURE: return TEXT("The common dialog box function failed to load a specified resource.");
	case CDERR_LOADSTRFAILURE: return TEXT("The common dialog box function failed to load a specified string.");
	case CDERR_LOCKRESFAILURE: return TEXT("The common dialog box function failed to lock a specified resource.");
	case CDERR_MEMALLOCFAILURE: return TEXT("The common dialog box function was unable to allocate memory for internal structures.");
	case CDERR_MEMLOCKFAILURE: return TEXT("The common dialog box function was unable to lock the memory associated with a handle.");
	case CDERR_NOHINSTANCE: return TEXT("No template HINSTANCE provided, but ENABLETEMPLATE flag was set.");
	case CDERR_NOHOOK: return TEXT("No pointer to hook procedure provided, but ENABLEHOOK flag was set.");
	case CDERR_NOTEMPLATE: return TEXT("No template provided, but ENABLETEMPLATE flag was set.");
	case CDERR_REGISTERMSGFAIL: TEXT("The RegisterWindowMessage function returned an error code when it was called by the common dialog box function.");
	case CDERR_STRUCTSIZE: return TEXT("Invalid lStructSize member in common dialog box initialization structure.");

		// Font chooser error codes
	case CFERR_MAXLESSTHANMIN: return TEXT("The size specified in the nSizeMax member of the CHOOSEFONT structure is less than the size specified in the nSizeMin member.");
	case CFERR_NOFONTS: return TEXT("No fonts exist.");

		// File chooser error codes
	case FNERR_BUFFERTOOSMALL: return TEXT("The lpstrFile buffer is too small for the specified file name.");
	case FNERR_INVALIDFILENAME: return TEXT("A file name is invalid.");
	case FNERR_SUBCLASSFAILURE: return TEXT("An attempt to subclass a list box failed because sufficient memory was not available.");

		// Find and replace error codes
	case FRERR_BUFFERLENGTHZERO: return TEXT("A member of the FINDREPLACE structure points to an invalid buffer.");
	
		// Print dialog error codes
	case PDERR_CREATEICFAILURE: return TEXT("The PrintDlg function failed when it attempted to create an information context.");
	case PDERR_DEFAULTDIFFERENT: return TEXT("DN_DEFAULTPRN flag specified in DEVNAMES.wDefault, but other structure members don't match current default printer. See MS documentation for the fix.");
	case PDERR_DNDMMISMATCH: return TEXT("The data in the DEVMODE and DEVNAMES structures describes two different printers.");
	case PDERR_GETDEVMODEFAIL: return TEXT("The printer driver failed to initialize a DEVMODE structure.");
	case PDERR_INITFAILURE: return TEXT("The PrintDlg function failed during initialization. No more information is known.");
	case PDERR_LOADDRVFAILURE: return TEXT("The PrintDlg function failed to load the device driver for the specified printer.");
	case PDERR_NODEFAULTPRN: return TEXT("A default printer does not exist.");
	case PDERR_NODEVICES: return TEXT("No printer drivers were found.");
	case PDERR_PARSEFAILURE: return TEXT("The PrintDlg function failed to parse the strings in the[devices] section of the WIN.INI file.");
	case PDERR_PRINTERNOTFOUND: return TEXT("The[devices] section of the WIN.INI file did not contain an entry for the requested printer.");
	case PDERR_RETDEFFAILURE: return TEXT("The PD_RETURNDEFAULT flag was specified in the Flags member of the PRINTDLG structure, but the hDevMode or hDevNames member was not NULL.");
	case PDERR_SETUPFAILURE: return TEXT("The PrintDlg function failed to load the required resources.");

	default: return TEXT("Unknown error. Have a nice day :)");
	}
}