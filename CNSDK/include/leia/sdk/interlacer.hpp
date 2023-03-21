#pragma once

#include "leia/sdk/gui.hpp"
#include "leia/sdk/sdk.hpp"

// TODO: remove glm from the public API
#include <glm/glm.hpp>

#if defined(LEIA_USE_NEW_RENDERER)
namespace LeiaRenderer
{
    class IRenderer;
    class IShader;
    class ITexture;
    class IRenderState;
}
#endif

#if defined(LEIA_USE_OPENGL)
#include "leia/common/opengl/utils.hpp"
#endif

#if defined(LEIA_USE_DIRECTX)
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11ShaderResourceView;
struct ID3D11DepthStencilView;
#endif

#if defined(LEIA_USE_DIRECTX12)
struct ID3D12Device;
struct ID3D12CommandQueue;
struct ID3D12Resource;
struct CD3DX12_CPU_DESCRIPTOR_HANDLE;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
typedef enum D3D12_RESOURCE_STATES;
#endif

#if defined(LEIA_USE_VULKAN)
// Note: We forward-declare Vulkan objects to avoid including the Vulkan SDK header.
//       Unfortunately we need two untyped enums VkFormat and VkImageLayout that can't be forward-declared on some compilers.
//       Our solution is to declare and use two integer types VkFormatInt and VkImageLayoutInt that can safely be cast to/from the Vulkan enums.
typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkFramebuffer_T* VkFramebuffer;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkSemaphore_T* VkSemaphore;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkQueue_T* VkQueue;
typedef int VkFormatInt;
typedef int VkImageLayoutInt;
#endif

struct leia_sdk_image_desc;

namespace leia {
namespace sdk {

struct LeiaInterlaceParameters;
struct DebugMenuInitArgs;

struct ThreadedInterlacerInitArgs {
    bool isProtected = false;
	bool useMegaTextureForViews = false;
    InterlacerDelegate* delegate = nullptr;
    leia::sdk::GraphicsAPI graphicsAPI = leia::sdk::GraphicsAPI::OpenGL;
};

struct IThreadedInterlacer {
    virtual void InitOnDesiredThread() = 0;
    virtual void Shutdown() = 0;
    virtual int GetNumViews() const = 0;
    virtual glm::vec3 GetViewOffset(int view) const = 0;
    virtual bool IsOnSameThread(std::thread::id const& callingThread) const = 0;
    virtual void SetFlipInputUVVertical(bool) = 0;
    virtual void SetShaderDebugMode(eLeiaShaderDebugMode mode) = 0;
    virtual void SetInterlaceMode(eLeiaInterlaceMode _interlaceMode) = 0;
    virtual void SetSourceViewsSize(int sourceWidth, int sourceHeight, bool isHorizontalViews) = 0;
    virtual void SetSingleViewMode(bool setSingleView) = 0;
    virtual void SetAlpha(float newAlpha) = 0;
    virtual void SetViewportPosition(int newXPos, int newYPos) = 0;
    virtual void ApplyInterlacing(int width, int height, bool blended) = 0;
    virtual void ApplySharpening(int width, int height, bool blended) = 0;
    virtual bool UpdateBuffers(int viewCount, eBufferType bufferType, int width, int height) = 0;
    virtual float GetConvergenceDistance() const = 0;
    virtual void SetConvergenceDistance(float distance) = 0;
    virtual float GetBaselineScaling() const = 0;
    virtual void SetBaselineScaling(float baseline) = 0;

    virtual void GetConvergedOrthographicViewInfo
    (
        int        viewIndex,
        const      glm::vec3& cameraPos,
        const      glm::vec3& cameraDir,
        const      glm::vec3& cameraUp,
        float      width,
        float      height,
        float      nearPlane,
        float      farPlane,
        glm::vec3* viewPos              = nullptr,
        glm::mat4* viewProjectionMatrix = nullptr,
        float*     viewShearX           = nullptr,
        float*     viewShearY           = nullptr
    ) = 0;

    virtual void GetConvergedPerspectiveViewInfo
    (
        int              viewIndex,
        const glm::vec3& cameraPos,
        const glm::vec3& cameraDir,
        const glm::vec3& cameraUp,
        float            fieldOfView,
        float            aspectRatio,
        float            nearPlane,
        float            farPlane,
        glm::vec3*       viewPos              = nullptr,
        glm::mat4*       viewProjectionMatrix = nullptr,
        float*           viewFieldOfView      = nullptr,
        float*           viewShearX           = nullptr,
        float*           viewShearY           = nullptr
    ) = 0;

#if defined(LEIA_USE_OPENGL)
    virtual void InitializeOpenGL(HGLRC context, eLeiaTaskResponsibility buffersResponsibility, eLeiaTaskResponsibility shadersResponsibility, eLeiaTaskResponsibility renderingResponsibility) = 0;
    virtual uint32_t GetRenderTargetForView(int view) = 0;
    virtual void SetViewTextureId(int newId) = 0;
    virtual void SetInterlaceViewTextureAtlas(uint32_t texture) = 0;
    virtual void SetOutputRenderTarget(uint32_t renderTarget) = 0;
    virtual void DoPostProcess(int width, int height, bool blendedInterlace = false, uint32_t outputRenderTarget = 0) = 0;
    virtual void DoPostProcessPicture(int width, int height, uint32_t views) = 0;
    virtual void DoPostProcessVideo(int width, int height, uint32_t views) = 0;
    virtual uint32_t CreateGLTexture(leia_sdk_image_desc const&) = 0;
#endif

#if defined(LEIA_USE_DIRECTX)
    virtual void InitializeD3D11(ID3D11DeviceContext* context, eLeiaTaskResponsibility buffersResponsibility, eLeiaTaskResponsibility shadersResponsibility, eLeiaTaskResponsibility renderingResponsibility) = 0;
    virtual void DoPostProcess(int width, int height, bool blendedInterlace, ID3D11RenderTargetView* resultTexture) = 0;
    virtual void DoPostProcessPicture(int width, int height, ID3D11ShaderResourceView* pictureShaderResourceView, ID3D11RenderTargetView* renderTargetView) = 0;
    virtual void SetViewTextureId(ID3D11ShaderResourceView* view) = 0;
    virtual void SetInterlaceViewTextureAtlas(ID3D11ShaderResourceView* texture) = 0;
    virtual ID3D11RenderTargetView* GetRenderTargetView(int view) = 0;
    virtual ID3D11DepthStencilView* GetDepthStencilView(int view) = 0;
#endif

#if defined(LEIA_USE_DIRECTX12)
    virtual void InitializeD3D12(ID3D12Device* device, ID3D12CommandQueue* commandQueue, eLeiaTaskResponsibility _buffersResponsibility, eLeiaTaskResponsibility _shadersResponsibility, eLeiaTaskResponsibility _renderingResponsibility) = 0;
    virtual void GetRenderTargetView(int view, CD3DX12_CPU_DESCRIPTOR_HANDLE* handle) = 0;
    virtual ID3D12Resource* GetRenderTargetResource(int view, D3D12_RESOURCE_STATES* state) = 0;
    virtual void GetDepthStencilView(int view, CD3DX12_CPU_DESCRIPTOR_HANDLE* handle) = 0;
    virtual ID3D12Resource* GetDepthStencilResource(int view, D3D12_RESOURCE_STATES* state) = 0;
    virtual void DoPostProcess(int width, int height, bool blendedInterlace, ID3D12Resource* renderTarget) = 0;
    virtual void DoPostProcessPicture(int width, int height, ID3D12Resource* pictureResource, ID3D12Resource* renderTarget) = 0;
    virtual void SetViewTextureId(ID3D12Resource* view) = 0;
    virtual void SetInterlaceViewTextureAtlas(ID3D12Resource* texture) = 0;
#endif

#if defined(LEIA_USE_VULKAN)
    virtual void InitializeVulkan(VkDevice device, VkPhysicalDevice physicalDevice, VkFormatInt textureFormat, VkFormatInt renderTargetFormat, VkFormatInt depthStencilFormat, int maxInFlightFrameCount, eLeiaTaskResponsibility _buffersResponsibility, eLeiaTaskResponsibility _shadersResponsibility, eLeiaTaskResponsibility _renderingResponsibility) = 0;
    virtual void GetRenderTargetView(int view, VkImageView* handle) = 0;
    virtual VkFramebuffer GetFramebuffer(int view) = 0;
    virtual VkImage GetRenderTargetImage(int view, VkImageLayoutInt* layout = nullptr) = 0;
    virtual VkImage GetDepthStencilImage(int view, VkImageLayoutInt* layout = nullptr) = 0;
    virtual int GetRenderTargetImageWidth() = 0;
    virtual int GetRenderTargetImageHeight() = 0;
    virtual void DoPostProcess(int width, int height, bool blendedInterlace, VkFramebuffer frameBuffer, VkImage colorImage, VkImage depthImage, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, int currentFrame) = 0;
    virtual void DoPostProcessPicture(int width, int height, VkImage pictureResource, VkFramebuffer frameBuffer, VkImage colorImage, VkImage depthImage, VkSemaphore imageAvailableSemaphore, VkSemaphore renderFinishedSemaphore, int currentFrame) = 0;
    virtual void SetViewTextureId(VkImage view) = 0;
    virtual void SetInterlaceViewTextureAtlas(VkImage texture) = 0;
#endif


    virtual void OnWindowSizeChanged(int width, int height) = 0;
    virtual void GetInterlaceParameters(LeiaInterlaceParameters& interlaceParameters) const = 0;
    virtual std::string GetInterlaceShaderText(eLeiaShaderType shaderType) const = 0;
    virtual std::string GetSharpenShaderText(eLeiaShaderType shaderType) const = 0;

    virtual void ToggleInterlaceMode() = 0;
    virtual void ToggleShaderDebugMode() = 0;

    virtual void SetFitMode(eFitMode mode) = 0;
    virtual eFitMode GetFitMode() const = 0;
    virtual void SetCustomTextureMatrix(const glm::mat4& textureTransform) = 0;
	virtual void GetFocusZoom(float *scaleFocus) = 0;
	virtual void SetFocusZoom(float scaleX, float scaleY, float focusX, float focusY) = 0;
    virtual void EnableReconvergence(bool enable) = 0;
    virtual bool IsReconvergenceEnabled() const = 0;
    virtual void SetReconvergence(float value) = 0;
    virtual void SetReconvergenceZoom(bool zoomX, bool zoomXY) = 0;
    virtual float GetReconvergence() const = 0;

    virtual void InitializeGui(DebugMenuInitArgs const&) = 0;
    virtual void ShutdownGui() = 0;
    virtual bool ToggleGui() = 0;
    virtual bool IsGuiVisible() = 0;
#if defined(LEIA_OS_WINDOWS)
    virtual Gui::InputState ProcessGuiInput(GuiSurface surface, uint32_t msg, uint64_t wparam, int64_t lparam) = 0;
#elif defined(LEIA_OS_ANDROID)
    virtual Gui::InputState ProcessGuiInput(AInputEvent const*) = 0;
    virtual Gui::InputState ProcessGuiMotionInput(JNIEnv*, jobject motionInput) = 0;
#endif
    virtual void RenderGui(int width, int height) = 0;
    virtual void SetAspectRatioOffset(float offset) = 0;
    virtual float GetAspectRatioOffset() const = 0;
};

} // namespace sdk
} // namespace leia
