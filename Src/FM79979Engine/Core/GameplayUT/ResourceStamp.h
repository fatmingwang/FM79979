#ifndef _RESOURCE_STAMP_H_
#define _RESOURCE_STAMP_H_
namespace FATMING_CORE
{
	class	cResourceStamp
	{
		struct	sStampVectorData
		{
			cNamedTypedObjectVector<NamedTypedObject>	m_AnimationParser;
			cNamedTypedObjectVector<NamedTypedObject>	m_ImageParser;
			cNamedTypedObjectVector<NamedTypedObject>	m_SoundParser;
			cNamedTypedObjectVector<NamedTypedObject>	m_PaticleManager;
			cNamedTypedObjectVector<NamedTypedObject>	m_BehaviorPaticleManager;
			cNamedTypedObjectVector<NamedTypedObject>	m_GlyphFontRenderVector;
			cNamedTypedObjectVector<NamedTypedObject>	m_ColladaParserVector;
			cNamedTypedObjectVector<NamedTypedObject>	m_2DImageCollisionDataVector;
			sStampVectorData();
			~sStampVectorData(){}
			void	Clear();
		};
		sStampVectorData	m_Stamp1Data;//old
		sStampVectorData	m_Stamp2Data;//old and new
		sStampVectorData	m_Stamp3Data;//only new
		void				StampResource(sStampVectorData*e_pStampVectorData);
		void				AssignNewData(cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData1,cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData2,cNamedTypedObjectVector<NamedTypedObject>*e_pNewStampVectorData);
	public:
		cResourceStamp();
		~cResourceStamp();
		void				StampResource1();
		void				StampResource2();
		//if u want to keep some new data please remove data at m_Stamp2Data
		void				ReleaseNewResource();
		void				Clear();
	};
//end namespace FATMING_CORE
}
//end _RESOURCE_STAMP_H_
#endif