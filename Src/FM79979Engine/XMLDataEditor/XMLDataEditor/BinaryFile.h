//#ifndef BINARY_FILE_H
//#define BINARY_FILE_H
////=======================================
////
////=======================================
//class	cBinaryFile
//{
//	FILE*	m_pFile;
//	//for big endian and little endian
//	int D0,D1,D2,D3;
//	int W0,W1;
//	void SwapToPc();
//	void SwapToXBOX();
//	VOID*			m_pData;
//	//copy data into memory.
//	void	FetchData();
//	bool	m_bBigEndian;
//public:
//	cBinaryFile(bool e_bBigEndian = true);
//	~cBinaryFile();
//	//this is wrong,and I don't know why
//	VOID WriteToFile(INT   val );
//	VOID WriteToFile(FLOAT val );
//	VOID WriteToFile(DWORD val );
//	VOID WriteToFile(WCHAR val );
//	VOID WriteToFile(SHORT val );
//	VOID WriteToFile(WORD val );
//	VOID WriteToFile(char* val );
//	VOID WriteToFile(WCHAR* val );
//	VOID WriteToFile(const WCHAR* val ){WriteToFile((WCHAR*)val);}
//	//create file
//	bool	Writefile(char*);
//	//fclose
//	void	CloseFile();
//	//fopen
//	bool	Openfile(char*);
//	//from m_pFontData
//	void*	GetDataFile(UINT e_uiStart);
//	//push data into e_pDestData
//	void*	GetDataFileSegment(UINT e_uiStart,UINT e_uiSLength,void*e_pDestData);
//};
//
//#endif