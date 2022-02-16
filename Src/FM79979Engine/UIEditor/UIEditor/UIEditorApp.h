#pragma once


class	cUIEditorApp :public cGameApp
{
protected:
	void				Update(float e_fElpaseTime);
	void				Render();
public:
	cUIEditorApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize);
	virtual ~cUIEditorApp();

	void							Init();
	void							Resize();

	virtual	void					MouseDown(int e_iPosX, int e_iPosY);
	virtual	void					MouseMove(int e_iPosX, int e_iPosY);
	virtual	void					MouseUp(int e_iPosX, int e_iPosY);
	void							KeyDown(char e_char);
	void							KeyUp(char e_char);
};