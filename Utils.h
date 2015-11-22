#include <Windows.h>
// Convert between TCHAR and char
#ifdef UNICODE
#define TCHAR_2_CHAR(dest, src, bufsize) \
	wcstombs_s(NULL, dest, bufsize, src, bufsize)

#define CHAR_2_TCHAR(dest, src, bufsize) \
	mbstowcs_s(NULL, dest, src, bufsize)
#else
#define TCHAR_2_CHAR(dest, src, size) \
	strcpy_s(dest, src, bufsize)

#define CHAR_2_TCHAR(dest, src, size) \
	strcpy_s(dest, src, bufsize)
#endif

///<summary>Returns a string representing the given dialog error code.</summary>
///
///<param name="dwErrorCode">The dialog error code.</param>
///<return>A string describing the given error code, or "Unknown".</return>
const TCHAR* CommDlgExtendedErrorToString(DWORD dwErrorCode);