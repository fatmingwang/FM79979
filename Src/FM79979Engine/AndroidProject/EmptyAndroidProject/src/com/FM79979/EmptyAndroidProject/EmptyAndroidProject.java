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
//
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

import java.util.ArrayList;
import java.lang.reflect.Method;
import java.util.Iterator;
import java.util.List;
import java.util.HashMap;
import java.util.Map;

import android.media.MediaPlayer;

import com.android.vending.billing.IInAppBillingService;
import org.json.JSONException;

import util.iab.IABManager;
import util.AccelerometerManager;
import util.AccelerometerListener;

import util.Define;
import util.AdMobUtility;
import android.view.ViewGroup.LayoutParams;
import android.view.WindowManager;

import util.NumericEditTextDialog;
import util.ExternalStorage;
import util.NetworkUtil;


import android.net.Uri;
import android.graphics.PixelFormat;

import android.graphics.Bitmap;
import java.io.File;
import java.io.IOException;
import java.io.FileOutputStream;

import util.Define;
import util.ZipUtilActivity;

import util.bluetooth.BluetoothChatFragment;

import android.widget.Toast;



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
	public IABManager				m_IABManager;
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState)
	{
		//m_IABManager = new IABManager(true,this);
		//m_IABManager.Create();
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
		if( m_IABManager != null )
		{
			// Pass on the activity result to the helper for handling
			if (m_IABManager.onActivityResult(requestCode, resultCode, data))
			{
				//
			}
		}
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
		if( AdMobUtility.m_AdView != null )
			AdMobUtility.m_AdView.pause();
		super.onPause();
	}
    @Override
    protected void onResume()
	{
		if( AdMobUtility.m_AdView != null )
			AdMobUtility.m_AdView.resume();
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
		if( AdMobUtility.m_AdView != null )
			AdMobUtility.m_AdView.destroy();
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
				try
				{
					//ZipUtilActivity.Unzip(e_strExtraceFileName,e_strDestDirectory,e_strPassword,false);
					//true for delete file
					ZipUtilActivity.Unzip(e_strExtraceFileName,e_strDestDirectory,e_strPassword,true);
					//ZipUtilActivity.Unzip("/sdcard/Download/79979.zip","/sdcard/Download/AAA","",false);
				}
				catch(ZipException e)
				{
					e.printStackTrace();
					ZipUtilActivity.UnzipProgress(-1);
				}
		//	}
		//}).start();
	}

	public boolean	IsWaitingForRespond()
	{
		return m_IABManager.m_bWaitingForIABRespond;
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
		m_IABManager.Create();
	}
	public void	AddSKUData(String e_strSKUID,String e_strName,int e_iType,boolean e_bConsumable)
	{
		m_IABManager.AddSKUData(e_strSKUID,e_strName,e_iType,e_bConsumable);
	}
	boolean	IsItemPurchased(String e_strSKUID,String e_strName,int e_iType)
	{
		return m_IABManager.IsItemPurchased(e_strSKUID,e_strName,e_iType);
	}
	public void	DoPurchaseProducts(int e_iProductsType,String e_strProductsID)
	{
		if( m_IABManager != null )
			m_IABManager.DoPurchaseProducts(e_iProductsType,e_strProductsID);
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
	/* This is the static constructor used to load the
	 * 'EmptyAndroidProject' library when the class is
	 * loaded.
	 */
	static
	{
		System.loadLibrary("EmptyAndroidProject");
	}
}
