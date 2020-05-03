#pragma once



struct sTrianglesToDrawIndicesBuffer
{
	std::vector<Vector4>	vColorVector;
	std::vector<Vector3>	vPosVector;
	std::vector<Vector2>	vUVVector;
	std::vector<int>		vIndexVector;
	sTrianglesToDrawIndicesBuffer();
	//sTrianglesToDrawIndicesBuffer(Vector3*e_pTriangleVertices,Vector2*e_vUVVector,int e_iNumPoints);
	//~sTrianglesToDrawIndicesBuffer();
	bool			ParseVertices(Vector3*e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumTriangles);
	void			Render(cMatrix44 e_Mat);
	void			RenderInfo(cMatrix44 e_Mat);
	//xml description position and uv xml
	void			ConvertUVDataToPISpace(Vector2 e_vPISize,Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector2>&e_ExportUVVector);
	TiXmlElement*	ToTixmlElement(Vector2 e_vPISize,Vector2 e_vTextureSize,Vector2 e_vImagePos);
	bool			GetBinaryData(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
		std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector);

};
int	IsVectorContain(std::vector<Vector3>&e_vVector,Vector3 e_Value);

//do export all triangles and uv and index buffer.