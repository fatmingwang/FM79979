#pragma once

template< class TYPE >
class cSingltonTemplate
{
	static TYPE*	m_pInstance;
public:
	cSingltonTemplate()
	{
		assert(m_pInstance == nullptr && "singlton only can have one instance" );
	}
	~cSingltonTemplate(){}
	static TYPE* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new TYPE();
		}
		return m_pInstance;
	}
	static void DestroyInstance()
	{
		SAFE_DELETE(m_pInstance);
	}
};

template<class TYPE>TYPE* cSingltonTemplate<TYPE>::m_pInstance = nullptr;