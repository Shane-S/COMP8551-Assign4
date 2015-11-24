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
///<param name="offscreenBuffer">A struct to store the metadata.</param>
///<returns>TRUE on success, FALSE on failure.</returns>
BOOL GetImageData(LPTSTR file, LPOFFSCREENBUFFER offscreenBuffer) {
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

	offscreenBuffer->width = width;
	offscreenBuffer->height = height;
	offscreenBuffer->bytesPerPixel = 4;

	offscreenBuffer->info.bmiHeader.biSize = sizeof(offscreenBuffer->info.bmiHeader);
	offscreenBuffer->info.bmiHeader.biWidth = offscreenBuffer->width;
	offscreenBuffer->info.bmiHeader.biHeight = -offscreenBuffer->height;
	offscreenBuffer->info.bmiHeader.biPlanes = 1;
	offscreenBuffer->info.bmiHeader.biBitCount = 32;
	offscreenBuffer->info.bmiHeader.biCompression = BI_RGB;

	// Convert the format of the image frame to 32bppRGBA
	error = factory->CreateFormatConverter(&convertedFrame);
	if (error != S_OK) return FALSE;

	error = convertedFrame->Initialize(
		frame,                       // Source frame to convert
		GUID_WICPixelFormat32bppBGR, // The desired pixel format
		WICBitmapDitherTypeNone,     // The desired dither pattern
		NULL,                        // The desired palette 
		0.f,                         // The desired alpha threshold
		WICBitmapPaletteTypeCustom   // Palette translation type
	);
	if (error != S_OK) return FALSE;

	// See https://social.msdn.microsoft.com/Forums/vstudio/en-US/9bf9dea5-e21e-4361-a0a6-be331efde835/how-do-you-calculate-the-image-stride-for-a-bitmap
	DWORD dwStride = DIB_WIDTHBYTES(width * 32);
	INT bufSize = dwStride * height;
	offscreenBuffer->pixels = (void *)realloc(offscreenBuffer->pixels, bufSize);
	error = convertedFrame->CopyPixels(NULL, dwStride, (UINT)bufSize, (BYTE*)(offscreenBuffer->pixels));
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

	if (!GetImageData(settings->lpszImageFile, &results->bufs[BLENDRESULT_IMAGE]) ||
		!GetImageData(settings->lpszKernelFile, &results->bufs[BLENDRESULT_KERNEL])) {
		error = BLEND_GENERAL_FAILURE;
		goto finish;
	}

	// Check if kernel is larger
	if (results->bufs[BLENDRESULT_IMAGE].height < results->bufs[BLENDRESULT_KERNEL].height ||
		results->bufs[BLENDRESULT_IMAGE].width < results->bufs[BLENDRESULT_KERNEL].width) {
		error = BLEND_KERNEL_2LARGE;
		goto finish;
	}

	// The blended image will be the same, so just copy it over
	memcpy(&results->bufs[BLENDRESULT_BLENDED].info, &results->bufs[BLENDRESULT_IMAGE].info, sizeof(BITMAPINFO));
	long bufSize = 32 * results->bufs[BLENDRESULT_IMAGE].height * results->bufs[BLENDRESULT_IMAGE].width;
	results->bufs[BLENDRESULT_BLENDED].pixels = realloc(results->bufs[BLENDRESULT_BLENDED].pixels, (size_t)bufSize);

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