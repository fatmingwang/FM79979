#ifndef PRT_START_POSITION_STYLE_H
#define PRT_START_POSITION_STYLE_H
namespace FATMING_CORE
{
	//================================================================
	//here divide two part one part is implement data,
	//another part is interface for transform the current interface to wanted by ChangeProperty(  Data.p and Data.cpp )
	//================================================================
	class cPrtStartPositionInitBySquareRange:public cParticleBase
	{
		GETP_SET_DEC(float,m_fHeight,GetHeight,SetHeight);
		GETP_SET_DEC(float,m_fWidth,GetWidth,SetWidth);
		GETP_SET_DEC(float,m_fDeepth,GetDeepth,SetDeepth);
	public :
		DEFINE_TYPE_INFO()
		cPrtStartPositionInitBySquareRange(){ m_fHeight = 10.f; m_fWidth =  10.f; m_fDeepth = 10.f; }
		virtual ~cPrtStartPositionInitBySquareRange(){}
		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			float l_fXPos = frand(m_fWidth,-m_fWidth);
			float l_fYPos = frand(m_fHeight,-m_fHeight);
			float l_fZPos = frand(m_fDeepth,-m_fDeepth);
			//for 2D
			//Vector3	l_vPos( l_fXPos,this->m_fHeight,l_fZPos);
			//for 3D
			Vector3	l_vPos( l_fXPos,l_fYPos,l_fZPos);
			e_pParticleData->vPos += l_vPos;
			e_pParticleData->vOriginalPos = e_pParticleData->vPos;
		}
		//usage explanation
		static char*	UsageExplanation;
		//
		virtual NamedTypedObject*  Clone();
		virtual char*	GetDataInfo();
		virtual char*	GetOutputDataString();
		//input the output data string,and analyze it
		virtual bool	SetDataByDataString(char*e_pString);
	};

	enum	eAttachFrameType
	{
		eAFT_CAMERA = 0,	//when the flag this on,the default position will be camera's pos
		eAFT_MODEL,			//frame
		eAFT_FRAME_OF_FRAME,//the frame of frame,cdxdxut, I am lazy to write more flexible,support Frame
		eAFT_FRAME_OF_MESH,//the frame os mesh,d3dxframe,cAnimationAgent
	};
	wchar_t*	AttachFrameTypeToString(eAttachFrameType e_eAttachFrameType);
	//============================================
	//all velocity will transform by model's transform
	//============================================
	class cPrtStartPositionInitByFrame:public cParticleBase
	{
		union
		{
			Frame*	m_pSubFrameOfModel;//for cddxut cddxutframe
			cMatrix44*	m_pD3DMatrix;//for animation
		};
		//destination subframe name
		wchar_t	m_psSubFrameName[MAX_PATH];
		//to get data from 
		GET_SET_DEC(eAttachFrameType,m_eAttachFrameType,GetAttachFramType,SetAttachFrameType);
		//destination frame
		GET_SET_DEC(Frame*,m_pFrame,GetFrame,SetFrame);
		GET_SET_DEC(Vector3,m_vOffsetPos,GetOffsetPos,SetOffsetPos);
	public :
		DEFINE_TYPE_INFO()
		cPrtStartPositionInitByFrame(){ m_pFrame = 0; memset(m_psSubFrameName,0,sizeof(m_psSubFrameName)); m_pSubFrameOfModel = 0; m_pD3DMatrix = 0; m_eAttachFrameType = eAFT_CAMERA; }
		virtual ~cPrtStartPositionInitByFrame(){}

		virtual inline void	Update(float e_fElpaseTime,int e_iIndex,sParticleData*e_pParticleData)
		{
			//Vector3	l_vOffsetPos = cMatrix44::StripPosFromMatrix(m_pFrame->GetWorldTransform()).TransformVector(m_vOffsetPos);
			switch(m_eAttachFrameType)
			{
				case eAFT_CAMERA:
				case eAFT_MODEL:
				{
					assert(0);
					//e_pParticleData->vPos = m_pFrame->GetWorldPosition() + l_vOffsetPos;
					//e_pParticleData->vVelocity = StripPosFromMatrix(m_pFrame->GetWorldTransform()).TransformVector(e_pParticleData->vVelocity);
				}
					break;
				case eAFT_FRAME_OF_FRAME:
					if( m_pSubFrameOfModel )
					{
						assert(0);
						//e_pParticleData->vPos = m_pSubFrameOfModel->GetWorldPosition() + l_vOffsetPos;
						//e_pParticleData->vVelocity = StripPosFromMatrix(m_pSubFrameOfModel->GetWorldTransform()).TransformVector(e_pParticleData->vVelocity);
					}
					break;
				case eAFT_FRAME_OF_MESH:
				{
					if( m_pSubFrameOfModel )
					{
						assert(0);
						//e_pParticleData->vPos = m_pSubFrameOfModel->GetWorldPosition() + l_vOffsetPos;
						//e_pParticleData->vVelocity = (*(cMatrix44*)m_pD3DMatrix).TransformVector(e_pParticleData->vVelocity);
					}
					break;
				}
			}
		}
		wchar_t*	GetSubFrameName(){ return m_psSubFrameName; }
		void	SetSubFrameName(wchar_t*e_pString);
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