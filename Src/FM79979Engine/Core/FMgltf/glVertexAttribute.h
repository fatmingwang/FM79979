#pragma once

#include <vector>

template<typename T>
class cOpenGlVertexAttribute
{
protected:
	unsigned int m_uiHandle;
	unsigned int m_uiCount;
private:
	cOpenGlVertexAttribute(const cOpenGlVertexAttribute& other);
	cOpenGlVertexAttribute& operator=(const cOpenGlVertexAttribute& other);
	void SetAttribPointer(unsigned int slot);
public:
	cOpenGlVertexAttribute();
	~cOpenGlVertexAttribute();

	void Set(T* inputArray, unsigned int arrayLength);
	void Set(std::vector<T>& input);

	void BindTo(unsigned int slot);
	void UnBindFrom(unsigned int slot);

	unsigned int Count();
	unsigned int GetHandle();
};