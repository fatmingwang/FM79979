#include "stdafx.h"
#include "FishCollectionScroller.h"

cFishCollectionScroller::cFishCollectionScroller(int e_iNumRow,int e_iNumColumn,Vector2 e_vGap)
{
	this->m_eOrientation = eO_HORIZONTAL;
	//this->m_eOrientation = eO_VERTICAL;
	m_iNumRow = e_iNumRow;
	m_iNumColumn = e_iNumColumn;
	m_vGap = e_vGap;
	this->m_bCheckOutOfWorking = false;
}

cFishCollectionScroller::~cFishCollectionScroller()
{

}

void	cFishCollectionScroller::Init()
{
	cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(0);
	Vector4	l_vOriginalViewRect = this->m_vCollisionRange;
	//this one will auto rest so set it back
    cMouse_TouchWithCancelButton::Init();
	m_vCollisionRange = l_vOriginalViewRect;
	m_bRollBack = false;
	m_fTotalLength = 0.f;
	if( this->m_pBG )
		m_pBG->Init();
    int l_iCOunt = this->Count();
    if( l_iCOunt == 0 )
        return;
    //POINT   l_Size = {*l_pClickMouseBehavior->GetWidth(),*l_pClickMouseBehavior->GetHeight()};
	POINT   l_Size = {(int)l_pClickMouseBehavior->GetCollisionRange().Width(),(int)l_pClickMouseBehavior->GetCollisionRange().Height()};
    Vector3 l_vStargPos = Vector3(m_vCollisionRange.x,m_vCollisionRange.y,0.f);

	for( int i=0;i<m_iNumColumn;++i )
	{
		Vector3 l_vPos;
		l_vPos.x = m_vGap.x*i+l_vStargPos.x;
		for( int j=0;j<m_iNumRow;++j )
		{
			l_vPos.y = m_vGap.y*j+l_vStargPos.y;
			int	l_iIndex = i*m_iNumRow+j;
			l_pClickMouseBehavior = GetObject(l_iIndex);
			if( l_pClickMouseBehavior )
			{
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
			}
			else
				return;
		}		
	}
}