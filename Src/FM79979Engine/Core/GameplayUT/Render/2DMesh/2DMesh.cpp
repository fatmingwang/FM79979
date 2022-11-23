#include "../../../stdafx.h"
#include "../../../Common/BinaryFile/BinaryFile.h"
#include "2DMesh.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(c2DMeshObjectVector);
	TYPDE_DEFINE_MARCO(c2DMeshObject);
	c2DMeshObject::c2DMeshObject(sMeshBuffer * e_pData, cTexture*e_pTexture)
	{
		m_pBufferReference = nullptr;
		m_pTexture = nullptr;
		m_pBufferReference = nullptr;
		//c2DMeshObject::*f_MyFunction = &c2DMeshObject::SetColor;
		//https://stackoverflow.com/questions/1485983/calling-c-class-methods-via-a-function-pointer
		c2DMeshObjectFunctionPointer l_FunctionPointer = &c2DMeshObject::SetColor;
		c2DMeshObjectFunctionPointer l_FunctionPointer2 = &c2DMeshObject::SetColor;
		c2DMeshObject l_Data(nullptr);
		//l_Data.*f_MyFunction = &c2DMeshObject::SetColor;
		(this->*f_MyFunction)(Vector4::One);
		(l_Data.*l_FunctionPointer)(Vector4::One);
		(l_Data.*l_FunctionPointer2)(Vector4::One);
		//*Method(Vector4::One);
		m_pBufferReference = e_pData;
		if (e_pTexture)
		{
			m_pTexture = e_pTexture;
			m_pTexture->AddRef(this);
		}
	}

	c2DMeshObject::c2DMeshObject(c2DMeshObject * e_p2DMeshObject)
	{
		m_pBufferReference = nullptr;
		m_pTexture = nullptr;
		m_pBufferReference = nullptr;
		if (e_p2DMeshObject)
		{
			m_pBufferReference = e_p2DMeshObject->m_pBufferReference;
			m_pTexture = e_p2DMeshObject->m_pTexture;
			if (m_pTexture)
				m_pTexture->AddRef(this);
		}
	}

	c2DMeshObject::~c2DMeshObject()
	{
		SAFE_RELEASE(m_pTexture, this);
	}

	void c2DMeshObject::SetColor(Vector4 e_vColor)
	{
		if (m_pBufferReference)
		{
			Vector4*l_pColor = (Vector4*)m_pBufferReference->ColorBuffer.pData;
			for (unsigned int i = 0; i < m_pBufferReference->ColorBuffer.uiDataCount; ++i)
				l_pColor[i] = e_vColor;
		}
	}

	void c2DMeshObject::Render()
	{
		if (m_pBufferReference)
		{
			if (m_pTexture)
			{
				m_pTexture->ApplyImage();
			}
			auto l_mat = this->GetWorldTransform();
			RenderVertexByIndexBuffer(l_mat, 3,
				(float*)m_pBufferReference->PosBuffer.pData, (float*)m_pBufferReference->UVBuffer.pData,
				(float*)m_pBufferReference->ColorBuffer.pData, m_pBufferReference->IndexBuffer.pData, m_pBufferReference->IndexBuffer.uiDataCount);
		}
	}

	c2DMeshObject::sMeshBuffer * c2DMeshObject::GetMeshBuffer()
	{
		return m_pBufferReference;
	}

	Vector3 c2DMeshObject::GetCenter()
	{
		if (m_pBufferReference)
			return m_pBufferReference->vPosCenter;
		return Vector3(0,0,0);
	}
	
	void c2DMeshObjectVector::Destroy()
	{
		cNamedTypedObjectVector::Destroy();
		SAFE_RELEASE(m_pMeshBufferMap, this);
	}

	c2DMeshObjectVector::c2DMeshObjectVector(const char*e_strImageName) :cBaseImage(e_strImageName),cSmartObject(nullptr)
	{
		m_pMeshBufferMap = new cMeshBufferMap(this);
		m_pMeshBufferMap->AddRef(this);
	}

	c2DMeshObjectVector::~c2DMeshObjectVector()
	{
		Destroy();
	}

	c2DMeshObjectVector::cMeshBufferMap::cMeshBufferMap(NamedTypedObject * e_pObject):cSmartObject(e_pObject)
	{
	}
	c2DMeshObjectVector::cMeshBufferMap::~cMeshBufferMap()
	{
		DELETE_MAP(m_BufferMap);
	}
	c2DMeshObjectManager::c2DMeshObjectManager()
	{
		m_pFileData = nullptr;
		m_pPIBFile = nullptr;
	}
	c2DMeshObjectManager::~c2DMeshObjectManager()
	{
		SAFE_DELETE(m_pPIBFile);
		Destroy();
	}
	bool c2DMeshObjectManager::MyParse(TiXmlElement * e_pRoot)
	{
		SAFE_DELETE(m_pPIBFile);
		m_pFileData = nullptr;
		m_pCurrent2DMeshObjectVector = nullptr;
		m_pCurrentTexture = nullptr;
		auto l_strImageName = e_pRoot->Attribute(L"ImageName");
		auto l_strPI_tri = e_pRoot->Attribute(CHAR_TO_WCHAR_DEFINE(TI_ELEMENT_NAME));
		if (l_strPI_tri)
		{//parse binary data.
			SAFE_DELETE(m_pPIBFile);
			m_pPIBFile = new cBinaryFile();
			std::string l_strPRIFileName = m_strCurrentDirectory;
			l_strPRIFileName += ValueToString(l_strPI_tri);
			if (m_pPIBFile->Openfile(l_strPRIFileName.c_str(),"rb"))
			{
				m_pFileData = (char*)m_pPIBFile->GetDataFile(0);
			}
		}
		if (!m_pFileData)
		{
			SAFE_DELETE(m_pPIBFile);
			return false;
		}
		c2DMeshObjectVector*l_pCurrent2DMeshObjectVector = nullptr;
		if (l_strImageName)
		{
			std::string l_strTextureFileName = m_strCurrentDirectory;
			l_strTextureFileName += ValueToString(l_strImageName);
			l_pCurrent2DMeshObjectVector = new c2DMeshObjectVector(l_strTextureFileName.c_str());
			l_pCurrent2DMeshObjectVector->AddRef(this);
			m_pCurrentTexture = l_pCurrent2DMeshObjectVector->GetTexture();
			if(!m_pCurrentTexture || m_pCurrentTexture->GetImageIndex() == -1)
			{
				SAFE_DELETE(l_pCurrent2DMeshObjectVector);
				return false;
			}
		}
		this->AddObjectNeglectExist(l_pCurrent2DMeshObjectVector);
		m_pCurrent2DMeshObjectVector = l_pCurrent2DMeshObjectVector;
		e_pRoot = e_pRoot->FirstChildElement();
		while (e_pRoot)
		{
			auto l_strValue = e_pRoot->Value();
			if (!wcscmp(l_strValue, L"PuzzleUnit"))
			{
				auto l_strPIUnitName = e_pRoot->Attribute(L"Name");
				auto l_pTriangleElement = e_pRoot->FirstChildElement();
				if (l_pTriangleElement && l_strPIUnitName)
				{
					l_strValue = l_pTriangleElement->Value();
					if (!wcscmp(l_strValue, L"sTrianglesToDrawIndicesBuffer"))
					{
						if (!ProcessPIUnitForTriangleData(l_pTriangleElement, l_strPIUnitName))
						{
							FMLog::Log(UT::ComposeMsgByFormat(L"ProcessPIUnitForTriangleData:%ls failed", l_strPIUnitName).c_str(), true);
						}
					}
				}
			}
			e_pRoot = e_pRoot->NextSiblingElement();
		}
		SAFE_DELETE(m_pPIBFile);
		return true;
	}
	//sTrianglesToDrawIndicesBuffer
	//<sTrianglesToDrawIndicesBuffer IndexBufferCount="6" IndexBufferBinarySize="24" VertexBufferCount="4" PosBufferBinarySize="48" UVBufferBinarySize="32" />
	bool c2DMeshObjectManager::ProcessPIUnitForTriangleData(TiXmlElement * e_pRoot, const wchar_t*e_strName)
	{
		auto l_strIndexBufferCount = e_pRoot->Attribute(L"IndexBufferCount");
		auto l_strIndexBufferBinarySize = e_pRoot->Attribute(L"IndexBufferBinarySize");
		auto l_strVertexBufferCount = e_pRoot->Attribute(L"VertexBufferCount");
		auto l_strPosBufferBinarySize = e_pRoot->Attribute(L"PosBufferBinarySize");
		auto l_strUVBufferBinarySize = e_pRoot->Attribute(L"UVBufferBinarySize");
		if (l_strIndexBufferCount &&
			l_strIndexBufferBinarySize &&
			l_strVertexBufferCount &&
			l_strPosBufferBinarySize &&l_strUVBufferBinarySize)
		{
			int l_iIndexBufferCount = GetInt(l_strIndexBufferCount);
			int l_iIndexBufferBinarySize = GetInt(l_strIndexBufferBinarySize);
			int l_iVertexBufferCount = GetInt(l_strVertexBufferCount);
			int l_iPosBufferBinarySize = GetInt(l_strPosBufferBinarySize);
			int l_iUVBufferBinarySize = GetInt(l_strUVBufferBinarySize);
			int l_iSingleIndexBufferSize = l_iIndexBufferBinarySize / l_iIndexBufferCount;
			eDataType l_DataType = eDataType::eDT_INT;
			switch (l_iSingleIndexBufferSize)
			{
				case sizeof(int) :
					l_DataType = eDataType::eDT_INT;
				break;
				case sizeof(char) :
					l_DataType = eDataType::eDT_BYTE;
				break;
				case sizeof(short) :
					l_DataType = eDataType::eDT_SHORT;
				break;
				default:
					l_DataType = eDataType::eDT_MAX;
				break;
			}
			if (l_DataType == eDataType::eDT_MAX)
				return false;
			c2DMeshObject::sMeshBuffer*l_p2DMeshBuffer = new c2DMeshObject::sMeshBuffer();
			l_p2DMeshBuffer->IndexBuffer.CopyData(m_pFileData, l_iIndexBufferBinarySize, l_DataType, l_iIndexBufferCount);
			m_pFileData += l_iIndexBufferBinarySize;
			l_p2DMeshBuffer->PosBuffer.CopyData(m_pFileData, l_iPosBufferBinarySize, eDataType::eDT_VECTOR3, l_iVertexBufferCount);
			l_p2DMeshBuffer->CalculatePosCenter();
			m_pFileData += l_iPosBufferBinarySize;
			l_p2DMeshBuffer->UVBuffer.CopyData(m_pFileData, l_iUVBufferBinarySize, eDataType::eDT_VECTOR2, l_iVertexBufferCount);
			l_p2DMeshBuffer->ColorBuffer.CreateData(sizeof(Vector4)*l_iVertexBufferCount, eDataType::eDT_VECTOR4, l_iVertexBufferCount);
			l_p2DMeshBuffer->pTargetTexture = this->m_pCurrentTexture;
			Vector4*l_pColor = (Vector4*)l_p2DMeshBuffer->ColorBuffer.pData;
			for (int i = 0; i < l_iVertexBufferCount; ++i)
				l_pColor[i] = Vector4::One;
			c2DMeshObject*l_p2DMeshObject = new c2DMeshObject(l_p2DMeshBuffer, m_pCurrentTexture);
			l_p2DMeshObject->SetName(e_strName);
			m_pCurrent2DMeshObjectVector->m_pMeshBufferMap->m_BufferMap.insert(std::make_pair(l_p2DMeshObject, l_p2DMeshBuffer));
			return 	m_pCurrent2DMeshObjectVector->AddObjectNeglectExist(l_p2DMeshObject);;
		}
		return false;
	}
	void c2DMeshObjectManager::Destroy()
	{
		auto l_iCount = this->Count();
		for (int i = 0; i < l_iCount; ++i)
		{
			auto l_pObject = this->m_ObjectList[i];
			SAFE_RELEASE(l_pObject, this);
		}
		this->m_ObjectList.clear();
	}
	NamedTypedObject* c2DMeshObjectManager::GetObjectByFileName(const char * e_strFileName)
	{
		NamedTypedObject*l_pObject = cNamedTypedObjectVector::GetObjectByFileName(e_strFileName);
		if (l_pObject)
			return l_pObject;
		if (this->ParseWithMyParse(e_strFileName))
		{
			return cNamedTypedObjectVector::GetObjectByFileName(e_strFileName);
		}
		return nullptr;
	}
	void c2DMeshObject::sMeshBuffer::CalculatePosCenter()
	{
		vPosCenter = Vector3::Zero;
		Vector3 l_vTotalPos = Vector3::Zero;
		Vector3* l_pvPos = (Vector3*)PosBuffer.pData;
		for (unsigned int i = 0; i < PosBuffer.uiDataCount; ++i)
		{
			l_vTotalPos += l_pvPos[i];
		}
		vPosCenter.x = l_vTotalPos.x / PosBuffer.uiDataCount;
		vPosCenter.y = l_vTotalPos.y / PosBuffer.uiDataCount;
		vPosCenter.z = l_vTotalPos.z / PosBuffer.uiDataCount;
	}
	void c2DMeshObject::sMeshBuffer::SetOriginalPointAtVertexCenter()
	{
		if (vPosCenter.x != 0 ||
			vPosCenter.y != 0 ||
			vPosCenter.z != 0)
		{
			Vector3* l_pvPos = (Vector3*)PosBuffer.pData;
			for (unsigned int i = 0; i < PosBuffer.uiDataCount; ++i)
			{
				l_pvPos[i] -= vPosCenter;
			}
		}
	}
	//namespace FATMING_CORE
}