#include "StdAfx.h"
#include "ShowFontPic.h"
typedef DWORD D3DCOLOR;
#define D3DCOLOR_ARGB(a,r,g,b) ((D3DCOLOR)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)))
#define D3DCOLOR_RGBA(r,g,b,a) D3DCOLOR_ARGB(a,r,g,b)
namespace DrawFont {

	System::Void ShowFontPic::SaveBitInfoAndPic()
	{
		if( m_pStringForOutput )
		{
			GCFORM::FolderBrowserDialog^l_pDialog = gcnew GCFORM::FolderBrowserDialog();
			DIALOG_OK(l_pDialog)
			{
				System::String^l_pStringForDirectory = l_pDialog->SelectedPath;
				l_pStringForDirectory+="\\";
				//System::IO::Directory::CreateDirectory(l_pStringForDirectory+"XBOX");
				//System::IO::Directory::CreateDirectory(l_pStringForDirectory+"WIN");
				System::Drawing::Color	l_DestColor;
				System::Drawing::Color	l_MaskColorWithAlpha = System::Drawing::Color::FromArgb(0,g_MaskColor);
				if(l_pStringForDirectory->Length)
				{
					for each ( System::String^l_pString in this->m_pFontInfoList->Keys )
					{							
						cFontInfo^l_pFontInfo = safe_cast<cFontInfo^>(m_pFontInfoList[l_pString]);
						int	l_iWidth = l_pFontInfo->m_pBitMap->Size.Width;
						int l_iHeight = l_pFontInfo->m_pBitMap->Size.Height;
						System::Drawing::Rectangle cloneRect = System::Drawing::Rectangle(0, 0,l_iWidth,l_iHeight);
						System::Drawing::Bitmap ^l_pBitmapForSave = l_pFontInfo->m_pBitMap->Clone(cloneRect,l_pFontInfo->m_pBitMap->PixelFormat);

						int	l_iImageWidth = l_pBitmapForSave->Width;
						for( int i=0;i<l_pBitmapForSave->Width;++i )
						{
							for( int j=0;j<l_pBitmapForSave->Height;++j )
							{
								l_DestColor = l_pBitmapForSave->GetPixel(i,j);
								if( g_MaskColor.R == l_DestColor.R )
								if( g_MaskColor.G == l_DestColor.G )
								if( g_MaskColor.B == l_DestColor.B )
								{

									l_pBitmapForSave->SetPixel(i,j,l_MaskColorWithAlpha);
								}
							}
						}
						System::Drawing::Imaging::ImageFormat^ l_ImageFormat = System::Drawing::Imaging::ImageFormat::Bmp;
						char*l_psFileExtension = ".bmp";
						switch( PicFormat_comboBox->SelectedIndex )
						{
							case 0:
								break;
							case 1:
								l_ImageFormat = System::Drawing::Imaging::ImageFormat::Gif;
								l_psFileExtension = ".gif";
								break;
							case 2:
								l_ImageFormat = System::Drawing::Imaging::ImageFormat::Jpeg;
								l_psFileExtension = ".jpeg";
								break;
							case 3:
								l_ImageFormat = System::Drawing::Imaging::ImageFormat::Png;
								l_psFileExtension = ".png";
								break;
							case 4:
								l_ImageFormat = System::Drawing::Imaging::ImageFormat::Tiff;
								l_psFileExtension = ".tiff";
								break;
						
						}

						l_pBitmapForSave->Save(l_pStringForDirectory+l_pString+TO_GCSTRING(l_psFileExtension),l_ImageFormat);
						GlyphWriter lGlyph;
						WCHAR	l_First =  this->m_pStringForOutput[0];
						WCHAR	l_End =  this->m_pStringForOutput[m_pStringForOutput->Length-1];						
						lGlyph.SetFontInfo(l_pFontInfo->m_pFont->Height,
							D3DCOLOR_RGBA(this->g_MaskColor.R,g_MaskColor.G,g_MaskColor.B,g_MaskColor.A),
							(DWORD)l_pFontInfo->m_GlypgAttriList->Count,this->m_pStringForOutput[m_pStringForOutput->Length-1]);
						for(int i=0;i<l_pFontInfo->m_GlypgAttriList->Count;i++)
						{
							cFontInfo::cGlyph^l_pGlyph = (cFontInfo::cGlyph^)l_pFontInfo->m_GlypgAttriList[i];
							lGlyph.SetGlyphAttr(i,l_pGlyph->fLeft,l_pGlyph->fTop,l_pGlyph->fRight,l_pGlyph->fBottom,l_pGlyph->fWidth,l_pGlyph->fHeight);
						}
						WCHAR	*l_AllString = new WCHAR[this->m_pStringForOutput->Length+1];
						for( int i=0;i<m_pStringForOutput->Length;i++ )
							l_AllString[i] = m_pStringForOutput[i];
						l_AllString[m_pStringForOutput->Length] = '\0';
						lGlyph.ExtractValidGlyphsFromWcharPointer( l_AllString );
						delete[] l_AllString;
						//debug text
						//DNCT::StreamWriteFileFromString(l_pFontInfo->m_psCharacterInfo,l_pStringForDirectory+l_pString+".txt");
						char	l_temp[MAX_PATH];
						//sprintf_s(l_temp,MAX_PATH,"%s\0",l_pStringForDirectory+"Win\\"+l_pString+".FontInfo");
						sprintf_s(l_temp,MAX_PATH,"%s\0",l_pStringForDirectory+l_pString+".FontInfo");
						lGlyph.SaveFile(l_temp,true);
						//sprintf_s(l_temp,MAX_PATH,"%s\0",l_pStringForDirectory+"XBOX\\"+l_pString+".FontInfo");
						//lGlyph.SaveFile(l_temp,false);
					}
				}
			}
		}
	}








	System::Drawing::Bitmap^	ShowFontPic::GenerateBitMapByString(String^e_pString,System::Drawing::Font^e_pFont,int e_iDistance
																	,int e_iWidth,int e_iHeight,bool e_bSequence,std::vector<FILE_GLYPH_ATTR>*e_pGlyphList)
	{
		e_pGlyphList->clear();
		System::Drawing::Color l_NewColor = g_MaskColor;
		System::Drawing::SolidBrush^ l_pDrawBrush = gcnew System::Drawing::SolidBrush( l_NewColor );
		System::Drawing::SolidBrush^ l_pFontBrush = gcnew System::Drawing::SolidBrush( g_FontColor);
		if( !e_bSequence )
		 {
			 System::Drawing::Size	l_Size = GetSizeFromString(e_pString);
			 System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( l_Size.Width,l_Size.Height);
			 System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
			 l_pGr->FillRectangle(l_pDrawBrush,0,0,l_pBitMap->Width,l_pBitMap->Height);
			 l_pGr->DrawString(e_pString,e_pFont,Brushes::Black,0,0);
			 label5->Text = "";
			 return l_pBitMap;
		 }
		 //for test this one will output a test file by txt
		 m_pForFontPosInfoTemp = "";
		 int	l_iLineJump = 0;//what line is it
		 int	l_iThisLineFirstWordPos = 0;
		 int	l_iPreviousCharsWidth = 0;
		 int	l_iPervoiusCharsHeight = 0;
		 int	l_iFontDis = e_iDistance;
		 System::String^l_pStringForDraw = gcnew System::String(e_pString);

		 System::Drawing::Bitmap^l_pBitMap = gcnew System::Drawing::Bitmap( e_iWidth,e_iHeight );
		 System::Drawing::Graphics^ l_pGr = System::Drawing::Graphics::FromImage(l_pBitMap);
		 if( AntiAlias_checkBox->Checked )
			l_pGr->TextRenderingHint = System::Drawing::Text::TextRenderingHint::AntiAliasGridFit;//:AntiAlias;
		 l_pGr->FillRectangle(l_pDrawBrush,0,0,l_pBitMap->Width,l_pBitMap->Height);
		 int	l_iCurrentCharHeight = e_pFont->Height;
		 int	l_iCurrentCharWidth = 0;
		 bool	l_bOver = false;
		 int	l_iMaxWidth = 0;
		 for(int i=0;i<l_pStringForDraw->Length;++i)
		 {
			 //here for single char
			 wchar_t l_Wchar = l_pStringForDraw[i];
			 System::String^	l_pCurrentGCSTring = l_pStringForDraw->Substring(i,1);
			 
			 l_iCurrentCharWidth = (int)l_pGr->MeasureString(l_pCurrentGCSTring,e_pFont).Width;
			 //there is too much empty pixel,here for save some buffer,but it could be occur many problems,so here neglect it
			 //if( !e_pFont->Italic )
			//	l_iCurrentCharWidth = (int)l_pGr->MeasureString(System::String::Concat(l_pCurrentGCSTring,l_pCurrentGCSTring),e_pFont).Width - l_iCurrentCharWidth;
			 //calculator the width over bitmap size or not
			 if( l_iCurrentCharWidth+l_iPreviousCharsWidth>l_pBitMap->Size.Width-2 )
			 {
				 l_iPreviousCharsWidth = 0;
				 l_iPervoiusCharsHeight+=e_pFont->Height;
			 }			 
			 //left up to right down  char
			 //width  to aviod overlap
			 int l_iLastWidth = l_iCurrentCharWidth+l_iPreviousCharsWidth;
			 int l_iLastHeight = l_iCurrentCharHeight+l_iPervoiusCharsHeight;
			 FILE_GLYPH_ATTR l_FILE_GLYPH_ATTR;
			 l_FILE_GLYPH_ATTR.fLeft = (float)(l_iPreviousCharsWidth)/(float)e_iWidth;
			 l_FILE_GLYPH_ATTR.fTop = (float)l_iPervoiusCharsHeight/(float)e_iHeight;
			 l_FILE_GLYPH_ATTR.fRight = (float)l_iLastWidth/(float)e_iWidth;
			 l_FILE_GLYPH_ATTR.fBottom = (float)l_iLastHeight/(float)e_iHeight;
			 l_FILE_GLYPH_ATTR.fWidth = (float)(l_iLastWidth-(l_iPreviousCharsWidth));
			 l_FILE_GLYPH_ATTR.fHeight = (float)(l_iLastHeight-(l_iPervoiusCharsHeight));
			 e_pGlyphList->push_back(l_FILE_GLYPH_ATTR);
			 //for character pos by pixel
			 WCHAR	l_temp[260];
			 //swprintf_s(l_temp,260,L"%d,%d,%d,%d,%c%c\n\0",l_iPreviousCharsWidth+1,l_iPervoiusCharsHeight,l_iLastWidth,l_iLastHeight,l_Wchar,13,10);
			 //m_pForFontPosInfoTemp+=TO_GCSTRING(l_temp);
			 //for character pos by texel
			 swprintf_s(l_temp,260,L"%f,%f,%f,%f,%f%c%c\0",
				 l_FILE_GLYPH_ATTR.fLeft,
				 l_FILE_GLYPH_ATTR.fTop,
				 l_FILE_GLYPH_ATTR.fRight,
				 l_FILE_GLYPH_ATTR.fBottom,
				 l_FILE_GLYPH_ATTR.fWidth,13,10);
			 m_pForFontPosInfoTemp+=TO_GCSTRING(l_temp);
			 wchar_t l_tempString[2] = { l_Wchar,L'\0' };
			 if( !l_bOver )
			 {
				 if( this->Outline_checkBox->Checked )
				 {
					//http://www.codeproject.com/Articles/42529/Outline-Text#initgdiplus
				 }
				l_pGr->DrawString(TO_GCSTRING(l_tempString),e_pFont,l_pFontBrush,(float)l_iPreviousCharsWidth, (float)l_iPervoiusCharsHeight);
			 }
			 //add one for character distance
			 l_iPreviousCharsWidth = l_iPreviousCharsWidth + l_iCurrentCharWidth+l_iFontDis+1;
			 //to determinal the suit width
			 if( l_iMaxWidth<l_iLastWidth )
				 l_iMaxWidth = l_iLastWidth;

			 if( l_iCurrentCharHeight+l_iPervoiusCharsHeight > l_pBitMap->Size.Height )
			 {//here do not to break,that's because we still get to know suit width and height
				 l_bOver = true;
			 }
		 }
		 if( abs(l_iPervoiusCharsHeight-e_iHeight)  || abs(l_iPreviousCharsWidth - e_iWidth) )
		 {
			char	l_temp[260];			
			label5->Text = "Suitable Size"+DNCT::GetChanglineString()
				+"X="+TO_GCSTRING(itoa(l_iMaxWidth,l_temp,10))
				+DNCT::GetChanglineString()
				+"Y="+ TO_GCSTRING(itoa(l_iCurrentCharHeight+l_iPervoiusCharsHeight,l_temp,10));
		 }
		 return l_pBitMap;
	}

	System::Void	ShowFontPic::SaveFontFile()
	{
		 GCNEW(GCFORM::TreeNode,l_pTreeNode);
		 for( int i=0;i<AllFont_listBox->Items->Count;++i )
		 {
			 l_pTreeNode->Nodes->Add(AllFont_listBox->Items[i]->ToString());
		 }

		 for each(GCFORM::TreeNode^l_pNode in l_pTreeNode->Nodes)
		 {
			 cFontInfo^l_pFontInfo = safe_cast<cFontInfo^>(this->m_pFontInfoList[l_pNode->Text]);
			 l_pNode->Nodes->Add("Bold");
			 l_pNode->Nodes[0]->Nodes->Add(l_pFontInfo->m_bBold?"1":"0");
			 l_pNode->Nodes->Add("Italic");
			 l_pNode->Nodes[1]->Nodes->Add(l_pFontInfo->m_bItalic?"1":"0");
			 l_pNode->Nodes->Add("StrikeOut");
			 l_pNode->Nodes[2]->Nodes->Add(l_pFontInfo->m_bStrikeOut?"1":"0");
			 l_pNode->Nodes->Add("Underline");
			 l_pNode->Nodes[3]->Nodes->Add(l_pFontInfo->m_bUnderline?"1":"0");
			 l_pNode->Nodes->Add("SpaceDis");
			 l_pNode->Nodes[4]->Nodes->Add(l_pFontInfo->m_iSpaceDis.ToString());
			 l_pNode->Nodes->Add("Size");
			 l_pNode->Nodes[5]->Nodes->Add(l_pFontInfo->m_fSize.ToString());
			 l_pNode->Nodes->Add("Name");
			 l_pNode->Nodes[6]->Nodes->Add(l_pFontInfo->m_psFontName);
			 l_pNode->Nodes->Add("Width");
			 l_pNode->Nodes[7]->Nodes->Add(l_pFontInfo->m_pBitMap->Width.ToString());
			 l_pNode->Nodes->Add("Height");
			 l_pNode->Nodes[8]->Nodes->Add(l_pFontInfo->m_pBitMap->Height.ToString());
		 }
		 DNCT::SaveTreeNodeToFile(l_pTreeNode);	
	}

	System::Void	ShowFontPic::OpenFontFile()
	{
		WARING_YES_NO_TO_YES("will lose current data!!")
		{
			this->m_pFontInfoList->Clear();
			AllFont_listBox->Items->Clear();
			GCFORM::TreeNode^l_pTreeNode = DNCT::OpenXmlFileToNode();
			for each (GCFORM::TreeNode^l_pNode in l_pTreeNode->Nodes)
			{				
				bool l_bBold,l_bItalic,l_bStrikeOut,l_bUnderLine;
				int l_iSpaceDis,l_iPicWidth,l_iPicHeight;
				float l_fSize;
				String^l_pName;
				l_pNode->Nodes[0]->Nodes[0]->Text == L"1"?l_bBold = true:l_bBold = false;
				l_pNode->Nodes[1]->Nodes[0]->Text == L"1"?l_bItalic = true:l_bItalic = false;
				l_pNode->Nodes[2]->Nodes[0]->Text == L"1"?l_bStrikeOut = true:l_bStrikeOut = false;
				l_pNode->Nodes[3]->Nodes[0]->Text == L"1"?l_bUnderLine = true:l_bUnderLine = false;
				l_iSpaceDis = System::Convert::ToInt32(l_pNode->Nodes[4]->Nodes[0]->Text);
				l_fSize = (float)System::Convert::ToDouble(l_pNode->Nodes[5]->Nodes[0]->Text);
				l_pName = l_pNode->Nodes[6]->Nodes[0]->Text;
				l_iPicWidth = System::Convert::ToInt32(l_pNode->Nodes[7]->Nodes[0]->Text);;
				l_iPicHeight = System::Convert::ToInt32(l_pNode->Nodes[8]->Nodes[0]->Text);;
				int l_iFontStyle = (int)System::Drawing::FontStyle::Regular;
				if( l_bBold )
					l_iFontStyle+=(int)System::Drawing::FontStyle::Bold;
				if( l_bItalic )
					l_iFontStyle+=(int)System::Drawing::FontStyle::Italic;
				if( l_bStrikeOut )
					l_iFontStyle+=(int)System::Drawing::FontStyle::Strikeout;
				if( l_bUnderLine )
					l_iFontStyle+=(int)System::Drawing::FontStyle::Underline;
				System::Drawing::Font^l_pFont = gcnew System::Drawing::Font(l_pName,l_fSize,(System::Drawing::FontStyle)l_iFontStyle);
				std::vector<FILE_GLYPH_ATTR>	l_GlypgAttriList;
				System::Drawing::Bitmap^l_pBitmap = this->GenerateBitMapByString(this->m_pStringForOutput,l_pFont,l_iSpaceDis,l_iPicWidth,l_iPicHeight,true,&l_GlypgAttriList);
				cFontInfo^l_pFontInfo =gcnew cFontInfo(l_pBitmap,l_pFont,l_iSpaceDis);
				for( UINT i=0;i<l_GlypgAttriList.size();i++ )
				{
					 cFontInfo::cGlyph^l_pGlyph = gcnew cFontInfo::cGlyph(l_GlypgAttriList[i]);
					 l_pFontInfo->m_GlypgAttriList->Add(l_pGlyph);
				}
				AllFont_listBox->Items->Add(l_pNode->Text);
				this->m_pFontInfoList->Add(l_pNode->Text,l_pFontInfo);
			}
		}		
	}

}