#ifndef	_FISH_TAG_H_
#define	_FISH_TAG_H_

#include "GameObjectSelector.h"


	class	cFishTag:public cGameItemInfoButton
	{
		Vector3			m_vModelPos;
		cAnimationMesh*m_pFishModel;
	public:
		cFishTag(TiXmlElement*e_pTemplateXml,cAnimationMesh*e_pFishModel,Vector3 e_vModelPosition);
		cFishTag(cFishTag*e_pFishTag);
		CLONE_MYSELF(cFishTag);
		~cFishTag();
		virtual	void			Update(float e_fElpaseTime);
		virtual	void			Render();
		void					SetModel(const WCHAR*e_strModelName);
		void					SetModel(cAnimationMesh*e_pFishModel,Vector3 e_vPos);
		virtual	void			Init();
	};

	//class	cFishTagManager:public cScroller
	//{
	//	//cGameItemInfoButton
	//public:
	//	cFishTagManager();
	//	~cFishTagManager();
 //       //set data to correct
 //       virtual void    Init();
 //       virtual	void    Update(float e_fElpaseTime);
 //       virtual	void    Render();
 //       virtual	void    DebugRender();
	//	//
 //       virtual void    MouseMove(int e_iPosX,int e_iPosY);
 //       virtual void    MouseDown(int e_iPosX,int e_iPosY);
 //       virtual void    MouseUp(int e_iPosX,int e_iPosY);
	//};


#endif