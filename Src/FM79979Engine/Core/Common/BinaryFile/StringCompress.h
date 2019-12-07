#pragma once
#include <string>
#include "../../Android/nv_file.h"

#define	HUFFMAN_MAGIC_ID	79979

bool	CompressHuffman(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen);
bool	DecompressHuffman(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen);

bool	CompressHuffmanToFile(unsigned char *pSrc, int nSrcLen, const char*e_strFileName,int e_iMagicID);
bool	DecompressHuffmanToFile(unsigned char *pSrc, int nSrcLen,const  char*e_strFileName);

bool	FileCompressHuffman(const char*e_strTargetFile,const char*e_strOutputFileName,int e_iMagicID = HUFFMAN_MAGIC_ID,bool e_bForceIfFileIsInlegal = true);
bool	FileDempressHuffman(const char*e_strTargetFile,const char*e_strOutputFileName,int e_iMagicID = HUFFMAN_MAGIC_ID,bool e_bForceIfFileIsInlegal = true);
//use it only while after fopen and nothing done yet.
bool	IsCompressFile(NvFile*e_pFile,int e_iMagicID = HUFFMAN_MAGIC_ID);
//to value
std::wstring	DecodeStringFromeValue(const wchar_t*e_strEncrypt, const wchar_t*e_strCharacterToSplit);
std::wstring	EncodeStringToValue(const wchar_t*e_strString, const wchar_t*e_strCharacterToSplit);