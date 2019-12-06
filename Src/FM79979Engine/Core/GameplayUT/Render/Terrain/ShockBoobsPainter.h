#pragma once
#include "ShockBoobsShader.h"
#include "TerrianVertex.h"
#include "../../../XML/XMLLoader.h"
#include "../CommonRender/BaseImage.h"
namespace FATMING_CORE
{
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
		ePM_GAME_MODE,//only parse terrian data then change mode to shock
	};
	class	cFrameCamera;
	class cShockBoobsPainter:public ISAXCallback
	{
		//
		virtual	void							HandleElementData(TiXmlElement*e_pTiXmlElement);
		void									ProcesscShockBoobsPainterData(TiXmlElement*e_pTiXmlElement);
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
		Vector3									m_vTerrianScale;
		UT::sTimeCounter						m_DrawGapTC;
		//
		Vector4									m_vColor;
		POINT									m_MousePos;
		//
		cBaseImage*								m_pPaintImage;
		cBaseImage*								m_pOriginalPaintImage;
		//
		void									UpdatePixelData(int e_iStartPosX,int e_iStartPosY,int e_iWidth,int e_iHeight);
		void									TerrainTransform(cFrameCamera*e_pFrameCamera);
		Vector2									m_vPatingImageOffsetPos;
	public:
		cShockBoobsPainter(char*e_strImageFileName,ePatinerMode e_ePatinerMode = ePM_PAINT);
		virtual ~cShockBoobsPainter();
		//
		ePaintType								m_eColorType;
		ePatinerMode							m_ePatinerMode;
		//
		void		Destroy();
		//
		void		SetSize(int e_iWidth,int e_iHeight,int e_iChannel);
		void		DrawSphere(POINT e_Pos);
		void		Clean();
		void		SetColor(Vector4 e_vColor);
		void		SetSphereRadius(int e_iRadius);
		void		Update(float e_fElpaseTime);
		void		Render();
		//
		void		RenderTerrian(cFrameCamera*e_pFrameCamera = 0);
		void		DebugRenderTerrian(cFrameCamera*e_pFrameCamera = 0);
		//
		void		MouseDown(int e_iPosX,int e_iPosY);
		void		MouseMove(int e_iPosX,int e_iPosY);
		void		MouseUp(int e_iPosX,int e_iPosY);
		void		SetParmater(float e_fShockSpeed,float e_fShockSpeedMinus,float e_fShockStrgethValue,float e_fMaximumShockStregth);
		void		SetShockDirection(Vector3 e_vDirection);
		Vector3		GetShockDirection();
		void		AddCurrentStrgeth(float e_fValue);
		//
		bool		IsShockDone(){ return m_pShockBoobsShader->IsShockDone(); }
		bool		ChangePaintingImage(char*e_strFileName,bool e_bAutoResizeTerrianToGameResoution = true);
		bool		SaveFile(char*e_strFileName);
		bool		OpenFile(char*e_strFileName);
		cShockBoobsShader*						GetShader(){return m_pShockBoobsShader;}
	};
//
}