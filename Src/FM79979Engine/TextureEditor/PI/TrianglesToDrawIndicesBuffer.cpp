#include "stdafx.h"
#include "TrianglesToDrawIndicesBuffer.h"
#include "PuzzleImageUnitTriangulator.h"
extern cGlyphFontRender*g_pDebugFont;
//sTrianglesToDrawIndicesBuffer::sTrianglesToDrawIndicesBuffer(Vector3 * e_pTriangleVertices, Vector2*e_vUVVector, int e_iNumPoints)
//{
//
//}

int sTrianglesToDrawIndicesBuffer::GetVertexIndexByPos(Vector3 e_vPos)
{
	for (int i = 0; i < (int)vPosVector.size(); ++i)
	{
		auto l_vPos = vPosVector[i];
		if (l_vPos.x == e_vPos.x &&l_vPos.y == e_vPos.y &&l_vPos.z == e_vPos.z)
		{
			return i;
		}
	}
	return -1;
}

int sTrianglesToDrawIndicesBuffer::FinClosestVertexIndex(Vector3 e_vPos)
{
	return GetClosestPointIndex(e_vPos,&vPosVector);
}

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

void sTrianglesToDrawIndicesBuffer::ConvertUVDataToPISpace(Vector2 e_vPISize,Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector2>& e_ExportUVVector)
{
	Vector2 l_vStartUV(e_vImagePos.x / e_vPISize.x, e_vImagePos.y / e_vPISize.y);
	Vector2 l_vEndUV((e_vImagePos.x + e_vTextureSize.x) / e_vPISize.x, (e_vImagePos.y + e_vTextureSize.y) / e_vPISize.y);
	Vector2 l_vThisTextureUVRange = l_vEndUV - l_vStartUV;
	auto l_uiSize = this->vUVVector.size();
	e_ExportUVVector.resize(l_uiSize);
	for (size_t i = 0; i < l_uiSize; i++)
	{
		e_ExportUVVector[i].x = vUVVector[i].x*l_vThisTextureUVRange.x + l_vStartUV.x;
		e_ExportUVVector[i].y = vUVVector[i].y*l_vThisTextureUVRange.y + l_vStartUV.y;
	}
}

TiXmlElement * sTrianglesToDrawIndicesBuffer::ToTixmlElement(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos)
{
	TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"sTrianglesToDrawIndicesBuffer");
	std::vector<Vector2>l_ExportUVVector;
	ConvertUVDataToPISpace(e_vPISize, e_vTextureSize, e_vImagePos, l_ExportUVVector);
	auto l_strPosVector = ValueToStringW(vPosVector);
	auto l_strIndexVector = ValueToStringW(vIndexVector);
	auto l_strUVVector = ValueToStringW(l_ExportUVVector);
	l_pTiXmlElement->SetAttribute(L"IndexBuffer", l_strIndexVector.c_str());
	l_pTiXmlElement->SetAttribute(L"UVBuffer", l_strUVVector.c_str());
	l_pTiXmlElement->SetAttribute(L"PosBuffer", l_strPosVector.c_str());
	return l_pTiXmlElement;
}

TiXmlElement * sTrianglesToDrawIndicesBuffer::ToTixmlElementWithBinaryData(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector3>& e_PosVector, std::vector<Vector2>& e_UVVector, std::vector<int>& e_iIndexBufferVector, Vector2 e_vCenterOffset)
{
	if (GetBinaryData(e_vPISize, e_vTextureSize, e_vImagePos, e_PosVector, e_UVVector, e_iIndexBufferVector))
	{
		TiXmlElement*l_pTiXmlElement = new TiXmlElement(L"sTrianglesToDrawIndicesBuffer");
		std::vector<Vector2>l_ExportUVVector;
		auto l_strPosVector = ValueToStringW(vPosVector);
		auto l_strIndexVector = ValueToStringW(vIndexVector);
		auto l_strUVVector = ValueToStringW(l_ExportUVVector);
		if (e_vCenterOffset.x != 0.f || e_vCenterOffset.y != 0.f)
			l_pTiXmlElement->SetAttribute(L"CenterOffset", ValueToStringW(e_vCenterOffset));
		l_pTiXmlElement->SetAttribute(L"IndexBufferCount", (int)vIndexVector.size());
		l_pTiXmlElement->SetAttribute(L"VertexBufferCount", (int)l_strPosVector.size());
		l_pTiXmlElement->SetAttribute(L"PosBufferBinarySize", (int)(l_strPosVector.size()*sizeof(Vector3)));
		l_pTiXmlElement->SetAttribute(L"UVBufferBinarySize", (int)(l_ExportUVVector.size() * sizeof(Vector2)));
		return l_pTiXmlElement;
	}
	return nullptr;
}

bool sTrianglesToDrawIndicesBuffer::ToTixmlElementWithBinaryData(ATG::XMLWriter * e_pXMLWriter, Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos, std::vector<Vector3>& e_PosVector, std::vector<Vector2>& e_UVVector, std::vector<int>& e_iIndexBufferVector,Vector2 e_vCenterOffset)
{
	if (GetBinaryData(e_vPISize, e_vTextureSize, e_vImagePos, e_PosVector, e_UVVector, e_iIndexBufferVector))
	{
		e_pXMLWriter->StartElement(L"sTrianglesToDrawIndicesBuffer");
		if(e_vCenterOffset.x != 0.f || e_vCenterOffset.y != 0.f)
			e_pXMLWriter->AddAttribute("CenterOffset",ValueToStringW(e_vCenterOffset));
		e_pXMLWriter->AddAttribute("IndexBufferCount", (int)vIndexVector.size());
		e_pXMLWriter->AddAttribute("IndexBufferBinarySize", (int)(e_iIndexBufferVector.size() * sizeof(int)));
		e_pXMLWriter->AddAttribute("VertexBufferCount", (int)e_PosVector.size());
		e_pXMLWriter->AddAttribute("PosBufferBinarySize", (int)(e_PosVector.size() * sizeof(Vector3)));
		e_pXMLWriter->AddAttribute("UVBufferBinarySize", (int)(e_UVVector.size() * sizeof(Vector2)));
		e_pXMLWriter->EndElement();
		return true;
	}
	return false;
}

bool sTrianglesToDrawIndicesBuffer::GetBinaryData(Vector2 e_vPISize, Vector2 e_vTextureSize, Vector2 e_vImagePos,
	std::vector<Vector3>&e_PosVector, std::vector<Vector2>&e_UVVector, std::vector<int>&e_iIndexBufferVector)
{
	ConvertUVDataToPISpace(e_vPISize, e_vTextureSize, e_vImagePos, e_UVVector);
	e_PosVector = vPosVector;
	e_iIndexBufferVector = vIndexVector;
	return true;
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