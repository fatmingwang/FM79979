#ifndef _DDSBASE_H_
#define _DDSBASE_H_


//take a look at http://software.intel.com/en-us/articles/android-texture-compression
 
//struct DDS_PIXELFORMAT
//{
//    DWORD dwSize;
//    DWORD dwFlags;
//    DWORD dwFourCC;
//    DWORD dwRGBBitCount;
//    DWORD dwRBitMask;
//    DWORD dwGBitMask;
//    DWORD dwBBitMask;
//    DWORD dwABitMask;
//};
 
#define DDS_FOURCC 0x00000004  // DDPF_FOURCC
#define DDS_RGB    0x00000040  // DDPF_RGB
#define DDS_RGBA   0x00000041  // DDPF_RGB | DDPF_ALPHAPIXELS
 
//const DDS_PIXELFORMAT DDSPF_DXT1 =
//    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','1'), 0, 0, 0, 0, 0 };
// 
//const DDS_PIXELFORMAT DDSPF_DXT2 =
//    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','2'), 0, 0, 0, 0, 0 };
// 
//const DDS_PIXELFORMAT DDSPF_DXT3 =
//    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','3'), 0, 0, 0, 0, 0 };
// 
//const DDS_PIXELFORMAT DDSPF_DXT4 =
//    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','4'), 0, 0, 0, 0, 0 };
// 
//const DDS_PIXELFORMAT DDSPF_DXT5 =
//    { sizeof(DDS_PIXELFORMAT), DDS_FOURCC, MAKEFOURCC('D','X','T','5'), 0, 0, 0, 0, 0 };
// 
//const DDS_PIXELFORMAT DDSPF_A8R8G8B8 =
//    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000 };
// 
//const DDS_PIXELFORMAT DDSPF_A1R5G5B5 =
//    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000 };
// 
//const DDS_PIXELFORMAT DDSPF_A4R4G4B4 =
//    { sizeof(DDS_PIXELFORMAT), DDS_RGBA, 0, 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000 };
// 
//const DDS_PIXELFORMAT DDSPF_R8G8B8 =
//    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000 };
// 
//const DDS_PIXELFORMAT DDSPF_R5G6B5 =
//    { sizeof(DDS_PIXELFORMAT), DDS_RGB, 0, 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000 };
 
#define DDS_HEADER_FLAGS_TEXTURE    0x00001007  // DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT 
#define DDS_HEADER_FLAGS_MIPMAP     0x00020000  // DDSD_MIPMAPCOUNT
#define DDS_HEADER_FLAGS_VOLUME     0x00800000  // DDSD_DEPTH
#define DDS_HEADER_FLAGS_PITCH      0x00000008  // DDSD_PITCH
#define DDS_HEADER_FLAGS_LINEARSIZE 0x00080000  // DDSD_LINEARSIZE
 
#define DDS_SURFACE_FLAGS_TEXTURE 0x00001000 // DDSCAPS_TEXTURE
#define DDS_SURFACE_FLAGS_MIPMAP  0x00400008 // DDSCAPS_COMPLEX | DDSCAPS_MIPMAP
#define DDS_SURFACE_FLAGS_CUBEMAP 0x00000008 // DDSCAPS_COMPLEX
 
#define DDS_CUBEMAP_POSITIVEX 0x00000600 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEX
#define DDS_CUBEMAP_NEGATIVEX 0x00000a00 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEX
#define DDS_CUBEMAP_POSITIVEY 0x00001200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEY
#define DDS_CUBEMAP_NEGATIVEY 0x00002200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEY
#define DDS_CUBEMAP_POSITIVEZ 0x00004200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_POSITIVEZ
#define DDS_CUBEMAP_NEGATIVEZ 0x00008200 // DDSCAPS2_CUBEMAP | DDSCAPS2_CUBEMAP_NEGATIVEZ
 
#define DDS_CUBEMAP_ALLFACES ( DDS_CUBEMAP_POSITIVEX | DDS_CUBEMAP_NEGATIVEX |\
                               DDS_CUBEMAP_POSITIVEY | DDS_CUBEMAP_NEGATIVEY |\
                               DDS_CUBEMAP_POSITIVEZ | DDS_CUBEMAP_NEGATIVEZ )
 
#define DDS_FLAGS_VOLUME 0x00200000 // DDSCAPS2_VOLUME
 
 
//struct DDS_HEADER
//{
//    DWORD dwSize;
//    DWORD dwHeaderFlags;
//    DWORD dwHeight;
//    DWORD dwWidth;
//    DWORD dwPitchOrLinearSize;
//    DWORD dwDepth; // only if DDS_HEADER_FLAGS_VOLUME is set in dwHeaderFlags
//    DWORD dwMipMapCount;
//    DWORD dwReserved1[11];
//    DDS_PIXELFORMAT ddspf;
//    DWORD dwSurfaceFlags;
//    DWORD dwCubemapFlags;
//    DWORD dwReserved2[3];
//};
 
 
#endif
 
 
//-------------------------------------------------------------------------
//¡± File¡G DDSConvert.h
//¡± Desc¡G CDDSConvert dds®æ¦¡‹H„ã
//¡± 
//¡± Date¡G 2011/6/6
//¡± Copyright(c) xychzh
//-------------------------------------------------------------------------
#ifndef _DDSCONVERT_H_
#define _DDSCONVERT_H_
 
//#include "DDSBase.h"
 
 
 
class CDDSConvert
{
public:
    CDDSConvert(IDirect3DDevice9* pD3DDevice);
    ~CDDSConvert();
 
public:
    void        ConvertDDS(const WCHAR* szSrcFile, const WCHAR* szDstFile, D3DFORMAT dstFormat);
    void        ConvertDDS(IDirect3DTexture9* pSrcTexture, IDirect3DTexture9** ppDstTexture, D3DFORMAT dstFormat);
    BOOL        OpenFile(LPCTSTR lpszPathName);
 
    HRESULT        Compress(IDirect3DTexture9* pSrc, IDirect3DTexture9** ppDst, D3DFORMAT fmtTo);
    HRESULT        ChangeFormat(LPDIRECT3DBASETEXTURE9 ptexCur, D3DFORMAT fmtTo, LPDIRECT3DBASETEXTURE9* pptexNew);
    HRESULT        BltAllLevels(D3DCUBEMAP_FACES FaceType, LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest);
 
    bool        IsVolumeMap()    {    return m_dwDepth > 0;    }
//    BOOL        IsCubeMap(VOID) { return (m_dwCubeMapFlags > 0); }
 
private:
    IDirect3DDevice9*            m_pD3DDevice;            // D3D„¦„Â†Á¶H
 
private:
    LPDIRECT3DBASETEXTURE9        m_ptexOrig;
    LPDIRECT3DBASETEXTURE9        m_ptexNew;
    DWORD                        m_dwWidth;
    DWORD                        m_dwHeight;
    DWORD                        m_dwDepth;
    int                            m_numMips;
 
    D3DFORMAT                    m_CurFormat;
};
 
 
#endif