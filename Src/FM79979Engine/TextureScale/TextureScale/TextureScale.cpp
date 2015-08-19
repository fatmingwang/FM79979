// TextureScale.cpp: 主要專案檔。

#include "stdafx.h"
#include "Form1.h"

namespace	TextureScale
{
	inline	int			 power_of_two(int input){ int value = 1; while (value < input)	value <<= 1; return value; }
	Image^ ScaleImage(Image^e_pImage,int e_iMaxWidth,int e_iMaxHeight)
	{
		double ratioX = (double)e_iMaxWidth / e_pImage->Width;
		double ratioY = (double)e_iMaxHeight / e_pImage->Height;
		//double ratio = Math::Min(ratioX, ratioY);

		double newWidth = (int)(e_pImage->Width * ratioX);
		double newHeight = (int)(e_pImage->Height * ratioY);

		Bitmap^ l_pNewImage = gcnew Bitmap((int)newWidth,(int)newHeight);
		Graphics::FromImage(l_pNewImage)->DrawImage(e_pImage, 0, 0, (int)newWidth, (int)newHeight);
		return l_pNewImage;
	}	

	void	Form1::ScaleImageWithDirectX(String^e_strDirectoryName,String^e_strFileName,D3DXIMAGE_FILEFORMAT e_D3DXIMAGE_FILEFORMAT,String^e_strExtensionName)
	{
		WCHAR*l_strName = DNCT::GcStringToWchar(e_strFileName);
		D3DXIMAGE_INFO imageinfo;
		D3DXIMAGE_INFO imageinfo2;
		bool l_bIsVolumeMap = false;
		bool l_bIsCueMap = false;
		DWORD	l_dwDepth = 0;
		DWORD	l_dwCubeMapFlags = 0;
		if( FAILED( D3DXGetImageInfoFromFile( l_strName, &imageinfo ) ) )
		{
			WARNING_MSG(DNCT::WcharToGcstring(l_strName)+"open failed");
			return;
		}
		IDirect3DTexture9* l_pTexture;
		switch( imageinfo.ResourceType )
		{
			case D3DRTYPE_TEXTURE:
				if( D3DXCreateTextureFromFileEx( g_pd3dDevice, l_strName, 
					imageinfo.Width, imageinfo.Height, imageinfo.MipLevels, 0,
					imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 0, 
					&imageinfo2, NULL, (LPDIRECT3DTEXTURE9*)&l_pTexture )  != D3D_OK )
				{
					WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
					return;
				}
				break;

			case D3DRTYPE_VOLUMETEXTURE:
				if( FAILED( D3DXCreateVolumeTextureFromFileEx( g_pd3dDevice, l_strName, 
					imageinfo.Width, imageinfo.Height, imageinfo.Depth, imageinfo.MipLevels,
					0, imageinfo.Format, D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE,
					0, &imageinfo2, NULL, (LPDIRECT3DVOLUMETEXTURE9*)&l_pTexture ) ) )
				{
					WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
					return;
				}
				l_bIsVolumeMap = true;
				l_dwDepth = imageinfo2.Depth;
				break;

			case D3DRTYPE_CUBETEXTURE:
				if( FAILED( D3DXCreateCubeTextureFromFileEx( g_pd3dDevice, l_strName, 
					imageinfo.Width, imageinfo.MipLevels, 0, imageinfo.Format, 
					D3DPOOL_MANAGED, D3DX_FILTER_NONE, D3DX_FILTER_NONE, 
					0, &imageinfo2, NULL, (LPDIRECT3DCUBETEXTURE9*)&l_pTexture ) ) )
				{
					WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" create failed");
					return;
				}
				l_bIsCueMap = true;
				l_dwCubeMapFlags = DDS_CUBEMAP_ALLFACES;
				break;
			default:
					WARNING_MSG(DNCT::WcharToGcstring(l_strName)+" type error");
				return;
		}
		UINT	l_uiWidth = (UINT)Width_numericUpDown->Value;
		UINT	l_uiHeight = (UINT)Height_numericUpDown->Value;
		if( TransformType_comboBox->SelectedIndex == 0)//percentage
		{
			float	l_fOrigianlWidth = (float)imageinfo.Width;
			float	l_fOrigianlHeight = (float)imageinfo.Height;
			l_uiWidth = (UINT)(l_fOrigianlWidth*l_uiWidth/100.f);
			l_uiHeight = (UINT)(l_fOrigianlHeight*l_uiHeight/100.f);
		}
		else
		if (TransformType_comboBox->SelectedIndex == 3)//power of two
		{
			l_uiWidth = power_of_two((int)(imageinfo.Width*((float)Width_numericUpDown->Value/100)));
			l_uiHeight = power_of_two((int)(imageinfo.Height*((float)Height_numericUpDown->Value/100)));
		}
		WCHAR	*l_strNewFileName = DNCT::GcStringToWchar(e_strDirectoryName+GetFileNameWithoutFullPath(e_strFileName));
		LPDIRECT3DBASETEXTURE9 l_pIDirect3DTexture9 = ImageResize(l_uiWidth,l_uiHeight,l_pTexture,imageinfo.MipLevels,l_bIsVolumeMap,l_bIsCueMap);
		if( l_pIDirect3DTexture9 )
		{
			if( CreateFolderIfNotReplaceOriginalFile_checkBox->Checked )
			{
				//original folder and file name length = original file name length - original directory length 
				int	l_iOriginalInternalFileNameLength = e_strFileName->Length;
				if( m_strDelectedDirectory )
					l_iOriginalInternalFileNameLength -= m_strDelectedDirectory->Length;
				//file name without full path directory length
				int	l_iFileNameLength = System::IO::Path::GetFileName(e_strFileName)->Length;
				//internal folder length = l_iOriginalInternalFileNameLength-l_iFileNameLength
				int	l_iInternalFolderLength = l_iOriginalInternalFileNameLength-l_iFileNameLength;
				//new folder name = new export folder path + internal folder name
				String^l_strNewDirectoryFolderPath;
				if( m_strDelectedDirectory )
				{
					l_strNewDirectoryFolderPath = e_strDirectoryName+
						e_strFileName->Substring(m_strDelectedDirectory->Length,l_iInternalFolderLength);
				}
				else
				{
					l_strNewDirectoryFolderPath = e_strDirectoryName;
				}
				String^l_strDirectoryName = l_strNewDirectoryFolderPath+System::IO::Path::GetFileName(e_strFileName);
				l_strNewFileName = DNCT::GcStringToWchar(l_strDirectoryName);
				CreateDirectory(l_strNewDirectoryFolderPath);
			}
			if( e_strExtensionName )
			{
				l_strNewFileName = DNCT::GcStringToWchar(DNCT::ChangeFileExtensionName(DNCT::WcharToGcstring(l_strNewFileName),e_strExtensionName));
			}					

			if( FAILED( D3DXSaveTextureToFile( l_strNewFileName, e_D3DXIMAGE_FILEFORMAT, l_pIDirect3DTexture9, NULL ) ) )
			{
				ErrorMsg_textBox->Text += "Error:"+e_strFileName+" write file failed "+DNCT::GetChanglineString();
			}
			else
			{
				if( e_D3DXIMAGE_FILEFORMAT == D3DXIFF_DDS )
				{
					//DDSPF_DXT1,0
					//DDSPF_DXT2,1
					//DDSPF_DXT3,2
					//DDSPF_DXT4,3
					//DDSPF_DXT5,4
					//DDS_RGBA,5
					D3DFORMAT	l_D3DFORMAT = D3DFMT_DXT3;
					switch(DDSFormat_comboBox->SelectedIndex)
					{
						case 0:
							l_D3DFORMAT = D3DFMT_DXT1;
							break;
						case 1:
							l_D3DFORMAT = D3DFMT_DXT2;
							break;
						case 2:
							l_D3DFORMAT = D3DFMT_DXT3;
							break;
						case 3:
							l_D3DFORMAT = D3DFMT_DXT4;
							break;
						case 4:
							l_D3DFORMAT = D3DFMT_DXT5;
							break;
						case 5:
							l_D3DFORMAT = D3DFMT_A8R8G8B8;
							break;
					}
					
					m_pCDDSConvert->ConvertDDS(l_strNewFileName,l_strNewFileName,l_D3DFORMAT);
				}
			}
			l_pIDirect3DTexture9->Release();
		}
		else
		{
			ErrorMsg_textBox->Text += "Error:"+e_strFileName+" write file failed "+"NewSize:"+l_uiWidth.ToString()+","+
				l_uiHeight.ToString()+DNCT::GetChanglineString();
		}
		l_pTexture->Release();
	}

	String^	Form1::NewDirectoryCheck(String^e_strDirectoryName,String^e_strFileName,String^e_strDelectedDirectory)
	{
		WCHAR	*l_strNewFileName = DNCT::GcStringToWchar(e_strDirectoryName+GetFileNameWithoutFullPath(e_strFileName));
		//original folder and file name length = original file name length - original directory length 
		int	l_iOriginalInternalFileNameLength = e_strFileName->Length;
		if( e_strDelectedDirectory )
			l_iOriginalInternalFileNameLength -= e_strDelectedDirectory->Length;
		//file name without full path directory length
		int	l_iFileNameLength = System::IO::Path::GetFileName(e_strFileName)->Length;
		//internal folder length = l_iOriginalInternalFileNameLength-l_iFileNameLength
		int	l_iInternalFolderLength = l_iOriginalInternalFileNameLength-l_iFileNameLength;
		//new folder name = new export folder path + internal folder name
		String^l_strNewDirectoryFolderPath;
		if( e_strDelectedDirectory )
		{
			l_strNewDirectoryFolderPath = e_strDirectoryName+e_strFileName->Substring(e_strDelectedDirectory->Length,l_iInternalFolderLength);
		}
		else
		{
			l_strNewDirectoryFolderPath = e_strDirectoryName;
		}
		l_strNewFileName = DNCT::GcStringToWchar(l_strNewDirectoryFolderPath+System::IO::Path::GetFileName(e_strFileName));
		//l_strNewFileName = DNCT::GcStringToWchar(e_strDirectoryName);
		CreateDirectory(l_strNewDirectoryFolderPath);
		//l_strNewFileName = e_strFileName
		return l_strNewDirectoryFolderPath;
	}

	void	Form1::ScaleImageWithDotNet(String^e_strDirectoryName,String^e_strFileName,String^e_strExtensionName)
	{
		Image^l_pImage = Image::FromFile(e_strFileName);
		if( l_pImage )
		{
			UINT	l_uiWidth = (UINT)Width_numericUpDown->Value;
			UINT	l_uiHeight = (UINT)Height_numericUpDown->Value;
			if( TransformType_comboBox->SelectedIndex == 0)//percentage
			{
				float	l_fOrigianlWidth = (float)l_pImage->Width;
				float	l_fOrigianlHeight = (float)l_pImage->Height;
				l_uiWidth = (UINT)(l_fOrigianlWidth*l_uiWidth/100.f);
				l_uiHeight = (UINT)(l_fOrigianlHeight*l_uiHeight/100.f);
			}
			WCHAR	*l_strNewFileName = DNCT::GcStringToWchar(e_strDirectoryName+GetFileNameWithoutFullPath(e_strFileName));
			String^l_strDirectory = e_strDirectoryName;
			if( CreateFolderIfNotReplaceOriginalFile_checkBox->Checked )
			{
				l_strDirectory = NewDirectoryCheck(e_strDirectoryName,e_strFileName,m_strDelectedDirectory);
			}
			try
			{
				if( TransformType_comboBox->SelectedIndex == 2)
				{
					System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( l_uiWidth,l_uiHeight,System::Drawing::Imaging::PixelFormat::Format32bppArgb);
					System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
					l_pGr->CompositingMode = System::Drawing::Drawing2D::CompositingMode::SourceCopy;l_pGr->Clear(System::Drawing::Color::Transparent);
					System::Drawing::Rectangle	l_DestRC = System::Drawing::Rectangle(0,0,l_uiWidth,l_uiHeight);
					System::Drawing::Rectangle	l_SrcRC = System::Drawing::Rectangle(0,0,l_uiWidth,l_uiHeight);
					l_pGr->DrawImage(l_pImage,l_DestRC,l_SrcRC,System::Drawing::GraphicsUnit::Pixel);
					String^l_strFileName = l_strDirectory+System::IO::Path::GetFileName(e_strFileName);
					l_pBitMap->Save(l_strFileName, System::Drawing::Imaging::ImageFormat::Png);
				}
				else
				{
					Image^l_pNewImage = ScaleImage(l_pImage, l_uiWidth, l_uiHeight);
					String^l_strFileName = l_strDirectory+System::IO::Path::GetFileName(e_strFileName);
					l_pNewImage->Save(l_strFileName, System::Drawing::Imaging::ImageFormat::Png);					
				}
			}
			catch(System::Exception^exp)
			{
				ErrorMsg_textBox->Text += exp->ToString()+" Error:"+e_strFileName+" write file failed "+"NewSize:"+l_uiWidth.ToString()+","+
					l_uiHeight.ToString()+DNCT::GetChanglineString();
			}
		}
	}

	HRESULT InitD3D( HWND hWnd )
	{
		// Create the D3D object, which is needed to create the D3DDevice.
		if( NULL == ( g_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
			return E_FAIL;

		// Set up the structure used to create the D3DDevice. Most parameters are
		// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
		// window, and then set the SwapEffect to "discard", which is the most
		// efficient method of presenting the back buffer to the display.  And 
		// we request a back buffer format that matches the current desktop display 
		// format.
		D3DPRESENT_PARAMETERS d3dpp;
		ZeroMemory( &d3dpp, sizeof( d3dpp ) );
		d3dpp.Windowed = TRUE;
		d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
		d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;

		// Create the Direct3D device. Here we are using the default adapter (most
		// systems only have one, unless they have multiple graphics hardware cards
		// installed) and requesting the HAL (which is saying we want the hardware
		// device rather than a software one). Software vertex processing is 
		// specified since we know it will work on all cards. On cards that support 
		// hardware vertex processing, though, we would see a big performance gain 
		// by specifying hardware vertex processing.
		if( FAILED( g_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
										  D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										  &d3dpp, &g_pd3dDevice ) ) )
		{
			return E_FAIL;
		}

		// Device state would normally be set here
		return S_OK;
	}




	//-----------------------------------------------------------------------------
	// Name: Cleanup()
	// Desc: Releases all previously initialized objects
	//-----------------------------------------------------------------------------
	VOID Cleanup()
	{
		if( g_pd3dDevice != NULL )
			g_pd3dDevice->Release();

		if( g_pD3D != NULL )
			g_pD3D->Release();
	}

	int	ToMaxMipMap(int e_iWidth,int e_iHeight)
	 {
		if( e_iWidth < 4||e_iHeight<4 )
			return 0;
		int	l_iMinmap = 0;
		int	l_i = 1;
		while( l_i<e_iWidth )
		{
			l_i = l_i<<1;
			l_iMinmap++;
		}
		int	l_iMinmap2 = 0;
		int	l_i2 = 1;
		while( l_i2<e_iHeight )
		{
			l_i2 = l_i2<<1;
			l_iMinmap2++;
		}
		return min(l_iMinmap2,l_iMinmap);
	 }

	LPDIRECT3DBASETEXTURE9 ImageResize(DWORD dwWidthNew, DWORD dwHeightNew,LPDIRECT3DBASETEXTURE9 pSrctex,DWORD e_dwMipMap,bool e_bIsVolumeMap,bool e_bIsCueMap)
	{
		HRESULT hr;
		LPDIRECT3DTEXTURE9 pmiptexNew;
		if( e_dwMipMap >1 )
		{
			int	l_iMaxMipMap = ToMaxMipMap(dwWidthNew,dwHeightNew);
			if( (DWORD)l_iMaxMipMap<e_dwMipMap )
				e_dwMipMap = l_iMaxMipMap;
		}
		hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, e_dwMipMap,
			 0, GetFormat(pSrctex,e_bIsVolumeMap,e_bIsCueMap), D3DPOOL_MANAGED, &pmiptexNew, NULL);
		if (FAILED(hr))
		{
			e_dwMipMap--;
		}
		hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, e_dwMipMap,
			 0, GetFormat(pSrctex,e_bIsVolumeMap,e_bIsCueMap), D3DPOOL_MANAGED, &pmiptexNew, NULL);
		if (FAILED(hr))
		{
			return 0;
		}
		if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, pSrctex, pmiptexNew,e_dwMipMap,e_bIsVolumeMap,e_bIsCueMap)))
			return 0;
		
		return pmiptexNew;

		//if( m_ptexNew != NULL )
		//{
		//    hr = g_pd3dDevice->CreateTexture(dwWidthNew, dwHeightNew, m_numMips, 
		//         0, GetFormat(m_ptexOrig), D3DPOOL_MANAGED, &pmiptexNew, NULL);
		//    if (FAILED(hr))
		//        return hr;
		//    if (FAILED(BltAllLevels(D3DCUBEMAP_FACE_FORCE_DWORD, m_ptexNew, pmiptexNew)))
		//        return hr;
		//    ReleasePpo(&m_ptexNew);
		//    m_ptexNew = pmiptexNew;
		//}
	}
	D3DFORMAT GetFormat(LPDIRECT3DBASETEXTURE9 ptex,bool e_bIsVolumeMap,bool e_bIsCueMap)
	{
		LPDIRECT3DTEXTURE9 pmiptex = NULL;
		LPDIRECT3DCUBETEXTURE9 pcubetex = NULL;
		LPDIRECT3DVOLUMETEXTURE9 pvoltex = NULL;
		D3DFORMAT fmt = D3DFMT_UNKNOWN;

		//if (IsVolumeMap())
		if( e_bIsVolumeMap )
			pvoltex = (LPDIRECT3DVOLUMETEXTURE9)ptex;
		//else if (IsCubeMap())
		else if( e_bIsCueMap )
			pcubetex = (LPDIRECT3DCUBETEXTURE9)ptex;
		else
			pmiptex = (LPDIRECT3DTEXTURE9)ptex;

		if (pvoltex != NULL)
		{
			D3DVOLUME_DESC vd;
			pvoltex->GetLevelDesc(0, &vd);
			fmt = vd.Format;
		}
		else if (pcubetex != NULL)
		{
			D3DSURFACE_DESC sd;
			pcubetex->GetLevelDesc(0, &sd);
			fmt = sd.Format;
		}
		else if( pmiptex != NULL )
		{
			D3DSURFACE_DESC sd;
			pmiptex->GetLevelDesc(0, &sd);
			fmt = sd.Format;
		}
		return fmt;
	}

	HRESULT BltAllLevels(D3DCUBEMAP_FACES FaceType,LPDIRECT3DBASETEXTURE9 ptexSrc, LPDIRECT3DBASETEXTURE9 ptexDest,DWORD e_dwNumMips,bool e_bIsVolumeMap,bool e_bIsCueMap)
	{
		HRESULT hr;
		LPDIRECT3DTEXTURE9 pmiptexSrc;
		LPDIRECT3DTEXTURE9 pmiptexDest;
		LPDIRECT3DCUBETEXTURE9 pcubetexSrc;
		LPDIRECT3DCUBETEXTURE9 pcubetexDest;
		LPDIRECT3DVOLUMETEXTURE9 pvoltexSrc;
		LPDIRECT3DVOLUMETEXTURE9 pvoltexDest;
		DWORD iLevel;

		if (e_bIsVolumeMap)
		{
			pvoltexSrc = (LPDIRECT3DVOLUMETEXTURE9)ptexSrc;
			pvoltexDest = (LPDIRECT3DVOLUMETEXTURE9)ptexDest;
		}
		else if (e_bIsCueMap)
		{
			pcubetexSrc = (LPDIRECT3DCUBETEXTURE9)ptexSrc;
			pcubetexDest = (LPDIRECT3DCUBETEXTURE9)ptexDest;
		}
		else
		{
			pmiptexSrc = (LPDIRECT3DTEXTURE9)ptexSrc;
			pmiptexDest = (LPDIRECT3DTEXTURE9)ptexDest;
		}

		for (iLevel = 0; iLevel < e_dwNumMips; iLevel++)
		{
			if( e_bIsVolumeMap )
			//if (IsVolumeMap())
			{
				LPDIRECT3DVOLUME9 pvolSrc = NULL;
				LPDIRECT3DVOLUME9 pvolDest = NULL;
				hr = pvoltexSrc->GetVolumeLevel(iLevel, &pvolSrc);
				hr = pvoltexDest->GetVolumeLevel(iLevel, &pvolDest);
				hr = D3DXLoadVolumeFromVolume(pvolDest, NULL, NULL, 
					pvolSrc, NULL, NULL, D3DX_DEFAULT, 0);
				if(pvolSrc)
					pvolSrc->Release();
				if(pvolDest)
					pvolDest->Release();
			}
			if( e_bIsCueMap )
			//else if (IsCubeMap())
			{
				LPDIRECT3DSURFACE9 psurfSrc = NULL;
				LPDIRECT3DSURFACE9 psurfDest = NULL;
				hr = pcubetexSrc->GetCubeMapSurface(FaceType, iLevel, &psurfSrc);
				hr = pcubetexDest->GetCubeMapSurface(FaceType, iLevel, &psurfDest);
				hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
					psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
				if(psurfSrc)
					psurfSrc->Release();
				if(psurfDest)
					psurfDest->Release();
			}
			else
			{
				LPDIRECT3DSURFACE9 psurfSrc = NULL;
				LPDIRECT3DSURFACE9 psurfDest = NULL;
				hr = pmiptexSrc->GetSurfaceLevel(iLevel, &psurfSrc);
				hr = pmiptexDest->GetSurfaceLevel(iLevel, &psurfDest);
				hr = D3DXLoadSurfaceFromSurface(psurfDest, NULL, NULL, 
					psurfSrc, NULL, NULL, D3DX_DEFAULT, 0);
				if(psurfSrc)
					psurfSrc->Release();
				if(psurfDest)
					psurfDest->Release();
			}
		}

		return S_OK;
	}
}

using namespace TextureScale;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// 建立任何控制項之前，先啟用 Windows XP 視覺化效果
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// 建立主視窗並執行
	Application::Run(gcnew Form1());
	return 0;
}
