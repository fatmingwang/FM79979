#pragma once


void	ComputerShaderInit();
void	ComputerShaderUpdate();
void	ComputerShaderRender();
void	ComputerShaderDestroy();



template <class T>
class ShaderBuffer {
public:
	ShaderBuffer(size_t size);  // size in number of elements
	~ShaderBuffer();

	void bind();
	void unbind();

	T* map(GLbitfield access = GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
	void unmap();

	GLuint getBuffer() { return m_buffer; }
	size_t getSize() const { return m_size; }

	void dump();

private:
	static const GLenum target = GL_SHADER_STORAGE_BUFFER;

	size_t m_size;

	GLuint m_buffer;
};
//https://stackoverflow.com/questions/38172696/should-i-ever-use-a-vec3-inside-of-a-uniform-buffer-or-shader-storage-buffer-o/38172697#38172697
//CS data alignment
template <class T>
ShaderBuffer<T>::ShaderBuffer(size_t size) :
	m_size(size)
{
	glGenBuffers(1, &m_buffer);

	bind();
	glBufferData(target, m_size * sizeof(T), 0, GL_STATIC_DRAW);
	unbind();
}

template <class T>
ShaderBuffer<T>::~ShaderBuffer()
{
	glDeleteBuffers(1, &m_buffer);
}

template <class T>
void ShaderBuffer<T>::bind()
{
	glBindBuffer(target, m_buffer);
}

template <class T>
void ShaderBuffer<T>::unbind()
{
	glBindBuffer(target, 0);
}

template <class T>
T* ShaderBuffer<T>::map(GLbitfield access)
{
	bind();
	return (T*)glMapBufferRange(target, 0, m_size * sizeof(T), access);
}

template <class T>
void ShaderBuffer<T>::unmap()
{
	bind();
	glUnmapBuffer(target);
}

template <class T>
void ShaderBuffer<T>::dump()
{
	T* data = map(GL_MAP_READ_BIT);
	for (size_t i = 0; i < m_size; i++) {
		std::cout << i << ": " << data[i] << std::endl;
	}
	unmap();
}

class cComputeShader:public cBaseShader
{
	f_BindGLDataFunction	m_fBindGLDataFunction;
public:
	cComputeShader();
	virtual ~cComputeShader();
	//
	virtual	void				Use(bool e_bUseLastWVPMatrix = true)override;
	virtual	void				Update(float e_fElpaseTime)override;
};