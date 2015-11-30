#ifndef MESH_H
#define MESH_H
#include "FatmingEffectData.h"

class	sTechniqueAndTexture:public cBaseImage
{
	GET_SET_DEC(eTechniqueType,m_eTechniqueType,GetTechniqueType,SetTechniqueType);
public:
	sTechniqueAndTexture(cBaseImage*e_pBaseImage,eTechniqueType e_eTechniqueType);
	~sTechniqueAndTexture();
};
//=================
//gather effect paramater and call mesh index,and texture with technique
//maybe we should keep vertices data inside
//before u delete this u have to ensure the texture the mesh used that 
//is remove from texture resource storeage.
//=================
//class	cMesh:public Frame,public cNamedTypedObjectVector<sTechniqueAndTexture>
//{
//
//	//
//	GET_SET_DEC(GLuint,m_uiMeshIndex,GetMeshIndex,SetMeshIndex);
//public:
//	cMesh();
//	~cMesh();
//	//currently we do not shader effect,so we just render diffuse as need
//	void	Render();
//};

//===================
//if mesh contain skelton data,the influnce blending bones would be 4.
//ensure u have enough skinning knowledge to fetch the vertex buffer data.
//just take a glance at FatmingGeometryData.h
//===================
class	cMesh:public Frame,public cNamedTypedObjectVector<sTechniqueAndTexture>
{
	//for 2D
	Vector4	m_v2DCollisionRect;
	//the collada output ID
	GET_SET_DEC(std::wstring,m_strGeoID,GetGeoID,SetGeoID);
	//
	GET_SET_DEC(bool,m_bShadowEffect,IsShadowEffect,SetShadowEffect);
	//friend class cMesh;
protected:
	cVBOBuffer*m_pVBOBuffer;
	//
	UINT	m_uiFVF_Format;
	//
	int		m_iOneVertexStride;		//the vertex data stride,total vertex data size is m_iOneVertexStride*m_uiVertexBufferCount;
	//float	*m_pfVertexBuffer;		//ensure the data size could be char or short int or float depen on FVF(basicly color is 4 char,skinning blending bone index is char)
	float*	m_ppfVerticesBuffer[TOTAL_FVF];//the data for bone id usually is 4 unsigned byte to become 1 float.if the bone ID is bigger than 512,u have to ensure data swap problem
	UINT	m_uiVertexBufferCount;
	bool	m_bVerticesBuffer[TOTAL_FVF];
	//
#ifdef WIN32
	UINT	*m_puiIndexBuffer;
#else
	unsigned short	*m_puiIndexBuffer;
#endif
	UINT	m_uiIndexBufferCount;//m_uiIndexBufferCount/3 equal num triangle to draw.
	sSphere	m_Sphere;
public:
	DEFINE_TYPE_INFO();
	cMesh(float*	e_ppfVertexBuffer[TOTAL_FVF],
		UINT	e_uiVertexBufferCount,
		UINT*	e_puiIndexBuffer,
		UINT	e_uiIndexBufferCount);
	cMesh(cMesh*e_pMesh);
	CLONE_MYSELF(cMesh);
	~cMesh();
	virtual	void	Render(WCHAR*e_strShaderName = STATIC_MESH_SHADER);
	bool	IsSkinningDataContain(){  return m_uiFVF_Format&FVF_SKINNING_WEIGHT?true:false; }
	void	AssignSkinningData(float*e_pfWeightsData,float*e_pfInflunceJointsData,UINT e_uiVertexCount);
	//the new diffuse texture should come from the resource,because mesh won't delete the texture
	void	ChangeDiffuseImage(cBaseImage*e_pBaseImage);
	sSphere	GetSphere(){return m_Sphere;}
	void	SetSphere(sSphere e_sSphere){m_Sphere = e_sSphere;}
	//for sphere and the matrix from visual scene node
	void	ApplyNodeMatrix(cMatrix44 e_Mat);
	Vector4	Get2DCollisionRect(){return m_v2DCollisionRect;}
	class cTriangles:public cNamedTypedObjectVector<sTechniqueAndTexture>
	{
		UINT	*m_puiIndexBuffer;
		cVBOBuffer*m_pVBOBuffer;
		int	m_iStartIndex;
		int	m_iIndexCount;
	public:
		cTriangles(){this->SetFromResource(true);}
		~cTriangles();
		virtual	void	Render();
	};
	static	cMatrix44	m_smatAxisTransform;//max is Z up
};

#endif