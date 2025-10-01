#pragma once

#include <string>
std::string                             GenerateVertexShaderWithFVF(int64 e_i64FVFFlags,int e_iNumMorphTarget);
std::string                             GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags,struct sTectureAndTexCoordinateIndex*e_pTectureAndTexCoordinateIndex);
std::string								GenerateShadowMapFragmentShader();
std::string								GenerateShadowMapVertexShader(int64 e_i64FVFFlags, int e_iNumMorphTarget);
//std::string                             GenerateFragmentShaderWithFVFForWASMBecauseLightProlem(int64 e_i64FVFFlags);