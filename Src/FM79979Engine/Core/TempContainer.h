#ifndef _TEMP_CONTAINER_H_
#define _TEMP_CONTAINER_H_

template <class	T>class	cTempContainer
{
public:
	T**		m_ppObjects;
	int		m_iCount;
	int		m_iNumWorking;
	cTempContainer()
	{
		m_iNumWorking = 0;
		m_ppObjects = 0;
		m_iCount = 0;
	}
	void	SetCount(int e_iCount)
	{
		m_iNumWorking = 0;
		m_iCount = e_iCount;
		m_ppObjects = new T*[e_iCount];
		for( int i=0;i<e_iCount;++i )
			m_ppObjects[i] = 0;	
	}
	virtual ~cTempContainer(){ SAFE_DELETE_ARRAY(m_ppObjects); }
	void	Remove(int e_iIndex)
	{
		if( e_iIndex > -1 && m_iNumWorking > 0 )
		{
			m_ppObjects[e_iIndex] = m_ppObjects[m_iNumWorking-1];
			--m_iNumWorking;
		}
	}
	void	Clear()
	{
		m_iCount = 0;
		m_iNumWorking = 0;
		SAFE_DELETE_ARRAY(m_ppObjects);
	}
};

#endif