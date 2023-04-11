#include <jni.h>
#include <string>

// CNSDK includes
#include <leia/sdk/sdk.hpp>
#include "leia/sdk/interlacer.hpp"
#include "leia/sdk/debugMenu.hpp"
#include "leia/common/platform.hpp"

// Global variables
leia::sdk::ILeiaSDK*            g_sdk              = nullptr;
leia::sdk::IThreadedInterlacer* g_interlacer       = nullptr;
bool                            g_isCNSDKInitOk    = false;
bool                            g_isGraphicsInitOk = false;

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

// todo: getRenderTargetWidth()
// todo: getRenderTargetHeight()
// todo: getConvergedPerspectiveViewInfo()
// todo: doPostProcess()
// todo: getConvergenceDistance()
// todo: shutdownCNSDK()