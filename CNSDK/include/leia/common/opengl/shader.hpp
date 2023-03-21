/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#pragma once

#include "leia/common/opengl/api.h"

// TODO: remove glm from the public API
#include <glm/glm.hpp>

#include <string>
#include <vector>

namespace leia {
namespace opengl {

// General purpsoe shader object. Compiles from file, generates
// compile/link-time error messages and hosts several utility 
// functions for easy management.
class Shader
{
public:
    // state
    unsigned int ID; 
    // constructor
    Shader() { ID = 0; }
    // sets the current shader as active
    LEIA_COMMON_OPENGL_API
    unsigned int Use();
    // utility functions
    LEIA_COMMON_OPENGL_API
    void SetFloat(const char *name, float value, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetInteger(const char *name, int value, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector2f(const char *name, float x, float y, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector2f(const char *name, const glm::vec2 &value, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector3f(const char *name, float x, float y, float z, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector3f(const char *name, const glm::vec3 &value, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector4f(const char *name, float x, float y, float z, float w, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector4f(const char *name, const glm::vec4 &value, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetFloatArray(const char* name, float* values, int count, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetVector4fArray(const char* name, const glm::vec4* values, int count, bool useShader = false);
    LEIA_COMMON_OPENGL_API
    void SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader = false);

    LEIA_COMMON_OPENGL_API
    void CompileFromFile(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr);
    LEIA_COMMON_OPENGL_API
    void CompileFromText(const char* vertexCode, const char* fragmentCode, const char* geometryCode = nullptr, int variantIndex = 0, std::vector<unsigned int> const& mutations = std::vector<unsigned int>());
    LEIA_COMMON_OPENGL_API
    void CacheShaderText(std::string const& vertexCode, std::string const& fragmentCode);

    const std::string& getVertexShaderText() const { return vertexCode; };
    const std::string& getFragmentShaderText() const { return fragmentCode; };
    const std::string& getGeometryShaderText() const { return geometryCode; };

    LEIA_COMMON_OPENGL_API
    void CreateVariant(int id, const char* defines, bool isVideoVariant = false);
    LEIA_COMMON_OPENGL_API
    void UseVariantWithPermutations(int id, std::vector<unsigned int> const& mutations = std::vector<unsigned int>());
    LEIA_COMMON_OPENGL_API
    int  GetOffsetForVariantPermutation(std::vector<unsigned int> const& mutations);
    LEIA_COMMON_OPENGL_API
    void InitTextureSlotsForVariant(const int variantIdx, const int count, const char * variableNameFormat);

    void SetVertexShaderHeader(const char* header) { vertexShaderHeader = header; }
    void SetPixelShaderHeader(const char* header) { pixelShaderHeader = header; }

	struct PermutationDefinition
	{
		int numVariations = 0;
		std::string mutationDefine;

		PermutationDefinition(int numberOfVariations, const char* mutDef)
		{
			numVariations = numberOfVariations;
			mutationDefine = mutDef;
		}

		PermutationDefinition()
		{
			numVariations = 0;
		}
	};

    LEIA_COMMON_OPENGL_API
	void SetPermutationTableForVariants(PermutationDefinition const* const permDefines, int count);

private:
    // checks if compilation or linking failed and if so, print the error logs
    void checkCompileErrors(unsigned int object, std::string type); 


    //used to generate text defines for shaders, temporary helper
	struct PermutationEntry
	{
		std::string mutationDefines;
		std::vector<unsigned int> subPermutations;
	};

    std::vector<PermutationEntry> subPermutationEntries;

	std::vector<PermutationDefinition> mutationDefines;
	void FillSubPermutationDefine(std::string parentDefine, std::string& mutationDefine, int idx, std::vector<PermutationEntry>& mutationEntries);


    struct Variant
    {
        int          id      = 0;
        int          v0      = 0;
        int          v1      = 0;
        int          index   = 0;
        bool         isVideo = false;

        //Stored in a way that can be retrieved quickly
        std::vector<unsigned int> programs;

        std::string defines;           // #define a b\n#define c d\n#define d e\n etc...

        bool operator<(const Variant& other) const
        {
            return index < other.index;
        }

    };

    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    std::string vertexShaderHeader;
    std::string pixelShaderHeader;
    std::string mainTextureName;
    std::vector<Variant> variants;
};

} // namespace opengl
} // namespace leia
