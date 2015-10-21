#ifndef	_FATMING_TIME_ANIMATION_OBJECT_RULE
#define	_FATMING_TIME_ANIMATION_OBJECT_RULE
#include "RenderNode.h"
namespace FATMING_CORE
{
	extern void		SetupShaderViewProjectionMatrix(float*e_pfVPMatrix,bool e_bDoOrientation);
	extern float*	GetCurrentViewProjectionMatrix();
    //=====================
    //a base class for animtion control(LERP)
    //a start time for animation control and a position(matrix is better!?)
    //base render and update interface to be call for any inherit calss
    //three major funciton are internal start,render,update,u have to implement them.
    //some common function have to implement with timing base.
    //=====================
    class	cFMTimelineAnimationObject:public cRenderNode
    {
		bool			m_bInitCalled;
    protected:
	    //how long we have past
	    float			m_fPastTime;
	    //total time we are going to play
	    float			m_fTotalPlayTime;
		//not exactly need but for quick info
		float			m_fCurrentProgress;
	    //cue time to start
	    GET_SET_DEC(float,m_fStartTime,GetStartTime,SetStartTime);
	    GET_SET_DEC(bool,m_bStart,IsStart,SetStart);						//while the time is over start time the real start is start,not just call start mean it start
		GET_SET_DEC(bool,m_bStayAtLastFrame,IsStayAtLastFrame,SetStayAtLastFrame);
		//how many time we want to play,if the value is -1 it will be loop
	    int				m_iPlayCount;
		int				m_iCurrentPlayCount;
	    //===================================
		char*			m_pUserData;
		virtual	void	InternalUpdateByGlobalTime(float e_fGlobalTime) = 0;
    public:
	    cFMTimelineAnimationObject();
	    cFMTimelineAnimationObject(cFMTimelineAnimationObject*e_pFatmingAnimationRule);
	    virtual	~cFMTimelineAnimationObject();
		float*			GetStartTimePointer();
	    virtual void	Init();
	    virtual void	Update(float e_fElpaseTime);
	    virtual void	Render();
	    bool			IsAnimationLoop();
	    virtual bool	IsAnimationDone();
		//
		int				GetCurrentPlayCount();
		void			SetPlayCount(int e_iPlayCount);
	    //the time has been past
	    float			GetPastTime();
	    virtual	void	SetAnimationLoop(bool e_bLoop);
	    virtual	void	SetAnimationDone(bool e_bAnimationDone);
	    virtual	void	Stop();
		//
	    virtual	void	RearrangeTime(float e_fNewTime) = 0;
		virtual	void	RearrangeTimeByPercent(float e_fPercenttage) = 0;
	    virtual	void	InvertOrder() = 0;
	    //start and last time.if end timeis -1 mean always update
	    virtual	float	GetEndTime() = 0;
		virtual	void	Destroy(){}
		//for extra data
		virtual	void	SetData(void*e_pData,bool e_bForceReplace = true);

		virtual	void	UpdateByGlobalTime(float e_fGlobalTime);
    };
}
#endif