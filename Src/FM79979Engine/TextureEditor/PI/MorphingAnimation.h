#pragma once

#include "TrianglesToDrawIndicesBuffer.h"


class cEditor_MorphingAnimation :public NamedTypedObject
{
	sTrianglesToDrawIndicesBuffer*	m_pTarget;
	//
	struct sVertexIndexAndPositionAndTimeVector
	{
		sVertexIndexAndPositionAndTimeVector();
		typedef std::map<float, Vector3> FloatTocVector3Map;
		//relasted to vRenderPosVector
		Vector3*			pPos;
		int					iVertexIndex;
		//time and 
		FloatTocVector3Map m_FormKeyFrames;
		Vector3	UpdateAnimationByGlobalTime(float e_fGlobalTime);
		void	AssignPositionPointer(Vector3*e_pAnimationPositionTarget, int e_iVertexIndex);
		void	AddData(Vector3 e_vPos, float   e_fTime);
		bool	DeleteData(float   e_fTime);
		bool	ChangeData(Vector3 e_vPos, float   e_fTime);
		bool	IsTimeAvaliable(float e_fTime);
		void	RearrangeTime(float e_fScale);
	};
	//
	//from sTrianglesToDrawIndicesBuffer::vPosVector
	std::vector<Vector3>								vMorphingPosVector;
	//
	std::vector<Vector3>								vRenderPosVector;
	////count and order should be same as sTrianglesToDrawIndicesBuffer::vPosVector
	std::vector<sVertexIndexAndPositionAndTimeVector>	m_VertexAnimationVector;
	//for UI listbox
	std::vector<float>									m_fListboxTimeVector;
	float												m_fCurrentMorphingAnimationTime;
	void												RearrangeTimeByScale(float e_fScale);
public:
	cEditor_MorphingAnimation(sTrianglesToDrawIndicesBuffer*e_pTarget);
	//std::map<float, std::vector<sMorphingData>>	m_TimeAndVertexPosVectorData;
	//if user add or delete triangle vertices,the index could be changed,so here need to reassign vertex index again
	void	ReassignVertexIndexData(std::map<int, int>&e_ChangedIndexMap);
	void	DeleteVertexIndexData(int e_iVertexIndex);
	//move time 0.
	void	VertexMove(int e_iVertexIndex, Vector3 e_vPos);
	void	AddData(int e_iVertexIndex, Vector3 e_vPos, float   e_fTime);
	bool	DeleteData(int e_iVertexIndex, float   e_fTime);
	bool	ChangeData(int e_iVertexIndex, Vector3 e_vPos, float   e_fTime);
	bool	IsTimeAvaliable(int e_iVertexIndex, float   e_fTime);
	//generate from sTrianglesToDrawIndicesBuffer,while morphing animation edit is active
	bool	ApplyData();
	void	UpdateAnimationByGlobalTime(float e_fElpaseTime);
	void	Render(cMatrix44 e_Mat,cBaseImage*e_pImage);
	void	RearrangeTime(float e_fTargetTime);

	bool	AddListboxTime(float e_fTime);
	bool	DeleteListboxTime(int e_iIndex);
	bool	ChangeListboxTime(int e_iIndex, float e_fTime);
	bool	SetCurrentListboxTime(int e_iIndex);
	float	GetCurrentListboxTime() { return m_fCurrentMorphingAnimationTime; }
	float	GetEndTime();
};


//do export all triangles and uv and index buffer.