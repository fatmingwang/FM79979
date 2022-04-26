// PI.cpp : main project file.

#include "stdafx.h"
#include "PIForm.h"

#include "../../../Include/IL/il.h"
#include "../../AllLibInclude.h"
#pragma comment(lib, "../../../lib/Devil.lib")
#include "../../Core/Common/BinaryFile/StringCompress.h"
#include "MorphingAnimation_Editor.h"
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
		m_ImageTale->Clear();
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
					auto l_iUIImageIndex = e_pPuzzleImage->GetObjectIndexByName(l_pUIImage->GetName());
					std::vector<Vector3>*l_pPointsVector = e_pPuzzleImage->GetImageShapePointVector(l_iUIImageIndex);
					//if (l_pPointsVector && l_pPointsVector->size())
					{
						m_pPuzzleImageUnitTriangulatorManager->AssignDataFromPuzzleImage(e_pPuzzleImage, l_pUIImage);
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
			 cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_pPuzzleUnitChild->GetName()));
			 cUIImage*l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_pPuzzleUnitChild->m_strAttachParentName.c_str()));
			 l_pUIImage->m_pEditorAttachParent = l_pAttachUIImage;
			 l_pUIImage->m_vEditorAttachParentRelativePos = l_pPuzzleUnitChild->m_vRelativePos;
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
					auto l_strCharFileName = DNCT::GcStringToChar(l_strFileName);
					UT::SaveTxtToFile(LAST_USE_PI_FILE_NAME, l_strCharFileName.c_str(), (int)l_strCharFileName.length());
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
						auto l_strMorphingFileName = DNCT::GcStringToChar(l_strFileName);
						l_strMorphingFileName = ChangeFileExtensionName(l_strMorphingFileName.c_str(), FM_MORPHING_XML_FILE_EXTENSION_NAME);
						if (m_pPuzzleImageUnitTriangulatorManager->ParseMorphingAnimation(l_strMorphingFileName.c_str()))
						{
						}
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
				WARNING_MSG("PuzzleImageUnitTriangulatorManager:objects are overlap are you sure you want to save file!?");
			}
		}
		 String^l_ImageFileName = e_strFileName;
		 if( e_bBinary )
			 l_ImageFileName += "b";
		 String^l_strXMLFileName = l_ImageFileName;
		 if(l_ImageFileName)
		 {
			if( !AllImage_listBox->Items->Count )
				return;
			cBinaryFile*l_pTrianglesBinaryData = new cBinaryFile();
			for(int i= l_ImageFileName->Length-1;i!=-1;--i)
			{
				if(l_ImageFileName[i] == L'.' )
				{
					if(this->ImageSaveAsDDS_checkBox->Checked)
						l_ImageFileName = gcnew String(l_ImageFileName->Substring(0,i+1)+"dds");
					else
					{
						l_ImageFileName = gcnew String(l_ImageFileName->Substring(0,i+1)+"png");
					}
					break;
				}
			}
			auto l_strTrianglesBinaryFileName = DNCT::GcStringToChar(l_strXMLFileName);
			l_strTrianglesBinaryFileName = UT::ChangeFileExtensionName(l_strTrianglesBinaryFileName.c_str(), TRIANGLE_AND_DRAW_INDEX_EXTENSION_FILE_NAME);
			l_pTrianglesBinaryData->Writefile(l_strTrianglesBinaryFileName.c_str(),false,false,"wb");
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
			float	l_fUV[4];
			std::string	l_strPIXMLFileName = DNCT::GcStringToChar(l_strXMLFileName);
			String^l_strUserNameAndData = GetUseerNameAndTime();
			std::string	l_strXmlFileName = l_strPIXMLFileName;
			ATG::XMLWriter	l_XMLWriter(l_strPIXMLFileName.c_str());
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
				std::string l_strPI_tri = UT::ChangeFileExtensionName(l_strTrianglesBinaryFileName.c_str(), TRIANGLE_AND_DRAW_INDEX_EXTENSION_FILE_NAME);
				l_strPI_tri = UT::GetFileNameWithoutFullPath(l_strPI_tri, false);
				std::string l_strPI_morphing = UT::ChangeFileExtensionName(l_strTrianglesBinaryFileName.c_str(), FM_MORPHING_XML_FILE_EXTENSION_NAME);
				l_strPI_morphing = UT::GetFileNameWithoutFullPath(l_strPI_morphing, false);
				l_XMLWriter.AddAttribute(TI_ELEMENT_NAME, l_strPI_tri.c_str());
				l_XMLWriter.AddAttribute(MX_ELEMENT_NAME, l_strPI_morphing.c_str());
			    l_XMLWriter.AddAttribute("OriginalNameSort",l_OriginalImageNameOrder.c_str());
				if(this->ImageSaveAsDDS_checkBox->Checked)
					l_XMLWriter.AddAttribute("ImageName",DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_ImageFileName,true)+".dds"));
				else
				{
					if( e_bBinary )
					{
						l_XMLWriter.AddAttribute("ImageName",DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_ImageFileName,true)+".pngb"));
					}
					else
					{
						l_XMLWriter.AddAttribute("ImageName", DNCT::GcStringToChar(DNCT::GetFileNameWithoutFullPath(l_ImageFileName, true) + ".png"));
					}
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
				    l_XMLWriter.StartElement("PuzzleUnit");
					{
						//l_XMLWriter.AddAttribute("Size",l_iPixelSize);
						l_XMLWriter.AddAttribute("Name", l_pUIImage->GetName());
						std::string	l_strUVData = DNCT::GcStringToChar(
							l_fUV[0].ToString() + "," +
							l_fUV[1].ToString() + "," +
							l_fUV[2].ToString() + "," +
							l_fUV[3].ToString());
						l_XMLWriter.AddAttribute("UV", l_strUVData.c_str());
						std::string	l_strOffsetPos = DNCT::GcStringToChar(
							l_Offset.x.ToString() + "," +
							l_Offset.y.ToString());
						l_XMLWriter.AddAttribute("OffsetPos", l_strOffsetPos.c_str());
						//the result here may not as expect so I just set size as showImageSize???
						std::string	l_strSize = DNCT::GcStringToChar(l_ImageRealPixelSize.x.ToString() + "," + l_ImageRealPixelSize.y.ToString());
						l_XMLWriter.AddAttribute("Size", l_strSize.c_str());
						std::string	l_strOriginalSize = DNCT::GcStringToChar(l_OriginaleSize.x.ToString() + "," + l_OriginaleSize.y.ToString());
						l_XMLWriter.AddAttribute("OriginalSize", l_strOriginalSize.c_str());
						assert(l_OriginaleSize.x >= l_ImageRealPixelSize.x &&"did u change the .pi file manually ?");
						assert(l_OriginaleSize.y >= l_ImageRealPixelSize.y &&"did u change the .pi file manually ?");
						RECT	l_rc = { l_RenderPoint.x,l_RenderPoint.y,l_RenderPoint.x + l_ImageRealPixelSize.x,+l_RenderPoint.y + l_ImageRealPixelSize.y };
						POINT	l_ShowPosInPI = { l_RenderPoint.x,l_RenderPoint.y };
						std::string	l_strShowPosInPI = DNCT::GcStringToChar(
							l_rc.left.ToString() + "," +
							l_rc.top.ToString());
						//char*	l_strShowPosInPI = DNCT::GcStringToChar(
						//	l_rc.left.ToString()+","+

						//	l_rc.top.ToString()+","+
						//	l_rc.right.ToString()+","+
						//	l_rc.bottom.ToString());
						l_XMLWriter.AddAttribute("ShowPosInPI", l_strShowPosInPI);

						//if (m_pPuzzleImageUnitTriangulatorManager && m_pPuzzleImageUnitTriangulatorManager->IsTriangulatorEdited(l_pUIImage))
						if (m_pPuzzleImageUnitTriangulatorManager)
						{
							auto l_pImageUnitTriangulator = m_pPuzzleImageUnitTriangulatorManager->GetObject(l_pUIImage);
							//if (l_pImageUnitTriangulator->IsEdited())
							{
								auto l_pPoints = l_pImageUnitTriangulator->GetPointsVector();
								if (l_pPoints->size())
								{
									l_XMLWriter.AddAttribute("TriangulatorPoints", ValueToString(*l_pPoints));
									int l_iLOD = l_pImageUnitTriangulator->GetLOD();
									if (l_iLOD > 1)
										l_XMLWriter.AddAttribute("TriangulatorPointsLOD", l_iLOD);
									auto l_vCenterOffset = l_pImageUnitTriangulator->GetCenterOffset();
									if(l_vCenterOffset.x != 0.f || l_vCenterOffset.y != 0.f)
										l_XMLWriter.AddAttribute("TriangulatorPointsCenterOffset", ValueToString(l_vCenterOffset).c_str());
								}
							}
							std::vector<int>l_iIndexBufferVector;
							std::vector<Vector3>l_PosVector;
							std::vector<Vector2>l_UVVector;
							l_pImageUnitTriangulator->ToTixmlElementWithBinaryData(&l_XMLWriter, Vector2(l_iWidth, l_iHeight),
								Vector2(l_ImageRealPixelSize.x, l_ImageRealPixelSize.y), Vector2(l_RenderPoint.x, l_RenderPoint.y),
								l_PosVector, l_UVVector, l_iIndexBufferVector);
							if (l_pTrianglesBinaryData)
							{
								//index,pos,uv
								l_pTrianglesBinaryData->WriteToFile((const char*)&l_iIndexBufferVector[0], sizeof(int)*l_iIndexBufferVector.size());
								l_pTrianglesBinaryData->WriteToFile((const char*)&l_PosVector[0], sizeof(Vector3)*l_PosVector.size());								
								l_pTrianglesBinaryData->WriteToFile((const char*)&l_UVVector[0], sizeof(Vector2)*l_UVVector.size());
							}
						}

						l_ppPuzzleData[i] = new sPuzzleData((WCHAR*)l_pUIImage->GetName(), l_fUV, l_Offset, l_ImageRealPixelSize, l_OriginaleSize, l_ShowPosInPI);
						//l_ppPuzzleData[i] = new sPuzzleData(l_pUIImage->GetName(),l_fUV,l_Offset,l_ImageRealPixelSize,l_OriginaleSize,l_rc);
						//for debug info
					//	l_strUVData = DNCT::GcStringToChar(
					//		(l_RenderPoint.x+l_pUIImage->GetOffsetPos()->x).ToString()+","+
					//		(l_RenderPoint.y+l_pUIImage->GetOffsetPos()->y).ToString()+","+
					//		(l_RenderPoint.x+l_pUIImage->GetRightDownStripOffPos().x+1).ToString()+","+
					//		(l_RenderPoint.y+l_pUIImage->GetRightDownStripOffPos().y+1).ToString()+","
					//		);
					}
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
				if(!ilSaveImage( (WCHAR*)DNCT::GcStringToChar(l_ImageFileName).c_str() ))
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
					//l_ImageFileName = l_ImageFileName->Substring(0,l_ImageFileName->Length-1);
					std::string	l_strTargetFileName = DNCT::GcStringToChar(l_ImageFileName);
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
					l_pBitMap->Save(l_ImageFileName);
					delete l_pBitMap;
				}
			}
			if( l_ppPuzzleData )
			{
				for( int i=0;i<l_pImageListBox->Items->Count;++i )
					SAFE_DELETE(l_ppPuzzleData[i]);
				SAFE_DELETE_ARRAY(l_ppPuzzleData);
			}
			auto l_strMorpginhXMLFileNameTest = ChangeFileExtensionName(l_strPIXMLFileName.c_str(), FM_MORPHING_XML_FILE_EXTENSION_NAME);
			if (m_pPuzzleImageUnitTriangulatorManager)
			{				
				this->m_pPuzzleImageUnitTriangulatorManager->ExportMorphingAnimation(l_strMorpginhXMLFileNameTest.c_str(), l_strPI_tri.c_str(), DNCT::GcStringToChar(l_ImageFileName).c_str());
			}
			SAFE_DELETE(l_pTrianglesBinaryData);
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

	int		cPIEditor::GetMouseWheelDelta(System::Windows::Forms::MouseEventArgs^  e)
	{
		auto l_iDelte = e->Delta;
		if (GCFORM::Control::ModifierKeys == Keys::Control)
		{
			l_iDelte *= 10;
		}
		return l_iDelte;
	}

	System::Void cPIEditor::AllImage_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (AllImage_listBox->SelectedIndex != -1)
		{
			if (e->KeyCode == Keys::Delete)
			{
				if (m_pImageomposerIRM->Count())
				{
					m_pImageomposerIRM->RemoveObject(AllImage_listBox->SelectedIndex);
					m_ImageTale->Remove(AllImage_listBox->SelectedItem->ToString());
					AllImage_listBox->Items->RemoveAt(AllImage_listBox->SelectedIndex);
					ImageDetail_textBox->Text = GetTotalPixelExisted();
				}
			}
		}
	}
	System::Void cPIEditor::DeleteImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (sender == DeleteImage_button)
		{
			this->timer1->Enabled = false;
			std::list<int>	l_iErseIndexList;
			System::Collections::ArrayList^	l_MyList = gcnew System::Collections::ArrayList;
			l_MyList->AddRange(AllImage_listBox->SelectedItems);
			for each (System::Object^l_pObject in l_MyList)
				l_iErseIndexList.push_back(AllImage_listBox->Items->IndexOf(l_pObject));
			if (l_MyList->Count)
				m_iCurrentSelectedObjectIndex = -1;
			for each (System::Object^l_pObject in l_MyList)
			{
				int	l_iIndex = AllImage_listBox->Items->IndexOf(l_pObject);
				AllImage_listBox->Items->Remove(l_pObject);
				NamedTypedObject*l_pNamedTypedObject = m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_pObject->ToString()));
				if (l_pNamedTypedObject->GetOwner())
					m_ImageTale->Remove(l_pObject->ToString());

				for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
				{
					cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					if (l_pUIImage2->m_pEditorAttachParent == l_pNamedTypedObject)
					{
						int	l_iIndex = AllImage_listBox->Items->IndexOf(DNCT::WcharToGcstring(l_pUIImage2->GetName()));
						AllImage_listBox->Items->RemoveAt(l_iIndex);
						m_pImageomposerIRM->RemoveObject(l_pUIImage2->GetName());
						--i;
					}
				}
				if (m_pPuzzleImageUnitTriangulatorManager)
					m_pPuzzleImageUnitTriangulatorManager->RemoveObject(dynamic_cast<cUIImage*>(l_pNamedTypedObject));
				m_pImageomposerIRM->RemoveObject(DNCT::GcStringToWchar(l_pObject->ToString()));
			}
			if (m_pImageomposerIRM->Count() != AllImage_listBox->Items->Count)
				UT::ErrorMsg(L"delete image error occur!!!", L"call fatming");
			this->timer1->Enabled = true;
		}
		else
		{
			cli::array<String^>^l_pImageNameList;
			if (addImagesByFolderToolStripMenuItem->Checked)
			{
				System::String^l_strDirectoryName = DNCT::SelectDirectory();
				if (l_strDirectoryName)
				{
					System::Collections::ArrayList^l_pArrayList = gcnew System::Collections::ArrayList;
					GetFilesNameByRecursivelyDirectory(l_strDirectoryName, l_pArrayList, ".png");
					if (l_pArrayList->Count)
					{
						l_pImageNameList = gcnew cli::array<String^>(l_pArrayList->Count);
						l_pArrayList->CopyTo(l_pImageNameList);
					}
				}
			}
			else
			{
				l_pImageNameList = DNCT::OpenFileAndGetNames("image Files(*.png;*.bmp;*.jpg;*.dds)|*.png;*.bmp;*.jpg;*.dds|All files (*.*)|*.*");
			}
			if (!l_pImageNameList)
				return;
			this->timer1->Enabled = false;
			for each(String^l_strFileName in l_pImageNameList)
			{
				String^l_strFileNameWithoutDirectory = DNCT::GetFileNameWithoutFullPath(l_strFileName, true);
				bool	l_bAlpha = false;
				if (l_strFileName->Contains(".png") || l_strFileName->Contains(".PNG"))
					l_bAlpha = true;
				int l_iOriginalIndex = -1;
				if (DNCT::CheckListContainStringAndAdd(AllImage_listBox, l_strFileNameWithoutDirectory))
				{
					WARING_YES_NO_TO_NO(l_strFileNameWithoutDirectory + "object name already exists,would u like to replace?" + DNCT::GetChanglineString() + "該物件名稱已存在,要覆蓋嗎?")
						continue;
					//remove old data
					m_ImageTale->Remove(l_strFileNameWithoutDirectory);
					l_iOriginalIndex = m_pImageomposerIRM->GetObjectIndexByName(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory).c_str());
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(DNCT::GcStringToWchar(l_strFileNameWithoutDirectory)));
					SAFE_DELETE(l_pUIImage);
				}
				try
				{
					//System::Diagnostics::Stopwatch^ sw = gcnew System::Diagnostics::Stopwatch();
					//sw->Start();
					System::Drawing::Bitmap^l_pImage;
					cUIImage*l_pUIImage = 0;
					bool	l_bUsingDDS = false;
					if (l_bUsingDDS)
					{
						l_pImage = DNCT::OpenImageFile(l_strFileName, &l_pUIImage);
					}
					else
					{
						l_pImage = DNCT::OpenImageFile(l_strFileName);
					}



					m_ImageTale[l_strFileNameWithoutDirectory] = l_pImage;
					std::wstring l_strWcharFileName = DNCT::GcStringToWchar(l_strFileName);
					std::wstring l_strwstringFileName = UT::GetFileNameWithoutFullPath(l_strWcharFileName.c_str());
					if (!l_pUIImage)
						l_pUIImage = GetNewUIImageByBitMap(l_pImage, l_strwstringFileName.c_str());

					//POINT	l_WidthHeight = { *l_pUIImage->GetWidth()-1,*l_pUIImage->GetHeight()-1 };
					//l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
					//sw->Stop();
					if (l_iOriginalIndex != -1)
						(*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
					else
						m_pImageomposerIRM->AddObject(l_pUIImage);
				}
				catch (System::Exception^l_pException)
				{
					WARNING_MSG("doesn't support such file" + l_strFileName + DNCT::GetChanglineString() + l_pException->ToString());
					AllImage_listBox->Items->Remove(l_strFileNameWithoutDirectory);
				}
			}
			this->timer1->Enabled = true;
		}
		ImageDetail_textBox->Text = GetTotalPixelExisted();
	}
	System::Void cPIEditor::timer1_Tick(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->menuStrip1->Visible == false)
			this->menuStrip1->Visible = true;
		if (!this->Visible)
		{
			::Threading::Thread::Sleep(100);
		}
		//wglMakeCurrent(nullptr,nullptr);
		wglMakeCurrent(m_HdcMV, m_HGLRCMV);
		static	float	l_sfTime = 0;
		l_sfTime += 0.01f;
		if (l_sfTime > 1.f)
			l_sfTime = 0.f;
		DEFAULT_SHADER = L"ICPathShader";
		NO_TEXTURE_SHADER = L"IC_NoTextureShader";
		DRAW_POINTS_SHADER = L"IC_DrawPointsShader";
		UseShaderProgram();
		m_pTimeAndFPS->Update();
		float	l_fElpaseTime = m_pTimeAndFPS->fElpaseTime;
		glViewport(0, 0, splitContainer2->Panel1->Width, splitContainer2->Panel1->Height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(m_pvBGColor->x, m_pvBGColor->y, m_pvBGColor->z, m_pvBGColor->w);
		glClearDepth(1.0f);									// Depth Buffer Setup
		cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer2->Panel1->Width;
		cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer2->Panel1->Height;
		cGameApp::ApplyViewPort();
		if (tabControl1->SelectedIndex == 2)
		{
			m_pOrthogonalCameraForTrianhulatorPIUnit->Render();
			m_pOrthogonalCameraForTrianhulatorPIUnit->DrawGrid(0, 0, Vector4(0.3f, 0.7f, 0.3f, 0.7f));
			if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
			{
				if(!EditAnimation_checkBox->Checked)
				{
					m_pCurrentSelectedPuzzleImageUnitTriangulator->Render();
				}
				else
				{
					if (PlayMorphingAnimation_checkBox->Checked)
					{
						int l_iNextStep = (int)(l_fElpaseTime * 1000);
						if (MorphingAnimation_trackBar->Value == MorphingAnimation_trackBar->Maximum)
						{
							MorphingAnimation_trackBar->Value = 0;
						}
						if (l_iNextStep + MorphingAnimation_trackBar->Value > MorphingAnimation_trackBar->Maximum)
						{
							MorphingAnimation_trackBar->Value = MorphingAnimation_trackBar->Maximum;
						}
						else
						{
							MorphingAnimation_trackBar->Value += l_iNextStep;
						}
						int l_iValue = this->MorphingAnimation_trackBar->Value;
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditUpdateanimationByBlobalTime(l_iValue / 1000.f);
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditRender();
						MorphintTime_label->Text = MorphingAnimation_trackBar->Value.ToString() + "/" + MorphingAnimation_trackBar->Maximum.ToString();
					}
					else
					{
						if (MorphingAnimationTime_listBox->SelectedIndex == -1)
						{
							m_pCurrentSelectedPuzzleImageUnitTriangulator->Render();
						}
						else
						{
							std::vector<int>	l_iTimeVector;
							for (int i = 0; i < this->MorphingAnimationTime_listBox->SelectedIndices->Count; ++i)
							{
								l_iTimeVector.push_back(MorphingAnimationTime_listBox->SelectedIndices[i]);
							}
							m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditRenderByTimeVectorIndex(l_iTimeVector);
						}
					}
				}
			}
		}
		else
		{
			m_pOrthogonalCamera->Render();
			m_pOrthogonalCamera->DrawGrid(0, 0, Vector4(0.3f, 0.7f, 0.3f, 0.7f));
			glEnable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			//glEnable(GL_TEXTURE_2D);
			for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
			{
				cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
				if (!l_p->m_pEditorAttachParent)
					l_p->Render();
			}
			glDisable(GL_TEXTURE_2D);
			int	l_iRectWidth = 0;
			int	l_iRectHeight = 0;
			if (AllShowBoundry_checkBox->Checked)
			{
				int	l_iNum = m_pImageomposerIRM->Count();
				for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
				{
					cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					//Vector4	l_vColor(0.5, 0.5, 0.5, 1.0);
					Vector4	l_vColor(1.5, 1.5, 1.5, 1.0);
					//Vector4	l_vColor(0, 0, 0, 1);
					//l_vColor.x = 1.f / ((i % 3) + 1);
					//l_vColor.y = 0.5f;
					//l_vColor.z = 1.f / ((l_iNum % (i + 1)) + 1);
					//l_vColor.w = 1;
					Vector3	l_vPos = l_p->GetPos();
					//+1 for offset start at 0,0
					l_iRectWidth = l_p->GetRightDownStripOffPos().x - l_p->GetOffsetPos()->x + 1;
					l_iRectHeight = l_p->GetRightDownStripOffPos().y - l_p->GetOffsetPos()->y + 1;
					l_vPos.x += l_p->GetOffsetPos()->x;
					l_vPos.y += l_p->GetOffsetPos()->y;
					POINT	l_Pos = { (int)l_vPos.x,(int)l_vPos.y };
					//+1 for 0,0 start offset
					RenderRectangle(l_Pos, l_iRectWidth, l_iRectHeight, l_vColor, 0, 1);
					//l_p->DebugRender();
				}
			}
			if (ShowTriangulaotrPoints_checkBox->Checked && m_pPuzzleImageUnitTriangulatorManager)
			{
				this->m_pPuzzleImageUnitTriangulatorManager->RenderPointsShapeLine();
			}
			if (AllImage_listBox->Items->Count&&m_iCurrentSelectedObjectIndex != -1)
			{
				if (ShowBoundary_checkBox->Checked)
				{
					static Vector4 l_vColor(0, 0, 0, 1);
					l_vColor.x = l_sfTime;
					l_vColor.y -= l_sfTime;
					l_vColor.z = l_sfTime;
					l_vColor.w = l_sfTime;
					if (l_vColor.x >= 1.f)
						l_vColor.x = 1.f;
					if (l_vColor.y <= 0)
						l_vColor.y = 0;
					if (m_iCurrentSelectedObjectIndex >= m_pImageomposerIRM->Count())
						m_iCurrentSelectedObjectIndex = m_pImageomposerIRM->Count() - 1;
					if (m_pImageomposerIRM->Count() > -1)
					{
						cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
						Vector4	l_vOriginalColor = *l_p->GetColor();
						l_p->SetColor(l_vColor);
						l_p->Render();
						l_p->SetColor(l_vOriginalColor);
						Vector3	l_vPos = l_p->GetPos();
						//+1 for offset start at 0,0
						l_iRectWidth = l_p->GetRightDownStripOffPos().x - l_p->GetOffsetPos()->x + 1;
						l_iRectHeight = l_p->GetRightDownStripOffPos().y - l_p->GetOffsetPos()->y + 1;
						l_vPos.x += l_p->GetOffsetPos()->x;
						l_vPos.y += l_p->GetOffsetPos()->y;
						POINT	l_Pos = { (int)l_vPos.x,(int)l_vPos.y };
						//+1 for 0,0 start offset
						RenderRectangle(l_Pos, l_iRectWidth, l_iRectHeight, l_vColor, 0, 1);
					}
				}
			}
			if (tabControl1->SelectedIndex != 2)
			{
				//for final image size
				POINT	l_Pos = { 0,0 };
				RenderRectangle(l_Pos, (int)ImageWidth_numericUpDown->Value, (int)ImageHeight_numericUpDown->Value, Vector4(1.f, 0.3f, 0.3f, 0.3f));
			}
			if (this->MouseControlMode_comboBox->SelectedIndex == 1)
			{
				m_pOrthogonalCamera->SetDrawSelectFrame(true);
				m_pOrthogonalCamera->DrawSelectFrame();
			}
			else
				m_pOrthogonalCamera->SetDrawSelectFrame(true);
			if (m_pDebugFont)
			{
				UseShaderProgram();
				glEnable2D(1280, 720);
				POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
				std::wstring l_strMousePos = DNCT::GcStringToWchar(ptCursor.x.ToString() + "," + ptCursor.y.ToString());
				m_pDebugFont->RenderFont(0, 0, l_strMousePos.c_str());
				glDisable(GL_TEXTURE_2D);
			}
			glDisable(GL_ALPHA_TEST);
		}
		//GLRender::glDisable2D();
		glFlush();
		SwapBuffers(m_HdcMV);
	}
	System::Void cPIEditor::AutoOrderPosition_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iPixelXGap = (int)ImageDistanceX_numericUpDown->Value;
		int	l_iPixelYGap = (int)ImageDistanceY_numericUpDown->Value;
		if (!AutoAligment_checkBox->Checked)
		{
			//old method
			int	l_iCurrentX = 0;
			int	l_iCurrentY = 0;
			int	l_iMaxX = (int)ImageWidth_numericUpDown->Value;
			//the max height in current row
			int	l_iYLastBigPos = 0;
			if (!m_pImageomposerIRM->Count())
				return;
			//so to fix UV problem,1 pixle problem
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(0));
			POINT	l_Pos = { -l_pUIImage->GetOffsetPos()->x,-l_pUIImage->GetOffsetPos()->y };
			l_pUIImage->SetPos(l_Pos);
			//+1 for start offset(0,0)
			l_iCurrentX = l_pUIImage->GetRightDownStripOffPos().x - l_pUIImage->GetOffsetPos()->x + 1;
			l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y - l_pUIImage->GetOffsetPos()->y + 1;
			//if(OnePixelDistance_checkBox->Checked)
			{
				l_iYLastBigPos += l_iPixelYGap;
				l_iCurrentX += l_iPixelXGap;
			}
			for (int i = 1; i < m_pImageomposerIRM->Count(); ++i)
			{
				l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
				if (l_pUIImage->m_pEditorAttachParent)
					continue;
				//this length has pixels,offset+width-rightdown pos,add 1 for avoid same image at same pixel
				//+1 for offset start at 0,0
				int	l_iXLengthForPixels = l_pUIImage->GetRightDownStripOffPos().x - l_pUIImage->GetOffsetPos()->x + 1;
				//if(OnePixelDistance_checkBox->Checked)
				l_iXLengthForPixels += l_iPixelXGap;
				l_iCurrentX += (l_iXLengthForPixels);
				//the y pixel we do not need
				//+1 for offset start at 0,0
				int	l_iGarbagePixelY = l_pUIImage->GetRightDownStripOffPos().y - l_pUIImage->GetOffsetPos()->y + 1;
				l_iGarbagePixelY += l_iPixelXGap;
				if (l_iCurrentX > l_iMaxX)
				{
					//for next start not object position
					l_iCurrentX = l_iXLengthForPixels;//-l_pUIImage->GetOffsetPos()->x;
					l_iCurrentY += l_iYLastBigPos;
					//+1 for offset start at 0,0
					l_iYLastBigPos = l_pUIImage->GetRightDownStripOffPos().y - l_pUIImage->GetOffsetPos()->y + 1;
					//if(OnePixelDistance_checkBox->Checked)
					l_iYLastBigPos += l_iPixelYGap;
					//set object position x to offset pos,because it's changing to next line
					//
					l_Pos.x = -l_pUIImage->GetOffsetPos()->x;
					l_Pos.y = l_iCurrentY - l_pUIImage->GetOffsetPos()->y;
					l_pUIImage->SetPos(l_Pos);
					continue;
				}
				if (l_iYLastBigPos < l_iGarbagePixelY)
					l_iYLastBigPos = l_iGarbagePixelY;
				//although we know the position we wanted,we still have to minus the offset position to position
				l_Pos.x = l_iCurrentX - (l_iXLengthForPixels)-l_pUIImage->GetOffsetPos()->x;
				l_Pos.y = l_iCurrentY - l_pUIImage->GetOffsetPos()->y;
				l_pUIImage->SetPos(l_Pos);
			}
		}
		else
		{
			int l_iImageCount = m_pImageomposerIRM->Count();
			// Create some 'content' to work on.
			BinPack2D::ContentAccumulator<cUIImage*> inputContent;
			for (int i = 0; i < l_iImageCount; i++)
			{
				auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
				if (l_pUIImage->m_pEditorAttachParent)
					continue;
				//inputContent += BinPack2D::Content<cUIImage*>(l_pUIImage, BinPack2D::Coord(), BinPack2D::Size(l_pUIImage->GetWidth()+l_iPixelXGap,l_pUIImage->GetHeight()+l_iPixelYGap), false );
				POINT l_LeftUpPos = *l_pUIImage->GetOffsetPos();
				POINT l_RightDownPos = l_pUIImage->GetRightDownStripOffPos();
				POINT l_Size = { l_RightDownPos.x - l_LeftUpPos.x,l_RightDownPos.y - l_LeftUpPos.y };
				inputContent += BinPack2D::Content<cUIImage*>(l_pUIImage, BinPack2D::Coord(), BinPack2D::Size(l_Size.x + l_iPixelXGap, l_Size.y + l_iPixelYGap), false);
			}

			// Sort the input content by size... usually packs better.
			inputContent.Sort();

			// Create some bins! ( 2 bins, 128x128 in this example )
			BinPack2D::CanvasArray<cUIImage*> canvasArray = BinPack2D::UniformCanvasArrayBuilder<cUIImage*>((int)ImageWidth_numericUpDown->Value, (int)ImageHeight_numericUpDown->Value, 2).Build();

			// A place to store content that didnt fit into the canvas array.
			BinPack2D::ContentAccumulator<cUIImage*> remainder;

			// try to pack content into the bins.
			bool success = canvasArray.Place(inputContent, remainder);

			// A place to store packed content.
			BinPack2D::ContentAccumulator<cUIImage*> outputContent;

			// Read all placed content.
			canvasArray.CollectContent(outputContent);

			// parse output.
			typedef BinPack2D::Content<cUIImage*>::Vector::iterator binpack2d_iterator;
			//printf("PLACED:\n");
			if (remainder.Get().size() == 0)
			{
				for (binpack2d_iterator itor = outputContent.Get().begin(); itor != outputContent.Get().end(); itor++)
				{

					const BinPack2D::Content<cUIImage*> &content = *itor;
					// retreive your data.
					auto myContent = content.content;
					myContent->SetPos(Vector3((float)content.coord.x - myContent->GetOffsetPos()->x, (float)content.coord.y - myContent->GetOffsetPos()->y, 0));

					//printf("\t%9s of size %3dx%3d at position %3d,%3d,%2d rotated=%s\n",
					//   myContent.str.c_str(), 
					//   content.size.w, 
					//   content.size.h, 
					//   content.coord.x, 
					//   content.coord.y, 
					//   content.coord.z, 
					//   (content.rotated ? "yes":" no"));
				}
				if (IsObjectOverlap(m_pImageomposerIRM))
				{
					WARNING_MSG("texture is too small");
				}
			}
			else
			{
				WARNING_MSG("texture is too small");
				return;
			}
			// printf("NOT PLACED:\n");
			// for( binpack2d_iterator itor = remainder.Get().begin(); itor != remainder.Get().end(); itor++ ) {

			   //const BinPack2D::Content<NamedTypedObject*> &content = *itor;

			   //const NamedTypedObject* &myContent = content.content;

			   //printf("\t%9s of size %3dx%3d\n",
			   //   myContent.str.c_str(), 
			   //   content.size.w, 
			   //   content.size.h);
			// }


		   //if( !m_pImageomposerIRM->Count() )
		   //	return;
		   //rect_xywhf* l_pRects = new rect_xywhf[m_pImageomposerIRM->Count()];
		   //if( m_pImageomposerIRM->Count() >= 1000 )
		   //{
		   //	WARNING_MSG("too many images,please call fatming to make image over 10000");
		   //	return;
		   //}
		   //rect_xywhf *l_ptr_rects[1000];
		   //for(int i = 0; i < m_pImageomposerIRM->Count(); ++i) {
		   //	auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
		   //	l_pRects[i] = rect_xywhf(0, 0, l_pUIImage->GetWidth()+l_iPixelXGap,l_pUIImage->GetHeight()+l_iPixelYGap,i);
		   //	l_ptr_rects[i] = l_pRects+i;
		   //}

		   //vector<bin> bins;
		   //int l_MaxSize = max((int)ImageHeight_numericUpDown->Value,(int)ImageWidth_numericUpDown->Value);
		   //for(int i=0;i<100;++i)
		   //{
		   //	int l_iMax = 100*i;
		   //	if(pack(l_ptr_rects, m_pImageomposerIRM->Count(), l_iMax, bins))
		   //	{
		   //		if( bins.size() == 1 )
		   //		{
		   //			for( int j=0;j<(int)bins[0].rects.size();++j )
		   //			{
		   //				int l_iIndex = bins[0].rects[j]->iImageIndex;
		   //				auto l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iIndex));
		   //				l_pUIImage->SetPos(Vector3(bins[0].rects[j]->x-l_pUIImage->GetOffsetPos()->x,bins[0].rects[j]->y-l_pUIImage->GetOffsetPos()->y,0));
		   //				cGameApp::OutputDebugInfoString(UT::ComposeMsgByFormat("%d:firsy:%d,%d,second:",l_iIndex,bins[0].rects[j]->w,bins[0].rects[j]->h,l_pUIImage->GetWidth(),l_pUIImage->GetHeight()));
		   //				int a=0;
		   //			}
		   //			break;
		   //		}
		   //		else
		   //		{
		   //			bins.clear();
		   //			//WARNING_MSG("please adjust texture size");
		   //		}
		   //	}
		   //}
		   //SAFE_DELETE(l_pRects);
		}
		AttachObjectPosAdjust();
	}
	System::Void cPIEditor::AllImage_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (AllImage_listBox->SelectedIndex != -1)
		{
			m_iCurrentSelectedObjectIndex = AllImage_listBox->SelectedIndex;
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
			if (l_pUIImage && m_pPuzzleImageUnitTriangulatorManager)
			{
				m_pCurrentSelectedPuzzleImageUnitTriangulator = m_pPuzzleImageUnitTriangulatorManager->GetObject(l_pUIImage);
				if (m_pCurrentSelectedPuzzleImageUnitTriangulator && TriangulatorMouseBehavior_comboBox->SelectedIndex != -1)
				{
					m_pCurrentSelectedPuzzleImageUnitTriangulator->SetPointsToTriangulatorType((ePointsToTriangulatorType)TriangulatorMouseBehavior_comboBox->SelectedIndex);
					m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditApplyEmptyAnimationData();
					ImageTriangulatorLOD_numericUpDown->Value = m_pCurrentSelectedPuzzleImageUnitTriangulator->GetLOD();
					auto l_vCenterPos = m_pCurrentSelectedPuzzleImageUnitTriangulator->GetCenterOffset();
					CenterOffsetPosX_numericUpDown->Value = (int)l_vCenterPos.x;
					CenterOffsetPosY_numericUpDown->Value = (int)l_vCenterPos.y;
				}
			}
			NewPIUnitName_textBox->Text = DNCT::WcharToGcstring(l_pUIImage->GetName());
			//+1 for offset start at 0,0
			POINT	l_ImageSize = l_pUIImage->GetImageRealSize();
			ImageDetail_textBox->Text = "X:" + l_pUIImage->GetPos().x.ToString() + "Y:" + l_pUIImage->GetPos().y.ToString() + DNCT::GetChanglineString() +
				"Size:" + l_ImageSize.x.ToString() + "," + l_ImageSize.y.ToString() + DNCT::GetChanglineString() +
				"OffsetPos:" + l_pUIImage->GetOffsetPos()->x.ToString() + "," + l_pUIImage->GetOffsetPos()->y.ToString() +
				GetTotalPixelExisted();
			if (l_pUIImage->m_pEditorAttachParent)
			{
				ImageDetail_textBox->Text += DNCT::GetChanglineString() + "AttachedPIUnit!!!";
				m_bAvoidDataAssignForPIUintChild = true;
				NewPIUnitStartX_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos().x;
				NewPIUnitStartY_numericUpDown->Value = (System::Decimal)l_pUIImage->GetPos().y;
				NewPIUnitEndX_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos().x + l_pUIImage->GetRightDownStripOffPos().x);
				NewPIUnitEndY_numericUpDown->Value = (System::Decimal)(l_pUIImage->GetPos().y + l_pUIImage->GetRightDownStripOffPos().y);
				m_bAvoidDataAssignForPIUintChild = false;
			}
			if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
			{
				MorphingAnimationTime_listBox->Items->Clear();
				auto l_pData = m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetListboxTimeVector();
				if (l_pData)
				{
					for (auto l_fTime : *l_pData)
						MorphingAnimationTime_listBox->Items->Add(l_fTime.ToString());
					int l_iMax = (int)(m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetEndTime() * 1000);
					MorphingAnimation_trackBar->Maximum = l_iMax;
				}
			}
		}
	}
	System::Void cPIEditor::button2_Click(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectIndex = AllImage_listBox->SelectedIndex;
		if (l_iSelectIndex == -1)
			return;
		std::list<int>	l_iAllSelectedIndex;
		bool	l_bContainTop = false;
		bool	l_bContainDown = false;
		for each(int l_iIndex in AllImage_listBox->SelectedIndices)
			l_iAllSelectedIndex.push_back(l_iIndex);
		l_iAllSelectedIndex.sort();
		AllImage_listBox->SelectedIndices->Clear();
		if (sender == ImageObjectDown_button)
		{
			while (l_iAllSelectedIndex.size())
			{
				int	l_iIndex = l_iAllSelectedIndex.back();
				l_iAllSelectedIndex.pop_back();
				//it's bottom
				if (l_iIndex >= AllImage_listBox->Items->Count - 1)
				{
					WARNING_MSG("buttom one can't be down any more");
					goto ADJUST_IMAG_POS;
				}
				cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
				m_pImageomposerIRM->RemoveObjectWithoutDelete(l_iIndex);
				m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin() + (l_iIndex + 1), l_pUIImage);
				DNCT::SwapListBoxValue(l_iIndex, l_iIndex + 1, AllImage_listBox);
				AllImage_listBox->SelectedIndices->Add(l_iIndex + 1);
			}
		}
		else
		{
			//it's top
			while (l_iAllSelectedIndex.size())
			{
				int	l_iIndex = l_iAllSelectedIndex.front();
				l_iAllSelectedIndex.pop_front();
				//it's top
				if (l_iIndex == 0)
				{
					WARNING_MSG("top one can't be up any more");
					goto ADJUST_IMAG_POS;
				}
				cUIImage*	l_pUIImage = m_pImageomposerIRM->GetUIImage(l_iIndex);
				m_pImageomposerIRM->RemoveObjectWithoutDelete(l_iIndex);
				m_pImageomposerIRM->GetList()->insert(m_pImageomposerIRM->GetList()->begin() + (l_iIndex - 1), l_pUIImage);
				DNCT::SwapListBoxValue(l_iIndex, l_iIndex - 1, AllImage_listBox);
				AllImage_listBox->SelectedIndices->Add(l_iIndex - 1);
			}
		}
	ADJUST_IMAG_POS:
		AutoOrderPosition_button_Click(sender, e);
		if (AllImage_listBox->SelectedIndex != -1)
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
			POINT	l_ImageSize = { l_pUIImage->GetRightDownStripOffPos().x - l_pUIImage->GetOffsetPos()->x + 1,
				l_pUIImage->GetRightDownStripOffPos().y - l_pUIImage->GetOffsetPos()->y + 1 };
			Vector3	l_vImagePos = l_pUIImage->GetPos();
			POINT	l_ImagePos = { (int)l_vImagePos.x,(int)l_vImagePos.y };
			ImageDetail_textBox->Text = "X:" + l_ImagePos.x.ToString() + "Y:" + l_ImagePos.y.ToString() + DNCT::GetChanglineString() +
				"Size:" + l_ImageSize.x.ToString() + "," + l_ImageSize.y.ToString() + DNCT::GetChanglineString() +
				"OffsetPos:" + l_pUIImage->GetOffsetPos()->x.ToString() + "," + l_pUIImage->GetOffsetPos()->y.ToString() +
				GetTotalPixelExisted();
		}
	}
			 //unform
	System::Void cPIEditor::tabControl_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->Visible)
		{
			timer1->Enabled = true;
			wglMakeCurrent(m_HdcMV, m_HGLRCMV);
			cGameApp::m_spOpenGLRender->m_vViewPortSize.x = 0.f;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.y = 0.f;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.z = (float)splitContainer2->Panel1->Width;
			cGameApp::m_spOpenGLRender->m_vViewPortSize.w = (float)splitContainer2->Panel1->Height;
			cGameApp::ApplyViewPort();
		}
		else
		{
			timer1->Enabled = false;

		}
	}
	System::Void cPIEditor::MyMouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (!timer1->Enabled)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		auto l_iDelte = GetMouseWheelDelta(e);
		if (tabControl1->SelectedIndex == 2)
		{

			POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
			m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			if (l_MouseButton == System::Windows::Forms::MouseButtons::Left)
			{
				if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
				{
					if (EditAnimation_checkBox->Checked)
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditMouseMove(ptCursor.x, ptCursor.y);
					else
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseMove(ptCursor.x, ptCursor.y);
				}
			}
		}
		else
		{
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			if (this->MouseControlMode_comboBox->SelectedIndex == 0)
			{
				POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
				MouseCollideForPickUpObject(e, splitContainer2->Panel1);
				ptCursor.x = e->X; ptCursor.y = e->Y;
				//HWND	l_Hwnd = WindowFromPoint(ptCursor);
				if (tabControl1->SelectedIndex == 2)
				{
					if (this->m_pPuzzleImageUnitTriangulatorManager)
					{
						m_pPuzzleImageUnitTriangulatorManager->MouseMove(ptCursor.x, ptCursor.y);
					}
				}
			}
		}
	}
	System::Void cPIEditor::MyMouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (!timer1->Enabled)
			return;
		splitContainer2->Panel1->Focus();
		GCFORM::MouseButtons l_MouseButton = e->Button;
		auto l_iDelte = GetMouseWheelDelta(e);
		if (tabControl1->SelectedIndex == 2)
		{
			m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			if(l_MouseButton == System::Windows::Forms::MouseButtons::Left)
			{
			POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
			if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
			{
				if (EditAnimation_checkBox->Checked)
					m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditMouseDown(ptCursor.x, ptCursor.y);
				else
					m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseDown(ptCursor.x, ptCursor.y);
			}
			}
		}
		else
		{
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			if (this->MouseControlMode_comboBox->SelectedIndex == 0)
			{
				if (this->Visible)
					MouseCollideForPickUpObject(e, splitContainer2->Panel1);
			}
			else
			{
				POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
				NewPIUnitStartX_numericUpDown->Value = ptCursor.x;
				NewPIUnitStartY_numericUpDown->Value = ptCursor.y;
			}
		}
	}
	System::Void cPIEditor::MyMouseUp(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (!timer1->Enabled)
			return;
		GCFORM::MouseButtons l_MouseButton = e->Button;
		auto l_iDelte = GetMouseWheelDelta(e);
		if (tabControl1->SelectedIndex == 2)
		{
			m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			POINT	ptCursor = { (int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().x,(int)m_pOrthogonalCameraForTrianhulatorPIUnit->GetMouseWorldPos().y };
			if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
			{
				if (l_MouseButton == System::Windows::Forms::MouseButtons::Left)
				{
					if (EditAnimation_checkBox->Checked)
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditMouseUp(ptCursor.x, ptCursor.y);
					else
					{
						m_pCurrentSelectedPuzzleImageUnitTriangulator->MouseUp(ptCursor.x, ptCursor.y);
						ImageTriangulator_textBox->Text = gcnew String(m_pCurrentSelectedPuzzleImageUnitTriangulator->GetInfo().c_str());
					}
				}
			}
		}
		else
		{
			m_pOrthogonalCamera->CameraUpdateByMouse(l_MouseButton == System::Windows::Forms::MouseButtons::Left ? true : false
				, l_MouseButton == System::Windows::Forms::MouseButtons::Right ? true : false, l_iDelte, e->X, e->Y, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
			if (this->MouseControlMode_comboBox->SelectedIndex == 0)
			{
				if (this->Visible)
					MouseCollideForPickUpObject(e, splitContainer2->Panel1);
			}
			else
			{
				POINT	ptCursor = { (int)m_pOrthogonalCamera->GetMouseWorldPos().x,(int)m_pOrthogonalCamera->GetMouseWorldPos().y };
				if (ptCursor.x < 0)
					ptCursor.x = 0;
				if (ptCursor.y < 0)
					ptCursor.y = 0;
				NewPIUnitEndX_numericUpDown->Value = ptCursor.x;
				NewPIUnitEndY_numericUpDown->Value = ptCursor.y;
			}
		}
	}
	System::Void cPIEditor::MyMouseHover(System::Object^  sender, System::EventArgs^  e) {
		if (!timer1->Enabled)
			return;
	}
	System::Void cPIEditor::MyKeyPress(System::Object^  sender, System::Windows::Forms::KeyPressEventArgs^  e) {
		if (!timer1->Enabled)
			return;
	}
	System::Void cPIEditor::MyKeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::Add)
		{
			m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale() - 0.1f);
		}
		else
			if (e->KeyCode == Keys::Subtract)
			{
				m_pOrthogonalCamera->SetScale(m_pOrthogonalCamera->GetScale() + 0.1f);
			}
		if (m_iCurrentSelectedObjectIndex != -1)
		{
			cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
			Vector3	l_Pos = l_p->GetPos();
			if (e->KeyCode == Keys::Right)
				l_Pos.x += 1;
			if (e->KeyCode == Keys::Left)
				l_Pos.x -= 1;
			if (e->KeyCode == Keys::Up)
				l_Pos.y -= 1;
			if (e->KeyCode == Keys::Down)
				l_Pos.y += 1;
			l_p->SetPos(l_Pos);
			AttachObjectPosAdjust();
		}
		//if(!timer1->Enabled)
		//	return;
	}
	System::Void cPIEditor::MyKeyDown(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e) {
		if (e->KeyCode == Keys::R)
			m_pOrthogonalCamera->Reset();
		//if(e->KeyCode == Keys::C)
		//{
		//	char l_strFileName[2048];
		//   SYSTEMTIME l_SYSTEMTIME;
		//   GetSystemTime(&l_SYSTEMTIME);
		//   sprintf(l_strFileName,"%d%d%d%d%d%d.bmp",l_SYSTEMTIME.wYear,l_SYSTEMTIME.wMonth,l_SYSTEMTIME.wDay,l_SYSTEMTIME.wHour,l_SYSTEMTIME.wMinute,l_SYSTEMTIME.wSecond);
		//   SaveCurrentBufferToImage(l_strFileName,(int)DRAW_PANEL_RESOLUTION_WIDTH,(int)DRAW_PANEL_RESOLUTION_HEIGHT);
		//}
		if (!timer1->Enabled)
			return;
		if (m_iCurrentSelectedObjectIndex != -1)
		{
			cUIImage*l_p = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(m_iCurrentSelectedObjectIndex));
			Vector3	l_Pos = l_p->GetPos();
			if (e->KeyCode == Keys::Right)
				l_Pos.x += 1;
			if (e->KeyCode == Keys::Left)
				l_Pos.x -= 1;
			if (e->KeyCode == Keys::Up)
				l_Pos.y -= 1;
			if (e->KeyCode == Keys::Down)
				l_Pos.y += 1;
			l_p->SetPos(l_Pos);
		}
	}
	System::Void cPIEditor::Test_button_Click(System::Object^  sender, System::EventArgs^  e)
{
	String^l_FileName = DNCT::SaveFileAndGetName("(*.pi)|*.pi");
	if (l_FileName)
	{
		SavePuzzleFile(l_FileName, false);
		if (BinaryExport_checkBox->Checked)
		{
			SavePuzzleFile(l_FileName, true);
		}
	}
}
	System::Void cPIEditor::button1_Click(System::Object^  sender, System::EventArgs^  e)
	{
		GeneratePowOf2Image(sender == generateAllImageWithPowerOfTwoToolStripMenuItem ? true : false);
	}
	System::Void cPIEditor::ImageWidth_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e) 
	{
		TotalPixel_label->Text = (ImageHeight_numericUpDown->Value*ImageWidth_numericUpDown->Value).ToString();
	}
	System::Void cPIEditor::InvertPuzzleImage_button_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		cli::array< String^ >^l_pNameList = DNCT::OpenFileAndGetNames("pi files (*.pi)|*.pi|All files (*.*)|*.*");
		if (!l_pNameList)
		{
			return;
		}
		String^l_strDirectory = DNCT::SelectDirectory();
		if (!l_strDirectory)
		{
			return;
		}
		if (l_pNameList)
		{
			for each (String ^ l_fileName in l_pNameList)
			{
				cNodeISAX	l_cNodeISAX;
				TiXmlElement* l_pRootTiXmlElement = 0;
				bool	l_b = l_cNodeISAX.ParseDataIntoXMLNode(DNCT::GcStringToChar(l_fileName).c_str());
				if (l_b)
				{
					const WCHAR* l_strFileName = 0;
					//find the tag we needed
					l_pRootTiXmlElement = l_cNodeISAX.GetXmlElementByNameFromRoot(L"PuzzleImage");
					while (l_pRootTiXmlElement)
					{
						l_strFileName = l_pRootTiXmlElement->Attribute(L"ImageName");
						System::Drawing::Bitmap^ l_pBitmapSource;
						String^ l_strPGileName = DNCT::GetDirectoryWithoutFileName(l_fileName) + String(l_strFileName).ToString();
						try
						{
							l_pBitmapSource = gcnew System::Drawing::Bitmap(l_strPGileName);
						}
						catch (System::Exception^ l_pExecption)
						{
							l_pExecption->ToString();
							WARNING_MSG(l_strPGileName + " not existed");
							continue;
						}
						int	l_iCount = _wtoi(l_pRootTiXmlElement->Attribute(L"Count"));
						TiXmlElement* l_pPuzzleImageUintTiXmlElement = l_pRootTiXmlElement->FirstChildElement();
						while (l_pPuzzleImageUintTiXmlElement)
						{
							TiXmlElement* l_pCurrentElement = l_pPuzzleImageUintTiXmlElement;
							TiXmlAttribute* l_pTiXmlAttribute = l_pCurrentElement->FirstAttribute();
							float	l_fUV[4];
							POINT	l_Size;
							POINT	l_OffsetPos;
							POINT	l_ShowPosInPI;
							POINT	l_OriginalSize;
							//new attribute so make sure the data has this
							bool	l_bContainShowPosInPI = false;
							const WCHAR* l_strPuzzleImageName = 0;
							while (l_pTiXmlAttribute)
							{
								if (!wcscmp(l_pTiXmlAttribute->Name(), L"UV"))
								{
									GetUV(l_pTiXmlAttribute->Value(), l_fUV);
								}
								else
									if (!wcscmp(l_pTiXmlAttribute->Name(), L"Size"))
									{
										l_Size = GetPoint(l_pTiXmlAttribute->Value());
									}
									else
										if (!wcscmp(l_pTiXmlAttribute->Name(), L"OffsetPos"))
										{
											l_OffsetPos = GetPoint(l_pTiXmlAttribute->Value());
										}
										else
											if (!wcscmp(l_pTiXmlAttribute->Name(), L"Name"))
											{
												l_strPuzzleImageName = l_pTiXmlAttribute->Value();
											}
											else
												if (!wcscmp(l_pTiXmlAttribute->Name(), L"ShowPosInPI"))
												{
													l_bContainShowPosInPI = true;
													l_ShowPosInPI = GetPoint(l_pTiXmlAttribute->Value());
												}
												else
													if (!wcscmp(l_pTiXmlAttribute->Name(), L"OriginalSize"))
													{
														l_OriginalSize = GetPoint(l_pTiXmlAttribute->Value());
													}
													else
													{
														assert(0);
													}
								l_pTiXmlAttribute = l_pTiXmlAttribute->Next();
							}
							//fill all alpha
							System::Drawing::Bitmap^ l_pBitMap = gcnew System::Drawing::Bitmap(l_OriginalSize.x, l_OriginalSize.y);
							System::Drawing::Color l_NewColor = System::Drawing::Color::Transparent;
							System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush(l_NewColor);
							System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
							l_pGr->FillRectangle(l_pDrawBrush, 0, 0, l_pBitMap->Width, l_pBitMap->Height);
							System::Drawing::Rectangle	l_rect = System::Drawing::Rectangle((int)(l_fUV[0] * l_pBitmapSource->Width), (int)(l_fUV[1] * l_pBitmapSource->Height), l_Size.x, l_Size.y);
							if (l_bContainShowPosInPI)
							{
								l_rect = System::Drawing::Rectangle(l_ShowPosInPI.x, l_ShowPosInPI.y, l_Size.x, l_Size.y);
							}
							if (0)
							{
								////copy Src
								//int	bufferSizeInPixels = l_pBitMap->Width*l_pBitMap->Height;
								//char*l_pbuff = new char[bufferSizeInPixels*l_iSrcChannel];
								//BitmapData^l_pData =
								//l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),
								//ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//memcpy(l_pbuff,l_pData->Scan0.ToPointer(),bufferSizeInPixels*l_iSrcChannel);
								//l_pBitmapForSave->UnlockBits(l_pData);
								////past to dest
								//l_pData = l_pBitMap->LockBits(System::Drawing::Rectangle(0, 0, l_pBitMap->Width, l_pBitMap->Height),ImageLockMode::WriteOnly,l_pBitmapForSave->PixelFormat);
								//char*l_strScrData = (char*)l_pData->Scan0.ToPointer();

								//int	l_iSrcRenderStartPosX = l_pUIImage->GetOffsetPos()->x;
								//int	l_iSrcRenderStartPosY = l_pUIImage->GetOffsetPos()->y;
								//+1 for offset start at 0,0
								//int	l_iSrcRenderEndPosX = l_pUIImage->GetRightDownStripOffPos().x+1;
								//int	l_iSrcRenderEndPosY = l_pUIImage->GetRightDownStripOffPos().y+1;
								//int	l_iDestRenderPosX = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->x+l_pUIImage->GetOffsetPos()->x);
								//int	l_iDestRenderPosY = StripFloatIfBiggerThanPoint5(l_pUIImage->GetPos()->y+l_pUIImage->GetOffsetPos()->y);
								//int	l_iWorkPixelX = l_iSrcRenderEndPosX-l_iSrcRenderStartPosX;
								//int	l_iWorkPixelY = l_iSrcRenderEndPosY-l_iSrcRenderStartPosY;
								//int	l_iIndex = 0;
								//for( int l_iStartPixelY=l_iSrcRenderStartPosY;l_iStartPixelY<l_iSrcRenderEndPosY;++l_iStartPixelY )
								//{
								//	int	l_iYIndex = ((l_iDestRenderPosY+l_iIndex)*l_iSrcChannel*l_pBitMap->Width);
								//	int	l_iXIndex = l_iSrcChannel*l_iDestRenderPosX;
								//	int	l_iStartCopyIndex = l_iXIndex+l_iYIndex;
								//	int	l_iCopyIntoIndex = (l_iStartPixelY*l_iSrcChannel*l_pBitmapForSave->Width)+(l_iSrcChannel*l_iSrcRenderStartPosX);
								//	memcpy(&l_strScrData[l_iStartCopyIndex],&l_pbuff[l_iCopyIntoIndex],l_iWorkPixelX*l_iSrcChannel);
								//	++l_iIndex;
								//}
								//l_pBitMap->UnlockBits(l_pData);
								//delete l_pbuff;
							}
							else
								l_pGr->DrawImage(l_pBitmapSource, l_OffsetPos.x, l_OffsetPos.y, l_rect, System::Drawing::GraphicsUnit::Pixel);
							l_pBitMap->Save(l_strDirectory + String(l_strPuzzleImageName).ToString() + ".png");
							l_pPuzzleImageUintTiXmlElement = l_pPuzzleImageUintTiXmlElement->NextSiblingElement();
						}
						if (l_pRootTiXmlElement->NextSibling())
							l_pRootTiXmlElement = GetXmlElementByNameFromElement(L"PuzzleImage", l_pRootTiXmlElement->NextSiblingElement());
						else
							l_pRootTiXmlElement = 0;
					}
				}
			}
		}
	}
	System::Void cPIEditor::TO96PdiPngFile_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strDirectory = DNCT::SelectDirectory();
		if (!l_strDirectory)
			return;
		cli::array<String^>^l_AllFileName = DNCT::OpenFileAndGetNames();
		for each(String^l_strName in l_AllFileName)
		{
			System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap(l_strName);
			l_pBitMap->Save(l_strDirectory + DNCT::GetFileNameWithoutFullPath(l_strName));
		}
		//System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( System::Drawing::Color::Transparent );
		//for( int i=0;i<m_pImageomposerIRM->Count();++i )
		//{
		//	cUIImage*l_pUIImage = (cUIImage*)((*m_pImageomposerIRM)[i]);
		//	System::Drawing::Bitmap^l_pBitmapForSave = (System::Drawing::Bitmap^)m_ImageTale[DNCT::WcharToGcstring((*m_pImageomposerIRM)[i]->GetName())];
		//	l_pBitMap->Save(l_strDirectory+DNCT::WcharToGcstring(l_pUIImage->GetName())+".png");
		//}
	}

	System::Void cPIEditor::openXMLToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		String^l_strFileName = DNCT::OpenFileAndGetName("PuzzleImage(*.pi;*.pib)|*.pi;*.pib");
		//String^l_strFileName = DNCT::OpenFileAndGetName("txt files (*.txt)|*.txt|All files (*.*)|*.*");
		if (l_strFileName)
		{
			PlayMorphingAnimation_checkBox->Checked = false;
			this->EditAnimation_checkBox->Checked = false;
			OpenPIFile(l_strFileName);
			auto l_strCharFileName = DNCT::GcStringToChar(l_strFileName);
			UT::SaveTxtToFile(LAST_USE_PI_FILE_NAME, l_strCharFileName.c_str(), (int)l_strCharFileName.length());
		}
	}
	System::Void cPIEditor::AddAnimationImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (sender == AddAnimationImage_button)
		{
			if (AllImage_listBox->SelectedIndices->Count == 0)
				return;
			while (AllImage_listBox->SelectedIndices->Count != 0)
			{
				int	l_iIndex = AllImage_listBox->SelectedIndices[0];
				float l_fTime = (float)AnimationTime_numericUpDown->Value;
				m_pImageIndexOfAnimation->AddNameObject(DNCT::GcStringToWchar(AllImage_listBox->SelectedItems[0]->ToString()).c_str(), AllImage_listBox->SelectedIndices[0], l_fTime);
				AnimationData_listBox->Items->Add(AllImage_listBox->SelectedItems[0]->ToString());
				AllImage_listBox->SelectedItems->Remove(AllImage_listBox->SelectedItems[0]);
			}
		}
		else
			if (sender == DelAnimationImage_button)
			{
				if (AnimationData_listBox->SelectedIndices->Count == 0)
					return;
				while (this->AnimationData_listBox->SelectedIndices->Count)
				{
					int	l_iIndex = AnimationData_listBox->SelectedIndices[0];
					m_pImageIndexOfAnimation->RemoveNameObject(l_iIndex);
					AnimationData_listBox->Items->RemoveAt(l_iIndex);
				}
			}
	}
	System::Void cPIEditor::AnimationDatAdd_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (sender == AnimationDatAdd_button)
		{
			if (AnimationDataName_textBox->Text->Length)
			{
				if (m_pImageIndexOfAnimation->Count())
				{
					if (!AnimationDataList_listBox->Items->Contains(AnimationDataName_textBox->Text))
					{
						AnimationDataList_listBox->Items->Add(AnimationDataName_textBox->Text);
						AnimationData_listBox->Items->Clear();
						cImageIndexOfAnimation*l_pImageIndexOfAnimation = new cImageIndexOfAnimation(m_pImageIndexOfAnimation);
						m_pImageIndexOfAnimation->Clear();
						l_pImageIndexOfAnimation->SetName(DNCT::GcStringToWchar(AnimationDataName_textBox->Text));
						m_pImageIndexOfAnimationList->AddObject(l_pImageIndexOfAnimation);
					}
				}
			}
		}
		else
			if (sender == AnimationDatDel_button)
			{
				while (AnimationDataList_listBox->SelectedItems->Count)
				{
					m_pImageIndexOfAnimationList->RemoveObject(DNCT::GcStringToWchar(AnimationDataList_listBox->SelectedItems[0]->ToString()));
					AnimationDataList_listBox->Items->Remove(AnimationDataList_listBox->SelectedItems[0]);
				}
			}
	}

	System::Void cPIEditor::ImagePosX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		int	l_iSelectedIndex = AllImage_listBox->SelectedIndex;
		if (l_iSelectedIndex != -1)
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[l_iSelectedIndex]);
			l_pUIImage->SetPos(Vector3((float)ImagePosX_numericUpDown->Value, (float)ImagePosY_numericUpDown->Value, 0.f));
		}
	}
	System::Void cPIEditor::AllImage_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		if (AllImage_listBox->SelectedIndex != -1)
		{
			label3->Text = "Pos:" + AllImage_listBox->Text;
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>((*m_pImageomposerIRM)[AllImage_listBox->SelectedIndex]);
			Vector3	l_vPos = l_pUIImage->GetPos();
			ImagePosX_numericUpDown->Value = (int)l_vPos.x;
			ImagePosY_numericUpDown->Value = (int)l_vPos.y;
		}
	}
	System::Void cPIEditor::AllImage_listBox_KeyUp_1(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if (e->KeyCode == Keys::Enter)
		{
			System::Windows::Forms::MouseEventArgs^  e2;
			AllImage_listBox_MouseDoubleClick(sender, e2);
		}
	}
	System::Void cPIEditor::AnimationData_listBox_MouseDoubleClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		AssignAnimationData();
	}
	System::Void cPIEditor::AnimationData_listBox_KeyUp(System::Object^  sender, System::Windows::Forms::KeyEventArgs^  e)
	{
		if (e->KeyCode == GCFORM::Keys::Enter)
			AssignAnimationData();
	}
	System::Void cPIEditor::ProperStripPixel_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		POINT	l_MinOffset = { 79979,79979 };
		POINT	l_MaxRightDown = { 0,0 };
		POINT	l_OffsetPos;
		POINT	l_RightDown;
		for each(int l_iSelectedIndex in AllImage_listBox->SelectedIndices)
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
			l_OffsetPos = *l_pUIImage->GetOffsetPos();
			l_RightDown = l_pUIImage->GetRightDownStripOffPos();
			//+1 for offset start at 0,0
			l_RightDown.x += 1;
			l_RightDown.y += 1;

			l_MinOffset.x = min(l_MinOffset.x, l_OffsetPos.x);
			l_MinOffset.y = min(l_MinOffset.y, l_OffsetPos.y);
			l_MaxRightDown.x = max(l_MaxRightDown.x, l_RightDown.x);
			l_MaxRightDown.y = max(l_MaxRightDown.y, l_RightDown.y);
		}
		for each(int l_iSelectedIndex in AllImage_listBox->SelectedIndices)
		{
			cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
			l_pUIImage->SetOffsetPos(l_MinOffset);
			l_pUIImage->SetRightDownStripOffPos(l_MaxRightDown);
		}
	}
	System::Void cPIEditor::StripAlpha_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (ToOriginalImage_button == sender)
		{
			for each(int l_iSelectedIndex in AllImage_listBox->SelectedIndices)
			{
				cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
				POINT	l_WidthHeight = { l_pUIImage->GetWidth() - 1,l_pUIImage->GetHeight() - 1 };
				POINT	l_Offset = { 0,0 };
				l_pUIImage->SetRightDownStripOffPos(l_WidthHeight);
				l_pUIImage->SetOffsetPos(l_Offset);
			}
			AllImage_listBox_SelectedIndexChanged(sender, e);
		}
		else
			if (StripAlpha_button == sender)
			{
				for each(int l_iSelectedIndex in AllImage_listBox->SelectedIndices)
				{
					System::Drawing::Bitmap^l_pImage = (System::Drawing::Bitmap^)m_ImageTale[AllImage_listBox->Items[l_iSelectedIndex]->ToString()];
					if (!l_pImage)
						continue;
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iSelectedIndex));
					l_pUIImage->SetOffsetPos(DNCT::ImageGetAlphaOffsetPosLeftTop(l_pImage));
					l_pUIImage->SetRightDownStripOffPos(DNCT::ImageGetAlphaOffsetPosRightDown(l_pImage));
					POINT	l_ImageRealSize = l_pUIImage->GetImageRealSize();
					POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
					POINT	l_OffsetPos = *l_pUIImage->GetOffsetPos();
					//because MPDI is setposition by center
					//if( l_ImageRealSize.x%2 )
					//{
					//	if( l_RightDownStripOffPos.x+1 < l_pUIImage->GetOriginalImageSize().x )
					//		l_RightDownStripOffPos.x += 1;
					//	else
					//	if( l_OffsetPos.x != 0 )
					//		l_OffsetPos.x -= 1;
					//}
					//if( l_ImageRealSize.y%2)
					//{
					//	if( l_RightDownStripOffPos.y+1 < l_pUIImage->GetOriginalImageSize().y )
					//		l_RightDownStripOffPos.y += 1;
					//	else
					//	if( l_OffsetPos.y != 0 )
					//		l_OffsetPos.y -= 1;
					//}
					l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
					l_pUIImage->SetOffsetPos(l_OffsetPos);
				}
				AllImage_listBox_SelectedIndexChanged(sender, e);
			}
	}

	System::Void cPIEditor::splitContainer2_Panel1_Resize(System::Object^  sender, System::EventArgs^  e)
	{
		if (!m_pOrthogonalCamera)
			return;
		POINT l_ViewportSize = { splitContainer2->Panel1->Width,splitContainer2->Panel1->Height };
		Vector2	l_vOriginalResolution = m_pOrthogonalCamera->GetResolution();
		Vector2	l_vOriginalLeftUpPos = m_pOrthogonalCamera->GetCameraPos() - (l_vOriginalResolution / 2.f);
		m_pOrthogonalCamera->SetResolution(Vector2((float)l_ViewportSize.x, (float)l_ViewportSize.y));
		m_pOrthogonalCamera->SetCameraPos(l_vOriginalLeftUpPos + m_pOrthogonalCamera->GetResolution() / 2.f);
		m_pOrthogonalCamera->CameraUpdateByMouse(false, false, 0, 0, 0, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
		if (!m_pOrthogonalCameraForTrianhulatorPIUnit)
			return;
		l_vOriginalResolution = m_pOrthogonalCameraForTrianhulatorPIUnit->GetResolution();
		l_vOriginalLeftUpPos = m_pOrthogonalCameraForTrianhulatorPIUnit->GetCameraPos() - (l_vOriginalResolution / 2.f);
		m_pOrthogonalCameraForTrianhulatorPIUnit->SetResolution(Vector2((float)l_ViewportSize.x, (float)l_ViewportSize.y));
		m_pOrthogonalCameraForTrianhulatorPIUnit->SetCameraPos(l_vOriginalLeftUpPos + m_pOrthogonalCameraForTrianhulatorPIUnit->GetResolution() / 2.f);
		m_pOrthogonalCameraForTrianhulatorPIUnit->CameraUpdateByMouse(false, false, 0, 0, 0, Vector2((float)splitContainer2->Panel1->Size.Width, (float)splitContainer2->Panel1->Size.Height));
	}

	System::Void cPIEditor::AddNewPIUnitImage_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (NewPIUnitStartX_numericUpDown->Value != NewPIUnitEndX_numericUpDown->Value &&
			NewPIUnitStartY_numericUpDown->Value != NewPIUnitEndY_numericUpDown->Value &&
			NewPIUnitEndX_numericUpDown->Value > NewPIUnitStartX_numericUpDown->Value &&
			NewPIUnitEndY_numericUpDown->Value > NewPIUnitStartY_numericUpDown->Value)
		{
			if (NewPIUnitName_textBox->Text->Length)
			{
				this->timer1->Enabled = false;
				int	l_iOriginalIndex = -1;
				POINT	l_OffsetPos = { 0,0 };
				POINT	l_RightDownStripOffPos = { (int)NewPIUnitEndX_numericUpDown->Value - (int)NewPIUnitStartX_numericUpDown->Value,(int)NewPIUnitEndY_numericUpDown->Value - (int)NewPIUnitStartY_numericUpDown->Value };
				Vector3	l_vPos((float)NewPIUnitStartX_numericUpDown->Value, (float)NewPIUnitStartY_numericUpDown->Value, 0.f);
				RECT	l_rc = { (int)l_vPos.x,(int)l_vPos.y,(int)l_vPos.x + l_RightDownStripOffPos.x,(int)l_vPos.y + l_RightDownStripOffPos.y };
				int	l_iCollideIndex = -1;
				for (int i = 0; i < m_pImageomposerIRM->Count(); ++i)
				{
					cUIImage*l_pUIImage2 = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					Vector3	l_vPos2 = l_pUIImage2->GetPos();
					POINT	l_RightDownStripOffPos2 = l_pUIImage2->GetRightDownStripOffPos();
					RECT	l_rc2 = { (int)l_vPos2.x + l_pUIImage2->GetOffsetPos()->x,(int)l_vPos2.y + l_pUIImage2->GetOffsetPos()->y,
						(int)l_vPos2.x + l_RightDownStripOffPos2.x,(int)l_vPos2.y + l_RightDownStripOffPos2.y };
					if (UT::RectCollideRect(l_rc2, l_rc))
					{
						FMLog::Log(l_pUIImage2->GetName(), false);
						if (l_iCollideIndex != -1)
						{
							if (!l_pUIImage2->m_pEditorAttachParent)
							{
								WARING_YES_NO_TO_NO("over 2 image would u like to do this anyway?")
								{
									goto LEAVE;
								}
								break;
							}
						}
						//ensure it is not a piunit child
						if (!l_pUIImage2->m_pEditorAttachParent)
							l_iCollideIndex = i;
					}
				}
				if (l_iCollideIndex == -1)
				{
					WARNING_MSG("no parent,please reset pos");
					goto LEAVE;
				}
				if (AllImage_listBox->Items->Contains(NewPIUnitName_textBox->Text))
				{
					WARING_YES_NO_TO_NO("replace exist object?")
					{
						goto LEAVE;
					}
					l_iOriginalIndex = AllImage_listBox->Items->IndexOf(NewPIUnitName_textBox->Text);
					AllImage_listBox->SelectedIndex = l_iOriginalIndex;
					DeleteImage_button_Click(DeleteImage_button, e);
				}
				cUIImage*l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iCollideIndex));
				cUIImage*l_pUIImage = nullptr;
				System::Drawing::Bitmap^l_pBitMap = (System::Drawing::Bitmap^)m_ImageTale[gcnew String(l_pAttachUIImage->GetName())];
				//if (l_pBitMap)
				{
					auto l_vParentPos = l_pAttachUIImage->GetPos();
					std::vector<Vector2> l_Vector =
					{
						Vector2((int)NewPIUnitStartX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitStartY_numericUpDown->Value - l_vParentPos.y),
						Vector2((int)NewPIUnitEndX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitStartY_numericUpDown->Value - l_vParentPos.y),
						Vector2((int)NewPIUnitStartX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitEndY_numericUpDown->Value - l_vParentPos.y),
						Vector2((int)NewPIUnitStartX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitEndY_numericUpDown->Value - l_vParentPos.y),
						Vector2((int)NewPIUnitEndX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitStartY_numericUpDown->Value - l_vParentPos.y),
						Vector2((int)NewPIUnitEndX_numericUpDown->Value - l_vParentPos.x,(int)NewPIUnitEndY_numericUpDown->Value - l_vParentPos.y)
					};
					List<System::Drawing::Point>^l_pPointList = Vector2ToListPoint(&l_Vector);
					Image^l_pImage = (Image^)l_pBitMap;
					Bitmap^l_pNew = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList, true);
					l_pUIImage = cPIEditor::GetNewUIImageByBitMap(l_pNew, DNCT::GcStringToWchar(NewPIUnitName_textBox->Text).c_str());
					m_ImageTale[NewPIUnitName_textBox->Text] = l_pNew;
				}
				l_pUIImage->m_vEditorAttachParentRelativePos = l_vPos - l_pAttachUIImage->GetPos();
				l_pUIImage->m_pEditorAttachParent = l_pAttachUIImage;
				l_pUIImage->SetPos(l_vPos);
				////it could be replaced,recheck index again
				l_iOriginalIndex = AllImage_listBox->Items->IndexOf(NewPIUnitName_textBox->Text);
				if (l_iOriginalIndex != -1)
				{
					(*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
					AllImage_listBox->Items[l_iOriginalIndex] = NewPIUnitName_textBox->Text;
				}
				else
				{
					m_pImageomposerIRM->AddObject(l_pUIImage);
					AllImage_listBox->Items->Add(NewPIUnitName_textBox->Text);
				}
			LEAVE:
				this->timer1->Enabled = true;
			}
			else
			{
				WARNING_MSG("please input new PI unit name");
			}

		}
		else
		{
			WARNING_MSG("data error");
		}
	}
	System::Void cPIEditor::NewPIUnitStartX_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		//if( AllImage_listBox->SelectedIndex == -1 || m_bAvoidDataAssignForPIUintChild )
		//{
		   //return;
		//}
		//cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(AllImage_listBox->SelectedIndex));
		//if( !l_pUIImage->m_pEditorAttachParent )
		   // return;
		//POINT	l_RightDownStripOffPos = l_pUIImage->GetRightDownStripOffPos();
		//Vector3	l_vPos = l_pUIImage->GetPos();
		//l_RightDownStripOffPos.x = (int)(NewPIUnitEndX_numericUpDown->Value-NewPIUnitStartX_numericUpDown->Value);	
		//l_RightDownStripOffPos.y = (int)(NewPIUnitEndY_numericUpDown->Value-NewPIUnitStartY_numericUpDown->Value);
		//l_vPos.x = (float)NewPIUnitStartX_numericUpDown->Value;
		//l_vPos.y = (float)NewPIUnitStartY_numericUpDown->Value;
		//l_pUIImage->SetRightDownStripOffPos(l_RightDownStripOffPos);
		//l_pUIImage->SetPos(l_vPos);
		//l_RightDownStripOffPos.x += 1;
		//l_RightDownStripOffPos.y += 1;
		//l_pUIImage->SetOriginalImageSize(l_RightDownStripOffPos);
	}
	System::Void cPIEditor::floatToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		static	bool	l_bDock = false;
		HWND	l_pHwnd = (HWND)this->Handle.ToPointer();
		//HWND	l_pHwnd = FindMyTopMostWindow();
		if (!this->m_pParentHandle)
		{
			//HWND	l_pThisHwnd = GetConsoleHwnd();
			//m_pParentHandle = GetParent(l_pThisHwnd);
			//DWORD pid = GetCurrentProcessId();
			//HANDLE hProcess2 = OpenProcess(0, FALSE, pid);
			//HANDLE hProcess2 = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			//m_pParentHandle = GetParent((HWND)hProcess2);
			//m_pParentHandle = GetParent((HWND)GetCurrentProcess());
			//m_pParentHandle = (HWND)this->Parent->Handle.ToPointer();
			//HWND	l_Hwnd2 = FindMyTopMostWindow();
			//m_pParentHandle = GetParent(l_Hwnd2);

			//m_pParentHandle = FindMyWindowByID(System::Int32::Parse(this->m_strParentProcessID));
			//EnumWindows(EnumWindowProc, 0);
			//m_pParentHandle = hwndMain;
			m_pParentHandle = GetParent(l_pHwnd);
			if (m_pParentHandle)
			{
				WARNING_MSG("find");
			}
			else
				WARNING_MSG("not find");
		}
		if (l_bDock)
		{
			SetParent(l_pHwnd, m_pParentHandle);
			l_bDock = false;
			WARNING_MSG("Dock");
		}
		else
		{
			SetParent(l_pHwnd, 0);
			l_bDock = true;
			WARNING_MSG("Float");
		}
	}
	System::Void cPIEditor::fileToolStripMenuItem_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		openXMLToolStripMenuItem->Owner->Show();
	}
	System::Void cPIEditor::addImagesByFolderToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		addImagesByFolderToolStripMenuItem->Checked = !addImagesByFolderToolStripMenuItem->Checked;
	}
	System::Void cPIEditor::TriangulatorMouseBehavior_comboBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator && TriangulatorMouseBehavior_comboBox->SelectedIndex != -1)
		{
			m_pCurrentSelectedPuzzleImageUnitTriangulator->SetPointsToTriangulatorType((ePointsToTriangulatorType)TriangulatorMouseBehavior_comboBox->SelectedIndex);
		}
	}

	System::Void cPIEditor::ImageTriangulatorLOD_numericUpDown_ValueChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (this->m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			if (m_pCurrentSelectedPuzzleImageUnitTriangulator->GetLOD() != (int)ImageTriangulatorLOD_numericUpDown->Value)
				m_pCurrentSelectedPuzzleImageUnitTriangulator->SetLOD((int)ImageTriangulatorLOD_numericUpDown->Value, false);
			ImageTriangulator_textBox->Text = gcnew String(m_pCurrentSelectedPuzzleImageUnitTriangulator->GetInfo().c_str());
		}
	}
	System::Void cPIEditor::AnimationData_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (AnimationData_listBox->SelectedIndex != -1 && m_pImageIndexOfAnimation)
		{
			AnimationTime_numericUpDown->Value = (System::Decimal)m_pImageIndexOfAnimation->m_ImageAnimationDataList[AnimationData_listBox->SelectedIndex].fTimeGap;
		}
	}
	System::Void cPIEditor::tabControl1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (tabControl1->SelectedIndex == 2)
		{
			if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
				ImageTriangulator_textBox->Text = gcnew String(m_pCurrentSelectedPuzzleImageUnitTriangulator->GetInfo().c_str());
		}
	}
	System::Void cPIEditor::generateTriangulatorImagesToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e)
	{
		auto l_strDirectory = DNCT::SelectDirectory();
		if (l_strDirectory)
		{
			if (m_pPuzzleImageUnitTriangulatorManager)
			{
				int l_iCount = AllImage_listBox->Items->Count;
				for (int i = 0; i < l_iCount; ++i)
				{
					cUIImage*l_pUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(i));
					auto l_pObject = m_pPuzzleImageUnitTriangulatorManager->GetObject(l_pUIImage);
					std::vector<Vector3>*l_pVector = l_pObject->GetTriangulatorPointsVector();
					if (l_pVector->size() > 3)
					{
						cUIImage*l_pUIImage = l_pObject->GetTargetImage();
						System::Drawing::Bitmap^l_pBitMap = nullptr;
						//if (l_pUIImage->m_pEditorAttachParent)
						//{
						//	l_pBitMap = (System::Drawing::Bitmap^)m_ImageTale[gcnew String(l_pUIImage->m_pEditorAttachParent->GetName())];
						//}
						//else
						{
							l_pBitMap = (System::Drawing::Bitmap^)m_ImageTale[AllImage_listBox->Items[i]->ToString()];
						}
						if (l_pBitMap)
						{
							List<System::Drawing::Point>^l_pPointList = Vector3ToListPoint(l_pVector);
							Image^l_pImage = (Image^)l_pBitMap;
							Bitmap^l_pFinalImage = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList);
							String^l_strFileName = l_strDirectory;
							l_strFileName += gcnew String(l_pUIImage->GetName());
							l_strFileName += ".png";
							l_pFinalImage->Save(l_strFileName);
						}
						else
						{
							int a = 0;
						}
					}
				}
			}
		}
	}

	System::Void cPIEditor::MorphingAnimationTime_listBox_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditSetCurrentListboxTime(MorphingAnimationTime_listBox->SelectedIndex);
		}
	}

	System::Void cPIEditor::AddTime_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			float l_fTime = (float)MorphingAnimationTime_numericUpDown->Value;
			if (!m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditAddListboxTime(l_fTime, MorphingAddKeyTimeUsePreKeyData_checkBox->Checked))
			{
				WARNING_MSG("add time failed");
			}
			else
			{
				this->MorphingAnimationTime_listBox->Items->Add(l_fTime.ToString());
				int l_iMax = (int)(m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetEndTime() * 1000);
				MorphingAnimation_trackBar->Maximum = l_iMax;
			}
		}
	}

	System::Void cPIEditor::RearrangeMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			float l_fTime = (float)RearrangeMorphingAnimationTime_numericUpDown->Value;
			if (!m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditRearrangeTime(l_fTime))
			{
				WARNING_MSG("Rearrange Time Failed");
			}
			else
			{
				auto l_pTimeVector = m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetListboxTimeVector();
				for (int i = 0; i < MorphingAnimationTime_listBox->Items->Count; ++i)
				{
					MorphingAnimationTime_listBox->Items[i] = (*l_pTimeVector)[i].ToString();
				}
				this->MorphingAnimation_trackBar->Maximum = (int)(m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetEndTime() * 1000);
			}
		}
	}
	System::Void cPIEditor::ChangeMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e) 
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator && MorphingAnimationTime_listBox->SelectedIndex != -1)
		{
			this->timer1->Enabled = false;
			float l_fTime = (float)MorphingAnimationTime_numericUpDown->Value;
			if (m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditChangeListboxTime(MorphingAnimationTime_listBox->SelectedIndex, l_fTime))
			{
				this->MorphingAnimationTime_listBox->Items[MorphingAnimationTime_listBox->SelectedIndex] = l_fTime.ToString();
				this->MorphingAnimation_trackBar->Maximum = (int)(m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetEndTime() * 1000);
			}
			else
			{
				WARNING_MSG("time must smaller than next and bigger than prevoius one");
			}
			this->timer1->Enabled = true;
		}
	}

	System::Void cPIEditor::InsertMorphingTime_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator && MorphingAnimationTime_listBox->SelectedIndex != -1)
		{
			this->timer1->Enabled = false;
			float l_fTime = (float)MorphingAnimationTime_numericUpDown->Value;
			if (m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditInsertTime(MorphingAnimationTime_listBox->SelectedIndex+1, l_fTime,this->MorphingAddKeyTimeUsePreKeyData_checkBox->Checked))
			{
				this->MorphingAnimationTime_listBox->Items->Insert(MorphingAnimationTime_listBox->SelectedIndex+1,l_fTime.ToString());
				this->MorphingAnimation_trackBar->Maximum = (int)(m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditGetEndTime() * 1000);
			}
			else
			{
				WARNING_MSG("time must smaller than next and bigger than prevoius one");
			}
			this->timer1->Enabled = true;
		}
	}

	System::Void cPIEditor::CenterOffsetPosX_numericUpDown_ValueChanged(System::Object^ sender, System::EventArgs^ e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			Vector2 l_vCenterOffset((float)CenterOffsetPosX_numericUpDown->Value, (float)CenterOffsetPosY_numericUpDown->Value);
			m_pCurrentSelectedPuzzleImageUnitTriangulator->SetCenterOffset(l_vCenterOffset);
		}
	}

	System::Void cPIEditor::OffsetCenter_button_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			auto l_vCenter = m_pCurrentSelectedPuzzleImageUnitTriangulator->CalculateCenter();
			CenterOffsetPosX_numericUpDown->Value = (int)l_vCenter.x;
			CenterOffsetPosY_numericUpDown->Value = (int)l_vCenter.y;
		}
	}
	System::Void cPIEditor::DividImageTool_button_Click(System::Object^ sender, System::EventArgs^ e)
	{

		//DivideToolRow_numericUpDown
		//DivideToolColumn_numericUpDown
		if (AllImage_listBox->SelectedItem)
		{
			auto l_strPrefixName = DividImagePrefixName_textBox->Text;
			auto l_pSelectedObject = m_pImageomposerIRM->GetUIImage(DNCT::GcStringToWchar(AllImage_listBox->SelectedItem->ToString()).c_str());
			this->timer1->Enabled = false;
			int	l_iOriginalIndex = -1;
			int	l_iCollideIndex = AllImage_listBox->SelectedIndex;
			cUIImage* l_pAttachUIImage = dynamic_cast<cUIImage*>(m_pImageomposerIRM->GetObject(l_iCollideIndex));
			cUIImage* l_pUIImage = nullptr;
			System::Drawing::Bitmap^ l_pBitMap = (System::Drawing::Bitmap^)m_ImageTale[gcnew String(l_pAttachUIImage->GetName())];
			if (l_pSelectedObject)
			{
				//int l_iTargetWidth = (int)l_pSelectedObject->GetWidth();
				int l_iNumNewRowObject = (int)DivideToolRow_numericUpDown->Value;
				int l_iNumNewColumnObject = (int)DivideToolColumn_numericUpDown->Value;
				int l_iTargetWidth = l_pSelectedObject->GetWidth() / l_iNumNewColumnObject;
				int l_iTargetHeight = l_pSelectedObject->GetHeight() / l_iNumNewRowObject;
				if (l_iTargetWidth&& l_iTargetHeight)
				{
					int l_iIndex = 0;
					for (int i = 0; i < l_iNumNewRowObject; ++i)
					{
						int l_iStartY = l_iTargetHeight * i;
						for (int j = 0; j < l_iNumNewColumnObject; ++j)
						{
							int l_iStartX = l_iTargetWidth * j;
							l_iIndex = (j * l_iNumNewRowObject) + i;
							String^ l_strNewImageName = l_strPrefixName + l_iIndex.ToString();
							{
								std::vector<Vector2> l_Vector =
								{
									Vector2(l_iStartX,l_iStartY),
									Vector2(l_iStartX+l_iTargetWidth,l_iStartY),
									Vector2(l_iStartX,l_iStartY+l_iTargetHeight),
									Vector2(l_iStartX,l_iStartY+l_iTargetHeight),
									Vector2(l_iStartX+l_iTargetWidth,l_iStartY),
									Vector2(l_iStartX+l_iTargetWidth,l_iStartY + l_iTargetHeight)
								};
								List<System::Drawing::Point>^ l_pPointList = Vector2ToListPoint(&l_Vector);
								Image^ l_pImage = (Image^)l_pBitMap;
								Bitmap^ l_pNew = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList, true);
								l_pUIImage = cPIEditor::GetNewUIImageByBitMap(l_pNew, DNCT::GcStringToWchar(l_strNewImageName).c_str());
								m_ImageTale[l_strNewImageName] = l_pNew;
							}
							l_pUIImage->m_vEditorAttachParentRelativePos = Vector3((float)l_iStartX, (float)l_iStartY, 0);
							l_pUIImage->m_pEditorAttachParent = l_pAttachUIImage;
							l_pUIImage->SetPos(Vector2(l_iStartX, l_iStartY) + l_pAttachUIImage->GetPos());
							////it could be replaced,recheck index again
							l_iOriginalIndex = AllImage_listBox->Items->IndexOf(l_strNewImageName);
							if (l_iOriginalIndex != -1)
							{
								(*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
								AllImage_listBox->Items[l_iOriginalIndex] = l_strNewImageName;
							}
							else
							{
								m_pImageomposerIRM->AddObject(l_pUIImage);
								AllImage_listBox->Items->Add(l_strNewImageName);
							}
						}
					}
				}

				//if (l_iNumNewObject)
				//{
				//	for (int i = 0; i < l_iNumNewObject; ++i)
				//	{
				//		String^ l_strNewImageName = l_strPrefixName + i.ToString();
				//		int l_iStartY = l_iTargetHeight * i;
				//		{
				//			std::vector<Vector2> l_Vector =
				//			{
				//				Vector2(0,l_iStartY),
				//				Vector2(l_iTargetWidth,l_iStartY),
				//				Vector2(0,l_iStartY + l_iTargetHeight),
				//				Vector2(0,l_iStartY + l_iTargetHeight),
				//				Vector2(l_iTargetWidth,l_iStartY),
				//				Vector2(l_iTargetWidth,l_iStartY + l_iTargetHeight)
				//			};
				//			List<System::Drawing::Point>^ l_pPointList = Vector2ToListPoint(&l_Vector);
				//			Image^ l_pImage = (Image^)l_pBitMap;
				//			Bitmap^ l_pNew = GetSelectedArea(l_pImage, Color::Transparent, l_pPointList, true);
				//			l_pUIImage = cPIEditor::GetNewUIImageByBitMap(l_pNew, DNCT::GcStringToWchar(l_strNewImageName).c_str());
				//			m_ImageTale[l_strNewImageName] = l_pNew;
				//		}
				//		l_pUIImage->m_vEditorAttachParentRelativePos = Vector3(0, (float)l_iStartY, 0);
				//		l_pUIImage->m_pEditorAttachParent = l_pAttachUIImage;
				//		l_pUIImage->SetPos(Vector2(0, l_iStartY) + l_pAttachUIImage->GetPos());
				//		////it could be replaced,recheck index again
				//		l_iOriginalIndex = AllImage_listBox->Items->IndexOf(l_strNewImageName);
				//		if (l_iOriginalIndex != -1)
				//		{
				//			(*m_pImageomposerIRM->GetList())[l_iOriginalIndex] = l_pUIImage;
				//			AllImage_listBox->Items[l_iOriginalIndex] = l_strNewImageName;
				//		}
				//		else
				//		{
				//			m_pImageomposerIRM->AddObject(l_pUIImage);
				//			AllImage_listBox->Items->Add(l_strNewImageName);
				//		}
				//	}
				//}
			}
			this->timer1->Enabled = true;
		}
	}
	
	System::Void cPIEditor::generateSelectedPIUnitXMLToolStripMenuItem_Click(System::Object^ sender, System::EventArgs^ e)
	{
		
		auto l_strFileName = DNCT::SaveFileAndGetName("xml files (*.xml)|*.xml|All files (*.*)|*.*");
		if (l_strFileName)
		{
			int l_iCount = AllImage_listBox->SelectedItems->Count;
			if (l_iCount)
			{
				TiXmlDocument l_Doc;
				TiXmlElement* l_pRoot = new TiXmlElement(L"Resource");

				for each (auto l_SelectedItem in AllImage_listBox->SelectedItems)
				{
					//<cPuzzleImageUnit PI="" PIUnit="" Name="" Pos="" Color=""/>
					TiXmlElement* l_pPIElement = new TiXmlElement(cPuzzleImageUnit::TypeID);
					l_pPIElement->SetAttribute(L"PI", DNCT::GcStringToWchar(this->Text).c_str());
					l_pPIElement->SetAttribute(L"PIUnit", DNCT::GcStringToWchar(l_SelectedItem->ToString()).c_str());
					l_pRoot->LinkEndChild(l_pPIElement);
				}
				l_Doc.LinkEndChild(l_pRoot);				
				l_Doc.SaveFile(DNCT::GcStringToChar(l_strFileName).c_str());
			}
		}
	}


	System::Void cPIEditor::DeleteTime_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (MorphingAnimationTime_listBox->SelectedIndex != -1)
		{
			this->timer1->Enabled = false;
			m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditDeleteListboxTime(MorphingAnimationTime_listBox->SelectedIndex);
			this->MorphingAnimationTime_listBox->Items->RemoveAt(MorphingAnimationTime_listBox->SelectedIndex);
			this->timer1->Enabled = true;
		}
	}

	System::Void cPIEditor::EditAnimation_checkBox_CheckedChanged(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditApplyEmptyAnimationData();
			LODToPoints_button->Enabled = !EditAnimation_checkBox->Checked;
			ImageTriangulatorLOD_numericUpDown->Enabled = !EditAnimation_checkBox->Checked;
			PlayMorphingAnimation_checkBox->Enabled = EditAnimation_checkBox->Checked;
			if (!PlayMorphingAnimation_checkBox->Enabled)
			{
				PlayMorphingAnimation_checkBox->Checked = false;
			}
		}
	}

	System::Void cPIEditor::LODToPoints_button_Click(System::Object^  sender, System::EventArgs^  e)
	{
		if (m_pCurrentSelectedPuzzleImageUnitTriangulator)
		{
			WARING_YES_NO_TO_YES("this object animation will be erase!Are you SURE?")
			{
				m_pCurrentSelectedPuzzleImageUnitTriangulator->LODToPoints();
				m_pCurrentSelectedPuzzleImageUnitTriangulator->MorphingEditDataCleanUp();
				MorphingAnimationTime_listBox->Items->Clear();
				ImageTriangulatorLOD_numericUpDown->Value = 1;
			}
		}
	}

	System::Void cPIEditor::MorphingAnimation_trackBar_Scroll(System::Object^  sender, System::EventArgs^  e)
	{
		MorphintTime_label->Text = MorphingAnimation_trackBar->Value.ToString()+"/"+ MorphingAnimation_trackBar->Maximum.ToString();
	}

	System::Void cPIEditor::MorphingAnimation_trackBar_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e)
	{
		this->PlayMorphingAnimation_checkBox->Checked = false;
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