
package com.AndroidGrageleProject;

import android.app.NativeActivity;
import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;
//
import android.content.ClipData;
import android.widget.ShareActionProvider;
import java.io.File;
import android.net.Uri;
import android.content.Intent;
import android.content.ClipboardManager;
import util.Define;
import util.NumericEditTextDialog;
import util.ExternalStorage;
import util.MyBitmap;

public class AndroidGrageleProject extends NativeActivity//Activity//NativeActivity
{
    public  void					OpenKeyBoard(){ Define.OpenKeyBoard(); }
    private ClipboardManager		m_Clipboard;
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        //
	Define.m_TempContext = getApplicationContext();
	Define.m_Activity = this;
	m_Clipboard = (ClipboardManager) this.getSystemService(CLIPBOARD_SERVICE); 
	NumericEditTextDialog.m_TempFragmentManager = getFragmentManager();
	//
        super.onCreate(savedInstanceState);
	//
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

	void	SetClipboardString(String e_strText)
	{
		//https://stackoverflow.com/questions/33207809/what-exactly-is-label-parameter-in-clipdata-in-android
		ClipData clip = ClipData.newPlainText("test", e_strText);
		m_Clipboard.setPrimaryClip(clip);
	}

	String	GetClipboardString()
	{
		CharSequence l_CharSequence = m_Clipboard.getText();
		if( l_CharSequence != null)
		{
			String l_str = l_CharSequence.toString();
			return l_str;
		}
		return "";
	}

    static { System.loadLibrary("AndroidGameApp"); }
}
