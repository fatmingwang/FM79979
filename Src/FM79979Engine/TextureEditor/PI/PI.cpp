// PI.cpp : main project file.

#include "stdafx.h"
#include "PIForm.h"

#include "../../../Include/IL/il.h"
#include "../../AllLibInclude.h"
#pragma comment(lib, "../../../lib/Devil.lib")
#include "../../Core/Common/BinaryFile/StringCompress.h"

//#include "../../../include/vld.h"
//#pragma comment(lib, "../../../lib/vld.lib")

using namespace System::Drawing::Imaging;
namespace PI
{

	System::Drawing::Bitmap^	ImageWithTrianulator(System::Drawing::Bitmap^e_pSrc, std::vector<Vector2>*e_pVector)
	{
		if (e_pVector)
		{
			List<System::Drawing::Point>^l_pPointList = Vector2ToListPoint(e_pVector);
			Image^l_pImage = (Image^)e_pSrc;
			Bitmap^l_pFinalImage = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList);
			return l_pFinalImage;
		}
		return nullptr;
	}

	//e_SrcPosition for Src's memory to change
	//e_DestPosition for Dest data for copy.
	//it will add 1 for e_SrcPosition,e_DestPosition
	void	CopyImageToImageBySpecificData(System::Drawing::Bitmap^e_pSrc,System::Drawing::Bitmap^e_pDest,RECT e_SrcPosition,RECT e_DestPosition)
	{
		e_DestPosition.right += 1;
		e_DestPosition.bottom +=1;
		e_SrcPosition.right += 1;
		e_SrcPosition.bottom += 1;
		int	l_iSrcChannel = e_pSrc->PixelFormat == PixelFormat::Format32bppArgb?4:3;
		//if( e_pSrc->PixelFormat == e_pDest->PixelFormat && e_pDest->Width>=e_pSrc->Width && e_pDest->Height>=e_pSrc->Height )
		//{
		//	int	bufferSizeInPixels = e_pSrc->Width*e_pSrc->Height;
		//	BitmapData^l_pData  = e_pSrc->LockBits(System::Drawing::Rectangle(0, 0,e_pSrc->Width,e_pSrc->Height),ImageLockMode::ReadOnly,e_pSrc->PixelFormat);
		//	char*l_pbuff = (char*)l_pData->Scan0.ToPointer();
		//	BitmapData^l_pData2 = e_pDest->LockBits(System::Drawing::Rectangle(0,0,e_pDest->Width,e_pDest->Height),ImageLockMode::WriteOnly,e_pDest->PixelFormat);
		//	char*l_strScrData = (char*)l_pData2->Scan0.ToPointer();
		//	int	l_iSrcRenderStartPosX = e_SrcPosition.left;
		//	int	l_iSrcRenderStartPosY = e_SrcPosition.top;
		//	int	l_iSrcRenderEndPosX = e_SrcPosition.right;
		//	int	l_iSrcRenderEndPosY = e_SrcPosition.bottom;
		//	int	l_iDestRenderPosX = e_DestPosition.left;
		//	int	l_iDestRenderPosY = e_DestPosition.top;
		//	//src's pixels for copy
		//	int	l_iWorkPixelX = e_SrcPosition.right-e_SrcPosition.left;
		//	int	l_iWorkPixelY = e_SrcPosition.bottom-e_SrcPosition.top;
		//	int	l_iIndex = 0;
		//	for( int l_iStartPixelY=l_iSrcRenderStartPosY;l_iStartPixelY<l_iSrcRenderEndPosY;++l_iStartPixelY )
		//	{
		//		int	l_iYIndex = ((l_iDestRenderPosY+l_iIndex)*l_iSrcChannel*e_pDest->Width);
		//		int	l_iXIndex = l_iSrcChannel*l_iDestRenderPosX;
		//		int	l_iStartCopyIndex = l_iXIndex+l_iYIndex;
		//		int	l_iCopyIntoIndex = (l_iStartPixelY*l_iSrcChannel*e_pSrc->Width)+(l_iSrcChannel*l_iSrcRenderStartPosX);
		//		memcpy(&l_strScrData[l_iStartCopyIndex],&l_pbuff[l_iCopyIntoIndex],l_iWorkPixelX*l_iSrcChannel);
		//		++l_iIndex;
		//	}
		//	e_pSrc->UnlockBits(l_pData);
		//	e_pDest->UnlockBits(l_pData2);
		//}
		//else
		{
			System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(e_pDest);
			l_pGr->CompositingMode = System::Drawing::Drawing2D::CompositingMode::SourceOver;
			l_pGr->DrawImage(e_pSrc,
				System::Drawing::Rectangle(e_DestPosition.left,e_DestPosition.top,e_DestPosition.right-e_DestPosition.left,e_DestPosition.bottom-e_DestPosition.top)
				,System::Drawing::Rectangle(e_SrcPosition.left,e_SrcPosition.top,e_SrcPosition.right-e_SrcPosition.left,e_SrcPosition.bottom-e_SrcPosition.top)
				,System::Drawing::GraphicsUnit::Pixel);
		}
	}





	int	StripFloatIfBiggerThanPoint5(float e_fValue)
	{
		int	l_iValue = (int)e_fValue;
		if(e_fValue-l_iValue>0.9f)
			return l_iValue+1;
		return l_iValue;
	}

	void		cPIEditor::AttachObjectPosAdjust()
	{
		for( int i=1;i<m_pImageomposerIRM->Count();++i )
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
			if( l_pUIImage->m_pEditorAttachParent )
			{
				l_pUIImage->SetPos(l_pUIImage->m_pEditorAttachParent->GetPos()+l_pUIImage->m_vEditorAttachParentRelativePos);
			}
		}	
	}
//=======================
//
//=======================
	cUIImage*	cPIEditor::GetNewUIImageByBitMap(System::Drawing::Bitmap^e_pBitMap,const WCHAR*e_strName)
	{
		int	l_iChannel = e_pBitMap->PixelFormat==PixelFormat::Format32bppArgb?4:3;
		int	bufferSizeInPixels = e_pBitMap->Width*e_pBitMap->Height;
		char*l_pbuff = new char[bufferSizeInPixels*l_iChannel];
		BitmapData^l_pData =
		e_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, e_pBitMap->Width, e_pBitMap->Height),
		ImageLockMode::ReadWrite,e_pBitMap->PixelFormat);
		memcpy(l_pbuff,l_pData->Scan0.ToPointer(),bufferSizeInPixels*l_iChannel);
		e_pBitMap->UnlockBits(l_pData);
		//if set opengl as bgr do not do this
		//for (int i = 0; i < bufferSizeInPixels; i++)
		//{
		//	int	l_iCurrentIndex = 4*i;
		//	char	l_OrginalColor[4] = {l_pbuff[l_iCurrentIndex],l_pbuff[l_iCurrentIndex+1],l_pbuff[l_iCurrentIndex+2],l_pbuff[l_iCurrentIndex+3]};
		//	l_pbuff[l_iCurrentIndex] = l_OrginalColor[2]; //This is the buffer value of  R.
		//	l_pbuff[l_iCurrentIndex+1] = l_OrginalColor[1];//This is the buffer value of G.
		//	l_pbuff[l_iCurrentIndex+2] = l_OrginalColor[0];//This is the buffer value of B.
		//	l_pbuff[l_iCurrentIndex+3] = l_OrginalColor[3];//This is the buffer value of A.
		//}
		cUIImage*l_pUIImage = new cUIImage(e_strName,(char*)l_pbuff,e_pBitMap->Width, e_pBitMap->Height,e_pBitMap->PixelFormat==PixelFormat::Format32bppArgb?GL_BGRA:GL_BGR);
		POINT	l_OriginalImageSize = {e_pBitMap->Width,e_pBitMap->Height};
		l_pUIImage->SetOriginalImageSize(l_OriginalImageSize);
		//cUIImage*l_pUIImage = new cUIImage(e_strName,(char*)l_pbuff,e_pBitMap->Width, e_pBitMap->Height,e_pBitMap->PixelFormat==PixelFormat::Format32bppArgb?GL_RGBA:GL_RGB);
		return l_pUIImage;
	}

	System::Void	cPIEditor::GeneratePowOf2Image(bool e_bPowerOfTwo)
	{
		String^l_strDirectory = DNCT::SelectDirectory();
		if( !l_strDirectory )
			return;
		System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( System::Drawing::Color::Transparent );
		for( int i=0;i<m_pImageomposerIRM->Count();++i )
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(((*m_pImageomposerIRM)[i]));
			System::Drawing::Bitmap^l_pBitmapForSave = (System::Drawing::Bitmap^)m_ImageTale[DNCT::WcharToGcstring((*m_pImageomposerIRM)[i]->GetName())];
			int	l_iWidth = l_pBitmapForSave->Width;
			int	l_iHeight = l_pBitmapForSave->Height;
			POINT	l_ImageSize = {l_pBitmapForSave->Width,l_pBitmapForSave->Height};
			//for power of two
			if( e_bPowerOfTwo )
			{
				l_ImageSize.x = power_of_two(l_ImageSize.x);
				l_ImageSize.y = power_of_two(l_ImageSize.y);
				//fill all alpha
				System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( l_ImageSize.x,l_ImageSize.y,System::Drawing::Imaging::PixelFormat::Format32bppArgb );
				System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
				l_pGr->FillRectangle(l_pDrawBrush,0,0,l_pBitMap->Width,l_pBitMap->Height);
				//render image
				l_pGr->DrawImage(l_pBitmapForSave,0,0,l_pBitmapForSave->Width,l_pBitmapForSave->Height);
				l_pBitMap->Save(l_strDirectory+DNCT::WcharToGcstring(l_pUIImage->GetName())+".png");
				delete l_pBitMap;
			}
			else
			{
				l_pBitmapForSave->Save(l_strDirectory+DNCT::WcharToGcstring(l_pUIImage->GetName())+".png");
			}
		}
	}


	class	cPuzzleUnitChild:public NamedTypedObject
	{
	public:
		std::wstring	m_strAttachParentName;
		Vector3			m_vRelativePos;
	};
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       
	bool	cPIEditor::ParsePuzzleImage(cPuzzleImage*e_pPuzzleImage,String^e_strFileName)
	{
		cNamedTypedObjectVector<cPuzzleUnitChild>	l_PIUnitChildVector;
		cNodeISAX	l_cNodeISAX;
		if(l_cNodeISAX.ParseDataIntoXMLNode(DNCT::GcStringToChar(e_strFileName).c_str()))
		{
			TiXmlElement*l_pTiXmlElement = l_cNodeISAX.GetRootElement();
			l_pTiXmlElement = l_pTiXmlElement->FirstChildElement();
			while( l_pTiXmlElement  )
			{
				if(!wcscmp(l_pTiXmlElement->Value(),L"PuzzleUnitChild"))
				{
					cPuzzleUnitChild*l_pPuzzleUnitChild = new cPuzzleUnitChild();
					l_pPuzzleUnitChild->SetName(l_pTiXmlElement->Attribute(L"Name"));
					l_pPuzzleUnitChild->m_strAttachParentName = l_pTiXmlElement->Attribute(L"AttachParent");
					l_pPuzzleUnitChild->m_vRelativePos = GetVector3(l_pTiXmlElement->Attribute(L"RelativePos"));
					l_PIUnitChildVector.AddObjectNeglectExist(l_pPuzzleUnitChild);
				}
				l_pTiXmlElement = l_pTiXmlElement->NextSiblingElement();
			}
		}
		const WCHAR*	l_strPuzzleImageName = e_pPuzzleImage->GetName();
		int	l_iNumImage = e_pPuzzleImage->GetNumImage();
		String^l_strCurrenDirectory = DNCT::GetDirectoryWithoutFileName(e_strFileName);
		String^l_strOriginalImageFileName ;
		if(l_cNodeISAX.GetRootElement()->Attribute(L"ImageName"))
			l_strOriginalImageFileName = l_strCurrenDirectory+DNCT::WcharToGcstring((WCHAR*)l_cNodeISAX.GetRootElement()->Attribute(L"ImageName"));
		else
			l_strOriginalImageFileName = l_strCurrenDirectory+DNCT::WcharToGcstring(e_pPuzzleImage->GetName())+".png";
		try
		 {
			//UT::sTimeAndFPS	l_t;
			System::Drawing::Bitmap^l_pOriginalImage;
			if( 1 )
			{
				//l_t.Update();
				l_pOriginalImage = DNCT::OpenImageFile(l_strOriginalImageFileName);
				//l_t.Update();
				//String^l_strExtensionName = System::IO::Path::GetExtension(l_strOriginalImageFileName);
				//if( l_strExtensionName->Equals(".dds")||
				//	l_strExtensionName->Equals(".DDS"))
					//l_pOriginalImage->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
			}
			else
				l_pOriginalImage = gcnew System::Drawing::Bitmap(l_strOriginalImageFileName);


			 
			 System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( System::Drawing::Color::FromArgb(0,0,0,0) );
			 System::Drawing::Bitmap^l_pBitMap;
			 int l_OriginalImageWidth = l_pOriginalImage->Width;
			 int l_OriginalImageHeight = l_pOriginalImage->Height;
			 bool	l_bPowerOfTwoImage = false;
			 if( l_OriginalImageWidth == UT::power_of_two(l_OriginalImageWidth)
				 &&l_OriginalImageHeight == UT::power_of_two(l_OriginalImageHeight))
				 l_bPowerOfTwoImage = true;
			 if( !g_bSupportNonPowerOfTwoTexture )//because we has change the UV,if it doesn't support power of two,here have to forec image size to power of two
			 {
				 l_OriginalImageWidth = UT::power_of_two(l_OriginalImageWidth);
				 l_OriginalImageHeight = UT::power_of_two(l_OriginalImageHeight);
			 }
			 bool l_bTextureHasBeenReized = false;
			 for( int i=0;i<l_iNumImage;++i )
			 {
				 sPuzzleData*l_pPuzzleData = e_pPuzzleImage->GetPuzzleData(i);
				 String^l_strFileName = DNCT::WcharToGcstring(l_pPuzzleData->strFileName)+".png";
				 float l_fPosX = l_pPuzzleData->fUV[0]*l_OriginalImageWidth;
				 float l_fPosY = l_pPuzzleData->fUV[1]*l_OriginalImageHeight;
				System::Drawing::Rectangle	l_SrcRC = 
					System::Drawing::Rectangle(
					(int)l_fPosX,
					(int)l_fPosY,
					//-1 for start at 0,0 if size is 256,256 it should be 0,0~255,255 so the size is match 256,256
					l_pPuzzleData->Size.x,
					l_pPuzzleData->Size.y);
				//if( (int)l_fPosX+l_pPuzzleData->Size.x>= l_pOriginalImage->Width || 
				//	(int)l_fPosY+l_pPuzzleData->Size.y>= l_pOriginalImage->Height)
				if ((int)l_fPosX + l_pPuzzleData->Size.x > l_pOriginalImage->Width + 1 ||//I am lazy to fix this bug
					(int)l_fPosY + l_pPuzzleData->Size.y > l_pOriginalImage->Height + 1)
				{
					l_bTextureHasBeenReized = true;
				}
				 //so reassign image to right position,more accurate,and to capable for old pi data
				if( (l_pPuzzleData->fUV[0] != 0.f||l_pPuzzleData->fUV[1] != 0.f) && (l_pPuzzleData->ShowPosInPI.x!=0||l_pPuzzleData->ShowPosInPI.y!=0) )
				 {
					 l_fPosX = (float)l_pPuzzleData->ShowPosInPI.x;
					 l_fPosY = (float)l_pPuzzleData->ShowPosInPI.y;
					 l_SrcRC = 
						System::Drawing::Rectangle(
						l_pPuzzleData->ShowPosInPI.x,
						l_pPuzzleData->ShowPosInPI.y,
						//-1 for start at 0,0 if size is 256,256 it should be 0,0~255,255 so the size is match 256,256
						l_pPuzzleData->Size.x,
						l_pPuzzleData->Size.y);
				 }
				 //if such file is not add
				 if( !m_pImageomposerIRM->GetObject(l_pPuzzleData->strFileName) )
				 {
					try
					{
						//prevent old version error
						if(l_pPuzzleData->OriginalSize.y == 0)
						{
							l_pPuzzleData->OriginalSize.y = l_pPuzzleData->Size.y;
							l_pPuzzleData->OriginalSize.x = l_pPuzzleData->Size.x;
						}
						l_pBitMap = gcnew System::Drawing::Bitmap( l_pPuzzleData->OriginalSize.x,l_pPuzzleData->OriginalSize.y,System::Drawing::Imaging::PixelFormat::Format32bppArgb );
						System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
						//l_pGr->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::HighQualityBicubic;
						l_pGr->CompositingMode = System::Drawing::Drawing2D::CompositingMode::SourceCopy;
						l_pGr->Clear(System::Drawing::Color::Transparent);
						System::Drawing::Rectangle	l_DestRC = System::Drawing::Rectangle(
							l_pPuzzleData->OffsetPos.x,l_pPuzzleData->OffsetPos.y,l_pPuzzleData->Size.x,l_pPuzzleData->Size.y);
						l_pGr->DrawImage(l_pOriginalImage,l_DestRC,l_SrcRC,System::Drawing::GraphicsUnit::Pixel);
						////for debug test
						//l_pBitMap->Save(i.ToString()+".png");
						m_ImageTale[gcnew String(DNCT::WcharToGcstring(l_pPuzzleData->strFileName))] = l_pBitMap;
					}
					catch(System::Exception^l_pExp)
					{
						WARNING_MSG(l_strCurrenDirectory+" not exists"+DNCT::GetChanglineString()+l_pExp->ToString());
					}

					if (m_pPuzzleImageUnitTriangulatorManager)
					{
						std::vector<Vector2>*l_pPointsVector = e_pPuzzleImage->GetImageShapePointVector(i);
						if (l_pPointsVector)
						{
							std::vector<Vector2>l_TriangulatorVector = Triangulator(l_pPointsVector);
							auto l_pTriangulatorBitmap = ImageWithTrianulator(l_pBitMap, &l_TriangulatorVector);

							l_pTriangulatorBitmap->Save(DNCT::WcharToGcstring(l_pPuzzleData->strFileName) + ".png");
							l_pBitMap = l_pTriangulatorBitmap;
							m_ImageTale[gcnew String(DNCT::WcharToGcstring(l_pPuzzleData->strFileName))] = l_pBitMap;
						}
					}
					cUIImage*l_pUIImage = GetNewUIImageByBitMap(l_pBitMap, l_pPuzzleData->strFileName.c_str());
					 //cUIImage*l_pUIImage = new cUIImage(DNCT::GcStringToChar(l_strCurrenDirectory+l_strFileName));
					l_pUIImage->SetOffsetPos(l_pPuzzleData->OffsetPos);
					POINT	l_RightDown = l_pPuzzleData->OffsetPos+l_pPuzzleData->Size;
					//start offset position at  0,0
					l_RightDown.x -=1;
					l_RightDown.y -=1;					
					l_pUIImage->SetRightDownStripOffPos(l_RightDown);
					POINT	l_OriginalSize = {l_pBitMap->Width,l_pBitMap->Height};
					l_pUIImage->SetOriginalImageSize(l_OriginalSize);
					bool l_bAddResult = m_pImageomposerIRM->AddObject(l_pUIImage);
					assert(l_bAddResult&&"same name!?");
					l_pUIImage->SetPos(Vector3(l_fPosX,l_fPosY,0.f));
					//fuck
					if (l_PIUnitChildVector.GetObjectIndexByName(l_pUIImage->GetName()) == -1)
					{
						this->m_pPuzzleImageUnitTriangulatorManager->AssignDataFromPuzzleImage(e_pPuzzleImage, l_pUIImage);
					}
					else
					{
						int a = 0;
					}
					//if(l_pPuzzleData->ShowPosInPI.x!=0||l_pPuzzleData->ShowPosInPI.y!=0)
					{
						 l_pUIImage->SetPos(Vector3((float)l_pPuzzleData->ShowPosInPI.x-l_pUIImage->GetOffsetPos()->x,
								(float)l_pPuzzleData->ShowPosInPI.y-l_pUIImage->GetOffsetPos()->y,0.f));
					}
					DNCT::CheckListContainStringAndAdd(AllImage_listBox,DNCT::WcharToGcstring(l_pPuzzleData->strFileName));
				 }
				 else
				 {//such data already exists
					 WARNING_MSG(DNCT::WcharToGcstring(l_pPuzzleData->strFileName)+" already existd");
					 SAFE_DELETE(e_pPuzzleImage);
					 continue;
				 }
			 }
			 delete l_pOriginalImage;
			 if( l_bTextureHasBeenReized )
			 {
				WARNING_MSG("texture has been resized!");
			 }
		 }
		 catch(System::Exception^l_pExp)
		 {
			 WARNING_MSG(l_strCurrenDirectory+" not exists"+DNCT::GetChanglineString()+l_pExp->ToString());
			 SAFE_DELETE(e_pPuzzleImage);
			 return false;
		 }
		 ImageWidth_numericUpDown->Value = e_pPuzzleImage->GetWidth();
		 ImageHeight_numericUpDown->Value = e_pPuzzleImage->GetHeight();
		 
		 for(int i=0;i<l_PIUnitChildVector.Count();++i)
		 {
			 cPuzzleUnitChild*l_pPuzzleUnitChild = l_PIUnitChildVector.GetObject(i);
			 if( m_ImageTale->ContainsKey(DNCT::WcharToGcstring(l_pPuzzleUnitChild->GetName())) )
				m_ImageTale->Remove(DNCT::WcharToGcstring(l_pPuzzleUnitChild->GetName()));

			 cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_pPuzzleUnitChild->GetName()));

			 cUIImage*l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_pPuzzleUnitChild->m_strAttachParentName.c_str()));
			 cUIImage*l_pNewUIImage = new cUIImage(l_pAttachUIImage);
			 l_pNewUIImage->m_pEditorAttachParent = l_pAttachUIImage;
			 l_pNewUIImage->m_vEditorAttachParentRelativePos = l_pPuzzleUnitChild->m_vRelativePos;
			 l_pNewUIImage->SetPos(l_pUIImage->GetPos());
			 l_pNewUIImage->SetName(l_pUIImage->GetName());
			 l_pNewUIImage->SetOffsetPos(*l_pUIImage->GetOffsetPos());
			 l_pNewUIImage->SetRightDownStripOffPos(l_pUIImage->GetRightDownStripOffPos());
			 l_pNewUIImage->SetOriginalImageSize(l_pUIImage->GetOriginalImageSize());
			 float l_fUV[4] =
			 {
				(float)l_pPuzzleUnitChild->m_vRelativePos.x/ l_pAttachUIImage->GetImageRealSize().x,
				(float)l_pPuzzleUnitChild->m_vRelativePos.y / l_pAttachUIImage->GetImageRealSize().y,
				(l_pPuzzleUnitChild->m_vRelativePos.x + (float)l_pNewUIImage->GetImageRealSize().x) / l_pAttachUIImage->GetImageRealSize().x,
				(l_pPuzzleUnitChild->m_vRelativePos.y + (float)l_pNewUIImage->GetImageRealSize().y) / l_pAttachUIImage->GetImageRealSize().y
			 };
			 l_pNewUIImage->SetUV(l_fUV);
			 l_pNewUIImage->SetWidth(l_pUIImage->GetOriginalImageSize().x);
			 l_pNewUIImage->SetHeight(l_pUIImage->GetOriginalImageSize().y);

			 std::vector<NamedTypedObject*>*l_pList = m_pImageomposerIRM->GetList();
			 int	l_iIndex = m_pImageomposerIRM->GetObjectIndexByPointer(l_pUIImage);
			 (*l_pList)[l_iIndex] = l_pNewUIImage;
			 this->m_pPuzzleImageUnitTriangulatorManager->AssignDataFromPuzzleImage(e_pPuzzleImage, l_pNewUIImage);
			 delete l_pUIImage;
		 }
		 SAFE_DELETE(e_pPuzzleImage);
		 return true;
	}

	void			cPIEditor::OpenPIFile(String^e_strFileName)
	{
		 this->timer1->Enabled = false;
		 AnimationData_listBox->Items->Clear();
		 AnimationDataList_listBox->Items->Clear();
		 m_pImageIndexOfAnimation->Clear();
		 m_pImageIndexOfAnimationList->Destroy();
		 m_pImageomposerIRM->Destroy();
		 AllImage_listBox->Items->Clear();
		 m_pPuzzleImageUnitTriangulatorManager->Destroy();
		 String^l_strFileName = e_strFileName;
		 if( l_strFileName )
		 {
				cPuzzleImage*l_pPuzzleImage = OpenPuzzleFile(l_strFileName);
				static	WCHAR	l_strPuzzleImageName[MAX_PATH];
				if( l_pPuzzleImage )
				{
					this->Text = e_strFileName;
				    cNamedTypedObjectVector<cImageIndexOfAnimation>*l_pImageIndexOfAnimationList =l_pPuzzleImage->GetImageIndexOfAnimationList();
				    if( l_pImageIndexOfAnimationList )
				    {
				        for( int i=0;i<l_pImageIndexOfAnimationList->Count();++i )
				        {
				            cImageIndexOfAnimation*l_pImageIndexOfAnimation = new cImageIndexOfAnimation(l_pImageIndexOfAnimationList->GetObject(i));
		                    m_pImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
		                    AnimationDataList_listBox->Items->Add(DNCT::WcharToGcstring(l_pImageIndexOfAnimation->GetName()));
		                }
		            }
		             
					swprintf(l_strPuzzleImageName,MAX_PATH,L"%ls\0",l_pPuzzleImage->GetName());
					//generate new ui image by pi
					if(!ParsePuzzleImage(l_pPuzzleImage,l_strFileName))
					{
						WARNING_MSG("error data");
					}
					else
					{
						m_strCurrentFileName = DNCT::WcharToGcstring(l_strPuzzleImageName);
					}
				}
				else
				{
					WARNING_MSG("error data");
				}
		 }
		 this->timer1->Enabled = true;	
	}
//=======================
//
//=======================
	cPuzzleImage*	cPIEditor::OpenPuzzleFile(String^e_strFileName)
	{
		 std::string l_strFileName = DNCT::GcStringToChar(e_strFileName);
		 cPuzzleImage*l_pPuzzleImage = m_pImageomposerIRM->GetPuzzleImageByFileName(l_strFileName.c_str());
		 if( l_pPuzzleImage )
		 {
			 if( m_pImageomposerIRM->GetObject(0)->Type() == cPuzzleImage::TypeID )
				 l_pPuzzleImage = dynamic_cast<cPuzzleImage*>(m_pImageomposerIRM->GetObject(0));
			 //ensure read the cPuzzleImage,or fetch data via animation parser,
			 if( !l_pPuzzleImage )
			 {
				return 0;
			 }
			 else
				m_pImageomposerIRM->RemoveObjectWithoutDelete(l_pPuzzleImage->GetName());
		 }
		 if (l_pPuzzleImage)
		 {
			 cNodeISAX	l_XMLParserOnlyForImagDistance;
			 if (l_XMLParserOnlyForImagDistance.ParseDataIntoXMLNode(l_strFileName.c_str()))
			 {
				 auto l_pRoot = l_XMLParserOnlyForImagDistance.GetRootElement();
				 auto l_strImageDistance = l_pRoot->Attribute(L"ImageDistance");
				 if (l_strImageDistance)
				 {
					 auto l_Dis = GetPoint(l_strImageDistance);
					 ImageDistanceX_numericUpDown->Value = l_Dis.x;
					 ImageDistanceY_numericUpDown->Value = l_Dis.y;
				 }
			 }
		 }
		 return l_pPuzzleImage;
	}
//=======================
//
//=======================
	System::Void	cPIEditor::SavePuzzleFile(String^e_strFileName,bool e_bBinary)
	{
		ImageDetail_textBox->Text = "Saving file...";
		auto l_strPIName = DNCT::GcStringToWchar(e_strFileName);
		l_strPIName = UT::GetFileNameWithoutFullPath(l_strPIName.c_str());
		for( int i=0;i<m_pImageomposerIRM->Count();++i )
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
			Vector3	l_vPos = l_pUIImage->GetPos();
			POINT	l_OffsetPos = *l_pUIImage->GetOffsetPos();
			POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
			if(l_OffsetPos.x+l_vPos.x<0 || l_vPos.x+l_RightDownStripOffPos.x>(float)ImageWidth_numericUpDown->Value||
				l_OffsetPos.y+l_vPos.y<0 || l_vPos.y+l_RightDownStripOffPos.y>(float)ImageHeight_numericUpDown->Value)
			{
				WARNING_MSG("image is not in the range");
				return;
			}
			if( l_pUIImage->GetImageRealSize().x%2 || l_pUIImage->GetImageRealSize().y%2 )
			{
				ImageDetail_textBox->Text += DNCT::WcharToGcstring(l_pUIImage->GetName())+"image width or height is not even"+DNCT::GetChanglineString();
			}
			if (l_pUIImage->IsSameName(l_strPIName.c_str()))
			{
				WARNING_MSG("PIUnit and file name can't be same:"+DNCT::WcharToGcstring(l_strPIName));
				return;
			}
		}
		if (this->m_pPuzzleImageUnitTriangulatorManager)
		{
			if (m_pPuzzleImageUnitTriangulatorManager->IsObjectOverlap())
			{
				WARNING_MSG("object is overlap are you sure you want to save file!?");
			}
		}
		 String^l_FileName = e_strFileName;
		 if( e_bBinary )
			 l_FileName += "b";
		 String^l_strXMLFileName = l_FileName;
		 if( l_FileName )
		 {
			if( !AllImage_listBox->Items->Count )
				return;
			for(int i=l_FileName->Length-1;i!=-1;--i)
			{
				if( l_FileName[i] == L'.' )
				{
					if(this->ImageSaveAsDDS_checkBox->Checked)
						l_FileName = gcnew String(l_FileName->Substring(0,i+1)+"dds");
					else
					{
						l_FileName = gcnew String(l_FileName->Substring(0,i+1)+"png");
					}
					break;
				}
			}
			sPuzzleData**l_ppPuzzleData = new sPuzzleData*[AllImage_listBox->Items->Count];
			for (int i = 0; i < AllImage_listBox->Items->Count; ++i)
			{
				l_ppPuzzleData[i] = nullptr;
			}
			System::Drawing::Imaging::ImageFormat^ l_ImageFormat = System::Drawing::Imaging::ImageFormat::Png;
			int	l_iWidth = (int)ImageWidth_numericUpDown->Value;
			int	l_iHeight = (int)ImageHeight_numericUpDown->Value;
			System::Drawing::Bitmap^l_pBitMap;
			System::Drawing::Graphics^l_pGr;
			if( !e_bBinary )
			{
				l_pBitMap = gcnew System::Drawing::Bitmap( l_iWidth,l_iHeight,System::Drawing::Imaging::PixelFormat::Format32bppArgb );
				int	l_iDestChannel = l_pBitMap->PixelFormat==PixelFormat::Format32bppArgb?4:3;
				//l_pBitMap-MakeTransparent();
				l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
				l_pGr->InterpolationMode = System::Drawing::Drawing2D::InterpolationMode::NearestNeighbor;
				l_pGr->SmoothingMode = System::Drawing::Drawing2D::SmoothingMode::None;
			}
			std::string	l_str = DNCT::GcStringToChar(l_FileName);
			char*	l_ExtensionName = &l_str[strlen(l_str.c_str())-3];
			sprintf(l_ExtensionName,"Image");
			//FILE*l_pFopen = fopen(l_str,"w");
			float	l_fUV[4];
			if( e_bBinary )
				sprintf(l_ExtensionName,"pib");
			else
				sprintf(l_ExtensionName,"pi");
			String^l_strUserNameAndData = GetUseerNameAndTime();
			std::string	l_strXmlFileName = l_str;
			ATG::XMLWriter	l_XMLWriter(l_str.c_str());
			l_XMLWriter.StartComment();
			l_XMLWriter.WriteString(DNCT::GcStringToChar(l_strUserNameAndData).c_str());
			l_XMLWriter.EndComment();
			l_XMLWriter.StartElement("PuzzleImage");
			    GCFORM::ListBox^l_pImageListBox = gcnew GCFORM::ListBox();
			    std::string	l_OriginalImageNameOrder;
			    for(int i=0;i<AllImage_listBox->Items->Count;++i)
			    {
				    String^l_strImageName = AllImage_listBox->Items[i]->ToString();
				    l_OriginalImageNameOrder += DNCT::GcStringToChar(l_strImageName);
				    if( i != AllImage_listBox->Items->Count-1 )
					    l_OriginalImageNameOrder += ",";
				    l_pImageListBox->Items->Add(l_strImageName);
			    }
			    l_pImageListBox->Sorted = true;
			    l_XMLWriter.AddAttribute("OriginalNameSort",l_OriginalImageNameOrder.c_str());
				if(this->ImageSaveAsDDS_checkBox->Checked)
					l_XMLWriter.AddAttribute("ImageName",DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_FileName,true)+".dds"));
				else
				{
					if( e_bBinary )
					{
						l_XMLWriter.AddAttribute("ImageName",DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_FileName,true)+".pngb"));
					}
					else
						l_XMLWriter.AddAttribute("ImageName",DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_FileName,true)+".png"));
				}
			    l_XMLWriter.AddAttribute("Count",m_pImageomposerIRM->Count());
			    l_XMLWriter.AddAttribute("GeneratePuzzleimageUnit","0");
				POINT	l_ImageDis = { (int)ImageDistanceX_numericUpDown->Value,(int)ImageDistanceY_numericUpDown->Value };
				l_XMLWriter.AddAttribute("ImageDistance", ValueToString(l_ImageDis));
			    //current only support png file
			    for( int i=0;i<AllImage_listBox->Items->Count;++i )
			    {
				    cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_pImageListBox->Items[i]->ToString())));
					if( !l_pUIImage )
					{
						UT::ErrorMsg(L"please do not named such weired name",DNCT::GcStringToWchar(l_pImageListBox->Items[i]->ToString()));
						continue;
					}
				    //int	l_iPixelSize = l_pUIImage->GetWidth()*l_pUIImage->GetHeight()*4;
				    POINT	l_RenderPoint = {(int)l_pUIImage->GetPos().x+l_pUIImage->GetOffsetPos()->x,(int)l_pUIImage->GetPos().y+l_pUIImage->GetOffsetPos()->y};
				    POINT	l_ImageRealPixelSize = l_pUIImage->GetImageRealSize();
					if( !e_bBinary )
					{
						System::Drawing::Bitmap^l_pBitmapForSave = nullptr;
						if (!l_pUIImage->m_pEditorAttachParent)
						{
							l_pBitmapForSave = (System::Drawing::Bitmap^)m_ImageTale[l_pImageListBox->Items[i]->ToString()];
						}
						//System::Drawing::Rectangle	l_UIIMageRealPixelRect(
						//    l_pUIImage->GetOffsetPos()->x,
						   // l_pUIImage->GetOffsetPos()->y,
						   // l_ImageRealPixelSize.x,
						   // l_ImageRealPixelSize.y+1);
						if( DebugLine_checkBox->Checked )
						{
							System::Drawing::Rectangle	l_DrawToFinalImageRect
								(l_RenderPoint.x,
								l_RenderPoint.y,
								l_ImageRealPixelSize.x,
								l_ImageRealPixelSize.y+1);
							l_pGr->DrawRectangle(gcnew System::Drawing::Pen(System::Drawing::Color::Green,1),l_DrawToFinalImageRect);
						}

						RECT	l_rcSrc = {(int)l_pUIImage->GetOffsetPos()->x,
											(int)l_pUIImage->GetOffsetPos()->y,
											l_pUIImage->GetRightDownStripOffPos().x,
											l_pUIImage->GetRightDownStripOffPos().y};
						RECT	l_rcDrc = {StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos().x+l_pUIImage->GetOffsetPos()->x),
											StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos().y+l_pUIImage->GetOffsetPos()->y),
											l_rcDrc.left+l_rcSrc.right-l_rcSrc.left,
											l_rcDrc.top+l_rcSrc.bottom-l_rcSrc.top};
						if (l_pBitmapForSave)
						{
							CopyImageToImageBySpecificData(l_pBitmapForSave, l_pBitMap, l_rcSrc, l_rcDrc);
						}
					}
				    //write xml	data
				    //UV,LTRB
				    l_fUV[0] = (l_RenderPoint.x)/(float)l_iWidth;
				    l_fUV[1] = (l_RenderPoint.y)/(float)l_iHeight;
				    l_fUV[2] = (l_RenderPoint.x+l_ImageRealPixelSize.x)/(float)l_iWidth;
				    l_fUV[3] = (l_RenderPoint.y+l_ImageRealPixelSize.y)/(float)l_iHeight;
				    //offset
				    POINT	l_Offset = *l_pUIImage->GetOffsetPos();
					
					POINT	l_OriginaleSize = {l_pUIImage->GetOriginalImageSize().x,l_pUIImage->GetOriginalImageSize().y};
				    //POINT	l_OriginaleSize = {l_pBitmapForSave->Width,l_pBitmapForSave->Height};
				    //fwrite(l_pUIImage->GetPixels(),l_iPixelSize,1,l_pFopen);
				    l_XMLWriter.StartElement("PuzzleUnit");
					    //l_XMLWriter.AddAttribute("Size",l_iPixelSize);
					    l_XMLWriter.AddAttribute("Name",l_pUIImage->GetName());
					    std::string	l_strUVData = DNCT::GcStringToChar(
						    l_fUV[0].ToString()+","+
						    l_fUV[1].ToString()+","+
						    l_fUV[2].ToString()+","+
						    l_fUV[3].ToString());
					    l_XMLWriter.AddAttribute("UV",l_strUVData.c_str());
					    std::string	l_strOffsetPos = DNCT::GcStringToChar(
						    l_Offset.x.ToString()+","+
						    l_Offset.y.ToString());
					    l_XMLWriter.AddAttribute("OffsetPos",l_strOffsetPos.c_str());
					    //the result here may not as expect so I just set size as showImageSize???
					    std::string	l_strSize = DNCT::GcStringToChar(l_ImageRealPixelSize.x.ToString()+","+l_ImageRealPixelSize.y.ToString());
					    l_XMLWriter.AddAttribute("Size",l_strSize.c_str());
					    std::string	l_strOriginalSize = DNCT::GcStringToChar(l_OriginaleSize.x.ToString()+","+l_OriginaleSize.y.ToString());
					    l_XMLWriter.AddAttribute("OriginalSize",l_strOriginalSize.c_str());
						assert(l_OriginaleSize.x>=l_ImageRealPixelSize.x &&"did u change the .pi file manually ?");
						assert(l_OriginaleSize.y>=l_ImageRealPixelSize.y &&"did u change the .pi file manually ?");
					    RECT	l_rc = {l_RenderPoint.x,l_RenderPoint.y,l_RenderPoint.x+l_ImageRealPixelSize.x,+l_RenderPoint.y+l_ImageRealPixelSize.y};
					    POINT	l_ShowPosInPI = {l_RenderPoint.x,l_RenderPoint.y};
					    std::string	l_strShowPosInPI = DNCT::GcStringToChar(
						    l_rc.left.ToString()+","+
						    l_rc.top.ToString());
					    //char*	l_strShowPosInPI = DNCT::GcStringToChar(
					    //	l_rc.left.ToString()+","+

					    //	l_rc.top.ToString()+","+
					    //	l_rc.right.ToString()+","+
					    //	l_rc.bottom.ToString());
					    l_XMLWriter.AddAttribute("ShowPosInPI", l_strShowPosInPI);

						if (m_pPuzzleImageUnitTriangulatorManager && m_pPuzzleImageUnitTriangulatorManager->IsTriangulatorEdited(l_pUIImage))
						{
							auto l_pImageUnitTriangulator = m_pPuzzleImageUnitTriangulatorManager->GetObject(l_pUIImage);
							if (l_pImageUnitTriangulator->isEdited())
							{
								auto l_pPoints = l_pImageUnitTriangulator->GetPointsVector();
								if (l_pPoints->size())
								{
									l_XMLWriter.AddAttribute("TriangulatorPoints", ValueToString(*l_pPoints));
									int l_iLOD = l_pImageUnitTriangulator->GetLOD();
									if(l_iLOD > 1)
										l_XMLWriter.AddAttribute("TriangulatorPointsLOD",l_iLOD);
								}
							}
						}

					    l_ppPuzzleData[i] = new sPuzzleData((WCHAR*)l_pUIImage->GetName(),l_fUV,l_Offset,l_ImageRealPixelSize,l_OriginaleSize,l_ShowPosInPI);
					    //l_ppPuzzleData[i] = new sPuzzleData(l_pUIImage->GetName(),l_fUV,l_Offset,l_ImageRealPixelSize,l_OriginaleSize,l_rc);
					    //for debug info
				    //	l_strUVData = DNCT::GcStringToChar(
				    //		(l_RenderPoint.x+l_pUIImage->GetOffsetPos()->x).ToString()+","+
				    //		(l_RenderPoint.y+l_pUIImage->GetOffsetPos()->y).ToString()+","+
				    //		(l_RenderPoint.x+l_pUIImage->GetRightDownStripOffPos().x+1).ToString()+","+
				    //		(l_RenderPoint.y+l_pUIImage->GetRightDownStripOffPos().y+1).ToString()+","
				    //		);
				    l_XMLWriter.EndElement();
				    //l_XMLWriter.StartComment();
				    //l_XMLWriter.WriteString(l_strUVData);
				    //l_XMLWriter.EndComment();
			    }
			    for( int i=0;i<AllImage_listBox->Items->Count;++i )
			    {
				    cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_pImageListBox->Items[i]->ToString())));
					if( l_pUIImage )
					{
						if(l_pUIImage->m_pEditorAttachParent)
						{
							l_XMLWriter.StartElement("PuzzleUnitChild");
							l_XMLWriter.AddAttribute("Name",l_pUIImage->GetName());
							l_XMLWriter.AddAttribute("AttachParent",l_pUIImage->m_pEditorAttachParent->GetName());
							l_XMLWriter.AddAttribute("RelativePos",ValueToStringW(l_pUIImage->m_vEditorAttachParentRelativePos));
							l_XMLWriter.EndElement();
						}
					}
				}

				if( AnimationDataList_listBox->Items->Count )
				{
					l_XMLWriter.StartElement("AnimationData");
					    l_XMLWriter.AddAttribute("Count",AnimationDataList_listBox->Items->Count);
					    cImageIndexOfAnimation*l_pImageIndexOfAnimation = 0;
					    for(int i=0;i<m_pImageIndexOfAnimationList->Count();++i)
					    {
						    l_pImageIndexOfAnimation = m_pImageIndexOfAnimationList->GetObject(i);
						    l_XMLWriter.StartElement("AnimationDataUnit");
						    l_XMLWriter.AddAttribute("Name",l_pImageIndexOfAnimation->GetName());
						    l_XMLWriter.AddAttribute("Count",(int)l_pImageIndexOfAnimation->m_pNameList->size());
						    wstring	l_strImageNameList;
							string	l_strImageTimeList;
						    for(int j=0;j<(int)l_pImageIndexOfAnimation->m_pNameList->size();++j)
						    {
							    l_strImageNameList += (*l_pImageIndexOfAnimation->m_pNameList)[j].c_str();
							    l_strImageNameList += L",";
								l_strImageTimeList += ValueToString(l_pImageIndexOfAnimation->m_ImageAnimationDataList[j].fTimeGap);
								l_strImageTimeList += ",";
						    }
						    l_XMLWriter.AddAttribute("ImageList",l_strImageNameList.c_str());
							l_XMLWriter.AddAttribute("TimeList",l_strImageTimeList.c_str());
						    l_XMLWriter.EndElement();//end AnimationDataUnit
					    }
					l_XMLWriter.EndElement();//end AnimationData
				}
			l_XMLWriter.EndElement();//end puzzle image
			l_XMLWriter.Close();
			if( e_bBinary )
			{
				FileCompressHuffman(l_strXmlFileName.c_str(),l_strXmlFileName.c_str());
			}
//			FileToUnicode(l_strXMLFileName);
			if(this->ImageSaveAsDDS_checkBox->Checked)
			{
				l_pBitMap->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
				ILuint uiID;
				ilGenImages( 1, &uiID );
				ilBindImage( uiID );
				System::Drawing::Rectangle l_Rect; l_Rect.X = 0; l_Rect.Y = 0; l_Rect.Width = l_pBitMap->Width; l_Rect.Height = l_pBitMap->Height;
				System::Drawing::Imaging::BitmapData^ l_pBd = l_pBitMap->LockBits(l_Rect,System::Drawing::Imaging::ImageLockMode::ReadOnly,System::Drawing::Imaging::PixelFormat::Format32bppArgb);
				ilTexImage( l_pBitMap->Width, l_pBitMap->Height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, (void*)(l_pBd->Scan0));
				ilSetInteger(IL_DXTC_FORMAT,  IL_DXT5);
				ilEnable(IL_FILE_OVERWRITE);
				if(!ilSaveImage( (WCHAR*)DNCT::GcStringToChar(l_FileName).c_str() ))
				{
					ILenum   l_ErrorIF = ilGetError();
					int a=0;
					UT::ErrorMsg(L"failed to save dds file",L"fuck");
				}
				ilDeleteImages(1, &uiID); // Because we have already copied image data into texture data
				l_pBitMap->UnlockBits(l_pBd);
				l_pBitMap->RotateFlip(System::Drawing::RotateFlipType::RotateNoneFlipY);
			}
			else
			{
				if( e_bBinary )
				{
					//l_FileName = l_FileName->Substring(0,l_FileName->Length-1);
					std::string	l_strTargetFileName = DNCT::GcStringToChar(l_FileName);
					FILE*l_pFile = fopen(l_strTargetFileName.c_str(),"rb");
					int	l_iLength = UT::GetFileSize(l_pFile);
					unsigned char*l_pData = new unsigned char[l_iLength];
					int	l_iRead = (int)fread(l_pData,1,l_iLength,l_pFile);
					l_strTargetFileName += "b";
					CompressHuffmanToFile(l_pData,l_iLength,l_strTargetFileName.c_str(),HUFFMAN_MAGIC_ID);
					delete l_pData;
					fclose(l_pFile);
					//std::string	l_strOutputFileName = l_strTargetFileName;
					//l_strOutputFileName += "b";
					//FileCompressHuffman(l_strTargetFileName.c_str(),l_strOutputFileName.c_str(),HUFFMAN_MAGIC_ID);
				}
				else
				{
					l_pBitMap->Save(l_FileName);
					//delete l_pBitMap;
				}
			}
			if( l_ppPuzzleData )
			{
				//for( int i=0;i<l_pImageListBox->Items->Count;++i )
				//	SAFE_DELETE(l_ppPuzzleData[i]);
				//SAFE_DELETE_ARRAY(l_ppPuzzleData);
			}
		}	
	}
//=======================
//
//=======================
	System::Void	cPIEditor::MouseCollideForPickUpObject( System::Windows::Forms::MouseEventArgs^  e,System::Windows::Forms::Panel^e_pPanel)
	{
		//while mouse is not click and collide object t same time,we do not dectect collide,until mouse left button up
		static bool		l_sbWaitForMouseButtonNone = false;
		//while mouse collie object,store the mouse position,so we could compare mouse move position,and setup new position to object
		static POINT	l_sMousePos;
		//mouse left button click and collide the object
		static bool		l_sbCollide = false;
		//which one is collided
		static int		l_siCollideIndex = -1;
		//GetCursorPos( &ptCursor );
		//ScreenToClient( (HWND)e_pPanel->Handle.ToPointer(), &ptCursor );
		POINT	ptCursor = {(int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y};
		//MousePos_label->Text = ptCursor.x.ToString()+","+ptCursor.y.ToString();


		if(GCFORM::MouseButtons::Left == e->Button )
		{
			if( !l_sbCollide )
			{
				for( int i=m_pImageomposerIRM->Count()-1;i!=-1;--i )
				{
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					RECT	l_rc = {
						(int)l_pUIImage->GetPos().x+l_pUIImage->GetOffsetPos()->x,
						(int)l_pUIImage->GetPos().y+l_pUIImage->GetOffsetPos()->y,
						(int)(l_rc.left+l_pUIImage->GetWidth()),
						(int)(l_rc.top+l_pUIImage->GetHeight())};
					//if(UT::RectCollidePoint(l_rc,ptCursor))
					{
						int	l_iLocalX = ptCursor.x-l_rc.left;
						int	l_iLocalY = ptCursor.y-l_rc.top;
						if( !l_pUIImage->m_pEditorAttachParent )
						if(l_pUIImage->CollideWithOutAlphaTest(l_iLocalX,l_iLocalY))
						{
							l_sbCollide = true;
							l_sMousePos = ptCursor;
							l_siCollideIndex = i;
							this->AllImage_listBox->SelectedIndex = i;
							m_pOrthogonalCamera->SetDrawSelectFrame(false);
							m_iCurrentSelectedObjectIndex = i;
							break;
						}
					}
				}
				if( !l_sbCollide )
				{
					m_pOrthogonalCamera->SetDrawSelectFrame(true);
					l_sbWaitForMouseButtonNone = true;
					this->AllImage_listBox->SelectedIndex = -1;
					m_iCurrentSelectedObjectIndex = -1;
				}
				m_pOrthogonalCamera->SetDrawSelectFrame(true);
			}
			else
			{
				if( !l_sbWaitForMouseButtonNone )
				{
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_siCollideIndex));
					POINT	l_iMovePosition = { ptCursor.x- l_sMousePos.x, ptCursor.y- l_sMousePos.y };
					Vector3	l_vImagePos = l_pUIImage->GetPos();
					POINT	l_ImagePos = {(int)l_vImagePos.x,(int)l_vImagePos.y};
					l_ImagePos.x+=l_iMovePosition.x;
					l_ImagePos.y+=l_iMovePosition.y;
					l_pUIImage->SetPos(l_ImagePos);
					l_sMousePos = ptCursor;
					m_pOrthogonalCamera->SetDrawSelectFrame(false);
					int	l_iTotalPixel = 0;
					for( int i=0;i<m_pImageomposerIRM->Count();++i )
					{
						cUIImage*l_p = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[i]);
						//+1 for offset start at 0,0
						int	l_iWidth = (l_p->GetRightDownStripOffPos().x-l_p->GetOffsetPos()->x+1);
						int	l_iHeight =(l_p->GetRightDownStripOffPos().y-l_p->GetOffsetPos()->y+1);
						l_iWidth = power_of_two(l_iWidth);
						l_iHeight = power_of_two(l_iHeight);
						l_iTotalPixel += l_iWidth*l_iHeight;
					}
					//+1 for offset start at 0,0
					POINT	l_ImageSize = {l_pUIImage->GetRightDownStripOffPos().x-l_pUIImage->GetOffsetPos()->x+1,
						l_pUIImage->GetRightDownStripOffPos().y-l_pUIImage->GetOffsetPos()->y+1};
					ImageDetail_textBox->Text = "X:"+l_ImagePos.x.ToString()+"Y:"+l_ImagePos.y.ToString()+DNCT::GetChanglineString()+
						"Size:"+l_ImageSize.x.ToString()+","+l_ImageSize.y.ToString()+DNCT::GetChanglineString()+
						"OffsetPos:"+l_pUIImage->GetOffsetPos()->x.ToString()+","+l_pUIImage->GetOffsetPos()->y.ToString()+
						GetTotalPixelExisted();
					AttachObjectPosAdjust();
				}
			}
		}
		else
		{
			if( GCFORM::MouseButtons::None == e->Button )
			{
				if( l_siCollideIndex!=-1 )
				{
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_siCollideIndex));
					//m_pParticlePropertyGrid->Position = Point(l_pUIImage->GetPos().x,l_pUIImage->GetPos().y);
					//m_pParticlePropertyGrid->Name = WcharToGcstring(l_pUIImage->GetName());
					//m_pParticlePropertyGrid->Size = System::Drawing::Size(l_pUIImage->GetWidth(),l_pUIImage->GetHeight());
					//m_pParticlePropertyGrid->Angle = l_pUIImage->GetAngle();
					//m_pPropertyGrid->SelectedObject = m_pParticlePropertyGrid;
				}
				l_sbWaitForMouseButtonNone = false;
			}
			l_sbCollide = false;
			l_siCollideIndex = -1;
		}
	}

	String^	cPIEditor::GetTotalPixelExisted()
	{
		int	l_iTotalPixel = 0;
		for( int i=0;i<m_pImageomposerIRM->Count();++i )
		{
			cUIImage*l_p = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[i]);
			//+1 for offset start at 0,0
			int	l_iWidth = (l_p->GetRightDownStripOffPos().x-l_p->GetOffsetPos()->x+1);
			int	l_iHeight =(l_p->GetRightDownStripOffPos().y-l_p->GetOffsetPos()->y+1);
			l_iWidth = power_of_two(l_iWidth);
			l_iHeight = power_of_two(l_iHeight);
			l_iTotalPixel += l_iWidth*l_iHeight;
		}
		return "Total Pixel: "+l_iTotalPixel.ToString();	
	}

	System::Void	cPIEditor::AssignAnimationData()
	{
		if(this->AnimationDataList_listBox->SelectedIndex != -1)
		{
		     m_pImageIndexOfAnimation->Clear();
			 AnimationData_listBox->Items->Clear();
			 cImageIndexOfAnimation*l_pImageIndexOfAnimation = m_pImageIndexOfAnimationList->GetObject(DNCT::GcStringToWchar(this->AnimationDataList_listBox->SelectedItem->ToString()));
			 for(int i=0;i<(int)l_pImageIndexOfAnimation->m_pNameList->size();++i)
			 {
				 WCHAR*l_strName = (WCHAR*)(*l_pImageIndexOfAnimation->m_pNameList)[i].c_str();
				 AnimationData_listBox->Items->Add(DNCT::WcharToGcstring(l_strName));
				 int	l_iIndex = AllImage_listBox->Items->IndexOf(DNCT::WcharToGcstring(l_strName));
				 m_pImageIndexOfAnimation->AddNameObject((WCHAR*)(*l_pImageIndexOfAnimation->m_pNameList)[i].c_str(),l_iIndex,0.1f);
			 }
		}
	}

	GCFORM::Form^CallForm(String^e_strFileName)
	{
		GCFORM::Form^l_pForm = gcnew GCFORM::Form();
#ifdef	USER_CONTROL_ENABLE
		GCFORM::TabControl^l_ptabControl;
		gcnew PI::cPIEditor(l_ptabControl,l_pForm,e_strFileName);
#else
		gcnew PI::cPIEditor(e_strFileName);
#endif
		return l_pForm;
	}
//end namespace
}

#ifndef USER_CONTROL_ENABLE

[STAThreadAttribute]
int main(cli::array<System::String ^> ^args)
{
	////https://github.com/greenm01/poly2tri
	//vector<p2t::Point*>*polyline = new vector<p2t::Point*>;
	//polyline->push_back(new p2t::Point(0, 0));
	//polyline->push_back(new p2t::Point(10, 0));
	//polyline->push_back(new p2t::Point(0, 10));
	//polyline->push_back(new p2t::Point(10, 10));

	//CDT* cdt = new CDT(*polyline);
	//cdt->Triangulate();
	////auto triangles = cdt->GetTriangles();
	//delete cdt;
	//DELETE_POINTER_VECTOR(polyline, p2t::Point*);
	//std::vector<Vector2>l_InputVector;
	//l_InputVector.push_back(Vector2(0, 0));
	//l_InputVector.push_back(Vector2(10, 0));
	//l_InputVector.push_back(Vector2(0, 10));
	//l_InputVector.push_back(Vector2(10, 10));
	//std::vector<size_t>l_Reult =  GetIndicesFromPolygon(l_InputVector);

	//SaveRomFile();
	ilInit();
	alutInit (0,0);
	// Enabling Windows XP visual effects before any controls are created
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	// Create the main window and run it
	if( args->Length == 0 )
		Application::Run(gcnew PI::cPIEditor(""));
	else
	{
		Application::Run(gcnew PI::cPIEditor(args[0]));
	}
	alutExit();
	ilShutDown();
	NamedTypedObject::DumpUnReleaseInfo();
#ifdef DEBUG
	_CrtDumpMemoryLeaks();
#endif
	return 0;
}

#endif