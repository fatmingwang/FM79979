#include "../stdafx.h"
#include "../Image/PuzzleImage.h"
#include "../Image/SimplePrimitive.h"
#include "3DScroller.h"
#include "GameApp.h"
#include "ObjectXMLParser.h"
namespace FATMING_CORE
{
	TYPDE_DEFINE_MARCO(c3DScroller);
    sKeepObjectPositionAtCenter::sKeepObjectPositionAtCenter(float e_fTimeToCenter,Vector4 e_vRect,bool e_bDirectionIsHorizontal)
    {
        bDirectionIsHorizontal = e_bDirectionIsHorizontal;
        SetData(e_fTimeToCenter,e_vRect);
        bMovingToCenter = false;
        pvPos = 0;
    }
    void    sKeepObjectPositionAtCenter::SetData(float e_fTimeToCenter,Vector4 e_vRect)
    {
        vRect = e_vRect;
        GotoCenterTC.SetTargetTime(e_fTimeToCenter);
    }
    void    sKeepObjectPositionAtCenter::SetPosMoveToCenter(Vector3*e_pPos)
    {
        pvPos = e_pPos;
        //Vector3 l_vCenter((vRect.z-vRect.x)/2.f,(vRect.y-vRect.w)/2.f,0.f );
        //now ignore height
        Vector3 l_vStartpos(vRect.x,vRect.y,0.f );
        vDistanceToCenter = l_vStartpos-*pvPos;
        bMovingToCenter = true;
        GotoCenterTC.Start();
    }
    //if mouse up event is actived,and object is not at center,set the object back to center position
    void    sKeepObjectPositionAtCenter::Start()
    {
        Vector3 l_vCenter((vRect.z-vRect.x)/2.f,(vRect.y-vRect.w)/2.f,0.f );
        vDistanceToCenter = *pvPos-l_vCenter;
        if( vDistanceToCenter.Length()>2.f )
            bMovingToCenter = true;
        else
            bMovingToCenter = false;
        GotoCenterTC.Start();
    }
    //
    void    sKeepObjectPositionAtCenter::Update(float e_fElpaseTime)
    {
        if( bMovingToCenter )
        {
            GotoCenterTC.Update(e_fElpaseTime);
            if( GotoCenterTC.bTragetTimrReached )
            {
                vMovedDistance = Vector3(vRect.x-pvPos->x,vRect.y-pvPos->y,0.f);
                //*pvPos = Vector3(this->vRect.x,this->vRect.y,0.f);
                bMovingToCenter = false;
            }
            else
            {
                float   l_fScale = 1/GotoCenterTC.fTargetTime;
                vMovedDistance = vDistanceToCenter*e_fElpaseTime*l_fScale;
                //*pvPos += vMovedDistance;
            }
        }
    }

    //0 for not ye,1 for left 2 for right
    int sKeepObjectPositionAtCenter::IsOverCenterRange()
    {
		if( !pvPos )
			return 0;
        float   l_fOverRangeLength;
        if( bDirectionIsHorizontal )
        {
            l_fOverRangeLength = this->vRect.z-this->vRect.x;
            if( this->pvPos->x+l_fOverRangeLength*0.3<vRect.x )
                return 1;
            if( this->pvPos->x+l_fOverRangeLength*0.6>vRect.z )
                return 2;
        }
        else
        {
            l_fOverRangeLength = this->vRect.w-this->vRect.y;
            if( this->pvPos->y+l_fOverRangeLength*0.3<vRect.y )
                return 1;
            if( this->pvPos->y+l_fOverRangeLength*0.6>vRect.w )
                return 2;
        }
        return 0;
    }

    c3DScroller::c3DScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton,cClickMouseBehavior*e_pIcon,bool e_bHorizontal)
	:cTabPage(e_pPI,e_vTotalViewRect,e_pCancelButton,e_pIcon)
    {
        m_bDirectionIsHorizontal = e_bHorizontal;
        m_KeepObjectPositionAtCenter.bDirectionIsHorizontal = m_bDirectionIsHorizontal;
        m_iCurrentFocusIndex = -1;
        m_fRotationAngle = 30.f;
        m_iViewAbleCount = 7;
		m_vCollisionRange.x = m_vCollisionRange.y = 0.f;
		m_vCollisionRange.z = cGameApp::m_svGameResolution.x;
		m_vCollisionRange.w = cGameApp::m_svGameResolution.y;
        m_fImageScaleSize = 0.8f;
    }
	//c3DScroller::c3DScroller(c3DScroller*e_p3DScroller):cNamedTypedObjectVector<cBaseImage>(e_p3DScroller)
	//{
 //       m_bDirectionIsHorizontal = e_p3DScroller->m_bDirectionIsHorizontal;
 //       m_KeepObjectPositionAtCenter.bDirectionIsHorizontal = m_bDirectionIsHorizontal;
 //       m_iCurrentFocusIndex = e_p3DScroller->m_iCurrentFocusIndex;
 //       m_fRotationAngle = e_p3DScroller->m_fRotationAngle;
 //       m_iViewAbleCount = e_p3DScroller->m_iViewAbleCount;
 //       m_vCollisionRange = e_p3DScroller->m_vCollisionRange;
 //       m_fImageScaleSize = e_p3DScroller->m_fImageScaleSize;
	//}
    //
    c3DScroller::~c3DScroller()
    {
    
    }
    //
    void    c3DScroller::Init()
    {
		Vector4	l_vOriginalViewRect = this->m_vCollisionRange;
		//this one will auto rest so set it back
        cMouse_TouchWithCancelButton::Init();
		m_vCollisionRange = l_vOriginalViewRect;
        int l_iCount = this->Count();
        if( l_iCount == 0 )
            return;
        if( m_iViewAbleCount>l_iCount )
            m_iViewAbleCount = l_iCount;
        //make it as odd numeral
        if( m_iViewAbleCount%2 == 0 )
        {
            m_iViewAbleCount -= 1;
        }
        m_iCurrentFocusIndex = 0;
        cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(GetObject(0));
        POINT   l_Size = {l_pBaseImage->GetWidth(),l_pBaseImage->GetHeight()};
        if( m_bDirectionIsHorizontal )
            this->m_fEachImageDistance = l_Size.x/2.f;
        else
            this->m_fEachImageDistance = l_Size.y/2.f;
		Vector3 l_vPos = Vector3((this->m_vCollisionRange.z-m_vCollisionRange.x)/2.f-l_Size.x/2.f,(m_vCollisionRange.w-m_vCollisionRange.y)/2.f-l_Size.y/2.f,0.f);
		//Vector3 l_vPos = Vector3::Zero;
        Vector4 l_vFocusRange(l_vPos.x,
                              l_vPos.y,
                              l_vPos.x+l_Size.x,
                              l_vPos.y+l_Size.y);
        if( l_iCount )
        {
            for( int i=0;i<l_iCount;++i )
            {
                cBaseImage*l_pImage = dynamic_cast<cBaseImage*>(GetObject(i));
                l_pImage->SetPos(l_vPos);
                if( m_bDirectionIsHorizontal )
                    l_vPos.x += m_fEachImageDistance;
                else
                    l_vPos.y += m_fEachImageDistance;
            }
        }
        this->m_KeepObjectPositionAtCenter.SetData(0.3f,l_vFocusRange);
		m_KeepObjectPositionAtCenter.pvPos = this->GetObject(this->m_iCurrentFocusIndex)->GetLocalPositionPointer();
        m_KeepObjectPositionAtCenter.bMovingToCenter = false;
    }    
    //
    void    c3DScroller::SetRotationAngle(float e_fRotationAngle)
    {
        m_fRotationAngle = e_fRotationAngle;
        while( m_fRotationAngle>90.f )
            m_fRotationAngle -= 90.f;
        if(this->Count() == 0)
            this->m_fEachImageDistance = 1.f;
        else
        {
            float   l_fScale = m_fRotationAngle/90.f;
			cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(this->GetObject(0));
            this->m_fEachImageDistance = l_pBaseImage->GetWidth()*l_fScale;
        }
        
    }
    //first time mouse into
    void    c3DScroller::MouseDown(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseDown(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseDown(e_iPosX,e_iPosY);
    }
    //
    void    c3DScroller::MouseMove(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseMove(e_iPosX,e_iPosY);
		if(!cClickMouseBehavior::m_bCollided)
			return;
		cClickMouseBehavior::MouseMove(e_iPosX,e_iPosY);
		if( this->m_bOutOfWorkingZone )
			return;
		if(!InternalCollide(e_iPosX,e_iPosY))
			return;
        Vector3 l_vPos;
        int l_iCount = this->Count();
        for( int i=0;i<l_iCount;++i )
        {
            cBaseImage*l_pBaseImage = dynamic_cast<cBaseImage*>(this->m_ObjectList[i]);
			l_vPos = *l_pBaseImage->GetLocalPositionPointer();
            if(this->m_bDirectionIsHorizontal)
				l_vPos.x += this->m_MouseMoveData.FrameMovePos.x;
            else
                l_vPos.y += this->m_MouseMoveData.FrameMovePos.y;
            l_pBaseImage->SetPos(l_vPos);
        }        
    }
    //while mouse clicked is up
    //while current index collide with its image position and width height
    //while focus to left get current focus image position and collide with left one image's position,start to next start
    //while focus to right get current focus image's position with its width and get right image's position to collide,nd to next end
    void    c3DScroller::MouseUp(int e_iPosX,int e_iPosY)
    {
		if(!m_bEnable)
			return;
		m_pCurrentWorkingObject = 0;
        if( this->m_KeepObjectPositionAtCenter.bMovingToCenter )
            return;
		//if out of working zone do nothing.
		bool	l_bOutOfWorkingZone = m_bOutOfWorkingZone;
		cMouse_TouchWithCancelButton::OutOfWorkingMouseUp(e_iPosX,e_iPosY);
		cClickMouseBehavior::MouseUp(e_iPosX,e_iPosY);
		m_eObjectMouseBehavior = eOMB_NONE;
		if( l_bOutOfWorkingZone )
			return;
        if(this->m_MouseMoveData.DownUpDistance()>5.f)
        {
            m_iNewFocusIndex = m_iCurrentFocusIndex;
            this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(m_iCurrentFocusIndex)->GetLocalPositionPointer());
            return;
        }
        int l_iCount = this->Count();
        if( l_iCount )
        {
            if( e_iPosY<m_vCollisionRange.y||e_iPosY>m_vCollisionRange.w )
                return;
            //how many ite shoudl be left or right side.
            int l_iSideCount = (this->m_iViewAbleCount-1)/2;
            cBaseImage*l_pImage = dynamic_cast<cBaseImage*>(this->GetObject(0));
            //collide for center image
            if( e_iPosX>=this->m_KeepObjectPositionAtCenter.vRect.x&&e_iPosY>=this->m_KeepObjectPositionAtCenter.vRect.y )
            {
                if( e_iPosX<=this->m_KeepObjectPositionAtCenter.vRect.z&&e_iPosY<=this->m_KeepObjectPositionAtCenter.vRect.w )
                {
					this->m_pCurrentWorkingObject = this->GetObject(m_iCurrentFocusIndex);
					m_bClickMouseBehaviorSatisfied = true;
                    return;
                }
            }
            int l_iLeftStartCollisionIndex = this->m_iCurrentFocusIndex-l_iSideCount;
            int l_iEndCount = this->m_iCurrentFocusIndex+l_iSideCount;
            if( this->m_iCurrentFocusIndex == 0 )
            {
                l_iEndCount = m_iViewAbleCount-1;
            }
            else
            if( this->m_iCurrentFocusIndex == this->Count()-1 )
            {
                l_iLeftStartCollisionIndex = m_iCurrentFocusIndex-m_iViewAbleCount+1;
            }
            if( l_iEndCount >= this->Count() )
                l_iEndCount = this->Count()-1;
            if( l_iLeftStartCollisionIndex < 0 )
                l_iLeftStartCollisionIndex = 0;
            //focus to left,up
            Vector2 l_vPreviousImagePos(this->m_bDirectionIsHorizontal?m_KeepObjectPositionAtCenter.vRect.x:m_KeepObjectPositionAtCenter.vRect.z,this->m_bDirectionIsHorizontal?m_KeepObjectPositionAtCenter.vRect.w:m_KeepObjectPositionAtCenter.vRect.y);
            Vector2 l_vCurrentImagePos;
            for( int i=m_iCurrentFocusIndex-1;i>l_iLeftStartCollisionIndex-1;--i )
            {
                l_pImage = dynamic_cast<cBaseImage*>(this->GetObject(i));
                l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x,l_pImage->GetLocalPositionPointer()->y);
                if( e_iPosX>=l_vCurrentImagePos.x&&e_iPosY>=l_vCurrentImagePos.y )
                if( e_iPosX<=l_vPreviousImagePos.x&&e_iPosY<=l_vPreviousImagePos.y )
                {
                    m_iNewFocusIndex = i;
                    this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                    return;
                }
                l_vPreviousImagePos.x = this->m_bDirectionIsHorizontal?l_vCurrentImagePos.x:l_vPreviousImagePos.x;
                l_vPreviousImagePos.y = m_bDirectionIsHorizontal?l_vCurrentImagePos.y+l_pImage->GetHeight():l_vCurrentImagePos.y;
            }
            //focus to right,down
            if( m_bDirectionIsHorizontal )
            {
                l_vPreviousImagePos = Vector2(m_KeepObjectPositionAtCenter.vRect.z,m_KeepObjectPositionAtCenter.vRect.y);
                for( int i=m_iCurrentFocusIndex+1;i<l_iEndCount+1;++i )
                {
                    l_pImage = dynamic_cast<cBaseImage*>(this->GetObject(i));
                    //get last image's end position
                    l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x+l_pImage->GetWidth(),l_pImage->GetLocalPositionPointer()->y+l_pImage->GetHeight());
                    if( e_iPosX>=l_vPreviousImagePos.x&&e_iPosY>=l_vPreviousImagePos.y )
                    if( e_iPosX<=l_vCurrentImagePos.x&&e_iPosY<=l_vCurrentImagePos.y )
                    {
                        m_iNewFocusIndex = i;
                        this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                        return;
                    }
                    l_vPreviousImagePos.x = l_pImage->GetLocalPositionPointer()->x;
                    l_vPreviousImagePos.y = l_pImage->GetLocalPositionPointer()->y;
                }
            }
            else
            {
                l_vPreviousImagePos = Vector2(m_KeepObjectPositionAtCenter.vRect.x,m_KeepObjectPositionAtCenter.vRect.w);
                for( int i=m_iCurrentFocusIndex+1;i<l_iEndCount+1;++i )
                {
                    l_pImage = dynamic_cast<cBaseImage*>(this->GetObject(i));
                    //get last image's end position
                    l_vCurrentImagePos = Vector2(l_pImage->GetLocalPositionPointer()->x+l_pImage->GetWidth(),l_pImage->GetLocalPositionPointer()->y+l_pImage->GetHeight());
                    if( e_iPosX>=l_vPreviousImagePos.x&&e_iPosY>=l_vPreviousImagePos.y )
                    if( e_iPosX<=l_vCurrentImagePos.x&&e_iPosY<=l_vCurrentImagePos.y )
                    {
                        m_iNewFocusIndex = i;
                        this->m_KeepObjectPositionAtCenter.SetPosMoveToCenter(this->GetObject(i)->GetLocalPositionPointer());
                        return;
                    }
                    l_vPreviousImagePos.x = l_pImage->GetLocalPositionPointer()->x;
                    l_vPreviousImagePos.y = l_vCurrentImagePos.y;
                }
            }
        }
    }

    bool    c3DScroller::InternalCollide(int e_iPosX,int e_iPosY)
    {
        if( this->m_eObjectMouseBehavior == eOMB_NONE|| this->m_eObjectMouseBehavior == eOMB_UP )
        {
            if(this->m_vCollisionRange.x<=e_iPosX&&
                this->m_vCollisionRange.y<=e_iPosY&&
                this->m_vCollisionRange.z>=e_iPosX&&
                this->m_vCollisionRange.w>=e_iPosY)
                return true;
             else
                return false;
        }
        return true;
    }

    void    c3DScroller::Update(float e_fElpaseTime)
    {
		if(!m_bEnable)
			return;
		cMouse_TouchWithCancelButton::Update(e_fElpaseTime);
		int	l_iCount = this->Count();
        if(m_KeepObjectPositionAtCenter.bMovingToCenter)
        {
            m_KeepObjectPositionAtCenter.Update(e_fElpaseTime);
            for( int i=0;i<l_iCount;++i )
            {
                //if( i!= this->m_iCurrentFocusIndex )
				this->m_ObjectList[i]->SetLocalPosition(*this->m_ObjectList[i]->GetLocalPositionPointer()+m_KeepObjectPositionAtCenter.vMovedDistance);
            }
            if(m_KeepObjectPositionAtCenter.bMovingToCenter )
            {
                this->m_iCurrentFocusIndex = m_iNewFocusIndex;
            }
            return;
        }
        //if current focus object is over the center set it to prior or next
        int l_iOverCentRange = m_KeepObjectPositionAtCenter.IsOverCenterRange();
        if( l_iOverCentRange == 1 )
        {//find prior
            if(this->m_iCurrentFocusIndex+1<l_iCount)
            {
                this->m_KeepObjectPositionAtCenter.pvPos = this->GetObject(m_iCurrentFocusIndex+1)->GetLocalPositionPointer();
                m_iCurrentFocusIndex +=1 ;
            }
        }
        else
        if( l_iOverCentRange == 2 )
        {//find next
            if(this->m_iCurrentFocusIndex>0)
            {
                this->m_KeepObjectPositionAtCenter.pvPos = this->GetObject(m_iCurrentFocusIndex-1)->GetLocalPositionPointer();
                m_iCurrentFocusIndex -=1 ;
            }
        }
    }
    
    void    c3DScroller::Render()
    {
		int	l_iCount = this->Count();
        int l_iSideCount = (this->m_iViewAbleCount-1)/2;
        int l_iLeftStartCollisionIndex = this->m_iCurrentFocusIndex-l_iSideCount;
        int l_iEndCount = this->m_iCurrentFocusIndex+l_iSideCount;
        if( this->m_iCurrentFocusIndex == 0 )
        {
            l_iEndCount = m_iViewAbleCount-1;
        }
        else
        if( this->m_iCurrentFocusIndex == l_iCount-1 )
        {
            l_iLeftStartCollisionIndex = m_iCurrentFocusIndex-m_iViewAbleCount+1;
        }
        if( l_iEndCount >= l_iCount )
            l_iEndCount = l_iCount-1;
        if( l_iLeftStartCollisionIndex < 0 )
            l_iLeftStartCollisionIndex = 0;
        //left to focus
        cBaseImage*l_pBaseImage = 0;
		cImageButton*l_pImageButton = 0;
        Vector3 l_vPos;
        float   l_fXScale = 1.f;
        float   l_fYScale = 1.f;
		float	l_fSplidValue = 3.f;
        this->m_bDirectionIsHorizontal?l_fYScale=m_fImageScaleSize:l_fXScale=m_fImageScaleSize;
        for( int i=l_iLeftStartCollisionIndex;i<this->m_iCurrentFocusIndex;++i )
        {
            l_pImageButton = dynamic_cast<cImageButton*>(this->m_ObjectList[i]);
			cMatrix44	l_Originalmat = l_pImageButton->GetLocalTransform();
			l_vPos = l_pImageButton->GetWorldPosition();
            l_vPos.x -= l_pImageButton->GetWidth()/l_fSplidValue;
            l_vPos.y += l_pImageButton->GetHeight()/l_fSplidValue;
			cMatrix44	l_mat = cMatrix44::ScaleMatrix(Vector3(l_fXScale,l_fYScale,0.8f))*cMatrix44::TranslationMatrix(l_vPos);
			l_pImageButton->SetLocalTransform(l_mat);
			l_pImageButton->Render();
			l_pImageButton->SetLocalTransform(l_Originalmat);
            //this->m_ObjectList[i]->Render(cMatrix44::TranslationMatrix(l_vPos)*cMatrix44::YAxisRotationMatrix(D3DXToRadian(45))*cMatrix44::ScaleMatrix(Vector3(1,m_fImageScaleSize,0.8f)));
			//l_pBaseImage->Render();
        }
        //right to focus
        for( int i=l_iEndCount;i>m_iCurrentFocusIndex;--i )
        {
            //l_pBaseImage = dynamic_cast<cBaseImage*>(this->m_ObjectList[i]);
            //l_vPos = l_pBaseImage->GetWorldPosition();
            //l_vPos.x += *l_pBaseImage->GetWidth()/2.f;
            //l_vPos.y += *l_pBaseImage->GetHeight()/2.f;
            //l_pBaseImage->Render(cMatrix44::ScaleMatrix(Vector3(l_fXScale,l_fYScale,0.8f))*cMatrix44::TranslationMatrix(l_vPos));


            l_pImageButton = dynamic_cast<cImageButton*>(this->m_ObjectList[i]);
			cMatrix44	l_Originalmat = l_pImageButton->GetLocalTransform();
			l_vPos = l_pImageButton->GetWorldPosition();
            l_vPos.x += l_pImageButton->GetWidth()/l_fSplidValue;
            l_vPos.y += l_pImageButton->GetHeight()/l_fSplidValue;
			cMatrix44	l_mat = cMatrix44::ScaleMatrix(Vector3(l_fXScale,l_fYScale,0.8f))*cMatrix44::TranslationMatrix(l_vPos);
			l_pImageButton->SetLocalTransform(l_mat);
			l_pImageButton->Render();
			l_pImageButton->SetLocalTransform(l_Originalmat);
        }
        //render focus
		if( m_iCurrentFocusIndex != -1 )
			this->m_ObjectList[m_iCurrentFocusIndex]->Render();
		MiscRender();
    }
    
    void    c3DScroller::DebugRender()
    {
		int	l_iCount = this->Count();
        if(l_iCount)
        {
            cBaseImage*l_pBaseImage =  dynamic_cast<cBaseImage*>(GetObject(0));
            Vector2 l_vLinePos[4];
            POINT   l_Size = {l_pBaseImage->GetWidth(),l_pBaseImage->GetHeight()};
            Vector3 l_vPos((m_vCollisionRange.z-m_vCollisionRange.x)/2.f-l_Size.x/2.f,(m_vCollisionRange.w-m_vCollisionRange.y)/2.f-l_Size.y/2.f,0.f);
            if(this->m_bDirectionIsHorizontal)
            {
                l_vLinePos[0] = Vector2(l_vPos.x,0.f);
                l_vLinePos[1] = Vector2(l_vPos.x,m_vCollisionRange.w);
                l_vLinePos[2] = Vector2(l_vPos.x+l_Size.x,0.f);
                l_vLinePos[3] = Vector2(l_vPos.x+l_Size.x,m_vCollisionRange.w);
            }
            else
            {
                l_vLinePos[0] = Vector2(0.f,l_vPos.y);
                l_vLinePos[1] = Vector2(m_vCollisionRange.z,l_vPos.y);
                l_vLinePos[2] = Vector2(0.f,l_vPos.y+l_Size.y);
                l_vLinePos[3] = Vector2(m_vCollisionRange.z,l_vPos.y+l_Size.y);
            }
            GLRender::RenderLine((float*)l_vLinePos,4,Vector4(1,1,0,1),2);
            GLRender::RenderLine((float*)l_vLinePos[2],4,Vector4(1,1,0,1),2);
        }
	}
	c3DScroller*	c3DScroller::GetMe(TiXmlElement*e_pElement)
	{
		return dynamic_cast<c3DScroller*>(cTabPage::GetMe(e_pElement));
	}
	//<GirlScroller>
	//	<<c3DScroller ScrollRect="100,100,720,1080" ItemGap="20" Direction="Horizontal">
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
	//  </c3DScroller>
	//</ChoiceGirlPhase>
	//same as cScroller*cScroller::GetMe(TiXmlElement*e_pTiXmlElement)
	//c3DScroller*c3DScroller::GetMe(TiXmlElement*e_pTiXmlElement)
	//{
	//	ELEMENT_VALUE_ASSERT_CHECK(e_pTiXmlElement,c3DScroller::TypeID);
	//	c3DScroller*l_pScroller = 0;
	//	cPuzzleImage*l_pPI = 0;
	//	float	l_fItemGap = 0.f;
	//	eOrientation	l_eOrientation = eO_HORIZONTAL;
	//	Vector4	l_vViewRect(0,0,cGameApp::m_svGameResolution.x,cGameApp::m_svGameResolution.y);
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
	//	l_pScroller = new c3DScroller(l_pPI,l_vViewRect,l_pChancelButton,l_eOrientation==eO_HORIZONTAL,l_pIconButton);
	//	if( l_pItemsTiXmlElement )
	//	{
	//		l_pItemsTiXmlElement = l_pItemsTiXmlElement->FirstChildElement();
	//		while( l_pItemsTiXmlElement )
	//		{
	//			cRenderObject*l_pItem = GetRenderObjectBehavior(l_pItemsTiXmlElement);
	//			if(!l_pItem || !l_pScroller->AddObject((cBaseImage*)l_pItem))
	//			{
	//				cGameApp::OutputDebugInfoString(L"cScroller::GetMe(TiXmlElement*e_pTiXmlElement) GetRenderObjectBehavior failed");
	//			}
	//			l_pItemsTiXmlElement = l_pItemsTiXmlElement->NextSiblingElement();
	//		}
	//	}
	//	return l_pScroller;
	//}
//end namespace FATMING_CORE
}