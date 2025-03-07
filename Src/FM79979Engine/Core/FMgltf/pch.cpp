// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"
// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include "tiny_gltf.h"
// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
