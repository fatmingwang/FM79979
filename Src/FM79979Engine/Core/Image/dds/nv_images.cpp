//----------------------------------------------------------------------------------
// File:        jni/nv_glesutil/nv_images.cpp
// SDK Version: v10.14 
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

#include "../../stdafx.h"
#include "nv_global.h"

//#include <GLES2/gl2.h>
//#include <GLES2/gl2ext.h>

#include "nv_images.h"
#include "../../GameplayUT/GameApp.h"
#include "nv_hhdds_internal.h"
//#include <nv_log/nv_log.h>

//#include <nv_file/nv_file.h>

static int LoadTextureFromDDSData(GLenum target, NvS32 startLevel, const NVHHDDSImage *pImage, NvBool useMipmaps )
{
    NvS32 w, h, level, m;
    NvS32 powerOfTwo, mipLevelsToLoad, maxLevel;
    GLenum baseTarget = (target == GL_TEXTURE_2D) ? GL_TEXTURE_2D : GL_TEXTURE_CUBE_MAP;

    if ( pImage == NULL ) {
        return 0;
    }

    w = pImage->width;
    h = pImage->height;
    level = startLevel;

    /* Need to detect if the image has a non-pow-2 dimension */
    powerOfTwo = ((w & (w - 1)) == 0) && ((h & (h - 1)) == 0);

    /* Only pow-2 images that are tagged for using mipmaps get them */
    mipLevelsToLoad = ((powerOfTwo && useMipmaps) ? pImage->numMipmaps : 1);

    maxLevel = level + mipLevelsToLoad;

    for ( m = 0; level < maxLevel; level++, m++ ) {
        if (pImage->compressed) {
            glCompressedTexImage2D( target, m, pImage->format, w, h,
                                    0, pImage->size[level],
                                    pImage->data[level] );
        } else {
            glTexImage2D( target, m, pImage->format, w, h, 0,
                          pImage->format, pImage->componentFormat,
                          pImage->data[level]);
        }

        w = (w == 1) ? w : w >> 1;
        h = (h == 1) ? h : h >> 1;
    }

    if (mipLevelsToLoad > 1)
    {
        glTexParameterf(baseTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
    }
    else
    {
        glTexParameterf(baseTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }

    glTexParameterf(baseTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    /* For now, we clamp everyone - apps wanting wrapping for a texture can set it manually */
    glTexParameterf(baseTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(baseTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return 1;
}

bool	GetInfoFromDDS(const char* filename, NvS32* width, NvS32* height)
{
	NvFile*l_pNvFile = MyFileOpen(filename, "rb");
	if (l_pNvFile)
	{
		DDS_HEADER ddsh;
		char filecode[4];
		// read in file marker, make sure its a DDS file
		NvFRead(filecode, 1, 4, l_pNvFile);
		if (memcmp(filecode, "DDS ", 4) != 0)
		{
			NvFClose(l_pNvFile);
			return false;
		}
		// read in DDS header
		NvFRead(&ddsh, sizeof(ddsh), 1, l_pNvFile);
		NvFClose(l_pNvFile);
		if(width)
			*width = ddsh.dwWidth;
		if(height)
			*height = ddsh.dwHeight;
		return true;
	}
	return false;
}
//extern NvS32 total_image_data_size(NVHHDDSImage* image);
void*	GetPixelsFromDDS(const char* filename, NvS32* width, NvS32* height, NvBool* alpha, int*e_piImageSize)
{
	void*l_pResultData = nullptr;
	NVHHDDSImage *img = NVHHDDSLoad(filename, 0);
	if (!img)
		return nullptr;
	if (e_piImageSize)
		*e_piImageSize = total_image_data_size(img)*4;//rgba
	if (width)
		*width = img->width;
	if (height)
		*height = img->height;
	if (alpha)
		*alpha = img->alpha ? NV_TRUE : NV_FALSE;
	l_pResultData = img->dataBlock;
	img->dataBlock = nullptr;
	NVHHDDSFree(img);
	return l_pResultData;
}

GLuint NvCreateTextureFromDDSEx(const char* filename, NvBool flipVertical, NvBool useMipmaps, NvS32* width, NvS32* height, NvBool* alpha, NvBool *isCubeMap)
{
    // Clear the GL error before this function, since at the end of this 
    // function, we test the error and report it.  We do not want to report
    // an error that happened at some random time before.  If we want to
    // catch those, we need more general/comprehensive handling.  But having
    // this function print a GL error for something that happened in other 
    // random code is confusing, especially to non-rail developers.
    // Some other code, like NVBitfont, prints error messages at the top of
    // the function and print a message that implies that the error was there
    // at the time of call.  That may make sense as an optional setting down
    // the road
    glGetError();

    GLuint tex = 0;
    NVHHDDSImage *img = NVHHDDSLoad(filename, flipVertical ? 1 : 0);
    if (img)
    {
        if(isCubeMap)
        {
            *isCubeMap = img->cubemap ? NV_TRUE:NV_FALSE;
        }
        if (width)
            *width = img->width;
        if (height)
            *height = img->height;
        if (alpha)
            *alpha = img->alpha ? NV_TRUE : NV_FALSE;

        glGenTextures(1, &tex);
        if (!img->cubemap)
        {
            glBindTexture(GL_TEXTURE_2D, tex);
            LoadTextureFromDDSData(GL_TEXTURE_2D, 0, img, useMipmaps);
        }
        else
        {
            NvS32 baseLevel = 0;
            glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;

            LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, baseLevel, img, useMipmaps);
            baseLevel += img->numMipmaps ? img->numMipmaps : 1;
        }

        GLint err = glGetError();
        if (err)
		{
            //NVLogError("NvCreateTextureFromDDSEx error", "");
			cGameApp::OutputDebugInfoString(L"NvCreateTextureFromDDSEx error");
		}

        NVHHDDSFree(img);
    }

    return tex;
}

GLuint NvCreateTextureFromHHDDSImage(const NVHHDDSImage& image,
                                NvBool useMipmaps, NvS32* width, NvS32* height, NvBool* alpha, NvBool *isCubeMap)
{
    // Clear the GL error before this function, since at the end of this 
    // function, we test the error and report it.  We do not want to report
    // an error that happened at some random time before.  If we want to
    // catch those, we need more general/comprehensive handling.  But having
    // this function print a GL error for something that happened in other 
    // random code is confusing, especially to non-rail developers.
    // Some other code, like NVBitfont, prints error messages at the top of
    // the function and print a message that implies that the error was there
    // at the time of call.  That may make sense as an optional setting down
    // the road
    glGetError();

    GLuint tex = 0;
    if(isCubeMap)
    {
        *isCubeMap = image.cubemap ? NV_TRUE:NV_FALSE;
    }
    if (width)
        *width = image.width;
    if (height)
        *height = image.height;
    if (alpha)
        *alpha = image.alpha ? NV_TRUE : NV_FALSE;

    glGenTextures(1, &tex);
    if (!image.cubemap)
    {
        glBindTexture(GL_TEXTURE_2D, tex);
        LoadTextureFromDDSData(GL_TEXTURE_2D, 0, &image, useMipmaps);
    }
    else
    {
        NvS32 baseLevel = 0;
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex);

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_X, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;

        LoadTextureFromDDSData(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, baseLevel, &image, useMipmaps);
        baseLevel += image.numMipmaps ? image.numMipmaps : 1;
    }

    GLint err = glGetError();
    if (err)
	{
        //NVLogError("NvCreateTextureFromHHDDSImage error", "");
		cGameApp::OutputDebugInfoString(L"NvCreateTextureFromHHDDSImage error");
	}

    return tex;
}

