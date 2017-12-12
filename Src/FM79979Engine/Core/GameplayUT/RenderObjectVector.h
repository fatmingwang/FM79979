#ifndef	_RENDER_OBJECT_VECTOR_
#define	_RENDER_OBJECT_VECTOR_
namespace FATMING_CORE
{
	class	cRenderObjectVector:public cBehaviorObjectList<cRenderObject>,public Frame
	{
		virtual	void			AddObjectNotification(cRenderObject* e_t){ if(e_t)this->AddChild(e_t,false); }
	public:
		cRenderObjectVector();
		cRenderObjectVector(cRenderObjectVector*e_pFontFrameVector);
		virtual ~cRenderObjectVector();
		DEFINE_TYPE_INFO()
		CLONE_MYSELF(cRenderObjectVector);
		static cRenderObjectVector*GetMe(TiXmlElement*e_pElement);
		void			AddData(wchar_t*e_strText,cGlyphFontRender*e_pGlyphFontRender,cSubMPDI*e_pPos);
		void			AddData(wchar_t*e_strText,cGlyphFontRender*e_pGlyphFontRender,TiXmlElement*e_pSubMPDIElement);
		void			ChangeData(int e_iIndex,const wchar_t*e_strText);
		//void			Init();
		void			Update(float e_fElpaseTime){cBehaviorObjectList<cRenderObject>::Update(e_fElpaseTime);}
		void			Render(){cBehaviorObjectList<cRenderObject>::Render();}
	};
//
}
#endif