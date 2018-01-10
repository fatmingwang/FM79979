#ifdef WIN32
//http://blog.csdn.net/whisperLin/archive/2009/12/15/5013251.aspx
#ifndef _avi_utils_H
#define _avi_utils_H
// AVI utilities -- for creating avi files
// (c) 2002 Lucian Wischik. No restrictions on use.
#include "Vfw.h"

DECLARE_HANDLE(HAVI);
// An HAVI identifies an avi file that is being created


HAVI CreateAvi(const wchar_t*fn, int frameperiod, const WAVEFORMATEX *wfx);
// CreateAvi - call this to start the creation of the avi file.
// The period is the number of ms between each bitmap frame.
// The waveformat can be null if you're not going to add any audio,
// or if you're going to add audio from a file.

HRESULT AddAviFrame(HAVI avi, HBITMAP hbm);

HRESULT AddAviAudio(HAVI avi, void *dat, unsigned long numbytes);
// AddAviFrame - adds this bitmap to the avi file. hbm must point be a DIBSection.
// It is the callers responsibility to free the hbm.
// AddAviAudio - adds this junk of audio. The format of audio was as specified in the
// wfx parameter to CreateAVI. This fails if nullptr was given.
// Both return S_OK if okay, otherwise one of the AVI errors.

HRESULT AddAviWav(HAVI avi, const char *wav, unsigned long flags);
// AddAviWav - a convenient way to add an entire wave file to the avi.
// The wav file may be in in memory (in which case flags=SND_MEMORY)
// or a file on disk (in which case flags=SND_FILENAME).
// This function requires that either a null WAVEFORMATEX was passed to CreateAvi,
// or that the wave file now being added has the same format as was
// added earlier.

HRESULT SetAviVideoCompression(HAVI avi, HBITMAP hbm, AVICOMPRESSOPTIONS *opts, bool ShowDialog, HWND hparent);
// SetAviVideoCompression - allows compression of the video. If compression is desired,
// then this function must have been called before any bitmap frames had been added.
// The bitmap hbm must be a DIBSection (so that avi knows what format/size you're giving it),
// but won't actually be added to the movie.
// This function can display a dialog box to let the user choose compression. In this case,
// set ShowDialog to true and specify the parent window. If opts is non-nullptr and its
// dwFlags property includes AVICOMPRESSF_VALID, then opts will be used to give initial
// values to the dialog. If opts is non-nullptr then the chosen options will be placed in it.
// This function can also be used to choose a compression without a dialog box. In this
// case, set ShowDialog to false, and hparent is ignored, and the compression specified
// in 'opts' is used, and there's no need to call GotAviVideoCompression afterwards.

HRESULT CloseAvi(HAVI avi);
// CloseAvi - the avi must be closed with this message.



unsigned int FormatAviMessage(HRESULT code, char *buf,unsigned int len);
// FormatAviMessage - given an error code, formats it as a string.
// It returns the length of the error message. If buf/len points
// to a real buffer, then it also writes as much as possible into there.


#endif

#endif

//Examples of how to create AVIs
//To create an AVI from a bunch of files. The following code creates an AVI out of five bitmaps (1.bmp, 2.bmp, ... 5.bmp) and a WAV file (wav.wav).
//
//const char *fns[] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp"};
//HAVI avi = CreateAvi("test.avi",1000,nullptr); // 1000ms is the period between frames
//for (int i=0; i<sizeof(fns)/sizeof(fns[0]); i++)
//{ HBITMAP hbm=(HBITMAP)LoadImage(nullptr,fns[i],IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
//  AddAviFrame(avi,hbm);
//  DeleteObject(hbm);
//}
//AddAviWav(avi,"wav.wav",SND_FILENAME);
//CloseAvi(avi);
//To create an AVI with DivX compression. This code creates the same AVI, with DivX compression for the video, and without the audio. The code actually just suggests DivX and pops up a dialog to let the user control more details (hwnd is some window to be used as the parent for that dialog). You might keep a copy of the opts structure so as to give the same defaults to the user next time. Or you can pass false to the SetAviVideoCompression function to suppress the user-interface dialog, if the opts structure is already filled out correctly.
//
//const char *fns[] = {"1.bmp","2.bmp","3.bmp","4.bmp","5.bmp"};
//HAVI avi = CreateAvi("test.avi",1000,nullptr);
//for (int i=0; i<sizeof(fns)/sizeof(fns[0]); i++)
//{ HBITMAP hbm=(HBITMAP)LoadImage(nullptr,fns[i],IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
//  if (i==0) // Set up compression just before the first frame
//  { AVICOMPRESSOPTIONS opts; ZeroMemory(&opts,sizeof(opts));
//    opts.fccHandler=mmioFOURCC('d','i','v','x');
//    SetAviVideoCompression(avi,hbm,&opts,true,hwnd);
//  }
//  AddAviFrame(avi,hbm);
//  DeleteObject(hbm);
//}
//CloseAvi(avi);
//To create an AVI from dynamically-drawn images. This code shows how to draw your own animations for the AVI file, rather than just relying on existing bitmap files. Note: you must use DIBSections instead of normal device-dependent bitmaps.
//
//HDC hdcscreen=GetDC(0), hdc=CreateCompatibleDC(hdcscreen); ReleaseDC(0,hdcscreen);
//BITMAPINFO bi; ZeroMemory(&bi,sizeof(bi)); BITMAPINFOHEADER &bih = bi.bmiHeader;
//bih.biSize=sizeof(bih);
//bih.biWidth=200;
//bih.biHeight=200;
//bih.biPlanes=1;
//bih.biBitCount=24;
//bih.biCompression=BI_RGB;
//bih.biSizeImage = ((bih.biWidth*bih.biBitCount/8+3)&0xFFFFFFFC)*bih.biHeight;
//bih.biXPelsPerMeter=10000;
//bih.biYPelsPerMeter=10000;
//bih.biClrUsed=0;
//bih.biClrImportant=0;
//void *bits; HBITMAP hbm=CreateDIBSection(hdc,(BITMAPINFO*)&bih,DIB_RGB_COLORS,&bits,nullptr,nullptr);
//
//HGDIOBJ holdb=SelectObject(hdc,hbm);
//HPEN hp = CreatePen(PS_SOLID,16,RGB(255,255,128));
//HGDIOBJ holdp=SelectObject(hdc,hp);
//
//HAVI avi = CreateAvi("test.avi",100,nullptr);
//for (int frame=0; frame<50; frame++)
//{ // static background
//  unsigned long seed=GetTickCount(); unsigned long *dbits=(unsigned long*)bits;
//  for (unsigned int i=0; i<bih.biSizeImage/sizeof(unsigned long); i++) {dbits[i]=seed; seed+=79;}
//   a line moving
//  MoveToEx(hdc,0,0,nullptr); LineTo(hdc,frame*3,100);
//  AddAviFrame(avi,hbm);
//}
//CloseAvi(avi);
//
//SelectObject(hdc,holdb); SelectObject(hdc,holdp);
//DeleteDC(hdc); DeleteObject(hbm); DeleteObject(hp);