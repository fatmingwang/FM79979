#pragma once

void XMAssert(const char* pExpression, const char* pFileName, unsigned int LineNumber);
#ifdef WIN32
#ifdef DEBUG
#define XMASSERT(Expression) ((void)(Expression || (XMAssert(#Expression, __FILE__, __LINE__), 0)))
#else // !DEBUG
#define XMASSERT(Expression) ((void)0)
#endif // !DEBUG
#else
#define XMASSERT(Expression)  ((void)0)
#endif


//please d onot use 60 as flag setting or core log wiil be ignore?
#define	CORE_LOG_FLAG		60
namespace FMLog
{
	void					Init();
	void					Destroy();
	//1-64
	void					DoORForLogFlag(int e_iFlag);
	void					SetLogFlag(int64 e_i64Flag);
	//force to write
	void					Log(const char*e_str   , bool e_bWriteLog);
	void					Log(const wchar_t*e_str, bool e_bWriteLog);
	//
	void					LogWithFlag(const wchar_t*e_str,int e_iLogFlagLevel, bool e_bWriteLog = false);
	void					LogWithFlag(std::wstring e_str ,int e_iLogFlagLevel, bool e_bWriteLog = false);
	void					LogWithFlag(std::string e_str  ,int e_iLogFlagLevel, bool e_bWriteLog = false);
	void					LogWithFlag(const char*e_str   ,int e_iLogFlagLevel, bool e_bWriteLog = false);
	//Init called
	void					WriteLog(const wchar_t*e_strMessage);
	void					WriteLog(std::wstring e_strMessage);
	void					WriteLog(const char*e_strMessage);
	void					WriteLog(std::string e_strMessage);
}