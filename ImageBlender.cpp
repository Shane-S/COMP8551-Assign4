#include <string.h>
#include "ImageBlender.h"
#include "CTimer.h"

VOID BlendSerially(LPBLENDRESULT results) {
	// Blend the stuff serially
}

VOID BlendWithMMX(LPBLENDRESULT results) {

}

VOID BlendWithSSE(LPBLENDRESULT results) {

}

///<summary>Performs the actual blending of the images.</summary>
///<param name="params">A buffer conaining a BLENDSETTINGS struct immediately followed by a BLENDRESULTS struct.</param>
///<returns>Whether the thread succeeded.</returns>
DWORD WINAPI BlendFunc(LPVOID params) {
	LPBLENDSETTINGS settings = *((LPBLENDSETTINGS*)params);
	LPBLENDRESULT   results = *((LPBLENDRESULT*)((LPBLENDSETTINGS*)params + 1));
	CTimer timer;

	timer.Start();
	if (settings->blendType == BLEND_SERIAL) {
		BlendSerially(results);
	} else if (settings->blendType == BLEND_MMX) {
		BlendWithMMX(results);
	} else {
		// Such a rebel that I'm not going to check for invalid blend types
		BlendWithSSE(results);
	}

	Sleep(1000);
	timer.End();
	timer.DiffMs(results->processTime);	
	DWORD dwError = SendMessage(results->hwndNotifyWindow, WM_BLENDFINISHED, BLEND_NO_ERROR, 0);

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