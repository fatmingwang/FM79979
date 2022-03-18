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
	void	JSViewportUpdate();
#endif