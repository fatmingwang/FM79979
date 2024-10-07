#pragma once

#include "Shader.h"

namespace FATMING_CORE
{
	class cTunnelEffect:public cBaseShader
	{
		float	m_fTimeToAnim;
		GLuint	m_uTimeToAnimLocation;
	public:
		cTunnelEffect(const char*e_strVS,const char*e_strPS,const wchar_t*e_strName = L"cTunnelEffect");
		virtual ~cTunnelEffect();
		static cTunnelEffect*CreateShader(const char*e_strVSFileName,const char*e_strPSFileName,const WCHAR*e_strShaderName = L"cTunnelEffect");
		virtual	void	Use(bool e_bUseLastWVPMatrix = true);
		virtual	void	Update(float e_fElpaseTime);
		void			SetTime(float);
		static	const char*m_sVS;//vertical
		static	const char*m_sFS;
	};
}

//vec3 doTunnel( sampler2D sampler, in vec2 uv, in float time ) 
//{
//    // get polar coordinates    
//    float a = atan( uv.y, uv.x );
//    float r = length( uv );
//    
//    // pack and animate    
//    uv = vec2( 1.0/r + time, a/3.1415927 );
//
//    // fetch from texture    
//    return texture( sampler, uv ).xyz;
//}