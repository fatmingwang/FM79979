#pragma once
namespace FATMING_CORE
{
	//================================================================
	//here divide two part one part is implement data,
	//another part is interface for transform the current interface to wanted by ChangeProperty(  Data.p and Data.cpp )
	//================================================================
	class cPrtColorInitrSetColor:public cParticleBase
	{
		GETP_SET_DEC(Vector4,m_vColor,GetColor,SetColor);
	public:
		DEFINE_TYPE_INFO()
		cPrtColorInitrSetColor(){ m_vColor = Vector4::One; }
		virtual ~cPrtColorInitrSetColor(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			e_pParticleData->vColor = m_vColor;
			e_pParticleData->vOriginalColor = m_vColor;
		}
		virtual NamedTypedObject*  Clone()override;
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual std::string	GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual std::string	GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool		SetDataByDataString(const char*e_pString)override;
	};
	//===========
	//
	//===========
	class cPrtColorInitrSetRandomColor :public cPrtColorInitrSetColor
	{
	public:
		DEFINE_TYPE_INFO()
		explicit  cPrtColorInitrSetRandomColor() throw(){}
		virtual ~cPrtColorInitrSetRandomColor(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			float	l_fR = UT::frand(m_vColor.r,1.f);
			float	l_fG = UT::frand(m_vColor.g,1.f);
			float	l_fB = UT::frand(m_vColor.b,1.f);
			float	l_fA = UT::frand(m_vColor.a,1.f);
			e_pParticleData->vColor = Vector4(l_fR,l_fG,l_fB,l_fA);
			e_pParticleData->vOriginalColor = e_pParticleData->vColor;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*	Clone()override;
		virtual std::string			GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual std::string			GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool				SetDataByDataString(const char*e_pString)override;
	};
	//===========
	//
	//===========
	class cPrtColorActBlending :public cParticleBase
	{
		GETP_SET_DEC(Vector4,m_vColor,GetColor,SetColor);
		GETP_SET_DEC(bool,m_bFade,IsFade,SetFade);
	public:
		cPrtColorActBlending(){m_vColor = Vector4::One;}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			//get value
			Vector4	l_vColor = e_fElpaseTime*m_vColor;
			if(m_bFade)
			{
				e_pParticleData->vColor-=l_vColor;
				if(e_pParticleData->vColor.r<0.f)
					e_pParticleData->vColor.r = 0.f;
				if(e_pParticleData->vColor.g<0.f)
					e_pParticleData->vColor.g = 0.f;
				if(e_pParticleData->vColor.b<0.f)
					e_pParticleData->vColor.b = 0.f;
				if(e_pParticleData->vColor.a<0.f)
					e_pParticleData->vColor.a = 0.f;
			}
			else
			{
				e_pParticleData->vColor += l_vColor;
				if(e_pParticleData->vColor.r>1.f)
					e_pParticleData->vColor.r = 1.f;
				if(e_pParticleData->vColor.g>1.f)
					e_pParticleData->vColor.g = 1.f;
				if(e_pParticleData->vColor.b>1.f)
					e_pParticleData->vColor.b = 1.f;
				if(e_pParticleData->vColor.a>1.f)
					e_pParticleData->vColor.a = 1.f;
				//e_pParticleData->Color+=l_dwValue;
			}	
		}
		DEFINE_TYPE_INFO()
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
		virtual std::string	GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual std::string	GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool		SetDataByDataString(const char*e_pString)override;
	};
	//===========
	//
	//===========
	class cPrtColorActBlendingByLife :public cPrtColorActBlending
	{
	public:
		DEFINE_TYPE_INFO()
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			float l_fCurrentProgress = 1-(e_pParticleData->fLifespan/e_pParticleData->fOriginaLifeSpan);
			e_pParticleData->vColor = (m_vColor-e_pParticleData->vOriginalColor)*l_fCurrentProgress+e_pParticleData->vOriginalColor;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone()override;
	};
	//===========
	//
	//
	//===========
	class cPrtColorActBlendingBy2Color :public cParticleBase
	{
		GETP_SET_DEC(Vector4,m_vColor1,GetColor1,SetColor1);
		GETP_SET_DEC(Vector4,m_vColor2,GetColor2,SetColor2);
	public:
		DEFINE_TYPE_INFO()
		cPrtColorActBlendingBy2Color()
		{
			m_vColor1 = Vector4::One;
			m_vColor2 = Vector4::Zero;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*	Clone()override;
		virtual std::string			GetDataInfo()override;
		//this one for output data value by "," to separate value
		virtual std::string			GetOutputDataString()override;
		//input the output data string,and analyze it
		virtual bool				SetDataByDataString(const char*e_pString)override;
		virtual inline void			Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)override
		{
			float l_fCurrentProgress = 1-(e_pParticleData->fLifespan/e_pParticleData->fOriginaLifeSpan);
			if( l_fCurrentProgress<0.5f )
			{
				l_fCurrentProgress*=2;
				e_pParticleData->vColor = (m_vColor1-e_pParticleData->vOriginalColor)*l_fCurrentProgress+e_pParticleData->vOriginalColor;
			}
			else
			{
				l_fCurrentProgress-=0.5f;
				l_fCurrentProgress*=2;
				e_pParticleData->vColor = (m_vColor2-m_vColor1)*l_fCurrentProgress+m_vColor1;
			}
		}
	};
//end namespace FATMING_CORE
};