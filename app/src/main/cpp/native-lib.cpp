#include <jni.h>
#include <string>

// CNSDK includes
#include <leia/sdk/sdk.hpp>
#include "leia/sdk/interlacer.hpp"
#include "leia/sdk/debugMenu.hpp"
#include "leia/common/platform.hpp"

struct ViewInfo
{
    float viewPos[3];
    float viewProjectionMatrix[16];
    float viewFieldOfView;
    float viewShearX;
    float viewShearY;
};

// Global variables
leia::sdk::ILeiaSDK*            g_sdk              = nullptr;
leia::sdk::IThreadedInterlacer* g_interlacer       = nullptr;
bool                            g_isCNSDKInitOk    = false;
bool                            g_isGraphicsInitOk = false;
ViewInfo                        g_viewInfo         = {};
int                             g_viewWidth        = 0;
int                             g_viewHeight       = 0;
bool                            g_showGUI          = true;

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
    glm::vec3 viewPos = {};
    glm::mat4 viewProjectionMatrix = {};

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
        &viewPos,
        &viewProjectionMatrix,
        &g_viewInfo.viewFieldOfView,
        &g_viewInfo.viewShearX,
        &g_viewInfo.viewShearY
    );

    g_viewInfo.viewPos[0] = viewPos[0];
    g_viewInfo.viewPos[1] = viewPos[1];
    g_viewInfo.viewPos[2] = viewPos[2];
    for (int i=0; i<16; i++)
        g_viewInfo.viewProjectionMatrix[i] = viewProjectionMatrix[i/4][i%4];
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewPosition(
        JNIEnv* env,
        jobject activity,
        int elementIndex)
{
    return g_viewInfo.viewPos[elementIndex];
}

extern "C" JNIEXPORT jfloat JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getConvergedPerspectiveViewProjectionMatrix(
        JNIEnv* env,
        jobject activity,
        int elementIndex)
{
    return g_viewInfo.viewProjectionMatrix[elementIndex];
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