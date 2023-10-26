#include <android/log.h>
#include <jni.h>
#include <string>

// CNSDK includes
#include <leia/core/cxx/core.hpp>
#include <leia/core/cxx/interlacer.opengl.hpp>

// Global variables
leia::CoreLibrary*      g_coreLibrary          = nullptr;
leia::Core*             g_cnsdk                = nullptr;
leia::InterlacerOpenGL* g_interlacer           = nullptr;
bool                    g_isCNSDKInitOk        = false;
bool                    g_isGraphicsInitOk     = false;
bool                    g_showGUI              = false;
leia::Vector3           g_viewPos              = {};
leia::Mat4              g_viewProjectionMatrix = {};
float                   g_viewFieldOfView      = 0.0f;
float                   g_viewShearX           = 0.0f;
float                   g_viewShearY           = 0.0f;
int                     g_viewWidth            = 0;
int                     g_viewHeight           = 0;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doCNSDKInit(
        JNIEnv* jniEnv,
        jobject activity) try {

    // If the CNSDK hasn't been initialized yet.
    if (!g_isCNSDKInitOk) {
        // Load the core library.
        JavaVM* vm;
        jniEnv->GetJavaVM(&vm);
        leia_core_library_load_android androidInfo = {
            .vm = vm,
            .context = activity
        };
        g_coreLibrary = new leia::CoreLibrary{&androidInfo};

        // Create the core.
        leia::CoreInitConfiguration config{*g_coreLibrary};
        config.SetPlatformAndroidHandle(LEIA_CORE_ANDROID_HANDLE_ACTIVITY, activity);
        g_cnsdk = new leia::Core(config);

        // CNSDK initialization is complete.
        g_isCNSDKInitOk = true;
    }

    return true;
} catch (std::exception& e) {
    __android_log_print(ANDROID_LOG_ERROR, "Native-Lib", "Failed to init CNSDK: %s", e.what());
    return false;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doGraphicsInit(
        JNIEnv*,
        jobject) {

    // If we havn't initialized the graphics yet.
    if (!g_isGraphicsInitOk) {

        // Wait for CNSDK to be initialized.
        if (g_cnsdk->IsInitialized()) {

            // Create interlacer.
            leia::InterlacerInitConfiguration config;
            config.SetUseAtlasForViews(true);
            g_interlacer = new leia::InterlacerOpenGL(*g_cnsdk, config, nullptr);
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

    return g_isGraphicsInitOk;
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_setRenderTargets(
        JNIEnv*,
        jobject,
        jint viewsTexture,
        jint viewsTextureWidth,
        jint viewsTextureHeight,
        jint outputFramebuffer) {
    g_interlacer->SetSourceViews(viewsTexture);
    g_interlacer->SetSourceViewsSize(viewsTextureWidth, viewsTextureHeight, true);
    g_interlacer->SetOutputRenderTarget(outputFramebuffer);
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doPostProcess(
        JNIEnv*,
        jobject,
        int width,
        int height) {
    g_interlacer->DoPostProcess(width, height, false);
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_calculateConvergedPerspectiveViewInfo(
        JNIEnv*,
        jobject,
        int viewIndex,
        float cameraPosX, float cameraPosY, float cameraPosZ,
        float cameraDirX, float cameraDirY, float cameraDirZ,
        float cameraUpX, float cameraUpY, float cameraUpZ,
        float fieldOfView,
        float aspectRatio,
        float nearPlane,
        float farPlane)
{
    leia::Vector3 cameraPos{.x = cameraPosX, .y = cameraPosY, .z = cameraPosZ};
    leia::Vector3 cameraDir{.x = cameraDirX, .y = cameraDirY, .z = cameraDirZ};
    leia::Vector3 cameraUp{.x = cameraUpX, .y = cameraUpY, .z = cameraUpZ};
    g_interlacer->GetConvergedPerspectiveViewInfo
    (
        viewIndex,
        leia::ToConstSlice(&cameraPos),
        leia::ToConstSlice(&cameraDir),
        leia::ToConstSlice(&cameraUp),
        fieldOfView,
        aspectRatio,
        nearPlane,
        farPlane,
        leia::ToSlice(&g_viewPos),
        leia::ToSlice(&g_viewProjectionMatrix),
        &g_viewFieldOfView,
        &g_viewShearX,
        &g_viewShearY
    );
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewPosition(
        JNIEnv*,
        jobject,
        int elementIndex)
{
    return g_viewPos.v[elementIndex];
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewProjectionMatrix(
        JNIEnv*,
        jobject,
        int elementIndex)
{
    return g_viewProjectionMatrix.m[elementIndex];
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doCNSDKShutdown(
        JNIEnv*,
        jobject)
{
    if (g_isCNSDKInitOk) {
        g_isCNSDKInitOk = false;

        delete g_interlacer;
        g_interlacer = nullptr;

        g_cnsdk->SetBacklight(false);

        delete g_cnsdk;
        g_cnsdk = nullptr;
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_setConvergenceDistance(
        JNIEnv*,
        jobject,
        float distance)
{
    g_interlacer->SetConvergenceDistance(distance);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getViewWidth(
        JNIEnv*,
        jobject)
{
    return g_viewWidth;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getViewHeight(
        JNIEnv*,
        jobject)
{
    return g_viewHeight;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_processGuiMotionInput(
        JNIEnv* env,
        jobject,
        jobject motionEvent)
{
    if (g_interlacer->IsGuiVisible()) {
        g_interlacer->ProcessGuiMotionInput(env, motionEvent);
        return true;
    }

    return false;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_isGuiVisible(
        JNIEnv*,
        jobject)
{
    if (g_interlacer != nullptr)
        return g_interlacer->IsGuiVisible();
    return false;
}
