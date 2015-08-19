#ifndef _EVENT_PACKAGE_RUNNER_H_
#define _EVENT_PACKAGE_RUNNER_H_

#include "EventObjectInstance.h"

namespace   FATMING_CORE
{
    //this is a pacjage
    class   cEventPackage:public cDoAllMouseBehaviorList<cEventObjectInstance>
    {
        Vector4	m_vBGColor;
        Vector3 m_vPagePos;
		bool	m_bRenderBGColor;
    public:
        //std::wstring    m_strNeighborPageNames[eFMBSPD_MAX];
        cEventPackage();
		cEventPackage(cEventPackage*e_pEventPackage);
        virtual ~cEventPackage(){}
        //while page is turned,turn back again someparameter must reset
        virtual void    Init();
        //while page is leave
        virtual void    Destroy();
        virtual void    Update(float e_fElpaseTime);
		virtual void    Render();
		Vector4	GetBGColor(){ return m_vBGColor; }
		void	SetBGColor(Vector4 e_vColor);
        virtual void    SetRelativePosition( Vector3 e_vPos );
        Vector3    GetRelativePosition(){ return m_vPagePos; }
        //bool    ContainData()
		TiXmlElement*	ToTiXmlElement();
    };

	class	cEventManager;
	//
	//all working pages
	//
	class	cEventPackageRunner:public cSimplePhase
	{
		cEventManager*			m_pEventManager;
		cEventPackage*			m_pCurrentEventPackage;
		//remove the object do note overlap,to save memory
		void					RemoveNotOverlapObject(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage);
		//fetch from variable manager
		std::wstring			m_strNewPageName;
		void					RemoveUnUsingEventObjectStatusAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage);
		void					RemoveUnUsingEventInstanceAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage);
		void					RemoveUnUsingResourceAtEventManager(cEventPackage*e_pOldEventPackage,cEventPackage*e_pNewEventPackage);
		//for sound or anything want to change ,now for sound
		void					StatusResourceDataChangeWhilePackageChange(NamedTypedObject*e_pNamedTypedObject);
	public:
		cEventPackageRunner(cEventManager*e_pEventManager);
		virtual ~cEventPackageRunner();
		virtual	void			Init();
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
        virtual void    		MouseMove(int e_iPosX,int e_iPosY);
        virtual void    		MouseDown(int e_iPosX,int e_iPosY);
        virtual void    		MouseUp(int e_iPosX,int e_iPosY);

		bool					LoadPackage(const WCHAR*e_strPackageName);
		//parse initial packagename in the variable manager's event variable
		bool					LoadPackage();
		cEventPackage*			GetCurrentEventPackage();
		//most time we set EventPackage by parse xml,but in editor we might need this to add something
		void					SetCurrentEventPackage(cEventPackage*e_pEventPackage);
		virtual	const WCHAR*	GetNextPhaseName();
	};
}
#endif