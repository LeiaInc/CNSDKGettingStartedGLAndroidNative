# Leia CNSDK For Android Java Activity Applications

## Purpose
Provide a tutorial that shows how to create a lightfield application in Java using JNI to access the Leia CNSDK.


## What is the Leia CNSDK?

The Leia CNSDK is middleware in the form of C++ and Java libraries and headers. It was created for application developers targeting Leia lightfield devices such as the LumePad 2, and provides a simple API that performs all necessary eye-tracking and graphics.

While not absolutely required, the CNSDK will allow you to harness the ability of your lightfield device in a simple manner. Any developer attempting to create Leia lightfield 3D experiences without CNSDK will encounter significant technical challenges. We therefore highly suggest using CNSDK for your application.

The CNSDK provides the following mechanisms to access functionality depending on your situation:

1. For Windows C++ applications, use classes `leia::sdk::Core` and `leia::sdk::Interlacer`.
2. For simple Android Java applications, use Leia-provided Java classes such as `LeiaSDK`, `InterlacedSurfaceView`, and `InputViewsAsset`.
3. For advanced Android Java applications, use Java JNI to access C++ classes directly.

In this tutorial we will focus on advanced Android Java applications with JNI.


## Project Setup

The provided sample project is a Java activity created in Android Studio 2021.2.1. It consists of 4 major parts:
1.	MainActivity.java – Small file containing some initialization code and the Java-side of the JNI interface.
2.	MainView.java – Class that represents a surface that can be displayed. It is an extension of `GLSurfaceView`.
3.	MainRenderer.java – All rendering is contained in this file. It implements base class `GLSurfaceView.Renderer`.
4.	C++ Library – The C++-side of the JNI interface. All CNSDK-related functionality is contained in this library.

To create a project with a similar structure to the provided example, follow these directions:
1.	Start Android Studio and create an empty Java project.
2.	Add the Leia CNSDK zip file to the project root directory and edit the project build.gradle file to extract the CNSDK files. This should look similar to the block below:
```
project.ext {
    cnsdkArchive = "cnsdk-android-${project.cnsdkVersion}.zip"
    cnsdkArchiveFile = new File(cnsdkArchive)
    if (!cnsdkArchiveFile.exists()) {
        def cnsdkUrl = "https://github.com/LeiaInc/leiainc.github.io/raw/master/CNSDK/cnsdk-android-${project.cnsdkVersion}.zip"
        new URL(cnsdkUrl).withInputStream{ i -> cnsdkArchiveFile.withOutputStream{ it << i }}
    } else {
        println("CNSDK archive is already downloaded")
    }
    cnsdkPath = "${project.buildDir}/cnsdk"
    println("cnsdkPath: ${cnsdkPath}")
    copy {
        from zipTree(cnsdkArchive)
        include 'android/**/*'
        include 'include/**/*'
        include 'lib/**/*'
        include 'share/**/*'
        into "${cnsdkPath}"
    }
    cnsdkAar = "${cnsdkPath}/android/sdk-faceTrackingService-${project.cnsdkVersion}.aar"
}
```
3.	Add empty MainActivity.java file and modify AndroidManifest.xml to include the activity. The activity block in the manifest file should look similar to the block below:
```
<activity
    android:name=".MainActivity"
    android:exported="true">
    <intent-filter>
        <action android:name="android.intent.action.MAIN" />
        <category android:name="android.intent.category.LAUNCHER" />
    </intent-filter>
</activity>
```
4.	Add a layout file by clicking File/New/XML/Layout XML File and name it activity_main.xml.
5.	Add MainRenderer.java class and include these placeholder methods:
```
package com.leia.cnsdkgettingstartedglandroidnative;

import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MainRenderer implements GLSurfaceView.Renderer {

    MainRenderer() {
    }

    @Override
    public void onDrawFrame(GL10 gl) {
    }

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
    }
}
```
6.	Add MainView.java class and include these placeholder methods:
```
package com.leia.cnsdkgettingstartedglandroidnative;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;

public class MainView extends GLSurfaceView {

    public MainView(Context context){
        super(context);
    }

    public MainView(Context context, AttributeSet attrs){
        super(context, attrs);
    }
}
```
7.	Add a new C++ library by creating a directory named {ROOT}/app/src/main/cpp and adding files CMakeLists.txt and native-lib.cpp.
8.	Edit your CMakeLists.txt file as follows:
```
cmake_minimum_required(VERSION 3.18.1)
project("cnsdkgettingstartedglandroidnative")
add_library(cnsdkgettingstartedglandroidnative SHARED native-lib.cpp)
find_package(CNSDK CONFIG REQUIRED)
target_link_libraries(cnsdkgettingstartedglandroidnative PRIVATE
    CNSDK::leiaSDK)
```
9.	Once setup, your project should look like this:

| ![alt text](https://github.com/LeiaInc/CNSDKGettingStartedGLAndroidNative/blob/main/ReadmeFiles/Project.png) |
|-|

## Project Implementation

Now that the project is setup, we need to implement the 3 java classes, the C++ library, 
The project will render a 3D spinning cube.
First, we need to setup the layout for the application. To do this, double-click the activity_main.xml file and add a view based on `com.leia.cnsdkgettingstartedglandroidnative.MainView` with id `mainView`.

### MainActivity.java
This is a simple class that performs some initialization and exposes the Java-side for the JNI interface. To access the C++ library, add the following lines to the top of the class:
```
access the C++ library, add the following lines to the top of the class:
static {
    System.loadLibrary("cnsdkgettingstartedglandroidnative");
}
```
Declare the following class variables:
```
private ActivityMainBinding binding      = null;
private MainView            mainView     = null;
private MainRenderer        mainRenderer = null;
```
Implement the OnCreate() override method. We first disable the title-bar and request full-screen. Then we set the content view, create the renderer, and create the view.
```
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
```
Implement the OnDestroy() override method. This gives the application a chance to shutdown the CNSDK gracefully.
```
@Override
public void onDestroy() {
    doCNSDKShutdown();
    super.onDestroy();
}
```
Declare the Java-side for our JNI library. In this tutorial, we have 13 functions that are implemented in approximately 200 lines of C++ code. This is sufficient for us to render, interlace, and display a spinning 3D cube. The functions are:
```
public native boolean doCNSDKInit();
public native boolean doGraphicsInit();
public native int getRenderTargetForView(int viewIndex);
public native void doPostProcess(int width, int height);
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
```

### MainView.java
This class represents a visible surface we can display. Our implementation does little more than simple initialization and handling user touch events to drive the (optional) CNSDK debug menu.
The class has two constructors that call a common initialization function:
```
public MainView(Context context){
    super(context);
    init();
}

public MainView(Context context, AttributeSet attrs){
    super(context, attrs);
    init();
}
```
The initialization function simply selects OpenGL ES 3.0 and avoids a destroyed context when the application is paused:
```
private void init() {
    setEGLContextClientVersion(3);
    setPreserveEGLContextOnPause(true);
}
```
Finally, we catch user touch events to drive the debug menu:
```
@Override
public boolean onTouchEvent(MotionEvent event) {
    if (mainActivity.isGuiVisible()) {
        queueEvent(() -> mainActivity.processGuiMotionInput(event));
        return true;
    }

    return super.onTouchEvent(event);
}
```
### MainRenderer.java
This class is where all the CNSDK initialization and graphics is performed.
We perform initialization in a deferred manner using the helper method below. It performs initialization of the CNSDK core, CNSDK graphics, and this application’s graphics in order. Please refer to the source code for the implementation of `doInternalInit()` which performs standard OpenGL initialization required for simple rendering.
```
boolean doDeferredInitialization()
{
    // Initialize CNSDK.
    if (!activity.doCNSDKInit())
        return false;

    // Initialize graphics used with CNSDK.
    if (!activity.doGraphicsInit())
        return false;

    // Initialize our internal resources.
    if (!doInternalInit())
        return false;

    // Ready to use CNSDK.
    return true;
}
```
The initialization function is called on the onDrawFrame
```
@Override
public void onDrawFrame(GL10 gl) {

    if (!doDeferredInitialization())
        return;

    // Perform all rendering here...
}
```
For this tutorial, we are rendering a spinning cube in stereoscopic mode. This means we render once from the left eye’s perspective and once from the right eye’s perspective. Additionally, we are using a single texture atlas as a render-target. This means we will render the left view into the left half of the render-target and the right view into the right half of the render-target.
To get the render-target to render into, do as follows:
```
int renderTarget = activity.getRenderTargetForView(0);
```
Then the render-target can be cleared:
```
GLES30.glBindFramebuffer(GLES30.GL_FRAMEBUFFER, renderTarget);
GLES30.glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);
```
In the main loop, we render twice, using the OpenGL viewport to restrict output to the correct side of the render-target. After getting the per-view settings, we compute a matrix to transform and project our cube, set the transform to our shader uniform location, and draw.
```
for (int viewIndex = 0; viewIndex < 2; viewIndex++)
{
    // Compute per-view values.
    activity.calculateConvergedPerspectiveViewInfo(
            viewIndex,
            cameraPos.x, cameraPos.y, cameraPos.z,
            cameraDir.x, cameraDir.y, cameraDir.z,
            cameraUp.x, cameraUp.y, cameraUp.z,
            fov,
            aspectRatio,
            nearz,
            farz);

    // Get the camera position for the current view.
    Vector3 viewCameraPos = new Vector3();
    viewCameraPos.x = activity.getConvergedPerspectiveViewPosition(0);
    viewCameraPos.y = activity.getConvergedPerspectiveViewPosition(1);
    viewCameraPos.z = activity.getConvergedPerspectiveViewPosition(2);

    // Get the camera target for the current view.
    Vector3 viewTargetPos = new Vector3();
    viewTargetPos.x = viewCameraPos.x + cameraDir.x;
    viewTargetPos.y = viewCameraPos.y + cameraDir.y;
    viewTargetPos.z = viewCameraPos.z + cameraDir.z;

    // Get the projection matrix for the current view.
    float viewProjectionMatrix[] = new float[16];
    for(int i=0; i<16; i++)
        viewProjectionMatrix[i] = activity.getConvergedPerspectiveViewProjectionMatrix(i);

    // Get camera transform.
    float cameraTransform[] = new float[16];
    Matrix.setLookAtM(cameraTransform, 0, viewCameraPos.x, viewCameraPos.y, viewCameraPos.z, viewTargetPos.x, viewTargetPos.y, viewTargetPos.z, cameraUp.x, cameraUp.y, cameraUp.z);

    // Compute combined matrix.
    float temp[] = new float[16];
    float mvp[] = new float[16];
    Matrix.multiplyMM(temp, 0, viewProjectionMatrix, 0, cameraTransform, 0);
    Matrix.multiplyMM(mvp, 0, temp, 0, geometryTransform, 0);

    // Render view into left or right of render-target.
    GLES30.glViewport(renderTargetWidth * viewIndex, 0, renderTargetWidth, renderTargetHeight);
    GLES30.glUseProgram(program);
    int tranformHandle = GLES30.glGetUniformLocation(program, "transform");
    GLES30.glUniformMatrix4fv(tranformHandle, 1, false, FloatBuffer.wrap(mvp));
    GLES30.glBindVertexArray(vao[0]);
    GLES30.glEnableVertexAttribArray(0);
    GLES30.glEnableVertexAttribArray(1);
    int triangles = 6 * 2;
    GLES30.glDrawElements(GLES30.GL_TRIANGLES, triangles * 3, GLES30.GL_UNSIGNED_SHORT, 0);
}
```
The final step is to perform the interlacing. We restore the previous viewport and interlace like this:
```
activity.doPostProcess(windowWidth, windowHeight);
```

### Native-lib.cpp
This class is where all the CNSDK-related logic is performed. The 13 methods declared in MainActivity.java are implemented here. Most methods are trivial trampoline methods that simply call into CNSDK. Only two methods, `doCNSDKInit()` and `doGraphicsInit()` have any significant logic.
In `doCNSDKInit()`, we initialize CNSDK:
```
// If the CNSDK hasn't been initialized yet.
if (!g_isCNSDKInitOk) {
    // Create SDK.
    leia::sdk::CoreInitConfiguration config;
    config.SetPlatformAndroidHandle(LEIA_CORE_ANDROID_HANDLE_ACTIVITY, activity);
    g_cnsdk = new leia::sdk::Core(config);

    // CNSDK initialization is complete.
    g_isCNSDKInitOk = true;
}
```
In `doGraphicsInit()`, we create an interlacer, set an initial baseline scaling value, enable the backlight, and optionally enable the CNSDK debug menu:
```
// If we havn't initialized the graphics yet.
if (!g_isGraphicsInitOk) {
    // Wait for CNSDK to be initialized.
    if (g_cnsdk->IsInitialized()) {
        // Create interlacer.
        leia::sdk::InterlacerInitConfiguration config;
        config.SetUseAtlasForViews(true);
        g_interlacer = new leia::sdk::InterlacerOpenGL(*g_cnsdk, config, nullptr);
        g_interlacer->SetBaselineScaling(20.0f);

        // Initialize interlacer GUI.
        if (g_showGUI)
        {
            leia_interlacer_debug_menu_configuration guiConfig = {};
            g_interlacer->InitializeGui(&guiConfig);
        }

        if (leia::device::Config* deviceConfig = g_cnsdk->GetDeviceConfig()) {
            g_viewWidth = deviceConfig->viewResolution[0];
            g_viewHeight = deviceConfig->viewResolution[1];
            g_cnsdk->ReleaseDeviceConfig(deviceConfig);
        }

        g_cnsdk->SetBacklight(true);

        // Graphics initialization complete.
        g_isGraphicsInitOk = true;
    }
}
```

### Running
Once you have implemented all required functionality, simply run the project by clicking Run/Run ‘app’ in Android Studio’s menu bar.
