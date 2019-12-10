#include "ImageParser.h"
#include "../../../Common/Utility.h"
#include "../CommonRender/NumeralImage.h"
#include "../../../XML/XMLLoader.h"
#ifdef WIN32
#include "../../../XML/AtgXmlWriter.h"
#endif
#include "../PuzzleImage/PuzzleImage.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cImageParser);
    //bool					g_bImageLoaderForFetchPixelData	= false;
	//sometimes we only need to PI data but no pixels data
	//bool    g_bSkipImageLoad = false;
	cImageParser::cImageParser()
	{
	}
	//===============
	//
	//===============
	cImageParser::~cImageParser()
	{

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


	cPuzzleImage*	cImageParser::GetPuzzleImageByFileName(const char*e_strName)
	{
		std::wstring	l_strName = UT::CharToWchar(e_strName);
		return GetPuzzleImage(e_strName, l_strName.c_str());
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
			cPuzzleImage*l_pPI = new cPuzzleImage();
			if (!l_pPI->ParseWithMyParse(e_strFileName))
			{
				UT::ErrorMsg("parse file eailed",e_strFileName);
				return 0;
			}
			this->AddObjectNeglectExist(l_pPI);
			return l_pPI;
		}
		return l_pPI;
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
										l_strImageNameList += l_pPuzzleImage->GetPuzzleData(l_pImageIndexOfAnimation->m_ImageAnimationDataList[k].iIndex)->strFileName;
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

	bool	IsObjectOverlap(cImageParser*e_pObject)
	{
		cImageParser l_Temp;
		l_Temp.CopyListPointer(e_pObject);
		while( l_Temp.Count() )
		{
			int l_iCount = l_Temp.Count();
			cUIImage*l_pObject = dynamic_cast<cUIImage*>(l_Temp.GetObject(0));
			for(int i=1;i<l_iCount;++i)
			{
				POINT l_POINT = *l_pObject->GetOffsetPos();
				RECT l_rect;
				l_rect.left = (long)(l_POINT.x+l_pObject->GetPos().x);
				l_rect.top = (long)(l_POINT.y+l_pObject->GetPos().y);
				POINT l_Size = {l_pObject->GetRightDownStripOffPos().x-l_pObject->GetOffsetPos()->x,l_pObject->GetRightDownStripOffPos().y-l_pObject->GetOffsetPos()->y};
				l_rect.right = (long)l_rect.left+l_Size.x;
				l_rect.bottom = (long)l_rect.top+l_Size.y;
				cUIImage*l_pCompareObject = dynamic_cast<cUIImage*>(l_Temp.GetObject(i));
				l_POINT = *l_pCompareObject->GetOffsetPos();
				RECT l_rect2;
				l_rect2.left = l_POINT.x+(long)l_pCompareObject->GetPos().x;
				l_rect2.top = l_POINT.y+(long)l_pCompareObject->GetPos().y;
				POINT l_Size2 = {l_pCompareObject->GetRightDownStripOffPos().x-l_pCompareObject->GetOffsetPos()->x,l_pCompareObject->GetRightDownStripOffPos().y-l_pCompareObject->GetOffsetPos()->y};
				l_rect2.right = l_rect2.left+l_Size2.x;
				l_rect2.bottom = l_rect2.top+l_Size2.y;
				if(UT::RectCollideRect(l_rect2,l_rect))
					return true;
			}
			l_Temp.RemoveObjectWithoutDelete(0);
		}
		return false;
	}
}