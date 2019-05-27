#ifndef _EVENT_IMAGE_H_
#define _EVENT_IMAGE_H_

namespace   FATMING_CORE
{
    class   cEventImage:public cEventObject
    {
        //==================================    
        cBaseImage*m_pImage;//it's possible could be PIUnit
		virtual void    InternalRender()override;
    public:
        DEFINE_TYPE_INFO();
        cEventImage(Vector3 e_vPos,Vector2 e_vSize,cBaseImage*e_pImage);
        cEventImage(cEventImage*e_pEvent_PIUnit);
		cEventImage(TiXmlElement*e_pElement);
        virtual ~cEventImage(){}        
		EVENT_CLONE_DEFINE(cEventImage);
        cBaseImage*					GetImage(){return m_pImage;}
		virtual NamedTypedObject*   GetResourcePointer()override;
		virtual	TiXmlElement*		ToTiXmlElement()override;
    };
}
//end namespace
#endif