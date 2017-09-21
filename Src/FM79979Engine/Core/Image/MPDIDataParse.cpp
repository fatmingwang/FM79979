#include "../stdafx.h"
#include "AnimationParser.h"
#include "../XML/AtgXmlWriter.h"
#include "MPDI.h"
#include "MPDIDataParse.h"
#include "../GameplayUT/GameApp.h"
#include "../Sound/SoundManager.h"
//===================
//only could be included for AnimationParser.cpp
//===================
#define	MPDI_VERSION	1.0//April 05 2011,fatming.
#define	MPDI_VERSION_1_0
namespace FATMING_CORE
{
	//2,1,4,1
	//size,angle,color(RGBA),imageIndex
	//<PointData P1="0,0,0,0,0,0,0,0" P2="0,0,0,0,0,0,0,0" P3="0,0,0,0,0,0,0,0">
	inline	sTexBehaviorDataWithImageIndexData*	GetTexBehaviorDataWithImageIndexData(const wchar_t*e_str,sPuzzleData**e_ppPuzzleData,int e_iNumData,wchar_t*e_pstrNotExistImageName,Vector3*e_pPosOffset)
	{
		sPuzzleData*l_pPuzzleData = 0;
		char	l_strData[TEMP_SIZE];
		sprintf(l_strData,"%s",UT::WcharToChar(e_str).c_str());
		sTexBehaviorDataWithImageIndexData*	l_pData = new sTexBehaviorDataWithImageIndexData;
		char*	l_str = strtok(l_strData,",:");
		//size
		l_pData->Size.x = (float)atof(l_str);
		l_str = strtok(0,",:");
		l_pData->Size.y = (float)atof(l_str);
		l_str = strtok(0,",:");
		//angle
		l_pData->vAngle.z = (float)atof(l_str);
		l_str = strtok(0,",:");
		//color
		l_pData->vColor.x = atoi(l_str)/255.f;
		l_str = strtok(0,",:");
		l_pData->vColor.y = atoi(l_str)/255.f;
		l_str = strtok(0,",:");
		l_pData->vColor.z = atoi(l_str)/255.f;
		l_str = strtok(0,",:");
		l_pData->vColor.w = atoi(l_str)/255.f;
		l_str = strtok(0,",:");
		//image index
		l_pData->iImageIndex = -1;
		char	*l_strForImageName = l_str;
		for( int i=0;i<e_iNumData;++i )
		{
			if(!wcscmp(e_ppPuzzleData[i]->strFileName,UT::CharToWchar(l_strForImageName).c_str()))
			{
				l_pData->iImageIndex = i;
				l_pPuzzleData = e_ppPuzzleData[i];
				break;
			}
		}
		if(l_pData->iImageIndex == -1)
		{
			l_pData->iImageIndex = 0;
			if(e_pstrNotExistImageName)
			{
				int	l_iSize = (int)strlen(l_strForImageName);
				WCHAR_TO_WCHAR(UT::CharToWchar(l_strForImageName).c_str(),e_pstrNotExistImageName);
				e_pstrNotExistImageName[l_iSize] = L'\0';
			}
		}
		else
		{
			if(e_pstrNotExistImageName)
				e_pstrNotExistImageName[0] = L'\0';
		}
		//mirror
		l_str = strtok(0,",:");
		if( l_str )
		{
			l_pData->bMirror = atoi(l_str)?true:false;
		}
		if( l_pPuzzleData )
		{
			e_pPosOffset->x -= l_pPuzzleData->OriginalSize.x/2.f;
			e_pPosOffset->y -= l_pPuzzleData->OriginalSize.y/2.f;
		}
		return l_pData;
	}
#ifdef WIN32
//for MPDI old version 
#ifdef MPDI_VERSION_1_0
inline	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime)
	{	
		int	l_iNumPoint = (int)e_pCueToStartCurvesWithTime->GetOriginalPointList().size();		
		sTexBehaviorDataWithImageIndexData l_sTexBehaviorDataWithImageIndexData;
		for( int l=0;l<l_iNumPoint;++l )
		{
			sTexBehaviorDataWithImageIndexData *l_pTexBehaviorDataWithImageIndexData = (*e_pCueToStartCurvesWithTime->GetPointDataList())[l];
			e_pXMLWriter->StartElement("PointDataList");
				e_pXMLWriter->AddAttribute("Index",l);//just for quick check non-use
				e_pXMLWriter->AddAttribute("Position",ValueToString(e_pCueToStartCurvesWithTime->GetOriginalPointList()[l]));
				e_pXMLWriter->AddAttribute("Time",e_pCueToStartCurvesWithTime->GetOriginalTimeList()[l]);
				e_pXMLWriter->AddAttribute("Size",ValueToString(l_pTexBehaviorDataWithImageIndexData->Size));
				if( g_fMPDIVersion >= 1.2f )
				{
					if( l_pTexBehaviorDataWithImageIndexData->pPI && l_pTexBehaviorDataWithImageIndexData->pPI->GetObject(l_pTexBehaviorDataWithImageIndexData->iImageIndex) )
					{
						Vector2	l_vScale(1.f,1.f);
						sPuzzleData*l_pPuzzleData = l_pTexBehaviorDataWithImageIndexData->GetPuzzleData();
						l_vScale.x = l_pTexBehaviorDataWithImageIndexData->Size.x/l_pPuzzleData->Size.x;
						l_vScale.y = l_pTexBehaviorDataWithImageIndexData->Size.y/l_pPuzzleData->Size.y;
						e_pXMLWriter->AddAttribute("Scale",ValueToString(l_vScale));
					}
				}

				if( l_pTexBehaviorDataWithImageIndexData->vAngle.Length() != 0.f )
				{
					//if( l_pTexBehaviorDataWithImageIndexData->vAngle.x == 0.f&&
					//	l_pTexBehaviorDataWithImageIndexData->vAngle.y)
					//	e_pXMLWriter->AddAttribute("Angle",l_pTexBehaviorDataWithImageIndexData->vAngle.z);
					//else
						e_pXMLWriter->AddAttribute("Angles",ValueToString(l_pTexBehaviorDataWithImageIndexData->vAngle));
				}
				if( l_pTexBehaviorDataWithImageIndexData->vColor != Vector4::One )
				    e_pXMLWriter->AddAttribute("Color",ValueToString(l_pTexBehaviorDataWithImageIndexData->vColor));
				if( l_pTexBehaviorDataWithImageIndexData->pPI )
				{
				    e_pXMLWriter->AddAttribute("PIName",l_pTexBehaviorDataWithImageIndexData->pPI->GetName());
					if( l_pTexBehaviorDataWithImageIndexData->iImageIndex == -1 )
						e_pXMLWriter->AddAttribute("ImageName",UT::WcharToChar(l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[0]->strFileName));
					else
						e_pXMLWriter->AddAttribute("ImageName",UT::WcharToChar(l_pTexBehaviorDataWithImageIndexData->pPI->GetPuzzleData()[l_pTexBehaviorDataWithImageIndexData->iImageIndex]->strFileName));
				}
				else
				{
					if( e_pCueToStartCurvesWithTime->GetPuzzleImage(0) )
					{
						e_pXMLWriter->AddAttribute("PIName",e_pCueToStartCurvesWithTime->GetPuzzleImage(0)->GetName());
						e_pXMLWriter->AddAttribute("ImageName",e_pCueToStartCurvesWithTime->GetPuzzleImage(0)->GetObject(0)->GetName());
					}
				}
				if( l_pTexBehaviorDataWithImageIndexData->bMirror )
				    e_pXMLWriter->AddAttribute("Mirror",1);
			e_pXMLWriter->EndElement();
		}
		if( e_pCueToStartCurvesWithTime->GetHintPointVector()->size() )
		{
			l_iNumPoint = (int)e_pCueToStartCurvesWithTime->GetHintPointVector()->size();
			sHintPoint*l_pHintVector = &(*e_pCueToStartCurvesWithTime->GetHintPointVector())[0];
			e_pXMLWriter->StartElement("HintPointList");
				for( int l=0;l<l_iNumPoint;++l )
				{
					sHintPoint*l_pHintPoint = &l_pHintVector[l];
					e_pXMLWriter->AddAttribute("PointIndex",l_pHintPoint->iPointIndex);
					e_pXMLWriter->AddAttribute("Event",(int)l_pHintPoint->AnimationEvent);
					e_pXMLWriter->AddAttribute("EventName",l_pHintPoint->strEventName);
				}
			e_pXMLWriter->EndElement();//HintPointList
		}		
	}
#else
	inline	char*TexBehaviorDataWithImageIndexDataToString(
		sTexBehaviorDataWithImageIndexData e_sTexBehaviorDataWithImageIndexData,
		sPuzzleData**e_ppPuzzleData,int e_iNumData)
	{
		assert(e_sTexBehaviorDataWithImageIndexData.iImageIndex!=-1&&e_sTexBehaviorDataWithImageIndexData.iImageIndex<e_iNumData);
		static char l_str[TEMP_SIZE];
		sprintf(l_str,"%d,%d,%.2f,%d,%d,%d,%d,%s,%d\0",
		(int)e_sTexBehaviorDataWithImageIndexData.Size.x,
		(int)e_sTexBehaviorDataWithImageIndexData.Size.y,
		e_sTexBehaviorDataWithImageIndexData.fAngle,
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.x*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.y*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.z*255),
		(int)(e_sTexBehaviorDataWithImageIndexData.vColor.w*255),
		UT::WcharToChar(e_ppPuzzleData[e_sTexBehaviorDataWithImageIndexData.iImageIndex]->strFileName).c_str(),
		e_sTexBehaviorDataWithImageIndexData.bMirror?1:0
		);
		return l_str;
	}

	inline	void	ExportPointDataList(ATG::XMLWriter*e_pXMLWriter,cCueToStartCurveWithTime*e_pCueToStartCurvesWithTime)
	{	
		char	l_strForTemp[TEMP_SIZE];
		int	l_iNumPoint = (int)e_pCueToStartCurvesWithTime->GetOriginalPointList().size();
		e_pXMLWriter->StartElement("CurvePoint");
			for( int l=0;l<l_iNumPoint;++l )
			{
				sprintf(l_strForTemp,"P%d\0",l);
				e_pXMLWriter->AddAttribute(l_strForTemp,Vector3ToString(e_pCueToStartCurvesWithTime->GetOriginalPointList()[l]));
			}
		e_pXMLWriter->EndElement();//CurvePoint
		e_pXMLWriter->StartElement("CurveTime");
			for( int l=0;l<l_iNumPoint;++l )
			{
				sprintf(l_strForTemp,"P%d\0",l);
				e_pXMLWriter->AddAttribute(l_strForTemp,e_pCueToStartCurvesWithTime->GetOriginalTimeList()[l]);
			}
		e_pXMLWriter->EndElement();//CurveTime
		e_pXMLWriter->StartElement("PointDataList");
			for( int l=0;l<l_iNumPoint;++l )
			{
				sprintf(l_strForTemp,"P%d\0",l);
				e_pXMLWriter->AddAttribute(l_strForTemp,TexBehaviorDataWithImageIndexDataToString((*e_pCueToStartCurvesWithTime->GetPointDataList())[l],e_pCueToStartCurvesWithTime->GetPuzzleData(),e_pCueToStartCurvesWithTime->GetNumImage()));
			}
		e_pXMLWriter->EndElement();
		if( e_pCueToStartCurvesWithTime->GetHintPointVector()->size() )
		{
			l_iNumPoint = e_pCueToStartCurvesWithTime->GetHintPointVector()->size();
			sHintPoint*l_pHintVector = &(*e_pCueToStartCurvesWithTime->GetHintPointVector())[0];
			e_pXMLWriter->StartElement("HintPointList");
				for( int l=0;l<l_iNumPoint;++l )
				{
					sHintPoint*l_pHintPoint = &l_pHintVector[l];
					sprintf(l_strForTemp,"P%d\0",l);
					e_pXMLWriter->AddAttribute(l_strForTemp,l_pHintPoint->ToStringData());
				}
			e_pXMLWriter->EndElement();//HintPointList
		}		
	}
#endif	
	
	void	ExportMPDI(ATG::XMLWriter*e_pXMLWriter,cMPDIList*e_pMPDIList)
	{
		std::wstring l_strErrorMsg;
		e_pXMLWriter->StartElement("MPDIList");
		wchar_t	l_strMDPIName[MAX_PATH];
		swprintf(l_strMDPIName,MAX_PATH,L"%ls_mpdi",e_pMPDIList->GetName());
		e_pXMLWriter->AddAttribute("Name",l_strMDPIName);
		e_pXMLWriter->AddAttribute("Count",e_pMPDIList->Count());
		//1.1 fix point to leftup not center.
		//e_pXMLWriter->AddAttribute("MPDIVersion","1.1");
		//1.2 image size change to image scale,.
		//e_pXMLWriter->AddAttribute("MPDIVersion","1.2");
		//open binary or not by cGameApp::m_sbAllowParseBinaryFile
		e_pXMLWriter->AddAttribute("MPDIVersion","1.3");
		wstring	l_strPIName;
		int	l_iNumPI = e_pMPDIList->GetPIList()->Count();
		for( int i=0;i<l_iNumPI;++i )
		{
			l_strPIName += e_pMPDIList->GetPIList()->GetObject(i)->GetName();
			l_strPIName += L".pi";
			if( l_iNumPI-1 != i )
			{
				l_strPIName += L",";
			}
		}
		e_pXMLWriter->AddAttribute("PICount",l_iNumPI);
		e_pXMLWriter->AddAttribute("ImageName",l_strPIName.c_str());
		int	l_iNum = e_pMPDIList->Count();
		for( int j=0;j<l_iNum;++j )
		{
			cMultiPathDynamicImage*l_pMultiPathDynamicImage = (*e_pMPDIList)[j];
			e_pXMLWriter->StartElement("MultiPathDynamicImage");
			e_pXMLWriter->AddAttribute("Name",l_pMultiPathDynamicImage->GetName());
			e_pXMLWriter->AddAttribute("PathCount",l_pMultiPathDynamicImage->Count());
			e_pXMLWriter->AddAttribute("PlayCount",l_pMultiPathDynamicImage->GetPlayCount());
			e_pXMLWriter->AddAttribute("CameraView",ValueToStringW(l_pMultiPathDynamicImage->GetLocalTransform()));


			if( l_pMultiPathDynamicImage->GetViewPort() )
				e_pXMLWriter->AddAttribute("ViewPort",ValueToStringW(*l_pMultiPathDynamicImage->GetViewPort()));
			l_pMultiPathDynamicImage->RefreshTotalPlayTime();
			e_pXMLWriter->AddAttribute("TotalPlayTime",(float)l_pMultiPathDynamicImage->GetEndTime());
			if( l_pMultiPathDynamicImage->IsStayAtLastFrame() )
				e_pXMLWriter->AddAttribute("StayAtLastFrame",1);
			int	l_iNumPath = l_pMultiPathDynamicImage->Count();
			for( int k=0;k<l_iNumPath;++k )
			{
				cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = (*l_pMultiPathDynamicImage)[k];
				e_pXMLWriter->StartElement("CueToStartCurvesWithTime");
					e_pXMLWriter->AddAttribute("Name",l_pCueToStartCurvesWithTime->GetName());
					if(l_pCueToStartCurvesWithTime->IsBehaviorUpdateActive())
						e_pXMLWriter->AddAttribute("BehaviorUpdate",1);
					if(l_pCueToStartCurvesWithTime->IsColorBlending())
					{
						e_pXMLWriter->AddAttribute("Blending",1);
						e_pXMLWriter->AddAttribute("SrcBlendingData",BlendingIndexToIndex(l_pCueToStartCurvesWithTime->GetSrcBlendingMode()));
						e_pXMLWriter->AddAttribute("DestBlendingData",BlendingIndexToIndex(l_pCueToStartCurvesWithTime->GetDestBlendingMode()));
					}
					e_pXMLWriter->AddAttribute("StartTime",(float)l_pCueToStartCurvesWithTime->GetStartTime());
					Vector3	l_vRotationPosOffset = *l_pCueToStartCurvesWithTime->GetRotationAnglePosOffset();
					if( l_vRotationPosOffset.x != 0 || l_vRotationPosOffset.y != 0 || l_vRotationPosOffset.z != 0 )
					{
						e_pXMLWriter->AddAttribute("RotationPosOffset",ValueToStringW(*l_pCueToStartCurvesWithTime->GetRotationAnglePosOffset()));
					}
					int l_iPointCount = (int)l_pCueToStartCurvesWithTime->GetOriginalPointList().size();
					int l_iLODValue = l_pCueToStartCurvesWithTime->GetLOD();
					e_pXMLWriter->AddAttribute("AnchorType",ValueToStringW(l_pCueToStartCurvesWithTime->GetAnchorType()));
					if( l_iPointCount == 1 && l_iLODValue > 1 )
					{
						l_strErrorMsg += l_pCueToStartCurvesWithTime->GetName();
						l_strErrorMsg += L" path point count is 1 but LOD is not 1 render could be wrong!\n";
						l_pCueToStartCurvesWithTime->SetLOD(1);
						l_iLODValue = 1;
					}
					e_pXMLWriter->AddAttribute("Count",l_iPointCount);
					e_pXMLWriter->AddAttribute("LOD",l_iLODValue);

					if( l_pCueToStartCurvesWithTime->IsAnimationLoop() )
						e_pXMLWriter->AddAttribute("Loop",1);
					ExportPointDataList(e_pXMLWriter,l_pCueToStartCurvesWithTime);
				e_pXMLWriter->EndElement();//CueToStartCurvesWithTime
			}
			e_pXMLWriter->EndElement();//MultiPathDynamicImage
		}
		e_pXMLWriter->EndElement();//MPDIList
		if( l_strErrorMsg.length() > 0 )
		{
			UT::ErrorMsg(l_strErrorMsg.c_str(),L"ignore this message, because I have setup path LOD to 1 qoo");
		}
	}
//end WIN32
#endif	
	void	cAnimationParser::ProcessMPDI()
	{
		COMPARE_ELEMENT_VALUE("MPDIList")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				assert(!m_pCurrentcMPDIList);
				cMPDIList*l_pCurrentcMPDIList = new cMPDIList();
				m_pCurrentcMPDIList = l_pCurrentcMPDIList;
				int l_iPICount = 1;//old format only one pi
				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("MPDIVersion")
					{
						this->m_fMPDIVersion = VALUE_TO_FLOAT;
					}
					else
					COMPARE_NAME("Name")
					{
						m_pCurrentcMPDIList	= l_pCurrentcMPDIList;
						std::string	l_strMPDIName = UT::GetFileNameWithoutFullPath(this->m_strFileName);
						l_strMPDIName += "_mpdi";
						l_pCurrentcMPDIList->SetName(UT::CharToWchar(l_strMPDIName.c_str()));
						//l_pCurrentcMPDIList->SetName(l_strValue);
						bool l_b = this->AddObject( l_pCurrentcMPDIList );
						if( !l_b )
						{
							//remove old
							this->RemoveObject(l_pCurrentcMPDIList->GetName());
							//add new one.
							this->AddObject( l_pCurrentcMPDIList );
							this->m_strErrorMsg += l_strValue;
							this->m_strErrorMsg += L" such object already existed";
						}
					}
					else
				    COMPARE_NAME("PICount")
					{
					    l_iPICount = VALUE_TO_INT;
					}
					else
					COMPARE_NAME("ImageName")
					{
#ifdef ANDROID
						std::string	l_strTemp;
					    char*l_strPIName = 0;
					    char   l_strOriginalName[MAX_PATH];
						sprintf(l_strOriginalName,"%s",UT::WcharToChar(l_strValue).c_str());
					    int l_iTotalLength = 0;
					    if( l_iPICount > 1 )
					    {
					        l_strPIName = strtok(l_strOriginalName,",");
					        l_iTotalLength += strlen(l_strPIName)+1;
					    }
					    else
					    {
							l_strTemp = UT::WcharToChar(l_strValue);
							l_strPIName = (char*)l_strTemp.c_str();
					    }
					    for(int i=0;i<l_iPICount;++i)
					    {
							std::string	l_strFileName = StringAddDirectory(l_strPIName);
						    cPuzzleImage*l_pPuzzleImage = 0;
							l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList.GetObject(UT::GetFileNameWithoutFullPath(UT::CharToWchar(l_strFileName.c_str()))));
						    if(!l_pPuzzleImage)
						    {
								bool l_b = m_AllBaseImageList.Parse(l_strFileName.c_str());
							    if( !l_b )
							    {
									this->m_strErrorMsg += UT::CharToWchar(l_strFileName.c_str());
								    this->m_strErrorMsg += L" parse failed,format is utf-8!?\n";
							    }
								else
									l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList[m_AllBaseImageList.Count()-1]);
						    }
						    l_pCurrentcMPDIList->SetPuzzleImage(l_pPuzzleImage);
							if( l_iPICount != 1 && l_iPICount!= i+1 )
							{
								sprintf(l_strOriginalName,"%s",UT::WcharToChar(&l_strValue[l_iTotalLength]).c_str());
							    l_strPIName = strtok(l_strOriginalName,",\0");
							    l_iTotalLength += strlen(l_strPIName)+1;
							}
						}
#else
					    wchar_t*l_strPIName = 0;
					    wchar_t   l_strOriginalName[MAX_PATH];
					    swprintf(l_strOriginalName,MAX_PATH,L"%ls\0",l_strValue);
					    int l_iTotalLength = 0;
					    if( l_iPICount > 1 )
					    {
					        l_strPIName = wcstok(l_strOriginalName,L",");
					        l_iTotalLength += (int)wcslen(l_strPIName)+1;
					    }
					    else
					    {
					        l_strPIName = (wchar_t*)l_strValue;
					    }
					    for(int i=0;i<l_iPICount;++i)
					    {
							std::string	l_strFileName = StringAddDirectory(UT::WcharToChar(l_strPIName));
						    cPuzzleImage*l_pPuzzleImage = 0;
							l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList.GetObject(UT::GetFileNameWithoutFullPath(UT::CharToWchar(l_strFileName.c_str()))));
						    if(!l_pPuzzleImage)
						    {
							    bool l_b = m_AllBaseImageList.Parse(l_strFileName);
							    if( !l_b )
							    {
								    this->m_strErrorMsg += UT::CharToWchar(l_strFileName);
								    this->m_strErrorMsg += L" parse failed,format is utf-8!?\n";
							    }
								else
									l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_AllBaseImageList[m_AllBaseImageList.Count()-1]);
						    }
	    #ifdef DEBUG
	    #ifdef WIN32
						    else
						    {
							    cGameApp::OutputDebugInfoString(L"using same texture ");
							    cGameApp::OutputDebugInfoString(l_pPuzzleImage->GetName());
							    cGameApp::OutputDebugInfoString(L"\n");
						    }
	    #endif
	    #endif
						    l_pCurrentcMPDIList->SetPuzzleImage(l_pPuzzleImage);
							if( l_iPICount != 1 && l_iPICount!= i+1 )
							{
							    swprintf(l_strOriginalName,MAX_PATH,L"%ls\0",&l_strValue[l_iTotalLength]);
							    l_strPIName = wcstok(l_strOriginalName,L",\0");
							    l_iTotalLength += (int)wcslen(l_strPIName)+1;
							}
						}
#endif
					}
				TO_NEXT_VALUE
			}
			else
			{
	#ifdef DEBUG
				int	l_iCount = _wtoi(m_pCurrentTiXmlElement->Attribute(L"Count"));
				assert(m_pCurrentcMPDIList->Count()==l_iCount&&"MPDIList count is not correct");
	#endif
				m_pCurrentcMPDIList = 0;
			}
		}
		else
		COMPARE_ELEMENT_VALUE("MultiPathDynamicImage")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				assert(!m_pCurrentMultiPathDynamicImage);
				cMultiPathDynamicImage*l_pCurrentMultiPathDynamicImage = new cMultiPathDynamicImage(m_pCurrentcMPDIList);
				m_pCurrentMultiPathDynamicImage = l_pCurrentMultiPathDynamicImage;
				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("Name")
					{
						l_pCurrentMultiPathDynamicImage->SetName(l_strValue);
						bool	l_b = m_pCurrentcMPDIList->AddObject(l_pCurrentMultiPathDynamicImage);
						if( !l_b )
						{
							this->m_strErrorMsg += l_pCurrentMultiPathDynamicImage->GetName();
							this->m_strErrorMsg += L" data exited\n";
						}
					}
					else
					COMPARE_NAME("PlayCount")
					{
						l_pCurrentMultiPathDynamicImage->SetPlayCount(_wtoi(l_strValue));
					}
					else
					COMPARE_NAME("TotalPlayTime")
					{
						float	l_fTime = (float)_wtof(l_strValue);
						l_pCurrentMultiPathDynamicImage->SetTotalPlayTime(l_fTime,0.f);
					}
					else
					if(!wcscmp(L"StopAtLastFrame",l_strName)||!wcscmp(L"StayAtLastFrame",l_strName))
					{
						l_pCurrentMultiPathDynamicImage->SetStayAtLastFrame(true);
					}
					else
					COMPARE_NAME("ViewPort")
					{
						Vector4	l_vViewPort = VALUE_TO_VECTOR4;
						l_pCurrentMultiPathDynamicImage->SetViewPort(l_vViewPort);
					}
					else
					COMPARE_NAME("CameraView")
					{
						cMatrix44 l_Mat = GetMatrix(l_strValue);
						l_pCurrentMultiPathDynamicImage->SetLocalTransform(l_Mat);
					}
				TO_NEXT_VALUE
			}
			else
			{
	#ifdef DEBUG
				int	l_iPathCount = _wtoi(m_pCurrentTiXmlElement->Attribute(L"PathCount"));
				assert(l_iPathCount == m_pCurrentMultiPathDynamicImage->Count()&&"pathcount is not match");
	#endif
				if(m_pCurrentMultiPathDynamicImage)
				{
					m_pCurrentMultiPathDynamicImage->RefreshData();
				}
				m_pCurrentMultiPathDynamicImage = 0;
			}
		}
		else
		if( !wcscmp(m_pCurrentTiXmlElement->Value(),L"CueToStart2DCurvesWithTime")||//to keep old version capbility
			!wcscmp(m_pCurrentTiXmlElement->Value(),L"CueToStartCurvesWithTime"))
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				assert(!m_pCurrentCueToStartCurvesWithTime);
				cCueToStartCurveWithTime*l_pCueToStartCurvesWithTime = new cCueToStartCurveWithTime(m_pCurrentcMPDIList);
				m_pCurrentCueToStartCurvesWithTime = l_pCueToStartCurvesWithTime;

				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("Name")
					{
						l_pCueToStartCurvesWithTime->SetName(l_strValue);
					}
					COMPARE_NAME("AnchorType")
					{
						l_pCueToStartCurvesWithTime->SetAnchorType(GetAnchorType(l_strValue));
					}
					COMPARE_NAME("StartTime")
					{
						l_pCueToStartCurvesWithTime->SetStartTime(VALUE_TO_FLOAT);
					}
					COMPARE_NAME("BehaviorUpdate")
					{
						l_pCueToStartCurvesWithTime->SetBehaviorUpdateActive(true);
					}
					COMPARE_NAME("Loop")
					{
						l_pCueToStartCurvesWithTime->SetAnimationLoop(VALUE_TO_BOOLEAN);
						l_pCueToStartCurvesWithTime->SetCurveLoop(VALUE_TO_BOOLEAN);
					}
					COMPARE_NAME("RotationPosOffset")
					{
						l_pCueToStartCurvesWithTime->SetRotationAnglePosOffset(VALUE_TO_VECTOR3);
					}
					COMPARE_NAME("Blending")
					{
						l_pCueToStartCurvesWithTime->SetColorBlending(true);
					}
					COMPARE_NAME("SrcBlendingData")
					{
						if( m_fMPDIVersion >= 1.3f )
							l_pCueToStartCurvesWithTime->SetSrcBlendingMode((GLenum)IndexToBlendingIndex(VALUE_TO_INT));
						else
							l_pCueToStartCurvesWithTime->SetSrcBlendingMode(VALUE_TO_INT);
						
					}
					COMPARE_NAME("DestBlendingData")
					{
						if( m_fMPDIVersion >= 1.3f )
							l_pCueToStartCurvesWithTime->SetDestBlendingMode((GLenum)IndexToBlendingIndex(VALUE_TO_INT));
						else
							l_pCueToStartCurvesWithTime->SetDestBlendingMode(VALUE_TO_INT);

					}
				TO_NEXT_VALUE
				//m_pCurrentMultiPathDynamicImage->AddObject(l_pCueToStartCurvesWithTime);
				m_pCurrentMultiPathDynamicImage->AddObjectNeglectExist(l_pCueToStartCurvesWithTime);
			}
			else
			{
#ifdef DEBUG
#ifdef WIN32
				UINT	l_iCount = _wtoi(m_pCurrentTiXmlElement->Attribute(L"Count"));
				assert( l_iCount == m_pCurrentCueToStartCurvesWithTime->GetOriginalPointList().size()&&"pathcount is not match point" );
				assert( l_iCount == m_pCurrentCueToStartCurvesWithTime->GetOriginalTimeList().size()&&"pathcount is not match time" );
				size_t l_iTestCount  = 0;
				l_iTestCount = m_pCurrentCueToStartCurvesWithTime->GetPointDataList()->size();
				std::vector<sTexBehaviorDataWithImageIndexData*>*l_pp = m_pCurrentCueToStartCurvesWithTime->GetPointDataList();
				l_iTestCount = l_pp->size();
				assert( l_iCount == m_pCurrentCueToStartCurvesWithTime->GetPointDataList()->size()&&"pathcount is not match pointlist" );
#endif
#endif
				int	l_iLOD = _wtoi(m_pCurrentTiXmlElement->Attribute(L"LOD"));
				m_pCurrentCueToStartCurvesWithTime->SetLOD(l_iLOD);
				m_pCurrentCueToStartCurvesWithTime->DoLOD();
				m_pCurrentCueToStartCurvesWithTime = 0;
			}
		}
		else//old version
		COMPARE_ELEMENT_VALUE("CurvePoint")
		{
			assert(this->m_fMPDIVersion <= 0.1f);
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				std::vector<Vector3>	l_PosList;
				PARSE_CURRENT_ELEMENT_START
					Vector3	l_vPos = GetVector3(l_strValue);
					l_PosList.push_back(l_vPos);
				TO_NEXT_VALUE
				m_pCurrentCueToStartCurvesWithTime->SetOriginalPointList(l_PosList);
			}
		}
		else//old version
		COMPARE_ELEMENT_VALUE("CurveTime")
		{
			assert(this->m_fMPDIVersion <= 0.1f);
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				std::vector<float>	l_TimeList;
				PARSE_CURRENT_ELEMENT_START
					float	l_fTime = (float)_wtof(l_strValue);
					l_TimeList.push_back(l_fTime);
				TO_NEXT_VALUE
				m_pCurrentCueToStartCurvesWithTime->SetOriginalTimeList(l_TimeList);
			}
		}
		else
		COMPARE_ELEMENT_VALUE("PointDataList")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{//for new version data
				if( this->m_fMPDIVersion >= 1.f )
				{
					float	l_fTime = 0.f;
					Vector3	l_vPos;
					cPuzzleImage*l_pPuzzleImage = 0;
					sPuzzleData*l_pPuzzleData = 0;
					const wchar_t*l_strPIName = 0;
					Vector2	l_vScale(-1.f,-1.f);
					sTexBehaviorDataWithImageIndexData	l_sTexBehaviorDataWithImageIndexData;
					PARSE_CURRENT_ELEMENT_START
						COMPARE_NAME("Index")
						{

						}
						else				
						COMPARE_NAME("Size")
						{
							l_sTexBehaviorDataWithImageIndexData.Size = VALUE_TO_VECTOR2;
						}
						else				
						COMPARE_NAME("Scale")
						{
							l_vScale = VALUE_TO_VECTOR2;
						}
						else//xy
						COMPARE_NAME("Angle")
						{
							l_sTexBehaviorDataWithImageIndexData.vAngle.z = VALUE_TO_FLOAT;
						}
						else//xyz
						COMPARE_NAME("Angles")
						{
							l_sTexBehaviorDataWithImageIndexData.vAngle = VALUE_TO_VECTOR3;
						}
						else
						COMPARE_NAME("Color")
						{
							l_sTexBehaviorDataWithImageIndexData.vColor = VALUE_TO_VECTOR4;
						}
						else
						COMPARE_NAME("PIName")
						{
							l_strPIName = l_strValue;
							l_pPuzzleImage = this->m_pCurrentcMPDIList->GetPuzzleImage(l_strValue);
							if( l_pPuzzleImage )
							{
								m_pCurrentCueToStartCurvesWithTime->SetPuzzleImage(l_pPuzzleImage);
							}
							else
							{
								//some people make a lazy way to fetch Image without PI ,this is totaly wrong,so I have to add this line to fix this -_-"
								if (wcslen(l_strValue) == 0 && abs(this->m_fMPDIVersion - 1.2)<0.01f)
								{
									const wchar_t* l_strImageName = m_pCurrentTiXmlElement->Attribute(L"ImageName");
									if (l_strImageName)
									{
										m_pCurrentCueToStartCurvesWithTime->SetName(l_strImageName);
									}
								}
								else
								{
									this->m_strErrorMsg += l_strValue;
									this->m_strErrorMsg += L" pi not exist!";
								}
							}
						}
						else
						COMPARE_NAME("ImageName")
						{
							if( l_pPuzzleImage )
							{
								l_sTexBehaviorDataWithImageIndexData.iImageIndex = l_pPuzzleImage->GetObjectIndexByName(l_strValue);
								if( l_sTexBehaviorDataWithImageIndexData.iImageIndex != -1 )
								{
									l_pPuzzleData = l_pPuzzleImage->GetPuzzleData()[l_sTexBehaviorDataWithImageIndexData.iImageIndex];
								}
							}
							else
								l_sTexBehaviorDataWithImageIndexData.iImageIndex = -1;
							if( l_sTexBehaviorDataWithImageIndexData.iImageIndex == -1 )
							{
								l_sTexBehaviorDataWithImageIndexData.iImageIndex = 0;
								this->m_strErrorMsg += l_strValue;
								this->m_strErrorMsg += L" is not exist at pi ";
								this->m_strErrorMsg += l_strPIName;
								this->m_strErrorMsg += L"\n";
							}
						}
						else
						COMPARE_NAME("Mirror")
						{
							l_sTexBehaviorDataWithImageIndexData.bMirror = VALUE_TO_BOOLEAN;
						}
						else
						COMPARE_NAME("Position")
						{
							l_vPos = VALUE_TO_VECTOR3;
						}
						else
						COMPARE_NAME("Time")
						{
							l_fTime = VALUE_TO_FLOAT;
						}
					PARSE_NAME_VALUE_END
					if(m_fMPDIVersion <= 1.0f )
					{
						//Vector2 l_vScale( (l_sTexBehaviorDataWithImageIndexData.Size.x/l_pPuzzleData->Size.x),(l_sTexBehaviorDataWithImageIndexData.Size.y/l_pPuzzleData->Size.y) );
						if( l_pPuzzleData )
						{
							l_vPos.x -= l_pPuzzleData->OriginalSize.x/2.f;
							l_vPos.y -= l_pPuzzleData->OriginalSize.y/2.f;
						}
					}
					if( l_vScale.x != -1 && l_pPuzzleImage  )
					{
						cPuzzleImageUnit*l_pPZUnit = l_pPuzzleImage->GetObject(l_sTexBehaviorDataWithImageIndexData.iImageIndex);
						POINT	l_vCheckSize = l_pPZUnit->GetPuzzleData()->Size;
						Vector2	l_vCompareScale(l_sTexBehaviorDataWithImageIndexData.Size.x/l_vCheckSize.x,l_sTexBehaviorDataWithImageIndexData.Size.y/l_vCheckSize.y);
						if( fabs(l_vScale.x - l_vCompareScale.x) > EPSIONAL )
						{
							l_sTexBehaviorDataWithImageIndexData.Size.x = l_vScale.x*l_vCheckSize.x;
						}
						if( fabs(l_vScale.y - l_vCompareScale.y) > EPSIONAL )
						{
							l_sTexBehaviorDataWithImageIndexData.Size.y = l_vScale.y*l_vCheckSize.y;
						}
					}
					m_pCurrentCueToStartCurvesWithTime->AddPoint(l_vPos,l_fTime,l_sTexBehaviorDataWithImageIndexData.Size,l_sTexBehaviorDataWithImageIndexData.vAngle,l_sTexBehaviorDataWithImageIndexData.vColor,l_sTexBehaviorDataWithImageIndexData.iImageIndex,l_sTexBehaviorDataWithImageIndexData.bMirror,l_pPuzzleImage,false);
				}
				else
				{
					std::vector<sTexBehaviorDataWithImageIndexData*>*l_pPointDataList = m_pCurrentCueToStartCurvesWithTime->GetPointDataList();
					std::vector<std::wstring>	l_strAllErrorImageName;
					wchar_t	l_strNotExistImageName[MAX_PATH];
					memset(l_strNotExistImageName,0,sizeof(wchar_t));
					bool	l_bError = false;
					int	l_iIndex = 0;
					PARSE_CURRENT_ELEMENT_START
						Vector3	l_vOriginalPos = m_pCurrentCueToStartCurvesWithTime->GetOriginalPointList()[l_iIndex];
					    sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = GetTexBehaviorDataWithImageIndexData(l_strValue,this->m_pCurrentcMPDIList->GetPuzzleData(),m_pCurrentcMPDIList->GetNumImage(),l_strNotExistImageName,&l_vOriginalPos);
						l_pTexBehaviorDataWithImageIndexData->pPI = m_pCurrentcMPDIList->GetPuzzleImage();
						l_pPointDataList->push_back(l_pTexBehaviorDataWithImageIndexData);
						if( wcslen(l_strNotExistImageName) )
							l_strAllErrorImageName.push_back(l_strNotExistImageName);
						m_pCurrentCueToStartCurvesWithTime->FixPoint(l_iIndex,l_vOriginalPos);
						++l_iIndex;
					TO_NEXT_VALUE
	#ifdef WIN32
					if(l_strAllErrorImageName.size())
					{
						for( size_t i=0;i<l_strAllErrorImageName.size();++i )
						{
							this->m_strErrorMsg += l_strAllErrorImageName[i].c_str();
							this->m_strErrorMsg += L" image not exist!\n";
						}
					}
	#endif//end win32
				}
			}
		}
		else
		COMPARE_ELEMENT_VALUE("HintPointList")
		{
			if(!this->m_pCurrentTiXmlElement->m_bDone)
			{
				std::vector<sHintPoint>*l_pHintVector = m_pCurrentCueToStartCurvesWithTime->GetHintPointVector();
				int l_iIndex = -1;
				std::wstring	l_strEventName;
				eAnimationEvent l_eAnimationEvent;
				PARSE_CURRENT_ELEMENT_START
					COMPARE_NAME("PointIndex")
					{
						l_iIndex = VALUE_TO_INT;
					}
					else
					COMPARE_NAME("Event")
					{
						l_eAnimationEvent = (eAnimationEvent)VALUE_TO_INT;
					}
					else
					COMPARE_NAME("EventName")
					{
						l_strEventName = l_strValue;
					}
				TO_NEXT_VALUE
				sHintPoint	l_HintPoint(l_iIndex,(wchar_t*)l_strEventName.c_str(),l_eAnimationEvent);
				if( l_eAnimationEvent == eAE_SOUND )
				{
					std::string	l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += ValueToString(l_strEventName);
					if(cGameApp::m_spSoundParser->AddStaticSound(m_pCurrentCueToStartCurvesWithTime,l_strFileName.c_str()))
					{
						
					}
				}
				l_pHintVector->push_back(l_HintPoint);
				//stupid sort method start
				std::vector<sHintPoint>	l_HintVector;
				//sort the hint point data let we could fetch the data if same point has more than one hint.see declation
				for(UINT i=0;i<m_pCurrentCueToStartCurvesWithTime->GetPointDataList()->size();++i)
				{
					for( UINT j=0;j<l_pHintVector->size();++j )
					{
						if( i == (*l_pHintVector)[j].iPointIndex)
							l_HintVector.push_back((*l_pHintVector)[j]);
					}
				}
				m_pCurrentCueToStartCurvesWithTime->SetHintPointVector(l_HintVector);
			}
		}
		else
		{
			//assert(0);
		}
	}
}