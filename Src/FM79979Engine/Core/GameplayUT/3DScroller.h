#ifndef _3D_SCROLLER_H_
#define _3D_SCROLLER_H_

#include "Mouse_Touch.h"
#include "Tabpage.h"
namespace FATMING_CORE
{
    class   cPuzzleImage;
    //we expect a object always stay at the center position
    //if object is move a littlet bit to right or left make this object back to center
    //and if the object is over the edge we set send a singnal it has over the edge
    //for now it only support horizontal
    struct  sKeepObjectPositionAtCenter
    {
        bool    bDirectionIsHorizontal;
        //Object's position
        Vector3*        pvPos;
        //the upperleft and lower right position.
        Vector4         vRect;
        //
        bool            bMovingToCenter;
        sTimeCounter    GotoCenterTC;
        //while stat is called set data for bMovingToCenter,GotoCenterTC,vDistanceToCenter
        Vector3         vDistanceToCenter;
        Vector3         vMovedDistance;
        sKeepObjectPositionAtCenter(float e_fTimeToCenter = 0.5f,Vector4 e_vRect = Vector4(0,0,100,100),bool e_bDirectionIsHorizontal = true);
        void    SetData(float e_fTimeToCenter,Vector4 e_vRect);
        //
        void    SetPosMoveToCenter(Vector3*e_pPos);
        //if mouse up event is actived,and object is not at center,set the object back to center position
        void    Start();
        //
        void    Update(float e_fElpaseTime);
        //0 for not ye,1 for left,up 2 for right,down
        int IsOverCenterRange();
    };
    //direction only for horizontal.
    //each image collision range will collpase 1/3 for rotation
    class   c3DScroller:public cTabPage//cClickMouseBehavior,public cNamedTypedObjectVector<cBaseImage>
    {
        bool    m_bDirectionIsHorizontal;//if true horizontal else vertical
        //only scale image height
        float   m_fImageScaleSize;
        int m_iNewFocusIndex;
        sKeepObjectPositionAtCenter m_KeepObjectPositionAtCenter;
        //
        int		m_iCurrentFocusIndex;
        //how many images show at screen,we also could collide image from this parameter.
        int		m_iViewAbleCount;
        //it index is bigger than m_iViewAbleCount,m_fRotationAngle is nagative or postive.
        float   m_fRotationAngle;
        //basicly it's viewport size,but it's possible has orther game UI,so make a area to define it would be better!.
        //Vector4 m_vTotalRect;
        //each image's distance,depend on image's width
        float   m_fEachImageDistance;
        //
        virtual bool    InternalCollide(int e_iPosX,int e_iPosY);
    public :
		DEFINE_TYPE_INFO();
        //if e_pPI is nullptr,uadd images by yourself or add pi later
        c3DScroller(cPuzzleImage*e_pPI,Vector4 e_vTotalViewRect,cClickMouseBehavior*e_pCancelButton = 0,cClickMouseBehavior*e_pIcon = 0,bool e_bHorizontal = true);
		//c3DScroller(c3DScroller*e_p3DScroller);
		//CLONE_MYSELF(c3DScroller);
		//static c3DScroller*GetMe(TiXmlElement*e_pElement);
        virtual ~c3DScroller();
        //
        float   GetRotationAngle(){return m_fRotationAngle;}
        void    SetRotationAngle(float e_fRotationAngle);
        //Vector4 GetTotalRect(){return m_vTotalRect;}
        //void    SetTotalRect(Vector4 e_vPos);
        //the index we focus,and it also is the index for last decision.
        int     GetCurrentFocusIndex(){return m_iCurrentFocusIndex;}
        //set data to correct
        virtual void    Init();
        virtual	void    Update(float e_fElpaseTime);
        void    Render();
        void    DebugRender();
        //
        //if mouse up and the position is not similar,it's mean the mouse move from the other,so ignore it
        virtual void    MouseMove(int e_iPosX,int e_iPosY);
        virtual void    MouseDown(int e_iPosX,int e_iPosY);
        virtual void    MouseUp(int e_iPosX,int e_iPosY);
		static c3DScroller*	GetMe(TiXmlElement*e_pElement);
    };
//end namespace FATMING_CORE
}

#endif