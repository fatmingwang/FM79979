#pragma once

//https://arm-software.github.io/opengl-es-sdk-for-android/compute_intro.html

//https://www.yumpu.com/en/document/read/32089810/opengl-4x-and-beyondpdf-nvidia-developer-zone
template <class T> class cShaderStorageBuffer 
{
public:
	cShaderStorageBuffer(size_t size);  // size in number of elements
	~cShaderStorageBuffer();

	void	Bind();
	void	Unbind();

	T*		Map(GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	void	Unmap();

	bool	CopyIntoSSB(char*e_pData,unsigned int e_uiDataLen,bool e_bAutoIncrease = true);

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
	glGenBuffers(1, &m_iBufferID);
	Bind();
	glBufferData(m_seBufferObjectTarget, m_uiSize * sizeof(T), 0, GL_STATIC_DRAW);
	Unbind();
}

template <class T> cShaderStorageBuffer<T>::~cShaderStorageBuffer()
{
	glDeleteBuffers(1, &m_iBufferID);
}

template <class T> void cShaderStorageBuffer<T>::Bind()
{
	glBindBuffer(m_seBufferObjectTarget, m_iBufferID);
}

template <class T> void cShaderStorageBuffer<T>::Unbind()
{
	glBindBuffer(m_seBufferObjectTarget, 0);
}

template <class T> T* cShaderStorageBuffer<T>::Map(GLbitfield access)
{
	Bind();
	return (T*)glMapBufferRange(m_seBufferObjectTarget, 0, m_uiSize * sizeof(T), access);
}

template <class T> void cShaderStorageBuffer<T>::Unmap()
{
	Bind();
	glUnmapBuffer(m_seBufferObjectTarget);
}

template<class T>
inline bool cShaderStorageBuffer<T>::CopyIntoSSB(char* e_pData, unsigned int e_uiDataLen, bool e_bAutoIncrease)
{
	if (e_uiDataLen > m_uiSize && e_bAutoIncrease)
	{ 
		glDeleteBuffers(1, &m_iBufferID);
		Bind();
		glBufferData(m_seBufferObjectTarget, m_uiSize * sizeof(T), 0, GL_STATIC_DRAW);
		m_uiSize = e_uiDataLen;
	}
	
	if (e_uiDataLen <= m_uiSize  )
	{
		auto* l_pData = Map();
		memcpy(l_pData, e_pData, e_uiSize);
		Unmap();
		return true;
	}
	return false;
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