/*
 * Copyright (C) 2009 The Android Open Source Project
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
package com.FM79979.EmptyAndroidProject;

import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
import android.app.NativeActivity;
import android.app.PendingIntent;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.IntentSender.SendIntentException;
import android.content.ServiceConnection;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.RemoteException;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.KeyEvent;
import android.widget.ImageView;
import android.os.Environment;
import android.widget.ShareActionProvider;
import android.media.MediaPlayer;
import android.net.Uri;
import android.graphics.PixelFormat;
import android.widget.Toast;
import android.graphics.Bitmap;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;

import java.util.ArrayList;
import java.lang.reflect.Method;
import java.util.Iterator;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

import org.json.JSONException;


import util.AccelerometerManager;
import util.AccelerometerListener;
import util.Define;

import util.NumericEditTextDialog;
import util.ExternalStorage;
import util.NetworkUtil;
import util.Define;


import java.io.File;
import java.io.IOException;
import java.io.FileOutputStream;




//import util.facebook.FBManager;
import net.lingala.zip4j.exception.ZipException;
/**
 * This class loads the Java Native Interface (JNI)
 * library, 'libEmptyAndroidProject.so', and provides access to the
 * exposed C functions.
 * The library is packaged and installed with the application.
 * See the C file, /jni/EmptyAndroidProject.c file for the
 * implementations of the native methods. 
 * 
 * For more information on JNI, see: http://java.sun.com/docs/books/jni/
 */
 //
 //import android.support.annotation.Nullable;//https://stackoverflow.com/questions/36906095/error28-34-error-package-android-support-annotation-does-not-exist 
 //
public class EmptyAndroidProject extends NativeActivity implements AccelerometerListener
{
	public  void					OpenKeyBoard(){ Define.OpenKeyBoard(); }
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		Define.m_TempContext = getApplicationContext();
		NetworkUtil.m_TempContext = getApplicationContext();
		NumericEditTextDialog.m_TempFragmentManager = getFragmentManager();
        super.onCreate(savedInstanceState);
		//AdMobUtility.m_LaunchNativeActivity = this;
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
	}

	public void onAccelerationChanged(float x, float y, float z)
	{
		Define.SensorChange(x,y,z);
    }
	public void onShake(float force)
	{
		Define.SensorForce(force);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) 
	{
		super.onActivityResult(requestCode, resultCode, data);
    }

	//@Override
 //   public boolean dispatchKeyEvent(KeyEvent e)
	//{
	//	String	l_str = "JAVA press:"+e.getKeyCode();
	//	Define.MyLog(l_str);
	//	//press delete key.
 //       if (e.getKeyCode() == KeyEvent.KEYCODE_FORWARD_DEL) 
	//	{ 
	//		/*Just switch out keycode if KEYCODE_FORWARD_DEL if its not the correct one*/
 //           //Toast.makeText(MainActivity.this, "YOU CLICKED Delete KEY",Toast.LENGTH_LONG).show();
 //       }
 //       return super.dispatchKeyEvent(e);
 //   };
 	@Override   public void onPause()
	{

		super.onPause();
	}
    @Override
    protected void onResume()
	{

		//if( mMediaPlayer != null )
			//mMediaPlayer.start();
		super.onResume();             
        //Check device supported Accelerometer senssor or not
        if (AccelerometerManager.isSupported(this)) 
		{//Start Accelerometer Listening
            AccelerometerManager.startListening(this);
        }
		//util.facebook.FBManager.publishInstallAsync(getApplicationContext(),getString(R.string.app_id));
    }

	@Override
    public void onStop()
	{
        super.onStop();
        //Check device supported Accelerometer senssor or not
        if (AccelerometerManager.isListening())
		{
            //Start Accelerometer Listening
            AccelerometerManager.stopListening();
        }
            
    }
	@Override
    public void onDestroy() 
	{

        super.onDestroy();
        //Check device supported Accelerometer senssor or not
        if (AccelerometerManager.isListening())
		{
            //Start Accelerometer Listening
            AccelerometerManager.stopListening();
        }
    }

	//public void CallUnzipUtility(String e_strFile,String e_strdirectory)
	public void CallUnzipUtility(final String e_strExtraceFileName,final String e_strDestDirectory,final String e_strPassword)
	{
		//new Thread(new Runnable() 
		//{
		//	@Override
		//	public void run() 
		//	{
		//	}
		//}).start();
	}

	public boolean	IsWaitingForRespond()
	{
		return true;
	}
	public void	FBPost()
	{
		//NumericEditTextDialog	l_NumericEditTextDialog = new NumericEditTextDialog();
		//l_NumericEditTextDialog.show(getFragmentManager(), "dialog");
		//Intent intent = new Intent(this, util.facebook.FBManager.class);
        //startActivity(intent);
	}

	//void		CloseAD()
	//{
	//	if( m_AdMobUtility != null )
	//	{
	//		m_AdMobUtilActivity.CloseAD();
	//		m_AdMobUtilActivity = null;
	//	}
	//}

	//void ShowAD(String e_strAdMobID,int e_iWidth,int e_iHeight)
	//{
	//	if( m_AdMobUtility == null )
	//	{
	//		m_AdMobUtility = new AdMobUtility(e_strAdMobID,e_iWidth,e_iHeight);
	//	}
	//}
	public void	IABCreate()
	{
	}

	public void	ShareToFriend(String e_strFileName)
	{
		Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("image/*");
        //intent.putExtra(Intent.EXTRA_TEXT, e_strFileName);
		final File photoFile = new File(e_strFileName);
		intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(photoFile));
        //intent.putExtra(Intent.EXTRA_STREAM, e_strFileName);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		ShareActionProvider shareActionProvider = new ShareActionProvider(getApplicationContext());
        //shareActionProvider.setShareHistoryFileName(ShareActionProvider.DEFAULT_SHARE_HISTORY_FILE_NAME);
        shareActionProvider.setShareIntent(intent);
		startActivity(intent);
	}


	public String	GetExternalSDCardPath()
	{
		Map<String, File> externalLocations = ExternalStorage.getAllStorageLocations();
		//File sdCard = externalLocations.get(ExternalStorage.SD_CARD);
		File externalSdCard = externalLocations.get(ExternalStorage.EXTERNAL_SD_CARD);		
		if( externalSdCard != null )
			return externalSdCard.getPath();
			return null;
	}

	
	public static String getPath( Context context, Uri uri ) 
	{
		String result = null;
		//String[] proj = { MediaStore.Images.Media.DATA };
		//Cursor cursor = context.getContentResolver( ).query( uri, proj, null, null, null );
		//if(cursor != null)
		//{
			//if ( cursor.moveToFirst( ) )
			//{
//				int column_index = cursor.getColumnIndexOrThrow( proj[0] );
				//result = cursor.getString( column_index );
			//}
			//cursor.close( );
		//}
		if(result == null)
		{
			result = "Not found";
		}
		return result;
	}

	String	GetClipboardString()
	{
		//CharSequence l_CharSequence = m_Clipboard.getText();
		//if( l_CharSequence != null)
		//{
//			String l_str = l_CharSequence.toString();
			//return l_str;
		//}
		return "";
	}

	void	SetClipboardString(String e_strText)
	{
		//https://stackoverflow.com/questions/33207809/what-exactly-is-label-parameter-in-clipdata-in-android
		//ClipData clip = ClipData.newPlainText("test", e_strText);
		//m_Clipboard.setPrimaryClip(clip);
	}

	public void	PickUpPhotoByIntent()
	{
		Intent getIntent = new Intent(Intent.ACTION_GET_CONTENT);
		getIntent.setType("image/*");

		Intent pickIntent = new Intent(Intent.ACTION_PICK, android.provider.MediaStore.Images.Media.EXTERNAL_CONTENT_URI);
		pickIntent.setType("image/*");

		Intent chooserIntent = Intent.createChooser(getIntent, "Select Image");
		chooserIntent.putExtra(Intent.EXTRA_INITIAL_INTENTS, new Intent[] {pickIntent});

		//startActivityForResult(chooserIntent, PICK_PHOTO);
	}

	public void	ShareImageFileToFriend(String e_strFileName)
	{
		Intent intent = new Intent(Intent.ACTION_SEND);
        intent.setType("image/*");
        //intent.putExtra(Intent.EXTRA_TEXT, e_strFileName);
		final File photoFile = new File(e_strFileName);
		intent.putExtra(Intent.EXTRA_STREAM, Uri.fromFile(photoFile));
        //intent.putExtra(Intent.EXTRA_STREAM, e_strFileName);
        intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
		ShareActionProvider shareActionProvider = new ShareActionProvider(getApplicationContext());
        //shareActionProvider.setShareHistoryFileName(ShareActionProvider.DEFAULT_SHARE_HISTORY_FILE_NAME);
        shareActionProvider.setShareIntent(intent);
		startActivity(intent);
	}
	/* This is the static constructor used to load the
	 * 'EmptyAndroidProject' library when the class is
	 * loaded.
	 */
	static
	{
		System.loadLibrary("EmptyAndroidProject");
	}
}
