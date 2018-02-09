#ifndef _PHASE_POPUP_MESSENGER_H
#define _PHASE_POPUP_MESSENGER_H

//#include "PhaseControl.h"
#include "Mouse_Touch.h"
#include "ImageButton.h"
namespace FATMING_CORE
{
	class	cButtonDialog;
	class	cFontWithFrame;
	//===========================================================
	//a very simple phase to switch quickly to construction the switch phase
	//
	//===========================================================
	class	cPhaseManager;
	class	cPhasePopUpMessager:public cClickMouseBehaviorVector<cImageButton>
	{
		friend class cPhaseManager;
		struct	sMessageData
		{
			std::wstring	strTargetPhaseName;
			bool			bInitNewPhase;
			bool			bDestroyOldPhase;
			sMessageData(){bInitNewPhase = true;bDestroyOldPhase = true;}
		};
		std::vector<sMessageData>	m_MessageDataVector;//size as many as cClickMouseBehaviorVector<cAssembleImageButton>::Count;
		void						ProcessMessengerButtonData(TiXmlElement*e_pTiXmlElement);
		void						ProcessMessengerData(TiXmlElement*e_pTiXmlElement);
		void						ProcessYesNoDialogData(TiXmlElement*e_pTiXmlElement);
		void						ProcessYesNoDialogData(cButtonDialog*e_pYesNoDialog);
		bool						m_bRenderLastPhase;
		bool						m_bUpdateLastPhase;
		cRenderObject*		m_pBG;
		cFontWithFrame*				m_pText;
public:
		DEFINE_TYPE_INFO()
		cPhasePopUpMessager(TiXmlElement*e_pTiXmlElement);
		cPhasePopUpMessager(cButtonDialog*e_pYesNoDialog);
		cPhasePopUpMessager();
		virtual ~cPhasePopUpMessager();
		virtual	void					Init()override;
		virtual void					Update(float e_fElpaseTime)override;
		virtual void					Render()override;
		virtual void					DebugRender()override { cClickMouseBehaviorVector<cImageButton>::DebugRender(); }
		virtual	const   wchar_t*			GetNextPhaseName()override;
		cRenderObject*			GetBG(){return m_pBG;}
		//const wchar_t*	GetWakePhaseName(){return m_strWakePhaseName.c_str();}
		//void			SetWakePhaseName(const wchar_t*e_strWakrPhaseName){m_strWakePhaseName = e_strWakrPhaseName;}
		//virtual	void*	GetData();
		sMessageData*	GetWorkingMessageData();
		//for advance use,u have to know as well as button index.
		//void			ClearMessengerData();
		//void			AddMessengerData(const wchar_t*e_strPhaseName,bool e_bInitNewPhase,bool e_bDestroyOldPhase);
		static cPhasePopUpMessager*	GetMe(TiXmlElement*e_pTiXmlElement);
		static cPhasePopUpMessager*	GetMe(char*e_strFileName);

	};
//end namespace
}
#endif