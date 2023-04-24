#include "AnimationParser.h"
#ifdef WIN32
#include "../../../XML/AtgXmlWriter.h"
#endif
#include "../MPDI/MPDI.h"
#include "../MPDI/MPDIDataParse.h"


namespace FATMING_CORE
{
	extern sTexBehaviorDataWithImageIndexData* GetTexBehaviorDataWithImageIndexData(const wchar_t* e_str, sPuzzleData* e_pAllPuzzleData, int e_iNumData, wchar_t* e_pstrNotExistImageName, Vector3* e_pPosOffset);
	const char*				cAnimationParser::ExtensionNameID( ".mpdi" );
	TYPDE_DEFINE_MARCO(cAnimationParser);
	cAnimationParser::cAnimationParser()
	{
		this->m_bTryWithoutFullFilePathName = true;
		m_fMPDIVersion = 0.f;
		m_pCurrentcMPDIList = 0;
		m_pCurrentMultiPathDynamicImage = 0;	
		m_pCurrentCueToStartCurvesWithTime = 0;
	}

	cAnimationParser::~cAnimationParser()
	{
		this->Destroy();
		m_AllBaseImageList.Destroy();
	}


	void cAnimationParser::ProcessMPDIList(TiXmlElement* e_pTiXmlElement)
	{
		cMPDIList*l_pCurrentcMPDIList = new cMPDIList();
		m_pCurrentcMPDIList = l_pCurrentcMPDIList;
		int l_iPICount = 1;//old format only one pi
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("MPDIVersion")
			{
				this->m_fMPDIVersion = VALUE_TO_FLOAT;
			}
			else
			COMPARE_NAME("Name")
			{
				m_pCurrentcMPDIList	= l_pCurrentcMPDIList;
				std::string	l_strMPDIName = this->m_strFileName;//UT::GetFileNameWithoutFullPath(this->m_strFileName);
				l_pCurrentcMPDIList->SetName(l_strMPDIName.c_str());
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
				//fuck test here.
				wchar_t*	l_strPIName = 0;
				wchar_t		l_strOriginalName[TEMP_SIZE];
				wchar_t*	l_pForwcstok_s = nullptr;
				swprintf(l_strOriginalName,TEMP_SIZE,L"%ls",l_strValue);
				int l_iTotalLength = 0;
				if( l_iPICount > 1 )
				{
					l_strPIName = wcstok_s(l_strOriginalName,L",",&l_pForwcstok_s);
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
						l_pPuzzleImage = m_AllBaseImageList.GetPuzzleImageByFileName(l_strFileName.c_str());
						if( !l_pPuzzleImage)
						{
							this->m_strErrorMsg += UT::CharToWchar(l_strFileName);
							this->m_strErrorMsg += L" parse failed,format is utf-8!?\n";
						}
					}
#ifdef DEBUG
#ifdef WIN32
					else
					{
								
						FMLog::LogWithFlag(ComposeMsgByFormat(L"using same texture %ls", l_pPuzzleImage->GetName()), CORE_LOG_FLAG);
					}
#endif
#endif
					l_pCurrentcMPDIList->SetPuzzleImage(l_pPuzzleImage);
					if( l_iPICount != 1 && l_iPICount!= i+1 )
					{
						swprintf(l_strOriginalName,MAX_PATH,L"%ls\0",&l_strValue[l_iTotalLength]);
						l_strPIName = wcstok_s(l_strOriginalName,L",\0", &l_pForwcstok_s);
						l_iTotalLength += (int)wcslen(l_strPIName)+1;
					}
				}
//#endif
			}
		TO_NEXT_VALUE
		auto l_pChildElement = e_pTiXmlElement->FirstChildElement();
		while (l_pChildElement)
		{
			COMPARE_TARGET_ELEMENT_VALUE(l_pChildElement, "MultiPathDynamicImage")
			{
				ProcessMultiPathDynamicImage(l_pChildElement);
			}
			l_pChildElement = l_pChildElement->NextSiblingElement();
		}
#ifdef DEBUG
		int	l_iCount = _wtoi(e_pTiXmlElement->Attribute(L"Count"));
		assert(m_pCurrentcMPDIList->Count()==l_iCount&&"MPDIList count is not correct");
#endif
		m_pCurrentcMPDIList = 0;
	}


	void cAnimationParser::ProcessMultiPathDynamicImage(TiXmlElement* e_pTiXmlElement)
	{
		cMultiPathDynamicImage*l_pCurrentMultiPathDynamicImage = new cMultiPathDynamicImage(m_pCurrentcMPDIList);
		m_pCurrentMultiPathDynamicImage = l_pCurrentMultiPathDynamicImage;
		PARSE_ELEMENT_START(e_pTiXmlElement)
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
		auto l_pChildElement = e_pTiXmlElement->FirstChildElement();
		while (l_pChildElement)
		{
			if (!wcscmp(l_pChildElement->Value(), L"CueToStartCurvesWithTime"))
			{
				ProcessCueToStartCurvesWithTime(l_pChildElement);
			}
			l_pChildElement = l_pChildElement->NextSiblingElement();
		}
#ifdef DEBUG
		int	l_iPathCount = _wtoi(e_pTiXmlElement->Attribute(L"PathCount"));
		assert(l_iPathCount == m_pCurrentMultiPathDynamicImage->Count()&&"pathcount is not match");
#endif
		if(m_pCurrentMultiPathDynamicImage)
		{
			m_pCurrentMultiPathDynamicImage->Init();
			m_pCurrentMultiPathDynamicImage->Update(0.0001f);
			m_pCurrentMultiPathDynamicImage->RefreshData();
			m_pCurrentMultiPathDynamicImage->SetAnimationDone(true);
		}
		m_pCurrentMultiPathDynamicImage = 0;
	}

	void cAnimationParser::ProcessCueToStartCurvesWithTime(TiXmlElement* e_pTiXmlElement)
	{
		cCueToStartCurveWithTime* l_pCueToStartCurvesWithTime = new cCueToStartCurveWithTime(m_pCurrentcMPDIList);
		m_pCurrentCueToStartCurvesWithTime = l_pCueToStartCurvesWithTime;
		PARSE_ELEMENT_START(e_pTiXmlElement)
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
			if (m_fMPDIVersion >= 1.3f)
				l_pCueToStartCurvesWithTime->SetSrcBlendingMode((GLenum)IndexToBlendingIndex(VALUE_TO_INT));
			else
				l_pCueToStartCurvesWithTime->SetSrcBlendingMode(VALUE_TO_INT);

		}
		COMPARE_NAME("DestBlendingData")
		{
			if (m_fMPDIVersion >= 1.3f)
				l_pCueToStartCurvesWithTime->SetDestBlendingMode((GLenum)IndexToBlendingIndex(VALUE_TO_INT));
			else
				l_pCueToStartCurvesWithTime->SetDestBlendingMode(VALUE_TO_INT);

		}
		TO_NEXT_VALUE
		m_pCurrentMultiPathDynamicImage->AddObjectNeglectExist(l_pCueToStartCurvesWithTime);
		auto l_pChildElement = e_pTiXmlElement->FirstChildElement();
		while (l_pChildElement)
		{
			if (!wcscmp(l_pChildElement->Value(), L"PointDataList"))
			{
				ProcessPointDataList(l_pChildElement);
			}
			l_pChildElement = l_pChildElement->NextSiblingElement();
		}
#ifdef DEBUG
#ifdef WIN32
		UINT	l_iCount = _wtoi(e_pTiXmlElement->Attribute(L"Count"));
		assert(l_iCount == m_pCurrentCueToStartCurvesWithTime->GetOriginalPointList().size() && "pathcount is not match point");
		assert(l_iCount == m_pCurrentCueToStartCurvesWithTime->GetOriginalTimeList().size() && "pathcount is not match time");
		size_t l_iTestCount = 0;
		l_iTestCount = m_pCurrentCueToStartCurvesWithTime->GetPointDataList()->size();
		std::vector<sTexBehaviorDataWithImageIndexData*>* l_pp = m_pCurrentCueToStartCurvesWithTime->GetPointDataList();
		l_iTestCount = l_pp->size();
		assert(l_iCount == m_pCurrentCueToStartCurvesWithTime->GetPointDataList()->size() && "pathcount is not match pointlist");
#endif
#endif
		int	l_iLOD = _wtoi(e_pTiXmlElement->Attribute(L"LOD"));
		m_pCurrentCueToStartCurvesWithTime->SetLOD(l_iLOD);
		m_pCurrentCueToStartCurvesWithTime->DoLOD();
	}

	void cAnimationParser::ProcessPointDataList(TiXmlElement* e_pTiXmlElement)
	{
		if( this->m_fMPDIVersion >= 1.f )
		{
			float	l_fTime = 0.f;
			Vector3	l_vPos;
			cPuzzleImage*l_pPuzzleImage = 0;
			sPuzzleData*l_pPuzzleData = 0;
			Vector2	l_vScale(-1.f,-1.f);
			sTexBehaviorDataWithImageIndexData	l_sTexBehaviorDataWithImageIndexData;
			PARSE_ELEMENT_START(e_pTiXmlElement)
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
					l_pPuzzleImage = this->m_pCurrentcMPDIList->GetPuzzleImage(l_strValue);
					if( l_pPuzzleImage )
					{
						m_pCurrentCueToStartCurvesWithTime->SetPuzzleImage(l_pPuzzleImage);
					}
					else
					{
						//some people make a lazy way to fetch Image without PI ,this is totaly wrong,so I have to add this line to fix this -_-"
						//07/03/2018 I forgot why add this...
						if (wcslen(l_strValue) == 0 && abs(this->m_fMPDIVersion - 1.2)<0.01f)
						{
							const wchar_t* l_strImageName = e_pTiXmlElement->Attribute(L"ImageName");
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
							l_pPuzzleData = l_pPuzzleImage->GetPuzzleData(l_sTexBehaviorDataWithImageIndexData.iImageIndex);
						}
					}
					else
					{
						l_sTexBehaviorDataWithImageIndexData.iImageIndex = -1;
					}
					if( l_sTexBehaviorDataWithImageIndexData.iImageIndex == -1 )
					{
						l_sTexBehaviorDataWithImageIndexData.iImageIndex = 0;
						this->m_strErrorMsg += l_strValue;
						this->m_strErrorMsg += L" is not exist at pi ";
						this->m_strErrorMsg += l_strValue;
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
			TO_NEXT_VALUE
			if(m_fMPDIVersion <= 1.0f )
			{
				//Vector2 l_vScale( (l_sTexBehaviorDataWithImageIndexData.Size.x/l_pPuzzleData->Size.x),(l_sTexBehaviorDataWithImageIndexData.Size.y/l_pPuzzleData->Size.y) );
				if( l_pPuzzleData )
				{
					l_vPos.x -= (float)l_pPuzzleData->OriginalSize.x/2.f;
					l_vPos.y -= (float)l_pPuzzleData->OriginalSize.y/2.f;
				}
			}
			if( l_vScale.x != -1 && l_pPuzzleImage  )
			{
				cPuzzleImageUnit*l_pPZUnit = l_pPuzzleImage->GetObject(l_sTexBehaviorDataWithImageIndexData.iImageIndex);
				POINT	l_vCheckSize = l_pPZUnit->GetPuzzleData()->Size;
				Vector2	l_vCompareScale(l_sTexBehaviorDataWithImageIndexData.Size.x/ (float)l_vCheckSize.x,l_sTexBehaviorDataWithImageIndexData.Size.y/ (float)l_vCheckSize.y);
				if( fabs(l_vScale.x - l_vCompareScale.x) > EPSIONAL )
				{
					l_sTexBehaviorDataWithImageIndexData.Size.x = l_vScale.x*(float)l_vCheckSize.x;
				}
				if( fabs(l_vScale.y - l_vCompareScale.y) > EPSIONAL )
				{
					l_sTexBehaviorDataWithImageIndexData.Size.y = l_vScale.y*(float)l_vCheckSize.y;
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
			PARSE_ELEMENT_START(e_pTiXmlElement)
				Vector3	l_vOriginalPos = m_pCurrentCueToStartCurvesWithTime->GetOriginalPointList()[l_iIndex];
				sTexBehaviorDataWithImageIndexData*l_pTexBehaviorDataWithImageIndexData = GetTexBehaviorDataWithImageIndexData(l_strValue,this->m_pCurrentcMPDIList->GetAllPuzzleData(),m_pCurrentcMPDIList->GetNumImage(),l_strNotExistImageName,&l_vOriginalPos);
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
	void cAnimationParser::ProcessHintPoint(TiXmlElement* e_pTiXmlElement)
	{
	}
	//kill imageplayer and image resource and puzzle data
	void	cAnimationParser::RemoveResourceObject(NamedTypedObject*e_pObject)
	{
		if(e_pObject->Type() == cMPDIList::TypeID)
		{
		
		}
		else
		{
			assert(0&&"not cSAList and is not MPDIList");
			return;
		}
		DeleteBaseImage(e_pObject);
		DeleteSoundObject(e_pObject);
	}

	void	cAnimationParser::DeleteSoundObject(NamedTypedObject*e_pNamedTypedObject)
	{
		cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(e_pNamedTypedObject);
		l_pMPDIList->ReleaseHintpointSoundData();
	}

	void	cAnimationParser::DeleteBaseImage(NamedTypedObject*e_pNamedTypedObject)
	{
		//now smart pointer working!?
		//its possible the orther object still using so skip this
		return;
		if( m_AllBaseImageList.Count() == 0 )
			return;
		cMulti_PI_Image*l_pMulti_PI_Image = dynamic_cast<cMulti_PI_Image*>(e_pNamedTypedObject);
		cNamedTypedObjectVector<cPuzzleImage>*l_pPuzzleImageList = l_pMulti_PI_Image->GetPIList();
		//if use value is biger than one it mean the other people is using this puzzleimage still
		int	l_iPICount = l_pPuzzleImageList->Count();
		for( int i=0;i<l_iPICount;++i )
		{
			cPuzzleImage*l_pPI = l_pPuzzleImageList->GetObject(i);
			int	l_iNumMPDI = this->Count();
			bool	l_bInUse = false;
			for(int j=0;j<l_iNumMPDI;++j)
			{
				cMulti_PI_Image*l_pCompare = dynamic_cast<cMulti_PI_Image*>(this->m_ObjectList[j]);
				if( l_pCompare && l_pCompare != l_pMulti_PI_Image )
				{
					if( l_pCompare->IsContainPI(l_pPI) )
					{
						//j = l_iNumPI;
						l_bInUse = true;
						break;
					}
				}
			}
			if( !l_bInUse )
			{
				bool	l_b = this->m_AllBaseImageList.RemoveObject(l_pPI);
				if(!l_b)
				{
					m_strErrorMsg += e_pNamedTypedObject->GetName();
					m_strErrorMsg += L"not existed void	cAnimationParser::DeleteBaseImage(NamedTypedObject*e_pNamedTypedObject)\n";
				}				
			}
		}
	}

	bool		cAnimationParser::MyParse(TiXmlElement* e_pRoot)
	{
		while (e_pRoot)
		{
			auto l_strValue = e_pRoot->Value();
			if (wcscmp(l_strValue, L"MPDIList") == 0)
			{
				ProcessMPDIList(e_pRoot);
				return true;
			}
			else
			{

			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
		return false;
	}


	cMPDIList*cAnimationParser::GetMPDIListByFileName(const char*e_strFileName,bool e_bShowErrorMessage)
	{
		return dynamic_cast<cMPDIList*>(GetObject(UT::CharToWchar(e_strFileName).c_str()));
	}

	cMPDIList*cAnimationParser::GetMPDIListByFileName(const wchar_t*e_strFileName,bool e_bShowErrorMessage)
	{
		std::string	l_strFileName = UT::WcharToChar(e_strFileName);
		return GetMPDIList(l_strFileName.c_str(), e_strFileName);
	}

	cMPDIList*cAnimationParser::GetMPDIList(const char*e_strFileName,const wchar_t*e_strName,bool e_bShowErrorMessage)
	{
		cMPDIList*l_pMPDIList = this->GetMPDIList(e_strName);
		if( !l_pMPDIList )
		{

			//bool	l_b = this->Parse(e_strFileName);
			bool	l_b = this->ParseWithMyParse(e_strFileName);
			if( !l_b )
			{
				if( this->m_bShowErrorMsg || e_bShowErrorMessage )
					UT::ErrorMsg(e_strFileName,"MPDIList parse failed");
				return 0;
			}
			l_pMPDIList = this->GetMPDIList(e_strName);
		}
		return l_pMPDIList;
	}

	int		cAnimationParser::NumObjectUsingPI(cPuzzleImage*e_pPI)
	{
		int	l_iNum = 0;
		int	l_iNumMPDI = this->Count();
		for(int j=0;j<l_iNumMPDI;++j)
		{
			cMulti_PI_Image*l_pCompare = dynamic_cast<cMulti_PI_Image*>(this->m_ObjectList[j]);
			if( l_pCompare->GetPuzzleImage(e_pPI->GetName()) )
			{
				++l_iNum;
			}
		}
		return l_iNum;
	}

	void	cAnimationParser::Export(const char*e_strFileName,bool e_bBinary)
	{
#if defined(WIN32) && !defined(UWP)
		ATG::XMLWriter	l_XMLWriter(e_strFileName);
		//l_XMLWriter.StartElement("Version");
		//l_XMLWriter.AddAttribute("MPDIVersion","1.0");
		//l_XMLWriter.EndElement();
		for( int i=0;i<Count();++i )
		{
			NamedTypedObject* l_pNamedTypedObject = (*this)[i];
			const	wchar_t*l_strType = l_pNamedTypedObject->Type();
			const wchar_t*	l_strName = l_pNamedTypedObject->GetName();
			if( !wcscmp(l_strType,cMPDIList::TypeID) )
			{
				cMPDIList*l_pMPDIList = dynamic_cast<cMPDIList*>(l_pNamedTypedObject);
				ExportMPDI(&l_XMLWriter,l_pMPDIList);
			}
		}
		l_XMLWriter.Close();
	#endif
	}
}