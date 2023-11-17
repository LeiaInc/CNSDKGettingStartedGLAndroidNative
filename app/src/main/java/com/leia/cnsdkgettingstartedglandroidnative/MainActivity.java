package com.leia.cnsdkgettingstartedglandroidnative;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.MotionEvent;

import com.leia.cnsdkgettingstartedglandroidnative.databinding.ActivityMainBinding;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'cnsdkgettingstartedglandroidnative' library on application startup.
    static {
        System.loadLibrary("cnsdkgettingstartedglandroidnative");
    }

    private ActivityMainBinding binding      = null;
    private MainView            mainView     = null;
    private MainRenderer        mainRenderer = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        // Hide title bar.
        this.getSupportActionBar().hide();

        // Set fullscreen.
        final android.view.WindowInsetsController insetsController = getWindow().getInsetsController();
        if (insetsController != null)
            insetsController.hide(android.view.WindowInsets.Type.statusBars());

        // Set the content view.
        binding = ActivityMainBinding.inflate(getLayoutInflater());
        setContentView(binding.getRoot());

        // Create renderer.
        mainRenderer = new MainRenderer(this);

        // Create view.
        mainView = (MainView) findViewById(R.id.mainView);
        mainView.setRenderer(mainRenderer);
        mainView.setActivity(this);
    }

    @Override
    public void onDestroy() {
        doCNSDKShutdown();
        super.onDestroy();
    }

    public native boolean doCNSDKInit();
    public native boolean doGraphicsInit();
    public native int getRenderTargetForView(int viewIndex);
    public native void doPostProcess(int width, int height);
    public native void setBaselineScaling(float baseline);
    public native float getViewComfortZoneNear(float frustumNearPlane, float frustumFarPlane);
    public native float getViewComfortZoneFar(float frustumNearPlane, float frustumFarPlane);
    public native void calculateConvergedPerspectiveViewInfo(
        int viewIndex,
        float cameraPosX, float cameraPosY, float cameraPosZ,
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
    public native boolean processGuiMotionInput(MotionEvent motionEvent);
    public native boolean isGuiVisible();
}
