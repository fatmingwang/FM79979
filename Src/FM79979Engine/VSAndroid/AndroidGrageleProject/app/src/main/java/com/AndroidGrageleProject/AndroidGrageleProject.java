
package com.AndroidGrageleProject;

import android.app.NativeActivity;
import android.app.Activity;
import android.widget.TextView;
import android.os.Bundle;

public class AndroidGrageleProject extends NativeActivity//Activity//NativeActivity
{
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState)
    {
        super.onCreate(savedInstanceState);

        /* Create a TextView and set its text to "Hello world" */
        TextView  tv = new TextView(this);
        tv.setText("Hello World!");
        setContentView(tv);
    }
    static { System.loadLibrary("AndroidGameApp"); }
}
