#ifndef	_TIME_UT_H_
#define	_TIME_UT_H_
#ifndef WIN32
#include <sys/time.h>
#else
#include <WinSock.h>
#endif

namespace UT
{
	struct sTimeAndFPS
	{
		timeval					TimevalStamp;
		//calculate	elpase time
		UINT					uiPreviousTime;		//previously frame time ticks
		UINT					uiCurrentTime;		//current frame time ticks
		UINT					uiElpaseTime;		//current frame substract previously frame = elpase time
		float					fElpaseTime;		//uiElpaseTime/1000.f
		//for frame rate
		UINT					uiNumFrame;			//per second frame rate
		float					fTimeFor1Sec;		//detect is time reached 1 sec

		char					strFrameRate[16];

		sTimeAndFPS();
		void					Update();
		char* 					GetFPS();
		char* 					GetDynamicFPS();
		static	float			GetSecondDiff(sTimeAndFPS*e_pT1,sTimeAndFPS*e_pT2);
		float					GetSecondDiff(timeval e_timeval);
	};
	//to calculate the destination time is reached or not,
	struct	sTimeCounter
	{
		bool					bLoop;//do not set this directly,instead by SetLoop
		float					fRestTime;
		float					fTargetTime;
		//while the target time is reached set to true,else false
		bool					bTragetTimrReached;
		sTimeCounter(){ bTragetTimrReached = true; fRestTime = fTargetTime = 1.f; bLoop = false; }
		sTimeCounter(float e_fTargetTime);
		void					Start(float e_fElpaseTime = 0.f);//it could be start by specific time that it have had past
		void					Update(float e_fElpaseTime);
		void					SetTargetTime(float e_fTargetTime);
		void					SetLoop(bool e_bLoop);
		//0 to 1.
		float					GetLERP();
		float					GetLERPAttenuation();
	};
	//
	//=============
	//only active once,if it has been actived,it won't be active until start is called again.
	//=============
	struct	sKeyTimeActive
	{
		bool				bTriggered;
		float				fTargetTime;
		//u can call this to reset active time
		void				Start(float e_fTargetTime){ fTargetTime = e_fTargetTime; bTriggered = false; }
		//ensure u have set dest time already
		void				Start(){ bTriggered = false; }
		bool				IsActive(float e_fCurrentTime);
	};
#ifdef WIN32
	#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
		#define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64
	#else
		#define DELTA_EPOCH_IN_MICROSECS  11644473600000000ULL
	#endif
	struct timezone 
	{
	  int  tz_minuteswest; /* minutes W of Greenwich */
	  int  tz_dsttime;     /* type of dst correction */
	};
	int					gettimeofday(struct timeval *tv, struct timezone *tz);
#endif
	int64				currentTimeInMilliseconds();
	long long			timeval_diff(timeval *difference,timeval *end_time,timeval *start_time);
	std::wstring		TimeInMillisecondsToString(long long e_llTime);
}
//end namespace UT
#endif