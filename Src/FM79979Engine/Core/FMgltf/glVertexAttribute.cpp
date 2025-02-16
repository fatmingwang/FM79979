//#pragma warning(disable : 26451)
#include "../../Core/AllCoreInclude.h"
#include "glVertexAttribute.h"
#undef APIENTRY


//#include "vec2.h"
//#include "vec3.h"
//#include "vec4.h"
//#include "quat.h"

template cOpenGlVertexAttribute<int>;
template cOpenGlVertexAttribute<float>;
template cOpenGlVertexAttribute<Vector2>;
template cOpenGlVertexAttribute<Vector3>;
template cOpenGlVertexAttribute<Vector4>;
//template cOpenGlVertexAttribute<ivec4>;
template cOpenGlVertexAttribute<Quaternion>;

template<typename T>
cOpenGlVertexAttribute<T>::cOpenGlVertexAttribute()
{
	glGenBuffers(1, &m_uiHandle);
	m_uiCount = 0;
}

template<typename T>
cOpenGlVertexAttribute<T>::~cOpenGlVertexAttribute()
{
	glDeleteBuffers(1, &m_uiHandle);
}

template<typename T>
void cOpenGlVertexAttribute<T>::Set(T* inputArray, unsigned int arrayLength)
{
	m_uiCount = arrayLength;
	unsigned int size = sizeof(T);

	glBindBuffer(GL_ARRAY_BUFFER, m_uiHandle);
	glBufferData(GL_ARRAY_BUFFER, size * m_uiCount, inputArray, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void cOpenGlVertexAttribute<T>::Set(std::vector<T>& input)
{
	Set(&input[0], (unsigned int)input.size());
}

template<typename T>
void cOpenGlVertexAttribute<T>::BindTo(unsigned int slot)
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiHandle);
	glEnableVertexAttribArray(slot);
	SetAttribPointer(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
void cOpenGlVertexAttribute<T>::UnBindFrom(unsigned int slot) 
{
	glBindBuffer(GL_ARRAY_BUFFER, m_uiHandle);
	glDisableVertexAttribArray(slot);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template<typename T>
unsigned int cOpenGlVertexAttribute<T>::Count()
{
	return m_uiCount;
}

template<typename T>
unsigned int cOpenGlVertexAttribute<T>::GetHandle()
{
	return m_uiHandle;
}

template<>
void cOpenGlVertexAttribute<int>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribIPointer(slot, 1, GL_INT, 0, (void*)0);
}

//template<>
//void cOpenGlVertexAttribute<ivec4>::SetAttribPointer(unsigned int slot)
//{
//	glVertexAttribIPointer(slot, 4, GL_INT, 0, (void*)0);
//}

template<>
void cOpenGlVertexAttribute<float>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribPointer(slot, 1, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void cOpenGlVertexAttribute<Vector2>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribPointer(slot, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void cOpenGlVertexAttribute<Vector3>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribPointer(slot, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void cOpenGlVertexAttribute<Vector4>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}

template<>
void cOpenGlVertexAttribute<Quaternion>::SetAttribPointer(unsigned int slot)
{
	glVertexAttribPointer(slot, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
}