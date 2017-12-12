#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H
//<Particle TPVersion="1.0">
//    <TextureList Name0="Default.png" />
//    <Emiter Name="Glyph" Data="0.001,0,5,10000,772,772,7,0.00,0.00,0.00,1,20.00" Texture="Default">
//        <InitPolicy Type="cPrtLifeInitrSetLife" Data="1.50,1.00,1" />
//        <InitPolicy Type="cPrtColorInitrSetColor" Data="0.00000,0.00000,0.00000,0.00000" />
//        <InitPolicy Type="cPrtSizeInitSetSize" Data="10.00,10.00" />
//        <ActPolicy Type="cPrtLifeActDyingByGameTime" />
//        <ActPolicy Type="cPrtColorActBlendingBy2Color" Data="1.00000,1.00000,1.00000,1.00000,0.00000,0.00000,0.00000,0.00000" />
//    </Emiter>
//    <Emiter Name="Fire" Data="0.001,0,10,100,772,769,7,0.00,-50.00,0.00,1,120.00" Texture="Default">
//        <InitPolicy Type="cPrtLifeInitrSetLife" Data="0.50,1.00,1" />
//        <InitPolicy Type="cPrtColorInitrSetColor" Data="1.00000,0.00000,1.00000,1.00000" />
//        <InitPolicy Type="cPrtSizeInitSetSize" Data="30.00,50.00" />
//        <ActPolicy Type="cPrtLifeActDyingByGameTime" />
//    </Emiter>
//</Particle>
namespace FATMING_CORE
{
	cParticleBase*NameGetParticleType(wchar_t*e_strTypeID);
	class cPaticleManager:public cNamedTypedObjectVector<cPrtEmitter>,public ISAXCallback
	{
		virtual	void	RemoveResourceObject(NamedTypedObject*e_pObject);
		//
		enum	ePRTDataList
		{
			ePRTDL_NONE = 0,
			ePRTDL_TEXTURE,
			ePRTDL_EMITER_DATA,
			ePRTDL_INIT_POLICY_DATA,
			ePRTDL_ACT_POLICY_DATA,
			ePRTDL_MAX,
		};
		ePRTDataList	m_ePRTDataList;
		cPrtEmitter*		m_pCurrentPrtEmitter;
		void	ProcressTexture();
		void	ProcressEmiter();
		void	ProcressInitPolicy();
		void	ProcressActPolicy();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		//
		GETP_SET_DEC(cImageParser,m_ImageParser,GetImageParser,SetImageParser);
	public:
		DEFINE_FILE_EXTENSION_NAME_INFO()
		DEFINE_TYPE_INFO()
		//cCurveManager	m_CurveManager;//now for test
		cPaticleManager();
		virtual ~cPaticleManager();
		cPrtEmitter*		GetEmitter(const char*e_strFileName,const wchar_t*e_strName);
		//if texture is used by emitters,input cPulleImage or cPullaImageUnit or cBaseImage.
		bool	TextureIsInUsing(NamedTypedObject*e_pNamedTypedObject,cPrtEmitter*e_Src);
		void	Export(char*,char*e_strDataAndAuthor = 0);
	};
	//=================
	//behavior collection list
	//=================
	//<-- data and author-->
	//<BehaviorPaticleManager PathFile="" ParticleFile="">
	//	<BehaviorGroup Name="" Count="2">
	//		<cParticleEmiterWithShowPosition Name="" StartTime="0" EndTime="10" Loop="0" PathType="" Path="" EmitterName="" />
	//		<cParticleEmiterWithShowPosition StartTime="0" EndTime="10" Loop="0" PathType="" Path="" EmitterName="" />
	//	<BehaviorGroup/>
	//<BehaviorPaticleManager/>
	class	cParticleEmitterGroup;                          //cParticleEmitterGroup
	class cBehaviorPaticleManager:public cNamedTypedObjectVector<NamedTypedObject>,public ISAXCallback
	{
		//delete not in use prtEmitter
		virtual	void	RemoveResourceObject(NamedTypedObject*e_pObject);
		//
		virtual	void	InternalParse();
		virtual	void	HandleElementData(TiXmlElement*e_pTiXmlElement);
		//prt pi path file..
		void	ProcessRootRelativeFile();
		cParticleEmitterGroup*m_pCurrentParticleEmitterGroup;
		void	ProcessBehaviorGroupType();
		void	ProcressWithTimeBehavior();
	public:
		DEFINE_FILE_EXTENSION_NAME_INFO()
		DEFINE_TYPE_INFO()
		cBehaviorPaticleManager();
		virtual ~cBehaviorPaticleManager();
		cPaticleManager	*m_pPaticleManager;
		cCurveManager	 m_CurveManager;
		void	Export(char*e_strFileName,char*e_strDataAndAuthor = 0);
		cPaticleManager*	GetPaticleManager(){return m_pPaticleManager;}
		cCurveManager*		GetCurveManager(){return &m_CurveManager;}
		//clear all resource
		virtual	void	Destroy();
		
		cParticleEmitterGroup*  GetPRTG(int e_iIndex);
		cParticleEmitterGroup*  GetPRTG(const wchar_t*e_strName);
		cParticleEmitterGroup*  GetPRTG(const char*e_strFileName,const wchar_t*e_strName);
	};
	
	typedef cNamedTypedObjectVector<cBehaviorPaticleManager>  cPrtGList;
//end namespace FATMING_CORE
}
#endif