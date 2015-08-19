#include "stdAfx.h"
#include "DDSConvert.h"
 
 
#ifndef ReleasePpo
#define ReleasePpo(ppo) \
    if (*(ppo) != NULL) \
        { \
        (*(ppo))->Release(); \
        *(ppo) = NULL; \
        } \
        else (VOID)0
#endif
 
 
CDDSConvert::CDDSConvert(IDirect3DDevice9* pD3DDevice)
{
    m_pD3DDevice = pD3DDevice;
    m_ptexOrig = NULL;
    m_ptexNew = NULL;
}
 
CDDSConvert::~CDDSConvert()
{
}
 
void CDDSConvert::ConvertDDS(const WCHAR* szSrcFile, const WCHAR* szDstFile, D3DFORMAT dstFormat)
{
    OpenFile(szSrcFile);
 
    IDirect3DTexture9* pSrcTexture = (IDirect3DTexture9*)m_ptexOrig;
    IDirect3DTexture9* pDstTexture = NULL;
 
    Compress(pSrcTexture, &pDstTexture, dstFormat);
 
    if( FAILED( D3DXSaveTextureToFile( szDstFile, D3DXIFF_DDS, pDstTexture, NULL ) ) )
    {
        return;
    }
 
    ReleasePpo(&m_ptexOrig);
     
    if (pDstTexture != NULL)
    {
        pDstTexture->Release();
        pDstTexture = NULL;
    }
}
 
void CDDSConvert::ConvertDDS(IDirect3DTexture9* pSrcTexture, IDirect3DTexture9** ppDstTexture, D3DFORMAT dstFormat)
{
    D3DSURFACE_DESC surfaceDesc;
    pSrcTexture->GetLevelDesc(0, &surfaceDesc);
 
    m_CurFormat = surfaceDesc.Format;
    m_dwWidth = surfaceDesc.Width;
    m_dwHeight = surfaceDesc.Height;
    m_dwDepth = 0;
    m_numMips = 1;
 
    Compress(pSrcTexture, ppDstTexture, dstFormat);
}
 
BOOL CDDSConvert::OpenFile(LPCTSTR lpszPathName)
{
    LPDIRECT3DDEVICE9 pd3ddev = m_pD3DDevice;
    D3DXIMAGE_INFO imageinfo;
    D3DXIMAGE_INFO imageinfo2;
 
    if( FAILED( D3DXGetImageInfoFromFile( lpszPathName, &imageinfo ) ) )
    {
        return FALSE;
    }
 
    switch( imageinfo.ResourceType )
    {
    case D3DRTYPE_TEXTURE:
        if( FAILED( D3DXCreateTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
            imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 
            &imageinfo2, NULL, (LPDIRECT3DTEXTURE9*)&m_ptexOrig ) ) )
        {
            return FALSE;
        }
 
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = 0;
        m_numMips = imageinfo2.MipLevels;
 
        D3DSURFACE_DESC surfaceDesc;
        ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetLevelDesc(0, &surfaceDesc);
        m_CurFormat = surfaceDesc.Format;
 
        if( imageinfo.ImageFileFormat == D3DXIFF_BMP )
        {
            // Look for "foo_a.bmp" for alpha channel
//             CString strPath = lpszPathName;
//             int i = strPath.ReverseFind('.');
//             HRESULT hr;
//             strPath = strPath.Left(i) + "_a.bmp";
//             CFileStatus status;
//             if (CFile::GetStatus(strPath, status))
//             {
//                 // Make sure there's an alpha channel to load alpha image into
//                 if (FAILED(EnsureAlpha(&m_ptexOrig)))
//                     return FALSE;
// 
//                 LPDIRECT3DSURFACE9 psurf;
// 
//                 hr = ((LPDIRECT3DTEXTURE9)m_ptexOrig)->GetSurfaceLevel(0, &psurf);
//                 if (FAILED(hr))
//                     return FALSE;
// 
//                 hr = LoadAlphaIntoSurface(strPath, psurf);
//                 ReleasePpo(&psurf);
//                 if (FAILED(hr))
//                     return FALSE;
//            }
        }
        break;
 
    case D3DRTYPE_VOLUMETEXTURE:
        if( FAILED( D3DXCreateVolumeTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.Height, imageinfo.Depth, imageinfo.MipLevels,
            0, imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
            0, &imageinfo2, NULL, (LPDIRECT3DVOLUMETEXTURE9*)&m_ptexOrig ) ) )
        {
            return FALSE;
        }
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = imageinfo2.Depth;
        m_numMips = imageinfo2.MipLevels;
        break;
 
    case D3DRTYPE_CUBETEXTURE:
        if( FAILED( D3DXCreateCubeTextureFromFileEx( pd3ddev, lpszPathName, 
            imageinfo.Width, imageinfo.MipLevels, 0, imageinfo.Format, 
            D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
            0, &imageinfo2, NULL, (LPDIRECT3DCUBETEXTURE9*)&m_ptexOrig ) ) )
        {
            return FALSE;
        }
        m_dwWidth = imageinfo2.Width;
        m_dwHeight = imageinfo2.Height;
        m_dwDepth = 0;
        m_numMips = imageinfo2.MipLevels;
    //    m_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
        break;
 
    default:
        return FALSE;
    }
 
    return TRUE;
}
 
HRESULT CDDSConvert::Compress(IDirect3DTexture9* pSrc, IDirect3DTexture9** ppDst, D3DFORMAT fmtTo)
{
    HRESULT hr;
    LPDIRECT3DBASETEXTURE9 ptexNew = NULL;
 
    if (FAILED(hr = ChangeFormat(pSrc, fmtTo, (LPDIRECT3DBASETEXTURE9*)ppDst)))
        return hr;
 
    return S_OK;
}
 
HRESULT CDDSConvert::ChangeFormat(LPDIRECT3DBASETEXTURE9 ptexCur, D3DFORMAT fmtTo, 
                                LPDIRECT3DBASETEXTURE9* pptexNew)
{
    HRESULT hr;
    LPDIRECT3DDEVICE9 pd3ddev = m_pD3DDevice;
    LPDIRECT3DTEXTURE9 pmiptex;
    LPDIRECT3DVOLUMETEXTURE9 pvoltex;
    D3DFORMAT fmtFrom;
    LPDIRECT3DTEXTURE9 pmiptexNew;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexNew;
 
    if (IsVolumeMap())
    {
        pvoltex = (LPDIRECT3DVOLUMETEXTURE9)ptexCur;
        D3DVOLUME_DESC vd;
        pvoltex->GetLevelDesc(0, &vd);
        fmtFrom = vd.Format;
    }
//     else if (m_dwCubeMapFlags > 0)
//     {
//         pcubetex = (LPDIRECT3DCUBETEXTURE9)ptexCur;
//         D3DSURFACE_DESC sd;
//         pcubetex->GetLevelDesc(0, &sd);
//         fmtFrom = sd.Format;
//     }
    else
    {
        pmiptex = (LPDIRECT3DTEXTURE9)ptexCur;
        D3DSURFACE_DESC sd;
        pmiptex->GetLevelDesc(0, &sd);
        fmtFrom = sd.Format;
    }
 
    if (fmtFrom == D3DFMT_DXT2 || fmtFrom == D3DFMT_DXT4)
    {
        if (fmtTo == D3DFMT_DXT1)
        {
        //    AfxMessageBox(ID_ERROR_PREMULTTODXT1);
        }
        else if (fmtTo != D3DFMT_DXT2 && fmtTo != D3DFMT_DXT4)
        {
        //    AfxMessageBox(ID_ERROR_PREMULTALPHA);
            return S_OK;
        }
    }
 
    if (IsVolumeMap())
    {
        hr = pd3ddev->CreateVolumeTexture(m_dwWidth, m_dwHeight, m_dwDepth, m_numMips,
            0, fmtTo, D3DPOOL_SYSTEMMEM, &pvoltexNew, NULL);
        if (FAILED(hr))
            return hr;
        *pptexNew = pvoltexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, ptexCur, *pptexNew)))
            return hr;
    }
//     else if (m_dwCubeMapFlags > 0)
//     {
//         hr = pd3ddev->CreateCubeTexture(m_dwWidth, m_numMips, 
//             0, fmtTo, D3DPOOL_MANAGED, &pcubetexNew, NULL);
//         if (FAILED(hr))
//             return hr;
//         *pptexNew = pcubetexNew;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_X, ptexCur, *pptexNew)))
//             return hr;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_X, ptexCur, *pptexNew)))
//             return hr;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Y, ptexCur, *pptexNew)))
//             return hr;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Y, ptexCur, *pptexNew)))
//             return hr;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_NEGATIVE_Z, ptexCur, *pptexNew)))
//             return hr;
//         if (FAILED(hr = BltAllLevels(D3DCUBEMAP_FACE_POSITIVE_Z, ptexCur, *pptexNew)))
//             return hr;
//     }
    else
    {
        if ((fmtTo == D3DFMT_DXT1 || fmtTo == D3DFMT_DXT2 ||
            fmtTo == D3DFMT_DXT3 || fmtTo == D3DFMT_DXT4 ||
            fmtTo == D3DFMT_DXT5) && (m_dwWidth % 4 != 0 || m_dwHeight % 4 != 0))
        {
        //    AfxMessageBox(ID_ERROR_NEEDMULTOF4);
            return E_FAIL;
        }
 
        hr = pd3ddev->CreateTexture(m_dwWidth, m_dwHeight, m_numMips, 
            0, fmtTo, D3DPOOL_MANAGED, &pmiptexNew, NULL);
        if (FAILED(hr))
            return hr;
        *pptexNew = pmiptexNew;
        if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, ptexCur, *pptexNew)))
            return hr;
    }
    return S_OK;
}
 
HRESULT CDDSConvert::BltAllLevels(D3DCUBEMAP_FACES FaceType, LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest)
{
    HRESULT hr;
    LPDIRECT3DTEXTURE9 pmiptexSrc;
    LPDIRECT3DTEXTURE9 pmiptexDest;
//   LPDIRECT3DCUBETEXTURE9 pcubetexSrc;
//   LPDIRECT3DCUBETEXTURE9 pcubetexDest;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexSrc;
    LPDIRECT3DVOLUMETEXTURE9 pvoltexDest;
    int iLevel;
 
    if (IsVolumeMap())
    {
        pvoltexSrc = (LPDIRECT3DVOLUMETEXTURE9)ptexSrc;
        pvoltexDest = (LPDIRECT3DVOLUMETEXTURE9)ptexDest;
    }
//     else if (IsCubeMap())
//     {
//         pcubetexSrc = (LPDIRECT3DCUBETEXTURE9)ptexSrc;
//         pcubetexDest = (LPDIRECT3DCUBETEXTURE9)ptexDest;
//     }
    else
    {
        pmiptexSrc = (LPDIRECT3DTEXTURE9)ptexSrc;
        pmiptexDest = (LPDIRECT3DTEXTURE9)ptexDest;
    }
 
    for (iLevel = 0; iLevel < m_numMips; iLevel++)
    {
        if (IsVolumeMap())
        {
            LPDIRECT3DVOLUME9 pvolSrc = NULL;
            LPDIRECT3DVOLUME9 pvolDest = NULL;
            hr = pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
            hr = pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
            hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
                pvolSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&pvolSrc);
            ReleasePpo(&pvolDest);
        }
//         else if (IsCubeMap())
//         {
//             LPDIRECT3DSURFACE9 psurfSrc = NULL;
//             LPDIRECT3DSURFACE9 psurfDest = NULL;
//             hr = pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
//             hr = pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
//             hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
//                 psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
//             ReleasePpo(&psurfSrc);
//             ReleasePpo(&psurfDest);
//         }
        else
        {
            LPDIRECT3DSURFACE9 psurfSrc = NULL;
            LPDIRECT3DSURFACE9 psurfDest = NULL;
            hr = pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
            hr = pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);
            hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
                psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
            ReleasePpo(&psurfSrc);
            ReleasePpo(&psurfDest);
        }
    }
 
    return S_OK;
}