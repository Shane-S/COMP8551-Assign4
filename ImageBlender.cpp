#include <string.h>
#include "ImageBlender.h"
#include "WICImagingFactory.h"
#include "CTimer.h"
#include "Utils.h"

VOID BlendSerially(LPBLENDRESULT results) {
	// Blend the stuff serially
}

VOID BlendWithMMX(LPBLENDRESULT results) {

}

VOID BlendWithSSE(LPBLENDRESULT results) {

}

///<summary>Retrieves the pixels from the image contained in file, storing them as RGBA8888 pixels and putting the metadata in info.</summary>
///<param name="file">The image file to load.</param>
///<param name="info">A struct to store the metadata.</param>
///<param name="pixels">A pointer which will contain the pixels after this call.</param>
///<returns>TRUE on success, FALSE on failure.</returns>
BOOL GetImageData(LPTSTR file, LPBITMAPINFO info, LPVOID *pixels) {
	// Bit of a strange way of doing this... Oh well
	IWICImagingFactory *factory = CWICImagingFactory::GetInstance().GetFactory();
	HRESULT error;

	// TODO: Error checking
	// These smart pointers will be freed automatically once out of scope
	// That's pretty smart
	CComPtr<IWICBitmapDecoder> decoder;
	CComPtr<IWICBitmapFrameDecode> frame;
	CComPtr<IWICFormatConverter> convertedFrame;
	wchar_t imageNameWide[MAX_PATH] = { 0 };
	TCHAR_2_WCHAR(imageNameWide, file, MAX_PATH);
	error = factory->CreateDecoderFromFilename(imageNameWide, NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (error != S_OK) return FALSE;

	error = decoder->GetFrame(0, &frame);
	if (error != S_OK) return FALSE;
	
	// Set the image metadata
	UINT width;
	UINT height;
	error = frame->GetSize(&width, &height);
	if (error != S_OK) return FALSE;

	info->bmiHeader.biWidth = width;
	info->bmiHeader.biHeight = height;
	info->bmiHeader.biBitCount = 32;
	info->bmiHeader.biCompression = BI_RGB;
	info->bmiHeader.biHeight *= -1; // Convert to top-down image
	info->bmiHeader.biPlanes = 1;
	info->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	// Convert the format of the image frame to 32bppRGBA
	error = factory->CreateFormatConverter(&convertedFrame);
	if (error != S_OK) return FALSE;

	error = convertedFrame->Initialize(
		frame,                        // Source frame to convert
		GUID_WICPixelFormat32bppRGBA, // The desired pixel format
		WICBitmapDitherTypeNone,      // The desired dither pattern
		NULL,                         // The desired palette 
		0.f,                          // The desired alpha threshold
		WICBitmapPaletteTypeCustom    // Palette translation type
	);
	if (error != S_OK) return FALSE;

	UINT bufSize = 32 * info->bmiHeader.biWidth * info->bmiHeader.biHeight;
	*pixels = (void *)realloc(*pixels, bufSize);
	error = convertedFrame->CopyPixels(NULL, 32 * info->bmiHeader.biWidth, bufSize, (BYTE*)(*pixels));
	return error == S_OK;
}

///<summary>Performs the actual blending of the images.</summary>
///<param name="params">A buffer conaining a BLENDSETTINGS struct immediately followed by a BLENDRESULTS struct.</param>
///<returns>Whether the thread succeeded.</returns>
DWORD WINAPI BlendFunc(LPVOID params) {
	LPBLENDSETTINGS settings = *((LPBLENDSETTINGS*)params);
	LPBLENDRESULT   results = *((LPBLENDRESULT*)((LPBLENDSETTINGS*)params + 1));
	CTimer timer;

	DWORD error = BLEND_NO_ERROR;

	if (!GetImageData(settings->lpszImageFile, &results->metaData[BLENDRESULT_IMAGE], &results->pixels[BLENDRESULT_IMAGE]) ||
		!GetImageData(settings->lpszKernelFile, &results->metaData[BLENDRESULT_KERNEL], &results->pixels[BLENDRESULT_KERNEL])) {
		error = BLEND_GENERAL_FAILURE;
		goto finish;
	}

	// Check if kernel is larger
	if (results->metaData[BLENDRESULT_IMAGE].bmiHeader.biHeight < results->metaData[BLENDRESULT_KERNEL].bmiHeader.biHeight ||
		results->metaData[BLENDRESULT_IMAGE].bmiHeader.biWidth < results->metaData[BLENDRESULT_KERNEL].bmiHeader.biWidth) {
		error = BLEND_KERNEL_2LARGE;
		goto finish;
	}

	// The blended image will be the same, so just copy it over
	memcpy(&results->metaData[BLENDRESULT_BLENDED], &results->metaData[BLENDRESULT_IMAGE], sizeof(BITMAPINFO));
	long bufSize = 32 * results->metaData[BLENDRESULT_IMAGE].bmiHeader.biHeight * results->metaData[BLENDRESULT_IMAGE].bmiHeader.biWidth * -1;
	results->pixels[BLENDRESULT_BLENDED] = realloc(results->pixels[BLENDRESULT_BLENDED], (size_t)bufSize);

	timer.Start();
	if (settings->blendType == BLEND_SERIAL) {
		BlendSerially(results);
	} else if (settings->blendType == BLEND_MMX) {
		BlendWithMMX(results);
	} else {
		// Such a rebel that I'm not going to check for invalid blend types
		BlendWithSSE(results);
	}

	timer.End();
	timer.DiffMs(results->processTime);

finish:
	SendMessage(results->hwndNotifyWindow, WM_BLENDFINISHED, error, 0);
	free(params);
	return 0;
}

VOID BlendImages(LPBLENDSETTINGS settings, LPBLENDRESULT results) {
	// Copy the arguments to a single buffer (must be on the heap so that the thread can still access it)
	// It's kinda wasteful to do this allocation since we know they're already aligned, but oh well
	// The thread will free this
	LPVOID pvThreadParams = (LPVOID)malloc(sizeof(LPBLENDSETTINGS) + sizeof(LPBLENDRESULT));
	*((LPBLENDSETTINGS*)pvThreadParams) = settings;
	*((LPBLENDRESULT*)((LPBLENDSETTINGS*)pvThreadParams + 1)) = results;
	
	CreateThread(NULL, 0, BlendFunc, pvThreadParams, 0, NULL);
}