#ifndef	_FISH_COLLECTION_SCROLLER_H_
#define	_FISH_COLLECTION_SCROLLER_H_


	class	cFishCollectionScroller:public cScroller
	{
		int			m_iNumRow;
		int			m_iNumColumn;
		Vector2		m_vGap;
	public:
		cFishCollectionScroller(int e_iNumRow,int e_iNumColumn,Vector2 e_vGap);
		~cFishCollectionScroller();

		virtual void			Init();
	};

#endif