package util.QRCode; 

import android.util.Log;
import android.graphics.Bitmap;
import java.util.Hashtable;

//zxing3.3.0.jar
import com.google.zxing.DecodeHintType;
import com.google.zxing.BarcodeFormat;
import com.google.zxing.BinaryBitmap;
import com.google.zxing.EncodeHintType;
import com.google.zxing.MultiFormatReader;
import com.google.zxing.MultiFormatWriter;
import com.google.zxing.NotFoundException;
import com.google.zxing.Result;
import com.google.zxing.WriterException;
import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
import com.google.zxing.client.j2se.MatrixToImageWriter;
import com.google.zxing.common.BitMatrix;
import com.google.zxing.common.HybridBinarizer;
import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;
import com.google.zxing.RGBLuminanceSource;
import com.google.zxing.Reader;
import com.google.zxing.ChecksumException;
import com.google.zxing.FormatException;
import com.google.zxing.qrcode.QRCodeReader;

public class ZxingQRCode
{
	public  static native void		QRCodeResult(String e_strText);
	private static boolean			m_bWaitQRCodeResult = false;
	public static void ZxingDecodeByPixelreadQRCode(int[] e_pJpegData,int e_iWidth,int e_iHeight)
	{
		if(m_bWaitQRCodeResult)
			return;
		m_bWaitQRCodeResult = true;
		final Bitmap l_OutputBitmap = Bitmap.createBitmap(e_pJpegData,e_iWidth,e_iHeight,Bitmap.Config.ARGB_8888);
		Thread l_Thread = new Thread
		(
			new Runnable() 
			{
				public void run() 
				{
					String l_strResult = readQRCode(l_OutputBitmap);
					QRCodeResult(l_strResult);
					l_OutputBitmap.recycle();
					m_bWaitQRCodeResult = false;
				}	
			}
		);
		l_Thread.start();
	}

	private static String readQRCode(Bitmap e_Bitmap)
	{
		Bitmap generatedQRCode = e_Bitmap;
		int width = generatedQRCode.getWidth();
		int height = generatedQRCode.getHeight();
		int[] pixels = new int[width * height];
		generatedQRCode.getPixels(pixels, 0, width, 0, 0, width, height);

		RGBLuminanceSource source = new RGBLuminanceSource(width, height, pixels);

		BinaryBitmap binaryBitmap = new BinaryBitmap(new HybridBinarizer(source));

		//Reader reader = new MultiFormatReader();
		Reader reader = new QRCodeReader();
		Result result = null;
		try 
		{
			boolean l_bUseDecodeHint = true;
			if(l_bUseDecodeHint)
			{
				Hashtable<DecodeHintType, Object> decodeHints = new Hashtable<DecodeHintType, Object>();
				decodeHints.put(DecodeHintType.TRY_HARDER, Boolean.TRUE);
				decodeHints.put(DecodeHintType.PURE_BARCODE, Boolean.TRUE);
				result = reader.decode(binaryBitmap,decodeHints);
			}
			else
			{
				result = reader.decode(binaryBitmap);
			}
		}
		catch (NotFoundException e) 
		{
			Log.d("RQCode","NotFoundException",e);

		}
		catch (ChecksumException e) 
		{
			Log.d("RQCode","ChecksumException",e);
		}
		catch (FormatException e)
		{
			Log.d("RQCode","FormatException",e);
		}
		if(result != null)
		{
			//Toast.makeText(this, result.getText(), Toast.LENGTH_LONG).show();
			return result.getText();
		}
		//Toast.makeText(this, "QR code parse failed", Toast.LENGTH_LONG).show();
		return "";
	}
}
