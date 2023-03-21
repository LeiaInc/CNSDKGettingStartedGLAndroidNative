package com.leia.cnsdkgettingstartedglandroidnative;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MainView extends GLSurfaceView {

    public MainView(Context context){
        super(context);
        init();
    }

    public MainView(Context context, AttributeSet attrs){
        super(context, attrs);
        init();
    }

    private void init() {
        setEGLContextClientVersion(2);
        setPreserveEGLContextOnPause(true);
    }
}