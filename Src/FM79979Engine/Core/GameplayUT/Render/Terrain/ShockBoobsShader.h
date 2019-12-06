#pragma once

#include "../../OpenGL/GLSL/Shader.h"
namespace FATMING_CORE
{
	class	cShockBoobsShader:public cBaseShader
	{
		GLuint	m_uiShockValueLocation;
		//the direction boobs Shock
		Vector3				m_vShockDirection;
		//
		GET_SET_DEC(float,m_fMaximumShockStregth,GetMaximumShockStregth,SetMaximumShockStregth);
		//
		//make a Sphere to Shock
		float				m_fShockSpeed;
		float				m_fShockSpeedMinus;
		float				m_fShockStrgethValue;
		//let shacking slow down
		float				m_fCurrentShockStregth;//if 0 no shacking
		float				m_fCurrentShockSpeed;//
		Vector3				m_vCurrentShockDirectionValue;
		bool				m_bShockDone;
	public:
		cShockBoobsShader(const char*e_strVS,const char*e_strPS,float e_fMaximumShockStregth = 30.f,bool e_bTexture = true,wchar_t*e_strName = L"cShockBoobsShader");
		virtual ~cShockBoobsShader();
		virtual	void	Use(bool e_bUseLastWVPMatrix = true);
		void			Update(float e_fElpaseTime);
		void			SetParmater(float e_fShockSpeed,float e_fShockSpeedMinus,float e_fShockStrgethValue,float e_fMaximumShockStregth);
		Vector3			GetShockDirection(){ return m_vShockDirection; }
		void			SetShockDirection(Vector3 e_vDirection);
		bool			IsShockDone(){ return m_bShockDone; }
		Vector3			GetCurrentShockDirectionValue(){return m_vCurrentShockDirectionValue;}
		//
		float			GetShockSpeed(){return m_fShockSpeed;}
		float			GetShockSpeedMinus(){return m_fShockSpeedMinus;}
		float			GetShockStrgethValue(){return  m_fShockStrgethValue;}
		//
		static	const char*m_sVS;
		static	const char*m_sFS;
	};
//end nemespace FATMING_CORE
}