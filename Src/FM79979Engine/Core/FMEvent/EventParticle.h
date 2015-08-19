#ifndef _EVENT_PARTICLE_H_
#define _EVENT_PARTICLE_H_

namespace   FATMING_CORE
{
    //this resourc will retun 0 because I am lazy to calculate relative resource in use or not
    class   cEventParticle:public cEventObject
    {
        //==================================    
        Vector3 m_vParticlePos;
		//its owner might be cPaticleManager,and cBehaviorPaticleManager might from cBehaviorPaticleManager
		//so get owner to find toppest owned
        GET_SET_DEC(cPrtEmitter*,m_pPrtEmitter,GetPrtEmitter,SetPrtEmitter);
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
        virtual void    InternalUpdate(float e_fElpaseTime);
		virtual void    InternalRender();
        virtual void    InternalInit();
    public:
        DEFINE_TYPE_INFO();
        cEventParticle(Vector3 e_vPos,cPrtEmitter*e_pPrtEmitter);
        cEventParticle(cEventParticle*e_pEventParticle);
		cEventParticle(TiXmlElement*e_pElement);
        virtual ~cEventParticle();
		EVENT_CLONE_DEFINE(cEventParticle);
        virtual void			Destroy();
		virtual NamedTypedObject*           GetResourcePointer();
		virtual	TiXmlElement*				ToTiXmlElement();
    };

	//this resourc will retun 0 because I am lazy to calculate relative resource in use or not
    class   cEventParticleGroup:public cEventObject
    {
        //==================================    
        GET_SET_DEC(cParticleEmitterGroup*,m_pParticleEmitterGroup,GetParticleEmitterGroup,SetParticleEmitterGroup);
        virtual void    InternalMouseUp(int e_iPosX,int e_iPosY);
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
		virtual void    InternalRender();
		std::wstring	m_strFileName;
    public:
        DEFINE_TYPE_INFO();
        cEventParticleGroup(const WCHAR*e_strFileName,Vector3 e_vPos,cParticleEmitterGroup*e_pParticleEmitterGroup);
        cEventParticleGroup(cEventParticleGroup*e_pEventParticleGroup);
		cEventParticleGroup(TiXmlElement*e_pElement);
        virtual ~cEventParticleGroup();
        virtual cEventObject*		Clone(){ return new cEventParticleGroup(this); }
        virtual void				Init();
        virtual void				PageLeave();
        virtual void				Update(float e_fElpaseTime);
		virtual NamedTypedObject*   GetResourcePointer();
		virtual	TiXmlElement*		ToTiXmlElement();
    };

    
}
//end namespace
#endif