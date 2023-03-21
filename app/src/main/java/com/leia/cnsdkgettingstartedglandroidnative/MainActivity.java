package com.leia.cnsdkgettingstartedglandroidnative;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.leia.sdk.LeiaSDK;

import com.leia.cnsdkgettingstartedglandroidnative.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {//} implements LeiaSDK.Delegate {

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
/*
    @Override
    protected void onResume() {
        super.onResume();
    }

    public void didInitialize(LeiaSDK leiaSDK) {
        //this.leiaSDK = leiaSDK;
        leiaSDK.enableBacklight(true);
    }

    public void onFaceTrackingStarted(LeiaSDK leiaSDK) {
        //Log.i(LogTag, "onFaceTrackingStarted");
    }

    public void onFaceTrackingStopped(LeiaSDK leiaSDK) {
        //Log.i(LogTag, "onFaceTrackingStopped");
    }

    public void onFaceTrackingFatalError(LeiaSDK leiaSDK) {
        LeiaSDK.FaceTrackingFatalError fatalError = leiaSDK.isFaceTrackingInFatalError();
        //if (fatalError != null)
          //  Log.e(LogTag, String.format("Face tracking fatal error: %s (%d)", fatalError.message, fatalError.code));
    }
*/
    public native boolean initializeCNSDK(Object context);
    public native int getRenderTargetForView(int viewIndex);
    public boolean isCNSDKInitialized = false;
}