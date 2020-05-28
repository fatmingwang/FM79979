#pragma once

#include "TrianglesToDrawIndicesBuffer.h"
#define	HINT_VERTEX_POINT_SIZE	8


#define	MORPHING_ANIMATION_VERSION		20200522
#define	MORPHING_ANIMATION_ROOT_NAME	L"MorphingAnimationRoot"
#define	MORPHING_ANIMATION_OBJECT_NAME	L"MorphingAnimation"
#define	MORPHING_ANIMATION_VERTEX_DATA	L"VertexData"

//morphing xml
#define FM_MORPHING_XML_FILE_EXTENSION_NAME					"mx"

#define	MX_ELEMENT_NAME										"PI_morphing"
//morphing binary
#define FM_MORPHING_FILE_BINARY_DATA_EXTENSION_NAME			"mb"

#define	FM_MORPHING_EDITOR_FILE_BINARY_DATA_EXTENSION_NAME	"editor_mb"

template<class FIRST, class SECOND>std::vector<FIRST>	GetFirstVectorFromMap(std::map<FIRST, SECOND>&e_Map)
{
	std::vector<FIRST> l_Vector;
	for (auto l_Iterator = e_Map.begin(); l_Iterator != e_Map.end(); ++l_Iterator)
	{
		l_Vector.push_back(l_Iterator->first);
	}
	return l_Vector;
}

template<class FIRST, class SECOND>std::vector<SECOND>	GetSecondVectorFromMap(std::map<FIRST, SECOND>&e_Map)
{
	std::vector<SECOND> l_Vector;
	for (auto l_Iterator = e_Map.begin(); l_Iterator != e_Map.end(); ++l_Iterator)
	{
		l_Vector.push_back(l_Iterator->second);
	}
	return l_Vector;
}

template<class FIRST, class SECOND>void	AssignDataToMap(std::map<FIRST, SECOND>&e_Map, std::vector<FIRST>&e_FirstDataVector, std::vector<SECOND>&e_SecondDataVector)
{
	assert(e_FirstDataVector.size() == e_SecondDataVector.size());
	auto l_uiSize = e_FirstDataVector.size();
	for (auto i = 0; i < l_uiSize; ++i)
	{
		e_Map[e_FirstDataVector[i]] = e_SecondDataVector[i];
	}
}


class cEditor_MorphingAnimation :public NamedTypedObject
{
	friend class cPuzzleImageUnitTriangulator;
	sTrianglesToDrawIndicesBuffer*	m_pTarget;
	//
	struct sVertexIndexAndPositionAndTimeVector
	{
		sVertexIndexAndPositionAndTimeVector();
		typedef std::map<float, Vector3> FloatTocVector3Map;
		//relasted to vRenderPosVector
		Vector3*			pRenderPos;
		Vector3*			pTime0VertexPos;
		//time and 
		FloatTocVector3Map	m_FormKeyFrames;
		Vector3				UpdateAnimationByGlobalTime(float e_fGlobalTime);
		//find previous key time position.
		bool				AddKey(float   e_fTime, bool e_bUsePreKeyData);
		bool				AssignKeyVector(std::vector<float>&e_fTimeVector);
		bool				DeleteKey(float   e_fTime);
		bool				ChangeData(Vector3 e_vPos, float   e_fTime);
		//bool	KeyTime0PositionChange(Vector3 e_vPos);
		bool				IsTimeAvaliable(float e_fTime);
		void				RearrangeTime(float e_fScale);
		void				RenderPointByTime(float e_fTime,Vector4 e_vColor,float e_fPointSize);
		void				SetTime0VertexPos(Vector3*e_pvPos);
		TiXmlElement*		ToTiXmlElement(bool e_bOptimizeTime);
		void				AssignDataFromElement(TiXmlElement*e_pTiXmlElement);
		FloatTocVector3Map	GetOptimizeDataSize();
	};
	//
	//from sTrianglesToDrawIndicesBuffer::vPosVector
	//std::vector<Vector3>								vVertexTime0PosVector;
	//
	std::vector<Vector3>								vRenderPosVector;
	////count and order should be same as sTrianglesToDrawIndicesBuffer::vPosVector
	std::vector<sVertexIndexAndPositionAndTimeVector>	m_VertexAnimationVector;
	//for UI listbox
	std::vector<float>									m_fListboxTimeVector;
	float												m_fCurrentMorphingAnimationTime;
	void												RearrangeTimeByScale(float e_fScale);
	void												AddKeyTime(float e_fTime, bool e_bUsePreKeyData);
	void												RemoveKeyTime(float e_fTime);
	void												ReassignRenderPosVectorAfterVertexIndexChange();
public:
	cEditor_MorphingAnimation(sTrianglesToDrawIndicesBuffer*e_pTarget);
	//std::map<float, std::vector<sMorphingData>>	m_TimeAndVertexPosVectorData;
	//if user add or delete triangle vertices,the index could be changed,so here need to reassign vertex index again
	void					ApplyVertexIndexChangeForMorphingAnimation(std::map<int, int>&e_ChangedIndexMap);
	void					DeleteVertexIndexData(int e_iVertexIndex);
	//move time 0.
	//void					VertexMove(int e_iVertexIndex, Vector3 e_vPos);
	bool					ChangeData(int e_iVertexIndex, Vector3 e_vPos, float   e_fTime);
	//also move position offset for key time data.
	//bool					ChangeKeyTime0Data(int e_iVertexIndex, Vector3 e_vPos);
	bool					IsTimeAvaliable(int e_iVertexIndex, float   e_fTime);
	//generate from sTrianglesToDrawIndicesBuffer,while morphing animation edit is active
	bool					ApplyEmptyAnimationData();
	void					UpdateAnimationByGlobalTime(float e_fElpaseTime);
	void					Render(cMatrix44 e_Mat,cBaseImage*e_pImage);
	void					RenderByTimeForHint(float e_fElpaseTime,Vector4 e_vColor,cMatrix44 e_Mat, cBaseImage*e_pImage);
	void					RearrangeTime(float e_fTargetTime);

	bool					AddListboxTime(float e_fTime,bool e_bUsePreKeyData);
	bool					DeleteListboxTime(int e_iIndex);
	bool					ChangeListboxTime(int e_iIndex, float e_fTime);
	bool					InsertListboxTime(int e_iIndex, float e_fTime, bool e_bUsePreKeyData);
	bool					SetCurrentListboxTime(int e_iIndex);
	float					GetCurrentListboxTime() { return m_fCurrentMorphingAnimationTime; }
	float					GetEndTime();
	std::vector<float>*		GetListboxTimeVector() { return &m_fListboxTimeVector; }
	int						FinClosestVertexIndex(Vector3 e_vPos);
	void					RenderVertexPointByVertexIndex(int e_iVertexIndex, Vector4 e_vColor, float e_fPointSize);
	void					DataCleanUp();
	TiXmlElement*			ToTiXmlElement(cBinaryFile*e_pMorphingData, cBinaryFile*e_pOptmizeMorphingData);
	//return binray step
	int						ParseMorphAnimationElement(TiXmlElement*e_pMorphAnimationElement, char*e_pBinaryData);
};


//do export all triangles and uv and index buffer.