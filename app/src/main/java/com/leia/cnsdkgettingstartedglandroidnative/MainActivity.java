package com.leia.cnsdkgettingstartedglandroidnative;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.widget.TextView;

import com.leia.core.Vector3;
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

    @Override
    public void onDestroy() {
        doCNSDKShutdown();  // todo: not called???
        super.onDestroy();
    }

    public native boolean doCNSDKInit();
    public native boolean doGraphicsInit();
    public native int getRenderTargetForView(int viewIndex);
    public native void doPostProcess(int width, int height);
    public native void calculateConvergedPerspectiveViewInfo(int viewIndex, float cameraPosX, float cameraPosY, float cameraPosZ,
                                                       float cameraDirX, float cameraDirY, float cameraDirZ,
                                                       float cameraUpX, float cameraUpY, float cameraUpZ,
                                                       float fieldOfView,
                                                       float aspectRatio,
                                                       float nearPlane,
                                                       float farPlane);
    public native float getConvergedPerspectiveViewPosition(int elementIndex);
    public native float getConvergedPerspectiveViewProjectionMatrix(int elementIndex);
    public native void doCNSDKShutdown();
    public native void setConvergenceDistance(float distance);
    public native int getViewWidth();
    public native int getViewHeight();


}