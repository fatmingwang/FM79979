#ifndef PHASE_CONTROL_H
#define PHASE_CONTROL_H
#include "Mouse_Touch.h"
#include "PhasePopupMessager.h"
namespace FATMING_CORE
{
//=====================
//here is the phase control
//we perhap have more than one phase in the game.(game over ,game intro,and so on)
//the phase have some common function,we always call update and render if we need more extract data,we have get it external,like mouse position or else
//
//before cSimplePhase update must call SetCondition!!the condition could be a pointer object or else,we could use this condition to determine what we need to do
//ex: when stage clear,we could using condition to determine what we need to draw
//
//while we created a lot phase we wanted,we could put whole phase into cPhaseManager,
//the manager will change pahse,while phase was satisified the condition,
//
//we only need to add object by cPhaseManager,and cPhaseManager will control everything,unless game is over will leave the whold game
//
//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
//=====================
	class	cPhaseManager;
	//
	class	cSimplePhase:virtual public cClickMouseBehavior
	{
	protected:
		std::wstring	m_strNextPhaseName;
		//if u do not allow m_bSatisfiedCondition to be false again,set it as true in the Init function if u wanted.
		bool			m_bSatisfiedCondition;
		GET_SET_DEC(bool,m_bKillAfterSatisfiedCondition,IsKillAfterSatisfiedCondition,SetKillAfterSatisfiedCondition);
		virtual bool    InternalCollide(int e_iPosX,int e_iPosY){ return true; }
	public:
		//DEFINE_TYPE_INFO()
		cSimplePhase(){ m_bSatisfiedCondition = false; m_bKillAfterSatisfiedCondition = false;  }
		virtual ~cSimplePhase(){}
		//virtual	void				Start(float e_fElpaseTime = EPSIONAL){}
		virtual	void	Init() = 0;//it would be called if the stage is changed,set m_bSatisfiedCondition as false!?
		virtual	void	Update(float e_fElpaseTime) = 0;
		virtual	void	Render() = 0;
		virtual	void	Destroy(){}
		virtual	void	DebugRender(){}
		virtual	void	KeyUp(char e_cKey){};
		virtual	void	KeyDown(char e_cKey){};
		virtual	void	KeyPress(char e_cKey){};
		//not necessary,some times we need to read data by multithread to prevent screen empty
		//must call after SetCondition via SetCondition we could know what the data is needed
		virtual	void	ReadDataByMultiThread(){};
		virtual	const   wchar_t*	GetNextPhaseName(){ return m_strNextPhaseName.c_str(); }
		void			SetNextPhaseName(const wchar_t* e_strPhaseName) { m_strNextPhaseName = e_strPhaseName; }
		//if the condition is full,sometimes we need to know the data we want use in this phase,ex: the menu final select object index or else...
		//u could release memory here if u ensure this will called while phase condition if filled.
		virtual	void*	GetData(){return 0;}
		//get the data from previous phase
		virtual	void	FetchData(const wchar_t*e_strPhaseName,void*e_pData){}
		virtual bool	IsSatisfiedCondition(){ return m_bSatisfiedCondition; }
		void			SetSatisfiedCondition(bool e_b){ m_bSatisfiedCondition = e_b;}
		cPhaseManager*	GetPhaseManager();
	};
//====================
//
//====================
	class	cPhasePopUpMessager;
	class	cPhaseManager: public cClickMouseBehaviorVector<cSimplePhase>,public cSimplePhase
	{
		cClickMouseBehaviorVector<cPhasePopUpMessager>*	m_pPhasePopUpMessagerVector;
		cPhasePopUpMessager*							m_pCurrentPopUpMessager;
		
		GET_SET_DEC(int,m_iCurrentPhase,GetCurrentPhase,SetCurrentPhase);
		//for internal use if u have default render or update override it,ex background image
		virtual	void			InternalRender(){}
		virtual	void			InternalUpdate(float e_fElpaseTime){}
		virtual bool			InternalCollide(int e_iPosX,int e_iPosY)override { return false; }
		void					SetNewWorkingPhase(int e_iNewPhaseIndex,int e_iOldPhaseIndex,bool e_bDestroyOldPhase = false,bool e_bInitNewPhase = true,bool e_bFetchData = true);
	protected:
		bool					PopUpMessagerUpdate(float e_fElpaseTime);
		bool					PopUpMessagerRender();
	public:
		cPhaseManager();
		virtual ~cPhaseManager();
		cPhaseManager(cPhaseManager*e_pPhaseManager);
		CLONE_MYSELF(cPhaseManager);
		const	wchar_t*			GetCurrentPhaseName();
		//char*	GetCurrentPhaseNameByChar();
		bool					SetCurrentCurrentPhase(const wchar_t*e_strPhaseNAme);
		virtual	bool			IsSatisfiedCondition()override { return this->m_iCurrentPhase == -1; }
		virtual void			SetSatisfiedCondition(bool e_b)override {}
		virtual	void			Init()override { cClickMouseBehaviorVector<cSimplePhase>::Init(); }
		virtual	void			Update(float e_fElpaseTime)override;
		virtual	void			Render()override;
		virtual	void			Destroy()override;
		virtual	void			DebugRender()override;
		virtual	void			MouseDown(int e_iX,int e_iY)override;
		virtual	void			MouseUp(int e_iX,int e_iY)override;
		virtual	void			MouseMove(int e_iX,int e_iY)override;
		virtual	void			KeyUp(char e_cKey)override;
		virtual	void			KeyDown(char e_cKey)override;
		virtual	void			KeyPress(char e_cKey)override;
		//
		virtual	const   wchar_t*	GetNextPhaseName()override { return 0;}
		virtual void			SingnalProcess(unsigned char e_usKey)override { cClickMouseBehaviorVector<cSimplePhase>::SingnalProcess(e_usKey); }
		virtual void			SingnalProcess()override { cClickMouseBehaviorVector<cSimplePhase>::SingnalProcess(); }
		virtual void			SetRelativePosition( Vector3 e_vPos )override {}
		bool					WakePopupMessenger(const wchar_t*e_strName,const wchar_t*e_strPhaseNameName);
		void					RegisterPopupMessenger(char*e_strFileName);
		cPhasePopUpMessager*	RegisterPopupMessenger(TiXmlElement*e_pTiXmlElement);
		cPhasePopUpMessager*	GetPopupMessenger(wchar_t*e_strName){ return m_pPhasePopUpMessagerVector->GetObject(e_strName); }
	};
//for quickly copy

//class	cGamePhase:public cSimplePhase
//{
//public:
//	DEFINE_TYPE_INFO()
//	cPhase();
//	virtual	void	Update(float e_fElpaseTime);
//	virtual	void	Init();
//	virtual	void	Render();
//	virtual	void	DebugRender();
//	virtual	void	MouseDown(int e_iX,int e_iY);
//	virtual	void	MouseUp(int e_iX,int e_iY);
//	virtual	void	MouseMove(int e_iX,int e_iY);
//	virtual	void	KeyUP(char e_cKey);
//	virtual	wchar_t*	GetNextPhaseName();	
//};

//cGamePhase::cGamePhase()
//{
//
//}
//
//void	cGamePhase::Update(float e_fElpaseTime)
//{
//
//}
//
//void	cGamePhase::Init()
//{
//
//}
//
//void	cGamePhase::Render()
//{
//
//}
//
//void	cGamePhase::DebugRender()
//{
//
//}
//
//void	cGamePhase::MouseDown(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::MouseMove(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::MouseUp(int e_iX,int e_iY)
//{
//
//}
//
//void	cGamePhase::KeyUP(char e_cKey)
//{
//
//}
//
//void*	cGamePhase::GetData()
//{
//	return this;
//}
//
//void	cGamePhase::FetchData(const wchar_t*e_strPhaseName,void*e_pData)
//{
//
//}
//end namespace
}

#endif