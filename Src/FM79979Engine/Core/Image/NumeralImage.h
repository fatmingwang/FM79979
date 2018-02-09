#ifndef _NUMERAL_IMAGE_H_
#define _NUMERAL_IMAGE_H_
#include "BaseImage.h"

class	TiXmlElement;
namespace FATMING_CORE
{
	class	cCueToStartCurveWithTime;
	//class	cNumeralImage:public cSpriteGLTexture
	//{
	//	int	m_iEachImageWidth;
	//public:
	//	DEFINE_TYPE_INFO()
	//	cNumeralImage(char*e_strImageName);
	//	virtual ~cNumeralImage();

	//	void	Draw(int	e_iValue,int e_iPosX,int e_iPosY);
	//	//aligment by
	//	void	DrawFromDownToUp(int	e_iValue,int e_iPosX,int e_iPosY,float e_fRotate = 90.f);
	//	void	DrawFromUpToDown(int	e_iValue,int e_iPosX,int e_iPosY,float e_fRotate = 270.f);
	//};


	//=======================
	//set color and angle by BaseImage
	//=======================
	//
	class	cNumeralImage:public cBaseImage
	{
		bool	m_bImageFromesource;
		GET_SET_DEC(float*,m_pfTexCoordinate,GetTexCoordinate,SetCoordinate);
		GET_SET_DEC(int,m_iNumIndex,GetNumIndex,SetNumIndex);
		//single image width
		GET_SET_DEC(int,m_iSingleImageWidth,GetSingleImageWidth,SetSingleImageWidth);
		//single image height
		GET_SET_DEC(int,m_iSingleImageHeight,GetSingleImageHeight,SetSingleImageHeight);
		//right to left please set eD_LEFT,up to down please set eD_DOWN,else not support
		GET_SET_DEC(eDirection,m_eDirection,GetDirection,SetDirection);
		void	Draw(const char*e_strData,int e_iCount,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
		int64	m_i64Value;
		GET_SET_DEC(bool,m_bDrawOnCenter,GetDrawOnCenter,SetDrawOnCenter);
	public:
		DEFINE_TYPE_INFO()
		cNumeralImage(char*e_strImageName);
		cNumeralImage(cBaseImage*e_pImage,float*e_pftexCoordinate);//copy from specific image and specific tex coordinate,ex:puzzle image
		cNumeralImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9);
		cNumeralImage(cNumeralImage*e_pNumeralImage);
		static	cNumeralImage*	GetMe(TiXmlElement*e_pXmlelement);
		CLONE_MYSELF(cNumeralImage)
		virtual ~cNumeralImage();
		//this one is right to left,if e_pmat is null apply normal one or apply a matrix to it.
		void	Draw(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
		void	Draw(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0,bool e_bCenter = false);
		//
		void	DrawOnCenter(int	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0);
		void	DrawOnCenter(int64	e_iValue,int e_iPosX,int e_iPosY,float*e_pmat = 0);
		//max u value for texture space
		float	GetMaxU(){ return m_pfTexCoordinate[38]; }
		virtual	void	Render()override;
		void	SetValue(int64 e_i64Value);
		int64	GetValue(){return m_i64Value;}
	};

	class	cTimeNumerialImage:public cNumeralImage
	{
		bool					m_bEnableHour;
		bool					m_bEnableMinute;
		bool					m_bEnableSecond;
		Vector3					m_vHourPos;
		Vector3					m_vMinPos;
		Vector3					m_vSecondPos;
		cRenderObject*	m_pDisableObject;
	public:
		DEFINE_TYPE_INFO()
		cTimeNumerialImage(cBaseImage*e_pImage0,cBaseImage*e_pImage9,cCueToStartCurveWithTime*e_pHourSubMPDI,cCueToStartCurveWithTime*e_pMinSubMPDI,cCueToStartCurveWithTime*e_pSecondSubMPDI,cRenderObject*e_pDisableObject = 0);
		virtual ~cTimeNumerialImage();
		virtual	void	Init()override;
		virtual	void	Update(float e_fElpaseTime)override;
		virtual	void	Render()override;
		static cTimeNumerialImage*GetMe(TiXmlElement*e_pXmlelement);
	};
}
#endif