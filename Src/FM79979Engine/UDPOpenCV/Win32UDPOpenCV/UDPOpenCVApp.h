#pragma once

class	cUDPOpenCVApp :public cGameApp
{
	cOrthogonalCamera* m_pOrthogonalCamera;
protected:
	void				Update(float e_fElpaseTime);
	void				Render();
public:
#ifdef WIN32
	cUDPOpenCVApp(HWND e_Hwnd, Vector2 e_vGameResolution, Vector2 e_vViewportSize);
#else
	cUDPOpenCVApp(Vector2 e_vGameResolution, Vector2 e_vViewportSize);
#endif
	virtual ~cUDPOpenCVApp();
	//
	void							Init();
	virtual	void					MouseDown(int e_iPosX, int e_iPosY);
	virtual	void					MouseMove(int e_iPosX, int e_iPosY);
	virtual	void					MouseUp(int e_iPosX, int e_iPosY);
	void							KeyDown(char e_char);
	void							KeyUp(char e_char);
	//bool	Export(const char*e_strFileName, bool e_bBinary = false);
};