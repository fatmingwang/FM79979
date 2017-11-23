#include "stdAfx.h"
#include "CoreDotNetTools.h"
#include "../../Include/IL/il.h"
#include "../Core/GameplayUT/StringCompress.h"
#pragma comment(lib, "../../../lib/Devil.lib")
namespace DNCT
{
	System::Windows::Forms::TreeNode^	MPDIListToTreeNode(cMPDIList*e_pMPDIList)
	{
        System::Windows::Forms::TreeNode^l_pTreeNode = gcnew System::Windows::Forms::TreeNode(String(e_pMPDIList->GetCharName().c_str()).ToString());
        l_pTreeNode->Name = String(e_pMPDIList->GetCharName().c_str()).ToString();
        for( int i=0;i<e_pMPDIList->Count();++i )
        {
			l_pTreeNode->Nodes->Add(String((*e_pMPDIList)[i]->GetCharName().c_str()).ToString())->Name = String((*e_pMPDIList)[i]->GetCharName().c_str()).ToString();
        }
        return l_pTreeNode;	
	}

	System::Windows::Forms::TreeNode^	PIToTreeNode(cPuzzleImage*e_pPI)
	{
		System::Windows::Forms::TreeNode^l_pTreeNode = gcnew System::Windows::Forms::TreeNode(String(e_pPI->GetCharName().c_str()).ToString());
        l_pTreeNode->Name = String(e_pPI->GetCharName().c_str()).ToString();
        for( int i=0;i<e_pPI->Count();++i )
        {
			l_pTreeNode->Nodes->Add(String((*e_pPI)[i]->GetCharName().c_str()).ToString())->Name = String((*e_pPI)[i]->GetCharName().c_str()).ToString();
        }
        return l_pTreeNode;
	}


	void	ImagerParserAddIntiTreeView(cNamedTypedObjectVector<NamedTypedObject>*e_pImageParser,System::Windows::Forms::TreeView^e_pTreeView,bool e_bClearTreeViewData)
	{
	    if( e_bClearTreeViewData )
	        e_pTreeView->Nodes->Clear();
	    for(int i=0;i<e_pImageParser->Count();++i)
	    {
	        if( (*e_pImageParser)[i]->Type() == cPuzzleImage::TypeID )
	        {
                e_pTreeView->Nodes->Add(PIToTreeNode(dynamic_cast<cPuzzleImage*>((*e_pImageParser)[i])));
	        }
	        else
	        {
	            e_pTreeView->Nodes->Add(String((*e_pImageParser)[i]->GetCharName().c_str()).ToString())->Name = String((*e_pImageParser)[i]->GetCharName().c_str()).ToString();
	        }
	    }
	}

	bool	RemoveTextureFromSelectedNodesTreeView(System::Windows::Forms::TreeView^e_pTreeView,cImageParser*e_pImageParser)
	{
		if(!e_pTreeView->SelectedNode)
			return false;
		if(!e_pTreeView->SelectedNode->Parent)
		{
			 if( !e_pImageParser->GetObject(DNCT::GcStringToWchar(e_pTreeView->SelectedNode->Text)) )
			 {
				WARNING_MSG("there is no such texture in the image parser!!");
				return false;
			 }
			 else
				return e_pImageParser->RemoveObject(DNCT::GcStringToWchar(e_pTreeView->SelectedNode->Text));
		}
		else
		{
			 WARNING_MSG("please select pi root");
			 return false;
		}
	}

	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName)
	{
		System::Drawing::Bitmap^l_pppp;
		ILuint s_iImageID;


		System::Drawing::Bitmap^l_pImage;
		String^l_strExtensionName = System::IO::Path::GetExtension(e_strFileName);
		if( l_strExtensionName->Equals(".dds")||
			l_strExtensionName->Equals(".DDS"))
		{
			ilGenImages(1, &s_iImageID);
			ilBindImage(s_iImageID);

			if (0==ilLoadImage((WCHAR*)DNCT::GcStringToChar(e_strFileName)))
			{
				return l_pppp;
			}
			int iW = ilGetInteger(IL_IMAGE_WIDTH);
			int iH = ilGetInteger(IL_IMAGE_HEIGHT);
			bool bFlip = ilGetInteger(IL_IMAGE_ORIGIN) == IL_ORIGIN_LOWER_LEFT;

			bool bResize = false;
			System::Drawing::Bitmap^ pBmp = gcnew System::Drawing::Bitmap(iW, iH, System::Drawing::Imaging::PixelFormat::Format32bppArgb);				

			System::Drawing::Rectangle rect; rect.X = 0; rect.Y = 0; rect.Width = iW; rect.Height = iH;

			System::Drawing::Imaging::BitmapData^ pBd = pBmp->LockBits(rect, 
				System::Drawing::Imaging::ImageLockMode::WriteOnly,
				System::Drawing::Imaging::PixelFormat::Format32bppArgb);

			void* pScan0 = (void*)(pBd->Scan0.ToPointer());
			ilConvertImage(IL_BGRA, IL_UNSIGNED_BYTE); // support for non 32bit images..
			//windows if BGRA
			//ilCopyPixels(0, 0, 0, iW, iH, 1, IL_BGRA, IL_UNSIGNED_BYTE, pScan0);
			//
			ilCopyPixels(0, 0, 0, iW, iH, 1, IL_RGBA, IL_UNSIGNED_BYTE, pScan0);
			
			ilDeleteImages(1, &s_iImageID);

			pBmp->UnlockBits(pBd);

			if (bFlip) pBmp->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
			return pBmp;
		}
		else
		{
			bool	l_bBinaryFile = false;
			if( String::Compare(l_strExtensionName,".pngb") == 0 )
				l_bBinaryFile = true;
			try
			{
				l_pImage = gcnew System::Drawing::Bitmap(e_strFileName);
			}
			catch(System::Exception^e)
			{
				String^l_str = e->ToString();
				l_pImage = nullptr;
				l_bBinaryFile = true;
			}
			if( l_bBinaryFile )
			{
				String^l_strExtensionName = System::IO::Path::GetExtension(e_strFileName);
				String^l_strTempFileName = System::IO::Path::GetDirectoryName(e_strFileName)+"/1234abcdXXZZ.png";
				std::string	l_strTempFileName2 = UT::GetDirectoryWithoutFileName(DNCT::GcStringToChar(e_strFileName));
				l_strTempFileName2	+= "1234abcdXXZZ.png";
				if( FileDempressHuffman(DNCT::GcStringToChar(e_strFileName),l_strTempFileName2.c_str()) )
				{
					
					System::IO::FileStream^ binStream = gcnew System::IO::FileStream(l_strTempFileName,System::IO::FileMode::Open);
					l_pImage = gcnew System::Drawing::Bitmap(binStream);
					binStream->Close();
					delete binStream;
					System::IO::File::Delete(l_strTempFileName);
				}
				//cSimpleGLTexture*l_pSimpleGLTexture = new cSimpleGLTexture(DNCT::GcStringToChar(e_strFileName),true);
				//int	l_iWidth = l_pSimpleGLTexture->GetWidth();
				//int	l_iHeight = l_pSimpleGLTexture->GetHeight();
				//System::Drawing::Imaging::PixelFormat	l_PixelFormat = l_pSimpleGLTexture->GetPixelFormat() == GL_RGBA?System::Drawing::Imaging::PixelFormat::Format32bppArgb:System::Drawing::Imaging::PixelFormat::Format24bppRgb;
				//System::Drawing::Bitmap^ pBmp = gcnew System::Drawing::Bitmap(l_iWidth, l_iHeight,l_PixelFormat);
				//System::Drawing::Rectangle rect; rect.X = 0; rect.Y = 0; rect.Width = l_iWidth; rect.Height = l_iHeight;
				//System::Drawing::Imaging::BitmapData^ pBd = pBmp->LockBits(rect,System::Drawing::Imaging::ImageLockMode::WriteOnly,l_PixelFormat);
				//void* pScan0 = (void*)(pBd->Scan0.ToPointer());
				////memcpy(pScan0,l_pSimpleGLTexture->GetPixels(),l_iWidth*l_iHeight*l_pSimpleGLTexture->GetPixelFormat() == GL_RGBA?4:3);
				//memset(pScan0,150,l_iWidth*l_iHeight*l_pSimpleGLTexture->GetPixelFormat() == GL_RGBA?4:3);
				//pBmp->UnlockBits(pBd);
				//delete l_pSimpleGLTexture;
				//l_pImage = pBmp;
			}
			//else
			//	l_pImage = gcnew System::Drawing::Bitmap(e_strFileName);
		}
		return l_pImage;
	}


	TiXmlElement*TreeNodeToTinyXMLNode(GCFORM::TreeNode^e_pNode,TiXmlElement*e_pParent)
	{
		TiXmlElement*l_pNewNode = new TiXmlElement(DNCT::GcStringToWchar(e_pNode->Text));
		if( e_pParent )
			e_pParent->LinkEndChild(l_pNewNode);
		for each( GCFORM::TreeNode^l_pChildNode in  e_pNode->Nodes )
		{
			//it's comment
			if(l_pChildNode->Text->Equals(XML_COMMENT))
			{
				continue;
			}
			//it's element
			if( l_pChildNode->Name->Length == 0 )
			{
				TreeNodeToTinyXMLNode(l_pChildNode,l_pNewNode);
			}
			else//it's attribute
			{
				wstring	l_strName = DNCT::GcStringToWchar(l_pChildNode->Name);
				wstring	l_strText = DNCT::GcStringToWchar(l_pChildNode->Text);
				l_pNewNode->SetAttribute( l_strText.c_str(),l_strName.c_str() );
			}
		}
		return l_pNewNode;
	}

	void	TinyXMLNodeToTreeNode(TiXmlElement*e_pTiXmlElement,GCFORM::TreeNode^e_pParentNode)
	{
		PARSE_ELEMENT_START(e_pTiXmlElement)
			GCFORM::TreeNode^l_pTreeNode = gcnew GCFORM::TreeNode();
			l_pTreeNode->Name = DNCT::WcharToGcstring(l_strValue);
			l_pTreeNode->Text = DNCT::WcharToGcstring(l_strName);
			e_pParentNode->Nodes->Add(l_pTreeNode);
		TO_NEXT_VALUE
		TiXmlElement*l_pTiXmlElement = e_pTiXmlElement->FirstChildElement();
		//while( l_pTiXmlElement )
		if( l_pTiXmlElement )
		{
			GCFORM::TreeNode^l_pTreeNode = gcnew GCFORM::TreeNode();
			l_pTreeNode->Text = DNCT::WcharToGcstring(l_pTiXmlElement->Value());
			e_pParentNode->Nodes->Add(l_pTreeNode);
			TinyXMLNodeToTreeNode(l_pTiXmlElement,l_pTreeNode);


			TiXmlElement*l_pNextSiblingElement = l_pTiXmlElement->NextSiblingElement();
			while( l_pNextSiblingElement )
			{
				GCFORM::TreeNode^l_pNextSiblinTreeNode = gcnew GCFORM::TreeNode();
				l_pNextSiblinTreeNode->Text = DNCT::WcharToGcstring(l_pNextSiblingElement->Value());
				e_pParentNode->Nodes->Add(l_pNextSiblinTreeNode);
				TinyXMLNodeToTreeNode(l_pNextSiblingElement,l_pNextSiblinTreeNode);
				l_pNextSiblingElement = l_pNextSiblingElement->NextSiblingElement();
			}
			//l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
		}
	}

	GCFORM::TreeNode^TinyXMLNodeToTreeNode(TiXmlElement*e_pTiXmlElement)
	{
		GCFORM::TreeNode^l_pNode;
		if(e_pTiXmlElement)
		{
			l_pNode = gcnew GCFORM::TreeNode();
			l_pNode->Text = DNCT::WcharToGcstring(e_pTiXmlElement->Value());
			TinyXMLNodeToTreeNode(e_pTiXmlElement,l_pNode);
		}
		return l_pNode;
	}

	System::Drawing::Bitmap^OpenImageFile(System::String^e_strFileName,cUIImage**e_pUIImage)
	{
		System::Drawing::Bitmap^l_pImage;
		String^l_strExtensionName = System::IO::Path::GetExtension(e_strFileName);
		if( l_strExtensionName->Equals(".dds")||
			l_strExtensionName->Equals(".DDS"))
		{
			assert(*e_pUIImage == 0);
			cUIImage*l_pUIImage = 0;
			l_pUIImage = new cUIImage(DNCT::GcStringToChar(e_strFileName));
			//cSimpleGLTexture	l_SimpleGLTexture(DNCT::GcStringToChar(e_strFileName),true);
			int	l_iChannel = l_pUIImage->GetPixelFormat()==GL_RGB?3:4;
			System::Drawing::Imaging::PixelFormat	l_PizelFormat = l_iChannel==4?System::Drawing::Imaging::PixelFormat::Format32bppArgb:System::Drawing::Imaging::PixelFormat::Format24bppRgb;
			l_pImage = gcnew System::Drawing::Bitmap( l_pUIImage->GetWidth(),l_pUIImage->GetHeight(),l_PizelFormat);
			System::Drawing::Rectangle l_Rect; l_Rect.X = 0; l_Rect.Y = 0; l_Rect.Width = l_pUIImage->GetWidth(); l_Rect.Height = l_pUIImage->GetHeight();
			System::Drawing::Imaging::BitmapData^ l_pBd = l_pImage->LockBits(l_Rect,System::Drawing::Imaging::ImageLockMode::WriteOnly,l_PizelFormat);
			void* pScan0 = (void*)(l_pBd->Scan0);
			memcpy(pScan0,l_pUIImage->GetTexture()->GetPixels(),sizeof(byte)*l_Rect.Width*l_Rect.Height*l_iChannel);
			if( l_iChannel == 4 )
			{
				//conver to dot net color
				//rgba to abgr
				UInt32* d = (UInt32*)pScan0;
				int l_iSize = l_pBd->Width * l_pBd->Height;
				for (int i = 0; i < l_iSize; i++)
				{
					*d = OpenglToDotNetColor(*d);
					++d;
				}
			}
			l_pImage->UnlockBits(l_pBd);
			POINT	l_OriginalImageSize = {l_pImage->Width,l_pImage->Height};
			l_pUIImage->SetOriginalImageSize(l_OriginalImageSize);
			*e_pUIImage = l_pUIImage;
		}
		else
		{
			l_pImage = gcnew System::Drawing::Bitmap(e_strFileName);
		}
		return l_pImage;
	}

	UINT	OpenglToDotNetColor(UINT32 e_RGBA)
	{
		return e_RGBA;
		UInt32 R = (e_RGBA&0xff000000)>>16;
		UInt32 G = (e_RGBA&0x00ff0000);
		UInt32 B = (e_RGBA&0x0000ff00)<<16;
		UInt32 A = (e_RGBA&0x000000ff);
		return R|G|B|A;	
	}
};