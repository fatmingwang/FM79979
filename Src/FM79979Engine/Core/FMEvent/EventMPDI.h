#ifndef _EVENT_ANIMATION_H_
#define _EVENT_ANIMATION_H_

namespace   FATMING_CORE
{
    class   cEventMPDI:public cEventObject
    {
        //==================================
        cMultiPathDynamicImage*m_pMPDI;
        GETP_SET_DEC(bool,m_bClickToPlay,IsClickToPlay,SetClickToPlay);
        bool    m_bMouseClicked;
		virtual	void	InternalInit();
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
        virtual void    InternalUpdate(float e_fElpaseTime);
        virtual void    InternalRender();
    public:
        DEFINE_TYPE_INFO();
        cEventMPDI(Vector3 e_vPos,cMultiPathDynamicImage*e_pMPDI,bool e_bClickToPlay,float e_fScale = 1.f,bool e_bLoop = false);
        cEventMPDI(cEventMPDI*e_pEvent_MPDI);
		cEventMPDI(TiXmlElement*e_pTiXmlElement);
		EVENT_CLONE_DEFINE(cEventMPDI);
        virtual ~cEventMPDI();
        cMultiPathDynamicImage*		GetMPDI(){return m_pMPDI;}
		//scale is not support....because I am lazy.
		virtual void    			DebugRender();
		virtual NamedTypedObject*   GetResourcePointer();
		virtual	TiXmlElement*		ToTiXmlElement();
		virtual	bool	IsPlayDone();
    };
}
//end namespace
#endif