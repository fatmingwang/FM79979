#pragma once

#include <string>
std::string                             GenerateVertexShaderWithFVF(int64 e_i64FVFFlags,int e_iNumMorphTarget);
std::string                             GenerateFragmentShaderWithFVF(int64 e_i64FVFFlags);
//std::string                             GenerateFragmentShaderWithFVFForWASMBecauseLightProlem(int64 e_i64FVFFlags);