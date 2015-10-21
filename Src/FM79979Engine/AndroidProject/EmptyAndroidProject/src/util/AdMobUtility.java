package util;

import com.google.android.gms.ads.InterstitialAd;
import android.app.NativeActivity;
import android.widget.PopupWindow;
import android.widget.PopupWindow;
import com.google.ads.*;
import com.google.android.gms.ads.AdView;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdRequest.Builder;
import com.google.android.gms.ads.AdListener;
import android.widget.LinearLayout;
import android.view.Gravity;
import android.view.View;
import android.view.WindowManager;
import android.view.ViewGroup.LayoutParams;
import android.view.ViewGroup.MarginLayoutParams;
import android.graphics.Color;
//
//import Define;
import android.os.Bundle;
//add this into AndroidMenifest.xml
//between application
//<!-- for google AdMobs-->
//<meta-data android:name="com.google.android.gms.version" android:value="6111000" />
//<activity android:name="com.google.android.gms.ads.AdActivity" android:configChanges="keyboard|keyboardHidden|orientation|screenLayout|uiMode|screenSize|smallestScreenSize"/>
public class AdMobUtility
{
	static	boolean					m_bOnlyShowAdOnce = true;
	static	boolean					m_bAdShowed = false;

	static public InterstitialAd	m_InterstitialAd;
	static public AdView			m_AdView;
	PopupWindow						m_PopUpWindow;
	static AdMobUtility				m_AdMobUtilActivity;
	public static NativeActivity	m_LaunchNativeActivity;
	LinearLayout					m_Layout;
	LinearLayout					m_MainLayout;
	static String					m_strAdMobID;
	static int						m_iWidth;
	static int						m_iHeight;

	//fuck it seems inhirent NativeActivity so GetCustomClass calling is stock!?
	static public void		Close()
	{
		if( m_AdMobUtilActivity != null && m_AdMobUtilActivity.m_PopUpWindow != null )
		{
			m_LaunchNativeActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					m_AdMobUtilActivity.m_PopUpWindow.dismiss();
					if (m_AdMobUtilActivity.m_AdView != null)
					{
						m_AdMobUtilActivity.m_AdView.destroy();
						m_AdMobUtilActivity.m_AdView = null;
					}
					m_AdMobUtilActivity.m_PopUpWindow = null;
					m_AdMobUtilActivity = null;
				}
			});
			//m_AdMobUtilActivity.m_PopUpWindow.setOutsideTouchable(true);
			//m_AdMobUtilActivity.m_PopUpWindow.setFocusable(true);
		}
	}
	static public void Show(String e_strAdMobID,int e_iWidth,int e_iHeight)
	{
		//please set m_LaunchNativeActivity first
		if( m_LaunchNativeActivity == null )
			return;
		if( m_AdMobUtilActivity == null )
			m_AdMobUtilActivity = new AdMobUtility();
		m_strAdMobID = e_strAdMobID;
		m_iWidth = e_iWidth;
		m_iHeight = e_iHeight;
		m_AdMobUtilActivity.showAdPopup();
	}

	//public void		CloseAD()
	//{
	//	if( m_PopUpWindow != null )
	//	{
	//		dismiss();
	//		Finish();
	//	}
	//}
	static public void	Refresh()
	{
		if( m_AdMobUtilActivity.m_AdView != null )
		{
			m_LaunchNativeActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					Define.MyLog("Refresh");
					AdRequest.Builder	l_AdBuilder = new AdRequest.Builder();
					AdRequest l_AdRequest = l_AdBuilder.build();
					Define.MyLog("1");
					if( Define.g_bDebugTest )
						l_AdBuilder.addTestDevice("5E06E2149F4130CC8D7210B8947075F8");//for xiomi
					Define.MyLog("2");
					m_AdMobUtilActivity.m_AdView.loadAd(l_AdRequest);
					Define.MyLog("3");
				}
			});
		}
	}
	// Our popup window, you will call it from your C/C++ code later
	 public void showAdPopup()
	 {
		if( m_AdView == null )
		{
			// Make your custom init here
			//m_LaunchNativeActivity.getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
			// Create our ad view here
			//m_AdView = new AdView(m_AdMobUtilActivity, AdSize.BANNER,m_strAdMobID);
			m_AdView = new AdView(m_LaunchNativeActivity);
			m_AdView.setAdSize(com.google.android.gms.ads.AdSize.BANNER);//AdSize.BANNER
			//m_AdView.setAdSize(com.google.android.gms.ads.AdSize.BANNER);
			//m_AdView.setAdSize(com.google.android.gms.ads.AdSize.SMART_BANNER);
			m_AdView.setAdUnitId(m_strAdMobID);
			m_LaunchNativeActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					// Out popup window
					m_PopUpWindow = new PopupWindow(m_LaunchNativeActivity);
					// This is the minimum size for AdMob, we need to set this in case our target device run at 320x480 resolution (Otherwise no ad will be shown, see the padding kill below)
					m_PopUpWindow.setWidth(m_AdMobUtilActivity.m_iWidth);
					m_PopUpWindow.setHeight(m_AdMobUtilActivity.m_iHeight);
					m_PopUpWindow.setWindowLayoutMode(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
					m_PopUpWindow.setClippingEnabled(false);
					m_AdView.setAdListener(new AdListener()
					{

						@Override public void onAdClosed()
						{
							super.onAdClosed();
							Define.MyLog("onAdClosed");
						}

						@Override public void onAdOpened()
						{
							super.onAdOpened();
							Define.MyLog("onAdOpened");
						}

						@Override public void onAdFailedToLoad(int errorCode)
						{
							super.onAdFailedToLoad(errorCode);
							String	l_str = "FailedCode:"+errorCode;
							Define.MyLog("onAdFailedToLoad"+l_str);
						}
						@Override public void onAdLoaded() 
						{
							super.onAdLoaded();
							//m_AdView.bringToFront();
							Define.MyLog("onAdLoaded");
							m_AdMobUtilActivity.m_AdView.setVisibility(View.VISIBLE);
							//m_LaunchNativeActivity.runOnUiThread(new Runnable()
							//{
							//	@Override public void run()
							//	{
							//		m_AdMobUtilActivity.m_Layout.requestLayout();
							//	}
							//});
						}
					});
					m_Layout = new LinearLayout(m_LaunchNativeActivity);
					m_MainLayout = new LinearLayout(m_LaunchNativeActivity);
					// The layout system for the PopupWindow will kill some pixels due to margins/paddings etc(No way to remove it), so padd it to adjust
					//m_Layout.setPadding(-5, -5, -5, -5);
					MarginLayoutParams params = new MarginLayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT);
					params.setMargins(0, 0, 0, 0);
					m_Layout.setOrientation(LinearLayout.VERTICAL);
					m_Layout.addView(m_AdView, params);
					m_PopUpWindow.setContentView(m_Layout);
					m_LaunchNativeActivity.setContentView(m_MainLayout, params);
					AdRequest.Builder	l_AdBuilder = new AdRequest.Builder();
					AdRequest l_AdRequest = l_AdBuilder.build();
					// Enable this if your are testing AdMob, otherwise you'll risk to be banned!
					//adRequest.addTestDevice(AdRequest.TEST_EMULATOR);
					if( Define.g_bDebugTest )
						l_AdBuilder.addTestDevice("187CE1607873E23059FBFC2CE29F2828");//for anderson samusang
						//l_AdBuilder.addTestDevice("5E06E2149F4130CC8D7210B8947075F8");//for xiomi
					try
					{
						m_AdMobUtilActivity.m_AdView.setBackgroundColor(Color.TRANSPARENT);
						m_AdMobUtilActivity.m_AdView.loadAd(l_AdRequest);
						m_AdView.setVisibility(View.GONE);
						Define.MyLog("AdMob");
					}
					catch (Exception ex)
					{
						Define.MyLog(ex.toString());
					}
					// Show our popup window
					m_PopUpWindow.showAtLocation(m_MainLayout, Gravity.BOTTOM, 0, 0);
					m_PopUpWindow.update();
				}
			});
		}
	}

	 static public void showInterstitialAd(String e_strAdMobID)
	 {
		if( m_bOnlyShowAdOnce && m_bAdShowed )
			return;
		m_strAdMobID = e_strAdMobID;
		if( m_AdMobUtilActivity == null )
			m_AdMobUtilActivity = new AdMobUtility();
		m_AdMobUtilActivity.showInterstitialAdPopup();
	 }

	 public void showInterstitialAdPopup()
	 {
		if( m_InterstitialAd == null )
		{
			Define.MyLog("onInterstitialAd prepare");
			m_LaunchNativeActivity.runOnUiThread(new Runnable()
			{
				@Override
				public void run()
				{
					Define.MyLog("onInterstitialAd 1");
					m_InterstitialAd = new InterstitialAd(m_LaunchNativeActivity);
					Define.MyLog("onInterstitialAd 2");
					m_InterstitialAd.setAdUnitId(m_strAdMobID);
					Define.MyLog("onInterstitialAd 3");
					m_InterstitialAd.setAdListener(new AdListener()
					{
						@Override public void onAdClosed()
						{
							super.onAdClosed();
							Define.MyLog("onInterstitialAdClosed");
							m_InterstitialAd = null;
						}

						@Override public void onAdOpened()
						{
							m_bAdShowed = true;
							super.onAdOpened();
							Define.MyLog("onInterstitialAdOpened");
						}

						@Override public void onAdFailedToLoad(int errorCode)
						{
							super.onAdFailedToLoad(errorCode);
							String	l_str = "FailedCode:"+errorCode;
							Define.MyLog("onInterstitialAdFailedToLoad:"+l_str);
						}
						@Override public void onAdLoaded() 
						{
							super.onAdLoaded();
							Define.MyLog("onInterstitialAdLoaded");
							m_InterstitialAd.show();
						}
					});
					Define.MyLog("onInterstitialAd 4");
					AdRequest.Builder	l_AdBuilder = new AdRequest.Builder();
					Define.MyLog("onInterstitialAd 5");
					AdRequest l_AdRequest = l_AdBuilder.build();
					Define.MyLog("onInterstitialAd 6");
					if( Define.g_bDebugTest )
						//l_AdBuilder.addTestDevice("187CE1607873E23059FBFC2CE29F2828");//for anderson samusang
						l_AdBuilder.addTestDevice("5E06E2149F4130CC8D7210B8947075F8");//for xiomi
					Define.MyLog("m_InterstitialAd start");
					// Begin loading your interstitial.
					try
					{
						m_InterstitialAd.loadAd(l_AdRequest);
					}
					catch(Exception e)
					{
						Define.MyLog(e.toString());
					}
					Define.MyLog("m_InterstitialAd loadAd");
				}
			});
		}
	}
}