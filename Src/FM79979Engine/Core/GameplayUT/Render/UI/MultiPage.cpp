#include "MultiPage.h"
namespace FATMING_CORE
{
	cMultiPages::cMultiPages(cPuzzleImageUnit*e_pIcon,
							 cPuzzleImage*e_pPI,
							 cPuzzleImageUnit*e_pArrowButtonImage,Vector3 e_vNextPageButtonPos,Vector3 e_vPriorPageButtonPos,
							 Vector3 e_vStartPos,Vector2 e_vObjectDistance,
							int e_iRowCount,int e_iColumnCount,Vector4 e_vWorkingZone,cImageButton*e_pCancelButton)
							:cTabPage(0,e_vWorkingZone,e_pCancelButton,e_pIcon)
	{
		m_iCurentPageIndex = -1;
		m_iTotalPages = 1;
		this->m_bFromResource = true;
		this->m_ObjectList.reserve(e_iRowCount*e_iColumnCount);
		m_iColumn = e_iColumnCount;
		m_iRow = e_iRowCount;
		int	l_iCount = e_pPI->Count();
		Vector3	l_vPos = Vector3::Zero;
		int	l_iCurrentRowIndex = 0;
		int	l_iCurrentColumnIndex = 0;
		e_pArrowButtonImage->SetMirror(true);
		m_pPriorPageImageButton = new cImageButton(e_pArrowButtonImage,e_vPriorPageButtonPos);
		e_pArrowButtonImage->SetMirror(false);
		m_pNextPageImageButton = new cImageButton(e_pArrowButtonImage,e_vNextPageButtonPos);
		for( int i=0;i<l_iCount;++i )
		{
			if( l_iCurrentColumnIndex == e_iColumnCount )
			{
				++l_iCurrentRowIndex;
				l_iCurrentColumnIndex = 0;
				if( l_iCurrentRowIndex == e_iRowCount )
				{
					l_iCurrentRowIndex = 0;
					++m_iTotalPages;
				}
			}
			l_vPos.x = e_vStartPos.x+l_iCurrentColumnIndex*e_vObjectDistance.x;
			l_vPos.y = e_vStartPos.y+l_iCurrentRowIndex*e_vObjectDistance.y;
			cImageButton*l_pImageButton = new cImageButton(e_pPI->GetObject(i),l_vPos);
			this->m_ImageButtonObjects.AddObjectNeglectExist(l_pImageButton);
			++l_iCurrentColumnIndex;
		}

		//m_vWorkingRect = Vector4(e_vStartPos.x,e_vStartPos.y,
		//	e_vStartPos.x+e_vObjectDistance.x*e_iRowCount,
		//	e_vStartPos.y+e_vObjectDistance.y*e_iColumnCount);
		if( l_iCount>0 )
			TurnToPage(0);
	}

	cMultiPages::~cMultiPages()
	{
		SAFE_DELETE(m_pPriorPageImageButton);
		SAFE_DELETE(m_pNextPageImageButton);
	}

	void    cMultiPages::Init()
	{
		cTabPage::Init();
		this->TurnToPage(0);
	}

	void    cMultiPages::Update(float e_fElpaseTime)
	{
		cTabPage::Update(e_fElpaseTime);
		//int	l_iCollidedIndex = this->IsChildrenCollided();
		if( m_bClickMouseBehaviorSatisfied )
		{
			//prior
			if( m_pPriorPageImageButton == this->m_pCurrentWorkingObject )
			{
				PriorPage();
			}//next
			else
			if( m_pNextPageImageButton == this->m_pCurrentWorkingObject )
			{
				NextPage();
			}
		}
	}

	bool	cMultiPages::TurnToPage(int e_iPagse)
	{
		this->m_bClickMouseBehaviorSatisfied = false;
		m_pCurrentWorkingObject = 0;
		m_pPriorPageImageButton->Init();
		m_pNextPageImageButton->Init();
		if( m_iCurentPageIndex == e_iPagse )
			return false;
		if( e_iPagse<0 || e_iPagse>=this->m_iTotalPages )
			return false;
		m_iCurentPageIndex = e_iPagse;
		this->Destroy();
		int	l_iOnePageObjectCount = this->m_iColumn*this->m_iRow;
		int	l_iStatIndex = e_iPagse*l_iOnePageObjectCount;
		int	l_iEndIndex = l_iStatIndex+l_iOnePageObjectCount;
		if( l_iEndIndex > m_ImageButtonObjects.Count() )
			l_iEndIndex = m_ImageButtonObjects.Count();
		for(int i=l_iStatIndex;i<l_iEndIndex;++i)
		{
			this->AddObjectNeglectExist(m_ImageButtonObjects[i]);
		}
		this->AddObjectNeglectExist(m_pPriorPageImageButton);
		this->AddObjectNeglectExist(m_pNextPageImageButton);
		return true;
	}
	bool	cMultiPages::NextPage()
	{
		m_pNextPageImageButton->Init();
		m_pCurrentWorkingObject = 0;
		this->m_bClickMouseBehaviorSatisfied = false;
		int	l_iTargetIndex = m_iCurentPageIndex;
		if( l_iTargetIndex+1<this->m_iTotalPages )
		{
			++l_iTargetIndex;
		}
		else//last page
			return false;
		return TurnToPage(l_iTargetIndex);
	}
	bool	cMultiPages::PriorPage()
	{
		m_pPriorPageImageButton->Init();
		m_pCurrentWorkingObject = 0;
		this->m_bClickMouseBehaviorSatisfied = false;
		int	l_iTargetIndex = m_iCurentPageIndex;
		if( l_iTargetIndex-1>-1 )
		{
			--l_iTargetIndex;
		}
		else//last page
			return false;
		return TurnToPage(l_iTargetIndex);
	}
//end namespace FATMING_CORE
}