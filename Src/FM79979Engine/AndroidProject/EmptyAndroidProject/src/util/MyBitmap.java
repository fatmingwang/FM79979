package util;

import android.graphics.Bitmap;
import java.io.IOException;
import java.io.File;
import android.net.Uri;
import android.graphics.BitmapFactory;
import android.content.Intent;
import android.app.Activity;
import android.os.Environment;
import java.io.FileOutputStream;
import android.view.View;
import android.util.Log;

public class MyBitmap
{
	
	public Bitmap	JpegToBitmap(byte[] e_pJpegData,int e_iFileLength)
	{
		BitmapFactory.Options opt = new BitmapFactory.Options();
		opt.inDither = true;
		opt.inPreferredConfig = Bitmap.Config.ARGB_8888;
		return BitmapFactory.decodeByteArray(e_pJpegData, 0, e_iFileLength,opt);
	}

	public void	RecycleBitmp( Bitmap e_pBitmap )
	{
		if( e_pBitmap != null )
		{
			e_pBitmap.recycle();
		}
	}

	public static void	JpegToFile(int[] e_pJpegData,int e_iWidth,int e_iHeight,String e_strFileName)
	{
		Bitmap l_OutputBitmap = Bitmap.createBitmap(e_pJpegData,e_iWidth,e_iHeight,Bitmap.Config.ARGB_8888);
		try 
		{
			l_OutputBitmap.compress(Bitmap.CompressFormat.JPEG, 100, new FileOutputStream(new File(e_strFileName)));
		}
		catch (Throwable e) 
		{
			// Several error may come out with file handling or DOM
			Log.d("GameApp", "GameAppJAVA:",e);
			e.printStackTrace();
		}

	}

	public static void openScreenshot(File imageFile,Activity e_Activity)
	{
		Intent intent = new Intent();
		intent.setAction(Intent.ACTION_VIEW);
		Uri uri = Uri.fromFile(imageFile);
		intent.setDataAndType(uri, "image");
		e_Activity.startActivity(intent);
	}
	//https://stackoverflow.com/questions/2661536/how-to-programmatically-take-a-screenshot
	//not for opengl
	public static void takeScreenshot(String e_strFileName,Activity e_Activity)
	{
		//Date now = new Date();
		//android.text.format.DateFormat.format("yyyy-MM-dd_hh:mm:ss", now);
		try 
		{
			// image naming and path  to include sd card  appending name you choose for file
			//String mPath = "sdcard/"+e_strFileName + ".jpg";
			String mPath = e_strFileName;

			// create bitmap screen capture
			View v1 = e_Activity.getWindow().getDecorView().getRootView();
			v1.setDrawingCacheEnabled(true);
			Bitmap bitmap = Bitmap.createBitmap(v1.getDrawingCache());
			v1.setDrawingCacheEnabled(false);

			File imageFile = new File(mPath);

			FileOutputStream outputStream = new FileOutputStream(imageFile);
			int quality = 100;
			bitmap.compress(Bitmap.CompressFormat.JPEG, quality, outputStream);
			outputStream.flush();
			outputStream.close();
			//
			//openScreenshot(imageFile);
		}
		catch (Throwable e) 
		{
			// Several error may come out with file handling or DOM
			e.printStackTrace();
		}
	}


}