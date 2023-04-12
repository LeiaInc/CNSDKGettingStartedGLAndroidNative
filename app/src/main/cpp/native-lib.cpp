#include <jni.h>
#include <string>

// CNSDK includes
#include <leia/sdk/sdk.hpp>
#include "leia/sdk/interlacer.hpp"
#include "leia/sdk/debugMenu.hpp"
#include "leia/common/platform.hpp"

// Global variables
leia::sdk::ILeiaSDK*            g_sdk                  = nullptr;
leia::sdk::IThreadedInterlacer* g_interlacer           = nullptr;
bool                            g_isCNSDKInitOk        = false;
bool                            g_isGraphicsInitOk     = false;
bool                            g_showGUI              = true;
glm::vec3                       g_viewPos              = {};
glm::mat4                       g_viewProjectionMatrix = {};
float                           g_viewFieldOfView      = 0.0f;
float                           g_viewShearX           = 0.0f;
float                           g_viewShearY           = 0.0f;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doCNSDKInit(
        JNIEnv* env,
        jobject activity) {

    // If the CNSDK hasn't been initialized yet.
    if (!g_isCNSDKInitOk) {

        // Create SDK.
        g_sdk = leia::sdk::CreateLeiaSDK();

        // Initialize Platform.
        leia::PlatformInitArgs pia = {};
        pia.androidActivity = activity;
        g_sdk->InitializePlatform(pia);

        // Initialize SDK.
        g_sdk->Initialize(nullptr);

        // Create interlacer.
        leia::sdk::ThreadedInterlacerInitArgs tiia = {};
        tiia.graphicsAPI = leia::sdk::GraphicsAPI::OpenGL;
        tiia.useMegaTextureForViews = true;
        g_interlacer = g_sdk->CreateNewThreadedInterlacer(tiia);

        g_sdk->SetBacklight(true);

        g_interlacer->SetBaselineScaling(20.0f);

        // CNSDK initialization is complete.
        g_isCNSDKInitOk = true;
    }

    return true;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doGraphicsInit(
        JNIEnv* env,
        jobject activity) {

    // If we havn't initialized the graphics yet.
    if (!g_isGraphicsInitOk) {

        // Wait for CNSDK to be initialized.
        if (g_sdk->IsInitialized()) {

            // Initialize graphics.
            g_interlacer->InitializeOpenGL(nullptr, leia::sdk::eLeiaTaskResponsibility::SDK,leia::sdk::eLeiaTaskResponsibility::SDK,leia::sdk::eLeiaTaskResponsibility::SDK);

            // Initialize interlacer GUI.
            if (g_showGUI)
            {
                leia::sdk::DebugMenuInitArgs debugMenuInitArgs;
                g_interlacer->InitializeGui(debugMenuInitArgs);
            }

            // Graphics initialization complete.
            g_isGraphicsInitOk = true;
        }
    }

    return g_isGraphicsInitOk;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getRenderTargetForView(
        JNIEnv* env,
        jobject activity,
        jint viewIndex) {

    return g_interlacer->GetRenderTargetForView(viewIndex);
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doPostProcess(
        JNIEnv* env,
        jobject activity,
        int width,
        int height) {

    if (g_interlacer->IsOnSameThread(std::this_thread::get_id()))
        g_interlacer->DoPostProcess(width, height, false, 0);
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_calculateConvergedPerspectiveViewInfo(
        JNIEnv* env,
        jobject activity,
        int viewIndex,
        float cameraPosX, float cameraPosY, float cameraPosZ,
        float cameraDirX, float cameraDirY, float cameraDirZ,
        float cameraUpX, float cameraUpY, float cameraUpZ,
        float fieldOfView,
        float aspectRatio,
        float nearPlane,
        float farPlane)
{
    g_interlacer->GetConvergedPerspectiveViewInfo
    (
        viewIndex,
        glm::vec3(cameraPosX, cameraPosY, cameraPosZ),
        glm::vec3(cameraDirX, cameraDirY, cameraDirZ),
        glm::vec3(cameraUpX, cameraUpY, cameraUpZ),
        fieldOfView,
        aspectRatio,
        nearPlane,
        farPlane,
        &g_viewPos,
        &g_viewProjectionMatrix,
        &g_viewFieldOfView,
        &g_viewShearX,
        &g_viewShearY
    );
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewPosition(
        JNIEnv* env,
        jobject activity,
        int elementIndex)
{
    return g_viewPos[elementIndex];
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewProjectionMatrix(
        JNIEnv* env,
        jobject activity,
        int elementIndex)
{
    return g_viewProjectionMatrix[elementIndex/4][elementIndex%4];
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_doCNSDKShutdown(
        JNIEnv* env,
        jobject activity)
{
    delete g_interlacer;
    g_sdk->SetBacklight(false);
    g_sdk->Destroy();
}

extern "C" JNIEXPORT void JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_setConvergenceDistance(
        JNIEnv* env,
        jobject activity,
        float distance)
{
    g_interlacer->SetConvergenceDistance(distance);
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getViewWidth(
        JNIEnv* env,
        jobject activity)
{
    return g_sdk->GetViewWidth();
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getViewHeight(
        JNIEnv* env,
        jobject activity)
{
    return g_sdk->GetViewHeight();
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_processGuiMotionInput(
        JNIEnv* env,
        jobject activity,
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
        JNIEnv* env,
        jobject activity)
{
    if (g_interlacer != nullptr)
        return g_interlacer->IsGuiVisible();
    return false;
}
