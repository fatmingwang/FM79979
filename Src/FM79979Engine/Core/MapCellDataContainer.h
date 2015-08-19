#ifndef MAP_CELL_EDITOR_MAP_DATA_H
#define MAP_CELL_EDITOR_MAP_DATA_H

	//============================
	//the data want to store must be a pointer,
	//similar with c2DDataContainer,but it's a pointer
	//a container alos could be a cell,so we could go deep and deep to figure out the data as needed.
	//============================
	template<class T>	class	cMap_CellDataContainer
	{
		int	m_iGridWidth;
		int	m_iGridHeight;
		T**	m_ppChildrenGridData;
		cMap_CellDataContainer<T>	*m_pParent;
		int	m_iIndexX;//if parent is exist,index could quickly to fetch the data from parent
		int	m_iIndexY;
	public:
		cMap_CellDataContainer(cMap_CellDataContainer<T>*e_pParent,int e_iIndexX,int e_iIndexY)
		{
			m_ppChildrenGridData = 0;
			m_pParent = e_pParent;
			m_iIndexX = e_iIndexX;
			m_iIndexY = e_iIndexY;
		}
		virtual ~cMap_CellDataContainer()
		{
			if( m_ppChildrenGridData )
			{
				int	l_iSize = m_iGridWidth*m_iGridHeight;
				for( int i=0;i<l_iSize;++i )
					SAFE_DELETE(m_ppChildrenGridData[i]);
				SAFE_DELETE(m_ppChildrenGridData);
			}
		}
		int		GetIndexX(){ return m_iIndexX; }
		int		GetIndexY(){ return m_iGridHeight; }
		void	SetIndex(int e_iIndexX,int e_iIndexY){ m_iIndexX = e_iIndexX; m_iIndexY = e_iIndexY; }
		//if parent is exist,check the index ID is correspond
		bool	IsIndexIDCorrect(){ return m_pParent?m_pParent[m_iIndexX,m_iIndexY]==this?true:false:true; }

		int	GetGridWidth(){ return m_iGridWidth; }
		int	GetGridHeight(){ return m_iGridHeight; }
		//if new valueis smaller than old one,it will forsake the old data that is over the capacity
		inline	bool	ReSize(int e_iGridWidth,int e_iGridHeight)
		{
			assert(e_iGridHeight>=1&&e_iGridWidth>=1);
			if( e_iGridHeight == 0 || e_iGridWidth == 0 )
			{
				return false;
			}
			if( !m_ppChildrenGridData )
			{
				m_iGridWidth = e_iGridWidth;
				m_iGridHeight = e_iGridHeight;
				m_ppChildrenGridData = new T*[e_iGridWidth*e_iGridHeight];
				int	l_iSize = m_iGridWidth*m_iGridHeight;
				for( int i=0;i<l_iSize;++i )
					m_ppChildrenGridData[i] = 0;

			}
			else
			{
				//check new size and assign new data into,just like a 2D texture(so ensure the width data is coorespond)
				if( e_iGridWidth != m_iGridWidth || e_iGridHeight != m_iGridHeight )
				{
					int	l_iNewSize = e_iGridWidth*e_iGridHeight;
					T**l_ppChildrenGridData = new T*[l_iNewSize];
					int	l_iBiggerWidth = m_iGridWidth>e_iGridWidth?m_iGridWidth:e_iGridWidth;
					int	l_iBiggerHeight = m_iGridHeight>e_iGridHeight?m_iGridHeight:e_iGridHeight;
					for( int i=0;i<l_iNewSize;++i )
						m_ppChildrenGridData[i] = 0;
					int	l_iCrrentIndex = 0;
					for( int i=0;i<l_iBiggerWidth;++i )//copy old data into new one
					{
						for( int j=0;j<l_iBiggerHeight;++j )
						{
							l_iCrrentIndex = j*l_iBiggerWidth+i;
							if( !m_ppChildrenGridData[l_iCrrentIndex] )//old data is nil so set new as nil too,and continue
							{
								l_ppChildrenGridData[l_iCrrentIndex] = 0;
								continue;
							}
							if( m_iGridWidth<i || m_iGridHeight<j )//over size,delete old one
							{
								delete m_ppChildrenGridData[l_iCrrentIndex];
							}
							else
							{
								if( m_iGridWidth>i || m_iGridHeight>j )//over new capacity set new data as nill
								{
									l_ppChildrenGridData[l_iCrrentIndex] = 0;
								}
								else//copy hte old data
								{
									l_ppChildrenGridData[l_iCrrentIndex] = m_ppChildrenGridData[l_iCrrentIndex];
								}
							}
						}
					}
					m_iGridWidth = e_iGridWidth;
					m_iGridHeight = e_iGridHeight;
					m_ppChildrenGridData = l_ppChildrenGridData;
				}
			}
			return true;
		}

		//inline	T*	operator [](int e_iX,int e_iY)
		//{
		//	assert(m_iGridWidth>=e_iX&&m_iGridHeight>=e_iY);
		//	return m_pChildrenGridData[e_iX+e_iY*m_iGridWidth];
		//}

		//inline	T*	operator [](int e_iIndex)
		//{
		//	assert(m_iGridWidth*m_iGridHeight>=e_iIndex);
		//	return m_pChildrenGridData[e_iIndex];
		//}
		
		inline	T*	GetObject(int e_iX,int e_iY)
		{
			assert(m_iGridWidth>=e_iX&&m_iGridHeight>=e_iY);
			return m_ppChildrenGridData[e_iX+e_iY*m_iGridWidth];		
		}

		inline	T*	GetObject(int e_iIndex)
		{
			assert(m_iGridWidth*m_iGridHeight>=e_iIndex);
			return m_ppChildrenGridData[e_iIndex];
		}
		//set data will erase original data.
		//inline	void operator =(int e_iX,int e_iY,T*e_pData)
		//{
		//	assert(m_iGridWidth>=e_iX&&m_iGridHeight>=e_iY);
		//	if( m_pChildrenGridData[e_iX+e_iY*m_iGridWidth] )
		//		delete m_pChildrenGridData[e_iX+e_iY*m_iGridWidth];
		//	m_pChildrenGridData[e_iX+e_iY*m_iGridWidth] = e_pData;
		//}

		//inline	void operator =(int e_iIndex,T*e_pData)
		//{
		//	assert(m_iGridWidth*m_iGridHeight>=e_iIndex);
		//	if( m_pChildrenGridData[e_iIndex] )
		//		delete m_pChildrenGridData[e_iIndex];
		//	m_pChildrenGridData[e_iIndex] = e_pData;
		//}
		
		inline	void SetObject(int e_iX,int e_iY,T*e_pData)
		{
			assert(m_iGridWidth>=e_iX&&m_iGridHeight>=e_iY);
			if( m_ppChildrenGridData[e_iX+e_iY*m_iGridWidth] )
				delete m_ppChildrenGridData[e_iX+e_iY*m_iGridWidth];
			m_ppChildrenGridData[e_iX+e_iY*m_iGridWidth] = e_pData;
		}

		inline	void SetObject(int e_iIndex,T*e_pData)
		{
			assert(m_iGridWidth*m_iGridHeight>=e_iIndex);
			if( m_ppChildrenGridData[e_iIndex] )
				delete m_ppChildrenGridData[e_iIndex];
			m_ppChildrenGridData[e_iIndex] = e_pData;
		}
	};

#endif