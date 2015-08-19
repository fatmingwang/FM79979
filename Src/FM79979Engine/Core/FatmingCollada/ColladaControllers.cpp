#include "StdAfx.h"
#include "ColladaControllers.h"
const wchar_t*	cLibraryControllers::TypeID( L"cLibraryControllers" );
//
//
//
sController::sSkin::sSource*	sController::sSkin::GetSourceByType(eControllerType e_eControllerType)
{
	for(UINT i=0;i<this->AllSource.size();++i)
	{
		if(AllSource[i]->ControllerType == e_eControllerType)
			return AllSource[i];
	}
	return 0;
}

sController::sSkin::sSource*	sController::sSkin::GetSourceBySourceID(const WCHAR*e_strSourceID)
{
	for(UINT i=0;i<this->AllSource.size();++i)
	{
		if(!wcscmp(AllSource[i]->strID,e_strSourceID))
			return AllSource[i];
	}
	return 0;
}


void	GetBiggestValueAndIndex(float *e_fValue,int e_iCount,int *e_piBiggestIndex,float *e_pfBiggestValue)
{
	float	l_fMax = e_fValue[0];
	*e_piBiggestIndex = 0;
	int	l_iMaxIndex = 0;
	for( int i=1;i<e_iCount;++i )
	{
		if(l_fMax<e_fValue[i])
		{
			l_fMax = e_fValue[i];
			*e_piBiggestIndex = i;
		}
	}
	*e_pfBiggestValue = l_fMax;
}

void	GetSmallestValueAndIndex(float *e_fValue,int e_iCount,int *e_piSmallestIndex,float *e_pfSmallestValue)
{
	float	l_fMin = e_fValue[0];
	int	l_iMinIndex = 0;
	for( int i=0;i<e_iCount;++i )
	{
		if(l_fMin>=e_fValue[i])
		{
			l_fMin = e_fValue[i];
			*e_piSmallestIndex = i;
		}
	}
	*e_pfSmallestValue = l_fMin;
}

void	SortValueAndGetNewIndex(bool e_bBiggerOrder,float *e_fValue,int e_iCount,int *e_piSmallestIndex,float *e_pfSmallValue)
{
	std::vector<float>	l_fValueVector;
	std::vector<int>	l_fIndexVector;
	l_fValueVector.reserve(e_iCount);
	for( int i=0;i<e_iCount;++i )
	{
		l_fValueVector.push_back(e_fValue[i]);
	}
	//first we have to find biggest or smallest
	float	l_fValueForCompare = e_fValue[0];
	if( e_bBiggerOrder )
	{
		for( int i=1;i<e_iCount;++i )
		{
			if(l_fValueForCompare>e_fValue[i])
				l_fValueForCompare = e_fValue[i];
		}
		l_fValueForCompare -= 1;
	}
	else
	{
		for( int i=1;i<e_iCount;++i )
		{
			if(l_fValueForCompare<e_fValue[i])
				l_fValueForCompare = e_fValue[i];
		}
		l_fValueForCompare += 1;
	}
	//so after we get smallest one we could replace the value as biggest+i,so it won't to reconsider again
	std::vector<float>	l_fNewValueVector;
	float	*l_pfValue = &l_fValueVector[0];
	for( int i=0;i<e_iCount;++i )
	{
		int	l_iIndex;
		float	l_fValue;
		if( e_bBiggerOrder )
		{
			GetBiggestValueAndIndex(l_pfValue,e_iCount,&l_iIndex,&l_fValue);
			l_fNewValueVector.push_back(l_fValue);
			l_pfValue[l_iIndex] = l_fValueForCompare;
		}
		else
		{
			GetSmallestValueAndIndex(l_pfValue,e_iCount,&l_iIndex,&l_fValue);
			l_fNewValueVector.push_back(l_fValue);
			l_pfValue[l_iIndex] = l_fValueForCompare;
		}
#ifdef DEBUG
		for( int j=0;j<(int)l_fIndexVector.size();++j )
		{
			if( l_iIndex == l_fIndexVector[j] )
				assert(0);
		}
#endif
		l_fIndexVector.push_back(l_iIndex);
	}
	memcpy(e_pfSmallValue,&l_fNewValueVector[0],sizeof(float)*e_iCount);
	memcpy(e_piSmallestIndex,&l_fIndexVector[0],sizeof(int)*e_iCount);
}
//
//
//
void	sController::sSkin::GenerateWeightsAndBonesData()
{
	assert(!pInflunceBonesForVertex);
	assert(!pInflunceWeightsForVertex);
	//first we have to find out joints and weights source
	sSource*l_pJointSource = GetSourceByType(eCT_JOINT);
	sSource*l_pWeightSource = GetSourceByType(eCT_SKINWEIGHTS);
	int	l_iNumBones = l_pJointSource->iCount;
	int*	pVcount = this->VertexWeights.pVcount;	//all vertex influnce bone count
	int*	pV		= this->VertexWeights.pV;		//related bones and weights
	float*	pWeights = 0;
	int	l_iWeightOffset = -1;
	for(UINT i = 0;i<this->VertexWeights.InputList.size();++i)
	{
		if( !wcscmp(VertexWeights.InputList[i]->strSemantic,L"WEIGHT") )
		{
			l_iWeightOffset = VertexWeights.InputList[i]->iOffset;			
			l_pWeightSource = GetSourceBySourceID(&VertexWeights.InputList[i]->strSource[1]);
			pWeights = l_pWeightSource->pData;
		}
	}
	int	l_iRelateDataCount = VertexWeights.iCount*MAX_RELEATED_BONE_COUNT;
	std::vector<float>	l_AllWeightsVector;
	std::vector<int>	l_AllJointsVector;
	std::vector<char>	l_AllJointsVectorInChar;
	l_AllWeightsVector.reserve(l_iRelateDataCount);
	l_AllJointsVector.reserve(l_iRelateDataCount);
	l_AllJointsVectorInChar.reserve(l_iRelateDataCount);
	//second compare relate bone count,if it's small then MAX_RELEATED_BONE_COUNT,optmize it,or scale it to four
	//and assign new data into pInflunceBonesForVertex and pInflunceWeightsForVertex.
	int	l_iCurrentProgess = 0;
	int	l_iOffsetForJoint = VertexWeights.GetInputyType(L"JOINT")->iOffset;
	int	l_iOffsetForWeight = VertexWeights.GetInputyType(L"WEIGHT")->iOffset;
	#define	MAX_SUPPORT_BONES_TEST	16
	int	l_iBestJointsArray[MAX_SUPPORT_BONES_TEST];
	float	l_fBestWeightsArray[MAX_SUPPORT_BONES_TEST];
	//assert(0&&"weired here could be wrong");
	for( int i=0;i<this->VertexWeights.iCount;++i )
	{
		memset(l_iBestJointsArray,0,sizeof(int)*MAX_SUPPORT_BONES_TEST);
		memset(l_fBestWeightsArray,0,sizeof(float)*MAX_SUPPORT_BONES_TEST);
		int	l_iNumInflunceBones = pVcount[i];
		assert(l_iNumInflunceBones<MAX_SUPPORT_BONES_TEST&&"fuck over MAX_SUPPORT_BONES_TEST inclunce joints!!???");
		for( int j=0;j<l_iNumInflunceBones;++j )
		{
			int	l_iWeightIndex2 = pV[l_iCurrentProgess+l_iOffsetForWeight];
			int	l_iJointIndex2 = pV[l_iCurrentProgess+l_iOffsetForJoint];
			float	l_fWeight = pWeights[pV[l_iCurrentProgess+l_iOffsetForWeight]];
			int		l_iJointIndex = pV[l_iCurrentProgess+l_iOffsetForJoint];
			l_iBestJointsArray[j] = l_iJointIndex;
			l_fBestWeightsArray[j] = l_fWeight;
			assert(l_iJointIndex<l_pJointSource->iCount&&"joint's index is bigger than joint source data count");
			assert(pV[l_iCurrentProgess+l_iOffsetForWeight]<l_pWeightSource->iCount&&"weight's index is bigger than weight source data count");
			l_iCurrentProgess+=2;//one is weight one is joint
		}
		if( l_iNumInflunceBones>MAX_RELEATED_BONE_COUNT )
		{
			int	l_iNewIndex[MAX_SUPPORT_BONES_TEST];
			float	l_fNewBestWeightsArray[MAX_SUPPORT_BONES_TEST];
			memcpy(l_fNewBestWeightsArray,l_fBestWeightsArray,sizeof(float)*MAX_SUPPORT_BONES_TEST);
			SortValueAndGetNewIndex(true,l_fBestWeightsArray,l_iNumInflunceBones,l_iNewIndex,l_fNewBestWeightsArray);
			float	l_fNewValue = 1.f/(l_fNewBestWeightsArray[0]+l_fNewBestWeightsArray[1]+l_fNewBestWeightsArray[2]+l_fNewBestWeightsArray[3]);
			for(int k=0;k<MAX_RELEATED_BONE_COUNT;++k)
			{
				float	l_fWeight = l_fNewBestWeightsArray[k]*l_fNewValue;
				int	l_iJointIndex = l_iBestJointsArray[l_iNewIndex[k]];
				l_AllWeightsVector.push_back(l_fWeight);
				l_AllJointsVector.push_back(l_iJointIndex);
				l_AllJointsVectorInChar.push_back((char)l_iJointIndex);
			}
			assert(l_fNewBestWeightsArray[0]>=l_fNewBestWeightsArray[1]&&
				l_fNewBestWeightsArray[1]>=l_fNewBestWeightsArray[2]&&
				l_fNewBestWeightsArray[2]>=l_fNewBestWeightsArray[3]);
		}
		else
		{//it's small than MAX_RELEATED_BONE_COUNT,it does't need to cook it.
			for(int k=0;k<MAX_RELEATED_BONE_COUNT;++k)
			{
				l_AllWeightsVector.push_back(l_fBestWeightsArray[k]);
				l_AllJointsVector.push_back(l_iBestJointsArray[k]);
				l_AllJointsVectorInChar.push_back((char)l_iBestJointsArray[k]);
			}			
		}
//#ifdef DEBUG
//		float l_fTotalWeight = 0.f;
//		for(int k=0;k<MAX_RELEATED_BONE_COUNT;++k)
//		{
//			float l_fWeightTest = l_AllWeightsVector[l_AllWeightsVector.size()-1-k];
//			l_fTotalWeight += l_fWeightTest;
//		}
//		if( l_fTotalWeight>1.1f||l_fTotalWeight<=0.9f )
//		{
//			assert(0);
//		}
//#endif
	}
	int	l_iWeightCount = l_AllWeightsVector.size();
	assert(l_AllWeightsVector.size() == l_iRelateDataCount);
	assert(l_AllJointsVector.size() == l_iRelateDataCount);
	//third create the data store
	//if total bones is bigger than 512 byte is not enough to store all bones instead by int.
	pInflunceBonesForVertex = new sDataAndDataType(l_iNumBones>512?eDT_INT:eDT_BYTE,l_iRelateDataCount);//basicly this data's count is vertices*MAX_RELEATED_BONE_COUNT
	pInflunceWeightsForVertex = new sDataAndDataType(eDT_FLOAT,l_iRelateDataCount);//basicly this data's count is vertices*MAX_RELEATED_BONE_COUNT
	pInflunceWeightsForVertex->eType = eDT_FLOAT;
	pInflunceWeightsForVertex->uiCount = l_iWeightCount;
	memcpy(pInflunceWeightsForVertex->pData,&l_AllWeightsVector[0],sizeof(float)*l_iRelateDataCount);
	pInflunceBonesForVertex->eType = l_pJointSource->iCount<255?eDT_BYTE:eDT_FLOAT;
	pInflunceBonesForVertex->uiCount = l_iWeightCount;
	if( pInflunceBonesForVertex->eType == eDT_BYTE )
		memcpy(pInflunceBonesForVertex->pData,&l_AllJointsVectorInChar[0],sizeof(char)*l_iRelateDataCount);
	else
	{
		assert(0&&"current does not support so much bone blending indices");
		memcpy(pInflunceBonesForVertex->pData,&l_AllJointsVector[0],sizeof(float)*l_iRelateDataCount);
	}
}
//
//
//
int		sController::sSkin::VisualSceneJointNodeNameToIndex(const WCHAR*e_strNodeName)
{
	sSource*l_pJointSource = GetSourceByType(eCT_JOINT);
	for(  int i=0;i<l_pJointSource->iCount;++i)
	{
		if(!wcscmp(l_pJointSource->strAllJointName[i].c_str(),e_strNodeName))
			return i;
	}
	return -1;
}
//
//
//
void	cLibraryControllers::ParseData(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller]))
		{
			sController*l_pController = new sController();
			m_pCurrentController = l_pController;
			m_ControllerList.push_back(l_pController);
			Parsecontroller(l_pChildTiXmlElement);
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}
}
//
//
//
void	cLibraryControllers::Parsecontroller(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentController->strID =  l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"name"))
		{
			m_pCurrentController->strName =  l_pTiXmlAttribute->Value();
		}
		
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin]))
		{
			Parsecontroller_skin( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,L"extra"))
		{//lazy to do this
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"source"))
		{
			m_pCurrentController->Skin.strSource =  l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_bind_shape_matrix]))
		{
			Parsecontroller_skin_bind_shape_matrix( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source]))
		{
			Parsecontroller_skin_source( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_joints]))
		{
			Parsecontroller_skin_joints( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_vertex_weights]))
		{
			Parsecontroller_skin_vertex_weights( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_bind_shape_matrix(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	m_pCurrentController->Skin.matBindShapeMatrix =  GetMatrix(l_strText);
}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strSourceID = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sController::sSkin::sSource*l_pSource = new sController::sSkin::sSource;
	m_pCurrentController->Skin.AllSource.push_back(l_pSource);
	m_pCurrentSource = l_pSource;
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"id"))
		{
			const WCHAR*	l_strTypeString = l_strValue+wcslen(this->m_pCurrentController->strID)+1;
			if( !wcscmp(l_strTypeString,L"joints") )
				m_pCurrentSource->ControllerType = sController::sSkin::eCT_JOINT;
			else
			if( !wcscmp(l_strTypeString,L"bind_poses") )
				m_pCurrentSource->ControllerType = sController::sSkin::eCT_BINDPOSE;
			else
			if( !wcscmp(l_strTypeString,L"weights") )
				m_pCurrentSource->ControllerType = sController::sSkin::eCT_SKINWEIGHTS;
			else
				UT::ErrorMsg((WCHAR*)l_strTypeString,L"unknow type");
			l_pSource->strID = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source_Name_array]))
		{
			Parsecontroller_skin_source_Name_array( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source_technique_common]))
		{
			Parsecontroller_skin_source_technique_common( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source_float_array]))
		{
			Parsecontroller_skin_source_float_array( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source_Name_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentSource->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentSource->strArrayID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	WCHAR*	l_strBoneName = wcstok((WCHAR*)l_strText,L" ");
	std::vector<std::wstring>l_strAllJointNameVector;
	int	l_iIndex = 0;
	while( l_strBoneName )
	{
		std::wstring	l_strAllJointName = l_strBoneName;
		try
		{
			l_strAllJointNameVector.push_back(l_strAllJointName);
			//m_pCurrentSource->strAllJointName.push_back(l_strAllJointName);
		}
		catch(std::exception exp)
		{
			const char*l_str = exp.what();
			int a=0;
		}
		l_iIndex++;
		l_strBoneName = wcstok(0,L" ");
	}
	m_pCurrentSource->strAllJointName = l_strAllJointNameVector;
	assert( l_iIndex == m_pCurrentSource->iCount );

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source_technique_common(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source_technique_common_accessor]))
		{
			Parsecontroller_skin_source_technique_common_accessor( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source_technique_common_accessor(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			//l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			//l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"stride"))
		{
			//l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_source_technique_common_accessor_param]))
		{
			Parsecontroller_skin_source_technique_common_accessor_param( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source_technique_common_accessor_param(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"name"))
		{
			//l_strValue;
		}
		else
		if(!wcscmp(l_strName,L"type"))
		{
			//l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_source_float_array(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			m_pCurrentSource->iCount = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"id"))
		{
			m_pCurrentSource->strArrayID = l_strValue;
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	if( m_pCurrentSource->ControllerType == sController::sSkin::eCT_BINDPOSE )
	{
		m_pCurrentSource->pMatrcies = GetMatrcies(l_strText,m_pCurrentSource->iCount/16);
		//for(int i=0;i<m_pCurrentSource->iCount/16;++i)
		//{
		//	m_pCurrentSource->pMatrcies[i] = m_pCurrentSource->pMatrcies[i];
		//}
	}
	else
	if( m_pCurrentSource->ControllerType == sController::sSkin::eCT_SKINWEIGHTS )
	{
		m_pCurrentSource->pData = ParseDataToGenerateIntPointer<float>(l_strText,m_pCurrentSource->iCount);
	}
	else
	{
		assert(0&&"forgot to setup source data type??");
	}
}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_joints(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_joints_input]))
		{
			Parsecontroller_skin_joints_input( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_joints_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sController::sSkin::sJoints::sInput*l_pInput = new sController::sSkin::sJoints::sInput;
	this->m_pCurrentController->Skin.Joints.InputList.push_back(l_pInput);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInput->strSemantic = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInput->strSource = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_vertex_weights(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"count"))
		{
			this->m_pCurrentController->Skin.VertexWeights.iCount = _wtoi(l_strValue);
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}
	TiXmlElement*l_pChildTiXmlElement = e_pTiXmlElement->FirstChildElement();
	while(l_pChildTiXmlElement)
	{
		l_strName = l_pChildTiXmlElement->Value();
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_vertex_weights_input]))
		{
			Parsecontroller_skin_vertex_weights_input( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_vertex_weights_vcount]))
		{
			Parsecontroller_skin_vertex_weights_vcount( l_pChildTiXmlElement );
		}
		else
		if(!wcscmp(l_strName,g_sLibraryControllersElement[eLC_controller_skin_vertex_weights_v]))
		{
			Parsecontroller_skin_vertex_weights_v( l_pChildTiXmlElement );
		}
		else
		{
			assert(0);
		}
		l_pChildTiXmlElement = l_pChildTiXmlElement->NextSiblingElement();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_vertex_weights_input(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	TiXmlAttribute*l_pTiXmlAttribute = e_pTiXmlElement->FirstAttribute();
	sController::sSkin::sVertexWeights::sInput*l_pInput = new sController::sSkin::sVertexWeights::sInput;
	this->m_pCurrentController->Skin.VertexWeights.InputList.push_back(l_pInput);
	while( l_pTiXmlAttribute )
	{
		l_strName = l_pTiXmlAttribute->Name();
		l_strValue = l_pTiXmlAttribute->Value();
		if(!wcscmp(l_strName,L"offset"))
		{
			l_pInput->iOffset = _wtoi(l_strValue);
		}
		else
		if(!wcscmp(l_strName,L"semantic"))
		{
			l_pInput->strSemantic = l_pTiXmlAttribute->Value();
		}
		else
		if(!wcscmp(l_strName,L"source"))
		{
			l_pInput->strSource = l_pTiXmlAttribute->Value();
		}
		else
		{
			assert(0);
		}
		l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
	}

}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_vertex_weights_vcount(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);

	std::vector<int>	l_NumerialList;
	l_NumerialList.reserve(this->m_pCurrentController->Skin.VertexWeights.iCount);
	WCHAR*	l_str = wcstok((WCHAR*)l_strText,L", ");
	while(l_str)
	{
		int	l_i = _wtoi(l_str);
		l_NumerialList.push_back(l_i);
		m_pCurrentController->Skin.VertexWeights.uiAllVertexInfluncebones += l_i;
		l_str = wcstok(0,L", ");
	}
	assert(this->m_pCurrentController->Skin.VertexWeights.iCount == l_NumerialList.size());
	this->m_pCurrentController->Skin.VertexWeights.pVcount = new int[l_NumerialList.size()];
	memcpy(this->m_pCurrentController->Skin.VertexWeights.pVcount,&l_NumerialList[0],sizeof(int)*l_NumerialList.size());
}
//
//
//
void	cLibraryControllers::Parsecontroller_skin_vertex_weights_v(TiXmlElement*e_pTiXmlElement)
{
	const WCHAR*l_strName = 0;
	const WCHAR*l_strValue = 0;
	const WCHAR*l_strText = e_pTiXmlElement->GetText();
	assert(l_strText);
	//find out stride,max offset +1 is stride
	int	l_iStride = 0;
	std::vector<sController::sSkin::sVertexWeights::sInput*>*l_pInput = &m_pCurrentController->Skin.VertexWeights.InputList;
	for(UINT i=0;i<l_pInput->size();++i)
	{
		if(l_iStride<(*l_pInput)[i]->iOffset)
			l_iStride = (*l_pInput)[i]->iOffset;
	}
	l_iStride+=1;
	//assume all bones have 4 bone influnce,let memory stable I assume it could be.
	UINT	l_uiSize = m_pCurrentController->Skin.VertexWeights.uiAllVertexInfluncebones*l_iStride;
	this->m_pCurrentController->Skin.VertexWeights.pV = ParseDataToGenerateIntPointer<int>((WCHAR*)l_strText,l_uiSize);
#ifdef DEBUG
	//ensure each vertex's influnce joints+weights count as same as all data count,so we could assign the data to each vertex.
	assert( m_pCurrentController->Skin.VertexWeights.uiAllVertexInfluncebones*l_iStride == l_uiSize );
	float	*l_pfWeightsData = this->m_pCurrentController->Skin.GetSourceByType(sController::sSkin::eCT_SKINWEIGHTS)->pData;
	int	l_iCurrentProgress = 0;
	int	l_iWeightOffset = -1;
	for(UINT i = 0;i<m_pCurrentController->Skin.VertexWeights.InputList.size();++i)
	{
		if( !wcscmp(m_pCurrentController->Skin.VertexWeights.InputList[i]->strSemantic,L"WEIGHT") )
		{
			l_iWeightOffset = m_pCurrentController->Skin.VertexWeights.InputList[i]->iOffset;
		}
	}
	for( int i=0;i<this->m_pCurrentController->Skin.VertexWeights.iCount;++i )
	{
		int	l_iNumInflunce = this->m_pCurrentController->Skin.VertexWeights.pVcount[i];
		float	l_fTotalWeight = 0.f;
		for( int j=0;j<l_iNumInflunce;++j )
		{
			int	l_iIndex = m_pCurrentController->Skin.VertexWeights.pV[l_iCurrentProgress+l_iWeightOffset];
			l_fTotalWeight += l_pfWeightsData[l_iIndex];
			l_iCurrentProgress+=2;
		}
		if( l_fTotalWeight<=0.9f )
		{
			assert(0&&"this vertex does't have any weight!!");
		}
	}
#endif
	this->m_pCurrentController->Skin.GenerateWeightsAndBonesData();
}
