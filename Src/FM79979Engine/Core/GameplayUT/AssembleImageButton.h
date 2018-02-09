#ifndef _ASSEMBLE_IMAGE_H_
#define _ASSEMBLE_IMAGE_H_
#include "../AllImageInclude.h"
#include "ImageButton.h"

	//multiple image but a button.
	class	cAssembleImageButton:public cBehaviorObjectList<cRenderObject>,public cImageButton
	{
		virtual void			SetTransformInternalData()override;
		virtual	void			AddObjectNotification(cRenderObject* e_t)override { if(e_t)this->AddChild(e_t); }
		virtual	void			RemoveObjectNotification(cRenderObject* e_t)override { if(e_t)e_t->SetParent(0); }
	public:
		DEFINE_TYPE_INFO()
		cAssembleImageButton();
		virtual ~cAssembleImageButton();
		cAssembleImageButton(cAssembleImageButton*e_pAssembleImageButton);
		CLONE_MYSELF(cAssembleImageButton);
		static	cAssembleImageButton*	GetMe(TiXmlElement*e_pElement);
		virtual void					Init()override;
		virtual	void					SetColor(Vector4 e_vColor)override;
		virtual	void					Update(float e_fElpaseTime)override;
		virtual	void					Render()override;
		virtual void					DebugRender()override;
	};

#endif