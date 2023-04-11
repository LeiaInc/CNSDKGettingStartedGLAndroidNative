package com.leia.cnsdkgettingstartedglandroidnative;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.leia.sdk.LeiaSDK;

import com.leia.cnsdkgettingstartedglandroidnative.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'cnsdkgettingstartedglandroidnative' library on application startup.
    static {
        System.loadLibrary("cnsdkgettingstartedglandroidnative");
    }

    private ActivityMainBinding binding;
    private MainView mainView;
    private MainRenderer mainRenderer = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        mainRenderer = new MainRenderer(this);

        mainView = (MainView) findViewById(R.id.mainView);
        mainView.setRenderer(mainRenderer);
    }

    public native boolean doCNSDKInit();
    public native boolean doGraphicsInit();
    public native int getRenderTargetForView(int viewIndex);
    public native void doPostProcess(int width, int height);
}