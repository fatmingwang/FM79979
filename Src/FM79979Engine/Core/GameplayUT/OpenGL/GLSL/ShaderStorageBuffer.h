#pragma once

#include "../Glh.h"

#ifdef LINUX
#include <GLES3/gl3.h>
#include <GLES3/gl31.h>
#include <GLES3/gl32.h>
#endif

#ifdef WASM

inline void		glBindProgramPipeline(GLuint pipeline) {}
inline GLuint	glCreateShaderProgramv(GLenum type, GLsizei count, const GLchar* const* strings) { return 0; }
inline void		glDeleteProgramPipelines(GLsizei n, const GLuint* pipelines) {}
inline void		glDispatchCompute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z){}
inline void		glGenProgramPipelines(GLsizei n,GLuint* pipelines){}
inline void		glGetProgramPipelineInfoLog(GLuint pipeline,GLsizei bufSize,GLsizei* length,GLchar* infoLog){}
inline void		glGetProgramPipelineiv(GLuint pipeline,GLenum pname,GLint* params){}inline
GLuint glGetProgramResourceIndex(GLuint program,GLenum programInterface,const char* name){return 0;}
inline void		glMemoryBarrier(GLbitfield barriers){}
inline void		glUseProgramStages(GLuint pipeline,GLbitfield stages,GLuint program){}
inline void		glValidateProgramPipeline(GLuint pipeline){}
#endif
//https://arm-software.github.io/opengl-es-sdk-for-android/compute_intro.html

//https://www.yumpu.com/en/document/read/32089810/opengl-4x-and-beyondpdf-nvidia-developer-zone

template <class T> class cShaderStorageBuffer :public NamedTypedObject
{
public:
	cShaderStorageBuffer(size_t size);  // size in number of elements
	~cShaderStorageBuffer();

	void	Bind();
	void	Unbind();

	T*		Map(GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	void	Unmap();

	bool	CopyIntoSSB(char*e_pData,unsigned int e_uiDataLen,bool e_bAutoIncrease = true);
	void	Resize(unsigned int e_uiSize,bool e_bIfSizeIsSnallerIgonreThis = true);

	GLuint	GetBufferID() { return m_iBufferID; }
	size_t	GetSize() const { return m_uiSize; }

	//void	dump();
private:
	static const GLenum m_seBufferObjectTarget = GL_SHADER_STORAGE_BUFFER;
	size_t m_uiSize;
	GLuint m_iBufferID;
};

//https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o/38172697#38172697
//CS data alignment
template <class T> cShaderStorageBuffer<T>::cShaderStorageBuffer(size_t size):m_uiSize(size)
{
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenBuffers(1, &m_iBufferID));
	Bind();
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBufferData(m_seBufferObjectTarget, m_uiSize * sizeof(T), 0, GL_DYNAMIC_DRAW));
	Unbind();
}

template <class T> cShaderStorageBuffer<T>::~cShaderStorageBuffer()
{
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glDeleteBuffers(1, &m_iBufferID));
}

template <class T> void cShaderStorageBuffer<T>::Bind()
{
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindBuffer(m_seBufferObjectTarget, m_iBufferID));
}

template <class T> void cShaderStorageBuffer<T>::Unbind()
{
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBindBuffer(m_seBufferObjectTarget, 0));
}

template <class T> T* cShaderStorageBuffer<T>::Map(GLbitfield access)
{
	Bind();
#ifdef WASM
	return nullptr;
#else
	return (T*)glMapBufferRange(m_seBufferObjectTarget, 0, m_uiSize * sizeof(T), access);
#endif
}

template <class T> void cShaderStorageBuffer<T>::Unmap()
{
	Bind();
#ifndef WASM
	LAZY_DO_GL_COMMAND_AND_GET_ERROR(glUnmapBuffer(m_seBufferObjectTarget));
#endif
}

template<class T>
inline bool cShaderStorageBuffer<T>::CopyIntoSSB(char* e_pData, unsigned int e_uiDataLen, bool e_bAutoIncrease)
{
	if (e_uiDataLen > m_uiSize && e_bAutoIncrease)
	{ 
		glDeleteBuffers(1, &m_iBufferID);
		Bind();
		glBufferData(m_seBufferObjectTarget, m_uiSize * sizeof(T), 0, GL_DYNAMIC_DRAW);
		m_uiSize = e_uiDataLen;
	}
	
	if (e_uiDataLen <= m_uiSize  )
	{
		auto* l_pData = Map();
		memcpy(l_pData, e_pData, e_uiDataLen);
		Unmap();
		return true;
	}
	return false;
}

template<class T>
inline void cShaderStorageBuffer<T>::Resize(unsigned int e_uiSize, bool e_bIfSizeIsSnallerIgonreThis)
{
	bool l_bDoResize = false;
	if ( this->m_uiSize != e_uiSize && !e_bIfSizeIsSnallerIgonreThis )
	{
		l_bDoResize = true;
	}
	else
	if (this->m_uiSize < e_uiSize)
	{
		l_bDoResize = true;
	}
	if (l_bDoResize)
	{
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glDeleteBuffers(1, &m_iBufferID));
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glGenBuffers(1, &m_iBufferID));
		Bind();
		LAZY_DO_GL_COMMAND_AND_GET_ERROR(glBufferData(m_seBufferObjectTarget, m_uiSize * sizeof(T), 0, GL_DYNAMIC_DRAW));
		Unbind();
	}
}

//template <class T> void cShaderStorageBuffer<T>::dump()
//{
//	T* data = Map(GL_MAP_READ_BIT);
//	//for (size_t i = 0; i < m_uiSize; i++)
//	//{
//	//	std::cout << i << ": " << data[i] << std::endl;
//	//}
//	Unmap();
//}