#pragma once

#include <string>
std::string                             GenerateVertexShaderWithFVF(unsigned int fvfFlags,int e_iNumMorphTarget);
std::string                             GenerateFragmentShaderWithFVF(unsigned int fvfFlags);