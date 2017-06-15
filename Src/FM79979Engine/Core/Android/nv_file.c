//----------------------------------------------------------------------------------
// File:        jni/nv_file/nv_file.c
// SDK Version: v10.10 
// Email:       tegradev@nvidia.com
// Site:        http://developer.nvidia.com/
//
// Copyright (c) 2007-2012, NVIDIA CORPORATION.  All rights reserved.
//
// TO  THE MAXIMUM  EXTENT PERMITTED  BY APPLICABLE  LAW, THIS SOFTWARE  IS PROVIDED
// *AS IS*  AND NVIDIA AND  ITS SUPPLIERS DISCLAIM  ALL WARRANTIES,  EITHER  EXPRESS
// OR IMPLIED, INCLUDING, BUT NOT LIMITED  TO, IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL  NVIDIA OR ITS SUPPLIERS
// BE  LIABLE  FOR  ANY  SPECIAL,  INCIDENTAL,  INDIRECT,  OR  CONSEQUENTIAL DAMAGES
// WHATSOEVER (INCLUDING, WITHOUT LIMITATION,  DAMAGES FOR LOSS OF BUSINESS PROFITS,
// BUSINESS INTERRUPTION, LOSS OF BUSINESS INFORMATION, OR ANY OTHER PECUNIARY LOSS)
// ARISING OUT OF THE  USE OF OR INABILITY  TO USE THIS SOFTWARE, EVEN IF NVIDIA HAS
// BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
//
//
//----------------------------------------------------------------------------------

#include "nv_file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "nv_apk_file.h"

#include <unistd.h>   
#include <sys/stat.h>   
#include <sys/time.h>   
#include <stdlib.h>   
#include <fcntl.h>

typedef struct
{
    int     type;
    void   *handle;
	int		iFileDescriptor;
} NvFileHandle;

enum
{
    APK_FILE,
    STD_FILE
};

void  NvFInit(AAssetManager* assetManager)
{
    NvAPKInit(assetManager);
}
//
////http://www.phonesdevelopers.com/1773184/
//int FILE_OPEN(const   char * filename)
//{
//	int fd;    
//	fd = open (filename, O_CREAT | O_RDWR, 0666);
//	if (fd == -1)
//		return -1;  
//	return fd;
//}
//
//NvFile* NvFOpen(char const* path,const char*e_strReadMode)
//{
//    NvFileHandle *handle = NULL;
//
//    // First, try the given path with no mods...
//    //FILE *fp = fopen(path, e_strReadMode);
//	int	l_iFileDescriptor = FILE_OPEN(path);
//    // No luck...  Try prepending /data...
//    if (l_iFileDescriptor == -1)
//    {
//        char buf[512];
//        sprintf(buf, "/data/%s", path); //TODO: use storage card path in the future?
//        //fp = fopen(buf, e_strReadMode);
//		l_iFileDescriptor = FILE_OPEN(buf);
//    }
//
//    if (l_iFileDescriptor != -1)
//    {
//        handle = (NvFileHandle*) malloc(sizeof(NvFileHandle));
//        handle->type = STD_FILE;
//		handle->iFileDescriptor = l_iFileDescriptor;
//		handle->handle = fdopen(handle->iFileDescriptor,e_strReadMode);
//    }
//    else
//    {
//        NvAPKFile* apk = NvAPKOpen(path);
//        if (apk)
//        {
//            handle = (NvFileHandle*) malloc(sizeof(NvFileHandle));
//            handle->type = APK_FILE;
//            handle->handle = NvAPKOpen(path);
//        }
//    }
//    return (NvFile*) handle;
//}
//void NvFClose(NvFile* file)
//{
//    NvFileHandle *handle = (NvFileHandle*) file;
//    if (!handle)
//        return;
//    if (handle->type != STD_FILE)
//	{
//        NvAPKClose(handle->handle);
//	}
//    else
//	{
//		close(handle->iFileDescriptor);
//	}
//    free(handle);
//}
//
//void NvFChdir(const char* dir)
//{
//    (void)chdir(dir);
//}
//
//int NvFGetc(NvFile *stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return -1;
//
//    if (handle->type != STD_FILE)
//        return NvAPKGetc(handle->handle);
//    else
//        return fgetc(handle->handle);
//}
//
//char* NvFGets(char* s, int size, NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return NULL;
//
//    if (handle->type != STD_FILE)
//        return NvAPKGets(s, size, handle->handle);
//    else
//        return fgets(s, size, handle->handle);
//}
//
//size_t NvFSize(NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return 0;
//
//    if (handle->type != STD_FILE)
//    {
//        return NvAPKSize(handle->handle);
//    }
//    else
//    {
//		int pos = lseek(handle->iFileDescriptor,0,SEEK_CUR);
//        int size = lseek(handle->iFileDescriptor, 0, SEEK_END);
//		lseek(handle->iFileDescriptor, pos, SEEK_SET);
//        return size;
//    }
//}
//
//long NvFSeek(NvFile* stream, long offset, int type)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return 0;
//
//    if (handle->type != STD_FILE)
//        return NvAPKSeek(handle->handle, offset, type);
//    else
//		return lseek(handle->iFileDescriptor, offset, type);
//}
//
//long NvFTell(NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return 0;
//
//    if (handle->type != STD_FILE)
//        return NvAPKTell(handle->handle);
//    else
//        return ftell(handle->handle);
//}
//
//size_t NvFRead(void* ptr, size_t size, size_t nmemb, NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return 0;
//
//    if (handle->type != STD_FILE)
//        return NvAPKRead(ptr, size, nmemb, handle->handle);
//    else
//	{
//		size_t	l_iRead = read(handle->iFileDescriptor,ptr, size*nmemb);
//		return l_iRead/size;
//	}
//}
//
//int NvFEOF(NvFile *stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return 0;
//
//    if (handle->type != STD_FILE)
//        return NvAPKEOF(handle->handle);
//    else
//        return feof(((FILE*)handle->handle));
//}
//
//size_t      NvFWrite(const void* ptr, size_t size, size_t nmemb, NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return -1;
//
//    if (handle->type != STD_FILE)
//        return -1;
//    else
//	{
//#ifdef DEBUG
//		size_t l_BeforeSize= NvFSize(stream);
//#endif
//		size_t	l_WriteSize = write(handle->iFileDescriptor,ptr,size*nmemb);
//#ifdef DEBUG
//		size_t l_AfterSize= NvFSize(stream);
//#endif
//		return l_WriteSize/size;
//		//return fwrite(ptr,size,nmemb,handle->handle);
//	}
//	return -1;
//}
//
//size_t      NvFFlush(NvFile* stream)
//{
//    NvFileHandle *handle = (NvFileHandle*) stream;
//
//    if (!handle)
//        return -1;
//
//    if (handle->type != STD_FILE)
//        return -1;
//    else
//	{
//		fsync(handle->iFileDescriptor);
//		//int	l_ifileDescriptor = fileno((FILE*)handle->handle);
//		//int l_iResult = fsync(l_ifileDescriptor);
//		//return l_iResult==0;
//        //return fflush(handle->handle);
//		//no need to do this
//		return -1;
//	}
//	return -1;
//}
//
//FILE*		NvGetFile(NvFile* stream)
//{
//#if	defined(ANDROID)
//	if( stream )
//	{
//		NvFileHandle *handle = (NvFileHandle*)stream;
//		if(handle->type == STD_FILE)
//		{
//			return (FILE*)handle->handle;
//		}
//	}
//	return 0;
//#else
//	return (FILE*)stream;
//#endif
//}

NvFile* NvFOpen(char const* path,const char*e_strReadMode)
{
    NvFileHandle *handle = NULL;

    // First, try the given path with no mods...
    FILE *fp = fopen(path, e_strReadMode);

    // No luck...  Try prepending /data...
    if (!fp)
    {
        char buf[512];
        sprintf(buf, "/data/%s", path); //TODO: use storage card path in the future?
        fp = fopen(buf, e_strReadMode);
    }

    if (fp)
    {
        handle = (NvFileHandle*) malloc(sizeof(NvFileHandle));
        handle->type = STD_FILE;
        handle->handle = fp;
    }
    else
    {
        NvAPKFile* apk = NvAPKOpen(path);
        if (apk)
        {
            handle = (NvFileHandle*) malloc(sizeof(NvFileHandle));
            handle->type = APK_FILE;
            handle->handle = NvAPKOpen(path);
        }
    }
    return (NvFile*) handle;
}
int NvFClose(NvFile* file)
{
    NvFileHandle *handle = (NvFileHandle*) file;
	int l_iResult = EOF;
    if (!handle)
        return l_iResult;
    if (handle->type != STD_FILE)
	{
        NvAPKClose(handle->handle);
	}
    else
	{
        l_iResult = fclose(handle->handle);
	}
    free(handle);
	return l_iResult;
}

void NvFChdir(const char* dir)
{
    (void)chdir(dir);
}

int NvFGetc(NvFile *stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return -1;

    if (handle->type != STD_FILE)
        return NvAPKGetc(handle->handle);
    else
        return fgetc(handle->handle);
}

char* NvFGets(char* s, int size, NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return NULL;

    if (handle->type != STD_FILE)
        return NvAPKGets(s, size, handle->handle);
    else
        return fgets(s, size, handle->handle);
}
size_t NvFSize(NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return 0;

    if (handle->type != STD_FILE)
    {
        return NvAPKSize(handle->handle);
    }
    else
    {
        int pos = ftell(handle->handle);
        int size = 0;
        fseek(handle->handle, 0, SEEK_END);
        size = ftell(handle->handle);
        fseek(handle->handle, pos, SEEK_SET);
        return size;
    }
}

long NvFSeek(NvFile* stream, long offset, int type)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return 0;

    if (handle->type != STD_FILE)
        return NvAPKSeek(handle->handle, offset, type);
    else
        return fseek(handle->handle, offset, type);
}

long NvFTell(NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return 0;

    if (handle->type != STD_FILE)
        return NvAPKTell(handle->handle);
    else
        return ftell(handle->handle);
}

size_t NvFRead(void* ptr, size_t size, size_t nmemb, NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return 0;

    if (handle->type != STD_FILE)
        return NvAPKRead(ptr, size, nmemb, handle->handle);
    else
        return fread(ptr, size, nmemb, handle->handle);
}

int NvFEOF(NvFile *stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return 0;

    if (handle->type != STD_FILE)
        return NvAPKEOF(handle->handle);
    else
        return feof(((FILE*)handle->handle));
}

size_t      NvFWrite(const void* ptr, size_t size, size_t nmemb, NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return -1;

    if (handle->type != STD_FILE)
        return -1;
    else
		return fwrite(ptr,size,nmemb,handle->handle);
	return -1;
}

size_t      NvFFlush(NvFile* stream)
{
    NvFileHandle *handle = (NvFileHandle*) stream;

    if (!handle)
        return -1;

    if (handle->type != STD_FILE)
        return -1;
    else
	{
		//int	l_ifileDescriptor = fileno((FILE*)handle->handle);
		//int l_iResult = fsync(l_ifileDescriptor);
		//return l_iResult==0;
        return fflush(handle->handle);
	}
	return -1;
}

int			NvFerror(NvFile*e_pFile)
{
	return 0;
	//sorry I have no idea with this,fuck
	//return ferror(NvGetFile(e_pFile));
}

FILE*		NvGetFile(NvFile* stream)
{
#if	defined(ANDROID)
	if( stream )
	{
		NvFileHandle *handle = (NvFileHandle*)stream;
		if(handle->type == STD_FILE)
		{
			return (FILE*)handle->handle;
		}
	}
	return 0;
#else
	return (FILE*)stream;
#endif
}