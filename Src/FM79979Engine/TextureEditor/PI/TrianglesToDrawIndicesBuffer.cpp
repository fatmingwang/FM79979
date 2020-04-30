#include "stdafx.h"
#include "TrianglesToDrawIndicesBuffer.h"
#include "PuzzleImageUnitTriangulator.h"
extern cGlyphFontRender*g_pDebugFont;
//sTrianglesToDrawIndicesBuffer::sTrianglesToDrawIndicesBuffer(Vector3 * e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumPoints)
//{
//
//}

sTrianglesToDrawIndicesBuffer::sTrianglesToDrawIndicesBuffer()
{
	//if (e_pPuzzleImageUnitTriangulator->m_s2DVertex.vPosVector.size())
	//{
	//	std::vector<cPuzzleImageUnitTriangulator::s2DVertex::s3PosPoints>*	l_pPosVector	= &e_pPuzzleImageUnitTriangulator->m_s2DVertex.vPosVector;
	//	std::vector<cPuzzleImageUnitTriangulator::s2DVertex::s3UVPoints>*	l_pvUVVector	= &e_pPuzzleImageUnitTriangulator->m_s2DVertex.vUVVector;
	//	std::vector<cPuzzleImageUnitTriangulator::s2DVertex::s3ColorPoints>*l_pColorVector	= &e_pPuzzleImageUnitTriangulator->m_s2DVertex.vColorVector;
	//	//ParseVertices(Vector3 * e_pTriangleVertices, Vector2 * e_vUVVector, int e_iNumPoints)
	//}
}

bool sTrianglesToDrawIndicesBuffer::ParseVertices(Vector3 * e_pTriangleVertices, Vector2 * e_vUVVector, int e_iNumTriangles)
{
	vColorVector.clear();
	vPosVector.clear();
	vUVVector.clear();
	vIndexVector.clear();
	const int l_ciTrianglePointsCount = 3;
	int l_iNumTriangles = e_iNumTriangles;
	vIndexVector.reserve(e_iNumTriangles * l_ciTrianglePointsCount);
	vPosVector.reserve(e_iNumTriangles * l_ciTrianglePointsCount);
	vUVVector.reserve(e_iNumTriangles * l_ciTrianglePointsCount);
	vColorVector.reserve(e_iNumTriangles * l_ciTrianglePointsCount);
	int l_iDrawBufferIndex = 0;
	for (int i = 0; i < l_iNumTriangles; i++)
	{
		for (int j = 0; j < l_ciTrianglePointsCount; j++)
		{
			int l_iIndex = i * l_ciTrianglePointsCount + j;
			auto l_vPos = e_pTriangleVertices[l_iIndex];
			int l_iVertexIndex = IsVectorContain(vPosVector, l_vPos);
			if (l_iVertexIndex == -1)
			{
				auto l_vUV = e_vUVVector[l_iIndex];
				vPosVector.push_back(l_vPos);
				vUVVector.push_back(l_vUV);
				vColorVector.push_back(Vector4::One);
				vIndexVector.push_back(l_iDrawBufferIndex);
				++l_iDrawBufferIndex;
			}
			else
			{
				vIndexVector.push_back(l_iVertexIndex);
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
		RenderVertexByIndexBuffer(e_Mat,3,(float*)&vPosVector[0], (float*)&vUVVector[0], (float*)&vColorVector[0], (float*)&vIndexVector[0],(int)l_iSize);
	}
}

void sTrianglesToDrawIndicesBuffer::RenderInfo(cMatrix44 e_Mat)
{
	if (g_pDebugFont)
	{
		std::wstring l_strInfo = UT::ComposeMsgByFormat(L"Vertex:%d,IndexCount:%d", (int)vPosVector.size(), (int)vIndexVector.size());
		auto l_vPos = e_Mat.GetTranslation();
		g_pDebugFont->RenderFont(l_vPos.x, l_vPos.y, l_strInfo.c_str());
	}
}

TiXmlElement * sTrianglesToDrawIndicesBuffer::ToTixmlElement(Vector2 e_vTextureSize, Vector2 e_vImagePos, char * e_pBinaryData)
{
	Vector2 l_vScale(e_vImagePos.x / e_vTextureSize.x, e_vImagePos.y / e_vTextureSize.y);
	this->vUVVector.size();
	return nullptr;
}

TiXmlElement * sTrianglesToDrawIndicesBuffer::ToTixmlElement(Vector2 e_vTextureSize, Vector2 e_vImagePos)
{
	return nullptr;
}

int IsVectorContain(std::vector<Vector3>& e_vVector, Vector3 e_Value)
{
	auto l_iSize = (int)e_vVector.size();
	for (int i = 0; i < l_iSize; i++)
	{
		auto l_pVector3 = &e_vVector[i];
		if (l_pVector3->x == e_Value.x &&
			l_pVector3->y == e_Value.y &&
			l_pVector3->z == e_Value.z)
			return i;
	}
	return -1;
}