#include "../stdafx.h"
#include "ImageButton.h"
#include "ObjectXMLParser.h"
#include "PhasePopupMessager.h"
#include "FontFrame.h"
#include "GameApp.h"
namespace FATMING_CORE
{
	//<cPhasePopUpMessager Name="Test" RenderLastPhase="1" UpdateLastPhase="1">
	//	<BG>
	//	<some render object...>
	//	</BG>
	//	<MessengerButton Name="" Init="" >
	//		<image butto or cAssembleImageButton>
	//	<MessengerButton/>
	//</cPhasePopUpMessager>


	////MessengerData's count must match ImageButton
	//<cPhasePopUpMessager Name="Test" RenderLastPhase="1" UpdateLastPhase="1">
	//	<BG>
	//	<some render object...>
	//	</BG>
	//<cButtonDialog>
	//</cButtonDialog>
	//<Messenger ButtonName="" NewPhaseName="" InitNewPhase="" DestroyOldPhase=""/>ButtonName will search all
	//<Messenger ButtonName="" NewPhaseName="" InitNewPhase="" DestroyOldPhase=""/>
	//</cPhasePopUpMessager>
	TYPDE_DEFINE_MARCO(cPhasePopUpMessager);
	cPhasePopUpMessager::cPhasePopUpMessager()
	{
		m_pText = 0;
		m_pBG = 0;
		m_bRenderLastPhase = true;
		m_bUpdateLastPhase = false;	
	}

	cPhasePopUpMessager*cPhasePopUpMessager::GetMe(char*e_strFileName)
	{
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(e_strFileName))
		{
			TiXmlElement*l_pRoot = l_cNodeISAX.GetRootElement();
			if( l_pRoot )
			{
				return GetMe(l_pRoot);
			}
		}
		return 0;
	}

	cPhasePopUpMessager*	cPhasePopUpMessager::GetMe(TiXmlElement*e_pTiXmlElement)
	{
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cPhasePopUpMessager::TypeID);
		cPhasePopUpMessager*l_pPhasePopUpMessager = new cPhasePopUpMessager(e_pTiXmlElement);
		return l_pPhasePopUpMessager;
	}
	//<cPhasePopUpMessager Name="HeartNotEnough" RenderLastPhase="1" UpdateLastPhase="1">
	//	<cButtonDialog Description="Test">
	//		<cMPDI cMPDIList="Image/Common.mpdi" cMPDI="GoToStoreDialog"/>
	//	</cButtonDialog>
	//	<Messenger ButtonName="Yes" NewPhaseName="Store" InitNewPhase="1" DestroyOldPhase="0"/>
	//	<Messenger ButtonName="No" NewPhaseName="ChoiceGirlStage" InitNewPhase="0" DestroyOldPhase="0"/>
	//</cPhasePopUpMessager>
	cPhasePopUpMessager::cPhasePopUpMessager(TiXmlElement*e_pTiXmlElement)
	{
		m_pText = 0;
		m_pBG = 0;
		m_bRenderLastPhase = true;
		m_bUpdateLastPhase = false;
		ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,L"cPhasePopUpMessager");
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("Name")
			{
				this->SetName(l_strValue);
			}
			else
			COMPARE_NAME("RenderLastPhase")
			{
				m_bRenderLastPhase = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("UpdateLastPhase")
			{
				m_bUpdateLastPhase = VALUE_TO_BOOLEAN;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		while( e_pTiXmlElement )
		{
			const wchar_t*l_strValue = e_pTiXmlElement->Value();
			COMPARE_VALUE("Messenger")
			{
				ProcessMessengerData(e_pTiXmlElement);
			}
			else
			COMPARE_VALUE("MessengerButton")
			{
				ProcessMessengerButtonData(e_pTiXmlElement);
			}
			else
			COMPARE_VALUE("BG")
			{
				this->m_pBG = GetRenderObjectBehavior(e_pTiXmlElement->FirstChildElement());
				if( m_pBG )
				{
					m_pBG->Init();
				}
			}
			else
			COMPARE_VALUE_WITH_DEFINE(cButtonDialog::TypeID)
			{
				ProcessYesNoDialogData(e_pTiXmlElement);
			}
			else
			COMPARE_VALUE("cFontWithFrame")
			{
				m_pText = cFontWithFrame::GetMe(e_pTiXmlElement);
			}
			e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
		}
	}

	cPhasePopUpMessager::cPhasePopUpMessager(cButtonDialog*e_pYesNoDialog)
	{
		m_pText = 0;
		m_pBG = 0;
		m_bRenderLastPhase = true;
		m_bUpdateLastPhase = false;
		ProcessYesNoDialogData(e_pYesNoDialog);
		//if( m_pBG )
		//	m_pBG = dynamic_cast<cRenderObject*>(e_pYesNoDialog->m_pBG->Clone());
		//iF( m_pText )
		//	m_pText = dynamic_cast<cFontWithFrame*>(e_pYesNoDialog->m_pText->Clone());
	}

	cPhasePopUpMessager::~cPhasePopUpMessager()
	{
		SAFE_DELETE(m_pBG);
		SAFE_DELETE(m_pText);
		SAFE_DELETE(m_pBG);
	}

	void	cPhasePopUpMessager::ProcessYesNoDialogData(cButtonDialog*e_pYesNoDialog)
	{
		//cClickMouseBehaviorVector<cImageButton>*l_pAllButton = e_pYesNoDialog->GetAllButton();
		for( int i=0;i<e_pYesNoDialog->Count();++i )
		{
			cImageButton*l_pYesImageButton = dynamic_cast<cImageButton*>(e_pYesNoDialog->GetObject(i)->Clone());
			this->AddObjectNeglectExist(l_pYesImageButton);		
		}
		//if( e_pYesNoDialog->GetYesImageButton() )
		//{
		//	cImageButton*l_pYesImageButton = dynamic_cast<cImageButton*>(e_pYesNoDialog->GetYesImageButton()->Clone());
		//	this->AddObjectNeglectExist(l_pYesImageButton);
		//}
		//if( e_pYesNoDialog->GetNoImageButton() )
		//{
		//	cImageButton*l_pNoImageButton = dynamic_cast<cImageButton*>(e_pYesNoDialog->GetNoImageButton()->Clone());
		//	this->AddObjectNeglectExist(l_pNoImageButton);
		//}
		cRenderObject*l_pBK = e_pYesNoDialog->GetBG();
		cRenderObject*l_pFont = e_pYesNoDialog->GetFont();
		std::wstring	l_strDescription = e_pYesNoDialog->GetDescription();
		if( l_pBK )
			m_pBG = dynamic_cast<cRenderObject*>(l_pBK->Clone());
		if( l_pFont )
		{
			m_pText = dynamic_cast<cFontWithFrame*>(l_pFont->Clone());
			this->AddChild(m_pText);
		}
		else
		if( l_strDescription.length() >0 )
		{
			cFontWithFrame*l_pFontWithFrame	= dynamic_cast<cFontWithFrame*>(e_pYesNoDialog->GetFont());
			m_pText = new cFontWithFrame(cGameApp::m_spGlyphFontRender,l_strDescription.c_str(),l_pFontWithFrame?l_pFontWithFrame->GetConnectRadianImage():0,l_pFontWithFrame?l_pFontWithFrame->GetLineImage():0);
			this->AddChild(m_pText);
			if( l_pFontWithFrame )
				m_pText->SetLocalTransform(l_pFontWithFrame->GetLocalTransform());
		}
		//e_pYesNoDialog
		//this->AddObject();
	}

	void	cPhasePopUpMessager::ProcessYesNoDialogData(TiXmlElement*e_pTiXmlElement)
	{
		cButtonDialog*l_pYesNoDialog = cButtonDialog::GetMe(e_pTiXmlElement);
		ProcessYesNoDialogData(l_pYesNoDialog);
		SAFE_DELETE(l_pYesNoDialog);
	}

	void	cPhasePopUpMessager::ProcessMessengerData(TiXmlElement*e_pTiXmlElement)
	{
		std::wstring	l_strExternalFunctionName;
		sMessageData	l_MessageData;
		int	l_iCount = this->Count();
		if( l_iCount > (int)this->m_MessageDataVector.size() )
		{
			size_t	l_iOffset = l_iCount-m_MessageDataVector.size();
			for( size_t i=0;i<l_iOffset;++i )
			{
				m_MessageDataVector.push_back(l_MessageData);
			}
		}
		int	l_iIndex = (int)m_MessageDataVector.size()-1;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("ButtonName")
			{
				l_iIndex = this->GetObjectIndexByName(l_strValue);
				if( l_iIndex == -1 )
					UT::ErrorMsg(L"There is no such button name cPhasePopUpMessager::ProcessMessengerData",l_strValue);
			}
			else
			COMPARE_NAME("NewPhaseName")
			{
				l_MessageData.strTargetPhaseName = l_strValue;
			}
			else
			COMPARE_NAME("InitNewPhase")
			{
				l_MessageData.bInitNewPhase = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("DestroyOldPhase")
			{
				l_MessageData.bDestroyOldPhase = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("ExternalFunction")
			{
				l_strExternalFunctionName = l_strValue;
			}
		PARSE_NAME_VALUE_END		
		m_MessageDataVector[l_iIndex] = l_MessageData;
		this->GetObject(l_iIndex)->SetExternalFunctionName(l_strExternalFunctionName);
	}

	void	cPhasePopUpMessager::ProcessMessengerButtonData(TiXmlElement*e_pTiXmlElement)
	{
		sMessageData	l_sMessageData;
		PARSE_ELEMENT_START(e_pTiXmlElement)
			COMPARE_NAME("InitNewPhase")
			{
				l_sMessageData.bInitNewPhase = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("DestroyOldPhase")
			{
				l_sMessageData.bDestroyOldPhase = VALUE_TO_BOOLEAN;
			}
			else
			COMPARE_NAME("NewPhaseName")
			{
				l_sMessageData.strTargetPhaseName = l_strValue;
			}
		PARSE_NAME_VALUE_END
		e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		if(e_pTiXmlElement)
		{
			cRenderObject*l_pRenderObjectBehavior = GetRenderObjectBehavior(e_pTiXmlElement);
			if( l_pRenderObjectBehavior )
			{
				cImageButton*l_pImageButton = dynamic_cast<cImageButton*>(l_pRenderObjectBehavior);
				if( l_pImageButton )
				{
					this->m_MessageDataVector.push_back(l_sMessageData);
					this->AddObject(l_pImageButton);
				}
				else
				{
					UT::ErrorMsg(l_pImageButton->GetName(),L"only support Imagebutton and else");
					SAFE_DELETE(l_pImageButton);
				}
			}
		}

	}

	void	cPhasePopUpMessager::Init()
	{
		assert(m_MessageDataVector.size() == this->Count());
		m_pBG->Init();
		cClickMouseBehaviorVector<cImageButton>::Init();
		//m_strWakePhaseName = L"";
	}

	void    cPhasePopUpMessager::Update(float e_fElpaseTime)
	{
		m_pBG->Update(e_fElpaseTime);
		cClickMouseBehaviorVector<cImageButton>::Update(e_fElpaseTime);
	}

	void    cPhasePopUpMessager::Render()
	{
		m_pBG->Render();
		cClickMouseBehaviorVector<cImageButton>::Render();
	}

	const   wchar_t*	cPhasePopUpMessager::GetNextPhaseName()
	{
		int	l_iIndex = this->GetCurrentWorkingObjectIndex();
		if( l_iIndex != -1 )
		{
			return m_MessageDataVector[l_iIndex].strTargetPhaseName.c_str();
		}
		return 0;
	}

	cPhasePopUpMessager::sMessageData*	cPhasePopUpMessager::GetWorkingMessageData()
	{
		int	l_iIndex = this->GetCurrentWorkingObjectIndex();
		if( l_iIndex != -1 )
		{
			return &m_MessageDataVector[l_iIndex];
		}
		return 0;	
	}

	//void*	cPhasePopUpMessager::GetData()
	//{
	//	int	l_iIndex = this->GetCurrentWorkingObjectIndex();
	//	if( l_iIndex != -1 )
	//		return &m_MessageDataVector[l_iIndex];
	//	return 0;
	//}

	//void	cPhasePopUpMessager::ClearMessengerData()
	//{
	//	m_MessageDataVector.clear();
	//}

	//void	cPhasePopUpMessager::AddMessengerData(const wchar_t*e_strMessenge,bool e_bInitNewPhase,bool e_bDestroyOldPhase)
	//{
	//	sMessageData	l_sMessageData;
	//	l_sMessageData.bDestroyOldPhase = e_bDestroyOldPhase;
	//	l_sMessageData.bInitNewPhase = e_bInitNewPhase;
	//	l_sMessageData.strTargetPhaseName = e_strPhaseName;
	//	m_MessageDataVector.push_back()l_sMessageData;
	//}
//end namespace FATMING_CORE
}