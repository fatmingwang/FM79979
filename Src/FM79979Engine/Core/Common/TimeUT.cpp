#include "TimeUT.h"
#include <time.h>
#ifdef WIN32
#include <direct.h>
#endif
#if defined(IOS)
#include <mach/mach_time.h>
#include <sys/time.h>
#elif defined(WIN32)
#include "strsafe.h"

#pragma warning( disable : 4793 )
#pragma warning( disable : 4995 )
#elif defined(LINUX)
//#include <SDL2/SDL.h>
#include <sys/time.h>
#elif defined(ANDROID)
#include "jni.h"
#include <sys/time.h>
#endif
//from NamedTypedObject
extern uint64	GetGlobalUniqueID();
//#ifdef UWP
//struct timeval 
//{
//	long    tv_sec;         /* seconds */
//	long    tv_usec;        /* and microseconds */
//};
//#endif

#include "StringToStructure.h"
namespace UT
{
	sTimeAndFPS::sTimeAndFPS()
	{
		pTimevalStamp = new timeval();
		uiElpaseTime = 0;
		uiPreviousTime = 0;
		uiCurrentTime = 0;

		uiNumFrame = 0;
		fTimeFor1Sec = 0.f;
		memset(strFrameRate,0,sizeof(strFrameRate));
		sprintf(strFrameRate,"%s fps","0");
	}
	sTimeAndFPS::~sTimeAndFPS()
	{
		SAFE_DELETE(pTimevalStamp);
	}
	void	sTimeAndFPS::Update()//total game running time
	{
		gettimeofday(pTimevalStamp,0);
		uiPreviousTime = uiCurrentTime;
#if defined(WIN32)
		uiCurrentTime = GetTickCount();
#else//android IOS or linux and else
		struct timeval current;
		gettimeofday(&current, nullptr);
		uiCurrentTime = current.tv_sec * 1000 + current.tv_usec/1000;
#endif

		uiElpaseTime = uiCurrentTime-uiPreviousTime;
		fElpaseTime = (float)uiElpaseTime/1000.f;
		uiNumFrame++;
		fTimeFor1Sec += fElpaseTime;
		if(fTimeFor1Sec>=1.f)
		{
			fTimeFor1Sec -= (int)fTimeFor1Sec;
			sprintf(strFrameRate,"%i fps",uiNumFrame);
			uiNumFrame = 0;
		}
	}

	float	sTimeAndFPS::GetSecondDiff(sTimeAndFPS*e_pT1,sTimeAndFPS*e_pT2)
	{
		UINT	l_uiResult = e_pT1->uiCurrentTime - e_pT2->uiCurrentTime;
		float	l_fElpaseTime = (float)(double)l_uiResult/1000.f;
		return l_fElpaseTime;
	}

	float	sTimeAndFPS::GetSecondDiff(timeval e_timeval)
	{
		timeval	l_Result;
		timeval_diff(&l_Result,pTimevalStamp,&e_timeval);
		return (float)(l_Result.tv_sec+l_Result.tv_usec/1000000.f);
	}

	char* sTimeAndFPS::GetFPS()
	{
		return strFrameRate;
	}

	sTimeCounter::sTimeCounter(float e_fTargetTime)
	{
		SetTargetTime(e_fTargetTime);
		bLoop = false;
	}

	void	sTimeCounter::SetTargetTime(float e_fTargetTime)
	{
		bTragetTimrReached = false;
		fRestTime = e_fTargetTime;
		this->fTargetTime = e_fTargetTime;	
	}

	float	sTimeCounter::GetLERP()
	{
		float	l_fLERP = (1-fRestTime/fTargetTime);
		return l_fLERP>1?1:l_fLERP;
	}
	float	sTimeCounter::GetLERPAttenuation()
	{
		float l_fValue = fRestTime / fTargetTime;
		l_fValue *= l_fValue;
		return l_fValue;
	}

	void	sTimeCounter::SetLoop(bool e_bLoop)
	{
		this->bLoop = e_bLoop;
		bTragetTimrReached = false;
	}

	//m_TimToFire.Start(m_TimToFire.fRestTime*-1);//start with past time set test as nagetive
	void	sTimeCounter::Start(float e_fElpaseTime)
	{
		bTragetTimrReached = false;
		fRestTime = fTargetTime-e_fElpaseTime;
	}

	void	sTimeCounter::Update(float e_fElpaseTime)
	{
		if( bTragetTimrReached )
		{
			if( bLoop )
				bTragetTimrReached = false;
			else
				return;
		}
		fRestTime -= e_fElpaseTime;
		if( fRestTime<=0.f )
		{
			fRestTime = 0.f;
			bTragetTimrReached = true;
			if( bLoop )
			{
				//fRestTime = fTargetTime+fRestTime;
				fRestTime = fTargetTime;
				//if( -fRestTime>fTargetTime )
				//{
				//	fRestTime = fTargetTime;
				//}
				//else
				//	fRestTime = fTargetTime+fRestTime;
			}
		}
	}
	bool	sKeyTimeActive::IsActive(float e_fCurrentTime)
	{
		if( !bTriggered )
		{
			if( e_fCurrentTime>=fTargetTime ) 
			{
				bTriggered = true;
				return true;
			}
		}
		return false;
	}
#ifdef WIN32
	int gettimeofday(struct timeval *tv, struct timezone *tz)
	{
		FILETIME ft;
		unsigned __int64 tmpres = 0;
		static int tzflag;

		if (nullptr != tv)
		{
			GetSystemTimeAsFileTime(&ft);

			tmpres |= ft.dwHighDateTime;
			tmpres <<= 32;
			tmpres |= ft.dwLowDateTime;

			/*converting file time to unix epoch*/
			tmpres -= DELTA_EPOCH_IN_MICROSECS;
			tmpres /= 10;  /*convert into microseconds*/
			tv->tv_sec = (long)(tmpres / 1000000UL);
			tv->tv_usec = (long)(tmpres % 1000000UL);
		}

		if (nullptr != tz)
		{
			if (!tzflag)
			{
#ifndef UWP
				_tzset();
#endif
				tzflag++;
			}
			tz->tz_minuteswest = _timezone / 60;
			tz->tz_dsttime = _daylight;
		}

		return 0;
	}
#endif
	int64 currentTimeInMilliseconds()
	{
		return time(0);
	}

	long long	timeval_diff(struct timeval *difference,struct timeval *end_time,struct timeval *start_time)
	{
		struct timeval temp_diff;
		if( difference == nullptr )
			difference = &temp_diff;

		difference->tv_sec =end_time->tv_sec -start_time->tv_sec ;
		difference->tv_usec=end_time->tv_usec-start_time->tv_usec;
		/* Using while instead of if below makes the code slightly more robust. */
		while(difference->tv_usec<0)
		{
			difference->tv_usec+=1000000;
			difference->tv_sec -=1;
		}

		return 1000000LL*difference->tv_sec+difference->tv_usec;
	}
	std::wstring		TimeInMillisecondsToString(long long e_llTime)
	{
		struct tm *tm;
		char buf[200];
		time_t ltime = (time_t)e_llTime;
		/* convert time_t to broken-down time representation */
		tm = localtime(&ltime);
		/* format time days.month.year hour:minute:seconds */
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);

		return FATMING_CORE::ValueToStringW(buf);
	}

	std::string EpochTimeToString(int64 e_llTime)
	{
		struct tm* tm;
		char buf[200];
		time_t ltime = (time_t)e_llTime;
		/* convert time_t to broken-down time representation */
		tm = localtime(&ltime);
		if (!tm)
		{
			return "get localtime failed";
		}
		//tm->tm_year;
		//tm->tm_mon;
		//tm->tm_mday;
		//tm->tm_hour;
		//tm->tm_min;
		//tm->tm_sec;
		/* format time days.month.year hour:minute:seconds */
		strftime(buf, sizeof(buf), "%d.%m.%Y %H:%M:%S", tm);

		return FATMING_CORE::ValueToString(buf);
	}

	std::wstring SecondsToTimeW(double e_dbSeconds, bool e_bWithFormatString)
	{
		int	 l_iSeconds = (int)e_dbSeconds;
		auto l_iMin = l_iSeconds / 60;
		auto l_iHour = l_iMin / 60;
		auto l_iDay = l_iHour / 24;
		l_iSeconds %= 60;
		l_iMin %= 60;
		l_iHour %= 24;
		if(e_bWithFormatString)
			return UT::ComposeMsgByFormat(L"%dD%dH%dM%dS", l_iDay, l_iHour, l_iMin, l_iSeconds);
		return UT::ComposeMsgByFormat(L"%d,%2d,%2d,%2d", l_iDay, l_iHour, l_iMin, l_iSeconds);
	}
	std::string SecondsToTime(double e_dbSeconds, bool e_bWithFormatString)
	{
		int	 l_iSeconds = (int)e_dbSeconds;
		auto l_iMin = l_iSeconds / 60;
		auto l_iHour = l_iMin / 60;
		auto l_iDay = l_iHour / 24;
		l_iSeconds %= 60;
		l_iMin %= 60;
		l_iHour %= 24;
		if(e_bWithFormatString)
			return UT::ComposeMsgByFormat("%dD%dH%dM%dS", l_iDay, l_iHour, l_iMin, l_iSeconds);
		return UT::ComposeMsgByFormat("%d,%2d,%2d,%2d", l_iDay, l_iHour, l_iMin, l_iSeconds);
	}
	cTimeoutCallBackFunction::cTimeoutCallBackFunction()
	{
	}
	cTimeoutCallBackFunction::~cTimeoutCallBackFunction()
	{
	}
	uint64 cTimeoutCallBackFunction::SetTimeout(f_TimeoutCallbackFunction e_f_TimeoutCallbackFunction, float e_fTime)
	{
		uint64 l_ui64ID = GetGlobalUniqueID();
		sTimeAndFunction l_TimeAndFunction = { e_fTime,e_f_TimeoutCallbackFunction };
		m_IDAndCallbackFunctionMap.insert(std::make_pair(l_ui64ID, l_TimeAndFunction));
		return l_ui64ID;
	}
	bool cTimeoutCallBackFunction::RemoveTimeoutFunction(uint64 e_i64ID)
	{
		auto l_IT = m_IDAndCallbackFunctionMap.find(e_i64ID);
		if (l_IT != m_IDAndCallbackFunctionMap.end())
		{
			m_IDAndCallbackFunctionMap.erase(l_IT);
			return true;
		}
		return false;
	}
	bool cTimeoutCallBackFunction::Clear()
	{
		while (m_IDAndCallbackFunctionMap.size())
		{
			m_IDAndCallbackFunctionMap.erase(m_IDAndCallbackFunctionMap.begin());
		}
		return true;
	}
	void cTimeoutCallBackFunction::Update(float e_fElpaseTime)
	{
		std::vector<uint64> l_ReachTimeVector;
		for (auto l_IT = m_IDAndCallbackFunctionMap.begin(); l_IT != m_IDAndCallbackFunctionMap.end(); ++l_IT)
		{
			l_IT->second.fTime -= e_fElpaseTime;
			if (l_IT->second.fTime <= 0.f)
			{
				l_ReachTimeVector.push_back(l_IT->first);
				if (l_IT->second.TimeoutCallbackFunction)
				{
					l_IT->second.TimeoutCallbackFunction();
				}
			}
		}
		while (l_ReachTimeVector.size())
		{
			auto l_ui64ID = l_ReachTimeVector[0];
			m_IDAndCallbackFunctionMap.erase(l_ui64ID);
			l_ReachTimeVector.erase(l_ReachTimeVector.begin());
		}
	}
	//end namespace UT
}