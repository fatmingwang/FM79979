#include "ParticleBase.h"
#include "PrtColor.h"
#include "PrtLife.h"
#include "PrtRotate.h"
#include "PrtSize.h"
#include "PrtStartPosition.h"
#include "PrtVelocity.h"
#include "PrtTexture.h"
#include "ParticleEmitter.h"
#include "ParticleManager.h"
#include "ParticleEmitterBehavior.h"
#include "ParticleEmitterGroup.h"
#include "../../GameApp/GameApp.h"
#ifdef WIN32
#include "../../../XML/AtgXmlWriter.h"
#endif
#define	PRTDATA_INIT_POLICY 	L"InitPolicy"
#define	PRTDATA_ACT_POLICY 		L"ActPolicy"
#define	PRTDATA_TEXTURE_LIST 	L"TextureList"
#define	PRTDATA_EMITER			L"Emiter"

namespace FATMING_CORE
{
	const char*cPaticleManager::ExtensionNameID( ".prt" );
	const char*cBehaviorPaticleManager::ExtensionNameID( ".prtg" );

	TYPDE_DEFINE_MARCO(cPaticleManager);
	TYPDE_DEFINE_MARCO(cBehaviorPaticleManager);

	cParticleBase*NameGetParticleType(const wchar_t*e_strTypeID)
	{
		cParticleBase*l_pParticleBase = 0;
		if( !wcscmp(e_strTypeID,cPrtColorInitrSetColor::TypeID) )
		{
			l_pParticleBase = new cPrtColorInitrSetColor();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtColorInitrSetRandomColor::TypeID) )
		{
			l_pParticleBase = new cPrtColorInitrSetRandomColor();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtColorActBlending::TypeID) )
		{
			l_pParticleBase = new cPrtColorActBlending();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtColorActBlendingByLife::TypeID) )
		{
			l_pParticleBase = new cPrtColorActBlendingByLife();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtRotateActRotate::TypeID) )
		{
			l_pParticleBase = new cPrtRotateActRotate();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtSizeInitSetSize::TypeID) )
		{
			l_pParticleBase = new cPrtSizeInitSetSize();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtSizeActBlending::TypeID) )
		{
			l_pParticleBase = new cPrtSizeActBlending();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtStartPositionInitBySquareRange::TypeID) )
		{
			l_pParticleBase = new cPrtStartPositionInitBySquareRange();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtVelocityActDircctionChange::TypeID) )
		{
			l_pParticleBase = new cPrtVelocityActDircctionChange();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtStartPositionInitByFrame::TypeID) )
		{
			//cPrtStartPositionInitByFrame*l_pPrtStartPositionInitByFrame = (cPrtStartPositionInitByFrame*)e_pParticleBase;
#ifdef	WIN32
			UT::ErrorMsg(L"not support for now",L"error");
#else
			printf("not support for now");
#endif
			//l_pUserControl = gcnew StartPositionInitByFrame();
		}
		//else
		//if( !wcscmp(e_strTypeID,cPrtVelocityInitRandomFly::TypeID) )
		//{
		//	l_pParticleBase = new cPrtVelocityInitRandomFly();
		//}
		else
		if( !wcscmp(e_strTypeID,cPrtVelocityInitSetVelocity::TypeID ))
		{
			l_pParticleBase = new cPrtVelocityInitSetVelocity();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtVelocityActAcceleration::TypeID) )
		{
			l_pParticleBase = new cPrtVelocityActAcceleration();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtVelocityActBySatelliteAction::TypeID) )
		{
			//l_pParticleBase = new cPrtVelocityActBySatelliteAction();
#ifdef	WIN32
			UT::ErrorMsg(L"not support for now",L"warning");
#else
			printf("not support for now");
#endif
		}
		else
		if( !wcscmp(e_strTypeID,cPrtLifeInitrSetLife::TypeID ))
		{
			l_pParticleBase = new cPrtLifeInitrSetLife();
		}
		else
		if( !wcscmp(e_strTypeID,cPrtLifeActDyingByGameTime::TypeID ))
		{
			l_pParticleBase = new cPrtLifeActDyingByGameTime;
		}
		else
		if( !wcscmp(e_strTypeID,cPrtColorActBlendingBy2Color::TypeID ))
		{
			l_pParticleBase = new cPrtColorActBlendingBy2Color;
		}
		else
		if( !wcscmp(e_strTypeID,cPrtRotateInitRotate::TypeID ))
		{
			l_pParticleBase = new cPrtRotateInitRotate;
		}
		else
		if( !wcscmp(e_strTypeID,cPrtTextureActDynamicTexture::TypeID ))
		{
			l_pParticleBase = new cPrtTextureActDynamicTexture;
		}
		else
		{
			assert(0&&"do not support this type");
		}
		if( l_pParticleBase )
			l_pParticleBase->SetName(e_strTypeID);
		return l_pParticleBase;
	}

	cPaticleManager::cPaticleManager()
	{
		m_ePRTDataList = ePRTDL_NONE;
		m_pCurrentPrtEmitter = 0;
		this->m_bSetRunBack = true;
		m_ImageParser.SetName(L"cPaticleManager->m_ImageParser");
	}

	cPaticleManager::~cPaticleManager()
	{
		this->Destroy();
		m_ImageParser.Destroy();
	}

	cPrtEmitter*  cPaticleManager::GetEmitter(const char*e_strFileName,const wchar_t*e_strName)
	{
		cPrtEmitter*l_pEmitter = this->GetObject(e_strName);
		if( !l_pEmitter )
		{
			bool	l_b = this->Parse(e_strFileName);
			if( !l_b )
			{
				if(this->m_bShowErrorMsg)
					UT::ErrorMsg(e_strFileName,"parse failed");
				return 0;
			}
			l_pEmitter = this->GetObject(e_strName);
		}
		return l_pEmitter;
	}

	NamedTypedObject* cPaticleManager::GetObjectByFileName(const char*e_strFileName)
	{
		auto l_strObjectName = UT::GetFileNameWithoutFullPath(e_strFileName);
		auto l_strWFileName = ValueToStringW(l_strObjectName.c_str());
		return GetEmitter(e_strFileName, l_strWFileName.c_str());
	}


	void	cPaticleManager::RemoveResourceObject(NamedTypedObject*e_pObject)
	{
		return;
	}
	//
	bool	cPaticleManager::TextureIsInUsing(NamedTypedObject*e_pNamedTypedObject,cPrtEmitter*e_Src)
	{
		int i = this->Count();
		i--;
		while(i)
		{
			if( e_pNamedTypedObject->Type() == cPuzzleImageUnit::TypeID )//only could kill parent not child
				return true;
			cPrtEmitter*l_pPrtEmitter = (*this)[i];
			if( e_Src != l_pPrtEmitter )
			{
				if( l_pPrtEmitter->GetBaseImage()->Type() == cPuzzleImageUnit::TypeID )
				{
					cPuzzleImageUnit*l_pPuzzleImageUnit = (cPuzzleImageUnit*)l_pPrtEmitter->GetBaseImage();
					if(l_pPuzzleImageUnit->GetParent() == e_pNamedTypedObject)
						return true;
				}
				else
				if(l_pPrtEmitter->GetBaseImage() == e_pNamedTypedObject)
				{
					return true;
				}
			}
			i--;
		}
		return false;
	}

	void	cPaticleManager::Export(const char*e_pString,char*e_strDataAndAuthor)
	{
		assert(e_pString);
#if defined(WIN32) && !defined(UWP)
		ATG::XMLWriter*     l_pXMLWriter = new ATG::XMLWriter( e_pString );
		//for time and user name
		if( e_strDataAndAuthor )
		{
			l_pXMLWriter->WriteComment(e_strDataAndAuthor);
		}
		l_pXMLWriter->StartElement( "Particle" );
		l_pXMLWriter->AddAttribute( "TPVersion", "1.0" );
	//====================================================for Pic		
			l_pXMLWriter->StartElement(PRTDATA_TEXTURE_LIST);
			char	l_tempForValue[MAX_PATH];
			wchar_t	l_strTexName[MAX_PATH];
			for(int i=0;i<this->m_ImageParser.Count();++i)
			{
				if( m_ImageParser[i]->Type() == cPuzzleImage::TypeID )
				{
					sprintf(l_tempForValue,"Name%d\0",i);
					swprintf(l_strTexName,MAX_PATH,L"%ls.pi\0",m_ImageParser[i]->GetName());
					l_pXMLWriter->AddAttribute(l_tempForValue,l_strTexName);
				}
				else
				{
					sprintf(l_tempForValue,"Name%d\0",i);
					swprintf(l_strTexName,MAX_PATH,L"%ls.png\0",m_ImageParser[i]->GetName());
					l_pXMLWriter->AddAttribute(l_tempForValue,l_strTexName);
				}
			}
			l_pXMLWriter->EndElement();
	//====================================================end TextureList
	//====================================================for EmiterDataListStart
			for(int i=0;i<this->Count();++i)
			{
				cPrtEmitter*l_pPrtEmitter = (*this)[i];
				l_pXMLWriter->StartElement( PRTDATA_EMITER );
				l_pXMLWriter->AddAttribute("Name",l_pPrtEmitter->GetName());
				l_pXMLWriter->AddAttribute("Data",l_pPrtEmitter->GetDataInfo());
				if( l_pPrtEmitter->GetBaseImage() )
				{
					if( l_pPrtEmitter->GetBaseImage()->Type() == cPuzzleImageUnit::TypeID )
					{
						cPuzzleImageUnit*l_pPIUnit = (cPuzzleImageUnit*)l_pPrtEmitter->GetBaseImage();
						l_pXMLWriter->AddAttribute("PIName",l_pPIUnit->GetParent()->GetName());
					}
					l_pXMLWriter->AddAttribute("Texture",l_pPrtEmitter->GetBaseImage()->GetName());
				}
				cQuickUpdateParticleObjectListByName*l_pInitPolicyParticleList = l_pPrtEmitter->GetInitPolicyParticleList();
				cParticleBase*l_pParticleBase = 0;
					for(int j=0;j<l_pInitPolicyParticleList->Count();++j)
					{
						l_pXMLWriter->StartElement( PRTDATA_INIT_POLICY );
						l_pParticleBase = (*l_pInitPolicyParticleList)[j];
						l_pXMLWriter->AddAttribute("Type",l_pParticleBase->Type());
						auto l_strData = l_pParticleBase->GetDataInfo();
						if (l_strData.length())
						{
							l_pXMLWriter->AddAttribute("Data", l_pParticleBase->GetOutputDataString()); 
						}
						l_pXMLWriter->EndElement();
					}
					cQuickUpdateParticleObjectListByName*l_pActPolicyParticleList = l_pPrtEmitter->GetActPolicyParticleList();
					for(int j=0;j<l_pActPolicyParticleList->Count();++j)
					{
						l_pXMLWriter->StartElement( PRTDATA_ACT_POLICY );
						l_pParticleBase = (*l_pActPolicyParticleList)[j];
						l_pXMLWriter->AddAttribute("Type",l_pParticleBase->Type());
						auto l_strData = l_pParticleBase->GetDataInfo();
						if (l_strData.length())
						{
							l_pXMLWriter->AddAttribute("Data", l_pParticleBase->GetOutputDataString());
						}
						l_pXMLWriter->EndElement();
					}
				l_pXMLWriter->EndElement();
			}
	//====================================================end PolicyDataList
		l_pXMLWriter->EndElement();//end fat
		l_pXMLWriter->Close();
		delete l_pXMLWriter;
		l_pXMLWriter = nullptr;
#endif
	}

	void	cPaticleManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*l_strName = e_pTiXmlElement->Value();
		COMPARE_NAME_WITH_DEFINE(PRTDATA_EMITER)
		{
			ProcressEmiter();
		}
		else
		COMPARE_NAME_WITH_DEFINE(PRTDATA_INIT_POLICY)
		{
			ProcressInitPolicy();
		}
		else
		COMPARE_NAME_WITH_DEFINE(PRTDATA_TEXTURE_LIST)
		{
			ProcressTexture();
		}
		else
		COMPARE_NAME_WITH_DEFINE(PRTDATA_ACT_POLICY)
		{
			ProcressActPolicy();
		}
		else
		{
		
		}
	}

	void	cPaticleManager::ProcressTexture()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			PARSE_CURRENT_ELEMENT_START
			std::wstring	l_strTexName = UT::GetFileNameWithoutFullPath((wchar_t*)l_strValue);
			NamedTypedObject*l_pSimpleGLTexture = this->m_ImageParser.GetObject(l_strTexName.c_str());
			if( !l_pSimpleGLTexture )
			{
				char	l_strFullPath[MAX_PATH];
				sprintf(l_strFullPath,"%s%s",m_strCurrentDirectory,UT::WcharToChar(l_strValue).c_str());
				std::wstring	l_strExtensionaName = UT::GetFileExtensionName((wchar_t*)l_strValue).c_str();
				if( !wcscmp(l_strExtensionaName.c_str(),L"pi") )
				{
					//2 Image Parser will occur cPuzzleImage memory leak...because the resource is sahare...
					//so add into GameApp::ImageParser then clone one.
					assert(cGameApp::m_spImageParser&&"please new cGameApp");
					auto*l_pPI = cGameApp::GetPuzzleImageByFileName(ValueToStringW(l_strFullPath));
					if (l_pPI)
					{
						auto*l_pClone = l_pPI->Clone();
						m_ImageParser.AddObject(l_pClone);
					}
					//bool	l_b = m_ImageParser.Parse(l_strFullPath);
					//if( !l_b )
					//{
					//	UT::ErrorMsg(l_strFullPath,"pi parse failed!");
					//}
				}
				else
				{
					l_pSimpleGLTexture = new cBaseImage(l_strFullPath);
					m_ImageParser.AddObject(l_pSimpleGLTexture);
				}
			}
			TO_NEXT_VALUE
		}
	}

	void	cPaticleManager::ProcressEmiter()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			cPrtEmitter*l_pPrtEmitter = 0;
			cPuzzleImage*l_pPI = 0;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					if( this->GetObject(l_strValue) )
					{
#ifdef _WIN32
#ifdef DEBUG
						FMLog::LogWithFlag(l_strValue, CORE_LOG_FLAG);
						FMLog::LogWithFlag(L" already exist skip parse\n", CORE_LOG_FLAG);
#endif
#endif
						return;
					}
					l_pPrtEmitter = new cPrtEmitter(l_strValue);
					m_pCurrentPrtEmitter = l_pPrtEmitter;
				}
				else
				COMPARE_NAME("Data")
				{
					std::string	l_str = UT::WcharToChar(l_strValue);
					l_pPrtEmitter->SetDataByDataString((char*)l_str.c_str());
				}
				else
				COMPARE_NAME("PIName")
				{
					l_pPI = dynamic_cast<cPuzzleImage*>(this->m_ImageParser.GetObject(l_strValue));
				}
				else
				COMPARE_NAME("Texture")
				{
					if( !l_pPI )
					{
						NamedTypedObject*l_pNamedTypedObject = this->m_ImageParser.GetObject(l_strValue);
						const wchar_t*l_strType = l_pNamedTypedObject->Type();
						if( l_strType == cBaseImage::TypeID )
						{
							l_pPrtEmitter->SetBaseImage(dynamic_cast<cBaseImage*>(l_pNamedTypedObject));
						}
						else
						if( l_strType == cPuzzleImage::TypeID )
						{
							cPuzzleImage*l_pPI2 = dynamic_cast<cPuzzleImage*>(l_pNamedTypedObject);
							l_pPrtEmitter->SetBaseImage(l_pPI2);
						}
						else
						{
							UT::ErrorMsg(L"unsupport format",l_strType);
						}
					}
					else
					{
						l_pPrtEmitter->SetBaseImage(l_pPI->GetObject(l_strValue));
					}
				}
			PARSE_NAME_VALUE_END
			this->AddObjectNeglectExist(l_pPrtEmitter);
		}
		else
		{
			if( !m_pCurrentPrtEmitter )
				return;
			cQuickUpdateParticleObjectListByName*l_pInit = m_pCurrentPrtEmitter->GetInitPolicyParticleList();
			l_pInit->GenerateListBuffer();
			cQuickUpdateParticleObjectListByName*l_pAct = m_pCurrentPrtEmitter->GetActPolicyParticleList();
			l_pAct->GenerateListBuffer();
			m_pCurrentPrtEmitter = 0;
		}
	}

	void	cPaticleManager::ProcressInitPolicy()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{//might exists so skip it
			if( !m_pCurrentPrtEmitter )
				return;
			cParticleBase*l_pParticleBase = 0;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Type")
				{
					l_pParticleBase = NameGetParticleType((wchar_t*)l_strValue);
					l_pParticleBase->SetEmitterOwner(m_pCurrentPrtEmitter);
				}
				else
				COMPARE_NAME("Data")
				{
					std::string	l_str = UT::WcharToChar(l_strValue);
					l_pParticleBase->SetDataByDataString((char*)l_str.c_str());
				}
			PARSE_NAME_VALUE_END
			m_pCurrentPrtEmitter->AddInitPolicy(l_pParticleBase);
		}
		else
		{
		
		}
	}

	void	cPaticleManager::ProcressActPolicy()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			//might exists so skip it
			if( !m_pCurrentPrtEmitter )
				return;
			cParticleBase*l_pParticleBase = 0;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Type")
				{
					l_pParticleBase = NameGetParticleType((wchar_t*)l_strValue);
					if( l_pParticleBase )
						l_pParticleBase->SetEmitterOwner(m_pCurrentPrtEmitter);
				}
				else
				COMPARE_NAME("Data")
				{
					if( l_pParticleBase )
					{
						std::string	l_str = UT::WcharToChar(l_strValue);
						l_pParticleBase->SetDataByDataString((char*)l_str.c_str());
					}
				}
			PARSE_NAME_VALUE_END
			if( l_pParticleBase )
			m_pCurrentPrtEmitter->AddActPolicy(l_pParticleBase);
		}
		else
		{
		
		}
	}
//==============================
//
//==============================
	cBehaviorPaticleManager::cBehaviorPaticleManager():ISAXCallback(true)
	{
		m_pPaticleManager = new cPaticleManager();
		m_pPaticleManager->SetOwner(this);
		m_pCurrentParticleEmitterGroup = 0;
	}

	cBehaviorPaticleManager::~cBehaviorPaticleManager()
	{
		Destroy();
	}
	//step 1,find all prtEmitter
	//step 2,check prtEmitter is in use or not,if in use remove it from list
	//step 3,delete all rest prtEmitter from m_pPaticleManager.
	void	cBehaviorPaticleManager::RemoveResourceObject(NamedTypedObject*e_pObject)
	{
		cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(e_pObject);
		cNamedTypedObjectVector<cPrtEmitter>	l_PrtEmitterList;
		l_PrtEmitterList.SetFromResource(true);
		int	l_iCount = l_pParticleEmitterGroup->Count();
		for( int i=0;i<l_iCount;++i )
		{
			int	l_iCount2 = l_pParticleEmitterGroup->Count();
			for( int j=0;j<l_iCount2;++j )
			{
				cPrtEmitter*l_pPrtEmitter = l_pParticleEmitterGroup->GetObject(j)->GetPrtEmitter();
				if(l_pPrtEmitter)
				{
					l_PrtEmitterList.AddObjectNeglectExist(l_pPrtEmitter);
				}
			}
		}
		l_iCount = this->Count();
		for( int i=0;i<l_iCount;++i )
		{
			cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(this->m_ObjectList[i]);
			if( e_pObject == l_pParticleEmitterGroup )
				continue;
			int	l_iCount2 = l_pParticleEmitterGroup->Count();
			for( int j=0;j<l_iCount2;++j )
			{
				//if input is in orther object keep PrtEmitter alive
				cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = l_pParticleEmitterGroup->GetObject(j);
				if( l_pParticleEmiterWithShowPosition->GetPrtEmitter() )
				{
					if(l_PrtEmitterList.GetObjectByUniqueID( l_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetUniqueID() ))
						l_PrtEmitterList.RemoveObjectWithoutDelete(l_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetName());
				}
			}
		}
		//now rest prtEmitter is not in use delete it
		l_iCount = l_PrtEmitterList.Count();
		for( int i=0;i<l_iCount;++i )
			this->m_pPaticleManager->RemoveObject(l_PrtEmitterList.GetObject(i)->GetName());
	}

	void	cBehaviorPaticleManager::InternalParse()
	{
		Destroy();
		m_pPaticleManager = new cPaticleManager();
	}

	void	cBehaviorPaticleManager::HandleElementData(TiXmlElement*e_pTiXmlElement)
	{
		const wchar_t*	l_strValue = e_pTiXmlElement->Value();
		COMPARE_VALUE("BehaviorPaticleManager")
		{
			ProcessRootRelativeFile();
		}
		else
		COMPARE_VALUE_WITH_DEFINE(cParticleEmitterGroup::TypeID)
		{
			ProcessBehaviorGroupType();
		}
		else
		COMPARE_VALUE_WITH_DEFINE(cParticleEmiterWithShowPosition::TypeID)
		{
			ProcressWithTimeBehavior();
		}
	}
	//<BehaviorPaticleManager PathFile="" ParticleFile="">
	void	cBehaviorPaticleManager::ProcessRootRelativeFile()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			this->m_CurveManager.Destroy();
			SAFE_DELETE(m_pPaticleManager);
			m_pPaticleManager = new cPaticleManager();
			string	l_strFileName;
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("PathFile")
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					bool	l_b = m_CurveManager.Parse((char*)l_strFileName.c_str());
					if( !l_b )
						UT::ErrorMsg((wchar_t*)l_strValue,L" path file not found!");
				}
				else
				COMPARE_NAME("ParticleFile")
				{
					l_strFileName = this->m_strCurrentDirectory;
					l_strFileName += UT::WcharToChar(l_strValue);
					bool	l_b = m_pPaticleManager->Parse((char*)l_strFileName.c_str());
					if( !l_b )
						UT::ErrorMsg((wchar_t*)l_strValue,L" particle file not found!");
				}
			PARSE_NAME_VALUE_END
		}
		else
		{
		
		}
	}
	//	<BehaviorGroup Name="" Count="2">
	void	cBehaviorPaticleManager::ProcessBehaviorGroupType()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			cParticleEmitterGroup*l_pParticleEmitterGroup = new cParticleEmitterGroup();
			this->m_pCurrentParticleEmitterGroup = l_pParticleEmitterGroup;
			//COMPARE_ELEMENT_VALUE(cParticleEmitterGroup::TypeID)
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					l_pParticleEmitterGroup->SetName(l_strValue);
				}
				else
				COMPARE_NAME("Count")
				{
					l_pParticleEmitterGroup->GetList()->reserve(VALUE_TO_INT);
				}
				else
				COMPARE_NAME("TotalPlayTime")
				{
					float	l_fTotalPlayTime = VALUE_TO_FLOAT;
					if(l_fTotalPlayTime<0.f)
					{//avoid my fucking stupid mistake to resoluve for now
						l_fTotalPlayTime = 0.0000001f;
					}
					l_pParticleEmitterGroup->SetTotalPlayTime(l_fTotalPlayTime,0.f);
				}
				else
				COMPARE_NAME("Loop")
				{
					l_pParticleEmitterGroup->SetAnimationLoop(VALUE_TO_BOOLEAN);
				}
			PARSE_NAME_VALUE_END
		}
		else
		{
			bool	l_b = this->AddObject(m_pCurrentParticleEmitterGroup);
			assert(l_b);
			m_pCurrentParticleEmitterGroup = 0;
		}
	}
	//		<cParticleEmiterWithShowPosition Name="1" StartTime="0" EndTime="10" Loop="0" PathType="" Path="" EmitterName="" />
	void	cBehaviorPaticleManager::ProcressWithTimeBehavior()
	{
		if(!this->m_pCurrentTiXmlElement->m_bDone)
		{
			cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = 0;//new cParticleEmiterWithShowPosition();
			PARSE_CURRENT_ELEMENT_START
				COMPARE_NAME("Name")
				{
					l_pParticleEmiterWithShowPosition = new cParticleEmiterWithShowPosition((wchar_t*)l_strValue);
				}
				else
				COMPARE_NAME("StartTime")
				{
					l_pParticleEmiterWithShowPosition->SetStartTime(VALUE_TO_FLOAT);
				}
				else
				COMPARE_NAME("EndTime")
				{
					l_pParticleEmiterWithShowPosition->SetEndTimeCounter(VALUE_TO_FLOAT);
				}
				else

				COMPARE_NAME("AnimationLoop")
				{
					l_pParticleEmiterWithShowPosition->SetAnimationLoop(VALUE_TO_BOOLEAN);
				}
				else
				COMPARE_NAME("PathType")
				{
					l_pParticleEmiterWithShowPosition->SetPathType(cParticleEmiterWithShowPosition::ePathType(VALUE_TO_INT));
				}
				else
				COMPARE_NAME("Path")
				{
					cCurveWithTime*l_pCurveWithTime = this->m_CurveManager.GetObject(l_strValue);
					if( !l_pCurveWithTime )
					{
						UT::ErrorMsg((wchar_t*)l_strValue,L"path not found");
					}
					l_pParticleEmiterWithShowPosition->SetCurveWithTime(l_pCurveWithTime);
				}
				else
				COMPARE_NAME("EmitterPosition")
				{
					l_pParticleEmiterWithShowPosition->SetEmitterPos(VALUE_TO_VECTOR3);
				}
				else
				COMPARE_NAME("PathAngle")
				{
					l_pParticleEmiterWithShowPosition->SetCurveAngle(VALUE_TO_VECTOR3);
				}
				else
				COMPARE_NAME("EmitterName")
				{
					cPrtEmitter*l_pSrcPrtEmitter = this->m_pPaticleManager->GetObject(l_strValue);
					assert(l_pSrcPrtEmitter);
					cPrtEmitter*l_pPrtEmitter = new cPrtEmitter(l_pSrcPrtEmitter);
					l_pParticleEmiterWithShowPosition->SetPrtEmitter(l_pPrtEmitter);
				}
				else
				COMPARE_NAME("EmitterDirection")
				{
					l_pParticleEmiterWithShowPosition->SetEmitDirection(VALUE_TO_VECTOR3);
				}
			PARSE_NAME_VALUE_END
			l_pParticleEmiterWithShowPosition->SetEndTimeCounter(l_pParticleEmiterWithShowPosition->GetEndTimeCounter()->fTargetTime);
			bool	l_b = m_pCurrentParticleEmitterGroup->AddObject(l_pParticleEmiterWithShowPosition);
			assert(l_b);
		}
		else
		{
		
		}
	}
	NamedTypedObject* cBehaviorPaticleManager::GetObjectByFileName(const char*e_strFileName)
	{
		auto l_strObjectName = UT::GetFileNameWithoutFullPath(e_strFileName);
		auto l_strWFileName = ValueToStringW(l_strObjectName.c_str());
		return GetPRTG(e_strFileName, l_strWFileName.c_str());
	}
	cParticleEmitterGroup*  cBehaviorPaticleManager::GetPRTG(int e_iIndex){ return dynamic_cast<cParticleEmitterGroup*>(this->m_ObjectList[e_iIndex]); }
	cParticleEmitterGroup*  cBehaviorPaticleManager::GetPRTG(const wchar_t*e_strName){ return dynamic_cast<cParticleEmitterGroup*>(this->GetObject(e_strName)); }
	cParticleEmitterGroup*  cBehaviorPaticleManager::GetPRTG(const char*e_strFileName,const wchar_t*e_strName)
	{
		cParticleEmitterGroup*l_pPRTG = this->GetPRTG(e_strName);
		if( !l_pPRTG )
		{
			bool	l_b = this->Parse(e_strFileName);
			if( !l_b )
			{
				if(this->m_bShowErrorMsg)
					UT::ErrorMsg(e_strFileName,"parse failed");
				return 0;
			}
			l_pPRTG = this->GetPRTG(e_strName);
		}
		return l_pPRTG;
	}	

	void	cBehaviorPaticleManager::Destroy()
	{
		cNamedTypedObjectVector<NamedTypedObject>::Destroy();
		this->m_CurveManager.Destroy();
		SAFE_DELETE(this->m_pPaticleManager);
	}

	void	cBehaviorPaticleManager::Export(const char*e_strFileName,char*e_strDataAndAuthor)
	{
#if defined(WIN32) && !defined(UWP)
		if(!this->Count())
			return;
		std::string	l_strDirectory = UT::GetDirectoryWithoutFileName(e_strFileName);
		std::string	l_strFileName = UT::GetFileNameWithoutFullPath(e_strFileName);
		std::string	l_strFullFileName = l_strDirectory+l_strFileName;
		ATG::XMLWriter*     l_pXMLWriter = new ATG::XMLWriter(e_strFileName);
		//for time and user name
		if( e_strDataAndAuthor )
		{
			l_pXMLWriter->WriteComment(e_strDataAndAuthor);
		}
		l_pXMLWriter->StartElement("BehaviorPaticleManager");
		//curve
		if( m_CurveManager.Count() )
		{
			std::string	l_strFinalSaveFileName = l_strFileName+m_CurveManager.ExtensionName();
			l_pXMLWriter->AddAttribute("PathFile",l_strFinalSaveFileName.c_str());
			l_strFinalSaveFileName = l_strFullFileName+m_CurveManager.ExtensionName();
			m_CurveManager.ExportData((char*)l_strFinalSaveFileName.c_str(),0,e_strDataAndAuthor);
		}
		//particle
		{
			std::string	l_strFinalSaveFileName = l_strFileName+m_pPaticleManager->ExtensionName();
			l_pXMLWriter->AddAttribute("ParticleFile",l_strFinalSaveFileName.c_str());
			l_strFinalSaveFileName = l_strFullFileName+m_pPaticleManager->ExtensionName();
			m_pPaticleManager->Export((char*)l_strFinalSaveFileName.c_str(),e_strDataAndAuthor);
		}
		for( int i=0;i<Count();++i )
		{
			NamedTypedObject* l_pNamedTypedObject = (*this)[i];
			const	wchar_t*l_strType = l_pNamedTypedObject->Type();
			if( cParticleEmitterGroup::TypeID == l_strType )
			{
				const wchar_t*	l_strName = l_pNamedTypedObject->GetName();
				cParticleEmitterGroup*l_pParticleEmitterGroup = dynamic_cast<cParticleEmitterGroup*>(l_pNamedTypedObject);
				l_pXMLWriter->StartElement(cParticleEmitterGroup::TypeID);
				l_pXMLWriter->AddAttribute("Name",l_pParticleEmitterGroup->GetName());
				l_pXMLWriter->AddAttribute("Count",l_pParticleEmitterGroup->Count());
				l_pParticleEmitterGroup->RefreshTotalPlayTime();
				l_pXMLWriter->AddAttribute("TotalPlayTime",l_pParticleEmitterGroup->GetEndTime());
				if( l_pParticleEmitterGroup->IsAnimationLoop() )
					l_pXMLWriter->AddAttribute("Loop",l_pParticleEmitterGroup->IsAnimationLoop()?1:0);
				for(int j=0;j<l_pParticleEmitterGroup->Count();++j)
				{
					NamedTypedObject* l_pNamedTypedObject2 = (*l_pParticleEmitterGroup)[j];
					if( !wcscmp(l_pNamedTypedObject2->Type(),cParticleEmiterWithShowPosition::TypeID) )
					{
						//<cParticleEmiterWithShowPosition StartTime="0" EndTime="10" Loop="0" PathType="" Path="" EmitterName="" />
						cParticleEmiterWithShowPosition*l_pParticleEmiterWithShowPosition = dynamic_cast<cParticleEmiterWithShowPosition*>(l_pNamedTypedObject2);
						l_pXMLWriter->StartElement(cParticleEmiterWithShowPosition::TypeID);
						l_pXMLWriter->AddAttribute("Name",l_pParticleEmiterWithShowPosition->GetName());
						l_pXMLWriter->AddAttribute("PathType",(int)l_pParticleEmiterWithShowPosition->GetPathType());
						if( l_pParticleEmiterWithShowPosition->GetPathType() != cParticleEmiterWithShowPosition::ePT_NO_PATH )
						{
							l_pXMLWriter->AddAttribute("Path",l_pParticleEmiterWithShowPosition->GetCurveWithTime()->GetName());
							l_pXMLWriter->AddAttribute("PathAngle",ValueToString(*l_pParticleEmiterWithShowPosition->GetCurveRotationAngle()));
						}
						l_pXMLWriter->AddAttribute("EmitterName",l_pParticleEmiterWithShowPosition->GetPrtEmitter()->GetName());
						l_pXMLWriter->AddAttribute("EmitterDirection",ValueToString(*l_pParticleEmiterWithShowPosition->GetEmitDirection()));
						l_pXMLWriter->AddAttribute("EmitterPosition",ValueToString(*l_pParticleEmiterWithShowPosition->GetEmitterPos()));
						l_pXMLWriter->AddAttribute("StartTime",l_pParticleEmiterWithShowPosition->GetStartTime());
						l_pXMLWriter->AddAttribute("EndTime",l_pParticleEmiterWithShowPosition->GetEndTimeCounter()->fTargetTime);
						l_pXMLWriter->AddAttribute("AnimationLoop",l_pParticleEmiterWithShowPosition->IsAnimationLoop()?1:0);
						//end cParticleEmiterWithShowPosition
						l_pXMLWriter->EndElement();
					}
					else
					{
						assert(0&&"what type is it!?void	cBehaviorPaticleManager::Export(char*e_strFileName,char*e_strDataAndAuthor)");
					}
				}
				//end cParticleEmitterGroup
				l_pXMLWriter->EndElement();
			}
			else
			{
				assert(0&&"what type is it!?void	cBehaviorPaticleManager::Export(char*e_strFileName,char*e_strDataAndAuthor)");
			}
		}
		//end
		l_pXMLWriter->EndElement();
		SAFE_DELETE(l_pXMLWriter);
	#endif	
	}
//end namespace FATMING_CORE
}