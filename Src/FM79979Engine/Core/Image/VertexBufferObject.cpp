#include "../stdafx.h"
#include "VertexBufferObject.h"
#include "../Image/SimplePrimitive.h"
#include "../OpenGL/GLSL/Shader.h"

#if defined(IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
//iphone 3g only support to 1.1!!
//#include <OpenGLES/ES1/gl.h>
//#include <OpenGLES/ES1/glext.h>
#elif defined(ANDROID)
	//#include <GLES/gl.h>
	//#include <GLES/glext.h>
	//#include <GLES/glplatform.h>
#endif

namespace FATMING_CORE
{
	bool	g_bVBOSupported = false;

	cVBOBuffer::cVBOBuffer()
	{
		m_iVertexStride = 0;
		memset(m_uiAllBufferID,0,sizeof(GLuint)*TOTAL_FVF);
		memset(m_iAllBufferStride,0,sizeof(int)*TOTAL_FVF);
		m_uiDrawIndicesBufferID = 0;
		m_uiIndicesCount = 0;
		m_uiDrawIndicesBufferID = 0;
	}

	cVBOBuffer::~cVBOBuffer()
	{
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAllBufferID[i] )
				glDeleteBuffers(1,&m_uiAllBufferID[i]);
		}
		if( m_uiDrawIndicesBufferID )
			glDeleteBuffers(1,&m_uiDrawIndicesBufferID);
	}

	//http://www.spec.org/gwpg/gpc.static/vbo_whitepaper.html
	//unsigned int currVboID; 
	//int vtxSize = numVertsInVBO * sizeof ( struct vector); 
	//int colSize = numVertsInVBO * sizeof ( struct colorvector); 
	//int nmlSize = numVertsInVBO * sizeof ( struct vector); 
	//int texSize = numVertsInVBO * sizeof ( struct texvector); 
	//int vtxOffs = 0; 
	//int colOffs = vtxOffs + vtxSize; 
	//int nmlOffs = colOffs + colSize; 
	//int texOffs = nmlOffs + nmlSize; 
	//int vboSize = texOffs + texSize;
	//int vtxDelta = prevVertexPointer - vertexData;
	//
	//if (numVertexBufferObjects >= allocVertexBufferObjects){ 
	//allocVertexBufferObjects = 2 * allocVertexBufferObjects + 16; 
	//vertexBufferObjects = realloc(vertexBufferObjects, allocVertexBufferObjects * sizeof (GLuint));
	//
	//if (!vertexBufferObjects) { 
	//printf("Error: could not allocate memory for vertexBufferObjects\n"); 
	//exit(0);
	//
	//} 
	//}
	//
	//glGenBuffers(1, (GLuint *) &currVboID); 
	//vertexBufferObjects[numVertexBufferObjects++] = currVboID; 
	//glBindBuffer(GL_ARRAY_BUFFER, currVboID); 
	//glBufferData(GL_ARRAY_BUFFER, vboSize, nullptr, mode.vboUsageMode); 
	//glBufferSubData(GL_ARRAY_BUFFER, vtxOffs, vtxSize, prevVertexPointer); 
	//glBufferSubData(GL_ARRAY_BUFFER, colOffs, colSize, prevColorPointer); 
	//glBufferSubData(GL_ARRAY_BUFFER, nmlOffs, nmlSize, prevNormalPointer); 
	//glBufferSubData(GL_ARRAY_BUFFER, texOffs, texSize, prevTexturePointer);

	//enusre the count is calculate by float
	void	cVBOBuffer::SetupVerticesBuffer(float*e_pfData,int e_iDataLocation,int e_iStride,UINT e_uiCount,int e_iDataSize)
	{
		if( m_iAllBufferStride[e_iDataLocation] != 0 )
		{
			glDeleteBuffers(1,&m_uiAllBufferID[e_iDataLocation]);
		}
		GLuint l_uiBuffer = 0;
		glGenBuffers(1,&l_uiBuffer);
		assert(l_uiBuffer != 0);
		m_uiAllBufferID[e_iDataLocation] = l_uiBuffer;
		m_iAllBufferStride[e_iDataLocation] = e_iStride;
		//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER_ARB,
		//				0,
		//				objectSize,
		//			    m_pU32FacesList);
		glBindBuffer(GL_ARRAY_BUFFER, l_uiBuffer);
		//glBufferSubData is better for directX?
		glBufferData(GL_ARRAY_BUFFER, e_uiCount*e_iDataSize*e_iStride, e_pfData, GL_STATIC_DRAW);
		//this one for openglES2.0
		//glBufferData(GL_ARRAY_BUFFER, e_uiCount*sizeof(float)*e_iStride, e_pfData, GL_STREAM_DRAW);
	#ifdef DEBUG
		assert(g_iFVF_DataStride[e_iDataLocation] == e_iStride);
	#endif
	}

	void	cVBOBuffer::SetupIndicesBuffer(float*e_pfData,UINT e_uiCount)
	{
		if( m_uiDrawIndicesBufferID != 0 )
		{
			glDeleteBuffers(1,&m_uiDrawIndicesBufferID);
		}
		GLuint l_uiBufferID;
		glGenBuffers(1,&l_uiBufferID);
		assert(l_uiBufferID != 0);
		m_uiDrawIndicesBufferID = l_uiBufferID;
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiDrawIndicesBufferID);
		//STREAM
		//The data store contents will be modified once and used at most a few times.

		//STATIC
		//The data store contents will be modified once and used many times.

		//DYNAMIC
		//The data store contents will be modified repeatedly and used many times.

#ifdef WIN32
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_uiCount*sizeof(UINT), e_pfData, GL_STATIC_DRAW);
#else
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, e_uiCount*sizeof(unsigned short), e_pfData, GL_STATIC_DRAW);
#endif
		m_uiIndicesCount = e_uiCount;
	}

	float*	cVBOBuffer::GetData(int e_iDataLocation)
	{
		//assert(0&&"fuck this is wrong");
		glBindBuffer(GL_ARRAY_BUFFER,m_uiAllBufferID[e_iDataLocation]);
		float*l_pData = 0;
		assert(0&&"glGetPointerv it seems not support for opengl ES2...so I comment below");
		//glGetPointerv(GL_VERTEX_ARRAY_POINTER,(void**)&l_pData);
		//glGetFloatv(GL_ARRAY_BUFFER,l_pData);

		return l_pData;

	}

	void	cVBOBuffer::Init()
	{
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_iAllBufferStride[i] != 0 )
			{
				m_iVertexStride += g_iFVF_DataSize[i];
			}
		}
	}
	void	cVBOBuffer::Render(bool e_benableDataStride)
	{
		if( m_iVertexStride == 0 )
			Init();
//#ifndef OPENGLES_2_X
//		glBindBuffer(GL_ARRAY_BUFFER, m_uiAllBufferID[FVF_POS]);
//		glVertexPointer(m_uiAllBufferID[FVF_POS], g_iFVF_DataType[FVF_POS], 0, 0);
//		//int	l_iPointerStartOffset = sizeof(float)*3;
//		if (m_uiAllBufferID[FVF_DIFFUSE])
//		{
//			glBindBuffer(GL_ARRAY_BUFFER, m_uiAllBufferID[FVF_DIFFUSE]);
//			glColorPointer(m_uiAllBufferID[FVF_DIFFUSE], g_iFVF_DataType[FVF_DIFFUSE], 0, 0);
//			//l_iPointerStartOffset += sizeof(float)*4;
//		}
//		if (m_uiAllBufferID[FVF_NORMAL])
//		{
//			glBindBuffer(GL_ARRAY_BUFFER, m_uiAllBufferID[FVF_NORMAL]);
//			glNormalPointer(m_uiAllBufferID[FVF_NORMAL], g_iFVF_DataType[FVF_NORMAL], 0);
//			//l_iPointerStartOffset += sizeof(float)*3;
//		}
//		if (m_uiAllBufferID[FVF_TEX0])
//		{
//			glBindBuffer(GL_ARRAY_BUFFER, m_uiAllBufferID[FVF_TEX0]);
//			glTexCoordPointer(m_uiAllBufferID[FVF_TEX0], g_iFVF_DataType[FVF_TEX0], 0, 0);
//			//l_iPointerStartOffset += sizeof(float)*2;
//		}
//#else
		//const char*l_strError = (const char*)glGetString(glGetError());
		//If a non-zero named buffer object is bound to the GL_ARRAY_BUFFER target (see glBindBuffer)
		//while a generic vertex attribute array is specified, pointer is treated as a byte offset into the buffer object's data store.
		//Also, the buffer object binding (GL_ARRAY_BUFFER_BINDING) is saved as generic vertex attribute array client-side state
		//(GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING) for index index
		int	l_iPointerStartOffset = 0;
		for( int i=0;i<TOTAL_FVF;++i )
		{
			if( m_uiAllBufferID[i] && g_uiAttribArray[i] != -1 )
			{
				glBindBuffer(GL_ARRAY_BUFFER, m_uiAllBufferID[i]);
				//myVertexAttribPointer(g_uiAttribArray[i],g_iFVF_DataStride[i], GL_FLOAT,GL_FALSE, 0, (GLvoid*)l_iPointerStartOffset);
				//ensure the shader has been call cBaseShader->Use,or g_uiAttribArray won't be assigned.
				myVertexAttribPointer(g_uiAttribArray[i],g_iFVF_DataStride[i], g_iFVF_DataType[i],GL_FALSE,e_benableDataStride?m_iVertexStride:0, 0);

				//l_iPointerStartOffset += sizeof(float)*g_iFVF_DataStride[i];
			}
		}
//#endif
		if( m_uiIndicesCount != 0 )
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiDrawIndicesBufferID);
			MY_GLDRAW_ELEMENTS(GL_TRIANGLES, m_uiIndicesCount, g_iDrawindiceType, 0);
		}
		//unbind data so rest object could render on the screen or it will render last bind data.
		glBindBuffer(GL_ARRAY_BUFFER,0);
	}
}