#pragma once



struct sTrianglesToDrawIndicesBuffer
{
	std::vector<Vector3>	vPosVector;
	std::vector<Vector2>	vUVVector;
	std::vector<int>		vIndexVector;
	sTrianglesToDrawIndicesBuffer(class cPuzzleImageUnitTriangulator*e_pPuzzleImageUnitTriangulator);
	//sTrianglesToDrawIndicesBuffer(Vector3*e_pTriangleVertices,Vector2*e_vUVVector,int e_iNumPoints);
	//~sTrianglesToDrawIndicesBuffer();
	bool	ParseVertices(Vector3*e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumPoints);
	void	Render(cMatrix44 e_Mat);

	TiXmlElement*ToTixmlElement();
};


//do export all triangles and uv and index buffer.