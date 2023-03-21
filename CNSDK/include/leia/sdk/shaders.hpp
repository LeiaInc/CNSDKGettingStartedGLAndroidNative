#pragma once

#include "leia/sdk/api.h"
#include "leia/common/opengl/shader.hpp"
// TODO: remove glm from the public API
#include <glm/glm.hpp>


namespace leia {
namespace sdk {


	struct LeiaInterlaceParameters
	{
		float hardwareViewsX = 0.0f;
		float hardwareViewsY = 0.0f;
        float softwareViews = 1.0f;
		float viewsResX = 0.0f;
		float viewsResY = 0.0f;
		float viewportX = 0.0f;
		float viewportY = 0.0f;
		float viewportWidth = 0.0f;
		float viewportHeight = 0.0f;
		float n = 0.0f;
		float d_over_n = 0.0f;
		float p_over_du = 0.0f;
		float p_over_dv = 0.0f;
		float colorSlant = 0.0f;
		float colorInversion = 0.0f;
		glm::vec3 face = glm::vec3(0.0f);
		glm::vec3 nonPredictedface = glm::vec3(0.0f);
		float pixelPitch = 0.0f;
		float du = 0.0f;
		float dv = 0.0f;
		float s = 0.0f;
		float cos_theta = 0.0f;
		float sin_theta = 0.0f;
		float No = 0.0f;
		float peelOffset = 0.0f;
		float displayResX = 0.0f;
		float displayResY = 0.0f;
		bool  blendViews = true;
		float minView = 0.0f;
		float maxView = 0.0f;
		int   interlaceMode = 0;
		int   debugMode = 0;
		int   perPixelCorrection = 0;
		int   viewTextureType = 0;
		float reconvergenceAmount = 0;
		float customTextureScaleX = 1.0f;
		float customTextureScaleY = 1.0f;
        float reconvergenceZoomX = 0.0f;
        float reconvergenceZoomY = 0.0f;

        glm::mat4 textureTransform = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        glm::mat4 rectTransform = glm::mat4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

		void debugLog();
	};

	struct LeiaSharpenParameters
	{
		float gamma = 0.0f;
		float sharpeningCenter = 0.0f;
		int   sharpeningValueCount = 0;
		glm::vec4  textureInvSize = glm::vec4(0, 0, 0, 0);
		glm::vec4  sharpeningValues[18] = { glm::vec4(0,0,0,0), glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0),glm::vec4(0,0,0,0) };

		float actNormalized = 0.0f;

		void debugLog();
	};

	enum class eInterlaceShaderVariants
	{
		CalibImg,
		StereoImg,
		Thumbnail,
		StereoIndivdualViews,
		StereoAtlasViews,
		SlideOrPeelIndividualViews,
		SlideOrPeelAtlasViews,
		ShowAllViews,
		ShowFirstView,
		ShowTextureCoordinates,
		UseVideoTextureForViews,
		StereoImage_TopLeftUV
	};


	enum eInterlaceShaderPermutations
	{
		ISP_PerPixelCorrection = 0,
		ISP_Reconvergence,
		ISP_UV_TOP,
		ISP_HORIZONTAL_VIEWS,
		ISP_SINGLE_VIEW_MODE,
        ISP_USE_HLSL,
		ISP_MEDIUM_PRECISION,
		ISP_SINGLE_PASS_ACT,
        ISP_USE_GLSL_VULKAN,

		//Keep all new permutations above this line
		ISP_TOTAL,
	};

    enum eSharpenShaderPermutations
    {
        SSP_UV_TOP = 0,
        SSP_USE_HLSL,
        SSP_USE_GLSL_VULKAN,

        //Keep all new permutations above this line
        SSP_TOTAL,
    };

	struct ShaderPermutationDefinition
	{
		eInterlaceShaderPermutations type = ISP_PerPixelCorrection;
		const char* define = NULL;
		unsigned int count = 0;

		ShaderPermutationDefinition(eInterlaceShaderPermutations t, const char* d, unsigned int c)
		: type (t)
		, define (d)
		, count (c)
		{	
		}

		ShaderPermutationDefinition() {}
	};

	static const leia::opengl::Shader::PermutationDefinition sInterlaceShaderPermutations[] =
	{
		leia::opengl::Shader::PermutationDefinition(3,	"#define PIXELCORRECTION %d\n"),
		leia::opengl::Shader::PermutationDefinition(2,	"#define ENABLE_RECONVERGENCE %d\n"),
		leia::opengl::Shader::PermutationDefinition(2,	"#define UV_TOP_LEFT %d\n"),
		leia::opengl::Shader::PermutationDefinition(2,	"#define HORIZONTAL_SOURCE_VIEWS %d\n"),
		leia::opengl::Shader::PermutationDefinition(2,	"#define SINGLE_VIEW_MODE %d\n"),
        leia::opengl::Shader::PermutationDefinition(2,	"#define USE_HLSL %d\n"),
		leia::opengl::Shader::PermutationDefinition(2,	"#define MEDIUM_P %d\n"),
        leia::opengl::Shader::PermutationDefinition(2,	"#define SP_ACT %d\n"),
        leia::opengl::Shader::PermutationDefinition(2,	"#define USE_GLSL_VULKAN %d\n")

	};

    static const leia::opengl::Shader::PermutationDefinition sSharpenShaderPermutations[] =
    {
        leia::opengl::Shader::PermutationDefinition(2, "#define UV_TOP_LEFT %d\n"),
        leia::opengl::Shader::PermutationDefinition(2, "#define USE_HLSL %d\n"),
        leia::opengl::Shader::PermutationDefinition(2, "#define USE_GLSL_VULKAN %d\n")
    };

} // namespace sdk
} // namespace leia
