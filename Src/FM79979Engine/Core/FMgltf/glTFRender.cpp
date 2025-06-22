//#include <GL/glew.h>
//#include <GLFW/glfw3.h>
//#include <tiny_gltf.h>
#include <iostream>
#include <vector>
#include "tiny_gltf.h"
#include "../AllCoreInclude.h"

void LoadModel(tinygltf::Model& model, const std::string& filename)
{
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    if (!warn.empty())
    {
        std::cout << "Warn: " << warn << std::endl;
    }
    if (!err.empty())
    {
        std::cerr << "Err: " << err << std::endl;
    }
    if (!ret)
    {
        std::cerr << "Failed to load glTF: " << filename << std::endl;
    }
}

void DrawModel(const tinygltf::Model& model)
{
    for (const auto& mesh : model.meshes)
    {
        for (const auto& primitive : mesh.primitives)
        {
            const tinygltf::Accessor& indexAccessor = model.accessors[primitive.indices];
            const tinygltf::BufferView& indexBufferView = model.bufferViews[indexAccessor.bufferView];
            const tinygltf::Buffer& indexBuffer = model.buffers[indexBufferView.buffer];

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferView.buffer);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferView.byteLength, &indexBuffer.data.at(0) + indexBufferView.byteOffset, GL_STATIC_DRAW);

            for (const auto& attrib : primitive.attributes)
            {
                const tinygltf::Accessor& accessor = model.accessors[attrib.second];
                const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
                const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

                glBindBuffer(GL_ARRAY_BUFFER, bufferView.buffer);
                glBufferData(GL_ARRAY_BUFFER, bufferView.byteLength, &buffer.data.at(0) + bufferView.byteOffset, GL_STATIC_DRAW);

                GLint size = 1;
                if (accessor.type == TINYGLTF_TYPE_SCALAR) size = 1;
                else if (accessor.type == TINYGLTF_TYPE_VEC2) size = 2;
                else if (accessor.type == TINYGLTF_TYPE_VEC3) size = 3;
                else if (accessor.type == TINYGLTF_TYPE_VEC4) size = 4;

                GLenum type = GL_FLOAT;
                if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_BYTE) type = GL_BYTE;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) type = GL_UNSIGNED_BYTE;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_SHORT) type = GL_SHORT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) type = GL_UNSIGNED_SHORT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_INT) type = GL_INT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT) type = GL_UNSIGNED_INT;
                else if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) type = GL_FLOAT;

                glVertexAttribPointer(attrib.first, size, type, accessor.normalized ? GL_TRUE : GL_FALSE, bufferView.byteStride, (void*)accessor.byteOffset);
                glEnableVertexAttribArray(attrib.first);
            }

            glDrawElements(primitive.mode, indexAccessor.count, indexAccessor.componentType, (void*)indexAccessor.byteOffset);
        }
    }
}

int main()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    GLFWwindow* window = glfwCreateWindow(800, 600, "glTF Viewer", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    tinygltf::Model model;
    LoadModel(model, "path/to/your/model.gltf");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        DrawModel(model);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
