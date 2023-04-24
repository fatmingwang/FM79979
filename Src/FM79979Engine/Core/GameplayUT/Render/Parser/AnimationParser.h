#pragma once

#include "../../../XML/XMLLoader.h"
//#include "SimplePrimitive.h"
#include <vector>
#include <map>
#include "../MPDI/MPDI.h"
#include "ImageParser.h"

namespace FATMING_CORE
{
	enum	eImagePlayerTypeList
	{
		eIPTL_COMMON = 0,
		eIPTL_MPDI,
		eIPTL_MAX,
	};

	//<ImagePlayer Name="2266" AnimationCount="1" ImageName="79979.png">
	//    <Animation Name="1" PlayCount="0" AnimationCount="18" MaxImageWidth="559" MaxImageHeight="363">
	//        <AnimationData Angle="0.000000" ShowSec="0.100000" ImageIndex="0" OffsetPos="88,124" Size="471,239" />
	//        <AnimationData Angle="0.000000" ShowSec="0.100000" ImageIndex="1" OffsetPos="0,0" Size="559,363" />
	//    </Animation>
	//</ImagePlayer>

	//eIPTL_MPDI
	//<MPDIList Name="" Count="" ImageName="79979.png" CameraView="i am matrix" />
		//<MultiPathDynamicImage Name="79979" PathCount="1"	Playcount="1" TotalPlayTime="" />
			//<CueToStartCurvesWithTime Name="" StartTime="1" Count="3" LOD="1" />
				//<CurvePoint  P1="0,0" P2="10,20" P3="20,20"  />
				//<CurveTime   P1="0.5" P2="0.5" P3="0.5"/>
				//<PointDataList P1="0,0,0,0,0,0,0,0" P2="0,0,0,0,0,0,0,0" P3="0,0,0,0,0,0,0,0">
			//<cCueToStartCurveWithTime/>
		//<MultiPathDynamicImage/>
	//<cMPDIList/>

	//===============
	//warning here !!!!
	//whole animation object's image store in m_pCurrentImagePlayer.
	//if would like to remove any object here ,be sure the bse image have to delete also,here do not rewrite any
	//function inherit from cNamedTypedObjectVector,like remove object replace and any else.
	//the base image object will be remove untill manager life cycle is end.
	//there is no any guarantee for multi thread!!!
	//
	//while u parse 2files and them using same textures.
	//ensure u kill them continuance,or texture resource will occor problem,because they share same texture resource.
	//===============
	//#define	AP_IMAGE_PLAYER	"ImagePlayer"
	//#define	AP_MPDI_LIST	"MPDIList"
	//#define	AP_VERSION		"Version"
	//MPDIList
	class	cAnimationParser:public cNamedTypedObjectVector<NamedTypedObject>,public cNodeISAX
	{
		virtual	void						RemoveResourceObject(NamedTypedObject*e_pObject)override;
		//the data has changed so mark this for version check,set it as 0.f while parse is called!
		float								m_fMPDIVersion;
		virtual	void						InternalParse()override { m_fMPDIVersion = 0.f; }
		//internal using
		cMPDIList*							m_pCurrentcMPDIList;
		cMultiPathDynamicImage*				m_pCurrentMultiPathDynamicImage;
		cCueToStartCurveWithTime*			m_pCurrentCueToStartCurvesWithTime;

		//for MPDI
		//old
		void								ProcessMPDI();
		//
		void								ProcessMPDIList(TiXmlElement* e_pTiXmlElement);
		void								ProcessMultiPathDynamicImage(TiXmlElement*e_pTiXmlElement);
		void								ProcessCueToStartCurvesWithTime(TiXmlElement* e_pTiXmlElement);
		void								ProcessPointDataList(TiXmlElement* e_pTiXmlElement);
		void								ProcessHintPoint(TiXmlElement* e_pTiXmlElement);
		//eImagePlayerTypeList				m_eImagePlayerTypeList;
		//whole imageplayer's base image are store in this vector,if u like to release memory pressure,u could call DeleteBaseImage before remove player
		cImageParser						m_AllBaseImageList;
		//bloew function can't be call directly instead call by RemoveObject!,or it will crash or ensure what u are doing.
		void								DeleteBaseImage(NamedTypedObject*e_pNamedTypedObject);
		//
		void								DeleteSoundObject(NamedTypedObject*e_pNamedTypedObject);
		//
		virtual	bool						MyParse(TiXmlElement* e_pRoot);
	public:
		DEFINE_TYPE_INFO()
		DEFINE_FILE_EXTENSION_NAME_INFO()
		cAnimationParser();
		virtual ~cAnimationParser();
		//if we want to remove pi but want to check how many object is using it call this function to check
		int									NumObjectUsingPI(cPuzzleImage*e_pPI);
		//if u want to get puzzle data
		cImageParser*						GetAllBaseImageList(){ return &m_AllBaseImageList; }
		//
		void								Export(const char*e_strFileName,bool e_bBinary = false);

		cMPDIList*GetMPDIList(const wchar_t*e_strName){ return dynamic_cast<cMPDIList*>(GetObject(e_strName)); }
		cMPDIList*GetMPDIListByFileName(const wchar_t*e_strFileName,bool e_bShowErrorMessage = false);
		cMPDIList*GetMPDIListByFileName(const char*e_strFileName,bool e_bShowErrorMessage = false);
		virtual NamedTypedObject* GetObjectByFileName(const char*e_strFileName)override { return GetMPDIListByFileName(e_strFileName); }
		cMPDIList*GetMPDIList(const char*e_strFileName,const wchar_t*e_strName,bool e_bShowErrorMessage = false);
		cMPDIList*GetMPDIList(int e_iIndex){ return dynamic_cast<cMPDIList*>(GetObject(e_iIndex)); }
	};
}