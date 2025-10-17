#pragma once

#include <string>
std::string                             GenerateVertexShaderWithFVF(int64 e_i64FVFFlags,int e_iNumMorphTarget);
std::string                             GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags,struct sTectureAndTexCoordinateIndex*e_pTectureAndTexCoordinateIndex);
std::string								GenerateShadowMapFragmentShader();
std::string								GenerateShadowMapVertexShader();


GLuint									CreateOpenGLShaderProgram(const std::string& vertexShaderCode, const std::string& fragmentShaderCode);
void									PopulateUniform(int e_iProgram);
void									PopulateAttribute(int e_iProgram);
//std::string                             GenerateFragmentShaderWithFVFForWASMBecauseLightProlem(int64 e_i64FVFFlags);