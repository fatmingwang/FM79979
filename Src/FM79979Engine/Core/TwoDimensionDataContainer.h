#ifndef TWO_DIMENSION_DATA_COTAINER_H
#define TWO_DIMENSION_DATA_COTAINER_H

	//============================
	//just like a 2D texture but a template
	//the T can't be a pointer...
	//if u would like to using a pointer instead the data u want to store use
	//u could treat T as a integer or struct that does't contain any pointer
	//because the data inside m_pChildrenGridData is not a pointer
	//============================
	template<class T>	class	c2DDataContainer
	{
	protected:
		int	m_iDataWidth;
		int	m_iDataHeight;
		T*	m_pChildrenGridData;
		c2DDataContainer<T*>	m_pParent;
	public:
		c2DDataContainer(c2DDataContainer<T*>*e_pParent)
		{
			m_pChildrenGridData = 0;
			m_pParent = e_pParent;
		}
		virtual ~c2DDataContainer()
		{
			SAFE_DELETE(m_pChildrenGridData);
		}

		int	GetGridWidth(){ return m_iDataWidth; }
		int	GetGridHeight(){ return m_iDataHeight; }
		//if new valueis smaller than old one,it will forsake the old data that is over the capacity
		inline	bool	ReSize(int e_iGridWidth,int e_iGridHeight)
		{
			assert(e_iGridHeight>=1&&e_iGridWidth>=1);
			if( e_iGridHeight == 0 || e_iGridWidth == 0 )
			{
				return false;
			}
			if( !m_pChildrenGridData )
			{
				m_pChildrenGridData = new T[e_iGridWidth*e_iGridHeight];
				m_iDataWidth = e_iGridWidth;
				m_iDataHeight = e_iGridHeight;
			}
			else
			{
				//check new size and assign new data into,just like a 2D texture(so ensure the width data is coorespond)
				if( e_iGridWidth != m_iDataWidth || e_iGridHeight != m_iDataHeight )
				{
					int	l_iNewSize = e_iGridWidth*e_iGridHeight;
					T*l_pChildrenGridData = new T[l_iNewSize];
					memset(l_pChildrenGridData,0,sizeof(T)*l_iNewSize);
					for( int i=0;i<m_iDataHeight;++i )//copy old data into new one
					{
						int	l_iSizeToCopy = m_iDataWidth*i;
						memcpy(l_pChildrenGridData[l_iSizeToCopy],m_pChildrenGridData[l_iSizeToCopy],sizeof(T)*m_iDataWidth);
					}
					m_iDataWidth = e_iGridWidth;
					m_iDataHeight = e_iGridHeight;
					SAFE_DELETE(m_pChildrenGridData);
					m_pChildrenGridData = l_pChildrenGridData;
				}
			}
			return true;
		}

		//inline	T*	operator [](int e_iX,int e_iY)
		//{
		//	assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
		//	return m_pChildrenGridData[e_iX+e_iY*m_iDataWidth];
		//}

		inline	T*	operator [](int e_iIndex)
		{
			assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
			return m_pChildrenGridData[e_iIndex];
		}
		
		inline	T*	GetObject(int e_iX,int e_iY)
		{
			assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
			return m_pChildrenGridData[e_iX+e_iY*m_iDataWidth];		
		}

		inline	T*	GetObject(int e_iIndex)
		{
			assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
			return m_pChildrenGridData[e_iIndex];
		}
		//set data will erase original data.
		//inline	void operator =(int e_iX,int e_iY,T*e_pData)
		//{
		//	assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
		//	m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] = e_pData;
		//}

		//inline	void operator =(int e_iIndex,T*e_pData)
		//{
		//	assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
		//	m_pChildrenGridData[e_iIndex] = e_pData;
		//}
		
		inline	void SetObject(int e_iX,int e_iY,T e_Data)
		{
			assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
			m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] = e_Data;
		}

		inline	void SetObject(int e_iIndex,T e_Data)
		{
			assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
			m_pChildrenGridData[e_iIndex] = e_Data;
		}
	};

#endif