#ifndef _BINARY_FILE_H_
#define _BINARY_FILE_H_
namespace FATMING_CORE
{
	//=======================================
	//
	//signed char: -127 to 127 (note, not -128 to 127; this accommodates 1's-complement platforms)
	//unsigned char: 0 to 255
	//"plain" char: -127 to 127 or 0 to 255 (depends on default char signedness)
	//signed short: -32767 to 32767
	//unsigned short: 0 to 65535
	//signed int: -32767 to 32767
	//unsigned int: 0 to 65535
	//signed long: -2147483647 to 2147483647
	//unsigned long: 0 to 4294967295
	//signed long long: -9223372036854775807 to 9223372036854775807
	//unsigned long long: 0 to 18446744073709551615
	//A C++ (or C) implementation can define the size of a type in bytes sizeof(type) to any value, as long as

	//the expression sizeof(type) * CHAR_BIT evaluates to the number of bits enough to contain required ranges, and
	//the ordering of type is still valid (e.g. sizeof(int) <= sizeof(long)).
	//The actual implementation-specific ranges can be found in <limits.h> header in C, or <climits> in C++ (or even better, templated std::numeric_limits in <limits> header).

	//For example, this is how you will find maximum range for int:

	//C:

	//#include <limits.h>
	//const int min_int = INT_MIN;
	//const int max_int = INT_MAX;
	//C++:

	//#include <limits>
	//const int min_int = std::numeric_limits<int>::min();
	//const int max_int = std::numeric_limits<int>::max();
	//=======================================
	class	cBinaryFile:public NamedTypedObject
	{
	protected:
		NvFile*				m_pFile;
		//for big endian and little endian
		int 				D0,D1,D2,D3,D4,D5,D6,D7;
		int 				W0,W1;
		void 				SwapToPc();
		void 				SwapToXBOX();
		char*				m_pData;
		//copy data into memory.
		//void				FetchData();
		bool				m_bBigEndian;
	public:
#ifdef WIN32
		HANDLE				m_FileHandle;
#endif
		cBinaryFile(bool e_bBigEndian = true);
		virtual ~cBinaryFile();

		void 				WriteToFile(int	  val );
		void 				WriteToFile(UINT  val );
		void 				WriteToFile(FLOAT val );
		//if u want to write double please use float or int64 instead
		//void 				WriteToFile(double val );
		void 				WriteToFile(unsigned long val );
		//void 				WriteToFile(wchar_t val );
		void 				WriteToFile(SHORT val );
		void 				WriteToFile(WORD val );
		void 				WriteToFile(long val );
		void 				WriteToFile(char* val );
		void 				WriteToFile(wchar_t* val );
		void 				WriteToFile(int64	  val );
		//create file
		//for cet to write for WIN32 to set flag
		virtual	bool		Writefile(const char*,bool e_bBinary,bool e_bForceToWrite);
		bool				Flush();
		//fclose
		void				CloseFile();
		//fopen
		//http://forum.codecall.net/topic/42319-what-mode-should-i-use-at-fopen-r-r-wb/
		bool				Openfile(const char*,const char*e_strformat = "r+");
		//from m_pFontData
		void*				GetDataFile(UINT e_uiStart);
		int					GetDataFileSize();
		//push data into e_pDestData
		void*				GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData);
		//below for log file
		void 				WriteToFileImmediatelyWithLine(const char* val,bool e_bTimeStamp = true);
		void 				WriteToFileImmediatelyWithLine(const wchar_t* val,bool e_bTimeStamp = true);
		void				CreateTimestamp();
		NvFile*				GetFile(){return m_pFile;}
		//pos for byte
		bool				SetFilePos(int e_iPos);
		static	int64		GetInt64(void*e_pData);
		static	float		GetFloat(void*e_pData);
		//static	double		GetDouble(void*e_pData);
		static	int			GetInt(void*e_pData);
		static	char		GetChar(void*e_pData);
	};

	//skip header to fetch specific data,for safety player file to avoid cheat.
	class cSkipHeaderBinaryFile:public cBinaryFile
	{
		int					m_iMagicNumberID;//if not match is's normal binary file
		int					m_iSkpiHeaderSize;
		bool				CreateHeader();
	public:
		cSkipHeaderBinaryFile(int e_iMagicID);
		~cSkipHeaderBinaryFile();
		bool				SetFilePos(int e_iPos);
		//this one will skip the rand header
		void*				GetDataFile(UINT e_uiStart);
		virtual	bool		Writefile(const char*e_strFileName,bool e_bBinary,bool e_bForceToWrite);
		int					GetSkipHeaderAndHMagicNumberHeader();
		void				AddGarbageEndFile();
	};
}
#endif