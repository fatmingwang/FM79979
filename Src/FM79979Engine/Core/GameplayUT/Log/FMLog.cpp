#include "../../stdafx.h"
#include "FMLog.h"
#include "../BinaryFile.h"
#include "../../XML/StringToStructure.h"
#include "../../Image/GlyphFontRender.h"
#ifdef WIN32
#include <direct.h>
#endif
#if defined(ANDROID)//openAL,android.c
#include <android/log.h>
//#include "../Android/nv_egl_util.h"
//#include "../Android/JNIUtil.h"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG,"GameApp",__VA_ARGS__))
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO,"GameApp",__VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN,"GameApp",__VA_ARGS__))
#endif
using namespace FATMING_CORE;
namespace FMLog
{
	bool	g_bFMLogEnable = false;
	int64	g_i64LogFlag = 0xffffffffffffffff;
	FATMING_CORE::cBinaryFile*	g_pFMLogFile = nullptr;
	std::list<std::wstring>*	g_pLatestLog = nullptr;
	int							g_iKeepLogCount = 0;
	bool	LogFlagLevelCompare(int e_iLogFlagLevel)
	{
		if (!g_bFMLogEnable)
			return false;
		int64 lll = 0xfffffffffffffff;
		int64 l_i64Flag = ((int64)1) << e_iLogFlagLevel;
		int64 l_i64TargetFlag = l_i64Flag & 0xffffffffffffffff;
		if (!(l_i64TargetFlag&g_i64LogFlag))
			return false;
		return true;
	}

	void					Init()
	{
		g_bFMLogEnable = true;
		if (!g_pFMLogFile)
		{
			Log(L"log file",true);
			g_pFMLogFile = new FATMING_CORE::cBinaryFile();
			g_pFMLogFile->SetName(L"FMLogFile");
#ifdef WIN32
			int	l_iValue = _mkdir("Log");
#endif
			//if(-1 != l_iValue)
			{
				std::wstring	l_strSystemTime = L"Log/log_";
				l_strSystemTime += UT::GetSystemTimeForFile(true);
				l_strSystemTime += L".txt";
				std::string	l_strFileName = UT::WcharToChar(l_strSystemTime);
				g_pFMLogFile->Writefile(l_strFileName.c_str(), false, false);
			}
//#else
//			if (g_pFMLogFile)
//			{
//				g_pFMLogFile->Writefile("log.txt", false, false);
//				if (g_pFMLogFile->GetFile())
//					Log(L"create log.txt OK",true);
//				else
//					Log(L"create log.txt failed",true);
//			}
//#endif
		}
	}

	void					EnableKeepLatestLog(int e_iKeepLatestLogCount)
	{
		if (e_iKeepLatestLogCount > 0 && g_bFMLogEnable)
		{
			g_iKeepLogCount = e_iKeepLatestLogCount;
			SAFE_DELETE(g_pLatestLog);
			g_pLatestLog = new std::list<std::wstring>;
		}
	}

	std::list<std::wstring>	GetLatestLog()
	{
		std::list<std::wstring>l_strLog;
		if (g_pLatestLog)
		{
			l_strLog = *g_pLatestLog;
			g_pLatestLog->clear();
		}
		return l_strLog;
	}

	void					Destroy()
	{
		SAFE_DELETE(g_pFMLogFile);
		SAFE_DELETE(g_pLatestLog);
		g_bFMLogEnable = false;
	}

	void					Log(const char*e_str, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		//Log(UT::CharToWchar(e_str).c_str(), e_bWriteLog);
		//#ifdef DEBUG
#if defined(WIN32)
		std::wstring l_str = FATMING_CORE::ValueToStringW(e_str);
		printf(e_str);
		OutputDebugString(l_str.c_str());;
		OutputDebugString(L"\n");
#elif	defined(ANDROID)
		std::string	l_str = "GameApp___ ";
		l_str += ValueToString(e_str);
		LOGI("%s", l_str.c_str());
#elif	defined(IOS) || defined(WASM) || defined(LINUX)
		std::string l_str = ValueToString(e_str);		l_str += "\n";
		printf(l_str.c_str());
#endif
		if (g_pLatestLog)
		{
			if ((int)g_pLatestLog->size() + 1 >= g_iKeepLogCount)
			{
				g_pLatestLog->pop_back();
			}
			g_pLatestLog->push_front(UT::CharToWchar(e_str));
		}
		if (e_bWriteLog)
		{
			WriteLog(e_str);
		}
	}
	void					Log(const wchar_t*e_str, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		//#ifdef DEBUG
#if defined(WIN32)
		std::string l_str = UT::WcharToChar(e_str);		l_str += "\n";
		printf(l_str.c_str());
		OutputDebugString(e_str);;
		OutputDebugString(L"\n");
#elif	defined(ANDROID)
		std::string	l_str = "GameApp___ ";
		l_str += UT::WcharToChar(e_str);
		LOGI("%s", l_str.c_str());
#elif	defined(IOS)|| defined(WASM) || defined(LINUX)
		std::string l_str = UT::WcharToChar(e_str);		l_str += "\n";
		printf(l_str.c_str());
#endif
		if (g_pLatestLog)
		{
			if (g_pLatestLog->size() + 1 >= g_iKeepLogCount)
			{
				g_pLatestLog->pop_back();
			}
			g_pLatestLog->push_front(e_str);
		}
		if (e_bWriteLog)
		{
			WriteLog(e_str);
		}
	}

	void LogWithFlag(const wchar_t * e_str,int e_iLogFlagLevel, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		if (!LogFlagLevelCompare(e_iLogFlagLevel))
		{
			return;
		}
		Log(e_str,  e_bWriteLog);
	}

	void LogWithFlag(std::wstring e_str, int e_iLogFlagLevel, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		if (!LogFlagLevelCompare(e_iLogFlagLevel))
		{
			return;
		}
		Log(e_str.c_str(),  e_bWriteLog);
	}

	void LogWithFlag(std::string e_str, int e_iLogFlagLevel, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		if (!LogFlagLevelCompare(e_iLogFlagLevel))
		{
			return;
		}
		//Log(UT::CharToWchar(e_str).c_str(),  e_bWriteLog);
		Log(e_str.c_str(), e_bWriteLog);
	}

	void LogWithFlag(const char * e_str, int e_iLogFlagLevel, bool e_bWriteLog)
	{
		if (!g_bFMLogEnable)
			return;
		if (!LogFlagLevelCompare(e_iLogFlagLevel))
		{
			return;
		}
		//Log(UT::CharToWchar(e_str).c_str(),  e_bWriteLog);
		Log(e_str, e_bWriteLog);
	}

	void WriteLog(const wchar_t * e_strMessage)
	{
		if(g_pFMLogFile && g_bFMLogEnable)
			g_pFMLogFile->WriteToFileImmediatelyWithLine(e_strMessage);
	}

	void WriteLog(std::wstring e_strMessage)
	{
		WriteLog(e_strMessage.c_str());
	}

	void WriteLog(const char * e_strMessage)
	{
		if (g_pFMLogFile && g_bFMLogEnable)
			g_pFMLogFile->WriteToFileImmediatelyWithLine(e_strMessage);
	}

	void WriteLog(std::string e_strMessage)
	{
		WriteLog(e_strMessage.c_str());
	}

	void Render(int e_iPosX, int e_iPosY,FATMING_CORE::cGlyphFontRender*e_pGlyphFontRender, int e_iYGap)
	{
		if (!g_bFMLogEnable)
			return;
		int l_iPosY = e_iPosY;
		if (g_pLatestLog && e_pGlyphFontRender)
		{
			for (auto l_str : *g_pLatestLog)
			{
				e_pGlyphFontRender->RenderFont((float)e_iPosX, (float)l_iPosY, l_str);
				l_iPosY += e_iYGap;
			}
		}
	}

	void DoORForLogFlag(int e_iFlag)
	{
		int64 l_i64Flag = ((int64)(1)) << e_iFlag;
		g_i64LogFlag |= l_i64Flag;
	}

	void SetLogFlag(int64 e_i64Flag)
	{
		g_i64LogFlag = e_i64Flag;
	}
}

#define XMASSERT_LINE_STRING_SIZE 16
void XMAssert(const char* pExpression, const char* pFileName, unsigned int LineNumber)
{
#ifdef WIN32
#ifdef DEBUG
	char        aLineString[XMASSERT_LINE_STRING_SIZE];
	char*       pLineString;
	UINT        Line;

	aLineString[XMASSERT_LINE_STRING_SIZE - 2] = '0';
	aLineString[XMASSERT_LINE_STRING_SIZE - 1] = '\0';
	for (Line = LineNumber, pLineString = aLineString + XMASSERT_LINE_STRING_SIZE - 2;
		Line != 0 && pLineString >= aLineString;
		Line /= 10, pLineString--)
	{
		*pLineString = (CHAR)('0' + (Line % 10));
	}

#ifndef NO_OUTPUT_DEBUG_STRING
	FMLog::Log("Assertion failed: ",false);
	FMLog::Log(pExpression, false);
	FMLog::Log(", file ", false);
	FMLog::Log(pFileName, false);
	FMLog::Log(", line ", false);
	FMLog::Log(pLineString + 1, false);
	FMLog::Log("\r\n", false);
#else
	DbgPrint("Assertion failed: %s, file %s, line %d\r\n", pExpression, pFileName, LineNumber);
#endif//NO_OUTPUT_DEBUG_STRING
	__debugbreak();
#endif//DEBUG
#endif//WIN32
}