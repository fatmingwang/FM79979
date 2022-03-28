#pragma once

#ifdef WASM

//https ://stackoverflow.com/questions/59532379/how-to-call-javascript-method-from-c-with-parameters-using-em-js

namespace EMSDK
{
	int		EMSDK_GetBrowserWidth();
	int		EMSDK_GetBrowserHeight();

	int		EMSDK_GetViewportWidth();
	int		EMSDK_GetViewportHeight();

	bool	EMSDK_IsPC();
	int		EMSDK_GetCanvasPosX();
	int		EMSDK_GetCanvasPosY();
	void	EMSDK_JSDoFullscreen();
	void	EMSDK_JSInit();
	//please call this in main loop or viewport get wrong.
	void	EMSDK_JSViewportUpdate();
}
//code example
//call js directly
		//EM_ASM
		//(
		//	//FS.createFolder('/', 'user_data', true, true)
		//	//FS.mount(IDBFS, {}, '/user_data');
		//	// Make a directory other than '/'
		//	Module.IndexedDBName = '/GameApp';
		//	FS.mkdir(Module.IndexedDBName);
		//	// Then mount with IDBFS type
		//	FS.mount(IDBFS, {}, Module.IndexedDBName);
		//	Module.print("start file sync");
		//	//flag to check when data are synchronized
		//	Module.syncdone = 0;
		//	Module.WaitAllFileFinish = 1;
		//	// Then sync
		//	FS.syncfs(true, function(err)
		//	{
		//		if (err)
		//		{
		//			console.log('ERROR!', err);
		//		}
		//		Module.syncdone = 1;
		//		console.log('/GameApp mount sync finished ');
		//	});
		//);

#endif