#pragma once
#include "ClickBehavior.h"
#include "../../MPDI/MPDI.h"
namespace FATMING_CORE
{
	//c++ type cast multi-inherent
	//https://stackoverflow.com/questions/7417189/c-multiple-inheritance-casting
	//this for auto delete render object and click object
	//do not call AddDefaultRenderClickBehaviorButton!(because render won't be deleted).
	class cMPDIToGameObject :public cClickBehaviorGroup
	{
	protected:
		cMPDI*						m_pBGMPDI;
		cMPDI*						m_pClonedMPDI;
		cRenderObject*				m_pRenderObject;
		virtual void				AddClickObject(cClickBehaviorGroup*e_pClickBehaviorGroup);
		virtual void				AddRenderObject(cRenderObject*e_pRenderObject);
		//
		using						cClickBehaviorGroup::AddDefaultRenderClickBehaviorButton;
	public:
		DEFINE_TYPE_INFO();
		cMPDIToGameObject();
		cMPDIToGameObject(cMPDI*e_pMPDI, std::vector<std::wstring>e_strButtonName, bool e_bKeepMPDIAsBG);
		virtual ~cMPDIToGameObject();
		//
		virtual bool	SetupMPDIAndButtonNameVector(cMPDI*e_pMPDI, std::vector<std::wstring>e_strButtonName, bool e_bKeepMPDIAsBG);
		//cScrollBox
		virtual void	AddObject(cMPDIToGameObject*e_pcMPDIToGameObject);
		virtual bool	CreateRenderObject();
		//ensure e_pRenderObject can be deleted or AddChild will occur unexpected problem.
		virtual cClickBehavior*	AddRenderObjectandGenerateClickBehavior(cRenderObject*e_pRenderObject, bool e_bEnableClickScale = true);
		bool	EnableBGCollide(bool e_bFullScreen);
		//need parent to call init!?
		void	DisConnectFromParent();
		virtual void	Init()override;
		void	Render();
		//disable button and visable is false
		void	SetHide(bool e_bHide);
		virtual void	Update(float e_fElpaseTime) override;
		virtual void	Destroy() override;
		cRenderObject*	GetRenderObject() { return m_pRenderObject; }
		//for visible update(ScrollerBox)
		bool			GenerateRectBoundForRenderObject(Vector4 e_vDrawRect);
		cClickBehavior*	AssignButtonClickFunction(const wchar_t*e_strButtonName, ClickFunction e_ClickFunction, cBasicSound*e_pSound);

		//change position of parent
		//void	InscreasePirority(int e_iPiority);
		//void	DecreasePirority(int e_iPiority);
	};

	//
	//class cRenderClickableObject:public cRenderObject,public cClickBehavior
	//{
	//public:
	//	cRenderClickableObject();
	//	~cRenderClickableObject();
	//};
//end namespace FATMING_CORE
}