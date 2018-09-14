/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
 //15.0
 //google-play-services-basement.jar
 //google-play-services-vision.jar
 //google-play-services-vision-common.jar
 package util.QRCode; 


import android.graphics.Bitmap;
import android.content.Context;


import com.google.android.gms.vision.Frame;
import com.google.android.gms.vision.barcode.BarcodeDetector;
import com.google.android.gms.vision.barcode.Barcode;
import android.util.SparseArray;

import util.Define;

/**
 * This class does all the work for setting up and managing Bluetooth
 * connections with other devices. It has a thread that listens for
 * incoming connections, a thread for connecting with a device, and a
 * thread for performing data transmissions when connected.
 */
public class GoogleMobileVisionQRCode
{
	public  static native void		QRCodeResult(String e_strText);
	private static boolean	m_bWaitQRCodeResult = false;

	public static void	DecodeQRCodePixelsWithGoogleVisionWithThread(int[] e_pJpegData,int e_iWidth,int e_iHeight)
	{
		if(m_bWaitQRCodeResult)
			return;
		m_bWaitQRCodeResult = true;
		final Bitmap l_OutputBitmap = Bitmap.createBitmap(e_pJpegData,e_iWidth,e_iHeight,Bitmap.Config.ARGB_8888);
		final Context l_Context = Define.m_TempContext;
		Thread l_Thread = new Thread
		(
			new Runnable() 
			{
				public void run() 
				{
					String l_strQRCodeResult = "";
					BarcodeDetector.Builder l_Builder = new BarcodeDetector.Builder(l_Context);
					BarcodeDetector l_BarcodeDetector = l_Builder.setBarcodeFormats(Barcode.QR_CODE).build();
					if (l_BarcodeDetector.isOperational())
					{
						Frame myFrame = new Frame.Builder().setBitmap(l_OutputBitmap).build();
						SparseArray<Barcode> barcodes = l_BarcodeDetector.detect(myFrame);                
						// Check if at least one barcode was detected
						for(int i=0;i<barcodes.size();++i)
						{
							l_strQRCodeResult += barcodes.valueAt(i).toString();;
						}
					}
					else
					{
						//l_strQRCodeResult = getString(R.string.barcode_service_down);
						l_strQRCodeResult = "";
					}
					l_BarcodeDetector.release();
					//String l_strResult = readQRCode(l_OutputBitmap);
					QRCodeResult(l_strQRCodeResult);
					l_OutputBitmap.recycle();
					m_bWaitQRCodeResult = false;
				}	
			}
		);
		l_Thread.start();
	}
}
