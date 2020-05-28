#pragma once

#define	TRIANGLE_AND_DRAW_INDEX_EXTENSION_FILE_NAME	"ti"
#define	TI_ELEMENT_NAME								"PI_tri"

namespace FATMING_CORE
{
	class c2DMeshObject :public cRenderObject
	{
	public:
		struct sMeshBuffer
		{
			sDataContainer	IndexBuffer;
			sDataContainer	PosBuffer;
			sDataContainer	UVBuffer;
			sDataContainer	ColorBuffer;
		};
	protected:
		sMeshBuffer*		m_pBufferReference;
		cTexture*			m_pTexture;
	public:
		c2DMeshObject(struct sMeshBuffer*e_pData,cTexture*e_pTexture);
		c2DMeshObject(c2DMeshObject*e_p2DMeshObject);
		virtual	void	SetColor(Vector4 e_vColor);
		virtual ~c2DMeshObject();
		CLONE_MYSELF(c2DMeshObject);
		DEFINE_TYPE_INFO();
		virtual	void	Render();
	};

	class c2DMeshObjectManager :public cNodeISAX, public cNamedTypedObjectVector<c2DMeshObject>, public cBaseImage , public cSmartObject
	{
		class cMeshBufferMap:public cSmartObject
		{
		public:
			cMeshBufferMap(NamedTypedObject*e_pObject);
			virtual ~cMeshBufferMap();
			std::map<c2DMeshObject*, c2DMeshObject::sMeshBuffer*> m_BufferMap;
		};
		cMeshBufferMap*	m_pMeshBufferMap;
		cBinaryFile*	m_pPIBFile;
		char*			m_pFileData;
		//cTexture
		virtual	bool	MyParse(TiXmlElement*e_pRoot)override;
		bool			ProcessPIUnitForTriangleData(TiXmlElement*e_pRoot, const wchar_t*e_strName);
	public:
		c2DMeshObjectManager();
		~c2DMeshObjectManager();
		CLONE_RETURN_NULLPTR();
		DEFINE_TYPE_INFO();
		virtual NamedTypedObject* GetObjectByFileName(const char*e_strFileName)override;
	};
//namespace FATMING_CORE
}