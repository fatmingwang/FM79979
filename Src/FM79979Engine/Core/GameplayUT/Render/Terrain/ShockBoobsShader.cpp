#include "ShockBoobsShader.h"

namespace FATMING_CORE
{
	extern cNamedTypedObjectVector<cBaseShader>*g_pAll2DShaderList;
	cShockBoobsShader::cShockBoobsShader(const char*e_strVS,const char*e_strPS,float e_fMaximumShockStregth,bool e_bTexture,const wchar_t*e_strName)
	:cBaseShader(e_strVS,e_strPS,e_strName,e_bTexture)
	{
		m_bShockDone = true;
		m_uiShockValueLocation = -1;
		m_uiShockValueLocation = GetUniFormLocationByName("ShockValue");
		m_vCurrentShockDirectionValue = m_vShockDirection = Vector3(0,1,0);
		m_fMaximumShockStregth = e_fMaximumShockStregth;
		m_fCurrentShockStregth = 0.f;
		g_pAll2DShaderList->AddObjectNeglectExist(this);
		m_fShockSpeed = m_fCurrentShockSpeed = 1.f;
		m_fShockSpeedMinus = 1.f;
		m_fShockStrgethValue = 1.f;
	}

	cShockBoobsShader::~cShockBoobsShader()
	{
		g_pAll2DShaderList->RemoveObjectWithoutDelete(this->GetName());
	}

	void	cShockBoobsShader::SetParmater(float e_fShockSpeed,float e_fShockSpeedMinus,float e_fShockStrgethValue,float e_fMaximumShockStregth)
	{
		m_bShockDone = false;
		m_fCurrentShockSpeed = m_fShockSpeed = e_fShockSpeed;
		m_fShockSpeedMinus = e_fShockSpeedMinus;
		m_fShockStrgethValue = e_fShockStrgethValue;
		m_fMaximumShockStregth = e_fMaximumShockStregth;
		m_fCurrentShockStregth = 0.f;
		if( m_vShockDirection.Length() > m_fMaximumShockStregth )
		{
			m_vShockDirection = m_vShockDirection.Normalize()*m_fMaximumShockStregth;
		}
	}

	void	cShockBoobsShader::SetShockDirection(Vector3 e_vDirection)
	{
		m_bShockDone = false;
		m_fCurrentShockSpeed = m_fShockSpeed;
		m_vShockDirection = e_vDirection;
		if( m_vShockDirection.Length() > m_fMaximumShockStregth )
		{
			m_vShockDirection = m_vShockDirection.Normalize()*m_fMaximumShockStregth;
		}
	}

	void	cShockBoobsShader::Update(float e_fElpaseTime)
	{
		if( m_fCurrentShockSpeed > 0.f )
		{
			m_bShockDone = false;
			m_fCurrentShockSpeed -= m_fShockSpeedMinus*e_fElpaseTime;
			if( m_fCurrentShockSpeed > 0.f )
			{
				//speed by m_vShockDirection length
				m_fCurrentShockStregth += m_fCurrentShockSpeed;
				float	l_fValue = sin(m_fCurrentShockStregth/2);
				Vector3	l_vShockDirection = m_vShockDirection;
				if( m_fCurrentShockStregth > 30 )
				{
					//l_vShockDirection = cMatrix44::RotationMatrix(Vector3(0,0,frand(0,m_fCurrentShockStregth))).TransformVector(m_vShockDirection);
				}
				m_vCurrentShockDirectionValue = l_fValue*l_vShockDirection;
			}
		}
		else
		{
			if(m_vCurrentShockDirectionValue.Length() >0.1 )
			{
				m_vCurrentShockDirectionValue *= 0.9f;
			}
			else
			{
				m_bShockDone = true;
				m_vCurrentShockDirectionValue = Vector3(0,0,0);
			}
		}
	}

	void	cShockBoobsShader::Use(bool e_bUseLastWVPMatrix)
	{
		cBaseShader::Use(e_bUseLastWVPMatrix);
		Vector3	l_vValue = m_vCurrentShockDirectionValue * m_fShockStrgethValue;
#if defined(WASM)
		m_uiShockValueLocation = GetUniFormLocationByName("ShockValue");
#endif
		glUniform3f(m_uiShockValueLocation,l_vValue.x,l_vValue.y,l_vValue.z);
	}
	const char*cShockBoobsShader::m_sVS = "					\
	attribute vec3 VSPosition;								\
	attribute vec2 VSTexcoord;								\
	uniform mat4 matVP;										\
	uniform mat4 matW;										\
	uniform vec3 ShockValue;								\
	varying vec2 PSTexcoord;								\
	void main()												\
	{														\
		vec3	l_vPos = VSPosition;						\
		if(l_vPos.z != float(0) )							\
		{																	\
			l_vPos = VSPosition.xyz+(VSPosition.z*ShockValue.xyz);		\
		}																	\
		gl_Position = matVP*matW*vec4(l_vPos,1);		\
		PSTexcoord = VSTexcoord;							\
	}";
	//const char*cShockBoobsShader::m_sVS = "					\
	//attribute vec3 VSPosition;								\
	//attribute vec3 VSNormal;								\
	//attribute vec2 VSTexcoord;								\
	//uniform mat4 matVP;										\
	//uniform mat4 matW;										\
	//uniform vec3 ShockValue;								\
	//varying vec2 PSTexcoord;								\
	//void main()												\
	//{														\
	//	vec3	l_vPos = VSPosition;						\
	//	if(l_vPos.z != float(0) )							\
	//	{																	\
	//		l_vPos = VSPosition.xyz+(VSPosition.z*ShockValue.xyz);		\
	//	}																	\
	//	gl_Position = matVP*matW*vec4(l_vPos,1);		\
	//	PSTexcoord = VSTexcoord;							\
	//}";

	#if defined(IOS) || defined(ANDROID) || defined(WASM)
		const char*cShockBoobsShader::m_sFS = "						\
		uniform sampler2D texSample;								\
		varying lowp vec2 PSTexcoord;								\
		void main()													\
		{															\
			gl_FragColor = texture2D(texSample, PSTexcoord);		\
		}";
	#else
		const char*cShockBoobsShader::m_sFS = "						\
		uniform sampler2D texSample;								\
		varying vec2 PSTexcoord;									\
		void main()													\
		{															\
			gl_FragColor = texture2D(texSample, PSTexcoord);		\
		}";
	#endif
//end nemespace FATMING_CORE
}