#ifndef PRT_SIZE_H
#define PRT_SIZE_H

namespace FATMING_CORE
{
	//==============================
	//default VB size
	////while primitive is points ActSizeBlending and initSize just fetch x data
	//==============================

	class cPrtSizeInitSetSize:public cParticleBase,public Vector2
	{
		GETP_SET_DEC(float,m_fRandomValue,GetRandomValue,SetRandomValue);//only between 1~0 else will occur exceed expactation,more close 1 more effective
	public:
		DEFINE_TYPE_INFO()
		explicit	cPrtSizeInitSetSize() throw(){  x = 1.f; y = 1.f; m_fRandomValue = 0.f; }
		virtual ~cPrtSizeInitSetSize(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			if( m_fRandomValue == 0.f )
			{
				e_pParticleData->vSize.x = x;
				e_pParticleData->vSize.y = y;
				e_pParticleData->vOriginalSize = e_pParticleData->vSize;
			}
			else
			{
				e_pParticleData->vSize.x = x*frand(0,m_fRandomValue);
				e_pParticleData->vSize.y = y*frand(0,m_fRandomValue);
				e_pParticleData->vOriginalSize = e_pParticleData->vSize;			
			}
		}
		inline	void	SetSize(float e_fX,float e_fY){ x = e_fX; y = e_fY; }
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone();
		virtual char*	GetDataInfo();
		virtual char*	GetOutputDataString();
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString);
	};
	//while primitive is points ActSizeBlending and initSize just fetch x data
	class cPrtSizeActBlending:public cParticleBase,public Vector2
	{//add or minus
		bool	m_bAdd;
	public:
		DEFINE_TYPE_INFO()
		cPrtSizeActBlending()throw():m_bAdd(true){ x = y = 5.f; }
		virtual ~cPrtSizeActBlending(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			float	l_fAddX = x*e_fElpaseTime;
			float	l_fAddY = y*e_fElpaseTime;
			if(m_bAdd)
			{
				e_pParticleData->vSize.x+=l_fAddX;
				e_pParticleData->vSize.y+=l_fAddY;
			}
			else
			{
				e_pParticleData->vSize.x-l_fAddX>0?
					e_pParticleData->vSize.x-=l_fAddX:
					e_pParticleData->vSize.x = 0.f;
				e_pParticleData->vSize.y-l_fAddY>0?
					e_pParticleData->vSize.y-=l_fAddY:
					e_pParticleData->vSize.y = 0.f;
			}	
		}
		void	SetSize(float e_fX,float e_fY){ x = e_fX; y = e_fY; }
		void	SetAddOrSubtract( bool e_b ){ m_bAdd = e_b; }
		bool	*IsAddOrSubtract(){ return &m_bAdd; }
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone();
		virtual char*	GetDataInfo();
		virtual char*	GetOutputDataString();
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString);
	};
//end namespace FATMING_CORE
}

#endif