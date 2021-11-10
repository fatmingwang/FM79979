#include <stdlib.h>
#include <iostream>
#include "../Utility.h"
#include "../StringToStructure.h"
#ifndef IOS
#include <malloc.h>
#endif
#ifdef WIN32
#pragma warning( disable : 4996 )
#endif

#include "../../Android/nv_file.h"
#include "StringCompress.h"

//////////////////////////////////////////////////////////////////////
//https://www.codeproject.com/Articles/9021/Simple-and-fast-Huffman-coding?msg=4226603
class CHuffmanNode
{
public:
	CHuffmanNode() {	memset(this, 0, sizeof(CHuffmanNode));	}

	int nFrequency;	// must be first for unsigned char align
	unsigned char byAscii;
	unsigned long dwCode;
	int nCodeLength;
	CHuffmanNode *pParent, *pLeft, *pRight;
};

int frequencyCompare(const void *elem1, const void *elem2 )
{
	CHuffmanNode *pNodes[2] = {	(CHuffmanNode*)elem1, (CHuffmanNode*)elem2 };
	if(pNodes[0]->nFrequency == pNodes[1]->nFrequency)
		return 0;
	return pNodes[0]->nFrequency < pNodes[1]->nFrequency ? 1 : -1;
}

int asciiCompare(const void *elem1, const void *elem2 )
{
	return ((CHuffmanNode*)elem1)->byAscii > ((CHuffmanNode*)elem2)->byAscii ? 1 : -1;
}

CHuffmanNode* PopNode(CHuffmanNode *pNodes[], int nIndex, bool bRight)
{
	CHuffmanNode* pNode = pNodes[nIndex];
	pNode->dwCode = bRight;
	pNode->nCodeLength = 1;
	return pNode;
}

void SetNodeCode(CHuffmanNode* pNode)
{
	CHuffmanNode* pParent = pNode->pParent;
	while(pParent && pParent->nCodeLength)
	{
		pNode->dwCode <<= 1;
		pNode->dwCode |= pParent->dwCode;
		pNode->nCodeLength++;
		pParent = pParent->pParent;
	}
}

int GetHuffmanTree(CHuffmanNode nodes[], bool bSetCodes = true)
{
	CHuffmanNode* pNodes[256], *pNode;
	// add used ascii to Huffman queue
	int nNodeCount = 0;
	for(int nCount = 0; nCount < 256 && nodes[nCount].nFrequency; nCount++)
		pNodes[nNodeCount++] = &nodes[nCount];
	int nParentNode = nNodeCount, nBackNode = nNodeCount-1;
	while(nBackNode > 0)
	{
		// parent node
		pNode = &nodes[nParentNode++];
		// pop first child
		pNode->pLeft = PopNode(pNodes, nBackNode--, false);
		// pop second child
		pNode->pRight = PopNode(pNodes, nBackNode--, true);
		// adjust parent of the two poped nodes
		pNode->pLeft->pParent = pNode->pRight->pParent = pNode;
		// adjust parent frequency
		pNode->nFrequency = pNode->pLeft->nFrequency + pNode->pRight->nFrequency;
		// insert parent node depending on its frequency
		int i;
		for( i= nBackNode; i >= 0; i--)
			if(pNodes[i]->nFrequency >= pNode->nFrequency)
				break;
#if defined(__LP64__) || defined(_WIN64) || defined(WASM)
		memmove(pNodes + i + 2, pNodes + i + 1, (nBackNode - i) * sizeof(void*));//64 bit,debug by jiangyong
#else
		memmove(pNodes+i+2, pNodes+i+1, (nBackNode-i)*sizeof(int));
#endif
		pNodes[i+1] = pNode;
		nBackNode++;
	}
	// set tree leaves nodes code
	int nCount;
	if(bSetCodes)
		for(nCount = 0; nCount < nNodeCount; nCount++)
			SetNodeCode(&nodes[nCount]);

	return nNodeCount;
}

bool CompressHuffman(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen)
{
	CHuffmanNode nodes[511];
	// initialize nodes ascii
	for (int nCount = 0; nCount < 256; nCount++)
	{
		nodes[nCount].byAscii = (unsigned char)nCount;
	}
	// get ascii frequencies
	int nCount;
	for(nCount = 0; nCount < nSrcLen; nCount++)
		nodes[pSrc[nCount]].nFrequency++;
	// sort ascii chars depending on frequency
	qsort(nodes, 256, sizeof(CHuffmanNode), frequencyCompare);
	// construct Huffman tree
	int nNodeCount = GetHuffmanTree(nodes);
	// construct compressed buffer
	int nNodeSize = sizeof(unsigned long)+sizeof(unsigned char);
	nDesLen = nSrcLen+nNodeCount*nNodeSize+sizeof(unsigned int*)+sizeof(unsigned char);
	pDes = (unsigned char*)malloc(nDesLen);
	unsigned char *pDesPtr = pDes;
	memset(pDesPtr, 0, nDesLen);
	// save source buffer length at the first unsigned long
	*(unsigned long*)pDesPtr = nSrcLen;
	pDesPtr += sizeof(unsigned long);
	// save Huffman tree leaves count-1 (as it may be 256)
	*pDesPtr = (unsigned char)(nNodeCount-1);
	pDesPtr += sizeof(unsigned char);
	// save Huffman tree used leaves nodes
	for(nCount = 0; nCount < nNodeCount; nCount++)
	{	// the array sorted on frequency so used nodes come first
		memcpy(pDesPtr, &nodes[nCount], nNodeSize);
		pDesPtr += nNodeSize;
	}
	// sort nodes depending on ascii to can index nodes with its ascii value
	qsort(nodes, 256, sizeof(CHuffmanNode), asciiCompare);

	int nDesIndex = 0;
	// loop to write codes
	for(nCount = 0; nCount < nSrcLen; nCount++)
	{
		*(unsigned long*)(pDesPtr+(nDesIndex>>3)) |= nodes[pSrc[nCount]].dwCode << (nDesIndex&7);
		nDesIndex += nodes[pSrc[nCount]].nCodeLength;
	}
	// update destination length
	nDesLen = (int)((pDesPtr-pDes)+(nDesIndex+7)/8);
	try
	{
		pDes = (unsigned char*)realloc(pDes, nDesLen);
	}
	catch(std::exception e)
	{
		if( pDes )
			free(pDes);
		return false;
	}

	return true;
}

bool DecompressHuffman(unsigned char *pSrc, int nSrcLen, unsigned char *&pDes, int &nDesLen)
{
	// copy destination final length
	nDesLen = *(unsigned long*)pSrc;
	// allocate buffer for decompressed buffer
	pDes = (unsigned char*)malloc(nDesLen+1);
	int nNodeCount = *(pSrc+sizeof(unsigned long))+1;
	// initialize Huffman nodes with frequency and ascii
	CHuffmanNode nodes[511], *pNode;
	int nNodeSize = sizeof(unsigned long)+sizeof(unsigned char), nSrcIndex = nNodeSize;
	for(int nCount = 0; nCount < nNodeCount; nCount++)
	{
		memcpy(&nodes[nCount], pSrc+nSrcIndex, nNodeSize);
		nSrcIndex += nNodeSize;
	}
	// construct Huffman tree
	GetHuffmanTree(nodes, false);
	// get Huffman tree root
	CHuffmanNode *pRoot = &nodes[0];
	while(pRoot->pParent)
		pRoot = pRoot->pParent;

	int nDesIndex = 0;
	unsigned long nCode;
	nSrcIndex <<= 3;	// convert from bits to unsigned chars
	while(nDesIndex < nDesLen)
	{
		nCode = (*(unsigned long*)(pSrc+(nSrcIndex>>3)))>>(nSrcIndex&7);
		pNode = pRoot;
		while(pNode->pLeft)	// if node has pLeft then it must has pRight
		{	// node not leaf
			pNode = (nCode&1) ? pNode->pRight : pNode->pLeft;
			nCode >>= 1;
			nSrcIndex++;
		}
		pDes[nDesIndex++] = pNode->byAscii;
	}

	return true;
}


bool	CompressHuffmanToFile(unsigned char *pSrc, int nSrcLen,const  char*e_strFileName,int e_iMagicID)
{
	// open destination buffer
	NvFile*l_pFile = MyFileOpen(e_strFileName,"wb");
	if( !l_pFile )
		return false;
	unsigned char *pDes = 0;
	int nDesLen;
	bool	l_bResult = CompressHuffman(pSrc, nSrcLen, pDes, nDesLen);
	if( l_bResult )
	{
		// write file buffer	//
		int	l_iHeaderID = e_iMagicID;
		NvFWrite(&l_iHeaderID,1,sizeof(int),l_pFile);
		//
		NvFWrite(pDes,sizeof(char),nDesLen,l_pFile);
		NvFClose(l_pFile);
		//for test
		//DecompressHuffmanToFile(pDes,nDesLen,"D:/Fish/FM79979/Media/Fish/Image/28825252.png",e_iMagicID)
		//DecompressHuffmanToFile(pDes, nDesLen, "C:/Users/fatming/Desktop/28825252.txt");
	}
	SAFE_DELETE_ARRAY(pDes);
	return l_bResult;
}

bool	DecompressHuffmanToFile(unsigned char *pSrc, int nSrcLen,const  char*e_strFileName)
{
	// open destination buffer
	NvFile*l_pFile = MyFileOpen(e_strFileName,"wb");
	if( !l_pFile )
		return false;
	unsigned char *pDes = 0;
	int nDesLen;
	bool	l_bResult = DecompressHuffman(pSrc, nSrcLen, pDes, nDesLen);
	if( l_bResult )
	{
		NvFWrite(pDes,sizeof(char),nDesLen,l_pFile);
	}
	SAFE_DELETE_ARRAY(pDes);
	NvFClose(l_pFile);
	return l_bResult;
}


bool	FileCompressHuffman(const char*e_strTargetFile,const char*e_strOutputFileName,int e_iMagicID,bool e_bForceIfFileIsInlegal)
{
	NvFile*l_pFile = MyFileOpen(e_strTargetFile,"rb");
	if( !l_pFile )
		return false;
	int nFileLength = UT::GetFileSize(l_pFile);
	unsigned char *pFileBuffer = new unsigned char[nFileLength];
	NvFRead(pFileBuffer,sizeof(char),nFileLength,l_pFile);
	if( IsCompressFile(l_pFile,e_iMagicID) && e_bForceIfFileIsInlegal )
	{//its compress file copy file
		NvFile*l_pFile2 = MyFileOpen(e_strOutputFileName,"wb");
		if( l_pFile2 )
		{
			NvFWrite(pFileBuffer,sizeof(char),nFileLength,l_pFile2);
			NvFClose(l_pFile2);
			return true;
		}
		return false;
	}
	NvFClose(l_pFile);
	bool	l_bResult = CompressHuffmanToFile(pFileBuffer,nFileLength,e_strOutputFileName,e_iMagicID);
	free(pFileBuffer);
	return l_bResult;
}

bool	FileDempressHuffman(const char*e_strTargetFile,const char*e_strOutputFileName,int e_iMagicID,bool e_bForceIfFileIsInlegal)
{
	NvFile*l_pFile = MyFileOpen(e_strTargetFile,"rb");
	if( !l_pFile )
		return false;
	int nFileLength = UT::GetFileSize(l_pFile);
	unsigned char *pFileBuffer = new unsigned char[nFileLength];
	NvFRead(pFileBuffer,sizeof(char),nFileLength,l_pFile);
	NvFClose(l_pFile);
	//
	int	l_iHeaderID = e_iMagicID;
	bool	l_bResult = false;
	if( *(int*)pFileBuffer == l_iHeaderID )
	{
		unsigned char*l_pSkipHeaderData = pFileBuffer + sizeof(int);
		l_bResult = DecompressHuffmanToFile(l_pSkipHeaderData,nFileLength-sizeof(int),e_strOutputFileName);
		//DecompressHuffman(l_pSkipHeaderData, nFileLength-sizeof(int), pDes, nDesLen);
	}
	else
	if( e_bForceIfFileIsInlegal )
	{
		NvFile*l_pFile2 = MyFileOpen(e_strOutputFileName,"wb");
		if( l_pFile2 )
		{
			NvFWrite(pFileBuffer,sizeof(char),nFileLength,l_pFile2);
			NvFClose(l_pFile2);
			return true;
		}
		return false;
	}
	delete[] pFileBuffer;
	return l_bResult;
}

bool	IsCompressFile(NvFile*e_pFile,int e_iMagicID)
{
	if( !e_pFile )
		return false;
	int nFileLength = UT::GetFileSize(e_pFile);
	if( nFileLength < (int)sizeof(int) )
		return false;
	//store curent position
	long l_lCurrentPosition = NvFTell( e_pFile );
	if( l_lCurrentPosition != 0 )
	{//go to start
		NvFSeek(e_pFile,0,SEEK_SET);
	}
	int l_iFileBuffer = -1;
	NvFRead(&l_iFileBuffer,sizeof(char),sizeof(int),e_pFile);
	//restore position
	if( l_lCurrentPosition != 0 )
		NvFSeek(e_pFile,l_lCurrentPosition,SEEK_SET);
	if(l_iFileBuffer == e_iMagicID )
	{
		return true;
	}
	return false;
}

std::wstring	DecodeStringFromeValue(const wchar_t*e_strEncrypt, const wchar_t*e_strCharacterToSplit)
{
	wchar_t*	l_strWchar = new wchar_t[wcslen(e_strEncrypt)];
	unsigned char	l_strtemp[79979];
	wchar_t*l_strStart = wcstok(l_strWchar,e_strCharacterToSplit);
	int	l_iCount = 0;
	while( l_strStart )
	{
		l_strtemp[l_iCount] = (unsigned char)FATMING_CORE::GetInt(l_strStart);
		++l_iCount;
		l_strStart = wcstok(0,e_strCharacterToSplit);
	}
	l_strtemp[l_iCount] = 0;
	unsigned char *pDes;
	int nDesLen;
	std::wstring	l_strResult;
	if(DecompressHuffman(l_strtemp, l_iCount, pDes, nDesLen))
	{
		memcpy(l_strtemp,pDes,nDesLen);
		l_strtemp[nDesLen] = 0;
		l_strResult = UT::CharToWchar((char*)l_strtemp);
		delete pDes;
	}
	delete[] l_strWchar;
	return l_strResult;
}

std::wstring	EncodeStringToValue(const wchar_t*e_strString, const wchar_t*e_strCharacterToSplit)
{
	std::wstring	l_strResult;

	std::string	l_strTemp = UT::WcharToChar(e_strString);
	unsigned char*l_strToCompress = (unsigned char*)l_strTemp.c_str();
	int	l_iCount = (int)strlen((char*)l_strToCompress);
	unsigned char *pDes;
	int nDesLen;
	if( CompressHuffman(l_strToCompress,l_iCount, pDes, nDesLen) )
	{
		for( int i=0;i<nDesLen;++i )
		{
			l_strResult += FATMING_CORE::ValueToStringW((int)pDes[i]);
			if( i != nDesLen-1 )
			{
				l_strResult += e_strCharacterToSplit;
			}
		}
		delete pDes;
	}

	return l_strResult;
}