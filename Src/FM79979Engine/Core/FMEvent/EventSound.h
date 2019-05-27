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
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY)override;
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY)override;
        virtual void    InternalInit()override;
		virtual void    InternalUpdate(float e_fElpaseTime)override;
		//std::wstring	m_strSoundFileName;
		bool			m_bPlayed;
    public:
        DEFINE_TYPE_INFO();
		cEventSound(TiXmlElement*e_pElement);
        cEventSound(cBasicSound*e_pBasicSound);
        cEventSound(cEventSound*e_pEventBook);
        virtual ~cEventSound();
		EVENT_CLONE_DEFINE(cEventSound);
        virtual void				Destroy()override;
		virtual NamedTypedObject*   GetResourcePointer()override;
		virtual	TiXmlElement*		ToTiXmlElement()override;
    };
}
//end namespace
#endif