#include <Windows.h>

// Message sent to the parent window when the blending has finished (or failed)
// wParam will be the error code
#define WM_BLENDFINISHED (WM_APP)

#define BLEND_NO_ERROR        0
#define BLEND_GENERAL_FAILURE 1
#define BLEND_KERNEL_2LARGE   3

#define BLEND_SERIAL 0
#define BLEND_MMX    1
#define BLEND_SSE    2
typedef struct _BLENDSETTINGS {
	TCHAR* lpszImageFile;  // The main image file
	TCHAR* lpszKernelFile; // The kernel image file
	double blendFactor;    // The blend factor (between 0 and 1)
	BYTE   blendType;      // How to perform the blending
} BLENDSETTINGS, *LPBLENDSETTINGS;

// Indices into the arrays within the struct
#define BLENDRESULT_IMAGE   0
#define BLENDRESULT_KERNEL  1
#define BLENDRESULT_BLENDED 2
typedef struct _BLENDRESULT {
	HWND   hwndNotifyWindow; // Window to notify on completion
	double processTime;      // The time in milliseconds taken to process the images
	BITMAPINFO metaData[3];  // Metadata for each of the images
	LPVOID pixels[3];        // The pixels of each image
} BLENDRESULT, *LPBLENDRESULT;

///<summary>Asynchronously blends the images specified in the settings, returning the results in the specified results struct.</summary>
///<remarks>Do no write to the settings struct and do not read from or write to the results struct until the blend has finished.</remarks>
///<param name="settings">Struct containing the images to blend, the blend factor, and the blend method to use.</param>
///<param name="results">Struct specifiying the window to notify on finishing the blend. It will hold the resulting bitmaps.</param>
VOID BlendImages(LPBLENDSETTINGS settings, LPBLENDRESULT results);