#pragma once
#ifdef WASM
	int		GetBrowserWidth();
	int		GetBrowserHeight();

	int		GetViewportWidth();
	int		GetViewportHeight();

	bool	IsNotPC();
	int		get_CanvasPosX();
	int		get_CanvasPosY();
	int		get_canvas_width();
	int		get_canvas_height();
	void	JSDoFullscreen();
	void	JSInit();
#endif