package util;


import java.util.List;

import android.app.AlertDialog;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.util.Log;
import android.content.Context;

import android.content.DialogInterface;
import android.os.Build;
import android.provider.Settings.Secure;
import android.content.pm.ResolveInfo;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.app.DownloadManager;
import android.widget.Toast;
import android.net.Uri;
import android.os.Environment;
import android.content.res.Configuration;
//network
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.telephony.TelephonyManager;
//
import android.view.inputmethod.InputMethodManager;


import java.io.File;
//please register m_TempContext
public class Define
{
	final public static boolean g_bDebugTest = false;
	public static Context		m_TempContext;
	public static void	MyLog(String e_str)
	{
		if( g_bDebugTest )
			Log.d("GameApp", "GameAppJAVA:	"+e_str);
	}
	public static void	MyLogError(String e_str)
	{
		if( g_bDebugTest )
			Log.e("GameApp", "GameAppJAVAError:	"+e_str);
	}
	public static void	MyLogWarning(String e_str)
	{
		if( g_bDebugTest )
			Log.w("GameApp", "GameAppJAVAWarning:	"+e_str);
	}
    public static void showToastAlert(String text)
	{
		showToastAlert(text,m_TempContext);
	}
    // We call this function from native to display a toast string
    public static void showToastAlert(String text,Context e_Context)
    {
		MyLog(text);
		//https://thenewcircle.com/s/post/881/example_show_alert_dialog_in_android
		AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(e_Context);
//// set dialog message
//			alertDialogBuilder
//				.setMessage("Click yes to exit!")
//				.setCancelable(false)
//				.setPositiveButton("Yes",new DialogInterface.OnClickListener() {
//					public void onClick(DialogInterface dialog,int id) {
//						// if this button is clicked, close
//						// current activity
//						MainActivity.this.finish();
//					}
//				  })
//				.setNegativeButton("No",new DialogInterface.OnClickListener() {
//					public void onClick(DialogInterface dialog,int id) {
//						// if this button is clicked, just close
//						// the dialog box and do nothing
//						dialog.cancel();
//					}
//				});
        // Setting Dialog Title
        alertDialogBuilder.setTitle("Alert Dialog");
        // Setting Dialog Message
        alertDialogBuilder.setMessage(text);
		AlertDialog alertDialog = alertDialogBuilder.create();
        // Setting OK Button
  //      alertDialogBuilder.setButton("OK", new DialogInterface.OnClickListener()
		//{
  //          public void onClick(DialogInterface dialog, int which)
		//	{
		//		Toast.makeText(m_TempContext,"You clicked on OK", Toast.LENGTH_SHORT).show();
		//	}
  //      });
        // Showing Alert Message
        alertDialog.show();
    }

	//For open keyboard
	public static void OpenKeyBoard()
	{
        InputMethodManager imm = (InputMethodManager) m_TempContext.getSystemService(Context.INPUT_METHOD_SERVICE);
		//edittext.setRawInputType(Configuration.KEYBOARD_12KEY);
        imm.toggleSoftInput(InputMethodManager.SHOW_FORCED,0);
    }
    //For close keyboard
    public void CloseKeyBoard()
	{
        InputMethodManager imm = (InputMethodManager) m_TempContext.getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.toggleSoftInput(InputMethodManager.HIDE_IMPLICIT_ONLY,0);
    }
	//its possible to get null
	public static String	GetDeviceID()
	{
		String	l_str = android.os.Build.SERIAL;
		if( l_str != null )
			return l_str;
		return Secure.ANDROID_ID;
	}
	//long	GetUsedMemory()
	//{
	//	MemoryInfo mi = new MemoryInfo();
	//	ActivityManager activityManager = (ActivityManager) getSystemService(ACTIVITY_SERVICE);
	//	activityManager.getMemoryInfo(mi);
	//	long availableMegs = mi.availMem / 1048576L;
	//	return availableMegs;
	//}
}