#include "StdAfx.h"
#include "Mesh.h"
#include "FatmingController.h"
//ehre could be get wrong bec
//from cMAtrix44.cpp
float MY_ZupToYUp[] = { 1, 0, 0, 0,
							  0, 0, -1, 0,
							  0, 1, 0, 0,
							  0, 0, 0, 1 };
cMatrix44	cMesh::m_smatAxisTransform = cMatrix44(MY_ZupToYUp);



sTechniqueAndTexture::sTechniqueAndTexture(cBaseImage*e_pBaseImage,eTechniqueType e_eTechniqueType):cBaseImage(e_pBaseImage)
{
	m_eTechniqueType = e_eTechniqueType;
}

sTechniqueAndTexture::~sTechniqueAndTexture()
{

}

TYPDE_DEFINE_MARCO(cMesh);

cMesh::cMesh(float*	e_ppfVertexBuffer[TOTAL_FVF],
		UINT	e_uiVertexBufferCount,
		UINT*	e_puiIndexBuffer,
		UINT	e_uiIndexBufferCount)
{
	//m_smatAxisTransform = cMatrix44::ZupToYUp;
	m_pVBOBuffer = nullptr;
	m_bShadowEffect = false;
	memset(m_v2DCollisionRect,0,sizeof(Vector4));
	m_Sphere.vCenter = Vector3::Zero;
	memset(m_bVerticesBuffer,0,sizeof(bool)*TOTAL_FVF);
	m_uiVertexBufferCount = e_uiVertexBufferCount;
	//fuck,because animation need position info
	//cOpenGLRender::m_sbVBOSupported = true;
	//cOpenGLRender::m_sbVBOSupported = false;	
	//cGameApp::OutputDebugInfoString(L"FUCK!!!\n");
	//cGameApp::OutputDebugInfoString(L"glGet GL_ARRAY_BUFFER_BINDING,GL_ELEMENT_ARRAY_BUFFER_BINDING,before get buffer data u have to \
	//				   glBindBuffer(GL_ARRAY_BUFFER, l_uiBufferID);");
	//
	if (cOpenGLRender::m_sbVBOSupported)
	{
		m_pVBOBuffer = new FATMING_CORE::cVBOBuffer();
	}
	for(int i=0;i<TOTAL_FVF;++i)
	{
		m_ppfVerticesBuffer[i] = 0;
		if(e_ppfVertexBuffer[i])
		{
			m_uiFVF_Format|=(1<<i);
			if( i == FVF_POS )
			{
				Vector3	l_vMin(FLT_MAX,FLT_MAX,FLT_MAX);
				Vector3	l_vMax(FLT_MIN,FLT_MIN,FLT_MIN);
				Vector3*l_pvAllVertex = (Vector3*)e_ppfVertexBuffer[i];
				if( l_pvAllVertex )
				{
					for(UINT i=0;i<this->m_uiVertexBufferCount;++i)
					{
						if(l_vMin.x>l_pvAllVertex[i].x)	l_vMin.x = l_pvAllVertex[i].x;
						if(l_vMin.y>l_pvAllVertex[i].y)	l_vMin.y = l_pvAllVertex[i].y;
						if(l_vMin.z>l_pvAllVertex[i].z)	l_vMin.z = l_pvAllVertex[i].z;
						if(l_vMax.x<l_pvAllVertex[i].x)	
							l_vMax.x = l_pvAllVertex[i].x;
						if(l_vMax.y<l_pvAllVertex[i].y)	
							l_vMax.y = l_pvAllVertex[i].y;
						if(l_vMax.z<l_pvAllVertex[i].z)	
							l_vMax.z = l_pvAllVertex[i].z;
					}
					m_v2DCollisionRect.x = l_vMin.x;
					m_v2DCollisionRect.y = l_vMin.y;
					m_v2DCollisionRect.z = l_vMax.x;
					m_v2DCollisionRect.w = l_vMax.y;
					m_Sphere.vCenter = (l_vMax-l_vMin)/2;
					m_Sphere.vCenter = cMatrix44::ZupToYUp.TransformVector(m_Sphere.vCenter);
					m_Sphere.fRadius = (l_vMax-l_vMin).Length()/2;
				}
			}
			if( m_pVBOBuffer )
			{
				m_pVBOBuffer->SetupVerticesBuffer(e_ppfVertexBuffer[i],i,g_iFVF_DataStride[i],e_uiVertexBufferCount);
			}
			else
			{
				UINT	l_uiSize = g_iFVF_DataStride[i]*e_uiVertexBufferCount;
				m_ppfVerticesBuffer[i] = e_ppfVertexBuffer[i];
				e_ppfVertexBuffer[i] = 0;
				//m_ppfVerticesBuffer[i] = new float[l_uiSize];
				//memcpy(m_ppfVerticesBuffer[i],e_ppfVertexBuffer[i],sizeof(float)*l_uiSize);
			}
			m_bVerticesBuffer[i] = true;
		}
	}
	assert(m_uiFVF_Format|(1<<0));//it must have position data
	m_uiIndexBufferCount = e_uiIndexBufferCount;
	if( m_pVBOBuffer )
	{
#ifdef WIN32
		m_pVBOBuffer->SetupIndicesBuffer((float*)e_puiIndexBuffer,m_uiIndexBufferCount);
#else
		//opengl es 2only support unsigned short index
		if (cOpenGLRender::m_siOpenGLESVersion < OPENGL_ES_30)
		{
			unsigned short* l_pusData = new unsigned short[e_uiIndexBufferCount];
			for (UINT i = 0; i < e_uiIndexBufferCount; ++i)
			{
				l_pusData[i] = (unsigned short)e_puiIndexBuffer[i];
			}
			m_pVBOBuffer->SetupIndicesBuffer((float*)l_pusData, m_uiIndexBufferCount,sizeof(unsigned short));
			delete[] l_pusData;
		}
		else
		{
			m_pVBOBuffer->SetupIndicesBuffer((float*)e_puiIndexBuffer,m_uiIndexBufferCount, sizeof(unsigned int));
		}
#endif
		
		m_puiIndexBuffer = 0;
	}
	else
	{
#ifdef WIN32
		m_puiIndexBuffer = new UINT[e_uiIndexBufferCount];
		memcpy(m_puiIndexBuffer,e_puiIndexBuffer,sizeof(UINT)*e_uiIndexBufferCount);
#else //opengl es only support unsigned short
		unsigned short *l_pusData = new unsigned short[e_uiIndexBufferCount];
		for( UINT i=0;i<e_uiIndexBufferCount;++i )
		{
			l_pusData[i] = (unsigned short)e_puiIndexBuffer[i];
		}
		m_puiIndexBuffer = (unsigned short*)l_pusData;
#endif
	}
}

cMesh::cMesh(cMesh*e_pMesh)
{
	m_bShadowEffect = e_pMesh->m_bShadowEffect;
	if( e_pMesh )
	{
		SetName(e_pMesh->GetName());
		memcpy(m_bVerticesBuffer,e_pMesh->m_bVerticesBuffer,sizeof(bool)*TOTAL_FVF);
		m_v2DCollisionRect = e_pMesh->m_v2DCollisionRect;
		m_pVBOBuffer = e_pMesh->m_pVBOBuffer;
		m_bFromResource = true;
		CopyListPointer(e_pMesh);
		for(int i=0;i<TOTAL_FVF;++i)
		{
			m_ppfVerticesBuffer[i] = e_pMesh->m_ppfVerticesBuffer[i];
		}
		m_uiFVF_Format = e_pMesh->m_uiFVF_Format;
		assert(m_uiFVF_Format|(1<<0));//it must have position data
		m_uiVertexBufferCount = e_pMesh->m_uiVertexBufferCount;
		m_uiIndexBufferCount = e_pMesh->m_uiIndexBufferCount;
		m_puiIndexBuffer = e_pMesh->m_puiIndexBuffer;
		m_Sphere = e_pMesh->m_Sphere;
	}
	else
	{
		memset(m_bVerticesBuffer,0,sizeof(bool)*TOTAL_FVF);
		m_uiVertexBufferCount = 0;
		m_uiIndexBufferCount = 0;
		m_puiIndexBuffer = 0;
		m_uiFVF_Format = 0;
		m_pVBOBuffer = 0;
		for(int i=0;i<TOTAL_FVF;++i)
			m_ppfVerticesBuffer[i] = 0;
	}
}
cMesh::~cMesh()
{
//if the mesh is load from collada parser,the texture is store in ColladaPaser::cLibraryImages::m_ImageList
	if(!this->m_bFromResource)
	{
		SAFE_DELETE(m_pVBOBuffer);
		for( int i=0;i<TOTAL_FVF;++i )
			SAFE_DELETE(m_ppfVerticesBuffer[i]);
		SAFE_DELETE(m_puiIndexBuffer);
	}
	this->Destroy();
}

void	cMesh::Render(WCHAR*e_strShaderName)
{//ensure u have create depth framebuffer...
	UseShaderProgram(e_strShaderName);
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		if((*this)[i]->GetTechniqueType() == eTL_DIFFUSE)
			(*this)[i]->ApplyImage();
	}
	SetupShaderWorldMatrix(this->GetWorldTransform()* cMatrix44::ZupToYUp);
	//SetupShaderWorldMatrix(cMatrix44::ZupToYUp);
#ifdef DEBUG
		GLuint ll[TOTAL_FVF];
		memcpy(ll,g_uiAttribArray,sizeof(GLuint)*TOTAL_FVF);
#endif
	if( m_pVBOBuffer )
	{
		m_pVBOBuffer->Render(false);
	}
	else
	{
		//RenderCube(Vector3(1,1,1),cMatrix44::Identity,Vector4(1,1,0,1));
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( this->m_bVerticesBuffer[i] && g_uiAttribArray[i] != -1 )
			{
				myVertexAttribPointer(g_uiAttribArray[i], g_iFVF_DataStride[i], g_iFVF_DataType[i],0,0, m_ppfVerticesBuffer[i]);
			}
		}
		MY_GLDRAW_ELEMENTS(GL_TRIANGLES,m_uiIndexBufferCount, FMGetDrawIndiexType(),m_puiIndexBuffer );
	}
}

void	cMesh::AssignSkinningData(float*e_pfWeightsData,float*e_pfInflunceJointsData,UINT e_uiVertexCount)
{
	assert(e_uiVertexCount==m_uiVertexBufferCount);
	assert(m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT] == 0);
	assert(m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX] == 0);
	this->m_bVerticesBuffer[FVF_SKINNING_WEIGHT] = true;
	this->m_bVerticesBuffer[FVF_SKINNING_BONE_INDEX] = true;
	if( m_pVBOBuffer )
	{
		m_pVBOBuffer->SetupVerticesBuffer(e_pfWeightsData,FVF_SKINNING_WEIGHT,g_iFVF_DataStride[FVF_SKINNING_WEIGHT],e_uiVertexCount);
		//unsign byte data size is 1
		m_pVBOBuffer->SetupVerticesBuffer(e_pfInflunceJointsData,FVF_SKINNING_BONE_INDEX,g_iFVF_DataStride[FVF_SKINNING_BONE_INDEX],e_uiVertexCount,1);
	}
	else
	{
		m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT] = new float[m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT];
		m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX] = (float*)new char[m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT];
		memcpy(m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT],e_pfWeightsData,sizeof(float)*m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT);
		memcpy(m_ppfVerticesBuffer[FVF_SKINNING_BONE_INDEX],e_pfInflunceJointsData,sizeof(char)*m_uiVertexBufferCount*MAX_RELEATED_BONE_COUNT);
//#ifdef DEBUG
//		for( UINT i=0;i<e_uiVertexCount;++i )
//		{
//			float	l_fWeight = m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+1]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+2]+
//				m_ppfVerticesBuffer[FVF_SKINNING_WEIGHT][i*4+3];
//			assert(l_fWeight>=0.9f&&l_fWeight<=1.1f);
//		}
//#endif
	}
}

void	cMesh::ApplyNodeMatrix(cMatrix44 e_Mat)
{
	m_Sphere.vCenter = e_Mat.GetTranslation();
	this->SetLocalTransform(e_Mat);
}

void	cMesh::ChangeDiffuseImage(cBaseImage*e_pBaseImage)
{
	int	l_iDiffuseIndex = -1;
	for(UINT i=0;i<this->m_ObjectList.size();++i)
	{
		sTechniqueAndTexture*l_pTechniqueAndTexture = (*this)[i];
		if(l_pTechniqueAndTexture->GetTechniqueType() == eTL_DIFFUSE)
		{
			l_iDiffuseIndex = i;
		}
	}
	if( l_iDiffuseIndex != -1 )
		this->RemoveObject(l_iDiffuseIndex);
	sTechniqueAndTexture*l_pTechniqueAndTexture = new sTechniqueAndTexture(e_pBaseImage,eTL_DIFFUSE);
	bool	l_b = this->AddObject(l_pTechniqueAndTexture);
	assert(l_b);
}