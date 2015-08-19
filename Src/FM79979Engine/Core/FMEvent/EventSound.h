#ifndef _EVENT_SOUND_H_
#define _EVENT_SOUND_H_

namespace   FATMING_CORE
{
    //currently I am confuse how to active sound play
    //moseUp to play?where is collision object!?
    class   cEventSound:public cEventObject
    {
		//bool			m_bResourceKeep;
        //==================================    
        GET_SET_DEC(cBasicSound*,m_pBasicSound,GetBasicSound,SetBasicSound);
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
        virtual void    InternalInit();
		virtual void    InternalUpdate(float e_fElpaseTime);
		//std::wstring	m_strSoundFileName;
		bool			m_bPlayed;
    public:
        DEFINE_TYPE_INFO();
		cEventSound(TiXmlElement*e_pElement);
        cEventSound(cBasicSound*e_pBasicSound);
        cEventSound(cEventSound*e_pEventBook);
        virtual ~cEventSound();
		EVENT_CLONE_DEFINE(cEventSound);
        virtual void				Destroy();        
		virtual NamedTypedObject*   GetResourcePointer();
		virtual	TiXmlElement*		ToTiXmlElement();
    };
}
//end namespace
#endif