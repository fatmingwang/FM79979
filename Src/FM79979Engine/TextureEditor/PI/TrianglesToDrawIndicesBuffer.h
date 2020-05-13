#pragma once


int	IsVectorContain(std::vector<Vector3>&e_vVector, Vector3 e_Value);

struct sTrianglesToDrawIndicesBuffer
{
	std::vector<Vector4>	vColorVector;
	std::vector<Vector3>	vPosVector;
	std::vector<Vector2>	vUVVector;
	std::vector<int>		vIndexVector;
	int						GetVertexIndexByPos(Vector3 e_vPos);
	sTrianglesToDrawIndicesBuffer();
	//sTrianglesToDrawIndicesBuffer(Vector3*e_pTriangleVertices,Vector2*e_vUVVector,int e_iNumPoints);
	//~sTrianglesToDrawIndicesBuffer();
	bool			ParseVertices(Vector3*e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumTriangles);
	void			Render(cMatrix44 e_Mat);
	void			RenderInfo(cMatrix44 e_Mat);
	//UV for PISpace
	bool			GetBinaryData(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector);
	//xml description position and uv xml
	void			ConvertUVDataToPISpace(Vector2 e_vPISize,Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector2>&e_ExportUVVector);
	TiXmlElement*	ToTixmlElement(Vector2 e_vPISize,Vector2 e_vTextureSize,Vector2 e_vImagePos);
	TiXmlElement*	ToTixmlElementWithBinaryData(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector);
	bool	ToTixmlElementWithBinaryData(ATG::XMLWriter*e_pXMLWriter,Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector);

};



class cEditor_MorphingAnimation:public NamedTypedObject
{
	sTrianglesToDrawIndicesBuffer*	m_pTarget;
	//
	struct sVertexIndexAndPositionAndTimeVector
	{
		sVertexIndexAndPositionAndTimeVector();
		typedef std::map<float, Vector3> FloatTocVector3Map;
		//relasted to vMorphingPosVector
		Vector3*			pPos;
		int					iVertexIndex;
		//time and 
		FloatTocVector3Map m_FormKeyFrames;
		Vector3	UpdateAnimationByGlobalTime(float e_fGlobalTime);
		void	AssignPositionPointer(Vector3*e_pAnimationPositionTarget,int e_iVertexIndex);
		void	AddData(Vector3 e_vPos,float   e_fTime);
		bool	DeleteData(float   e_fTime);
		bool	ChangeData(int e_iDataIndex,Vector3 e_vPos, float   e_fTime);
	};
	//
	//from sTrianglesToDrawIndicesBuffer::vPosVector
	std::vector<Vector3>			vMorphingPosVector;
	////count and order should be same as sTrianglesToDrawIndicesBuffer::vPosVector
	std::vector<sVertexIndexAndPositionAndTimeVector>	m_VertexAnimationVector;
	//std::map<float, std::vector<sMorphingData>>	m_TimeAndVertexPosVectorData;
	//if user add or delete triangle vertices,the index could be changed,so here need to reassign vertex index again
	void	ReassignVertexIndexData(std::vector<int> e_iOldVertexIndexVector, std::vector<int> e_iNewVertexIndexVector);
	void	DeleteVertexIndexData(int e_iVertexIndex);
	void	VertexMove(int e_iVertexIndex, Vector3 e_vPos);
};


//do export all triangles and uv and index buffer.