#pragma once

#include "leia/sdk/api.h"
#include "leia/common/jniTypes.h"

#if defined(LEIA_OS_WINDOWS)
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif

#include <thread>
#include <cstdint>

struct AInputEvent;
struct ImGuiContext;

namespace leia {
namespace sdk {

#if defined(LEIA_OS_WINDOWS)
using GuiSurface = HWND;
#else
using GuiSurface = void*;
#endif

enum class GuiGraphicsAPI
{
    OpenGL,
    D3D11,
    D3D12,
    Vulkan
};

struct GuiInitArgs {
    float fontGlobalScale = 0.0f;
    GuiSurface surface = nullptr;
    void*      d3d11Device = nullptr;
    void*      d3d11DeviceContext = nullptr;
    void*      d3d12Device = nullptr;
    int        d3d12NumFramesInFlight = 0;
    int        d3d12RtvFormat = 0;
    void*      d3d12DeviceCbvSrvHeap = nullptr;
    uint64_t   d3d12FontSrvCpuDescHandle = 0;
    uint64_t   d3d12FontSrvGpuDescHandle = 0;
    void*      d3d12CommandList = nullptr;
    GuiGraphicsAPI graphicsAPI = GuiGraphicsAPI::OpenGL;
    void*    vulkanDevice         = nullptr;
    void*    vulkanDescriptorPool = nullptr;
    uint32_t vulkanImageCount     = 0;
    void*    vulkanInstance       = nullptr;
    uint32_t vulkanMinImageCount  = 0;
    int      vulkanMSAASamples    = 0;
    void*    vulkanPhysicalDevice = nullptr;
    void*    vulkanPipelineCache  = nullptr;
    void*    vulkanQueue          = nullptr;
    uint32_t vulkanQueueFamily    = 0;
    uint32_t vulkanSubpass        = 0;
    void*    vulkanRenderPass     = nullptr;
    void*    vulkanCommandBuffer  = nullptr;
    void*    vulkanCommandPool    = nullptr;
};

class Gui {
public:
    bool isVisible = true;

    LEIASDK_API
    Gui(GuiInitArgs const&);

    LEIASDK_API
    ~Gui();

    struct InputState {
        bool wantCaptureInput = false;     // if true, do not process any input
        bool wantCaptureMouse = false;     // if true, do not process mouse input
        bool wantCaptureKeyboard = false;  // if true, do not process keyboard input
        bool wantTextInput = false;        // if true, open textual keyboard input
    };

#if defined(LEIA_OS_WINDOWS)
    LEIASDK_API
    InputState ProcessInput(GuiSurface surface, uint32_t msg, uint64_t wparam, int64_t lparam);
#elif defined(LEIA_OS_ANDROID)
    LEIASDK_API
    InputState ProcessInput(AInputEvent const* inputEvent);
    InputState ProcessInputMotion(JNIEnv* jniEnv, jobject motionEvent);
#endif

    LEIASDK_API
    bool NewFrame(int width, int height);

    LEIASDK_API
    void Render();

private:
    ImGuiContext* _imGuiContext;
    std::thread::id _owningThreadId;
    void* _d3d12CommandList = nullptr;
    void* _vulkanCommandBuffer = nullptr;
    GuiGraphicsAPI graphicsAPI = GuiGraphicsAPI::OpenGL;
};

} // namespace sdk
} // namespace leia
