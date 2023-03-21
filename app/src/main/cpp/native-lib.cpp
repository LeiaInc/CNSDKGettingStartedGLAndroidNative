#include <jni.h>
#include <string>

// CNSDK includes
#include <leia/sdk/sdk.hpp>
#include "leia/sdk/interlacer.hpp"
#include "leia/sdk/debugMenu.hpp"
#include "leia/common/platform.hpp"

leia::sdk::ILeiaSDK* g_sdk = nullptr;
leia::sdk::IThreadedInterlacer* g_interlacer = nullptr;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_initializeCNSDK(
        JNIEnv* env,
        jobject activity,
        jobject context) {

    // Create SDK.
    g_sdk = leia::sdk::CreateLeiaSDK();

    // Initialize Platform.
    leia::PlatformInitArgs pia = {};
    pia.androidActivity = activity;
    pia.androidContext = context;
    g_sdk->InitializePlatform(pia);

    // Initialize SDK.
    g_sdk->Initialize(nullptr); /// <----- crashes in here calling the BaseServiceConnection class constructor

    // Create interlacer.
    leia::sdk::ThreadedInterlacerInitArgs tiia = {};
    tiia.graphicsAPI = leia::sdk::GraphicsAPI::OpenGL;
    tiia.useMegaTextureForViews = true;
    g_interlacer = g_sdk->CreateNewThreadedInterlacer(tiia);

    // Initialize OpenGL.
    g_interlacer->InitializeOpenGL(nullptr, leia::sdk::eLeiaTaskResponsibility::SDK,leia::sdk::eLeiaTaskResponsibility::SDK,leia::sdk::eLeiaTaskResponsibility::SDK);

    return true;
}

extern "C" JNIEXPORT jint JNICALL
Java_com_leia_cnsdkgettingstartedglandroidnative_MainActivity_getRenderTargetForView(
        JNIEnv* env,
        jobject activity,
        jint viewIndex) {

    return g_interlacer->GetRenderTargetForView(viewIndex);
}

// todo: getRenderTargetWidth()
// todo: getRenderTargetHeight()
// todo: getConvergedPerspectiveViewInfo()
// todo: doPostProcess()
// todo: getConvergenceDistance()
// todo: shutdownCNSDK()