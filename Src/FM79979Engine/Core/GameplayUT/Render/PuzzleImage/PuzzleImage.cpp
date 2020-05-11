#include "PuzzleImage.h"
#include "../../GameApp/GameApp.h"
#include "../CommonRender/BaseImage.h"
#include "../CommonRender/NumeralImage.h"
namespace FATMING_CORE
{

	bool	g_bImageLoaderForFetchPixelData = false;
	//sometimes we only need to PI data but no pixels data
	bool    g_bSkipImageLoad = false;
	bool cPuzzleImage::m_sbSortPIFileAsOriginal = false;

	TYPDE_DEFINE_MARCO(cPuzzleImage);
	TYPDE_DEFINE_MARCO(cPuzzleImageUnit);
	cImageIndexOfAnimation::cImageIndexOfAnimation(bool e_bNewNameList)
	{
		m_pNameList = nullptr;
		if (e_bNewNameList)
		{
			m_pNameList = new std::vector<std::wstring>;
		}
		m_pfEndTime = nullptr;
	}
	cImageIndexOfAnimation::cImageIndexOfAnimation(cImageIndexOfAnimation*e_pImageIndexOfAnimation)
	{
		SetName(e_pImageIndexOfAnimation->GetName());
		m_pfEndTime = 0;
	    m_pNameList = 0;
	    if( e_pImageIndexOfAnimation->m_pNameList )
	    {
	        m_pNameList = new std::vector<std::wstring>;
            *m_pNameList = *e_pImageIndexOfAnimation->m_pNameList;
        }
        m_ImageAnimationDataList = e_pImageIndexOfAnimation->m_ImageAnimationDataList;
	}
	cImageIndexOfAnimation::~cImageIndexOfAnimation()
	{
		SAFE_DELETE(m_pNameList);
		SAFE_DELETE(m_pfEndTime);
	}
	void    cImageIndexOfAnimation::GenerateImageIndexByPI(cPuzzleImage*e_pPI,std::vector<std::wstring>   *e_pNameList)
	{
	    size_t  l_iSize = e_pNameList->size();
	    m_ImageAnimationDataList.resize(l_iSize);
	    for( size_t i=0;i<l_iSize;++i )
	    {
			m_ImageAnimationDataList[i].iIndex = e_pPI->GetObjectIndexByName((*e_pNameList)[i].c_str());
	    }
	}

	void    cImageIndexOfAnimation::Clear()
	{
		if (m_pNameList)
		{
			m_pNameList->clear();
		}
		m_ImageAnimationDataList.clear();
	}
	void    cImageIndexOfAnimation::AddNameObject(const wchar_t*e_strName, int e_iIndex, float e_fTimeGap)
	{
		std::wstring    l_strName = e_strName;
		m_pNameList->push_back(l_strName);
		m_ImageAnimationDataList.push_back(sImageIndexAndTimeGap(e_iIndex, e_fTimeGap));
	}
	void    cImageIndexOfAnimation::RemoveNameObject(int e_iIndex)
	{
		m_pNameList->erase(m_pNameList->begin() + e_iIndex);
		m_ImageAnimationDataList.erase(m_ImageAnimationDataList.begin() + e_iIndex);
	}
	float cImageIndexOfAnimation::GetEndTime()
	{
		if (!m_pfEndTime)
		{
			m_pfEndTime = new float;
			*m_pfEndTime = 0.f;
			size_t	l_iSize = m_ImageAnimationDataList.size();
			for (size_t i = 0; i < l_iSize; ++i)
			{
				*m_pfEndTime += m_ImageAnimationDataList[i].fTimeGap;
			}
		}
		return *m_pfEndTime;
	}

	int	cImageIndexOfAnimation::GetImageIndexByCurrentTime( float e_fTime )
	{
		float	l_fModulus = UT::GetFloatModulus(e_fTime,this->GetEndTime());
		assert(m_ImageAnimationDataList.size()>0);
		int	l_iIndex = 0;
		float	l_fTargetGap = m_ImageAnimationDataList[l_iIndex].fTimeGap;
		while( l_fModulus > l_fTargetGap )
		{
			l_fModulus -= l_fTargetGap;
			++l_iIndex;
			l_fTargetGap = m_ImageAnimationDataList[l_iIndex].fTimeGap;
		}
		return m_ImageAnimationDataList[l_iIndex].iIndex;
	}
		
	cPuzzleImageUnit::cPuzzleImageUnit(sPuzzleData*e_pPuzzleData,cPuzzleImage*e_pPuzzleImageParent)
	:cBaseImage(e_pPuzzleImageParent)
	{
		SetName(e_pPuzzleData->strFileName);
		m_pNext = nullptr;
		m_pPrior = nullptr;
		//m_pImageShapePointVector = nullptr;
		memcpy(this->m_fUV,e_pPuzzleData->fUV,sizeof(float)*4);
		this->m_OffsetPos = e_pPuzzleData->OffsetPos;
		this->m_iWidth  = e_pPuzzleData->Size.x;
		this->m_iHeight = e_pPuzzleData->Size.y;
		this->m_OriginalSize = e_pPuzzleData->OriginalSize;
		m_pPuzzleData = e_pPuzzleData;
	}

	cPuzzleImageUnit::cPuzzleImageUnit(cPuzzleImageUnit*e_pPuzzleImageUnit):cBaseImage(e_pPuzzleImageUnit)
	{
		SetName(e_pPuzzleImageUnit->GetName());
		//m_pImageShapePointVector = e_pPuzzleImageUnit->m_pImageShapePointVector;
		m_pPuzzleData = e_pPuzzleImageUnit->m_pPuzzleData;
		m_pNext = e_pPuzzleImageUnit->GetNext();
		m_pPrior = e_pPuzzleImageUnit->GetPrior();
	}
	cPuzzleImageUnit::cPuzzleImageUnit(cBaseImage*e_pBaseImage):cBaseImage(e_pBaseImage)
	{
		m_pPuzzleData = nullptr;
		//m_pImageShapePointVector = nullptr;
		m_pNext = nullptr;
		m_pPrior = nullptr;
	}

	cPuzzleImageUnit::~cPuzzleImageUnit()
	{
	}

	//<cPuzzleImageUnit PI="" PIUnit="" Name="" Pos="" Color=""/>
	cPuzzleImageUnit*		cPuzzleImageUnit::GetMe(TiXmlElement*e_pElement,bool e_bClone)
	{
		cPuzzleImageUnit*l_pPuzzleImageUnit = 0;
		cPuzzleImage*l_pPI = 0;
		std::wstring	l_strNewName;
		ELEMENT_VALUE_ASSERT_CHECK(e_pElement,cPuzzleImageUnit::TypeID);
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Name")
			{
				l_strNewName = l_strValue;
			}
			else
			COMPARE_NAME("PI")
			{
				l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
			}
			else
			COMPARE_NAME("PIUnit")
			{
				if( l_pPI )
					l_pPuzzleImageUnit = l_pPI->GetObject(l_strValue);
				if( !l_pPuzzleImageUnit )
				{
					std::wstring	l_strErrorMsg = L"PIUnit not exist:";
					l_strErrorMsg += l_strValue;
					FMLog::LogWithFlag(l_strErrorMsg.c_str(), CORE_LOG_FLAG);
				}
			}
			else
			IMAGE_COMMON_PARAMETERS(l_pPuzzleImageUnit)
		PARSE_NAME_VALUE_END
		if( !e_bClone )
			return l_pPuzzleImageUnit;
		cPuzzleImageUnit*l_pClone = 0;
		if( l_pPuzzleImageUnit )
		{
			l_pClone = dynamic_cast<cPuzzleImageUnit*>(l_pPuzzleImageUnit->Clone());
			if( wcslen(l_strNewName.c_str()) )
				l_pClone->SetName(l_strNewName.c_str());
		}
		return l_pClone;
	}

	cPuzzleImage::cPuzzleImage():cBaseImage(L"cPuzzleImage")
	{
		m_pImageShapePointVectorVector = new std::vector<std::vector<Vector3>>();
		m_pImageShapePointLODVector = new std::vector<int>();
		m_pImageIndexOfAnimation = nullptr;
		m_iNumImage = 0;
		m_pAllPuzzleData = nullptr;
		m_pfAllChildrenTriangleStripUV = nullptr;
		m_pfAllChildrenTwoTriangleUV = nullptr;
	}

	cPuzzleImage::cPuzzleImage(cPuzzleImage*e_pPuzzleImage):cBaseImage(e_pPuzzleImage)
	{
		m_pImageShapePointVectorVector = e_pPuzzleImage->m_pImageShapePointVectorVector;
		m_pImageShapePointLODVector = e_pPuzzleImage->m_pImageShapePointLODVector;
		m_pImageIndexOfAnimation = e_pPuzzleImage->m_pImageIndexOfAnimation;
		m_iNumImage = e_pPuzzleImage->GetNumImage();
		m_pAllPuzzleData = e_pPuzzleImage->m_pAllPuzzleData;
		m_pfAllChildrenTriangleStripUV = e_pPuzzleImage->m_pfAllChildrenTriangleStripUV;
		m_pfAllChildrenTwoTriangleUV = e_pPuzzleImage->m_pfAllChildrenTwoTriangleUV;
		this->CopyListPointer(e_pPuzzleImage);
	}

	cPuzzleImage::~cPuzzleImage()
	{
		cNamedTypedObjectVector<cPuzzleImageUnit>::Destroy();
		int	l_iRefCount = this->m_pTexture->GetReferenceCount();
		//left 1 to reference
		if( l_iRefCount == 1 )
		{
		    assert( this->m_bFromResource == false &&"both ot them shoudl not cloned object,or image parser delete order is wrong!?particle or new image parser?! " );
		    SAFE_DELETE(m_pImageIndexOfAnimation);
			SAFE_DELETE(m_pImageShapePointVectorVector);
			SAFE_DELETE(m_pImageShapePointLODVector);
		    SAFE_DELETE(m_pfAllChildrenTriangleStripUV);
		    SAFE_DELETE(m_pfAllChildrenTwoTriangleUV);
			SAFE_DELETE_ARRAY(m_pAllPuzzleData);
		}
	}

	bool	cPuzzleImage::MyParse(TiXmlElement*e_pRoot)
	{
		auto l_pRoot = e_pRoot;
		auto l_strImageName = e_pRoot->Attribute(L"ImageName");
		auto l_strCount = e_pRoot->Attribute(L"Count");
		if(l_strImageName)
		{
			auto l_strImageFullPathName = StringAddDirectory(UT::WcharToChar(l_strImageName));
			if (!g_bSkipImageLoad)
			{
				this->ParseTexture(l_strImageFullPathName.c_str(), g_bImageLoaderForFetchPixelData);
			}
			else
			{
			}
			SetName(UT::GetFileNameWithoutFullPath(this->m_strFileName).c_str());
		}
		if(l_strCount)
		{
			m_iNumImage = GetInt(l_strCount);
			m_pAllPuzzleData = new sPuzzleData[m_iNumImage];
		}
		int l_iPIUnitIndex = 0;
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pRoot)
			auto l_strName = e_pRoot->Value();
			COMPARE_NAME("AnimationData")
			{
				ProcessAnimationData(e_pRoot);
			}
			else
			COMPARE_NAME("PuzzleUnit")
			{
				ProcessPuzzleUnit(e_pRoot, l_iPIUnitIndex);
				++l_iPIUnitIndex;
			}
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pRoot)
		GenerateAllPuzzleImageUnit();
		ProcessDataCheck(l_pRoot);
		if (!g_bSupportNonPowerOfTwoTexture)
		{
			if (GetWidth() != power_of_two(GetWidth()) || GetHeight() != power_of_two(GetHeight()))
			{
				float	l_fWidthScale = GetUV()[2] / 1.f;
				float	l_fHeightScale = GetUV()[3] / 1.f;
				for (int i = 0; i < this->GetNumImage(); ++i)
				{
					float*	l_pfUV = m_pAllPuzzleData[i].fUV;
					l_pfUV[0] *= l_fWidthScale;
					l_pfUV[2] *= l_fWidthScale;
					l_pfUV[1] *= l_fHeightScale;
					l_pfUV[3] *= l_fHeightScale;
				}
			}
		}
		return true;
	}

    //<AnimationData Count="4">
    //    <AnimationDataUnit Name="Coinanime" Count="5" ImageList="GoldCoin1,GoldCoin2,GoldCoin3,GoldCoin4,GoldCoin5," TimeList="0.10000,0.10000,0.10000,0.10000,0.10000," />
    //    <AnimationDataUnit Name="Treasure_01" Count="1" ImageList="Diamond_01," TimeList="0.10000," />
    //    <AnimationDataUnit Name="Treasure_02" Count="1" ImageList="Diamond_02," TimeList="0.10000," />
    //    <AnimationDataUnit Name="Treasure_03" Count="1" ImageList="Diamond_03," TimeList="0.10000," />
    //</AnimationData>
	void cPuzzleImage::ProcessAnimationData(TiXmlElement* e_pElement)
	{
		SAFE_DELETE(m_pImageIndexOfAnimation);
		m_pImageIndexOfAnimation = new cNamedTypedObjectVector<cImageIndexOfAnimation>();
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_START(e_pElement)
			cImageIndexOfAnimation*l_pImageIndexOfAnimation = nullptr;
			int l_iCount = 0;
			PARSE_ELEMENT_START(e_pElement)
				COMPARE_NAME("Name")
				{
				    l_pImageIndexOfAnimation = new cImageIndexOfAnimation(true);
                    l_pImageIndexOfAnimation->SetName(l_strValue);
                    bool    l_b = m_pImageIndexOfAnimation->AddObject(l_pImageIndexOfAnimation);
                    assert(l_b&&"this animation data has been added!");
				}
				else
				COMPARE_NAME("Count")
				{
					l_iCount = VALUE_TO_INT;
				}
				else
				COMPARE_NAME("ImageList")
				{
				    wchar_t*l_strImageName = wcstok((wchar_t*)l_strValue,L",");
				    for( int i=0;i< l_iCount;++i )
				    {
				        l_pImageIndexOfAnimation->AddNameObject(l_strImageName,-1,0.1f);
				        l_strImageName = wcstok(0,L",");
				    }
				    assert(!l_strImageName);
				}
				else
				COMPARE_NAME("TimeList")
				{
				    wchar_t*l_strImageName = wcstok((wchar_t*)l_strValue,L",");
				    for( int i=0;i< l_iCount;++i )
				    {
						l_pImageIndexOfAnimation->m_ImageAnimationDataList[i].fTimeGap = VALUE_TO_FLOAT;
						l_strImageName = wcstok(0,L",");
				    }
				    assert(!l_strImageName);
				}
			PARSE_NAME_VALUE_END
		FOR_ALL_FIRST_CHILD_AND_ITS_CIBLING_END(e_pElement)
	}
	//<PuzzleUnit Name = "GoldCoin5" UV = "0.7901235,0,0.9845679,0.2386364" OffsetPos = "0,0" Size = "63,63" OriginalSize = "63,63" ShowPosInPI = "256,0" / >
	void cPuzzleImage::ProcessPuzzleUnit(TiXmlElement* e_pElement, int e_iIndex)
	{
		int l_iTriangulatorPointsLOD = 1;
		std::vector<Vector3> l_TriangulatorVector;
		sPuzzleData*l_pPuzzleData = &m_pAllPuzzleData[e_iIndex];
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("Name")
			{
				l_pPuzzleData->strFileName = l_strValue;
			}
			else
			COMPARE_NAME("UV")
			{
				FATMING_CORE::GetUV((wchar_t*)l_strValue, l_pPuzzleData->fUV);
			}
			else
			COMPARE_NAME("OffsetPos")
			{
				l_pPuzzleData->OffsetPos = GetPoint(l_strValue);
			}
			else
			COMPARE_NAME("Size")
			{
				l_pPuzzleData->Size = GetPoint(l_strValue);
			}
			else
			COMPARE_NAME("OriginalSize")
			{
				l_pPuzzleData->OriginalSize = GetPoint(l_strValue);
			}
			else
			COMPARE_NAME("ShowPosInPI")
			{
				l_pPuzzleData->ShowPosInPI = GetPoint(l_strValue);
				//float	l_fUV[4] = {l_sPuzzleData.ShowPosInPI.x/1024.f,
				//	l_sPuzzleData.ShowPosInPI.y/1024.f,
				//	(l_sPuzzleData.ShowPosInPI.x+l_sPuzzleData.Size.x)/1024.f,
				//	(l_sPuzzleData.ShowPosInPI.y+l_sPuzzleData.Size.y)/1024.f
				//};
				//int a=0;

			}
			else
			COMPARE_NAME("TriangulatorPoints")
			{
				l_TriangulatorVector = StringToVector3Vector(l_strValue);
			}
			else
			COMPARE_NAME("TriangulatorPointsLOD")
			{
				l_iTriangulatorPointsLOD = VALUE_TO_INT;
			}
		PARSE_NAME_VALUE_END
		if (m_pImageShapePointVectorVector)
		{
			m_pImageShapePointVectorVector->push_back(l_TriangulatorVector);
			m_pImageShapePointLODVector->push_back(l_iTriangulatorPointsLOD);
		}
	}
	//<PuzzleImage OriginalNameSort="background_0001,BG_01_Plant_00000,BG_01_Plant_00001,BG_01_Plant_00002,BG_01_Plant_00003,BG_01_Darker" ImageName="BG_01.png" Count="6" GeneratePuzzleimageUnit="0">
	void cPuzzleImage::ProcessDataCheck(TiXmlElement*e_pElement)
	{
		//ImageName = "BG_01.png" Count = "6" GeneratePuzzleimageUnit = "0"
		int l_iCount = 0;
		PARSE_ELEMENT_START(e_pElement)
			COMPARE_NAME("ImageName")
			{
			}
			else
			COMPARE_NAME("PI_tri")
			{
			}
			else
			COMPARE_NAME("Count")
			{
#ifdef DEBUG
				int l_iCount = this->Count();
				if(l_iCount != _wtoi(l_strValue))
					assert(0&&"cPuzzleImage::ProcessDataCheck() count is not correct");
				if (m_pImageShapePointVectorVector)
				{
					if (l_iCount != (int)m_pImageShapePointVectorVector->size() || m_pImageShapePointLODVector->size()!= m_pImageShapePointVectorVector->size())
					{
						assert(0 && "cPuzzleImage::ProcessDataCheck():m_pImageShapePointVectorVector count is not correct");
					}
				}
#endif
			}
			else
			COMPARE_NAME("ImageDistance")
			{
			//bool	l_bGeneratePuzzleimageUnit = atoi(l_str)?true:false;
			}
			else
			COMPARE_NAME("GeneratePuzzleimageUnit")
			{
				//bool	l_bGeneratePuzzleimageUnit = atoi(l_str)?true:false;
			}
			//only for editor most time we don't expect sort it back again.
			else
			COMPARE_NAME("OriginalNameSort")
			{//ther only should work at editor!.
				if(m_sbSortPIFileAsOriginal)
				{
					wchar_t*l_ImageName = wcstok((wchar_t*)l_strValue,L",");
					std::vector<cPuzzleImageUnit*>		l_PuzzleImageUnitVector;
					std::vector<std::vector<Vector3>>	l_TriangulatorPointsVectorVector;
					std::vector<int>					l_iTriangulatorPointsLODVector;
					while(l_ImageName)
					{
						int l_iCount = Count();
						for( int i=0;i< l_iCount;++i )
						{
							if(!wcscmp((*this)[i]->GetName(),l_ImageName))
							{
								l_PuzzleImageUnitVector.push_back((*this)[i]);
								if (m_pImageShapePointVectorVector)
								{
									l_TriangulatorPointsVectorVector.push_back((*m_pImageShapePointVectorVector)[i]);
									l_iTriangulatorPointsLODVector.push_back((*m_pImageShapePointLODVector)[i]);
									m_pImageShapePointVectorVector->erase(m_pImageShapePointVectorVector->begin() + i);
									m_pImageShapePointLODVector->erase(m_pImageShapePointLODVector->begin() + i);
								}
								this->RemoveObjectWithoutDelete(i);
								break;
							}
						}
						l_ImageName = wcstok(0,L",");
					}
					size_t l_uiSize = l_PuzzleImageUnitVector.size();
					for (size_t i = 0; i < l_uiSize; i++)
					{
						this->AddObjectNeglectExist(l_PuzzleImageUnitVector[i]);
						if (m_pImageShapePointVectorVector)
						{
							m_pImageShapePointVectorVector->push_back(l_TriangulatorPointsVectorVector[i]);
							m_pImageShapePointLODVector->push_back(l_iTriangulatorPointsLODVector[i]);
						}
					}
				}
			}
		PARSE_NAME_VALUE_END
	}

	void	cPuzzleImage::GenerateAllPuzzleImageUnit()
	{
		if( m_iNumImage == Count() )
			return;
		std::vector<cPuzzleImageUnit*>	AllPuzzleImageUnit;
		for( int i=0;i<this->m_iNumImage;++i )
		{
#ifndef RETAILER
#ifndef WASM//accuatlly in game it doesn't matter
			if (this->m_pTexture&&m_pTexture->IsSameName(m_pAllPuzzleData[i].strFileName.c_str()))
			{
				std::wstring l_strSame = m_pAllPuzzleData[i].strFileName.c_str();
				l_strSame += L"_Copy";
				UT::ErrorMsgByFormat(L"PIUnit and PI cannt be same name(%ls),now change name to %ls _Copy", m_pAllPuzzleData[i].strFileName.c_str(), m_pAllPuzzleData[i].strFileName.c_str());
				m_pAllPuzzleData[i].strFileName = l_strSame;
			}
#endif
#endif
			cPuzzleImageUnit*l_pUnit = new cPuzzleImageUnit(&m_pAllPuzzleData[i],this);
			l_pUnit->SetName(m_pAllPuzzleData[i].strFileName);
			this->AddObjectNeglectExist(l_pUnit);
			//bool	l_b = this->AddObject(l_p);
			//if( !l_b )
			//{
			//	UT::ErrorMsgByFormat(L"PIUnit %s exists!", m_pAllPuzzleData[i].strFileName.c_str());
			//	SAFE_DELETE(l_p);
			//}
			//else
			AllPuzzleImageUnit.push_back(l_pUnit);
		}
		cPuzzleImageUnit*l_pPrior = 0;
		for( int i=0;i<this->m_iNumImage-1;++i )
		{
			AllPuzzleImageUnit[i]->SetPrior(l_pPrior);
			AllPuzzleImageUnit[i]->SetNext(AllPuzzleImageUnit[i+1]);
			l_pPrior = AllPuzzleImageUnit[i];
		}
		AllPuzzleImageUnit[m_iNumImage-1]->SetPrior(l_pPrior);
		AllPuzzleImageUnit[m_iNumImage-1]->SetNext(0);
	}


	void	cPuzzleImage::Render(int e_iIndex)
	{
		if( !m_bVisible )
			return;
		(*this)[e_iIndex]->Render();
	}

	float*   cPuzzleImage::GetAllChildrenTriangleStripUV()
	{
	    if(!m_pfAllChildrenTriangleStripUV)
	    {
            assert(m_iNumImage);
            m_pfAllChildrenTriangleStripUV = new float[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT*m_iNumImage];
			for (int i = 0; i < this->m_iNumImage; ++i)
			{
				memcpy(&m_pfAllChildrenTriangleStripUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT*i],
					UVToTriangleStrip(this->GetObject(i)->GetUV()), sizeof(float)*TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT);
			}
        }
        return m_pfAllChildrenTriangleStripUV;
	}
	
	float*   cPuzzleImage::GetAllChildrenTwoTriangleUV()
	{
	    if(!m_pfAllChildrenTwoTriangleUV)
	    {
            assert(m_iNumImage);
            m_pfAllChildrenTwoTriangleUV = new float[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT*m_iNumImage];
            for(int i=0;i<this->m_iNumImage;++i)
                memcpy(&m_pfAllChildrenTwoTriangleUV[TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT*i],
                UVToTwoTriangle(this->GetObject(i)->GetUV()),sizeof(float)*TWO_TRIANGLE_VERTICES_UV_TO_QUAD_UV_COUNT);	
        }
        return m_pfAllChildrenTwoTriangleUV;
	}

	cNumeralImage*	cPuzzleImage::GetNumeralImageByName(const wchar_t*e_str0ImaneName,const wchar_t*e_str9ImaneName)
	{
		cPuzzleImageUnit*l_pPuzzleImageUnit0 = GetObject(e_str0ImaneName);
		cPuzzleImageUnit*l_pPuzzleImageUnit9 = GetObject(e_str9ImaneName);
		if( l_pPuzzleImageUnit0&&l_pPuzzleImageUnit9 )
		{
			cNumeralImage*l_pNumeralImage = new cNumeralImage(l_pPuzzleImageUnit0,l_pPuzzleImageUnit9);
			return l_pNumeralImage;
		}
		return 0;	
	}

	std::vector<std::vector<Vector3>>* cPuzzleImage::GetImageShapePointVectorVector()
	{
		return m_pImageShapePointVectorVector;
	}

	std::vector<Vector3>* cPuzzleImage::GetImageShapePointVector(int e_iIndex)
	{
		if (m_pImageShapePointVectorVector && m_pImageShapePointVectorVector->size() > e_iIndex)
		{
			auto l_pObject = &(*m_pImageShapePointVectorVector)[e_iIndex];
			if (l_pObject->size() > 0)
				return l_pObject;
		}
		return nullptr;
	}

	int	cPuzzleImage::GetImageShapePointLOD(int e_iIndex)
	{
		return (*m_pImageShapePointLODVector)[e_iIndex];
	}

	cNumeralImage*	cPuzzleImage::GetNumeralImageByName(const wchar_t*e_strNumerImageName)
	{
		cPuzzleImageUnit*l_pPuzzleImageUnit = GetObject(e_strNumerImageName);
		if( l_pPuzzleImageUnit )
		{
			cNumeralImage*l_pNumeralImage = new cNumeralImage(l_pPuzzleImageUnit,l_pPuzzleImageUnit->GetUV());
			return l_pNumeralImage;
		}
		return 0;
	}
	
	std::string       cPuzzleImage::GetFileName(const wchar_t*e_strObjectName)
	{
		char	l_str[TEMP_SIZE];
		sprintf(l_str,"%s.pi",UT::WcharToChar(e_strObjectName).c_str());
		std::string	l_strtemp = l_str;
	    return l_strtemp;
	}

	std::string       cPuzzleImage::GetFileName(const char*e_strObjectName)
	{
		char	l_str[TEMP_SIZE];
	    sprintf(l_str,"%s.pi",e_strObjectName);
		std::string	l_strtemp = l_str;
	    return l_strtemp;
	}
	////<PuzzleImage PI=""/>
	//cPuzzleImage*	cPuzzleImage::GetMe(TiXmlElement*e_pElement)
	//{
	//	e_pElement->Attribute();
	//	cGameApp::GetPuzzleImageByFileName();
	//}
}