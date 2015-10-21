#include "../stdafx.h"
#include "SimplePrimitive.h"
#include "PuzzleImage.h"
#include "MPDINode.h"
namespace FATMING_CORE
{
	cMPDINodeImageData::cMPDINodeImageData()
	{
		m_pCurrentRenderObject = 0;
	}

	cMPDINodeImageData::cMPDINodeImageData(cMPDINodeImageData*e_pImageData)
	{
		m_pCurrentRenderObject = 0;
		SetData(e_pImageData->m_TimeVector,&e_pImageData->m_RenderObjectVector);
	}

	cTimeAndDataLinerUpdateInterface*	cMPDINodeImageData::Clone()
	{
		cTimeAndDataLinerUpdateInterface*l_pData = new cMPDINodeImageData(this);
		return l_pData;
	}

	cMPDINodeImageData::~cMPDINodeImageData()
	{

	}
	void	cMPDINodeImageData::Init()
	{
		cTimeAndDataLinerUpdateInterface::Init();
		m_pCurrentRenderObject = 0;
	}
	TiXmlElement*	cMPDINodeImageData::ToTiXmlElement()
	{
		return 0;
	}

	cBaseImage*		cMPDINodeImageData::GetCurrentData()
	{
		return m_pCurrentRenderObject; 
	}

	void	cMPDINodeImageData::InternalInit()
	{
		this->m_pCurrentRenderObject = 0;
	}

	void	cMPDINodeImageData::Update(float e_fElpaseTime)
	{
		//for linear interpolation
		float	l_fRestTimeToNextStep = 0.f;
		float	l_fNextStepTimeDiff = 0.f;
		float	l_fCurrentStepLerpValue = 0.f;
		bool	l_bEndPoint = false;
		//get current working point
		m_iCurrentWorkingIndex = UpdateTimeVectorDataIndex(e_fElpaseTime,&l_fCurrentStepLerpValue,&l_fRestTimeToNextStep,&l_fNextStepTimeDiff,&l_bEndPoint);
		if( m_iCurrentWorkingIndex == -1 )
			this->m_pCurrentRenderObject = 0;
		else
			m_pCurrentRenderObject = this->m_RenderObjectVector[m_iCurrentWorkingIndex];
	}


	void	cMPDINodeImageData::InvertOrder()
	{
		int	l_iSize = (int)m_TimeVector.size();
		int	l_iSize2 = (int)m_TimeVector.size()-1;
		std::vector<float>	l_TimeVector = m_TimeVector;
		std::vector<cBaseImage*>	l_LinerDataVector = m_RenderObjectVector;
		l_LinerDataVector.resize(l_iSize);
		for( int i=0;i<l_iSize;++i )
			l_LinerDataVector[i] = m_RenderObjectVector[i];
		for( int i=0;i<l_iSize;++i )
		{
			m_TimeVector[i] = l_TimeVector[l_iSize2-i];
			m_RenderObjectVector[i] = l_LinerDataVector[l_iSize2-i];
		}
	}

	void	cMPDINodeImageData::Clear()
	{
		m_RenderObjectVector.clear();
		m_TimeVector.clear();
	}

	void	cMPDINodeImageData::AddData(cBaseImage*e_pBaseImage,float e_fTime)
	{
		this->m_RenderObjectVector.push_back(e_pBaseImage);
		this->m_TimeVector.push_back(e_fTime);
	}

	bool	cMPDINodeImageData::ChangeData(int e_iIndex,float e_fTime,cBaseImage*e_pBaseImage)
	{
		if(IsLegalIndexInVector<cBaseImage*>(&m_RenderObjectVector,e_iIndex))
		{
			this->m_TimeVector[e_iIndex] = e_fTime;
			m_RenderObjectVector[e_iIndex] = e_pBaseImage;
			return true;
		}
		return false;
	}

	bool	cMPDINodeImageData::InsertData(int e_iIndex,float e_fTime,cBaseImage*e_pBaseImage)
	{
		if(IsLegalIndexInVector<cBaseImage*>(&m_RenderObjectVector,e_iIndex))
		{
			this->m_RenderObjectVector.insert(m_RenderObjectVector.begin()+e_iIndex,e_pBaseImage);
			return true;
		}
		return false;
	}

	bool	cMPDINodeImageData::RemoveDtaa(int e_iIndex)
	{
		if(IsLegalIndexInVector<cBaseImage*>(&m_RenderObjectVector,e_iIndex))
		{
			m_RenderObjectVector.erase(m_RenderObjectVector.begin()+e_iIndex);
			return true;
		}
		return false;
	}

	void	cMPDINodeImageData::SetData(std::vector<float>e_TimeVector,std::vector<cBaseImage*>*e_pPIDataVector)
	{
		this->m_TimeVector = e_TimeVector;
		m_RenderObjectVector.clear();
		size_t l_uiSize = e_pPIDataVector->size();
		m_RenderObjectVector.reserve(l_uiSize);
		for(size_t i=0;i<l_uiSize;++i)
			m_RenderObjectVector.push_back((*e_pPIDataVector)[i]);
	}
		

	cMPDINode::cMPDINode()
	{
		//m_pLinerDataContainer						=	nullptr;
		//m_pImageData								=	nullptr;
		//m_pPosLinerDataProcessor					=	nullptr;
		//m_pColorLinerDataProcessor					=	nullptr;
		//m_pSizeLinerDataProcessor					=	nullptr;
		//m_pRotationLinerDataProcessor				=	nullptr;
		m_bColorBlending							=	false;
		m_SrcBlendingMode							=	GL_SRC_ALPHA;
		m_DestBlendingMode							=	GL_ONE_MINUS_SRC_ALPHA;
		m_eMPDINodeType								=	eMPDI_NT_ANIMATION;
		m_pUserData									=	nullptr;
		m_pWorkingpImage							=	0;
		m_fMaxLinerDataPlayTime						=	0.f;
		m_vRotationAnglePosOffset					=	Vector3::Zero;
		m_vWorkingPosition							=	Vector3::Zero;
		m_vWorkingRotation							=	Vector3::Zero;
		m_vWorkingColor								=	Vector4::One;
		m_vWorkingSize								=	Vector2::Zero;
		m_matAnimationMatrix						=	cMatrix44::Identity;
		m_pLinerDataContainer						=	new cLinerDataContainer;
		m_pImageData								=	new cMPDINodeImageData();
		m_pPosLinerDataProcessor					=	new cLinerDataProcessor<Vector3>;
		m_pColorLinerDataProcessor					=	new cLinerDataProcessor<Vector4>;
		m_pSizeLinerDataProcessor					=	new cLinerDataProcessor<Vector2>;
		m_pRotationLinerDataProcessor				=	new cLinerDataProcessor<Vector3>;

		m_pLinerDataContainer->AddData(m_pImageData);
		m_pLinerDataContainer->AddData(m_pPosLinerDataProcessor);
		m_pLinerDataContainer->AddData(m_pColorLinerDataProcessor);
		m_pLinerDataContainer->AddData(m_pSizeLinerDataProcessor);
		m_pLinerDataContainer->AddData(m_pRotationLinerDataProcessor);
	}

	cMPDINode::cMPDINode(cMPDINode*e_pMPDINode)
	{
		this->SetName(e_pMPDINode->GetName());
		//m_pLinerDataContainer						=	nullptr;
		//m_pImageData								=	nullptr;
		//m_pPosLinerDataProcessor					=	nullptr;
		//m_pColorLinerDataProcessor					=	nullptr;
		//m_pSizeLinerDataProcessor					=	nullptr;
		//m_pRotationLinerDataProcessor				=	nullptr;
		m_bColorBlending							=	e_pMPDINode->m_bColorBlending;
		m_SrcBlendingMode							=	e_pMPDINode->m_SrcBlendingMode;
		m_DestBlendingMode							=	e_pMPDINode->m_DestBlendingMode;
		m_eMPDINodeType								=	eMPDI_NT_ANIMATION;
		m_vWorkingPosition							=	Vector3::Zero;
		m_vWorkingRotation							=	Vector3::Zero;
		m_vWorkingColor								=	Vector4::One;
		m_vWorkingSize								=	Vector2::Zero;
		m_vWorkingRotation							=	Vector3::Zero;
		m_matAnimationMatrix						=	cMatrix44::Identity;
		m_pUserData									=	nullptr;
		m_pWorkingpImage							=	0;
		m_vRotationAnglePosOffset					=	e_pMPDINode->m_vRotationAnglePosOffset;
		m_fMaxLinerDataPlayTime						=	e_pMPDINode->m_fMaxLinerDataPlayTime;
		//
		m_pLinerDataContainer						=	new cLinerDataContainer(e_pMPDINode->m_pLinerDataContainer);
		m_pImageData =								(cMPDINodeImageData*)m_pLinerDataContainer->m_ContainerVector[0];
		m_pPosLinerDataProcessor =					(cLinerDataProcessor<Vector3>*)m_pLinerDataContainer->m_ContainerVector[1];
		m_pColorLinerDataProcessor =				(cLinerDataProcessor<Vector4>*)m_pLinerDataContainer->m_ContainerVector[2];
		m_pSizeLinerDataProcessor =					(cLinerDataProcessor<Vector2>*)m_pLinerDataContainer->m_ContainerVector[3];
		m_pRotationLinerDataProcessor =				(cLinerDataProcessor<Vector3>*)m_pLinerDataContainer->m_ContainerVector[3];

		Frame*l_pFrame = e_pMPDINode->GetFirstChild();
		while( l_pFrame )
		{
			Frame*CloneFrame = reinterpret_cast<Frame*>(l_pFrame->Clone());
			this->AddChildToLast(CloneFrame);
			l_pFrame = l_pFrame->GetNextSibling();
		}
	}


	cMPDINode::~cMPDINode()
	{
		SAFE_DELETE(m_pLinerDataContainer);
		SAFE_DELETE(m_pUserData);
		m_pImageData = 0;
		m_pPosLinerDataProcessor = 0;
		m_pColorLinerDataProcessor = 0;
		m_pSizeLinerDataProcessor = 0;
		m_pRotationLinerDataProcessor = 0;
		SetDestroyConnectionWhileDestoruction(false);
		Frame*l_pFirstChild = this->GetFirstChild();
		while( l_pFirstChild )
		{
			l_pFirstChild->SetDestroyConnectionWhileDestoruction(false);
			Frame*l_pDeleteObject = l_pFirstChild;
			l_pFirstChild = l_pFirstChild->GetNextSibling();
			SAFE_DELETE(l_pDeleteObject);
		}
	}
	extern const FLOAT    FRAME_DIRTY_WORLD_CACHE = 1e10f;
	void	cMPDINode::UpdateCachedWorldTransformIfNeeded()
	{
		Frame::UpdateCachedWorldTransformIfNeeded();
		m_CachedWorldTransform = m_CachedWorldTransform*this->m_matAnimationMatrix;
	}

	float	cMPDINode::GetMaxLinerDataPlayTime()
	{
		if( m_fMaxLinerDataPlayTime == 0.f )
		{
			RefreshMaxLinerDataPlayTime();
		}	
		return m_fMaxLinerDataPlayTime;
	}

	void	cMPDINode::RefreshMaxLinerDataPlayTime()
	{
		m_fMaxLinerDataPlayTime = 0.f;
		size_t	l_uiSize = m_pLinerDataContainer->m_ContainerVector.size();
		for( size_t i=0;i<l_uiSize;++i )
		{
			float	l_fEndTime = m_pLinerDataContainer->m_ContainerVector[i]->GetEndTime();
			if( l_fEndTime > m_fMaxLinerDataPlayTime )
				m_fMaxLinerDataPlayTime = l_fEndTime;
		}	
	}

	void	cMPDINode::InternalInit()
	{
		GetMaxLinerDataPlayTime();
		m_pWorkingpImage = 0;
		m_pLinerDataContainer->Init();
		//m_pPosLinerDataProcessor->Init();
		//m_pColorLinerDataProcessor->Init();
		//m_pSizeLinerDataProcessor->Init();
		//m_pRotationLinerDataProcessor->Init();
		//m_pImageData->Init();
	}  

	void	cMPDINode::InternalUpdate(float e_fElpaseTime)
	{
		m_pLinerDataContainer->Update(e_fElpaseTime);
		if(m_pImageData->IsDuringWorking())
		{
			m_pWorkingpImage = m_pImageData->GetCurrentData();
		}
		if(m_pPosLinerDataProcessor->IsDuringWorking())
		{
			m_vWorkingPosition = m_pPosLinerDataProcessor->GetCurrentData();
		}
		if(m_pColorLinerDataProcessor->IsDuringWorking())
		{
			m_vWorkingColor = m_pColorLinerDataProcessor->GetCurrentData();
		}
		if(m_pSizeLinerDataProcessor->IsDuringWorking())
		{
			m_vWorkingSize = m_pSizeLinerDataProcessor->GetCurrentData();
		}
		if(m_pRotationLinerDataProcessor->IsDuringWorking())
		{
			m_vWorkingRotation = m_pRotationLinerDataProcessor->GetCurrentData();
		}
		if( m_pWorkingpImage != nullptr )
		{
			Vector2	l_vScale(m_vWorkingSize.x/m_pWorkingpImage->GetWidth(),m_vWorkingSize.y/m_pWorkingpImage->GetHeight());
			Vector2	l_vImageSize(m_vWorkingSize.x,m_vWorkingSize.y);
			Vector2	l_vImageHalfSize = l_vImageSize/2.f;		
			Vector2	l_vOffsetPos((float)m_pWorkingpImage->GetOffsetPos()->x,(float)m_pWorkingpImage->GetOffsetPos()->y);
			Vector2	l_vOriginalSize((float)m_pWorkingpImage->GetOriginalSize().x,(float)m_pWorkingpImage->GetOriginalSize().y);
			Vector2	l_vHalfOriginalSize = l_vOriginalSize/2.f;
			Vector2	l_vCenterToOffset = l_vOffsetPos-l_vHalfOriginalSize;

			Vector2	l_vFinalOffsetPos = l_vOriginalSize/2;
			//although this one is not correct but work for some function.
			Vector4	l_vEntendImagePoint = GetExtendImagePoint(m_eAnchorType,l_vImageHalfSize,l_vImageSize,1.f);
			Vector4	l_vDrawRect = l_vEntendImagePoint;
			if( m_eAnchorType == eIAT_CENTER_CENTER )
			{
				l_vCenterToOffset.x *= l_vScale.x;
				l_vCenterToOffset.y *= l_vScale.y;
				l_vDrawRect = Vector4(0,0,l_vImageSize.x,l_vImageSize.y);
			}
			m_2DVertices.vPos[0].x = 0;					m_2DVertices.vPos[0].y = 0;					m_2DVertices.vPos[0].z = 0.f;
			m_2DVertices.vPos[1].x = l_vImageSize.x;	m_2DVertices.vPos[1].y =	0;				m_2DVertices.vPos[1].z = 0.f;
			m_2DVertices.vPos[2].x = 0;					m_2DVertices.vPos[2].y = l_vImageSize.y;	m_2DVertices.vPos[2].z = 0.f;
			m_2DVertices.vPos[3].x = l_vImageSize.x;	m_2DVertices.vPos[3].y= l_vImageSize.y;		m_2DVertices.vPos[3].z = 0.f;
			AssignUVDataToTriangleStrip(m_pWorkingpImage->GetUV(),(float*)&m_2DVertices.fUV[0],false);
			//
			////not quaterion is because quaterion has more complex calculation.
			Vector3	l_vPos = Vector3(l_vFinalOffsetPos.x,l_vFinalOffsetPos.y,0.f);
			m_matAnimationMatrix = cMatrix44::TranslationMatrix(l_vPos+m_vWorkingPosition);
			m_matAnimationMatrix *= cMatrix44::RotationMatrix(m_vWorkingRotation);
			m_matAnimationMatrix *= cMatrix44::TranslationMatrix(m_vRotationAnglePosOffset+l_vCenterToOffset);
		}
		else
		{
			m_matAnimationMatrix = cMatrix44::TranslationMatrix(m_vWorkingPosition);
			m_matAnimationMatrix *= cMatrix44::RotationMatrix(m_vWorkingRotation);
			m_matAnimationMatrix *= cMatrix44::TranslationMatrix(m_vRotationAnglePosOffset);
		}
		this->SetCachedWorldTransformDirty();
	}

	void	cMPDINode::InternalRender()
	{
		if( m_pWorkingpImage && m_vWorkingColor.a > 0.f )
		{
			if( !this->IsAnimationDone() || this->m_bStayAtLastFrame )
			{
				m_pWorkingpImage->ApplyImage();
				cMatrix44	l_RenderMatrix = cMatrix44::Identity;
				if( this->GetParent() )
				{
					l_RenderMatrix = this->GetParent()->GetWorldTransform();
				}
				//cMatrix44	l_RenderMatrix = this->GetWorldTransform();
				GLenum	l_OriginalSrc,l_OriginalDest;
				if(this->m_bColorBlending)
				{
					glGetIntegerv(GL_BLEND_SRC,(GLint*)&l_OriginalSrc);
					glGetIntegerv(GL_BLEND_DST,(GLint*)&l_OriginalDest);
					glBlendFunc(m_SrcBlendingMode,m_DestBlendingMode);
				}
				DrawQuadWithMatrix((float*)m_2DVertices.vPos,(float*)m_2DVertices.fUV,m_vWorkingColor,l_RenderMatrix,3,1);
				if(this->m_bColorBlending)
				{
					glBlendFunc(l_OriginalSrc,l_OriginalDest);
				}
			}
		}
	}

	void	cMPDINode::RearrangeTime(float e_fNewTime)
	{
		m_pLinerDataContainer->Rearrange(e_fNewTime);
	}

	void	cMPDINode::RearrangeTimeByPercent(float e_fPercenttage)
	{
		float	l_fEndTime = m_pLinerDataContainer->GetEndTime();
		m_pLinerDataContainer->Rearrange(l_fEndTime*e_fPercenttage);
	}

	void	cMPDINode::RenderByGlobalTime(float e_fTime)
	{
		UpdateByGlobalTime(e_fTime);
		this->Render();
	}
	void	cMPDINode::InvertOrder()
	{
		m_pPosLinerDataProcessor->InvertOrder();
		m_pColorLinerDataProcessor->InvertOrder();
		m_pSizeLinerDataProcessor->InvertOrder();
		m_pRotationLinerDataProcessor->InvertOrder();
	}
	//start and last time.
	float	cMPDINode::GetEndTime()
	{
		return 0.1f;
		if( m_fTotalPlayTime == 0.f )
		{
			m_fTotalPlayTime = m_pLinerDataContainer->GetEndTime();
		}
		return this->m_fStartTime+m_fTotalPlayTime;
	}
	void	cMPDINode::InternalDestroy()
	{
		m_pPosLinerDataProcessor->Clear();
		m_pColorLinerDataProcessor->Clear();
		m_pSizeLinerDataProcessor->Clear();
		m_pRotationLinerDataProcessor->Clear();
		m_pImageData->Clear();
	}

	cLinerDataProcessor<Vector3>*	cMPDINode::GetPosData()
	{
		return m_pPosLinerDataProcessor;
	}

	cLinerDataProcessor<Vector4>*	cMPDINode::GetColorData()
	{
		return m_pColorLinerDataProcessor;	
	}

	cLinerDataProcessor<Vector2>*	cMPDINode::GetSizeData()
	{
		return m_pSizeLinerDataProcessor;
	}

	cLinerDataProcessor<Vector3>*	cMPDINode::GetRotationData()
	{
		return m_pRotationLinerDataProcessor;
	}

	cMPDINodeImageData*	cMPDINode::GetImageData()
	{
		return m_pImageData;
	}

	void	cMPDINode::InternalDebugRender()
	{
		cMatrix44	l_RenderMatrix = cMatrix44::Identity;
		if( this->GetParent() )
		{
			l_RenderMatrix = this->GetParent()->GetWorldTransform();
		}
		this->m_pPosLinerDataProcessor->DebugRender(true,true,Vector4::Red,l_RenderMatrix);
	}

	void	cMPDINode::InternalUpdateByGlobalTime(float e_fGlobalTime)
	{
		//if( this->m_fPastTime != e_fGlobalTime )
		{
			this->SetAnimationLoop(true);
			//this->Init();
			m_fPastTime = 0.f;
			this->Update(e_fGlobalTime);
		}	
	}
//end namespace FATMING_CORE
}