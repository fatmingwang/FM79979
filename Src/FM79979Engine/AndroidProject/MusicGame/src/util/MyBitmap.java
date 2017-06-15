package util;

import android.graphics.Bitmap;
import java.io.IOException;
import android.graphics.BitmapFactory;
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
}