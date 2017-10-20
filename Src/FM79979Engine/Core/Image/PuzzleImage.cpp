#include "../stdafx.h"
#include "SimplePrimitive.h"
#include "BaseImage.h"
#include "PuzzleImage.h"
#include "NumeralImage.h"
#include "../GameplayUT/GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cPuzzleImage);
	TYPDE_DEFINE_MARCO(cPuzzleImageUnit);

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
	
	void    cImageIndexOfAnimation::GenerateImageIndexByPI(cPuzzleImage*e_pPI,std::vector<std::wstring>   *e_pNameList)
	{
	    size_t  l_iSize = e_pNameList->size();
	    m_ImageAnimationDataList.resize(l_iSize);
	    for( size_t i=0;i<l_iSize;++i )
	    {
			m_ImageAnimationDataList[i].iIndex = e_pPI->GetObjectIndexByName((*e_pNameList)[i].c_str());
	    }
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
		m_pNext = 0;
		m_pPrior = 0;
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
		m_pPuzzleData = e_pPuzzleImageUnit->m_pPuzzleData;
		m_pNext = e_pPuzzleImageUnit->GetNext();
		m_pPrior = e_pPuzzleImageUnit->GetPrior();
	}
	cPuzzleImageUnit::cPuzzleImageUnit(cBaseImage*e_pBaseImage):cBaseImage(e_pBaseImage)
	{
		m_pPuzzleData = 0;
		m_pNext = 0;
		m_pPrior = 0;
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
					cGameApp::OutputDebugInfoString(l_strErrorMsg);
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

	cPuzzleImage::cPuzzleImage(cPuzzleImage*e_pPuzzleImage):cBaseImage(e_pPuzzleImage)
	{
		m_pImageIndexOfAnimation = e_pPuzzleImage->m_pImageIndexOfAnimation;
		m_iNumImage = e_pPuzzleImage->GetNumImage();
		m_ppPuzzleData = e_pPuzzleImage->GetPuzzleData();
		m_pfAllChildrenTriangleStripUV = e_pPuzzleImage->m_pfAllChildrenTriangleStripUV;
		m_pfAllChildrenTwoTriangleUV = e_pPuzzleImage->m_pfAllChildrenTwoTriangleUV;
		this->CopyListPointer(e_pPuzzleImage);
	}

	cPuzzleImage::cPuzzleImage(char*e_strName,std::vector<sPuzzleData> *e_pPuzzleDataList,bool e_bGenerateAllUnit,bool e_bFetchPixels)
	:cBaseImage(e_strName,e_bFetchPixels)
	{
		m_pImageIndexOfAnimation = 0;
		m_pfAllChildrenTriangleStripUV = 0;
		m_pfAllChildrenTwoTriangleUV = 0;
		if( e_pPuzzleDataList )
		{
			m_ppPuzzleData = new sPuzzleData*[e_pPuzzleDataList->size()];
			m_iNumImage = (int)e_pPuzzleDataList->size();
			for( int i=0;i<m_iNumImage;++i )
			{
				sPuzzleData l_DestsPuzzleData = (*e_pPuzzleDataList)[i];
				m_ppPuzzleData[i] = new sPuzzleData(l_DestsPuzzleData.strFileName,l_DestsPuzzleData.fUV,l_DestsPuzzleData.OffsetPos,l_DestsPuzzleData.Size,l_DestsPuzzleData.OriginalSize,l_DestsPuzzleData.ShowPosInPI);
				if( e_bGenerateAllUnit )
				{
					cPuzzleImageUnit*l_p = new cPuzzleImageUnit(m_ppPuzzleData[i],this);
					l_p->SetName(m_ppPuzzleData[i]->strFileName);
					this->AddObject(l_p);
				}
			}
		}
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
		    SAFE_DELETE(m_pfAllChildrenTriangleStripUV);
		    SAFE_DELETE(m_pfAllChildrenTwoTriangleUV);
			for( int i=0;i<this->m_iNumImage;++i )
				SAFE_DELETE(m_ppPuzzleData[i]);
			SAFE_DELETE_ARRAY(m_ppPuzzleData);
		}
		//DELETE_VECTOR(m_PuzzleImageChildrenList,cPuzzleImage*);
	}

	void	cPuzzleImage::GenerateAllPuzzleImageUnit()
	{
		if( m_iNumImage == Count() )
			return;
		std::vector<cPuzzleImageUnit*>	AllPuzzleImageUnit;
		for( int i=0;i<this->m_iNumImage;++i )
		{
			cPuzzleImageUnit*l_p = new cPuzzleImageUnit(m_ppPuzzleData[i],this);
			l_p->SetName(m_ppPuzzleData[i]->strFileName);
			bool	l_b = this->AddObject(l_p);
			if( !l_b )
			{
				SAFE_DELETE(l_p);
			}
			else
				AllPuzzleImageUnit.push_back(l_p);
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
            for(int i=0;i<this->m_iNumImage;++i)
                memcpy(&m_pfAllChildrenTriangleStripUV[TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT*i],
                UVToTriangleStrip(this->GetObject(i)->GetUV()),sizeof(float)*TWO_TRIANGLE_STRIP_UV_TO_QUAD_UV_COUNT);	
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