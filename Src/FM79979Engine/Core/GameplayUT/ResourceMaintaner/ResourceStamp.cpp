#include "ResourceStamp.h"
#include "../../Sound/SoundManager.h"
#include "../Render/CommonRender/BaseImage.h"
#include "../Render/MPDI/MPDI.h"
#include "../Render/Parser/AnimationParser.h"
#include "../Render/Particle/AllParticleInclude.h"
#include "../../Physic/2DImageCollisionData.h"
#include "../GameApp/GameApp.h"

///#include "GameApp.h"
namespace FATMING_CORE
{
	cResourceStamp::sStampVectorData::sStampVectorData()
	{
		m_AnimationParser.SetFromResource(true);
		m_ImageParser.SetFromResource(true);;
		m_SoundParser.SetFromResource(true);;
		m_PaticleManager.SetFromResource(true);;
		m_BehaviorPaticleManager.SetFromResource(true);;
		m_GlyphFontRenderVector.SetFromResource(true);
	}

	void	cResourceStamp::sStampVectorData::Clear()
	{
		m_AnimationParser.Destroy();
		m_ImageParser.Destroy();
		m_SoundParser.Destroy();
		m_PaticleManager.Destroy();
		m_BehaviorPaticleManager.Destroy();	
		m_GlyphFontRenderVector.Destroy();
	}

	cResourceStamp::cResourceStamp()
	{
	
	}

	cResourceStamp::~cResourceStamp()
	{
	
	}

	void	cResourceStamp::AssignNewData(cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData1,cNamedTypedObjectVector<NamedTypedObject>*e_pStampVectorData2,cNamedTypedObjectVector<NamedTypedObject>*e_pNewStampVectorData)
	{
		int	l_iOldCount = e_pStampVectorData1->Count();
		int	l_iCount = e_pStampVectorData2->Count();
		for( int i=l_iOldCount;i<l_iCount;++i )
			e_pNewStampVectorData->AddObjectNeglectExist(e_pStampVectorData2->GetObject(i));
	}
	//class	cParticleBase;
	void	cResourceStamp::StampResource(sStampVectorData*e_pStampVectorData)
	{
		e_pStampVectorData->Clear();
		e_pStampVectorData->m_AnimationParser.CopyListPointer(cGameApp::m_spAnimationParser);
		e_pStampVectorData->m_ImageParser.CopyListPointer(cGameApp::m_spImageParser);
		e_pStampVectorData->m_SoundParser.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spSoundParser);
		e_pStampVectorData->m_PaticleManager.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spPaticleManager);
		e_pStampVectorData->m_BehaviorPaticleManager.CopyListPointer(cGameApp::m_spBehaviorPaticleManager);
		e_pStampVectorData->m_GlyphFontRenderVector.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spGlyphFontRenderVector);
		if( cGameApp::m_spColladaParserVector )
			e_pStampVectorData->m_ColladaParserVector.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_spColladaParserVector);
		e_pStampVectorData->m_2DImageCollisionDataVector.CopyListPointer((cNamedTypedObjectVector<NamedTypedObject>*)cGameApp::m_sp2DImageCollisionDataVector);
	}

	void	cResourceStamp::StampResource1()
	{
		StampResource(&m_Stamp1Data);
	}

	void	cResourceStamp::StampResource2()
	{
		StampResource(&m_Stamp2Data);
	}

	void	cResourceStamp::ReleaseNewResource()
	{
		AssignNewData(&m_Stamp1Data.m_AnimationParser,				&m_Stamp2Data.m_AnimationParser,		&m_Stamp3Data.m_AnimationParser);
		AssignNewData(&m_Stamp1Data.m_BehaviorPaticleManager,		&m_Stamp2Data.m_BehaviorPaticleManager,	&m_Stamp3Data.m_BehaviorPaticleManager);
		AssignNewData(&m_Stamp1Data.m_ImageParser,					&m_Stamp2Data.m_ImageParser,			&m_Stamp3Data.m_ImageParser);
		AssignNewData(&m_Stamp1Data.m_PaticleManager,				&m_Stamp2Data.m_PaticleManager,			&m_Stamp3Data.m_PaticleManager);
		AssignNewData(&m_Stamp1Data.m_SoundParser,					&m_Stamp2Data.m_SoundParser,			&m_Stamp3Data.m_SoundParser);
		AssignNewData(&m_Stamp1Data.m_GlyphFontRenderVector,		&m_Stamp2Data.m_GlyphFontRenderVector,	&m_Stamp3Data.m_GlyphFontRenderVector);
		AssignNewData(&m_Stamp1Data.m_ColladaParserVector,			&m_Stamp2Data.m_ColladaParserVector,	&m_Stamp3Data.m_ColladaParserVector);
		AssignNewData(&m_Stamp1Data.m_2DImageCollisionDataVector,	&m_Stamp2Data.m_2DImageCollisionDataVector,	&m_Stamp3Data.m_2DImageCollisionDataVector);

		for(int i=0;i<m_Stamp3Data.m_AnimationParser.Count();++i)
			cGameApp::m_spAnimationParser->RemoveObject(m_Stamp3Data.m_AnimationParser[i]);
		for(int i=0;i<m_Stamp3Data.m_BehaviorPaticleManager.Count();++i)
			cGameApp::m_spBehaviorPaticleManager->RemoveObject(m_Stamp3Data.m_BehaviorPaticleManager[i]);
		for(int i=0;i<m_Stamp3Data.m_ImageParser.Count();++i)
			cGameApp::m_spImageParser->RemoveObject(m_Stamp3Data.m_ImageParser[i]);
		for(int i=0;i<m_Stamp3Data.m_PaticleManager.Count();++i)
			cGameApp::m_spPaticleManager->RemoveObject(m_Stamp3Data.m_PaticleManager[i]);
		for(int i=0;i<m_Stamp3Data.m_SoundParser.Count();++i)
			cGameApp::m_spSoundParser->RemoveObject(m_Stamp3Data.m_SoundParser[i]);
		for(int i=0;i<m_Stamp3Data.m_GlyphFontRenderVector.Count();++i)
			cGameApp::m_spGlyphFontRenderVector->RemoveObject(m_Stamp3Data.m_GlyphFontRenderVector[i]);
		for(int i=0;i<m_Stamp3Data.m_ColladaParserVector.Count();++i)
			cGameApp::m_spColladaParserVector->RemoveObject(m_Stamp3Data.m_ColladaParserVector[i]);
		for(int i=0;i<m_Stamp3Data.m_2DImageCollisionDataVector.Count();++i)
			cGameApp::m_sp2DImageCollisionDataVector->RemoveObject(m_Stamp3Data.m_2DImageCollisionDataVector[i]);
		Clear();
	}
	void	cResourceStamp::Clear()
	{
		m_Stamp1Data.Clear();
		m_Stamp2Data.Clear();
		m_Stamp3Data.Clear();
	}
//end namespace FATMING_CORE
}