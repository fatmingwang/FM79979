#pragma once


int	IsVectorContain(std::vector<Vector3>&e_vVector, Vector3 e_Value);

struct sTrianglesToDrawIndicesBuffer
{
	std::vector<Vector4>	vColorVector;
	std::vector<Vector3>	vPosVector;
	std::vector<Vector2>	vUVVector;
	std::vector<int>		vIndexVector;
	int						GetVertexIndexByPos(Vector3 e_vPos);
	int						FinClosestVertexIndex(Vector3 e_vPos);
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
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector,Vector2 e_vCenterOffset);
	bool	ToTixmlElementWithBinaryData(ATG::XMLWriter*e_pXMLWriter,Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector, Vector2 e_vCenterOffset);

};