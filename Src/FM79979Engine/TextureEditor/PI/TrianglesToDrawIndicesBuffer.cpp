#include "stdafx.h"
#include "TrianglesToDrawIndicesBuffer.h"

//sTrianglesToDrawIndicesBuffer::sTrianglesToDrawIndicesBuffer(Vector3 * e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumPoints)
//{
//
//}

bool sTrianglesToDrawIndicesBuffer::ParseVertices(Vector3 * e_pTriangleVertices, Vector2 * e_vUVVector, int e_iNumPoints)
{
	if (e_iNumPoints % 3)
	{
		FMLog::Log("triangles points count is not correct!", false);
		return false;
	}
	vPosVector.clear();
	vUVVector.clear();
	vIndexVector.clear();
	vIndexVector.reserve(e_iNumPoints);
	vPosVector.reserve(e_iNumPoints);
	vUVVector.reserve(e_iNumPoints);
	const int l_ciTrianglePointsCount = 3;
	int l_iNumTriangles = e_iNumPoints / l_ciTrianglePointsCount;
	int l_iIndex = 0;
	std::map<Vector3, int>	l_PosAndVertexIncexMap;
	for (int i = 0; i < l_iNumTriangles; i++)
	{
		for (int j = 0; j < l_ciTrianglePointsCount; j++)
		{
			int l_iIndex = i * l_ciTrianglePointsCount + j;
			auto l_vPos = e_pTriangleVertices[l_iIndex];
			auto l_Iterator = l_PosAndVertexIncexMap.find(l_vPos);
			if (l_Iterator != l_PosAndVertexIncexMap.end())
			{
				l_PosAndVertexIncexMap[l_vPos] = l_iIndex;
				vPosVector.push_back(l_vPos);
				auto l_vUV = e_vUVVector[l_iIndex];
				vUVVector.push_back(l_vUV);
				vIndexVector.push_back(l_iIndex);
				++l_iIndex;
			}
			else
			{
				vIndexVector.push_back(l_Iterator->second);
			}
		}
	}
	return true;
}

void sTrianglesToDrawIndicesBuffer::Render(cMatrix44 e_Mat)
{
	auto l_iSize = (GLsizei)vIndexVector.size();
	if (l_iSize)
	{
		myGlVertexPointer(3, &vPosVector[0]);
		myGlUVPointer(2,&vUVVector[0]);
		//myGlColorPointer(4, l_f2DColorOne);
		MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)l_iSize, g_iDrawindiceType, &vIndexVector[0]);
	}
}