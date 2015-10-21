#include "../stdafx.h"
#include "../Utility.h"
#include "NumeralImage.h"
#include "ImageParser.h"
#include "../XML/AtgXmlWriter.h"
#include "PuzzleImage.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cImageParser);
	const char*				cImageParser::ExtensionNameID( ".pi" );
    bool					g_bImageLoaderForFetchPixelData	= false;
	//sometimes we only need to PI data but no pixels data
	bool    g_bSkipImageLoad = false;
	cImageParser::cImageParser(bool e_bSortPIFileAsOriginal):ISAXCallback(true)
	{
		m_bSortPIFileAsOriginal = e_bSortPIFileAsOriginal;
		m_eImageType = eIT_MAX;
		m_pCurrentImageIndexOfAnimationList = 0;
		m_bFileExist = false;
	}
	//===============
	//
	//===============
	cImageParser::~cImageParser()
	{

	}

	void	cImageParser::RemoveResourceObject(NamedTypedObject*e_pObject)
	{
		if(e_pObject->Type() == cTexture::TypeID)
		{
			cTexture*l_pSimpleGLTexture = dynamic_cast<cTexture*>(e_pObject);
			l_pSimpleGLTexture->Release(this);
		}
	}
	
    void	cImageParser::InternalParse()
    {
        m_pCurrentImageIndexOfAnimationList = 0;
		m_bFileExist = false;
        //g_bImageLoaderForFetchPixelData = false;
    }

	cUIImage*	cImageParser::GetUIImage(int e_iIndex)
	{
		return dynamic_cast<cUIImage*>(GetObject(e_iIndex));
	}

	cUIImage*	cImageParser::GetUIImage(const wchar_t*e_strName)
	{
		return dynamic_cast<cUIImage*>(GetObject(e_strName));
	}

	cBaseImage*	cImageParser::GetBaseImage(const wchar_t*e_strName)
	{
		return dynamic_cast<cBaseImage*>(GetObject(e_strName));
	}

	cPuzzleImage*	cImageParser::GetPuzzleImageByFileName(const wchar_t*e_strName)
	{
		std::string	l_strFileName = UT::WcharToChar(e_strName);
		std::wstring l_strPIName = UT::GetFileNameWithoutFullPath(e_strName);
		return GetPuzzleImage(l_strFileName.c_str(),l_strPIName.c_str());
	}

	cPuzzleImage*	cImageParser::GetPuzzleImage(const wchar_t*e_strName)
	{
	    return dynamic_cast<cPuzzleImage*>(GetObject(e_strName));
	}
	
	cPuzzleImage*	cImageParser::GetPuzzleImage(int e_iIndex)
	{
	    return dynamic_cast<cPuzzleImage*>(GetObject(e_iIndex));
	}

	cPuzzleImage*	cImageParser::GetPuzzleImage(const char*e_strFileName,const wchar_t*e_strPIName)
	{
		cPuzzleImage*l_pPI = GetPuzzleImage(e_strPIName);
		if( !l_pPI )
		{
			this->m_strErrorMsg.clear();
			int	l_iCount = this->Count();
			bool	l_b = this->Parse(e_strFileName);
			if( !l_b )
			{
				UT::ErrorMsg("parse file eailed",e_strFileName);
				return 0;
			}
			if( l_iCount +1 != this->Count() )
			{
				UT::ErrorMsg("parse file eailed",e_strFileName);
				return 0;
			}
			return this->GetPuzzleImage(this->Count()-1);
		}
		return l_pPI;
	}

	void	cImageParser::HandleElementData(TiXmlElement *e_pTiXmlElement)
	{
		const wchar_t*	l_strName = e_pTiXmlElement->Value();
		if( !wcslen(l_strName) )
			return;
		COMPARE_NAME("Common")
			m_eImageType = eIT_COMMON;
		else
		COMPARE_NAME("UI")
			m_eImageType = eIT_UI;
		else
		COMPARE_NAME("Numeral")
			m_eImageType = eIT_NUMERAL;
		else
		COMPARE_NAME("PuzzleImage")
		{
			m_eImageType = eIT_PUZZLE_IMAGE;
			const wchar_t*l_strImageName = this->m_pCurrentTiXmlElement->Attribute(L"ImageName");
			if( l_strImageName )
			{
				if(this->GetObject(UT::GetFileNameWithoutFullPath(l_strImageName)))
				{
					m_bFileExist = true;
				}
			}
		}
		DistributeElementToLoaders();
	}
	//===============
	//
	//===============
	void	cImageParser::DistributeElementToLoaders()
	{
		if( m_bFileExist )
			return;
		switch(m_eImageType)
		{
			case eIT_COMMON:
				ProcessCommonTexture();
				break;
			case eIT_UI:
				ProcessUITexture();
				break;
			case eIT_NUMERAL:
				ProcessNumeralTexture();
				break;
			case eIT_PUZZLE_IMAGE:
				ProcessPuzzleImageTexture();
				break;
		}
	}
	//<Common Name="BackGround/BKImage.png" UV="0,0,1,1" />
	void	cImageParser::ProcessCommonTexture()
	{
		UT::ErrorMsg("no more support","error");
		//if(!this->m_pCurrentTiXmlElement->m_bDone)
		//{
		//	bool	l_bSkipUV = false;
		//	cBaseImage*l_p = 0;
		//	PARSE_CURRENT_ELEMENT_START
		//		COMPARE_NAME("Name")
		//		{
		//			char*	l_strFileName = StringAddDirectory(UT::WcharToChar(l_strValue));
		//			l_p = new cBaseImage(l_strFileName,g_bImageLoaderForFetchPixelData);
		//			bool	l_bSame = this->AddObject(l_p);
		//			if( !l_bSame )
		//			{
		//				this->m_strErrorMsg += UT::CharToWchar(l_strFileName);
		//				this->m_strErrorMsg += L"loading same image error occur\n";
		//			}
		//			//none power of two texture,we have already adjust UV while loading so do not adjust it again,
		//			//or we have make this image as a power of 2 image,and we need to assign the UV data not automaticly,
		//			//because generate power of 2 image while loading will waste time,or it just good because it's just 
		//			//a power of 2 image so we do not to care it ,too.
		//			if( l_p->GetUV()[2] == 1&& l_p->GetUV()[3] == 1 )
		//				l_bSkipUV = true;
		//		}
		//		else
		//		COMPARE_NAME("UV")
		//		{
		//			if( l_bSkipUV )
		//			{
		//				l_pAttribute = l_pAttribute->Next();
		//				continue;
		//			}
		//			if( l_p )
		//			{
		//				assert(*l_p->GetHeight()==UT::power_of_two(*l_p->GetHeight()));
		//				float	l_fUV[4];
		//				GetUV(l_strValue,l_fUV);
		//				l_p->SetUV(l_fUV);
		//				l_p->SetImageSize((int)(l_p->GetWidth()*(l_fUV[2]-l_fUV[0])),(int)(l_p->GetHeight()*(l_fUV[3]-l_fUV[0])));
		//			}
		//		}
		//	PARSE_NAME_VALUE_END
		//}
	}
	//<UI Name="388.png" Position="0,0" UV="0,0,1,1" />
	void	cImageParser::ProcessUITexture()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			POINT	l_Point;
			cUIImage*	l_pUIImage = 0;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{

					l_pUIImage = new cUIImage(
						(char*)StringAddDirectory(
						UT::WcharToChar(l_pAttribute->Value())
						).c_str()
						);
				}
				else
				COMPARE_NAME("Position")
				{
					l_Point = GetPoint(l_strValue);
				}
				else
				COMPARE_NAME("UV")
				{
					float	l_fUV[4];
					GetUV(l_strValue,l_fUV);
					if( l_pUIImage )
						l_pUIImage->SetUV(l_fUV);
				}
			PARSE_NAME_VALUE_END
			assert(l_pUIImage&&"failed to load image");
			l_pUIImage->SetPos(l_Point);
			//assert(l_Point.x&&"Position x is nagtive");
			//assert(l_Point.y&&"Position y is nagtive");
			bool	l_b = this->AddObject(l_pUIImage);
			if( !l_b )
			{
				this->m_strErrorMsg += l_pUIImage->GetName();
				this->m_strErrorMsg += L"already Existed\n";
			}
		}
	}
	//	<Numeral Name="NumeralImage/CardNI.png" />
	void	cImageParser::ProcessNumeralTexture()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					const char*l_strFileName = this->StringAddDirectory(UT::WcharToChar(l_strValue)).c_str();
					//l_strValue = StringAddDirectory(l_pAttribute->Value());
					//l_Value = StringAddRootDirectory(l_Value);
					cNumeralImage*l_p = new cNumeralImage((char*)l_strFileName);
					bool	l_bSame = this->AddObject(l_p);
					if( !l_bSame )
					{
						this->m_strErrorMsg += l_strValue;
						this->m_strErrorMsg += L"loading Numeral texture failed\n";
					}
				}
			PARSE_NAME_VALUE_END
		}
	}

	//<PuzzleImage Name="79979" Count="6" >
	//    <PuzzleUnit Name="ST2StonePart" UV="0,0,0.32875,0.1275," OffsetPos="9,9," />
	//    <PuzzleUnit Name="ST3StonePart" UV="0.33,0,0.81875,0.11625," OffsetPos="4,4," />
	//    <PuzzleUnit Name="ST4StonePart" UV="0,0.12875,0.3575,0.2525," OffsetPos="11,11," />
	//    <PuzzleUnit Name="ST5StonePart" UV="0.35875,0.12875,0.885,0.25375," OffsetPos="52,52," />
	//    <PuzzleUnit Name="ST6StonePart" UV="0,0.255,0.33375,0.39125," OffsetPos="30,30," />
	//    <PuzzleUnit Name="ST7StonePart" UV="0.335,0.255,0.665,0.38375," OffsetPos="5,5," />
	//</PuzzleImage>
	void	cImageParser::ProcessPuzzleImageTexture()
	{
		static	std::vector<sPuzzleData>	*l_pPuzzleDataList = 0;
		COMPARE_ELEMENT_VALUE("PuzzleImage")
		{
			if( this->m_pCurrentTiXmlElement->m_bDone )
			{
				cPuzzleImage*l_pPuzzleImage = 0;
				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("ImageName")
					{
						if( !g_bSkipImageLoad )
							l_pPuzzleImage = new cPuzzleImage((char*)StringAddDirectory(UT::WcharToChar(l_strValue)).c_str(),l_pPuzzleDataList,false,g_bImageLoaderForFetchPixelData);
						else
						{
							l_pPuzzleImage = new cPuzzleImage(0,l_pPuzzleDataList,false,g_bImageLoaderForFetchPixelData);
						}
						l_pPuzzleImage->SetName(UT::GetFileNameWithoutFullPath(this->m_strFileName).c_str());
						if(!g_bSupportNonPowerOfTwoTexture)
						{
							float	l_fWidthScale = l_pPuzzleImage->GetUV()[2]/1.f;
							float	l_fHeightScale = l_pPuzzleImage->GetUV()[3]/1.f;
							for( int i=0;i<l_pPuzzleImage->GetNumImage();++i )
							{
								float*	l_pfUV = l_pPuzzleImage->GetPuzzleData()[i]->fUV;
								l_pfUV[0]*=l_fWidthScale;
								l_pfUV[2]*=l_fWidthScale;
								l_pfUV[1]*=l_fHeightScale;
								l_pfUV[3]*=l_fHeightScale;
							}
						}

						l_pPuzzleImage->GenerateAllPuzzleImageUnit();
						bool	l_b = AddObject(l_pPuzzleImage);
						if( m_pCurrentImageIndexOfAnimationList )
						{
						    l_pPuzzleImage->SetImageIndexOfAnimationList(m_pCurrentImageIndexOfAnimationList);
						    for( int i=0;i<m_pCurrentImageIndexOfAnimationList->Count();++i )
						    {
						        (*m_pCurrentImageIndexOfAnimationList)[i]->GenerateImageIndexByPI(l_pPuzzleImage,(*m_pCurrentImageIndexOfAnimationList)[i]->m_pNameList);
#ifndef DEBUG
                                SAFE_DELETE((*m_pCurrentImageIndexOfAnimationList)[i]->m_pNameList);
#endif
						    }
						    m_pCurrentImageIndexOfAnimationList = 0;
						}
						if( !l_b )
						{
							this->m_strErrorMsg += l_pPuzzleImage->GetName();
							this->m_strErrorMsg += L"such object already existed\n";
						}
					}
					else
					COMPARE_NAME("Count")
					{
#ifdef DEBUG
						if(l_pPuzzleDataList->size() != _wtoi(l_strValue))						
							assert(0&&"ProcessPuzzleImageTexture() count is not correct");
#endif
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
						if( m_bSortPIFileAsOriginal )
						{
						    size_t l_iOriginalSize = l_pPuzzleDataList->size();
							wchar_t*l_ImageName = wcstok((wchar_t*)l_strValue,L",");
							std::vector<sPuzzleData>	l_PuzzleDataList;
							while(l_ImageName)
							{
								for( size_t i=0;i<l_pPuzzleDataList->size();++i )
								{
									if(!wcscmp((*l_pPuzzleDataList)[i].strFileName,l_ImageName))
									{
										l_PuzzleDataList.push_back((*l_pPuzzleDataList)[i]);
										(*l_pPuzzleDataList).erase(l_pPuzzleDataList->begin()+i);
										break;
									}
								}
								l_ImageName = wcstok(0,L",");
							}
							if( l_iOriginalSize != l_PuzzleDataList.size() )
							    UT::ErrorMsg( L"the file OriginalNameSort is not match pi's image list data",L"WARNINGMSG");
							*l_pPuzzleDataList = l_PuzzleDataList;
						}
					}
				PARSE_NAME_VALUE_END
				SAFE_DELETE(l_pPuzzleDataList);
			}
			else
			{
				assert(!l_pPuzzleDataList);
				std::vector<sPuzzleData>*l_pNewData = new std::vector<sPuzzleData>;
				l_pPuzzleDataList = l_pNewData;
			}		
		}
		else
		COMPARE_ELEMENT_VALUE("PuzzleUnit")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone )
			{
				//float	fUV[2];			//texture uv.
				//POINT	OffsetPos;		//offset position,via this we coul add alpha part to the detination texture while create texture.
				//POINT	Size;			//width height.
				//wchar_t	strFileName[TEMP_SIZE];

				sPuzzleData	l_sPuzzleData;
				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("Name")
					{
						WCHAR_TO_WCHAR(l_strValue,l_sPuzzleData.strFileName);
					}
					else
					COMPARE_NAME("UV")
					{
						GetUV((wchar_t*)l_strValue,l_sPuzzleData.fUV);
					}
					else
					COMPARE_NAME("OffsetPos")
					{
						l_sPuzzleData.OffsetPos = GetPoint(l_strValue);
					}
					else
					COMPARE_NAME("Size")
					{
						l_sPuzzleData.Size = GetPoint(l_strValue);
					}
					else
					COMPARE_NAME("OriginalSize")
					{
						l_sPuzzleData.OriginalSize = GetPoint(l_strValue);
					}
					else
					COMPARE_NAME("ShowPosInPI")
					{
						l_sPuzzleData.ShowPosInPI = GetPoint(l_strValue);
						//float	l_fUV[4] = {l_sPuzzleData.ShowPosInPI.x/1024.f,
						//	l_sPuzzleData.ShowPosInPI.y/1024.f,
						//	(l_sPuzzleData.ShowPosInPI.x+l_sPuzzleData.Size.x)/1024.f,
						//	(l_sPuzzleData.ShowPosInPI.y+l_sPuzzleData.Size.y)/1024.f
						//};
						//int a=0;

					}
				PARSE_NAME_VALUE_END
				l_pPuzzleDataList->push_back(l_sPuzzleData);
			}
		}
		else
		COMPARE_ELEMENT_VALUE("AnimationData")
		{
		    if(!this->m_pCurrentTiXmlElement->m_bDone )
		    {
		        assert( m_pCurrentImageIndexOfAnimationList == 0 );
		        cNamedTypedObjectVector<cImageIndexOfAnimation>*l_pImageIndexOfAnimationList = new cNamedTypedObjectVector<cImageIndexOfAnimation>;
		        m_pCurrentImageIndexOfAnimationList = l_pImageIndexOfAnimationList;
			}
			else
			{
#ifdef DEBUG
                PARSE_CURRENT_ELEMENT_START
				    COMPARE_NAME("Count")
				    {
                        assert(m_pCurrentImageIndexOfAnimationList->Count() == VALUE_TO_INT );
				    }
			    PARSE_NAME_VALUE_END
#endif
			}
		}
		else
		COMPARE_ELEMENT_VALUE("AnimationDataUnit")
		{
		    if(!this->m_pCurrentTiXmlElement->m_bDone )
		    {
		        cImageIndexOfAnimation*l_pImageIndexOfAnimation = 0;
		        int l_iCount = 0;
                PARSE_CURRENT_ELEMENT_START
				    COMPARE_NAME("Name")
				    {
				        l_pImageIndexOfAnimation = new cImageIndexOfAnimation(true);
                        l_pImageIndexOfAnimation->SetName(l_strValue);
                        bool    l_b = m_pCurrentImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
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
				        for( int i=0;i<l_iCount;++i )
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
				        for( int i=0;i<l_iCount;++i )
				        {
							l_pImageIndexOfAnimation->m_ImageAnimationDataList[i].fTimeGap = VALUE_TO_FLOAT;
							l_strImageName = wcstok(0,L",");
				        }
				        assert(!l_strImageName);
					}
			    PARSE_NAME_VALUE_END
			}
		}
	}

	void	cImageParser::Export(char*e_strFileName)
	{
	#ifdef WIN32
		ATG::XMLWriter*     l_pXMLWriter = new ATG::XMLWriter(e_strFileName);
		l_pXMLWriter->StartElement("Image");
		for( int i=0;i<Count();++i )
		{
			NamedTypedObject* l_pNamedTypedObject = (*this)[i];
			const	wchar_t*l_strType = l_pNamedTypedObject->Type();
			const wchar_t*	l_strName = l_pNamedTypedObject->GetName();
			wchar_t	l_strWithExtendName[TEMP_SIZE];
			swprintf(l_strWithExtendName,TEMP_SIZE,L"%s.png",l_strName);

		//<Numeral Name="NumeralImage/CardNI.png" MaxU="1" />
		//<UI Name="388.png" Position="0,0" UV="0,0,1,1" />
		//<Common Name="BackGround/BKImage.png" UV="0,0,1,1" />
		//<Sprite Count="4" Row="2" TexU="0.34375" TexV="0.34375" Name="ReversePoker.png" />
	//<PuzzleImage Name="79979" Count="6" GeneratePuzzleimageUnit="0">
	//    <PuzzleUnit Name="ST2StonePart" UV="0,0,0.32875,0.1275," OffsetPos="9,9," Size="263,102" />
	//    <PuzzleUnit Name="ST3StonePart" UV="0.33,0,0.81875,0.11625," OffsetPos="4,4," Size="391,93" />
	//    <PuzzleUnit Name="ST4StonePart" UV="0,0.12875,0.3575,0.2525," OffsetPos="11,11," Size="286,99" />
	//    <PuzzleUnit Name="ST5StonePart" UV="0.35875,0.12875,0.885,0.25375," OffsetPos="52,52," Size="421,100" />
	//    <PuzzleUnit Name="ST6StonePart" UV="0,0.255,0.33375,0.39125," OffsetPos="30,30," Size="267,109" />
	//    <PuzzleUnit Name="ST7StonePart" UV="0.335,0.255,0.665,0.38375," OffsetPos="5,5," Size="264,103" />
	//</PuzzleImage>
			if( !wcscmp(l_strType,cBaseImage::TypeID) )
			{
				cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(l_pNamedTypedObject);
				l_pXMLWriter->StartElement("Common");
				l_pXMLWriter->AddAttribute("Name",UT::WcharToChar(l_strWithExtendName));
				l_pXMLWriter->AddAttribute("UV",UVToString(l_pBaseImage->GetUV()));
				l_pXMLWriter->EndElement();
			}
			else
			if( !wcscmp(l_strType,cUIImage::TypeID) )
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(l_pNamedTypedObject);
				//UI Image="xxx.png" Position="x,y"
				l_pXMLWriter->StartElement("UI");
				l_pXMLWriter->AddAttribute("Name",UT::WcharToChar(l_strWithExtendName));
				char	l_strPos[TEMP_SIZE];
				sprintf(l_strPos,"%.2f,%.2f\0",l_pUIImage->GetPos().x,l_pUIImage->GetPos().y);
				l_pXMLWriter->AddAttribute("Position",l_strPos);
				l_pXMLWriter->AddAttribute("UV",UVToString(l_pUIImage->GetUV()));
				l_pXMLWriter->EndElement();
			}
			else
			if( !wcscmp(l_strType,cNumeralImage::TypeID) )
			{
				cNumeralImage*l_pNumeralImage = dynamic_cast<cNumeralImage*>(l_pNamedTypedObject);
				l_pXMLWriter->StartElement("Numeral");
				l_pXMLWriter->AddAttribute("MaxU",l_pNumeralImage->GetMaxU());
				l_pXMLWriter->AddAttribute("Name",UT::WcharToChar(l_strWithExtendName));
				l_pXMLWriter->EndElement();
			}
			else
			if( !wcscmp(l_strType,cPuzzleImage::TypeID) )
			{
				cPuzzleImage*l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(l_pNamedTypedObject);
				int	l_iPIWidth = l_pPuzzleImage->GetWidth();
				int	l_iPIHeight = l_pPuzzleImage->GetHeight();
				l_pXMLWriter->StartElement("PuzzleImage");
				l_pXMLWriter->AddAttribute("ImageName",l_strWithExtendName);
				l_pXMLWriter->AddAttribute("Count",(int)l_pPuzzleImage->GetList()->size());
				l_pXMLWriter->AddAttribute("GeneratePuzzleimageUnit","0");
				float	l_fUV[4];
				for( UINT i=0;i<l_pPuzzleImage->GetList()->size();++i )
				{
					sPuzzleData*l_pPuzzleData =	l_pPuzzleImage->GetPuzzleData(i);
					l_fUV[0] = (float)l_pPuzzleData->ShowPosInPI.x/l_iPIWidth;
					l_fUV[1] = (float)l_pPuzzleData->ShowPosInPI.y/l_iPIHeight;
					l_fUV[2] = (float)(l_pPuzzleData->ShowPosInPI.x+l_pPuzzleData->OriginalSize.x)/l_iPIWidth;
					l_fUV[3] = (float)(l_pPuzzleData->ShowPosInPI.y+l_pPuzzleData->OriginalSize.y)/l_iPIHeight;
					l_pXMLWriter->StartElement("PuzzleUnit");
					    l_pXMLWriter->AddAttribute("Name",l_pPuzzleData->strFileName);
					    l_pXMLWriter->AddAttribute("UV",UVToString(l_fUV));
					    l_pXMLWriter->AddAttribute("OffsetPos",ValueToString(l_pPuzzleData->OffsetPos));
					    l_pXMLWriter->AddAttribute("Size",ValueToString(l_pPuzzleData->Size));
					    l_pXMLWriter->AddAttribute("OriginalSize",ValueToString(l_pPuzzleData->OriginalSize));
					    l_pXMLWriter->AddAttribute("ShowPosInPI",ValueToString(l_pPuzzleData->ShowPosInPI));
					l_pXMLWriter->EndElement();//end PuzzleUnit
				}
					//    <PuzzleUnit Name="ST7StonePart" UV="0.335,0.255,0.665,0.38375," OffsetPos="5,5," Size="264,103" /
                if( l_pPuzzleImage->GetImageIndexOfAnimationList() && l_pPuzzleImage->GetImageIndexOfAnimationList()->Count() )
                {
			        l_pXMLWriter->StartElement("AnimationData");
						int	l_iAnimationDataCount = l_pPuzzleImage->GetImageIndexOfAnimationList()->Count();
			            l_pXMLWriter->AddAttribute("Count",l_iAnimationDataCount);
			            cImageIndexOfAnimation*l_pImageIndexOfAnimation = 0;
			            for(int j=0;j<l_iAnimationDataCount;++j)
			            {
				            l_pImageIndexOfAnimation = l_pPuzzleImage->GetImageIndexOfAnimationList()->GetObject(j);
				            l_pXMLWriter->StartElement("AnimationDataUnit");
								l_pXMLWriter->AddAttribute("Name",l_pImageIndexOfAnimation->GetName());
								l_pXMLWriter->AddAttribute("Count",(int)l_pImageIndexOfAnimation->m_ImageAnimationDataList.size());
								wstring	l_strImageNameList;
								string	l_strImageTimeGapList;
								if( l_pImageIndexOfAnimation->m_pNameList )
								{
									for(int k=0;k<(int)l_pImageIndexOfAnimation->m_pNameList->size();++k)
									{
										l_strImageNameList += (*l_pImageIndexOfAnimation->m_pNameList)[k].c_str();
										l_strImageNameList += L",";
										if( (int)l_pImageIndexOfAnimation->m_ImageAnimationDataList.size()>k )
										{
											l_strImageTimeGapList += ValueToString(l_pImageIndexOfAnimation->m_ImageAnimationDataList[k].fTimeGap);
											l_strImageTimeGapList += ",";
										}
									}
								}
								else
								{
									for(int k=0;k<(int)l_pImageIndexOfAnimation->m_ImageAnimationDataList.size();++k)
									{
										l_strImageNameList += l_pPuzzleImage->GetPuzzleData()[l_pImageIndexOfAnimation->m_ImageAnimationDataList[k].iIndex]->strFileName;
										l_strImageNameList += L",";
										l_strImageTimeGapList += ValueToString(l_pImageIndexOfAnimation->m_ImageAnimationDataList[k].fTimeGap);
										l_strImageTimeGapList += ",";
									}									
								}
								l_pXMLWriter->AddAttribute("ImageList",l_strImageNameList.c_str());
								if( strlen(l_strImageTimeGapList.c_str()) )
									l_pXMLWriter->AddAttribute("TimeList",l_strImageTimeGapList.c_str());
				            l_pXMLWriter->EndElement();//end AnimationDataUnit
			            }
			        l_pXMLWriter->EndElement();//end AnimationData
			    }
				l_pXMLWriter->EndElement();//puzzleimage
			}
		}
		l_pXMLWriter->EndElement();
		SAFE_DELETE(l_pXMLWriter);
	#endif
	}
}