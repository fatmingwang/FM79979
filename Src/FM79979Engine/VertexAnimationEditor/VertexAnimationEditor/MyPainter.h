#ifndef	_MY_PAINTER_H_
#define	_MY_PAINTER_H_

#include "../../Core/Terrian/TerrianVertex.h"
#include "../../Core/Terrian/ShockBoobsShader.h"
//use dot net to make a painter

//please delete data after fetch
void*		FetchPixeldata(System::Drawing::Bitmap^e_pImage,int*e_piWidth,int*e_piHeight,int*e_piChannel);

enum	ePaintType
{
	eTP_ADD = 0,
	eTP_DEL,
	eTP_NONE,
};
enum	ePatinerMode
{
	ePM_PAINT = 0,
	ePM_SHOCK,
};

class cMyBoobsPainter:public ISAXCallback
{
	//
	virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
	void									ProcesscMyBoobsPainterData(TiXmlElement*e_pTiXmlElement);
	void									ProcessShaderParmeterData(TiXmlElement*e_pTiXmlElement);
	//
	float									m_fImageMoveDistance;
	Vector2									m_vMouseDownPos;
	//
	unsigned char*							m_pPixelDataForImage;
	float*									m_pFloatPixelDataForModify;
	cShockBoobsShader*						m_pShockBoobsShader;
	//
	FATMING_CORE::cTerrianVertex*			m_pTerrianVertex;
	bool									m_bMouseDown;
	int										m_iRadius;
	UT::sTimeCounter						m_DrawGapTC;
	//
	Vector4									m_vColor;
	POINT									m_MousePos;
	//
	cBaseImage*								m_pPaintImage;
	cBaseImage*								m_pOriginalPaintImage;
	//
	void									UpdatePixelData(int e_iStartPosX,int e_iStartPosY,int e_iWidth,int e_iHeight);
public:
	cMyBoobsPainter(char*e_strImageFileName);
	~cMyBoobsPainter();
	//
	ePaintType								m_eColorType;
	ePatinerMode							m_ePatinerMode;
	//
	void		Destroy();
	//
	void		SetSize(int e_iWidth,int e_iHeight,int e_iChannel);
	void		DrawCircle(POINT e_Pos);
	void		Clean();
	void		SetColor(Vector4 e_vColor);
	void		SetCircleRadius(int e_iRadius);
	void		Update(float e_fElpaseTime);
	void		Render();
	//
	void		RenderTerrian(cFrameCamera*e_pFrameCamera);
	void		DebugRenderTerrian(cFrameCamera*e_pFrameCamera);
	//
	void		MouseDown(int e_iPosX,int e_iPosY);
	void		MouseMove(int e_iPosX,int e_iPosY);
	void		MouseUp(int e_iPosX,int e_iPosY);
	void		SetParmater(float e_fShockSpeed,float e_fShockSpeedMinus,float e_fShockStrgethValue,float e_fMaximumShockStregth);
	void		SetShockDirection(Vector3 e_vDirection);
	//
	bool		IsShockDone(){ return m_pShockBoobsShader->IsShockDone(); }
	bool		ChangePaintingImage(char*e_strFileName);
	bool		SaveFile(char*e_strFileName);
	bool		OpenFile(char*e_strFileName);
	cShockBoobsShader*						GetShader(){return m_pShockBoobsShader;}
};
//
#endif