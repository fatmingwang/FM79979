/*
 * Copyright (C) 2014 The Android Open Source Project
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

package util.bluetooth;

//set your package name
import com.FM79979.EmptyAndroidProject.R;

import android.app.ActionBar;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
//copy android-support-annotations.jar from SDK/libs.
import android.support.annotation.Nullable;//https://stackoverflow.com/questions/36906095/error28-34-error-package-android-support-annotation-does-not-exist
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentActivity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;


/**
 * This fragment controls Bluetooth to communicate with other devices.
 */

 //https://github.com/googlesamples/android-BluetoothChat
 //sendMessage to send data
public class BluetoothChatFragment extends FragmentActivity 
{
	public static void	BluetoothStart(Activity e_Activity)
	{
        Intent serverIntent = new Intent(e_Activity, BluetoothChatFragment.class);
        //e_Activity.startActivityForResult(serverIntent,79979);
		e_Activity.startActivity(serverIntent);
	}

	public static void	BluetoothShutDown()
	{
		BluetoothChatFragment.ShutBluetoothService();
	}

	public static void	BluetoothSendBuffer(byte[] buffer)
	{
		if(mChatService != null)
		{
			mChatService.write(buffer);
		}
	}

    private static final String TAG = "BluetoothChatFragment";
    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE_SECURE = 1;
    private static final int REQUEST_CONNECT_DEVICE_INSECURE = 2;
    private static final int REQUEST_ENABLE_BT = 3;
    /**
     * Name of the connected device
     */
    private String mConnectedDeviceName = null;

    /**
     * Local Bluetooth adapter
     */
    private BluetoothAdapter mBluetoothAdapter = null;

    /**
     * Member object for the chat services
     */
    private static BluetoothChatService mChatService = null;

	private Button m_ScannDevicesButton;

    @Override
    public void onCreate(Bundle savedInstanceState) 
	{
        super.onCreate(savedInstanceState);
		setContentView(R.layout.fragment_bluetooth_chat);
		m_ScannDevicesButton = (Button) findViewById(R.id.scanningDevicesButton);
        m_ScannDevicesButton.setOnClickListener
		(
			new View.OnClickListener() 
			{
				public void onClick(View view) 
				{
					Intent serverIntent = new Intent(BluetoothChatFragment.this, DeviceListActivity.class);
					startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_INSECURE);
				}
			}
		);

        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null)
		{
			Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
        }
		else
		{
			ensureDiscoverable();
		}
    }


    @Override
    public void onStart() 
	{
        super.onStart();
        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) 
		{
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            // Otherwise, setup the chat session
        }
		else
		if (mChatService == null)
		{
			mChatService = new BluetoothChatService(this, mHandler);
        }
    }

    @Override
    public void onDestroy() 
	{
        super.onDestroy();
    }

	public static void ShutBluetoothService()
	{
        if (mChatService != null) 
		{
            mChatService.stop();
			mChatService = null;
        }	
	}

    @Override
    public void onResume()
	{
        super.onResume();
        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) 
		{
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) 
			{
                // Start the Bluetooth chat services
                mChatService.start();
            }
        }
    }

    /**
     * Makes this device discoverable for 300 seconds (5 minutes).
     */
    private void ensureDiscoverable() 
	{
        if (mBluetoothAdapter.getScanMode() != BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) 
		{
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }

    /**
     * Sends a message.
     *
     * @param message A string of text to send.
     */
    private void sendMessage(String message) 
	{
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) 
		{
            //Toast.makeText(getActivity(), R.string.not_connected, Toast.LENGTH_SHORT).show();
			Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }

        // Check that there's actually something to send
        if (message.length() > 0) 
		{
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);
        }
    }


    /**
     * The Handler that gets information back from the BluetoothChatService
     */
    private final Handler mHandler = new Handler() 
	{
        @Override
        public void handleMessage(Message msg) 
		{
            //FragmentActivity activity = getActivity();
			FragmentActivity activity = BluetoothChatFragment.this;
            switch (msg.what) 
			{
                case Constants.MESSAGE_STATE_CHANGE:
                    break;
                case Constants.MESSAGE_WRITE:
                    byte[] writeBuf = (byte[]) msg.obj;
                    break;
                case Constants.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    break;
                case Constants.MESSAGE_DEVICE_NAME:
                    // save the connected device's name
                    mConnectedDeviceName = msg.getData().getString(Constants.DEVICE_NAME);
                    if (null != activity) 
					{
                        Toast.makeText(activity, "Connected to " + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                    }
                    break;
                case Constants.MESSAGE_TOAST:
                    if (null != activity) 
					{
                        Toast.makeText(activity, msg.getData().getString(Constants.TOAST), Toast.LENGTH_SHORT).show();
                    }
                    break;
            }
        }
    };

    public void onActivityResult(int requestCode, int resultCode, Intent data)
	{
        switch (requestCode) 
		{
            case REQUEST_CONNECT_DEVICE_SECURE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) 
				{
                    connectDevice(data, true);
					this.finish();
                }
                break;
            case REQUEST_CONNECT_DEVICE_INSECURE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == Activity.RESULT_OK) 
				{
                    connectDevice(data, false);
					this.finish();
                }
                break;
            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode == Activity.RESULT_OK) 
				{
                    // Bluetooth is now enabled, so set up a chat session
                    mChatService = new BluetoothChatService(this, mHandler);
                }
				else 
				{
                    // User did not enable Bluetooth or an error occurred
                    //Log.d(TAG, "BT not enabled");
                    //Toast.makeText(getActivity(), R.string.bt_not_enabled_leaving,Toast.LENGTH_SHORT).show();
                    //getActivity().finish();
					Toast.makeText(this, R.string.bt_not_enabled_leaving,Toast.LENGTH_SHORT).show();
					finish();
                }
        }
    }

    /**
     * Establish connection with other device
     *
     * @param data   An {@link Intent} with {@link DeviceListActivity#EXTRA_DEVICE_ADDRESS} extra.
     * @param secure Socket Security type - Secure (true) , Insecure (false)
     */
    private void connectDevice(Intent data, boolean secure)
	{
        // Get the device MAC address
        String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        // Get the BluetoothDevice object
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        // Attempt to connect to the device
        mChatService.connect(device, secure);
    }

	public void	ConnectTodevice(boolean e_bSecureConnection)
	{
		//Intent serverIntent = new Intent(getActivity(), DeviceListActivity.class);
		Intent serverIntent = new Intent(this, DeviceListActivity.class);
		if(e_bSecureConnection)
			startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_SECURE);
		else
			startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE_INSECURE);
	}
}