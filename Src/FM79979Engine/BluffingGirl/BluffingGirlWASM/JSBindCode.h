#pragma once
#ifdef WASM
	int		GetBrowserWidth();
	int		GetBrowserHeight();

	int		GetViewportWidth();
	int		GetViewportHeight();

	bool	IsNotPC();
	int		get_CanvasPosX();
	int		get_CanvasPosY();
	void	JSDoFullscreen();
	void	JSInit();
	//please call this in main loop or viewport get wrong.
	void	JSViewportUpdate();
#endif