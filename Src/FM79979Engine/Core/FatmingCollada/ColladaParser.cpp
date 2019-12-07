#include "StdAfx.h"
#include "ColladaParser.h"
#include "AnimationMesh.h"
std::vector<std::string>	*g_strParsingDataList = 0;

cColladaParser::cColladaParser()
{
	m_pAllAnimationMesh = 0;
	m_pAllMesh = 0;
	memset(m_bXMLParseFlag,1,sizeof(bool)*eCMT_MAX);
}
cColladaParser::~cColladaParser()
{
	SAFE_DELETE(m_pAllMesh);
	SAFE_DELETE(m_pAllAnimationMesh);
}

void	cColladaParser::ClearData()
{
	m_LibraryGeometries.ClearData();
	//cLibraryImages		m_LibraryImages;
	//cLibraryEffects		m_LibraryEffects;
	//cLibraryMaterials	m_LibraryMaterials;
	//cLibraryVisualScenes	m_LibraryVisualScenes;
	m_LibraryControllers.ClearData();
	m_LibraryAnimation.ClearData();
}

bool	cColladaParser::ParseDataIntoXMLNode(const char*e_strFileName,bool e_bStroeInfo)
{
#ifdef DEBUG
	std::string	l_strDebugInfo = e_strFileName;
	l_strDebugInfo += " start parse!";
	cGameApp::OutputDebugInfoString(l_strDebugInfo);
#endif
	this->SetName(e_strFileName);
	bool	l_bErrorMsgExists = true;
	if( !FMLog::g_spstrLogErrorMsgString )
	{
		l_bErrorMsgExists = false;
		try
		{
			if( !FMLog::g_spstrLogErrorMsgString )
				FMLog::g_spstrLogErrorMsgString = new std::wstring;
			if( FMLog::g_spstrLogErrorMsgString == nullptr )
			{
				int a=0;
			}
		}
		catch(const std::exception& ex)
		{
			const char*l_str= ex.what();
			int a=0;
		}
	}
	int l_iShowErrorMsgBoxType = FMLog::g_siLogShowErrorType == 1?true:false;
	FMLog::g_siLogShowErrorType = 0;
	if( e_bStroeInfo )
		g_strParsingDataList = &this->m_strParsingDataList;
	if(!cNodeISAX::ParseDataIntoXMLNode(e_strFileName))
		return false;
	if( m_pRootElement )
	{
		TiXmlElement*l_pTiXmlElement =  m_pRootElement->FirstChildElement();
		while( l_pTiXmlElement )
		{
			const WCHAR*l_strName = l_pTiXmlElement->Value();
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_EFFECTS]) )
			{
				if( m_bXMLParseFlag[eCMT_EFFECTS] )
					m_LibraryEffects.ParseData(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_MATERIALS]) )
			{
				if( m_bXMLParseFlag[eCMT_MATERIALS] )
					m_LibraryMaterials.ParseData(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_GEOMETRIES]) )
			{
				if( m_bXMLParseFlag[eCMT_GEOMETRIES] )
					m_LibraryGeometries.ParseData(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_LIGHTS]) )
			{
				//if( m_bXMLParseFlag[eCMT_LIGHTS] )
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_IMAGES]) )
			{
				if( m_bXMLParseFlag[eCMT_IMAGES] )
				{
					m_LibraryImages.SetDirectory(m_strCurrentDirectory);
					m_LibraryImages.ParseData(l_pTiXmlElement);
				}
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_CAMERAS]) )
			{
				//if( m_bXMLParseFlag[eCMT_CAMERAS] )
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_CONTROLLERS]) )
			{
				if( m_bXMLParseFlag[eCMT_CONTROLLERS] )
					m_LibraryControllers.ParseData(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_VISUAL_SCENE]) )
			{
				if( m_bXMLParseFlag[eCMT_VISUAL_SCENE] )
					m_LibraryVisualScenes.ParseData(l_pTiXmlElement);
			}
			else
			if( !wcscmp(l_strName,g_sColladaMainType[eCMT_ANIMATION]) )
			{
				if( m_bXMLParseFlag[eCMT_ANIMATION] )
					m_LibraryAnimation.ParseData(l_pTiXmlElement);
			}
			l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
		}
	}
	ProcessWeightAndBlendingBoneIndicesMergeToMesh();
	ProcessGeometries();
	ProcessImages();
	ProcessAnimations();
	ProcessController();
	ProcessVisualSceneas();
	//clear share data
	for(UINT i=0;i<m_LibraryGeometries.m_ColladaGeomatryList.size();++i)
	{
		sColladaGeomatries*l_pColladaGeomatries = m_LibraryGeometries.m_ColladaGeomatryList[i];
		if( l_pColladaGeomatries->pGeometryMesh )
			l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList.clear();
	}

	FMLog::g_siLogShowErrorType = l_iShowErrorMsgBoxType;
	if(FMLog::g_spstrLogErrorMsgString)
	{
		if(FMLog::g_spstrLogErrorMsgString->length())
		{
			UT::ErrorMsg((WCHAR*)FMLog::g_spstrLogErrorMsgString->c_str(),L"Error msg");
		}
	}
	if( !l_bErrorMsgExists )
	{
		SAFE_DELETE(FMLog::g_spstrLogErrorMsgString);
	}
	return true;
}

void	cColladaParser::ProcessEffects()
{

}
void	cColladaParser::ProcessMaterials()
{

}
//========================
//find match technique and texture
//to create mesh data
//it's quite weired here,we have to assign texture by visual scene node,
//but somehow it's possible lost assign texture assigment,because the node does't has
//instance element,so we have to assign by geometry's triangle data.
//========================
void	cColladaParser::ProcessGeometries()
{
	if( !m_pAllMesh )
		m_pAllMesh = new cNamedTypedObjectVector<cMesh>;
	//copy data but keep share,because we need data to setup material
	for(UINT i=0;i<m_LibraryGeometries.m_ColladaGeomatryList.size();++i)
	{
		sColladaGeomatries*l_pColladaGeomatries = m_LibraryGeometries.m_ColladaGeomatryList[i];
		if( l_pColladaGeomatries->pGeometryMesh )
		for(UINT l_iNumMesh = 0;l_iNumMesh<l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList.size();++l_iNumMesh)
		{
			cMesh*l_pMesh = l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList[l_iNumMesh];
			m_pAllMesh->AddObject(l_pMesh);
		}
	}
	//assign texture data by mesh data
	size_t	l_iInstanceNode = m_LibraryVisualScenes.m_VisualSceneData.AllInstanceNodesVector.size();
	for(size_t l_iNumInstance = 0;l_iNumInstance<l_iInstanceNode;++l_iNumInstance)
	{
		sVisualSceneLibrary::sNode*l_pNode = m_LibraryVisualScenes.m_VisualSceneData.AllInstanceNodesVector[l_iNumInstance];
		sColladaGeomatries*l_pColladaGeomatries = 0;
		if( l_pNode->pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_GEOMETRY )
			l_pColladaGeomatries = m_LibraryGeometries.GetGeometryByID(&l_pNode->pInstance->strUrl[1]);
		else
		if( l_pNode->pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_CONTROLLER )
		{
			size_t l_iNumController = this->m_LibraryControllers.m_ControllerList.size();
			for(size_t i=0;i<l_iNumController;++i)
			{
				const WCHAR*l_strControllerName = &l_pNode->pInstance->strUrl[1];
				sController*l_pController = this->m_LibraryControllers.m_ControllerList[i];
				if(!wcscmp(l_pController->strID,l_strControllerName))
				{
					const WCHAR*l_strGeometrySource = &l_pController->Skin.strSource[1];
					l_pColladaGeomatries = m_LibraryGeometries.GetGeometryByID(l_strGeometrySource);
				}
			}
		}
		if( !l_pColladaGeomatries||!l_pColladaGeomatries->pGeometryMesh )
		{
			if( l_pColladaGeomatries )
			{
				cGameApp::OutputDebugInfoString(l_pColladaGeomatries->strID);
				cGameApp::OutputDebugInfoString(L" does't support\n");
			}
			else
			{
				cGameApp::OutputDebugInfoString(&l_pNode->pInstance->strUrl[1]);
				cGameApp::OutputDebugInfoString(L" does't support\n");
			}
			continue;
		}
		assert(l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList.size() == 1&&"current only support one geometry one mesh ask artist to merge them as one,because I do not know how to assign weight and influnce joints");
		for(UINT l_iNumMesh = 0;l_iNumMesh<l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList.size();++l_iNumMesh)
		{
			cMesh*l_pMesh = l_pColladaGeomatries->pGeometryMesh->AllMeshIndexList[l_iNumMesh];
			if( l_pNode->pInstance->IndtanceType == sVisualSceneLibrary::sNode::eIT_GEOMETRY )
			{
				//if( l_pNode->pParent )
				//{
				//	sVisualSceneLibrary::sNode*l_pParent = l_pNode->pParent;
				//	cMatrix44	l_mat = cMatrix44::Identity;
				//	while(l_pParent)
				//	{
				//		l_mat*=l_pParent->mat;
				//		l_pParent = l_pParent->pParent;
				//	}
				//	l_pMesh->SetLocalTransform(l_mat);
				//}
				//else
					l_pMesh->SetLocalTransform(l_pNode->mat);
				cGameApp::OutputDebugInfoString(l_pMesh->GetName());
				cGameApp::OutputDebugInfoString(L"  setMatrix\n\0");
			}
			const WCHAR*l_strMaterialTarget = &l_pNode->pInstance->strMaterialTarget[1];//strip #
			sMaterial*l_sMaterial = m_LibraryMaterials.GetMaterialByID(l_strMaterialTarget);
			sEffectData*l_pEffectData = m_LibraryEffects.GetEffectByID(&l_sMaterial->strInstanceEffectName[1]);
			sEffectData::sComonEffect*l_pComonEffect = &l_pEffectData->ComonEffect;
			for(UINT m=0;m<l_pComonEffect->AllTechniqueAndTextureName.size();++m)
			{
				sEffectData::sComonEffect::sTechniqueData*l_pTechniqueData = l_pComonEffect->AllTechniqueAndTextureName[m];
				int	l_EL = l_pTechniqueData->eTL;
				if( !l_pTechniqueData->strTextureName )
					continue;
				std::wstring	l_strForClone = l_pTechniqueData->strTextureName;
				WCHAR	*l_strTextureName = wcstok((WCHAR*)l_strForClone.c_str(),L"-");
				cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(this->m_LibraryImages.m_pImageList->GetObject(l_strTextureName));
				if( !l_pBaseImage )
				{
					WCHAR	*l_strTechniqueTextureName = wcstok((WCHAR*)l_pTechniqueData->strTextureName,L"-");
					l_pBaseImage = this->m_LibraryImages.m_pImageList->GetBaseImage(l_strTextureName);
					if( !l_pBaseImage )
					{//for FCollada export
						int	l_iLastDashIndex = FindChacterIndexFromBackBuffer(l_strTechniqueTextureName,L'_');
						if( l_iLastDashIndex != -1 )
						{
							l_strTechniqueTextureName[l_iLastDashIndex] = L'.';
							l_pBaseImage = this->m_LibraryImages.m_pImageList->GetBaseImage(l_strTechniqueTextureName);
						}
						if( !l_pBaseImage )
						{							

							cGameApp::OutputDebugInfoString(l_pTechniqueData->strTextureName);
							cGameApp::OutputDebugInfoString(L" does not exist\n");
							//UT::ErrorMsg("failed to fetech texture",(char*)l_pTechniqueData->strTextureName);
							continue;
						}
					}
				}
				if(l_pMesh->GetObject(l_pBaseImage->GetName()))
					continue;
				sTechniqueAndTexture*l_psTechniqueAndTexture = new sTechniqueAndTexture(l_pBaseImage,l_pTechniqueData->eTL);
				l_pMesh->AddObject(l_psTechniqueAndTexture);
			}
			if( l_pMesh->Count() == 0 )
			{
				if( !l_pEffectData->m_strName )
					continue;
				cBaseImage*l_pBaseImage = this->m_LibraryImages.m_pImageList->GetBaseImage(l_pEffectData->m_strName);
				if( !l_pBaseImage )
				{
					continue;
				}
				//assign it to our mesh
				sTechniqueAndTexture*l_psTechniqueAndTexture = new sTechniqueAndTexture(l_pBaseImage,eTL_DIFFUSE);
				l_pMesh->AddObject(l_psTechniqueAndTexture);
			}
		}
	}
	//m_CurveList.clear();
	//m_CurveList = m_LibraryGeometries.ProcessCurveData();
}
void	cColladaParser::ProcessLights()
{

}
void	cColladaParser::ProcessImages()
{
	//int	l_iNum = this->m_LibraryImages.m_pImageList->Count();
	//std::vector<WCHAR*>	l_NonUseImageVector;
	//for(int i=0;i<l_iNum;++i)
	//{
	//	const WCHAR*l_str = m_LibraryImages.m_pImageList->GetObjectW(i)->GetName();
	//	bool	l_bTextureAssigned = false;
	//	for( int j=0;j<this->m_pAllMesh->Count();++j )
	//	{
	//		if((*m_pAllMesh)[j]->GetObject(l_str))
	//		{
	//			l_bTextureAssigned = true;
	//			j = m_pAllMesh->Count();
	//		}
	//	}
	//	if(!l_bTextureAssigned)
	//	{
	//		//l_NonUseImageVector.push_back(l_str);
	//		UT::ErrorMsg(l_str,L"this image has not been assigned");
	//	}
	//}
	//for( UINT i=0;i<l_NonUseImageVector.size();++i )
	//{
	//	m_LibraryImages.m_pImageList->RemoveObject(l_NonUseImageVector[i]);
	//}
}
void	cColladaParser::ProcessCameras()
{

}
void	cColladaParser::ProcessVisualSceneas()
{
	if(m_LibraryVisualScenes.m_VisualSceneData.pRootNode && m_pAllMesh)
	{
		sVisualSceneLibrary::sNode*l_pNode =  m_LibraryVisualScenes.m_VisualSceneData.pRootNode->pFirstChild;
		while( l_pNode )
		{
			cMesh*l_pMesh = m_pAllMesh->GetObject(l_pNode->strName);
			if( l_pMesh )
				l_pMesh->ApplyNodeMatrix(l_pNode->mat);
			l_pNode = l_pNode->pSibling;
		}
	}
}

void	cColladaParser::ProcessAnimations()
{
	bool	l_bShowDebugInfo = false;
	if(!m_LibraryVisualScenes.m_VisualSceneData.AllSkeletonNodeVector.size())
		return;
	sVisualSceneLibrary::sSkeletonNode**	l_ppSkeletonNode = &m_LibraryVisualScenes.m_VisualSceneData.AllSkeletonNodeVector[0];
	int	l_iNumSkeleton = (int)m_LibraryVisualScenes.m_VisualSceneData.AllSkeletonNodeVector.size();
	//first find out match mesh
	//second get bone as need and assign animation into bone.
	for( int i=0;i<l_iNumSkeleton;++i )
	{
		sVisualSceneLibrary::sSkeletonNode*	l_pSkeletonNode = l_ppSkeletonNode[i];
		int	l_iNumBone = l_pSkeletonNode->m_AllBoneData.Count();
		for( int j=0;j<l_iNumBone;++j )
		{
			cBone*l_pAllBone = l_pSkeletonNode->m_AllBoneData.GetObject(j);
			//WCHAR*l_strBoneName = l_pAllBone->GetName();
			const WCHAR*l_strBoneName = l_pSkeletonNode->AllNodesOfSkeletonNodeVector[j]->strID;
			sAnimationLibrary::sAnimation*	l_pAnimation = m_LibraryAnimation.m_AnimationLibrary.GetAnimation(l_strBoneName);
			if( l_pAnimation )
			{
				l_pAllBone->SetFormKeyFrames(l_pAnimation->GenerateKeyFrameMatrix(l_strBoneName));
				l_pAllBone->m_bAnimation = true;
			}
			else
			{
				l_pAllBone->m_bAnimation = false;
			}
			if( l_bShowDebugInfo )
			{
				if( l_pAnimation )
				{
					cGameApp::OutputDebugInfoString(l_strBoneName);
					cGameApp::OutputDebugInfoString(L" has animation\n");
				}
				else
				{
					cGameApp::OutputDebugInfoString(l_strBoneName);
					cGameApp::OutputDebugInfoString(L" does't have animation data\n\0");
				}
			}

		}
	}
}


void	cColladaParser::ProcessWeightAndBlendingBoneIndicesMergeToMesh()
{
	size_t	l_uiSize = m_LibraryControllers.m_ControllerList.size();
	for(size_t i=0;i<l_uiSize;++i )
	{
		sController*l_pController = m_LibraryControllers.m_ControllerList[i];
		if(m_LibraryGeometries.GenerateSkinningTriangleBlendicesWeights(l_pController))
			continue;
		else
		{
			if( this->m_bXMLParseFlag[eCMT_GEOMETRIES] )
				UT::ErrorMsg((WCHAR*)l_pController->Skin.strSource,L"  can't find respond geometry mesh");
		}
	}
}