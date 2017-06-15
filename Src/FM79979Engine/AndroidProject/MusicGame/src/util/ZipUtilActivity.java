package util;


import net.lingala.zip4j.core.ZipFile;
import net.lingala.zip4j.exception.ZipException;
import net.lingala.zip4j.io.ZipInputStream;
import net.lingala.zip4j.model.FileHeader;
import net.lingala.zip4j.unzip.UnzipUtil;
import net.lingala.zip4j.progress.ProgressMonitor;

import android.content.Intent;
import android.os.Bundle;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.support.v4.app.FragmentManager;
import android.os.Message;
import android.os.Handler;

import android.os.Environment;

import java.io.File;
import java.io.IOException;
import java.io.FileOutputStream;

import util.Define;

//void	CallZipUtilActivity(String e_strFileName,String e_strdirectory)
//{
//	Intent intent = new Intent(this, ZipUtil.class); 
//	intent.putExtra("FileName", e_strFileName);
//	intent.putExtra("ExtractDirectory", e_strdirectory);
//	startActivity(intent);
//}

public class ZipUtilActivity// extends FragmentActivity
{
	static int		m_iPercent = 0;
	static String	m_strZipFileName;
	public static native void	UnzipProgress(int e_iProgress);

	public static void UnzipWithSystemFolder(String e_strZipfileName, String e_strDestDirectory,String e_strPassword,final boolean isDeleteZipFile)
	{
		String	l_strExternal = Environment.getExternalStorageDirectory().toString();
		l_strExternal += e_strZipfileName;
		try
		{
			ZipUtilActivity.Unzip(l_strExternal,e_strDestDirectory,e_strPassword,isDeleteZipFile);
		}
		catch(ZipException e)
		{
			e.printStackTrace();
			ZipUtilActivity.UnzipProgress(-1);
		}
	}
	//another way to fetch data
	//http://stackoverflow.com/questions/18974389/extracting-files-with-zip4j-temporarily-extracted-for-reading-but-not-visible
	public static void Unzip(String e_strZipfileName, String dest,String passwd,final boolean isDeleteZipFile)throws ZipException
	{
		m_iPercent = 0;
		UnzipProgress(0);
		m_strZipFileName = e_strZipfileName;
		ZipFile zFile = new ZipFile(e_strZipfileName);
		File destDir = new File(dest); // Unzip directory
		if (destDir.isDirectory() && !destDir.exists()) 
		{
			destDir.mkdirs();
		}
		if (zFile.isEncrypted()) 
		{
			zFile.setPassword(passwd.toCharArray());
		}

		final ProgressMonitor progressMonitor = zFile.getProgressMonitor();

		Thread progressThread = new Thread(new Runnable() 
		{
			@Override
			public void run() 
			{
				Bundle bundle = null;
				Message msg = null;
				try 
				{
					int percentDone = 0;
					while (true) 
					{
						Thread.sleep(100);
						percentDone = progressMonitor.getPercentDone();
						if( m_iPercent < 100 )
						{
							m_iPercent = percentDone;
							UnzipProgress(m_iPercent);
						}
						else
						{
							String	l_strStatus = "Status:"+progressMonitor.getState();
							Define.MyLog(l_strStatus);
							if( progressMonitor.getState() == ProgressMonitor.RESULT_SUCCESS )
							{
								UnzipProgress(100);
								break;
							}						
						}
					}
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
				}
				finally
				{
					if(isDeleteZipFile)
					{
						File	l_ZipFile = new File(m_strZipFileName);
						l_ZipFile.delete();
						Define.MyLog("File delete " + m_strZipFileName );
					}
				}
			}
		});
		progressThread.start();
		zFile.setRunInThread(true);
		zFile.extractAll(dest);
	}
}