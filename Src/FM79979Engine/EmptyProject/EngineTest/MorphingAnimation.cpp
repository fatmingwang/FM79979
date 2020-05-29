#include "stdafx.h"
#include "MorphingAnimation.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cFMMorphingAnimation);
	TYPDE_DEFINE_MARCO(cFMMorphingAnimationVector);
	Vector3 cFMMorphingAnimation::sTimeAndPosAnimationData::UpdateAnimationByGlobalTime(float e_fGlobalTime)
	{
		if (m_FormKeyFrames.size() == 0)
			return Vector3::Zero;
		//find keyframe before and after and do linear interpolation between them
		FloatTocVector3Map::iterator prevKey(m_FormKeyFrames.lower_bound(e_fGlobalTime));
		FloatTocVector3Map::iterator nextKey(prevKey);
		if ((prevKey == m_FormKeyFrames.end()) || ((prevKey != m_FormKeyFrames.begin()) && (prevKey->first > e_fGlobalTime)))
			--prevKey;
		// because m_FormKeyFrames.size() != 0 prevKey should be a valid iterator here
		assert(prevKey != m_FormKeyFrames.end());

		if ((prevKey == nextKey) || (nextKey == m_FormKeyFrames.end()))
		{
			return prevKey->second;
		}

		float time0 = prevKey->first;
		float time1 = nextKey->first;
		float timeRange = time1 - time0;
		float l_fTimeDis = (e_fGlobalTime - time0) / timeRange;

		const Vector3& m0 = prevKey->second;
		const Vector3& m1 = nextKey->second;
		auto l_vPos = (m1 - m0)*l_fTimeDis + m0;
		return l_vPos;
	}
	
	void	cFMMorphingAnimation::sTimeAndPosAnimationData::AssignData(int e_iCount, float*e_pfTimeArray, Vector3*e_pfPosArray)
	{
		m_FormKeyFrames.clear();
		for (int i = 0; i < e_iCount; ++i)
		{
			m_FormKeyFrames[e_pfTimeArray[i]] = e_pfPosArray[i];
		}
	}
	void cFMMorphingAnimation::InternalInit()
	{
	}

	void cFMMorphingAnimation::InternalUpdate(float e_fElpaseTime)
	{
		float l_fPastTime = this->m_fPastTime*this->m_fSpeed;
		if (m_bInvertOrder)
		{
			float l_fEndTime = this->GetEndTime();
			l_fPastTime = l_fEndTime-(l_fEndTime-this->m_fPastTime);
		}
		UpdateByGlobalTime(l_fPastTime);
	}

	void cFMMorphingAnimation::InternalRender()
	{
		if (m_pMeshBuffer)
		{
			if (m_pMeshBuffer->pTargetTexture)
				m_pMeshBuffer->pTargetTexture->ApplyImage();
			auto l_uiSize = (GLsizei)m_pMeshBuffer->IndexBuffer.uiDataCount;
			RenderVertexByIndexBuffer(this->GetWorldTransform(), 3, (float*)&m_RenderVertex[0],
				(float*)&this->m_pMeshBuffer->UVBuffer.pData[0], (float*)&this->m_pMeshBuffer->ColorBuffer.pData[0],
				(float*)&this->m_pMeshBuffer->IndexBuffer.pData, (int)l_uiSize);
		}
	}

	cFMMorphingAnimation::cFMMorphingAnimation(c2DMeshObject::sMeshBuffer * e_pMeshBuffer, std::vector<sTimeAndPosAnimationData>*e_pTimeAndPosAnimationVector)
	{
		m_pTimeAndPosAnimationVector = e_pTimeAndPosAnimationVector;
		m_fEndTime = 0.f;
		m_fSpeed = 1.f;
		m_bInvertOrder = false;
	}

	cFMMorphingAnimation::cFMMorphingAnimation(cFMMorphingAnimation * e_pFMMorphingAnimation)
	{
		m_fEndTime = e_pFMMorphingAnimation->m_fEndTime;
		m_fSpeed = e_pFMMorphingAnimation->m_fSpeed;
		m_pTimeAndPosAnimationVector = e_pFMMorphingAnimation->m_pTimeAndPosAnimationVector;
		m_pMeshBuffer = e_pFMMorphingAnimation->m_pMeshBuffer;
		m_RenderVertex = e_pFMMorphingAnimation->m_RenderVertex;
		m_bInvertOrder = m_bInvertOrder;
	}

	cFMMorphingAnimation::~cFMMorphingAnimation()
	{
	}


	void cFMMorphingAnimation::UpdateByGlobalTime(float e_fGlobalTime)
	{
		auto l_uiSize = m_pTimeAndPosAnimationVector->size();
		if (m_RenderVertex.size() != l_uiSize)
		{
			m_RenderVertex.resize(l_uiSize);
		}
		float l_fTargetTime = m_fSpeed* e_fGlobalTime;
		for (auto i=0;i< l_uiSize;++i)
		{
			m_RenderVertex[i] = (*m_pTimeAndPosAnimationVector)[i].UpdateAnimationByGlobalTime(l_fTargetTime);
		}
	}

	void cFMMorphingAnimation::RenderByGlobalTime(float e_fTime)
	{
		UpdateByGlobalTime(e_fTime);
		InternalRender();
	}

	void cFMMorphingAnimation::RearrangeTime(float e_fNewTime)
	{
		m_fSpeed = this->GetEndTime()/ e_fNewTime;
	}

	void cFMMorphingAnimation::RearrangeTimeByPercent(float e_fPercenttage)
	{
		m_fSpeed = e_fPercenttage;
	}

	void cFMMorphingAnimation::InvertOrder()
	{
		m_bInvertOrder = !m_bInvertOrder;
	}

	float cFMMorphingAnimation::GetEndTime()
	{
		return m_fEndTime*this->m_fSpeed;
	}
	cFMMorphingAnimationVector::cFMMorphingAnimationVector()
	{
		m_pAnimationDataMap = new cAnimationDataMap(this);
	}

	cFMMorphingAnimationVector::~cFMMorphingAnimationVector()
	{
		SAFE_RELEASE(m_pAnimationDataMap, this);
	}

	cFMMorphingAnimationVector::cAnimationDataMap::cAnimationDataMap(NamedTypedObject * e_pObject)
		:cSmartObject(e_pObject)
	{
	}

	cFMMorphingAnimationVector::cAnimationDataMap::~cAnimationDataMap()
	{
		DELETE_MAP(m_AnimationMap);
	}
//<MorphingAnimationRoot Version="20200522">
//    <MorphingAnimation Name="15686247788274" TimeList="0.00000,0.25000,0.50000,1.00000" VertexCount="56">
//        <VertexData Index="0" PosVectorDataSize="48" OptimizeTimeVectorDataSize="4" OptimizePosVectorDataSize="12" />
//        <VertexData Index="1" PosVectorDataSize="48" OptimizeTimeVectorDataSize="4" OptimizePosVectorDataSize="12" />
//    </MorphingAnimation>
//</MorphingAnimationRoot>
	bool cFMMorphingAnimationManager::MyParse(TiXmlElement * e_pRoot)
	{
		SAFE_DELETE(m_pMBFile);
		m_uiCurrentDataPos = 0;
		auto l_strVersion = e_pRoot->Attribute(L"Version");
		auto l_strTIFileName = e_pRoot->Attribute(CHAR_TO_WCHAR_DEFINE(TI_ELEMENT_NAME));
		auto l_strPI_morphing = e_pRoot->Attribute(CHAR_TO_WCHAR_DEFINE(MX_ELEMENT_NAME));
		if (!l_strPI_morphing)
		{
			this->m_strErrorMsg += CHAR_TO_WCHAR_DEFINE(MX_ELEMENT_NAME);
			this->m_strErrorMsg += L" attribute not exists!";
			return false;
		}
		//<MorphingAnimationRoot Version = "20200522" PI_tri = "111.ti">
		if (GetInt(l_strVersion) == FM_MORPHING_ANIMATION_VER)
		{
			if (!Process_TRIANGLE_AND_DRAW_INDEX_ElementData(l_strPI_morphing))
			{
				this->m_strErrorMsg += l_strPI_morphing;
				this->m_strErrorMsg += L" parse faild!\n";
				return false;
			}
			//m_pMBFile
			e_pRoot = e_pRoot->FirstChildElement();
			while (e_pRoot)
			{
				if (!wcscmp(e_pRoot->Value(), L"MorphingAnimation"))
				{
					Process_MorphingAnimationData(e_pRoot);
				}
				e_pRoot = e_pRoot->NextSiblingElement();
			}
		}
		else
		{
			this->m_strErrorMsg += L"Version not match ";
			this->m_strErrorMsg += l_strVersion;
			this->m_strErrorMsg += L"\n";
		}
		SAFE_DELETE(m_pMBFile);
		return true;
	}

	bool cFMMorphingAnimationManager::Process_MorphingAnimationData(TiXmlElement*e_pTiXmlElement)
	{
		auto l_strName = e_pTiXmlElement->Attribute(L"Name");
		if (l_strName)
		{
			auto l_p2DMeshObject = m_pCurrent2DMeshObjectManager->GetObject(l_strName);
			if (l_p2DMeshObject)
			{
				std::vector<cFMMorphingAnimation::sTimeAndPosAnimationData>*l_pVector = new std::vector<cFMMorphingAnimation::sTimeAndPosAnimationData>;
				cFMMorphingAnimation*l_pFMMorphingAnimation = new cFMMorphingAnimation(l_p2DMeshObject->GetMeshBuffer(), l_pVector);
				m_pCurrentFMMorphingAnimationVector->m_pAnimationDataMap->m_AnimationMap[l_pFMMorphingAnimation] = l_pVector;
				l_pFMMorphingAnimation->SetName(l_strName);
				//this->AddObjectNeglectExist(l_pFMMorphingAnimation);
				auto l_pFirstChildElement = e_pTiXmlElement->FirstChildElement();
				char*l_pMBData = (char*)this->m_pMBFile->GetDataFile(m_uiCurrentDataPos);
				while (l_pFirstChildElement)
				{
					if (!wcscmp(l_pFirstChildElement->Value(), L"VertexData"))
					{
						auto l_strOptimizeTimeVectorDataSize = l_pFirstChildElement->Attribute(L"OptimizeTimeVectorDataSize");
						auto l_strOptimizePosVectorDataSize = l_pFirstChildElement->Attribute(L"OptimizePosVectorDataSize");
						if (l_strOptimizeTimeVectorDataSize && l_strOptimizePosVectorDataSize)
						{
							int l_iTimeSize = GetInt(l_strOptimizeTimeVectorDataSize);
							int l_iPosSize = GetInt(l_strOptimizePosVectorDataSize);
							float*l_pfTimeArray = (float*)l_pMBData;
							l_pMBData += l_iTimeSize;
							Vector3*l_pvPosArray = (Vector3*)l_pMBData;
							l_pMBData += l_iPosSize;
							int l_iCount = l_iTimeSize/sizeof(float);
							cFMMorphingAnimation::sTimeAndPosAnimationData l_TimeAndPosAnimationData;
							l_TimeAndPosAnimationData.AssignData(l_iCount, l_pfTimeArray, l_pvPosArray);
							l_pVector->push_back(l_TimeAndPosAnimationData);
							m_uiCurrentDataPos += l_iTimeSize;
							m_uiCurrentDataPos += l_iPosSize;
						}
						else
						{
							this->m_strErrorMsg += L"VertexData attribute get wrong\n";
							return false;
						}
					}
					l_pFirstChildElement = l_pFirstChildElement->NextSiblingElement();
				}
				return true;
			}
			else
			{
				this->m_strErrorMsg += L"2D mesh object";
				this->m_strErrorMsg += l_strName;
				this->m_strErrorMsg += L" not found!\n";
			}
		}
		return false;
	}

	bool cFMMorphingAnimationManager::Process_TRIANGLE_AND_DRAW_INDEX_ElementData(const wchar_t*e_strTIFileName)
	{
		std::string l_strDirectory = this->m_strCurrentDirectory;
		l_strDirectory += ValueToString(e_strTIFileName);
		if(m_pCurrentFMMorphingAnimationVector->GetObjectByFileName(l_strDirectory.c_str()))
		{
			return true;
		}
		return false;
	}
	cFMMorphingAnimationManager::cFMMorphingAnimationManager()
	{
		m_pMBFile = nullptr;
		m_uiCurrentDataPos = 0;
		m_pCurrentFMMorphingAnimationVector = nullptr;
		m_pCurrent2DMeshObjectManager = nullptr;
	}
	cFMMorphingAnimationManager::~cFMMorphingAnimationManager()
	{
		SAFE_DELETE(m_pMBFile);
	}
	NamedTypedObject * cFMMorphingAnimationManager::GetObjectByFileName(const char * e_strFileName)
	{
		auto l_pObject = cNamedTypedObjectVector::GetObjectByFileName(e_strFileName);
		if (l_pObject)
			return l_pObject;
		if (this->ParseWithMyParse(e_strFileName))
		{
			return cNamedTypedObjectVector::GetObjectByFileName(e_strFileName);
		}
		return nullptr;
	}
}