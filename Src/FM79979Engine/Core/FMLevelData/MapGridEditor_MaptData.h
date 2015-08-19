#ifndef MAP_GRID_EDITOR_MAP_DATA_H
#define MAP_GRID_EDITOR_MAP_DATA_H
//==================
//the different between this and c2DDataContainer
//the children data could be null because it's a pointer pointer
//==================
	template<class T>	class	cMap_GridDataContainer
	{
		int	m_iGridWidth;
		int	m_iGridHeight;
		T**	m_ppChildrenGridData;
		cMap_GridDataContainer<T*>	m_pParent;
	public:
		cMapGridDataContainer(cMapGridDataContainer<>*e_pParent)
		{
			m_ppChildrenGridData = 0;
			m_pParent = e_pParent;
		}
		~c2DDataContainer()
		{
			if( m_ppChildrenGridData )
			{
				int	l_iSize = m_iGridWidth*m_iGridHeight;
				for( int i=0;i<l_iSize;++i )
					SAFE_DELETE(m_ppChildrenGridData[i]);
			}
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
				m_iDataWidth = e_iGridWidth;
				m_iDataHeight = e_iGridHeight;
				m_ppChildrenGridData = new T*[e_iGridWidth*e_iGridHeight];
				int	l_iSize = m_iGridWidth*m_iGridHeight;
				for( int i=0;i<l_iSize;++i )
					m_ppChildrenGridData[i] = 0;

			}
			else
			{
				//check new size and assign new data into,just like a 2D texture(so ensure the width data is coorespond)
				if( e_iGridWidth != m_iDataWidth || e_iGridHeight != m_iDataHeight )
				{
					int	l_iNewSize = e_iGridWidth*e_iGridHeight;
					T*l_pChildrenGridData = new T*[l_iNewSize];
					int	l_iBiggerWidth = m_iDataWidth>e_iGridWidth?m_iDataWidth:e_iGridWidth;
					int	l_iBiggerHeight = m_iDataHeight>e_iDataHeight?m_iDataHeight:e_iDataHeight;
					for( int i=0;i<l_iNewSize;++i )
						m_ppChildrenGridData[i] = 0;
					int	l_iCrrentIndex = 0;
					for( int i=0;i<l_iBiggerWidth;++i )//copy old data into new one
					{
						for( int j=0;j<l_iBiggerHeight;++j )
						{
							l_iCrrentIndex = j*l_iBiggerWidth+i
							if( !m_ppChildrenGridData[l_iCrrentIndex] )//old data is nil so set new as nil too,and continue
							{
								l_pChildrenGridData[l_iCrrentIndex] = 0;
								continue;
							}
							if( m_iDataWidth<i || m_iDataHeight<j )//over size,delete old one
							{
								delete m_ppChildrenGridData[l_iCrrentIndex];
							}
							else
							{
								if( m_iDataWidth>i || m_iDataHeight>j )//over new capacity set new data as nill
								{
									l_pChildrenGridData[l_iCrrentIndex] = 0;
								}
								else//copy hte old data
								{
									l_pChildrenGridData[l_iCrrentIndex] = m_ppChildrenGridData[l_iCrrentIndex];
								}
							}
						}
					}
					m_iDataWidth = e_iGridWidth;
					m_iDataHeight = e_iGridHeight;
					m_pChildrenGridData = l_pChildrenGridData;
				}
			}
			return true;
		}

		inline	T*	operator [](int e_iX,int e_iY)
		{
			assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
			return m_pChildrenGridData[e_iX+e_iY*m_iDataWidth];
		}

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
		inline	void operator =(int e_iX,int e_iY,T*e_pData)
		{
			assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
			if( m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] )
				delete m_pChildrenGridData[e_iX+e_iY*m_iDataWidth];
			m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] = e_pData;
		}

		inline	void operator =(int e_iIndex,T*e_pData)
		{
			assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
			if( m_pChildrenGridData[e_iIndex] )
				delete m_pChildrenGridData[e_iIndex];
			m_pChildrenGridData[e_iIndex] = e_pData;
		}
		
		inline	void SetObject(int e_iX,int e_iY,T*e_pData)
		{
			assert(m_iDataWidth>=e_iX&&m_iDataHeight>=e_iY);
			if( m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] )
				delete m_pChildrenGridData[e_iX+e_iY*m_iDataWidth];
			m_pChildrenGridData[e_iX+e_iY*m_iDataWidth] = e_pData;
		}

		inline	void SetObject(int e_iIndex,T*e_pData)
		{
			assert(m_iDataWidth*m_iDataHeight>=e_iIndex);
			if( m_pChildrenGridData[e_iIndex] )
				delete m_pChildrenGridData[e_iIndex];
			m_pChildrenGridData[e_iIndex] = e_pData;
		}
	};

#endif