#include "Scroller.h"
#include "../../GameApp/GameApp.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(cScroller);
	cScroller::cScroller():cTabPage()
	{
		m_eOrientation = eO_HORIZONTAL;
		this->m_vCollisionRange = cGameApp::m_spOpenGLRender->m_vViewPortSize;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
		m_fObjectGap = 0.f;	
	}

	cScroller::cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,cPuzzleImageUnit*e_pIcon,bool e_bHorizontal,Vector3* e_pvIconPos)
	:cTabPage(e_pPI,e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
    {
		m_eOrientation = e_bHorizontal?eO_HORIZONTAL:eO_VERTICAL;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
		m_fObjectGap = 0.f;
    }

	cScroller::cScroller(std::vector<cBaseImage*>*e_pImageList,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,bool e_bHorizontal,cPuzzleImageUnit*e_pIcon,Vector3* e_pvIconPos)
	:cTabPage(0,e_vTotalViewRect,e_pCancelButton,e_pIcon,e_pvIconPos)
	{
		m_fObjectGap = 0.f;
        m_eOrientation = e_bHorizontal?eO_HORIZONTAL:eO_VERTICAL;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
        this->AddObject(e_pImageList);	
	}
	//
	cScroller::cScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,cClickMouseBehavior*e_pIcon,bool e_bHorizontal)
	:cTabPage(e_pPI,e_vTotalViewRect,e_pCancelButton,e_pIcon)
	{
		m_eOrientation = e_bHorizontal?eO_HORIZONTAL:eO_VERTICAL;
		this->m_vCollisionRange = e_vTotalViewRect;
		m_RollBackTC.SetTargetTime(0.5f);
		m_vDisToRightPos = Vector3::Zero;
		m_fTotalLength = 0.f;
		m_bRollBack = false;
		m_fObjectGap = 0.f;
	}
    //
    cScroller::~cScroller()
    {
    
    }
	//
	bool	cScroller::CollideObjectIndex(cClickMouseBehavior*e_pClickMouseBehavior,int e_iPosX,int e_iPosY)
	{
		if( e_pClickMouseBehavior->IsEnable() )
			return e_pClickMouseBehavior->Collide(e_iPosX,e_iPosY);
		else
			return false;
		bool	l_bCollided = false;
		if( m_eOrientation == eO_HORIZONTAL )
		{
			if((float)e_iPosX>e_pClickMouseBehavior->GetLocalPositionPointer()->x)
			{
				if((float)e_iPosX<e_pClickMouseBehavior->GetLocalPositionPointer()->x+ e_pClickMouseBehavior->GetCollisionRange().Width() )
					l_bCollided = true;
			}
		}
		else
		{
			if((float)e_iPosY>e_pClickMouseBehavior->GetLocalPositionPointer()->y)
			{
				if((float)e_iPosY<e_pClickMouseBehavior->GetLocalPositionPointer()->y+e_pClickMouseBehavior->GetCollisionRange().Height() )
					l_bCollided = true;
			}					
		}
		return l_bCollided;
	}

    //
    void    cScroller::Init()
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
        Vector3 l_vPos = Vector3(m_vCollisionRange.x,m_vCollisionRange.y,0.f);
        int l_iCount = Count();
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                l_pClickMouseBehavior = GetObject(i);
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
				l_pClickMouseBehavior->Init();
                if( m_eOrientation == eO_HORIZONTAL )
				{
					m_fTotalLength += l_pClickMouseBehavior->GetCollisionRange().Width() + m_fObjectGap;
					l_vPos.x += l_pClickMouseBehavior->GetCollisionRange().Width() + m_fObjectGap;
				}
                else
				{
					m_fTotalLength += l_pClickMouseBehavior->GetCollisionRange().Height() + m_fObjectGap;
					l_vPos.y += l_pClickMouseBehavior->GetCollisionRange().Height()+m_fObjectGap;
				}
            }
        }
    }

	void cScroller::ArrangeObjectInitWithMultiLines(Vector2 e_vStartOffsetPos, Vector2 e_vObjectGap)
	{
		cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(0);
		Vector4	l_vOriginalViewRect = this->m_vCollisionRange;
		//this one will auto rest so set it back
		cMouse_TouchWithCancelButton::Init();
		m_vCollisionRange = l_vOriginalViewRect;
		m_bRollBack = false;
		m_fTotalLength = 0.f;
		if (this->m_pBG)
			m_pBG->Init();
		int l_iCOunt = this->Count();
		if (l_iCOunt == 0)
			return;
		//POINT   l_Size = {*l_pClickMouseBehavior->GetWidth(),*l_pClickMouseBehavior->GetHeight()};
		POINT   l_Size = { (int)l_pClickMouseBehavior->GetCollisionRange().Width(),(int)l_pClickMouseBehavior->GetCollisionRange().Height() };
		Vector3 l_vPos = Vector3(m_vCollisionRange.x+ e_vStartOffsetPos.x, m_vCollisionRange.y+ e_vStartOffsetPos.y, 0.f);
		int l_iCount = Count();
		if (l_iCount)
		{
			for (int i = 0; i < l_iCount; ++i)
			{
				l_pClickMouseBehavior = GetObject(i);
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
				l_pClickMouseBehavior->Init();
				if (m_eOrientation == eO_HORIZONTAL)
				{
					float l_fObjectHeight = l_pClickMouseBehavior->GetCollisionRange().Height();
					l_vPos.y += (l_fObjectHeight + e_vObjectGap.y);
					if (l_vPos.y + l_fObjectHeight >= m_vCollisionRange.w)
					{
						l_vPos.y = m_vCollisionRange.y+ e_vStartOffsetPos.y;
						float l_fObjectWidthAndGap = l_pClickMouseBehavior->GetCollisionRange().Width()+ e_vObjectGap.x;
						l_vPos.x += l_fObjectWidthAndGap;
						m_fTotalLength += l_fObjectWidthAndGap;
					}
				}
				else
				{
					float l_fObjectWidth = l_pClickMouseBehavior->GetCollisionRange().Width();
					l_vPos.x += l_fObjectWidth + e_vObjectGap.x;
					if (l_vPos.x + l_fObjectWidth >= m_vCollisionRange.z)
					{
						l_vPos.x = m_vCollisionRange.x+ e_vStartOffsetPos.x;
						float l_fObjectHeightAndGap = l_pClickMouseBehavior->GetCollisionRange().Height() + e_vObjectGap.y;
						l_vPos.y += l_fObjectHeightAndGap;
						m_fTotalLength += l_fObjectHeightAndGap;
					}
				}
			}
		}
	}

	bool	cScroller::IsScrollable()
	{
		int l_iCount = this->Count();
		if( l_iCount<2 )
			return false;
		//cImageButton*l_pClickMouseBehavior = (cImageButton*)this->GetObject(0);
		//float	l_fImageLength = 0.f;
		////check last if it's not at last pos
		//if( m_bDirectionIsHorizontal )
		//{
		//	l_fImageLength = (float)*l_pClickMouseBehavior->GetWidth();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.z-m_vCollisionRange.x)
		//	{//it less than total viewable length so do not scroller
		//		return false;
		//	}
		//}
		//else
		//{
		//	l_fImageLength = (float)*l_pClickMouseBehavior->GetHeight();
		//	if(l_iCount*l_fImageLength<=this->m_vCollisionRange.w-m_vCollisionRange.y)
		//	{
		//		return false;
		//	}
		//}
		return true;
	}
    //first time mouse into
    void    cScroller::MouseDown(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseDown(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
		//m_eObjectMouseBehavior = eOMB_FIRST_TIME_INTO;
		//if( this->m_bOutOfWorkingZone )
		//	return;
		m_bRollBack = false;
        int l_iCount = this->Count();
        if( l_iCount )
        {
			//if( this->m_MouseMoveData.DownUpDistance()<5.f )
			{
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(i);
					//fuck offset problem so I have to collide with fucking weired way
					//if(l_pClickMouseBehavior->CollideTexture(e_iPosX,e_iPosY,false))
					if(CollideObjectIndex(l_pClickMouseBehavior,e_iPosX,e_iPosY))
					{
						m_pCurrentMouseDownCollidedObject = l_pClickMouseBehavior;
						return;
					}
				}
			}
		}
    }
	//
    void    cScroller::MouseMove(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
		if( m_bRollBack )
			return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseMove(e_iPosX,e_iPosY);
		if(!cClickMouseBehavior::m_bCollided)
			return;
		cClickMouseBehavior::MouseMove(e_iPosX,e_iPosY);
		//if( this->m_eObjectMouseBehavior == eOMB_NONE )
		//	return;
		if( this->m_bOutOfWorkingZone )
			return;
        Vector3 l_vPos;
        int l_iCount = this->Count();
		if(l_iCount>1)
		{
			cClickMouseBehavior*l_pClickMouseBehavior = 0;
			for( int i=0;i<l_iCount;++i )
			{
				l_pClickMouseBehavior = this->m_ObjectList[i];
				l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
				if( m_eOrientation == eO_HORIZONTAL )
					l_vPos.x += (float)this->m_MouseMoveData.FrameMovePos.x;
				else
					l_vPos.y += (float)this->m_MouseMoveData.FrameMovePos.y;
				l_pClickMouseBehavior->SetLocalPosition(l_vPos);
			}
		}
    }
    //while mouse clicked is up
    //while current index collide with its image position and width height
    //while focus to left get current focus image position and collide with left one image's position,start to next start
    //while focus to right get current focus image's position with its width and get right image's position to collide,nd to next end
    void    cScroller::MouseUp(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
//		m_eObjectMouseBehavior = eOMB_NONE;
		if( m_bRollBack )
			return;
		//if out of working zone do nothing.
		bool	l_bOutOfWorkingZone = m_bOutOfWorkingZone;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseUp(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
		m_eObjectMouseBehavior = eOMB_NONE;
		if( l_bOutOfWorkingZone )
			return;
        int l_iCount = this->Count();
        if( l_iCount)
        {
			if( this->m_MouseMoveData.DownUpDistance()<15.f &&this->m_vCollisionRange.CollidePoint(e_iPosX,e_iPosY) )
			{
				int	l_iCurrentIndex = this->GetObjectIndexByPointer(m_pCurrentMouseDownCollidedObject);
				for( int i=0;i<l_iCount;++i )
				{
					//CollideTextureWithTransform
					//if(this->GetObject(i)->CollideTextureWithTransform(e_iPosX,e_iPosY,false))
					cClickMouseBehavior*l_pClickMouseBehavior = this->GetObject(i);
					//if(l_pClickMouseBehavior->CollideTexture(e_iPosX,e_iPosY,false))
					//fuck offset problem so I have to collide with fucking weired way
					if(CollideObjectIndex(l_pClickMouseBehavior,e_iPosX,e_iPosY))
					{
						if(l_iCurrentIndex == i )
						{
							this->m_bClickMouseBehaviorSatisfied = true;
							this->m_pCurrentWorkingObject = l_pClickMouseBehavior;
							return;
						}
						else
							m_pCurrentWorkingObject = 0;
						return;
					}
				}
				return;
			}
			if(!IsScrollable())
				return;
			m_vAddedDis = Vector3::Zero;
			//check first if it over range
			cClickMouseBehavior*l_pFirstObject = this->GetObject(0);
			//assert(cImageButton::TypeID == l_pClickMouseBehavior->Type() );
			Vector3	l_vPos = *l_pFirstObject->GetLocalPositionPointer();
			float	l_fImageLength = 0.f;
			float	l_fDidOfFirstToStart = 0.f;
			float	l_fDisOfLastToEnd = 0.f;
			if( m_eOrientation == eO_HORIZONTAL )
			{
				l_fImageLength = l_pFirstObject->GetCollisionRange().Width();
				l_fDidOfFirstToStart = l_vPos.x-this->m_vCollisionRange.x;
				m_vDisToRightPos = Vector3(-l_fDidOfFirstToStart,0.f,0.f);
			}
			else
			{
				l_fImageLength = l_pFirstObject->GetCollisionRange().Height();
				l_fDidOfFirstToStart = l_vPos.y-this->m_vCollisionRange.y;
				m_vDisToRightPos = Vector3(0.f,-l_fDidOfFirstToStart,0.f);
			}
			if( l_fDidOfFirstToStart>3.f )
			{
				if( l_iCount > 1 )
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;
				}
			}
			//if total length is not over slide range,check scroll to first or last
			//check last if it's not at last pos
			cClickMouseBehavior*l_pLasttObject = this->GetObject(this->Count()-1);
			l_vPos = *l_pLasttObject->GetLocalPositionPointer();
			if( m_eOrientation == eO_HORIZONTAL )
			{
				if(this->m_vCollisionRange.z>(l_vPos.x-l_pFirstObject->GetLocalPositionPointer()->x)+l_pLasttObject->GetCollisionRange().Width())
				{
					if( l_iCount > 1 )
					{
						this->m_bRollBack = true;
						this->m_RollBackTC.Start();
						return;
					}
				}
			}
			else
			{
				//if(this->m_vCollisionRange.w>(l_vPos.y-l_pFirstObject->GetLocalPositionPointer()->y)+l_pLasttObject->GetCollisionRange().Height())
				//{
				//	//l_fDisOfLastToEnd = l_vPos.y-this->m_vCollisionRange.w+l_pLasttObject->GetCollisionRange().Height();
				//	//this->m_vDisToRightPos = Vector3(0.f,-l_fDisOfLastToEnd,0.f);
				//	this->m_bRollBack = true;
				//	this->m_RollBackTC.Start();
				//	return;			
				//}			
			}

			if( m_eOrientation == eO_HORIZONTAL )
			{
				l_fDisOfLastToEnd = l_vPos.x-this->m_vCollisionRange.z+l_pLasttObject->GetCollisionRange().Width();
				this->m_vDisToRightPos = Vector3(-l_fDisOfLastToEnd,0.f,0.f);
			}
			else
			{
				l_fDisOfLastToEnd = l_vPos.y-this->m_vCollisionRange.w+l_pLasttObject->GetCollisionRange().Height();
				this->m_vDisToRightPos = Vector3(0.f,-l_fDisOfLastToEnd,0.f);
			}
			if( l_fDisOfLastToEnd < 3.f)
			{
				if( l_iCount > 1 )
				{
					this->m_bRollBack = true;
					this->m_RollBackTC.Start();
					return;
				}
			}
			//if down and up is smaller than 0.2f do a smooth scroller
			const float	l_fMinTime = 0.2f;
			const float	l_fMinPencentage = 0.0f;
			if( this->m_MouseMoveData.fDownToUpElpaseTime <l_fMinTime )
			{
				float	l_fViewRectLength;
				if( m_eOrientation == eO_HORIZONTAL )
					l_fViewRectLength = this->m_vCollisionRange.z-this->m_vCollisionRange.x;
				else
					l_fViewRectLength = this->m_vCollisionRange.w-this->m_vCollisionRange.y;
				Vector2	l_vDownUpDirection;
				float	l_fDistance = this->m_MouseMoveData.DownUpDistance(&l_vDownUpDirection);
				float	l_fPercentage = l_fDistance/l_fViewRectLength;
				if( l_fPercentage>1.f )
					l_fPercentage = 1.f;
				float	l_fLengthToScroll = m_fTotalLength * l_fPercentage;
				if( l_fPercentage>l_fMinPencentage )
				{
					if( m_eOrientation == eO_HORIZONTAL )
					{
						if(l_vDownUpDirection.x<0.f)//to right
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(-l_fLengthToScroll,0.f,0.f);
						}
						else//to left
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(l_fLengthToScroll,0.f,0.f);
						}
					}
					else
					{
						if(l_vDownUpDirection.y<0.f)//to down
						{
							if( l_fLengthToScroll>l_fDisOfLastToEnd )
								l_fLengthToScroll = l_fDisOfLastToEnd;
							m_vDisToRightPos = Vector3(0.f,-l_fLengthToScroll,0.f);
						}
						else//to top
						{
							if( l_fLengthToScroll>-l_fDidOfFirstToStart )
								l_fLengthToScroll = -l_fDidOfFirstToStart;
							m_vDisToRightPos = Vector3(0.f,l_fLengthToScroll,0.f);
						}

					}
					if( l_iCount > 1 )
					{
						this->m_bRollBack = true;
						this->m_RollBackTC.Start();
					}
				}
			}
        }
    }

    void    cScroller::Update(float e_fElpaseTime)
    {
		if(!m_bEnable)
			return;
		cMouse_TouchWithCancelButton::Update(e_fElpaseTime);
		if(this->m_pBG)
		{
			m_pBG->Update(e_fElpaseTime);
		}
		if(this->m_bRollBack)
        {
			m_RollBackTC.Update(e_fElpaseTime);
			Vector3	l_vDisToMove;
			if( m_RollBackTC.bTragetTimrReached )
			{
				
				l_vDisToMove = m_vDisToRightPos-m_vAddedDis;
				m_bRollBack = false;				
			}
			else
			{
				l_vDisToMove = this->m_vDisToRightPos*(e_fElpaseTime/m_RollBackTC.fTargetTime);
			}
			m_vAddedDis += l_vDisToMove;
			int	l_iCount = this->Count();
			for( int i=0;i<l_iCount;++i )
			{
				cClickMouseBehavior*l_pClickMouseBehavior = this->m_ObjectList[i];
				l_pClickMouseBehavior->SetLocalPosition(*l_pClickMouseBehavior->GetLocalPositionPointer()+l_vDisToMove);
			}
            return;
        }
		if( m_pCurrentWorkingObject )
			m_pCurrentWorkingObject->Update(e_fElpaseTime);
    }
    
    void    cScroller::Render()
    {
		int	l_iCount = this->Count();
		if( l_iCount == 0 )
			return;
		cClickMouseBehavior*l_pClickMouseBehavior = 0;
		Vector3	l_vPos;
		Vector4	l_vOriginalColor;
		if(this->m_pBG)
		{
			m_pBG->Render();
		}
		GLboolean	l_vSissorTest = false;
		int	l_SicissorRange[4];
		MyglGetBooleanv(GL_SCISSOR_TEST,&l_vSissorTest);
		if( !l_vSissorTest )
			MyGLEnable(GL_SCISSOR_TEST);
		else
			MyGLGetIntegerv(GL_SCISSOR_BOX,l_SicissorRange);
		Vector4	l_vViewRect = ViewRectToOpenGLScissor(m_vCollisionRange);
		glScissor((int)l_vViewRect.x,(int)l_vViewRect.y,(int)l_vViewRect.z,(int)l_vViewRect.w);
		int	l_iCurrentSelectedIndex = this->GetCurrentWorkingObjectIndex();
		if( m_eOrientation == eO_HORIZONTAL )
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pClickMouseBehavior = this->m_ObjectList[i];
				l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
				if(l_vPos.x+l_pClickMouseBehavior->GetCollisionRange().Width()>=this->m_vCollisionRange.x)
				{
					if( l_vPos.x>this->m_vCollisionRange.z )
					{
						break;
					}
					l_pClickMouseBehavior->Render();
					if( l_iCurrentSelectedIndex == i )
					{
						GLRender::RenderRectangle(*l_pClickMouseBehavior->GetLocalPositionPointer(),l_pClickMouseBehavior->GetCollisionRange().Width(),l_pClickMouseBehavior->GetCollisionRange().Height(),Vector4(1,1,0,1));
					}
				}
			}
		}
		else
		{
			for( int i=0;i<l_iCount;++i )
			{
				l_pClickMouseBehavior = this->m_ObjectList[i];
				l_vPos = *l_pClickMouseBehavior->GetLocalPositionPointer();
				float	l_fPosY = l_vPos.y+l_pClickMouseBehavior->GetCollisionRange().Height();
				if(l_fPosY >= this->m_vCollisionRange.y)
				{
					if( l_vPos.y>this->m_vCollisionRange.w )
					{
						break;
					}
					l_pClickMouseBehavior->Render();
					if( l_iCurrentSelectedIndex == i )
					{
						GLRender::RenderRectangle(*l_pClickMouseBehavior->GetLocalPositionPointer(),l_pClickMouseBehavior->GetCollisionRange().Width(),l_pClickMouseBehavior->GetCollisionRange().Height(),Vector4(1,1,0,1));
					}
				}
			}
		}
		if( !l_vSissorTest )
			MyGLDisable(GL_SCISSOR_TEST);
		else
			glScissor(l_SicissorRange[0],l_SicissorRange[1],l_SicissorRange[2],l_SicissorRange[3]);
		MiscRender();
    }
    
    void    cScroller::DebugRender()
    {
        if(this->Count())
        {
            Vector2 l_vLinePos[4];
			if( m_eOrientation == eO_HORIZONTAL )
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);
			}
			else
			{
				l_vLinePos[0] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.y);
				l_vLinePos[1] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.y);
				l_vLinePos[2] = Vector2(this->m_vCollisionRange.x,this->m_vCollisionRange.w);
				l_vLinePos[3] = Vector2(this->m_vCollisionRange.z,this->m_vCollisionRange.w);			
			}
			GLRender::RenderLine((float*)l_vLinePos,4,Vector4(1,1,0,1),2,cMatrix44::Identity,5);
			GLRender::RenderLine((float*)l_vLinePos[2],4,Vector4(1,1,0,1),2,cMatrix44::Identity,5);
        }
    }

	cScroller*		cScroller::GetMe(TiXmlElement*e_pElement)
	{
		return dynamic_cast<cScroller*>(cTabPage::GetMe(e_pElement));
	}
	//<GirlScroller>
	//	<<cScroller ScrollRect="100,100,720,1080" ItemGap="20" Direction="Horizontal">
	//		<ChancelButton>
	//			<ImageButton />
	//		<ChancelButton>
	//		<ScrollItem>
	//			<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="1">
	//			<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="2">
	//			<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="3">
	//			<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="4">
	//			<cPuzzleImageUnit PI="Image/Girl/ChoiceGirl.pi" PIUnit="5">
	//		</ScrollItem>
	//		<Icon>
	//			<cImageButton />
	//		</Icon>
	//</ChoiceGirlPhase>
	//cScroller*cScroller::GetMe(TiXmlElement*e_pTiXmlElement)
	//{
	//	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,cScroller::TypeID);
	//	cScroller*l_pScroller = 0;
	//	cPuzzleImage*l_pPI = 0;
	//	float	l_fItemGap = 0.f;
	//	eOrientation	l_eOrientation = eO_HORIZONTAL;
	//	Vector4	l_vViewRect(0,0,cGameApp::m_spOpenGLRender->m_vGameResolution.x,cGameApp::m_spOpenGLRender->m_vGameResolution.y);
	//	PARSE_ELEMENT_START(e_pTiXmlElement)
	//		COMPARE_NAME("PI")
	//		{
	//			l_pPI = cGameApp::GetPuzzleImageByFileName(l_strValue);
	//		}
	//		else
	//		COMPARE_NAME("ScrollRect")
	//		{
	//			l_vViewRect = VALUE_TO_VECTOR4;
	//		}
	//		else
	//		COMPARE_NAME("ItemGap")
	//		{
	//			l_fItemGap = VALUE_TO_FLOAT;
	//		}
	//		else
	//		COMPARE_NAME("Direction")
	//		{
	//			l_eOrientation = GetOrientation(l_strValue);
	//		}
	//	PARSE_NAME_VALUE_END
	//	e_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
	//	cClickMouseBehavior*l_pChancelButton = 0;
	//	cClickMouseBehavior*l_pIconButton = 0;
	//	TiXmlElement*l_pItemsTiXmlElement = 0;
	//	while( e_pTiXmlElement )
	//	{
	//		l_strValue = e_pTiXmlElement->Value();
	//		COMPARE_VALUE("ChancelButton")
	//		{
	//			l_pChancelButton = GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement());
	//		}
	//		else
	//		COMPARE_VALUE("Icon")
	//		{
	//			l_pIconButton = GetClickMouseBehavior(e_pTiXmlElement->FirstChildElement());
	//		}
	//		else
	//		COMPARE_VALUE("Items")
	//		{
	//			l_pItemsTiXmlElement = e_pTiXmlElement;
	//		}
	//		e_pTiXmlElement = e_pTiXmlElement->NextSiblingElement();
	//	}
	//	l_pScroller = new cScroller(l_pPI,l_vViewRect,l_pChancelButton,l_eOrientation==eO_HORIZONTAL,l_pIconButton);
	//	if( l_pItemsTiXmlElement )
	//	{
	//		l_pItemsTiXmlElement = l_pItemsTiXmlElement->FirstChildElement();
	//		while( l_pItemsTiXmlElement )
	//		{
	//			cRenderObject*l_pItem = GetRenderObjectBehavior(l_pItemsTiXmlElement);
	//			if(!l_pItem || !l_pScroller->AddObject((cBaseImage*)l_pItem))
	//			{
	//				FMLog::LogWithFlag(L"cScroller::GetMe(TiXmlElement*e_pTiXmlElement) GetRenderObjectBehavior failed");
	//			}
	//			l_pItemsTiXmlElement = l_pItemsTiXmlElement->NextSiblingElement();
	//		}
	//	}
	//	return l_pScroller;
	//}
//end namespace FATMING_CORE

//end namespace
};