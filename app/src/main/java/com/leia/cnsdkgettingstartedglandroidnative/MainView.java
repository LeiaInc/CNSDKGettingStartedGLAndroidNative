package com.leia.cnsdkgettingstartedglandroidnative;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.MotionEvent;

public class MainView extends GLSurfaceView {

    MainActivity mainActivity = null;

    public MainView(Context context){
        super(context);
        init();
    }

    public MainView(Context context, AttributeSet attrs){
        super(context, attrs);
        init();
    }

    public void setActivity(MainActivity ma){
        mainActivity = ma;
    }

    private void init() {
        setEGLContextClientVersion(3);
        setPreserveEGLContextOnPause(true);
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (mainActivity.isGuiVisible()) {
            queueEvent(() -> mainActivity.processGuiMotionInput(event));
            return true;
        }

        return super.onTouchEvent(event);
    }
}
