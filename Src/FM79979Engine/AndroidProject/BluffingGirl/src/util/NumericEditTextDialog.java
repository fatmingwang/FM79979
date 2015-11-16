package util;


import android.widget.EditText;
import android.text.InputType;
import android.os.Bundle;

import android.view.View;
import android.view.View.OnClickListener;
import android.content.DialogInterface;

import android.widget.Button;
import android.widget.TextView;

import android.app.AlertDialog;
import android.app.Dialog;
import android.app.DialogFragment;
import android.app.FragmentManager;

import util.Define;


//http://stackoverflow.com/questions/12622742/get-value-from-dialogfragment
public class NumericEditTextDialog extends DialogFragment implements OnClickListener
{
	public static FragmentManager	m_TempFragmentManager;
	native	static void	 FinishInput(String e_strValue);
	native	static void	 Chancel();
	private EditText editQuantity;
	private static String m_strTitle;
	private static String m_strMessage;
    @Override
    public Dialog onCreateDialog(Bundle savedInstanceState)
	{
        editQuantity = new EditText(getActivity());
		//editQuantity.setInputType(InputType.TYPE_NUMBER_FLAG_DECIMAL);
        editQuantity.setInputType(InputType.TYPE_CLASS_NUMBER);

        return new AlertDialog.Builder(getActivity())
							.setTitle(m_strTitle)
							.setMessage(m_strMessage)
							.setPositiveButton("OK",
								new DialogInterface.OnClickListener() 
								{
									public void onClick(DialogInterface dialog, int id) 
									{
										Define.MyLog("Positive click!");
										String value = editQuantity.getText().toString();
										FinishInput(value);
									}
								})
							.setNegativeButton("CANCEL",
								new DialogInterface.OnClickListener() 
								{
									public void onClick(DialogInterface dialog, int id) 
									{
										Define.MyLog("Negative click!");
										Chancel();
									}
								})
							.setView(editQuantity).create();
    }

    @Override
    public void onClick(View v)
	{
        String value = editQuantity.getText().toString();
        Define.MyLog("Quantity"+value);
        //MainActivity callingActivity = (MainActivity) getActivity();
        //callingActivity.onUserSelectValue(value);
        dismiss();
    }
	static void	ShowDialog(String e_strTitle,String e_strMessage)
	{
		if( m_TempFragmentManager != null )
		{
			m_strTitle = e_strTitle;
			m_strMessage = e_strMessage;
			NumericEditTextDialog	l_NumericEditTextDialog = new NumericEditTextDialog();
			l_NumericEditTextDialog.show(m_TempFragmentManager, "dialog");	
		}
	}
}