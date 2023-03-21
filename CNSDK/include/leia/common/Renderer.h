#pragma once

#include "leia/common/api.h"

#if defined(_WIN32)
#include <Windows.h> // HGLRC, HDC
#endif

#include <string>
#include <vector>

// Select which APIs to support. For now just support the single API the CNSDK is built for.
#if defined(LEIA_USE_OPENGL)
#define LEIA_RENDERER_OPENGL
#endif

#if defined(LEIA_USE_DIRECTX)
#define LEIA_RENDERER_D3D11
#endif

#if defined(LEIA_USE_DIRECTX12)
#define LEIA_RENDERER_D3D12
#endif

#if defined(LEIA_USE_VULKAN)
#define LEIA_RENDERER_VULKAN
#endif

// Enable OpenGL
#ifdef LEIA_RENDERER_OPENGL
#include <GL/CAPI_GLE.h>
#if defined(LEIA_OS_ANDROID)
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#define GLFW_INCLUDE_ES3
typedef EGLContext HGLRC;
#endif
#endif

// Enable D3D11
#ifdef LEIA_RENDERER_D3D11
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11ShaderResourceView;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct IDXGISwapChain;
#endif

// Enable D3D12
#ifdef LEIA_RENDERER_D3D12
struct ID3D12Device;
struct ID3D12Resource;
struct IDXGISwapChain;
struct ID3D12CommandQueue;
struct ID3D12GraphicsCommandList;
struct D3D12_CPU_DESCRIPTOR_HANDLE;
struct CD3DX12_CPU_DESCRIPTOR_HANDLE;
enum D3D12_RESOURCE_STATES;
#endif

// Enable Vulkan
#ifdef LEIA_RENDERER_VULKAN
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
typedef int VkFormatInt;
typedef int VkImageLayoutInt;
#endif

namespace LeiaRenderer
{

#include "RendererMath.h"
using namespace Math;

enum class eGraphicsAPI
{
	OpenGL = 0,
	D3D11  = 1,
	Vulkan = 2,
    D3D12  = 3,
};

enum class eResourceUsageFlags : unsigned char
{
	None         = 0,
	ShaderInput  = 0x1,
	RenderTarget = 0x2,
    DepthStencil = 0x4,
    Alias        = 0x8
};

inline eResourceUsageFlags operator|(eResourceUsageFlags _LHS, eResourceUsageFlags _RHS) { return (eResourceUsageFlags)((unsigned char)_LHS | (unsigned char)_RHS); }
inline eResourceUsageFlags operator&(eResourceUsageFlags _LHS, eResourceUsageFlags _RHS) { return (eResourceUsageFlags)((unsigned char)_LHS & (unsigned char)_RHS); }

enum class eFilterMode
{
	Point  = 0,
	Linear = 1
};

enum class eWrapMode
{
    Clamp = 0,
    Wrap  = 1
};

enum class ePixelFormat
{
    RGB  = 0,
    RGBA = 1
};

struct ShaderPermutationDefinition
{
    int numVariations = 0;
    std::string mutationDefine;

    ShaderPermutationDefinition(int numberOfVariations, const char* mutDef)
    {
        numVariations = numberOfVariations;
        mutationDefine = mutDef;
    }

    ShaderPermutationDefinition()
    {
        numVariations = 0;
    }
};

class IShader
{
public:
    virtual void               Destroy               () = 0;
    virtual const std::string& GetPixelShaderText    () const = 0;
    virtual const std::string& GetVertexShaderText   () const = 0;
    virtual void               SetVertexShaderHeader (const char* header) = 0;
    virtual void               SetPixelShaderHeader  (const char* header) = 0;

    // TODO: move these into IRenderState eventually
    virtual void CreateVariant                  (int id, const char* defines, bool isVideoVariant = false) = 0;
    virtual void UseVariantWithPermutations     (int id, std::vector<unsigned int> const& mutations = std::vector<unsigned int>()) = 0; // sets current variant so that IRenderState::MakeCurrent() can use it
    virtual void SetPermutationTableForVariants (ShaderPermutationDefinition const* const permDefines, int count) = 0;
};

class ITexture
{
public:
	virtual bool               Create    (int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage, const void* initialData = nullptr) = 0;
    virtual void               Destroy   () = 0;
    virtual ePixelFormat       GetFormat () const = 0;
    virtual int                GetHeight () const = 0;
    virtual int                GetWidth  () const = 0;
    virtual int                GetSlices () const = 0;
    virtual void               Update    (int width, int height) = 0;
    virtual void               SetHints  (const char* hints) = 0;
    virtual const std::string& GetHints  () const = 0;
};

class IRenderState
{
public:
    virtual void AddShaderTexture      (const char* name, eFilterMode filterMode = eFilterMode::Point, eWrapMode wrapMode = eWrapMode::Wrap) = 0;
    virtual void AddShaderTextures     (const char* name, int arraySize, eFilterMode filterMode = eFilterMode::Point, eWrapMode wrapMode = eWrapMode::Wrap) = 0;
    virtual void AddShaderUniformi     (const char* name) = 0;
    virtual void AddShaderUniformiv    (const char* name, int arraySize) = 0;
    virtual void AddShaderUniformf     (const char* name) = 0;
	virtual void AddShaderUniformfv    (const char* name, int arraySize) = 0;
    virtual void AddShaderUniform2f    (const char* name) = 0;
    virtual void AddShaderUniform3f    (const char* name) = 0;
	virtual void AddShaderUniform3fv   (const char* name, int arraySize) = 0;
    virtual void AddShaderUniform4f    (const char* name) = 0;
	virtual void AddShaderUniform4fv   (const char* name, int arraySize) = 0;
    virtual void AddShaderUniformm     (const char* name, bool isGeometryTransform = false) = 0;
    virtual void AddShaderUniformmv    (const char* name, int arraySize, bool isGeometryTransform = false) = 0;
    virtual void ClearViewport         () = 0;
	virtual void Destroy               () = 0;
	virtual void SetBlending           (bool enabled) = 0;
	virtual void SetShader             (IShader* shader) = 0;
    virtual void SetShaderTexture      (const char* name, ITexture* texture) = 0;
    virtual void SetShaderTextures     (const char* name, int count, ITexture** textures) = 0;
    virtual void SetShaderTextureFilter(const char* name, LeiaRenderer::eFilterMode filterMode) = 0;
    virtual void SetShaderUniformi     (const char* name, const int value) = 0;
    virtual void SetShaderUniformiv    (const char* name, const int* values) = 0;
	virtual void SetShaderUniformf     (const char* name, const float value) = 0;
	virtual void SetShaderUniformfv    (const char* name, const float* values) = 0;
	virtual void SetShaderUniform2f    (const char* name, const vec2f& value) = 0;
    virtual void SetShaderUniform3f    (const char* name, const vec3f& value) = 0;
	virtual void SetShaderUniform3fv   (const char* name, const vec3f* values) = 0;
    virtual void SetShaderUniform4f    (const char* name, const vec4f& value) = 0;
	virtual void SetShaderUniform4fv   (const char* name, const vec4f* values) = 0;
    virtual void SetShaderUniformm     (const char* name, const mat4f& value) = 0;
    virtual void SetShaderUniformmv    (const char* name, const mat4f* values) = 0;
    virtual void SetViewport           (int x, int y, int width, int height) = 0;
};

class IGeometry
{
public:
	virtual void Destroy        () = 0;
	virtual void Rotate         (float x, float y, float z) = 0;
	virtual void SetOrientation (const mat3f& orientation) = 0;
	virtual void SetPosition    (const vec3f& position) = 0;
	virtual void SetRenderState (IRenderState* renderState) = 0;
};

class IRenderer
{
public:
    virtual void          Apply2DEffect         (IRenderState* renderState, ITexture* dstTexture = nullptr) = 0; // fullscreen rect render with render-state to specified output texture
    virtual void          ClearScene            (ITexture* dstTexture = nullptr, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f) = 0;
	virtual ITexture*     CreateTexture         (int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage, const void* initialData = nullptr) = 0;
    virtual ITexture*     CreateTextureFromFile (const char* filename, eResourceUsageFlags usage) = 0;
    virtual IShader*      CreateShaderFromFile  (const char* vertexShaderFile, const char* vertexShaderEntryPoint, const char* pixelShaderFile, const char* pixelShaderEntryPoint) = 0;
	virtual IShader*      CreateShaderFromText  (const char* vertexShaderText, const char* vertexShaderEntryPoint, const char* pixelShaderText, const char* pixelShaderEntryPoint) = 0;
	virtual IRenderState* CreateRenderState     (IShader* shader = nullptr) = 0;
	virtual IGeometry*    CreateGeometry        (int vertexCount, const float* vertices, int indexCount, const int* indices, IRenderState* renderState = nullptr) = 0;
	virtual IGeometry*    CreateGeometryCube    (const vec3f& size, IRenderState* renderState = nullptr, bool coloredVerticesAndUVs = false) = 0;
    virtual void          Destroy               () = 0;
    virtual const vec3f&  GetCameraDirection    () const = 0;
    virtual const vec3f&  GetCameraPosition     () const = 0;
    virtual vec3f         GetCameraRight        () const = 0;
    virtual vec3f         GetCameraUp           () const = 0;
    virtual void          OnWindowSizeChanged   (int width, int height) = 0;
    virtual void          RenderScene           (ITexture* dstTexture = nullptr) = 0; // render each geometry to destination texture
	virtual void          SetCameraPosition     (const vec3f& position) = 0;
	virtual void          SetCameraDirection    (const vec3f& direction) = 0;
	virtual void          SetCameraProjection   (float fovY, float aspectRatio, float zNear, float zFar, float shearX = 0.0f, float shearY = 0.0f) = 0;
    virtual void          BeginFrame            (int frameIndex = 0) = 0;
    virtual void          EndFrame              () = 0;
};

#ifdef LEIA_RENDERER_OPENGL
class ITextureGL
{
public:
    virtual unsigned int GetFrameBuffer (int index = 0) const = 0;
    virtual unsigned int GetTexture     (int index = 0) const = 0;
};

class IRendererOpenGL
{
public:
    virtual ITexture* CreateTextureAlias (unsigned int id, int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage) = 0;
    virtual void      Initialize         (HGLRC context) = 0;
    virtual void      BindFrameBuffer    (ITexture* texture) = 0;
#if defined(_WIN32)
    virtual void      Present            (HDC hDC) = 0;
#elif defined(LEIA_OS_ANDROID)
    virtual void      Present            () = 0;
#endif
};
LEIA_COMMON_API
IRendererOpenGL* AsOpenGL(IRenderer*);
LEIA_COMMON_API
ITextureGL* AsOpenGL(ITexture*);
#endif

#ifdef LEIA_RENDERER_D3D11
class ITextureD3D11
{
public:
    virtual ID3D11RenderTargetView*   GetRenderTargetView   (int slice = 0) const = 0;
    virtual ID3D11ShaderResourceView* GetShaderResourceView () const = 0;
    virtual ID3D11DepthStencilView*   GetDepthStencilView   (int slice = 0) const = 0;
    virtual ID3D11Texture2D*          GetTexture2D          () const = 0;
};

class IRendererD3D11
{
public:
    virtual ITexture* CreateTextureAlias (ID3D11Texture2D* texture, ID3D11ShaderResourceView* shaderResourceView, ID3D11RenderTargetView* renderTargetView, ID3D11Texture2D* depthStencilTexture, ID3D11DepthStencilView* depthStencilView, int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage) = 0;
    virtual bool      Initialize         (ID3D11DeviceContext* deviceContext) = 0;
    virtual void      SetRenderTarget    (ITexture* texture) = 0;
    virtual void      Present            (IDXGISwapChain* swapChain) = 0;
};
LEIA_COMMON_API
IRendererD3D11* AsD3D11(IRenderer*);
LEIA_COMMON_API
ITextureD3D11* AsD3D11(ITexture*);
#endif

#ifdef LEIA_RENDERER_D3D12
class ITextureD3D12
{
public:
    virtual void                  GetRenderTargetViewHandle   (CD3DX12_CPU_DESCRIPTOR_HANDLE* rtvHandle, int slice = 0) const = 0;
    virtual ID3D12Resource*       GetResource                 () const = 0;
    virtual ID3D12Resource*       GetDepthResource            () const = 0;
    virtual D3D12_RESOURCE_STATES GetResourceState            () const = 0;
    virtual D3D12_RESOURCE_STATES GetDepthResourceState       () const = 0;
    virtual void                  GetDepthStencilViewHandle   (CD3DX12_CPU_DESCRIPTOR_HANDLE* _dsvHandle, int slice = 0) const = 0;
    virtual void                  GetShaderResourceViewHandle (CD3DX12_CPU_DESCRIPTOR_HANDLE* _srvHandle) const = 0;
    virtual D3D12_RESOURCE_STATES TransitionShaderResource    (ID3D12GraphicsCommandList* commandList) = 0;
    virtual D3D12_RESOURCE_STATES TransitionRenderTarget      (ID3D12GraphicsCommandList* commandList) = 0;
    virtual D3D12_RESOURCE_STATES TransitionDepthWrite        (ID3D12GraphicsCommandList* commandList) = 0;
    virtual D3D12_RESOURCE_STATES TransitionPresent           (ID3D12GraphicsCommandList* commandList) = 0;
};

class IRendererD3D12
{
public:
    virtual ITexture* CreateTextureAlias (ID3D12Resource* texture, CD3DX12_CPU_DESCRIPTOR_HANDLE* shaderResourceView, CD3DX12_CPU_DESCRIPTOR_HANDLE* renderTargetView, D3D12_RESOURCE_STATES* textureState, ID3D12Resource* depthTexture, CD3DX12_CPU_DESCRIPTOR_HANDLE* depthStencilView, D3D12_RESOURCE_STATES* depthTextureState, int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage) = 0;
    virtual bool      Initialize         (ID3D12Device* device, ID3D12CommandQueue* commandQueue) = 0;
    virtual void      SetRenderTarget    (ITexture* texture) = 0;
    virtual void      Present            (IDXGISwapChain* swapChain) = 0;
};
LEIA_COMMON_API
IRendererD3D12* AsD3D12(IRenderer*);
LEIA_COMMON_API
ITextureD3D12* AsD3D12(ITexture*);
#endif

#ifdef LEIA_RENDERER_VULKAN
class ITextureVulkan
{
public:
    virtual VkImage          GetRenderTargetImage       (int slice = 0) const = 0;
    virtual VkImageLayoutInt GetRenderTargetImageLayout (int slice = 0) const = 0;
    virtual VkImageView      GetRenderTargetViewHandle  (int slice = 0) const = 0;
    virtual VkImage          GetDepthStencilImage       (int slice = 0) const = 0;
    virtual VkImageLayoutInt GetDepthStencilImageLayout (int slice = 0) const = 0;
    virtual VkFramebuffer    GetFramebuffer             (int slice = 0) const = 0;
};

class IRendererVulkan
{
public:
    virtual void        Apply2DEffect                  (IRenderState* renderState, ITexture* dstTexture = nullptr, VkSemaphore waitSemaphore = nullptr, VkSemaphore signalSemaphore = nullptr) = 0;
    virtual void        ClearScene                     (ITexture* dstTexture = nullptr, float r = 0.0f, float g = 0.0f, float b = 0.0f, float a = 1.0f, VkSemaphore waitSemaphore = nullptr, VkSemaphore signalSemaphore = nullptr) = 0;
    virtual void        RenderScene                    (ITexture* dstTexture = nullptr, VkSemaphore firstPassWaitSemaphore = nullptr, VkSemaphore finalPassSignalSemaphore = nullptr) = 0;
    virtual ITexture*   CreateTextureAlias             (VkFramebuffer frameBuffer, VkImage imageBuffer, VkImageLayoutInt imageLayout, VkFormatInt imageFormat, VkImage depthImageBuffer, VkImageLayoutInt depthImageLayout, int width, int height, int sliceCount, ePixelFormat format, eResourceUsageFlags usage) = 0;
    virtual bool        Initialize                     (VkDevice device, VkPhysicalDevice physicalDevice, VkFormatInt textureFormat, VkFormatInt renderTargetFormat, VkFormatInt depthStencilFormat, int maxInFlightFrameCount) = 0;
    virtual void        TransitionPresent              (VkCommandBuffer commandBuffer, VkImage image) = 0;
    virtual void        TransitionRenderTarget         (VkCommandBuffer commandBuffer, VkImage image) = 0;
    virtual VkSemaphore GetPreviousPassSignalSemaphore () const = 0;
};
LEIA_COMMON_API
IRendererVulkan* AsVulkan(IRenderer*);
#endif

// Single global entrypoint.
LEIA_COMMON_API IRenderer * CreateRenderer(eGraphicsAPI graphicsAPI);
}