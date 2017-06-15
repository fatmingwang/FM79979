package util;

import org.apache.http.conn.util.InetAddressUtils;
import java.util.List;
import android.util.Log;
import android.content.Context;
import android.os.Build;
import android.content.pm.ResolveInfo;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.app.DownloadManager;
import android.net.Uri;
import android.os.Environment;
//network
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.telephony.TelephonyManager;
import android.database.Cursor;

import java.lang.InterruptedException;
import java.io.File;
import java.lang.Thread;
import java.util.NoSuchElementException;
import util.Define;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiInfo;

import java.util.Enumeration;
import java.net.NetworkInterface;
import java.net.InetAddress;

public class NetworkUtil
{
	static	DownloadManager			m_TempDownloadManager;
	static	long					m_TempRequestID = 79979;
	static	int						m_iProgress = 0;
	public	static	 Context		m_TempContext;
	public  static native void		DownloadProgress(int e_iProgress);


	private static void CheckDwnloadStatus()
	{
		// TODO Auto-generated method stub
		DownloadManager.Query query = new DownloadManager.Query();
		query.setFilterById(m_TempRequestID);
		Cursor cursor = m_TempDownloadManager.query(query);
		if(cursor.moveToFirst())
		{
			int columnIndex = cursor.getColumnIndex(DownloadManager.COLUMN_STATUS);
			int status = cursor.getInt(columnIndex);
			int columnReason = cursor.getColumnIndex(DownloadManager.COLUMN_REASON);
			int reason = cursor.getInt(columnReason);
			switch(status)
			{
				case DownloadManager.STATUS_FAILED:
					String failedReason = "";
					switch(reason)
					{
						case DownloadManager.ERROR_CANNOT_RESUME:
							failedReason = "ERROR_CANNOT_RESUME";
						break;
						case DownloadManager.ERROR_DEVICE_NOT_FOUND:
							failedReason = "ERROR_DEVICE_NOT_FOUND";
						break;
						case DownloadManager.ERROR_FILE_ALREADY_EXISTS:
							failedReason = "ERROR_FILE_ALREADY_EXISTS";
						break;
						case DownloadManager.ERROR_FILE_ERROR:
							failedReason = "ERROR_FILE_ERROR";
						break;
						case DownloadManager.ERROR_HTTP_DATA_ERROR:
							failedReason = "ERROR_HTTP_DATA_ERROR";
						break;
						case DownloadManager.ERROR_INSUFFICIENT_SPACE:
							failedReason = "ERROR_INSUFFICIENT_SPACE";
						break;
						case DownloadManager.ERROR_TOO_MANY_REDIRECTS:
							failedReason = "ERROR_TOO_MANY_REDIRECTS";
						break;
						case DownloadManager.ERROR_UNHANDLED_HTTP_CODE:
							failedReason = "ERROR_UNHANDLED_HTTP_CODE";
						break;
						case DownloadManager.ERROR_UNKNOWN:
							failedReason = "ERROR_UNKNOWN";
						break;
					}
					Define.MyLog(failedReason);
				break;
				case DownloadManager.STATUS_PAUSED:
					String pausedReason = "";  
					switch(reason)
					{
						case DownloadManager.PAUSED_QUEUED_FOR_WIFI:
						pausedReason = "PAUSED_QUEUED_FOR_WIFI";
						break;
						case DownloadManager.PAUSED_UNKNOWN:
						pausedReason = "PAUSED_UNKNOWN";
						break;
						case DownloadManager.PAUSED_WAITING_FOR_NETWORK:
						pausedReason = "PAUSED_WAITING_FOR_NETWORK";
						break;
						case DownloadManager.PAUSED_WAITING_TO_RETRY:
						pausedReason = "PAUSED_WAITING_TO_RETRY";
						break;
					}  
					Define.MyLog(pausedReason);
				break;
				case DownloadManager.STATUS_PENDING:
					Define.MyLog("PENDING");
				break;
				case DownloadManager.STATUS_RUNNING:
					Define.MyLog("RUNNING");
				break;
				case DownloadManager.STATUS_SUCCESSFUL:  
					Define.MyLog("SUCCESSFUL");

				break;
			}
		}
	}
	/**
	 * @param context used to check the device version and DownloadManager information
	 * @return true if the download manager is available
	 */
	 //http://stackoverflow.com/questions/3028306/download-a-file-with-android-and-showing-the-progress-in-a-progressdialog
	public static boolean isDownloadManagerAvailable()
	{
		try 
		{
			if (Build.VERSION.SDK_INT < Build.VERSION_CODES.GINGERBREAD) 
			{
				return false;
			}
			Intent intent = new Intent(Intent.ACTION_MAIN);
			intent.addCategory(Intent.CATEGORY_LAUNCHER);
			intent.setClassName("com.android.providers.downloads.ui", "com.android.providers.downloads.ui.DownloadList");
			List<ResolveInfo> list = m_TempContext.getPackageManager().queryIntentActivities(intent,PackageManager.MATCH_DEFAULT_ONLY);
			return list.size() > 0;
		}
		catch (Exception e) 
		{
			return false;
		}
	}
	//http://blog.vogella.com/2011/06/14/android-downloadmanager-example/
	//http://www.mysamplecode.com/2012/09/android-downloadmanager-example.html
	//-1 download manager is not alivable,0 file is exists,else return request ID
	//the default path for download is "sdcard/Download/e_strSaveToFileName"
	public static void	DownloadByDownloadManager(String e_strfileURL,String e_strDescription,String e_strTitle,String e_strSaveToFileName,boolean e_bDeleteFileIfFileExists)
	{
		Define.MyLog("79979");
		m_iProgress = 0;
		DownloadProgress(m_iProgress);
		if( isDownloadManagerAvailable() == false )
		{
			m_iProgress = -1;
			DownloadProgress(m_iProgress);
			return;
		}
		String	l_strFileName = "/sdcard/Download/"+e_strSaveToFileName;
		File l_UnPackSaveFile = new File(l_strFileName);
		if( l_UnPackSaveFile.exists() )
		{//extra it
			if( e_bDeleteFileIfFileExists )
			{
				Define.MyLog(l_strFileName+" file exists delete old one");
				l_UnPackSaveFile.delete();
			}
			else
			{
				DownloadProgress(100);
				return;
			}
		}
		String url = e_strfileURL;
		DownloadManager.Request request = new DownloadManager.Request(Uri.parse(url));
		request.setDescription(e_strDescription);
		request.setTitle(e_strTitle);
		// in order for this if to run, you must use the android 3.2 to compile your app
		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) 
		{
			request.allowScanningByMediaScanner();
			request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED);
		}
		//without show in UI notification.
		//request.setNotificationVisibility(DownloadManager.Request.VISIBILITY_VISIBLE_NOTIFY_COMPLETED);
		request.setDestinationInExternalPublicDir(Environment.DIRECTORY_DOWNLOADS,e_strSaveToFileName);

		// get download service and enqueue file
		DownloadManager manager = (DownloadManager)m_TempContext.getSystemService(Context.DOWNLOAD_SERVICE);
		m_TempDownloadManager = manager;
		m_TempRequestID = manager.enqueue(request);	
		new Thread(new Runnable() 
		{
            @Override
            public void run() 
			{
                boolean downloading = true;
				try
				{
					while (downloading)
					{
						Thread.sleep(1000);
						DownloadManager.Query q = new DownloadManager.Query();
						q.setFilterById(m_TempRequestID);
						Cursor cursor = m_TempDownloadManager.query(q);
						cursor.moveToFirst();
						int bytes_downloaded = cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_BYTES_DOWNLOADED_SO_FAR));
						int bytes_total = cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_TOTAL_SIZE_BYTES));
						if (cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_STATUS)) == DownloadManager.STATUS_SUCCESSFUL)
						{
							downloading = false;
							m_iProgress = 100;
							DownloadProgress(m_iProgress);
							Define.MyLog("download done");
						}
						else
						if (cursor.getInt(cursor.getColumnIndex(DownloadManager.COLUMN_STATUS)) == DownloadManager.STATUS_FAILED)
						{
							downloading = false;
							m_iProgress = -1;
							DownloadProgress(m_iProgress);
							CheckDwnloadStatus();
							Define.MyLog("download failed");
						}
						else
						{
							m_iProgress = (int) ((bytes_downloaded * 100l) / bytes_total);
							//String	l_str = "Progress:"+m_iProgress;
							//Define.MyLog(l_str);
							DownloadProgress(m_iProgress);
						}
						cursor.close();
					}
				}
				catch (InterruptedException e)
				{
					e.printStackTrace();
					m_iProgress = -1;
					DownloadProgress(m_iProgress);
				}
            }
        }).start();
		return;
		//return m_TempRequestID;
		//return true;
	}

	// CHECK request by id
	private String statusMessage(Cursor c)
	{
		String msg = "???";
		switch (c.getInt(c.getColumnIndex(DownloadManager.COLUMN_STATUS))) {
		case DownloadManager.STATUS_FAILED:
			msg = "Download failed!";
			break;

		case DownloadManager.STATUS_PAUSED:
			msg = "Download paused!";
			break;

		case DownloadManager.STATUS_PENDING:
			msg = "Download pending!";
			break;

		case DownloadManager.STATUS_RUNNING:
			msg = "Download in progress!";
			break;

		case DownloadManager.STATUS_SUCCESSFUL:
			msg = "Download complete!";
			break;

		default:
			msg = "Download is nowhere in sight";
			break;
		}

		return (msg);
	}

	//<uses-permission android:name="android.permission.INTERNET" />
	//<uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
	public static boolean checkWifiInternetConn()
	{
        //Create object for ConnectivityManager class which returns network related info
        ConnectivityManager connectivity = (ConnectivityManager) m_TempContext.getSystemService(m_TempContext.CONNECTIVITY_SERVICE);
        //If connectivity object is not null
        if (connectivity != null)
		{
            //Get network info - WIFI internet access
            NetworkInfo info = connectivity.getNetworkInfo(ConnectivityManager.TYPE_WIFI);
            if (info != null)
			{
                //Look for whether device is currently connected to WIFI network
                if (info.isConnected())
				{
                    return true;
                }
            }
        }
        return false;
    }

	public static boolean checkMobileInternetConn()
	{
        //Create object for ConnectivityManager class which returns network related info
        ConnectivityManager connectivity = (ConnectivityManager) m_TempContext.getSystemService(m_TempContext.CONNECTIVITY_SERVICE);
        //If connectivity object is not null
        if (connectivity != null) 
		{
            //Get network info - Mobile internet access
            NetworkInfo info = connectivity.getNetworkInfo(ConnectivityManager.TYPE_MOBILE);
            if (info != null)
			{
                //Look for whether device is currently connected to Mobile internet
                if (info.isConnected())
				{
                    return true;
                }
            }
        }
        return false;
    }

	public static String GetMobileCarrierName()
	{
		TelephonyManager manager = (TelephonyManager)m_TempContext.getSystemService(m_TempContext.TELEPHONY_SERVICE);
		return manager.getNetworkOperatorName();
	}
	static boolean IsConnectionAlivable()
	{
		ConnectivityManager cm = (ConnectivityManager) m_TempContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (cm.getActiveNetworkInfo() != null && cm.getActiveNetworkInfo().isAvailable() && cm.getActiveNetworkInfo().isConnected())
		{
            return true;
        }
        return false;
	}
	public static synchronized String getLocalAddress()
	{
        ConnectivityManager connectivityManager = (ConnectivityManager)m_TempContext.getSystemService(Context.CONNECTIVITY_SERVICE);
        if (connectivityManager.getActiveNetworkInfo() == null || !connectivityManager.getActiveNetworkInfo().isAvailable() || !connectivityManager.getActiveNetworkInfo().isConnected())
		{
            return null;
        }
        NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
        if (activeNetworkInfo.getType() == ConnectivityManager.TYPE_WIFI)
		{
            WifiManager myWifiManager = (WifiManager) m_TempContext.getSystemService(Context.WIFI_SERVICE);
            WifiInfo myWifiInfo = myWifiManager.getConnectionInfo();
            int ipAddress = myWifiInfo.getIpAddress();
            return android.text.format.Formatter.formatIpAddress(ipAddress);
        }
		else 
		if(activeNetworkInfo.getType() == ConnectivityManager.TYPE_MOBILE)
		{
   //         try 
			//{
   //             Enumeration<NetworkInterface> interfs = NetworkInterface.getNetworkInterfaces();
   //             NetworkInterface activeInterf = null;
   //             String hostName = NetworkInterface.getByInetAddress(InetAddress.getLocalHost()).getName();
   //             InetAddress ret = null;
   //             while (interfs.hasMoreElements()) 
			//	{
   //                 try 
			//		{
   //                     activeInterf = interfs.nextElement();
   //                     if (activeInterf.isLoopback()) 
			//			{
   //                         continue;
   //                     }
			//			else 
			//			if(!hostName.equalsIgnoreCase(activeInterf.getDisplayName())) 
			//			{
   //                         ret = activeInterf.getInetAddresses().nextElement();
   //                         break;
   //                     }
   //                 }
			//		catch (NoSuchElementException e) 
			//		{
   //                     continue;
   //                 }
   //             }
   //             return ret.getHostAddress();
   //         }
			//catch (Exception e)
			//{

   //         }
			try 
			{
				String	l_strIP4;
				for (Enumeration<NetworkInterface> en = NetworkInterface.getNetworkInterfaces(); en.hasMoreElements();) 
				{
					NetworkInterface intf = en.nextElement();
					for (Enumeration<InetAddress> enumIpAddr = intf.getInetAddresses(); enumIpAddr.hasMoreElements();) 
					{
						InetAddress inetAddress = enumIpAddr.nextElement();
						Define.MyLog("ip1--:" + inetAddress);
						Define.MyLog("ip2--:" + inetAddress.getHostAddress());
						// for getting IPV4 format
						if (!inetAddress.isLoopbackAddress() && InetAddressUtils.isIPv4Address(l_strIP4 = inetAddress.getHostAddress())) 
						{

							String ip = inetAddress.getHostAddress().toString();
							Define.MyLog("ip---::" + ip);
							// return inetAddress.getHostAddress().toString();
							return l_strIP4;
						}
					}
				}
			}
			catch (Exception ex) 
			{
				Log.e("IP Address", ex.toString());
			}
        }
		else
		{
            return null;
        }
        return null;
    }
}