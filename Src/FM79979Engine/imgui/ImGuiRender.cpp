//#define USE_SDL2
#include "imgui.h"
#include "ImGuiRender.h"
#include "../Core/AllCoreInclude.h"


#include <unordered_map>
#include <string>
#include <filesystem>
#ifdef WIN32
#include "windowsx.h"
#elif defined(WASM)
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/em_js.h>
EM_JS(void, ImGui_ImplSDL2_EmscriptenOpenURL, (char const* url), { url = url ? UTF8ToString(url) : null; if (url) window.open(url, '_blank'); });
#endif


#ifdef WIN32
void    ImGui_ImplWin32_Shutdown();
void    ImGui_ImplWin32_NewFrame(float* e_pGameResolutionSize, int e_iContextIndex);
#elif defined(WASM)
void	ImGui_ImplSDL2_NewFrame();
#endif
bool g_bUseMyViewPort = true;
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================
//==========================================================================

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120       "#version 120"
//  3.0       130       "#version 130"
//  3.1       140       "#version 140"
//  3.2       150       "#version 150"
//  3.3       330       "#version 330 core"
//  4.0       400       "#version 400 core"
//  4.1       410       "#version 410 core"
//  4.2       420       "#version 410 core"
//  4.3       430       "#version 430 core"
//  ES 2.0    100       "#version 100"      = WebGL 1.0
//  ES 3.0    300       "#version 300 es"   = WebGL 2.0
//----------------------------------------

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif



#define GL_CALL(_CALL)      LAZY_DO_GL_COMMAND_AND_GET_ERROR(_CALL)



int g_iNumImGuiContext = 1;


std::map<int, std::function<float*(float*)>>    g_ContextIndexAndImGuiCameraPositionConvertFunctionMap;
std::map<int, std::function<void(long&, long&)>>g_ContextIndexAndImGuiGetCameraCursorPositionMap;
std::map<int, bool>                             g_ContextIndexAndMouseEventEnableMap;
std::vector<ImGuiContext*>*g_pImGuiContextVector = nullptr;
int                         g_iCurrenctContextIndex = 0;

const wchar_t* g_strImGuiShaderName = L"ImGuiShader";
cBaseShader* g_pImGuiShader = nullptr;

// OpenGL Data
struct ImGui_ImplOpenGL3_Data
{
    GLuint          FontTexture;
    GLsizeiptr      VertexBufferSize;
    GLsizeiptr      IndexBufferSize;
    unsigned int    VboHandle, ElementsHandle;
    //bool            UseBufferSubData;
#ifdef WASM
    SDL_Window* Window;
    Uint32                  WindowID;
    Uint64                  Time;
    char* ClipboardTextData;

    // Mouse handling
    Uint32                  MouseWindowID;
    int                     MouseButtonsDown;
    SDL_Cursor* MouseCursors[ImGuiMouseCursor_COUNT];
    SDL_Cursor* MouseLastCursor;
    int                     MouseLastLeaveFrame;
    bool                    MouseCanUseGlobalState;
    bool                    WantUpdateGamepadsList;
#endif
    ImGui_ImplOpenGL3_Data() { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendRendererUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple windows) instead of multiple Dear ImGui contexts.
ImGui_ImplOpenGL3_Data* ImGui_ImplOpenGL3_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplOpenGL3_Data*)ImGui::GetIO().BackendRendererUserData : nullptr;
}



std::unordered_map<std::string, ImFont*>* g_ImGuiFontMap = nullptr;;
void AddImGuiFonts(const std::vector<const char*>& fontPaths, const std::vector<float> e_FontSizeVector)
{
    assert(g_ImGuiFontMap == nullptr && "font added!?");
    g_ImGuiFontMap = new std::unordered_map<std::string, ImFont*>();
    ImGuiIO& io = ImGui::GetIO();
    for(size_t i=0;i< e_FontSizeVector.size();++i)
    //for (const char* fontPath : fontPaths)
    {
        const char* fontPath = fontPaths[i];
        // Extract the font file name (e.g., "Arial.ttf") from the full path
        std::string fontFileName = std::filesystem::path(fontPath).filename().string();

        // Add the font to ImGui and store it in the map
        float l_fFontSize = e_FontSizeVector[i];
        ImFont* font = io.Fonts->AddFontFromFileTTF(fontPath, l_fFontSize,nullptr, io.Fonts->GetGlyphRangesChineseFull());
        if (font)
        {
            (*g_ImGuiFontMap)[fontFileName] = font;  // Store it in the unordered_map with the file name as the key
        }
        else
        {
            // Handle the case where the font couldn't be loaded (e.g., file not found)
            //std::cerr << "Failed to load font: " << fontPath << std::endl;
            FMLOG("Failed to load font:%s", fontPath);
        }
    }

    // Build the font atlas after adding all fonts
    io.Fonts->Build();
}

// Function to get a font by its file name from the container
ImFont* GetFontByFileName(const std::string& fontFileName)
{
    if (!g_ImGuiFontMap)
    {
        return nullptr;
    }
    auto it = g_ImGuiFontMap->find(fontFileName);
    if (it != g_ImGuiFontMap->end())
    {
        return it->second;  // Return the font if found
    }
    else
    {
        return nullptr;  // Return nullptr if the font is not found
    }
}


bool ImGui_ImplOpenGL3_CreateFontsTexture()
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();
    if (!g_ImGuiFontMap)
    {
        ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    }

    // Build texture atlas
    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bit (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

    // Upload texture to graphics system
    // (Bilinear sampling is required by default. Set 'io.Fonts->Flags |= ImFontAtlasFlags_NoBakedLines' or 'style.AntiAliasedLinesUseTex = false' to allow point/nearest sampling)
    GLint last_texture;
    GL_CALL(glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture));
    GL_CALL(glGenTextures(1, &bd->FontTexture));
    GL_CALL(glBindTexture(GL_TEXTURE_2D, bd->FontTexture));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(WASM)// Not on WebGL/ES
    GL_CALL(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
#endif
    GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels));

    // Store our identifier
    io.Fonts->SetTexID((ImTextureID)(intptr_t)bd->FontTexture);

    // Restore state
    GL_CALL(glBindTexture(GL_TEXTURE_2D, last_texture));

    return true;
}
bool    ImguiCreateShader()
{
    const char* vertex_shader_glsl_120 =
        "uniform mat4 matVP;\n"
        "uniform mat4 matW;\n"
        "attribute vec2 VSPosition;\n"
        "attribute vec2 VSTexcoord;\n"
        "attribute vec4 VSColor;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = VSTexcoord;\n"
        "    Frag_Color = VSColor;\n"
        "    gl_Position = matVP*matW* vec4(VSPosition.xy,0,1);\n"
        "}\n";

    const char* fragment_shader_glsl_120 =
        "#ifdef GL_ES\n"
        "    precision mediump float;\n"
        "#endif\n"
        "uniform sampler2D texSample;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = Frag_Color * texture2D(texSample, Frag_UV.st);\n"
        "}\n";

    const char* vertex_shader_glsl_300_es =
        "precision highp float;\n"
        "attribute vec2 VSPosition;\n"
        "attribute vec2 VSTexcoord;\n"
        "attribute vec4 VSColor;\n"
        "uniform mat4 matVP;\n"
        "uniform mat4 matW;\n"
        "varying vec2 Frag_UV;\n"
        "varying vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    Frag_UV = VSTexcoord;\n"
        "    Frag_Color = VSColor;\n"
        "    gl_Position = matVP*matW* vec4(VSPosition.xy,0,1);\n"
        "}\n";

    const char* fragment_shader_glsl_300_es =
        "uniform sampler2D texSample;\n"
        "varying lowp vec2 Frag_UV;\n"
        "varying lowp vec4 Frag_Color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = Frag_Color * texture2D(texSample, Frag_UV.st);\n"
        "}\n";


    // Select shaders matching our GLSL versions
    const char* vertex_shader = nullptr;
    const char* fragment_shader = nullptr;
#ifdef WIN32
        vertex_shader = vertex_shader_glsl_120;
        fragment_shader = fragment_shader_glsl_120;
#else
    vertex_shader = vertex_shader_glsl_300_es;
    fragment_shader = fragment_shader_glsl_300_es;
#endif
    g_pImGuiShader = CreateShader(g_bCommonVSClientState, vertex_shader, fragment_shader, g_strImGuiShaderName);
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    glGenBuffers(1, &bd->VboHandle);
    glGenBuffers(1, &bd->ElementsHandle);
    return true;
}
#ifdef WIN32
bool ImGui_ImplWin32_InitEx(void* hwnd, bool platform_has_own_dc);
// Functions
bool    ImGui_ImplOpenGL3_Init(void* hwnd, const char* glsl_version, int e_iNumContext)
#else
bool    ImGui_ImplOpenGL3_Init(const char* glsl_version)
#endif
{
#ifdef WIN32
    ImGui_ImplWin32_InitEx(hwnd, true);
    //ImGui_ImplWin32_InitForOpenGL(hwnd);
#endif
    g_iNumImGuiContext = e_iNumContext;
    for (int i = 0; i < g_iNumImGuiContext; ++i)
    {
        if (g_pImGuiContextVector)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[i]);
        }
        ImGuiIO& io = ImGui::GetIO();
        IM_ASSERT(io.BackendRendererUserData == nullptr && "Already initialized a renderer backend!");
        // Setup backend capabilities flags
        ImGui_ImplOpenGL3_Data* l_BS = IM_NEW(ImGui_ImplOpenGL3_Data)();
        io.BackendRendererUserData = (void*)l_BS;
#ifdef WASM
        io.BackendRendererName = "imgui_impl_fm79979_WASM";
#else
        io.BackendRendererName = "imgui_impl_fm79979_OPENGL";
#endif
        if (!l_BS->FontTexture)
        {
            ImGui_ImplOpenGL3_CreateFontsTexture();
        }
        ImguiCreateShader();
    }
    return true;
}

void ImGui_ImplOpenGL3_ShutdownInner()
{
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    if (bd->VboHandle)
    {
        glDeleteBuffers(1, &bd->VboHandle); bd->VboHandle = 0;
    }
    if (bd->ElementsHandle)
    {
        glDeleteBuffers(1, &bd->ElementsHandle); bd->ElementsHandle = 0;
    }
    if (bd->FontTexture)
    {
        glDeleteTextures(1, &bd->FontTexture);
        io.Fonts->SetTexID(0);
        bd->FontTexture = 0;
    }
    io.BackendRendererName = nullptr;
    io.BackendRendererUserData = nullptr;
}

void    SetImGuiGetCameraCursorPosition(std::function<void(long&, long&)> e_Function, int e_iContextIndex)
{
    g_ContextIndexAndImGuiGetCameraCursorPositionMap[e_iContextIndex] = e_Function;
}

void    SetImGuiCameraPositionConvertFunction(std::function<float* (float*)> e_Function, int e_iContextIndex)
{
    g_ContextIndexAndImGuiCameraPositionConvertFunctionMap[e_iContextIndex] = e_Function;
}

void    SetImGuiMouseEnable(bool e_bMouseEventEnable, int e_iContextIndex)
{
    g_ContextIndexAndMouseEventEnableMap[e_iContextIndex] = e_bMouseEventEnable;
}

void ImGui_ImplOpenGL3_Shutdown()
{
#ifdef WIN32
    ImGui_ImplWin32_Shutdown();
#else
    if (g_pImGuiContextVector)
    {
        for (size_t i = 0; i < g_pImGuiContextVector->size(); ++i)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[i]);
            ImGui_ImplOpenGL3_ShutdownInner();
        };
    }
    else
    {
        ImGui_ImplOpenGL3_ShutdownInner();
    }
    
#endif
    if (g_pImGuiContextVector)
    {
        for (size_t i = 0; i < g_pImGuiContextVector->size(); ++i)
        {
            ImGui::DestroyContext((*g_pImGuiContextVector)[i]);
        }
        
        SAFE_DELETE(g_pImGuiContextVector);
    }
    else
    {
        ImGui::DestroyContext();
    }
}


void ImGui_ImplOpenGL3_SetupRenderState(ImDrawData* draw_data, int e_iFrameBufferWidth, int e_iFrameBufferHeight, GLuint vertex_array_object, float* e_pCameraMatrix, float* e_pGameResolutionSize)
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();

    // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glEnable(GL_SCISSOR_TEST);


    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayPos is (0,0) for single viewport apps.
    float l_fWindowWidth = cGameApp::m_spOpenGLRender->m_vGameResolution.x;
    float l_fWindowHeight = cGameApp::m_spOpenGLRender->m_vGameResolution.y;
    if (e_pGameResolutionSize)
    {
        l_fWindowWidth = e_pGameResolutionSize[0];
        l_fWindowHeight = e_pGameResolutionSize[1];
    }
    else
    if (!g_bUseMyViewPort)
    {
        l_fWindowWidth = (float)e_iFrameBufferWidth;
        l_fWindowHeight = (float)e_iFrameBufferHeight;
    }
    //float L = draw_data->DisplayPos.x;
    //float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    //float T = draw_data->DisplayPos.y;
    //float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    UseShaderProgram(g_strImGuiShaderName);
    g_pImGuiShader->Use();
    //SetupShaderViewProjectionMatrix((float*)ortho_projection, true);
    if (e_pCameraMatrix)
    {
        //FATMING_CORE::SetupShaderWorldMatrix(e_pCameraMatrix);
        FATMING_CORE::SetupShaderViewProjectionMatrix(e_pCameraMatrix, true);
    }
    else
    {
        glEnable2D(l_fWindowWidth, l_fWindowHeight);
    }
    FATMING_CORE::SetupShaderWorldMatrix(cMatrix44::Identity);
    (void)vertex_array_object;
    glBindVertexArray(vertex_array_object);
    // Bind vertex/index buffers and setup attributes for ImDrawVert
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, bd->VboHandle));
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bd->ElementsHandle));
    GL_CALL(glEnableVertexAttribArray(g_pImGuiShader->m_uiAttribArray[FVF_POS]));
    GL_CALL(glEnableVertexAttribArray(g_pImGuiShader->m_uiAttribArray[FVF_TEX0]));
    GL_CALL(glEnableVertexAttribArray(g_pImGuiShader->m_uiAttribArray[FVF_DIFFUSE]));
    GL_CALL(glVertexAttribPointer(g_pImGuiShader->m_uiAttribArray[FVF_POS], 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, pos)));
    GL_CALL(glVertexAttribPointer(g_pImGuiShader->m_uiAttribArray[FVF_TEX0], 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, uv)));
    GL_CALL(glVertexAttribPointer(g_pImGuiShader->m_uiAttribArray[FVF_DIFFUSE], 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)offsetof(ImDrawVert, col)));
}

// OpenGL3 Render function.
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly.
// This is in order to be able to run within an OpenGL engine that doesn't do so.
void    ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data, float* e_pCameraMatrix, float* e_pGameResolutionSize)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    int l_iFrameBufferWidth= (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
    int l_iFrameBufferHeight = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
    if (g_bUseMyViewPort)
    {
        l_iFrameBufferWidth = (int)cGameApp::m_spOpenGLRender->m_vGameResolution.x;
        l_iFrameBufferHeight = (int)cGameApp::m_spOpenGLRender->m_vGameResolution.y;
    }
    if (l_iFrameBufferWidth <= 0 || l_iFrameBufferHeight <= 0)
    {
        return;
    }
#ifdef WASM
    static int l_siTestX = 0;
    static int l_siTestY = 0;
    if (l_siTestX != l_iFrameBufferWidth || l_siTestY != l_iFrameBufferHeight)
    {
        l_siTestX = l_iFrameBufferWidth;
        l_siTestY = l_iFrameBufferHeight;
        int l_iBrowserWidth = EMSDK::EMSDK_GetBrowserWidth();
        int l_iBrowserHeight = EMSDK::EMSDK_GetBrowserHeight();
        int	l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
        int	l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();
        int	l_iCanvasPosX = EMSDK::EMSDK_GetCanvasPosX();
        int	l_iCanvasPosY = EMSDK::EMSDK_GetCanvasPosY();
        FMLog::Log(UT::ComposeMsgByFormat("FramebufferWidth:%d,FramebufferHeight:%d\nBrowserSize:%d,%d\nViewportSize:%d,%d\nCanvasPos:%d,%d", 
            l_siTestX, l_siTestY,
            l_iBrowserWidth, l_iBrowserHeight,
            l_iViewportWidth, l_iViewportHeight,
            l_iCanvasPosX, l_iCanvasPosY).c_str(), false);
    }
#endif
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    // Backup GL state
    GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
    glActiveTexture(GL_TEXTURE0);
    GLuint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, (GLint*)&last_program);
    GLuint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, (GLint*)&last_texture);
    GLuint last_array_buffer; 
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, (GLint*)&last_array_buffer);

    GLuint last_vertex_array_object; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, (GLint*)&last_vertex_array_object);

    GLint last_viewport[4]; glGetIntegerv(GL_VIEWPORT, last_viewport);
    GLint last_scissor_box[4]; glGetIntegerv(GL_SCISSOR_BOX, last_scissor_box);
    GLenum last_blend_src_rgb; glGetIntegerv(GL_BLEND_SRC_RGB, (GLint*)&last_blend_src_rgb);
    GLenum last_blend_dst_rgb; glGetIntegerv(GL_BLEND_DST_RGB, (GLint*)&last_blend_dst_rgb);
    GLenum last_blend_src_alpha; glGetIntegerv(GL_BLEND_SRC_ALPHA, (GLint*)&last_blend_src_alpha);
    GLenum last_blend_dst_alpha; glGetIntegerv(GL_BLEND_DST_ALPHA, (GLint*)&last_blend_dst_alpha);
    GLenum last_blend_equation_rgb; glGetIntegerv(GL_BLEND_EQUATION_RGB, (GLint*)&last_blend_equation_rgb);
    GLenum last_blend_equation_alpha; glGetIntegerv(GL_BLEND_EQUATION_ALPHA, (GLint*)&last_blend_equation_alpha);
    GLboolean last_enable_blend = glIsEnabled(GL_BLEND);
    GLboolean last_enable_cull_face = glIsEnabled(GL_CULL_FACE);
    GLboolean last_enable_depth_test = glIsEnabled(GL_DEPTH_TEST);
    GLboolean last_enable_stencil_test = glIsEnabled(GL_STENCIL_TEST);
    GLboolean last_enable_scissor_test = glIsEnabled(GL_SCISSOR_TEST);

    // Setup desired GL state
    // Recreate the VAO every time (this is to easily allow multiple GL contexts to be rendered to. VAO are not shared among GL contexts)
    // The renderer would actually work without any VAO bound, but then our VertexAttrib calls would overwrite the default one currently bound.
    GLuint vertex_array_object = 0;

    GL_CALL(glGenVertexArrays(1, &vertex_array_object));

    //fuck imhui using compact vertex struct not separate data
    ImGui_ImplOpenGL3_SetupRenderState(draw_data, l_iFrameBufferWidth, l_iFrameBufferHeight, vertex_array_object, e_pCameraMatrix, e_pGameResolutionSize);


    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)
    Vector2	l_vScale = cGameApp::m_spOpenGLRender->GetViewPortAndGameResolutionScale();
    if (e_pGameResolutionSize)
    {
        ImGuiIO& io = ImGui::GetIO();
        auto l_DisplaySize = io.DisplaySize;
        l_vScale.x = l_DisplaySize.x/ e_pGameResolutionSize[0];
        l_vScale.y = l_DisplaySize.y/ e_pGameResolutionSize[1];
    }
    std::function<float* (float*)>l_fImGuiCameraPositionConvertFunction;
    auto l_IT = g_ContextIndexAndImGuiCameraPositionConvertFunctionMap.find(g_iCurrenctContextIndex);
    if (l_IT != g_ContextIndexAndImGuiCameraPositionConvertFunctionMap.end())
    {
        l_fImGuiCameraPositionConvertFunction = l_IT->second;
    }
    
    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const GLsizeiptr vtx_buffer_size = (GLsizeiptr)cmd_list->VtxBuffer.Size * (int)sizeof(ImDrawVert);
        const GLsizeiptr idx_buffer_size = (GLsizeiptr)cmd_list->IdxBuffer.Size * (int)sizeof(ImDrawIdx);
        GL_CALL(glBufferData(GL_ARRAY_BUFFER, vtx_buffer_size, (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW));
        GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, idx_buffer_size, (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW));

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != nullptr)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                //if (!pcmd->UserCallbackData)
                {
                    // Project scissor/clipping rectangles into framebuffer space
                    ImVec2 clip_min((pcmd->ClipRect.x - clip_off.x) * clip_scale.x, (pcmd->ClipRect.y - clip_off.y) * clip_scale.y);
                    ImVec2 clip_max((pcmd->ClipRect.z - clip_off.x) * clip_scale.x, (pcmd->ClipRect.w - clip_off.y) * clip_scale.y);
                    if (clip_max.x <= clip_min.x || clip_max.y <= clip_min.y)
                    {
                        continue;
                    }
                    if (g_bUseMyViewPort)
                    {
                        Vector4 l_vCameraRect;
                        if (l_fImGuiCameraPositionConvertFunction)
                        {
                            // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
                            float l_fStartX = clip_min.x;
                            float l_fStartY = pcmd->ClipRect.y;
                            //float l_fWidth = (clip_max.x - clip_min.x)* l_vScale.x;
                            //float l_fHeight = (clip_max.y - clip_min.y)* l_vScale.y;
                            float l_fWidth = (clip_max.x - clip_min.x);
                            float l_fHeight = (clip_max.y - clip_min.y);
                            Vector4 l_vRect(l_fStartX, l_fStartY, l_fStartX + l_fWidth, l_fStartY + l_fHeight);
                            l_vCameraRect = l_fImGuiCameraPositionConvertFunction(l_vRect);
                            //if (l_vCameraRect.x < 0)
                            //    l_vCameraRect.x = 0;
                            //if (l_vCameraRect.y < 0)
                            //    l_vCameraRect.y = 0;
                            //if (l_vCameraRect.z > cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Width())
                            //{
                            //    l_vCameraRect.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Width();
                            //}
                            //if (l_vCameraRect.w > cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Height())
                            //{
                            //    l_vCameraRect.z = cGameApp::m_spOpenGLRender->m_vDeviceViewPortSize.Height();
                            //}
                            GL_CALL(glScissor((int)(l_vCameraRect.x),
                                              (int)(l_vCameraRect.y),
                                              (int)(l_vCameraRect.z),
                                              (int)(l_vCameraRect.w)));
                        }
                        else
                        {
                            float l_fMyStartX = cGameApp::m_spOpenGLRender->m_vViewPortSize.x;
                            float l_fMyStartY = cGameApp::m_spOpenGLRender->m_vViewPortSize.y;// 
                            // Apply scissor/clipping rectangle (Y is inverted in OpenGL)
                            float l_fStartX = clip_min.x / l_vScale.x;
                            float l_fStartY = (l_iFrameBufferHeight - clip_max.y) / l_vScale.y;
                            float l_fWidth = (clip_max.x - clip_min.x) / l_vScale.x;
                            float l_fHeight = (clip_max.y - clip_min.y) / l_vScale.y;
                            Vector4 l_v((l_fMyStartX + l_fStartX), ((float)l_fMyStartY + l_fStartY), l_fWidth, l_fHeight);
                            GL_CALL(glScissor((int)(l_fMyStartX + l_fStartX),
                                              (int)((float)l_fMyStartY + l_fStartY),
                                              (int)(l_fWidth),
                                              (int)(l_fHeight)));
                        }
                    }
                    else
                    {
                        GL_CALL(glScissor((int)clip_min.x, (int)((float)l_iFrameBufferHeight - clip_max.y), (int)(clip_max.x - clip_min.x), (int)(clip_max.y - clip_min.y)));
                    }
                }
                // Bind texture, Draw
                GL_CALL(glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->GetTexID()));
                MY_GLDRAW_ELEMENTS(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, (void*)(intptr_t)(pcmd->IdxOffset * sizeof(ImDrawIdx)));
            }
        }
    }

    // Destroy the temporary VAO

    GL_CALL(glDeleteVertexArrays(1, &vertex_array_object));


    // Restore modified GL state
    // This "glIsProgram()" check is required because if the program is "pending deletion" at the time of binding backup, it will have been deleted by now and will cause an OpenGL error. See #6220.
    if (last_program == 0 || glIsProgram(last_program)) glUseProgram(last_program);
    glBindTexture(GL_TEXTURE_2D, last_texture);


    glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

    glBlendEquationSeparate(last_blend_equation_rgb, last_blend_equation_alpha);
    glBlendFuncSeparate(last_blend_src_rgb, last_blend_dst_rgb, last_blend_src_alpha, last_blend_dst_alpha);
    if (last_enable_blend) glEnable(GL_BLEND); else glDisable(GL_BLEND);
    if (last_enable_cull_face) glEnable(GL_CULL_FACE); else glDisable(GL_CULL_FACE);
    if (last_enable_depth_test) glEnable(GL_DEPTH_TEST); else glDisable(GL_DEPTH_TEST);
    if (last_enable_stencil_test) glEnable(GL_STENCIL_TEST); else glDisable(GL_STENCIL_TEST);
    if (last_enable_scissor_test) glEnable(GL_SCISSOR_TEST); else glDisable(GL_SCISSOR_TEST);

    glViewport(last_viewport[0], last_viewport[1], (GLsizei)last_viewport[2], (GLsizei)last_viewport[3]);
    glScissor(last_scissor_box[0], last_scissor_box[1], (GLsizei)last_scissor_box[2], (GLsizei)last_scissor_box[3]);
    (void)bd; // Not all compilation paths use this
}

void ImGui_StartFrame(float* e_pGameResolutionSize,int e_iContextIndex)
{
    g_iCurrenctContextIndex = e_iContextIndex;
#if defined(WIN32)
    ImGui_ImplWin32_NewFrame(e_pGameResolutionSize, e_iContextIndex);
#elif defined(WASM)
    ImGui_ImplSDL2_NewFrame();
#endif
    ImGui::NewFrame();
}

void ImGui_EndFrame(float* e_pfMatrix, float* e_pGameResolutionSize)
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData(), e_pfMatrix, e_pGameResolutionSize);
}


//=======================
#ifdef WIN32
#define IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
struct ImGui_ImplWin32_Data
{
    HWND                        hWnd;
    HWND                        MouseHwnd;
    int                         MouseTrackedArea;   // 0: not tracked, 1: client area, 2: non-client area
    int                         MouseButtonsDown;
    INT64                       Time;
    INT64                       TicksPerSecond;
    ImGuiMouseCursor            LastMouseCursor;
    UINT32                      KeyboardCodePage;

#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    bool                        HasGamepad;
    bool                        WantUpdateHasGamepad;
    HMODULE                     XInputDLL;
    PFN_XInputGetCapabilities   XInputGetCapabilities;
    PFN_XInputGetState          XInputGetState;
#endif

    ImGui_ImplWin32_Data() { memset((void*)this, 0, sizeof(*this)); }
};

// Backend data stored in io.BackendPlatformUserData to allow support for multiple Dear ImGui contexts
// It is STRONGLY preferred that you use docking branch with multi-viewports (== single Dear ImGui context + multiple windows) instead of multiple Dear ImGui contexts.
// FIXME: multi-context support is not well tested and probably dysfunctional in this backend.
// FIXME: some shared resources (mouse cursor shape, gamepad) are mishandled when using multi-context.
ImGui_ImplWin32_Data* ImGui_ImplWin32_GetBackendData()
{
    return ImGui::GetCurrentContext() ? (ImGui_ImplWin32_Data*)ImGui::GetIO().BackendPlatformUserData : nullptr;
}

// Functions
void ImGui_ImplWin32_UpdateKeyboardCodePage()
{
    // Retrieve keyboard code page, required for handling of non-Unicode Windows.
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    HKL keyboard_layout = ::GetKeyboardLayout(0);
    LCID keyboard_lcid = MAKELCID(HIWORD(keyboard_layout), SORT_DEFAULT);
    if (::GetLocaleInfoA(keyboard_lcid, (LOCALE_RETURN_NUMBER | LOCALE_IDEFAULTANSICODEPAGE), (LPSTR)&bd->KeyboardCodePage, sizeof(bd->KeyboardCodePage)) == 0)
        bd->KeyboardCodePage = CP_ACP; // Fallback to default ANSI code page when fails.
}

bool    ImGui_ImplWin32_InitExInner(void* hwnd, bool platform_has_own_dc)
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    INT64 perf_frequency, perf_counter;
    if (!::QueryPerformanceFrequency((LARGE_INTEGER*)&perf_frequency))
        return false;
    if (!::QueryPerformanceCounter((LARGE_INTEGER*)&perf_counter))
        return false;

    // Setup backend capabilities flags
    ImGui_ImplWin32_Data* bd = IM_NEW(ImGui_ImplWin32_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_win32";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;         // We can honor GetMouseCursor() values (optional)
    //io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;          // We can honor io.WantSetMousePos requests (optional, rarely used)

    bd->hWnd = (HWND)hwnd;
    bd->TicksPerSecond = perf_frequency;
    bd->Time = perf_counter;
    bd->LastMouseCursor = ImGuiMouseCursor_COUNT;
    ImGui_ImplWin32_UpdateKeyboardCodePage();

    // Set platform dependent data in viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = main_viewport->PlatformHandleRaw = (void*)bd->hWnd;
    IM_UNUSED(platform_has_own_dc); // Used in 'docking' branch

    // Dynamically load XInput library
#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    bd->WantUpdateHasGamepad = true;
    const char* xinput_dll_names[] =
    {
        "xinput1_4.dll",   // Windows 8+
        "xinput1_3.dll",   // DirectX SDK
        "xinput9_1_0.dll", // Windows Vista, Windows 7
        "xinput1_2.dll",   // DirectX SDK
        "xinput1_1.dll"    // DirectX SDK
    };
    for (int n = 0; n < IM_ARRAYSIZE(xinput_dll_names); n++)
        if (HMODULE dll = ::LoadLibraryA(xinput_dll_names[n]))
        {
            bd->XInputDLL = dll;
            bd->XInputGetCapabilities = (PFN_XInputGetCapabilities)::GetProcAddress(dll, "XInputGetCapabilities");
            bd->XInputGetState = (PFN_XInputGetState)::GetProcAddress(dll, "XInputGetState");
            break;
        }
#endif // IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    return true;
}

bool ImGui_ImplWin32_InitEx(void* hwnd, bool platform_has_own_dc)
{
    IMGUI_CHECKVERSION();
    if (g_iNumImGuiContext > 1 && !g_pImGuiContextVector)
    {
        g_pImGuiContextVector = new std::vector<ImGuiContext*>();
        for (int i = 0; i < g_iNumImGuiContext; ++i)
        {
            auto l_pDefaultImGuiContext = ImGui::CreateContext();
            ImGui::SetCurrentContext(l_pDefaultImGuiContext);
            g_pImGuiContextVector->push_back(l_pDefaultImGuiContext);
            ImGui_ImplWin32_InitExInner(hwnd, platform_has_own_dc);
        }
    }
    else
    {
        ImGui::CreateContext();
        ImGui_ImplWin32_InitExInner(hwnd, platform_has_own_dc);
    }

    return true;
}

void ReleaseWin32BackendData()
{
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "No platform backend to shutdown, or already shutdown?");
    ImGuiIO& io = ImGui::GetIO();
    // Unload XInput library
#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    if (bd->XInputDLL)
        ::FreeLibrary(bd->XInputDLL);
#endif // IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    ImGui_ImplOpenGL3_ShutdownInner();
    io.BackendPlatformName = nullptr;
    io.BackendPlatformUserData = nullptr;
    io.BackendFlags &= ~(ImGuiBackendFlags_HasMouseCursors | ImGuiBackendFlags_HasSetMousePos | ImGuiBackendFlags_HasGamepad);
    SAFE_DELETE(io.BackendRendererUserData);// = nullptr;
    IM_DELETE(bd);
}

void    ImGui_ImplWin32_Shutdown()
{
    if (g_pImGuiContextVector)
    {
        for (size_t i = 0; i < g_pImGuiContextVector->size(); ++i)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[i]);
            ReleaseWin32BackendData();
        }
        SAFE_DELETE(g_pImGuiContextVector);
    }
    else
    {
        ReleaseWin32BackendData();
    }
}

bool ImGui_ImplWin32_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return false;

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (imgui_cursor == ImGuiMouseCursor_None || io.MouseDrawCursor)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        ::SetCursor(nullptr);
    }
    else
    {
        // Show OS mouse cursor
        LPTSTR win32_cursor = IDC_ARROW;
        switch (imgui_cursor)
        {
        case ImGuiMouseCursor_Arrow:        win32_cursor = IDC_ARROW; break;
        case ImGuiMouseCursor_TextInput:    win32_cursor = IDC_IBEAM; break;
        case ImGuiMouseCursor_ResizeAll:    win32_cursor = IDC_SIZEALL; break;
        case ImGuiMouseCursor_ResizeEW:     win32_cursor = IDC_SIZEWE; break;
        case ImGuiMouseCursor_ResizeNS:     win32_cursor = IDC_SIZENS; break;
        case ImGuiMouseCursor_ResizeNESW:   win32_cursor = IDC_SIZENESW; break;
        case ImGuiMouseCursor_ResizeNWSE:   win32_cursor = IDC_SIZENWSE; break;
        case ImGuiMouseCursor_Hand:         win32_cursor = IDC_HAND; break;
        case ImGuiMouseCursor_NotAllowed:   win32_cursor = IDC_NO; break;
        }
        ::SetCursor(::LoadCursor(nullptr, win32_cursor));
    }
    return true;
}

bool IsVkDown(int vk)
{
    return (::GetKeyState(vk) & 0x8000) != 0;
}

void ImGui_ImplWin32_AddKeyEvent(ImGuiKey key, bool down, int native_keycode, int native_scancode = -1)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(key, down);
    io.SetKeyEventNativeData(key, native_keycode, native_scancode); // To support legacy indexing (<1.87 user code)
    IM_UNUSED(native_scancode);
}

void ImGui_ImplWin32_ProcessKeyEventsWorkarounds()
{
    // Left & right Shift keys: when both are pressed together, Windows tend to not generate the WM_KEYUP event for the first released one.
    if (ImGui::IsKeyDown(ImGuiKey_LeftShift) && !IsVkDown(VK_LSHIFT))
        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_LeftShift, false, VK_LSHIFT);
    if (ImGui::IsKeyDown(ImGuiKey_RightShift) && !IsVkDown(VK_RSHIFT))
        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_RightShift, false, VK_RSHIFT);

    // Sometimes WM_KEYUP for Win key is not passed down to the app (e.g. for Win+V on some setups, according to GLFW).
    if (ImGui::IsKeyDown(ImGuiKey_LeftSuper) && !IsVkDown(VK_LWIN))
        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_LeftSuper, false, VK_LWIN);
    if (ImGui::IsKeyDown(ImGuiKey_RightSuper) && !IsVkDown(VK_RWIN))
        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_RightSuper, false, VK_RWIN);
}

void ImGui_ImplWin32_UpdateKeyModifiers()
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl, IsVkDown(VK_CONTROL));
    io.AddKeyEvent(ImGuiMod_Shift, IsVkDown(VK_SHIFT));
    io.AddKeyEvent(ImGuiMod_Alt, IsVkDown(VK_MENU));
    io.AddKeyEvent(ImGuiMod_Super, IsVkDown(VK_LWIN) || IsVkDown(VK_RWIN));
}

void ImGui_ImplWin32_UpdateMouseData()
{
    return;
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(bd->hWnd != 0);

    HWND focused_window = ::GetForegroundWindow();
    const bool is_app_focused = (focused_window == bd->hWnd);
    if (is_app_focused)
    {
        // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
        {
            POINT pos = { (int)io.MousePos.x, (int)io.MousePos.y };
            if (::ClientToScreen(bd->hWnd, &pos))
                ::SetCursorPos(pos.x, pos.y);
        }

        // (Optional) Fallback to provide mouse position when focused (WM_MOUSEMOVE already provides this when hovered or captured)
        // This also fills a short gap when clicking non-client area: WM_NCMOUSELEAVE -> modal OS move -> gap -> WM_NCMOUSEMOVE
        if (!io.WantSetMousePos && bd->MouseTrackedArea == 0)
        {
            POINT pos;
            if (::GetCursorPos(&pos) && ::ScreenToClient(bd->hWnd, &pos))
                io.AddMousePosEvent((float)pos.x, (float)pos.y);
        }
    }
}

// Gamepad navigation mapping
void ImGui_ImplWin32_UpdateGamepads()
{
#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    //if ((io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad) == 0) // FIXME: Technically feeding gamepad shouldn't depend on this now that they are regular inputs.
    //    return;

    // Calling XInputGetState() every frame on disconnected gamepads is unfortunately too slow.
    // Instead we refresh gamepad availability by calling XInputGetCapabilities() _only_ after receiving WM_DEVICECHANGE.
    if (bd->WantUpdateHasGamepad)
    {
        XINPUT_CAPABILITIES caps = {};
        bd->HasGamepad = bd->XInputGetCapabilities ? (bd->XInputGetCapabilities(0, XINPUT_FLAG_GAMEPAD, &caps) == ERROR_SUCCESS) : false;
        bd->WantUpdateHasGamepad = false;
    }

    io.BackendFlags &= ~ImGuiBackendFlags_HasGamepad;
    XINPUT_STATE xinput_state;
    XINPUT_GAMEPAD& gamepad = xinput_state.Gamepad;
    if (!bd->HasGamepad || bd->XInputGetState == nullptr || bd->XInputGetState(0, &xinput_state) != ERROR_SUCCESS)
        return;
    io.BackendFlags |= ImGuiBackendFlags_HasGamepad;

#define IM_SATURATE(V)                      (V < 0.0f ? 0.0f : V > 1.0f ? 1.0f : V)
#define MAP_BUTTON(KEY_NO, BUTTON_ENUM)     { io.AddKeyEvent(KEY_NO, (gamepad.wButtons & BUTTON_ENUM) != 0); }
#define MAP_ANALOG(KEY_NO, VALUE, V0, V1)   { float vn = (float)(VALUE - V0) / (float)(V1 - V0); io.AddKeyAnalogEvent(KEY_NO, vn > 0.10f, IM_SATURATE(vn)); }
    MAP_BUTTON(ImGuiKey_GamepadStart, XINPUT_GAMEPAD_START);
    MAP_BUTTON(ImGuiKey_GamepadBack, XINPUT_GAMEPAD_BACK);
    MAP_BUTTON(ImGuiKey_GamepadFaceLeft, XINPUT_GAMEPAD_X);
    MAP_BUTTON(ImGuiKey_GamepadFaceRight, XINPUT_GAMEPAD_B);
    MAP_BUTTON(ImGuiKey_GamepadFaceUp, XINPUT_GAMEPAD_Y);
    MAP_BUTTON(ImGuiKey_GamepadFaceDown, XINPUT_GAMEPAD_A);
    MAP_BUTTON(ImGuiKey_GamepadDpadLeft, XINPUT_GAMEPAD_DPAD_LEFT);
    MAP_BUTTON(ImGuiKey_GamepadDpadRight, XINPUT_GAMEPAD_DPAD_RIGHT);
    MAP_BUTTON(ImGuiKey_GamepadDpadUp, XINPUT_GAMEPAD_DPAD_UP);
    MAP_BUTTON(ImGuiKey_GamepadDpadDown, XINPUT_GAMEPAD_DPAD_DOWN);
    MAP_BUTTON(ImGuiKey_GamepadL1, XINPUT_GAMEPAD_LEFT_SHOULDER);
    MAP_BUTTON(ImGuiKey_GamepadR1, XINPUT_GAMEPAD_RIGHT_SHOULDER);
    MAP_ANALOG(ImGuiKey_GamepadL2, gamepad.bLeftTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_ANALOG(ImGuiKey_GamepadR2, gamepad.bRightTrigger, XINPUT_GAMEPAD_TRIGGER_THRESHOLD, 255);
    MAP_BUTTON(ImGuiKey_GamepadL3, XINPUT_GAMEPAD_LEFT_THUMB);
    MAP_BUTTON(ImGuiKey_GamepadR3, XINPUT_GAMEPAD_RIGHT_THUMB);
    MAP_ANALOG(ImGuiKey_GamepadLStickLeft, gamepad.sThumbLX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(ImGuiKey_GamepadLStickRight, gamepad.sThumbLX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(ImGuiKey_GamepadLStickUp, gamepad.sThumbLY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(ImGuiKey_GamepadLStickDown, gamepad.sThumbLY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickLeft, gamepad.sThumbRX, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
    MAP_ANALOG(ImGuiKey_GamepadRStickRight, gamepad.sThumbRX, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickUp, gamepad.sThumbRY, +XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, +32767);
    MAP_ANALOG(ImGuiKey_GamepadRStickDown, gamepad.sThumbRY, -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE, -32768);
#undef MAP_BUTTON
#undef MAP_ANALOG
#endif // #ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
}

void    ImGui_ImplWin32_NewFrame(float* e_pGameResolutionSize,int e_iContextIndex)
{
    if (e_iContextIndex)
    {
        //ImGui::SetCurrentContext(e_pDefaultImGuiContext);
        if (g_pImGuiContextVector)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[e_iContextIndex]);
        }
    }
    else
    {
        if (g_pImGuiContextVector)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[0]);
        }
    }
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized? Did you call ImGui_ImplWin32_Init()?");
    ImGuiIO& io = ImGui::GetIO();
    // Setup display size (every frame to accommodate for window resizing)
    RECT rect = { 0, 0, 0, 0 };
    ::GetClientRect(bd->hWnd, &rect);
    if (g_bUseMyViewPort)
    {
        if (e_pGameResolutionSize)
        {
            io.DisplaySize = ImVec2(e_pGameResolutionSize[0], e_pGameResolutionSize[1]);// 
        }
        else
        {
            io.DisplaySize = ImVec2(cGameApp::m_spOpenGLRender->m_vGameResolution.x, cGameApp::m_spOpenGLRender->m_vGameResolution.y);// 
        }
        
    }
    else
    {
        io.DisplaySize = ImVec2((float)(rect.right - rect.left), (float)(rect.bottom - rect.top));
    }
    // Setup time step
    INT64 current_time = 0;
    ::QueryPerformanceCounter((LARGE_INTEGER*)&current_time);
    io.DeltaTime = (float)(current_time - bd->Time) / bd->TicksPerSecond;
    bd->Time = current_time;

    // Update OS mouse position
    ImGui_ImplWin32_UpdateMouseData();

    // Process workarounds for known Windows key handling issues
    ImGui_ImplWin32_ProcessKeyEventsWorkarounds();

    // Update OS mouse cursor with the cursor requested by imgui
    ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None : ImGui::GetMouseCursor();
    if (bd->LastMouseCursor != mouse_cursor)
    {
        bd->LastMouseCursor = mouse_cursor;
        ImGui_ImplWin32_UpdateMouseCursor();
    }

    // Update game controllers (if enabled and available)
    ImGui_ImplWin32_UpdateGamepads();
}

// Map VK_xxx to ImGuiKey_xxx.
// Not static to allow third-party code to use that if they want to (but undocumented)
ImGuiKey ImGui_ImplWin32_KeyEventToImGuiKey(WPARAM wParam, LPARAM lParam)
{
    // There is no distinct VK_xxx for keypad enter, instead it is VK_RETURN + KF_EXTENDED.
    if ((wParam == VK_RETURN) && (HIWORD(lParam) & KF_EXTENDED))
        return ImGuiKey_KeypadEnter;

    switch (wParam)
    {
    case VK_TAB: return ImGuiKey_Tab;
    case VK_LEFT: return ImGuiKey_LeftArrow;
    case VK_RIGHT: return ImGuiKey_RightArrow;
    case VK_UP: return ImGuiKey_UpArrow;
    case VK_DOWN: return ImGuiKey_DownArrow;
    case VK_PRIOR: return ImGuiKey_PageUp;
    case VK_NEXT: return ImGuiKey_PageDown;
    case VK_HOME: return ImGuiKey_Home;
    case VK_END: return ImGuiKey_End;
    case VK_INSERT: return ImGuiKey_Insert;
    case VK_DELETE: return ImGuiKey_Delete;
    case VK_BACK: return ImGuiKey_Backspace;
    case VK_SPACE: return ImGuiKey_Space;
    case VK_RETURN: return ImGuiKey_Enter;
    case VK_ESCAPE: return ImGuiKey_Escape;
    case VK_OEM_7: return ImGuiKey_Apostrophe;
    case VK_OEM_COMMA: return ImGuiKey_Comma;
    case VK_OEM_MINUS: return ImGuiKey_Minus;
    case VK_OEM_PERIOD: return ImGuiKey_Period;
    case VK_OEM_2: return ImGuiKey_Slash;
    case VK_OEM_1: return ImGuiKey_Semicolon;
    case VK_OEM_PLUS: return ImGuiKey_Equal;
    case VK_OEM_4: return ImGuiKey_LeftBracket;
    case VK_OEM_5: return ImGuiKey_Backslash;
    case VK_OEM_6: return ImGuiKey_RightBracket;
    case VK_OEM_3: return ImGuiKey_GraveAccent;
    case VK_CAPITAL: return ImGuiKey_CapsLock;
    case VK_SCROLL: return ImGuiKey_ScrollLock;
    case VK_NUMLOCK: return ImGuiKey_NumLock;
    case VK_SNAPSHOT: return ImGuiKey_PrintScreen;
    case VK_PAUSE: return ImGuiKey_Pause;
    case VK_NUMPAD0: return ImGuiKey_Keypad0;
    case VK_NUMPAD1: return ImGuiKey_Keypad1;
    case VK_NUMPAD2: return ImGuiKey_Keypad2;
    case VK_NUMPAD3: return ImGuiKey_Keypad3;
    case VK_NUMPAD4: return ImGuiKey_Keypad4;
    case VK_NUMPAD5: return ImGuiKey_Keypad5;
    case VK_NUMPAD6: return ImGuiKey_Keypad6;
    case VK_NUMPAD7: return ImGuiKey_Keypad7;
    case VK_NUMPAD8: return ImGuiKey_Keypad8;
    case VK_NUMPAD9: return ImGuiKey_Keypad9;
    case VK_DECIMAL: return ImGuiKey_KeypadDecimal;
    case VK_DIVIDE: return ImGuiKey_KeypadDivide;
    case VK_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case VK_SUBTRACT: return ImGuiKey_KeypadSubtract;
    case VK_ADD: return ImGuiKey_KeypadAdd;
    case VK_LSHIFT: return ImGuiKey_LeftShift;
    case VK_LCONTROL: return ImGuiKey_LeftCtrl;
    case VK_LMENU: return ImGuiKey_LeftAlt;
    case VK_LWIN: return ImGuiKey_LeftSuper;
    case VK_RSHIFT: return ImGuiKey_RightShift;
    case VK_RCONTROL: return ImGuiKey_RightCtrl;
    case VK_RMENU: return ImGuiKey_RightAlt;
    case VK_RWIN: return ImGuiKey_RightSuper;
    case VK_APPS: return ImGuiKey_Menu;
    case '0': return ImGuiKey_0;
    case '1': return ImGuiKey_1;
    case '2': return ImGuiKey_2;
    case '3': return ImGuiKey_3;
    case '4': return ImGuiKey_4;
    case '5': return ImGuiKey_5;
    case '6': return ImGuiKey_6;
    case '7': return ImGuiKey_7;
    case '8': return ImGuiKey_8;
    case '9': return ImGuiKey_9;
    case 'A': return ImGuiKey_A;
    case 'B': return ImGuiKey_B;
    case 'C': return ImGuiKey_C;
    case 'D': return ImGuiKey_D;
    case 'E': return ImGuiKey_E;
    case 'F': return ImGuiKey_F;
    case 'G': return ImGuiKey_G;
    case 'H': return ImGuiKey_H;
    case 'I': return ImGuiKey_I;
    case 'J': return ImGuiKey_J;
    case 'K': return ImGuiKey_K;
    case 'L': return ImGuiKey_L;
    case 'M': return ImGuiKey_M;
    case 'N': return ImGuiKey_N;
    case 'O': return ImGuiKey_O;
    case 'P': return ImGuiKey_P;
    case 'Q': return ImGuiKey_Q;
    case 'R': return ImGuiKey_R;
    case 'S': return ImGuiKey_S;
    case 'T': return ImGuiKey_T;
    case 'U': return ImGuiKey_U;
    case 'V': return ImGuiKey_V;
    case 'W': return ImGuiKey_W;
    case 'X': return ImGuiKey_X;
    case 'Y': return ImGuiKey_Y;
    case 'Z': return ImGuiKey_Z;
    case VK_F1: return ImGuiKey_F1;
    case VK_F2: return ImGuiKey_F2;
    case VK_F3: return ImGuiKey_F3;
    case VK_F4: return ImGuiKey_F4;
    case VK_F5: return ImGuiKey_F5;
    case VK_F6: return ImGuiKey_F6;
    case VK_F7: return ImGuiKey_F7;
    case VK_F8: return ImGuiKey_F8;
    case VK_F9: return ImGuiKey_F9;
    case VK_F10: return ImGuiKey_F10;
    case VK_F11: return ImGuiKey_F11;
    case VK_F12: return ImGuiKey_F12;
    case VK_F13: return ImGuiKey_F13;
    case VK_F14: return ImGuiKey_F14;
    case VK_F15: return ImGuiKey_F15;
    case VK_F16: return ImGuiKey_F16;
    case VK_F17: return ImGuiKey_F17;
    case VK_F18: return ImGuiKey_F18;
    case VK_F19: return ImGuiKey_F19;
    case VK_F20: return ImGuiKey_F20;
    case VK_F21: return ImGuiKey_F21;
    case VK_F22: return ImGuiKey_F22;
    case VK_F23: return ImGuiKey_F23;
    case VK_F24: return ImGuiKey_F24;
    case VK_BROWSER_BACK: return ImGuiKey_AppBack;
    case VK_BROWSER_FORWARD: return ImGuiKey_AppForward;
    default: return ImGuiKey_None;
    }
}

// Allow compilation with old Windows SDK. MinGW doesn't have default _WIN32_WINNT/WINVER versions.
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL 0x020E
#endif
#ifndef DBT_DEVNODES_CHANGED
#define DBT_DEVNODES_CHANGED 0x0007
#endif

// Win32 message handler (process Win32 mouse/keyboard inputs, etc.)
// Call from your application's message handler. Keep calling your message handler unless this function returns TRUE.
// When implementing your own backend, you can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if Dear ImGui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to Dear ImGui, and hide them from your application based on those two flags.
// PS: In this Win32 handler, we use the capture API (GetCapture/SetCapture/ReleaseCapture) to be able to read mouse coordinates when dragging mouse outside of our window bounds.
// PS: We treat DBLCLK messages as regular mouse down messages, so this code will work on windows classes that have the CS_DBLCLKS flag set. Our own example app code doesn't set this flag.
#if 0
// Copy this line into your .cpp file to forward declare the function.
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif

// See https://learn.microsoft.com/en-us/windows/win32/tablet/system-events-and-mouse-messages
// Prefer to call this at the top of the message handler to avoid the possibility of other Win32 calls interfering with this.
ImGuiMouseSource GetMouseSourceFromMessageExtraInfo()
{
    LPARAM extra_info = ::GetMessageExtraInfo();
    if ((extra_info & 0xFFFFFF80) == 0xFF515700)
        return ImGuiMouseSource_Pen;
    if ((extra_info & 0xFFFFFF80) == 0xFF515780)
        return ImGuiMouseSource_TouchScreen;
    return ImGuiMouseSource_Mouse;
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandlerInner(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam,int e_iContextIndex)
{
    std::function<void(long&, long&)> l_fImGuiGetCameraCursorPosition;
    auto l_IT = g_ContextIndexAndImGuiGetCameraCursorPositionMap.find(e_iContextIndex);
    if (l_IT != g_ContextIndexAndImGuiGetCameraCursorPositionMap.end())
    {
        l_fImGuiGetCameraCursorPosition = l_IT->second;
    }
    auto l_IT2 = g_ContextIndexAndMouseEventEnableMap.find(e_iContextIndex);
    if (l_IT2 != g_ContextIndexAndMouseEventEnableMap.end())
    {
        if (!l_IT2->second)
        {
            return false;
        }
    }
    // Most backends don't have silent checks like this one, but we need it because WndProc are called early in CreateWindow().
    // We silently allow both context or just only backend data to be nullptr.
    ImGui_ImplWin32_Data* bd = ImGui_ImplWin32_GetBackendData();
    if (bd == nullptr)
    {
        return 0;
    }
    ImGuiIO& io = ImGui::GetIO();

    switch (msg)
    {
        case WM_MOUSEMOVE:
        case WM_NCMOUSEMOVE:
        {
            // We need to call TrackMouseEvent in order to receive WM_MOUSELEAVE events
            ImGuiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
            const int area = (msg == WM_MOUSEMOVE) ? 1 : 2;
            bd->MouseHwnd = hwnd;
            if (bd->MouseTrackedArea != area)
            {
                TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hwnd, 0 };
                TRACKMOUSEEVENT tme_track = { sizeof(tme_track), (DWORD)((area == 2) ? (TME_LEAVE | TME_NONCLIENT) : TME_LEAVE), hwnd, 0 };
                if (bd->MouseTrackedArea != 0)
                    ::TrackMouseEvent(&tme_cancel);
                ::TrackMouseEvent(&tme_track);
                bd->MouseTrackedArea = area;
            }
            POINT mouse_pos = { (LONG)GET_X_LPARAM(lParam), (LONG)GET_Y_LPARAM(lParam) };
            if (msg == WM_NCMOUSEMOVE && ::ScreenToClient(hwnd, &mouse_pos) == FALSE) // WM_NCMOUSEMOVE are provided in absolute coordinates.
            {
                return 0;
            }
            if (g_bUseMyViewPort)
            {
                if (l_fImGuiGetCameraCursorPosition)
                {
                    l_fImGuiGetCameraCursorPosition(mouse_pos.x, mouse_pos.y);
                }
                else
                {
                    mouse_pos = cGameApp::m_sMousePosition;
                }
            }
            io.AddMouseSourceEvent(mouse_source);
            io.AddMousePosEvent((float)mouse_pos.x, (float)mouse_pos.y);
            return 0;
        }
        case WM_MOUSELEAVE:
        case WM_NCMOUSELEAVE:
        {
            const int area = (msg == WM_MOUSELEAVE) ? 1 : 2;
            if (bd->MouseTrackedArea == area)
            {
                if (bd->MouseHwnd == hwnd)
                    bd->MouseHwnd = nullptr;
                bd->MouseTrackedArea = 0;
                io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
            }
            return 0;
        }
        case WM_DESTROY:
        if (bd->MouseHwnd == hwnd && bd->MouseTrackedArea != 0)
        {
            TRACKMOUSEEVENT tme_cancel = { sizeof(tme_cancel), TME_CANCEL, hwnd, 0 };
            ::TrackMouseEvent(&tme_cancel);
            bd->MouseHwnd = nullptr;
            bd->MouseTrackedArea = 0;
            io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
        }
        return 0;
        case WM_LBUTTONDOWN: case WM_LBUTTONDBLCLK:
        case WM_RBUTTONDOWN: case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDOWN: case WM_MBUTTONDBLCLK:
        case WM_XBUTTONDOWN: case WM_XBUTTONDBLCLK:
        {
            ImGuiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
            int button = 0;
            if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
            {
                button = 0;
            }
            if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
            {
                button = 1;
            }
            if (msg == WM_MBUTTONDOWN || msg == WM_MBUTTONDBLCLK)
            {
                button = 2;
            }
            if (msg == WM_XBUTTONDOWN || msg == WM_XBUTTONDBLCLK)
            {
                button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
            }
            if (bd->MouseButtonsDown == 0 && ::GetCapture() == nullptr)
                ::SetCapture(hwnd);
            bd->MouseButtonsDown |= 1 << button;
            io.AddMouseSourceEvent(mouse_source);
            io.AddMouseButtonEvent(button, true);
            return 0;
        }
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
        case WM_XBUTTONUP:
        {
            ImGuiMouseSource mouse_source = GetMouseSourceFromMessageExtraInfo();
            int button = 0;
            if (msg == WM_LBUTTONUP)
            {
                button = 0;
            }
            if (msg == WM_RBUTTONUP)
            {
                button = 1;
            }
            if (msg == WM_MBUTTONUP)
            {
                button = 2;
            }
            if (msg == WM_XBUTTONUP)
            {
                button = (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? 3 : 4;
            }
            bd->MouseButtonsDown &= ~(1 << button);
            if (bd->MouseButtonsDown == 0 && ::GetCapture() == hwnd)
                ::ReleaseCapture();
            io.AddMouseSourceEvent(mouse_source);
            io.AddMouseButtonEvent(button, false);
            return 0;
        }
        case WM_MOUSEWHEEL:
        io.AddMouseWheelEvent(0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA);
        return 0;
        case WM_MOUSEHWHEEL:
        io.AddMouseWheelEvent(-(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA, 0.0f);
        return 0;
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            const bool is_key_down = (msg == WM_KEYDOWN || msg == WM_SYSKEYDOWN);
            if (wParam < 256)
            {
                // Submit modifiers
                ImGui_ImplWin32_UpdateKeyModifiers();

                // Obtain virtual key code and convert to ImGuiKey
                const ImGuiKey key = ImGui_ImplWin32_KeyEventToImGuiKey(wParam, lParam);
                const int vk = (int)wParam;
                const int scancode = (int)LOBYTE(HIWORD(lParam));

                // Special behavior for VK_SNAPSHOT / ImGuiKey_PrintScreen as Windows doesn't emit the key down event.
                if (key == ImGuiKey_PrintScreen && !is_key_down)
                    ImGui_ImplWin32_AddKeyEvent(key, true, vk, scancode);

                // Submit key event
                if (key != ImGuiKey_None)
                    ImGui_ImplWin32_AddKeyEvent(key, is_key_down, vk, scancode);

                // Submit individual left/right modifier events
                if (vk == VK_SHIFT)
                {
                    // Important: Shift keys tend to get stuck when pressed together, missing key-up events are corrected in ImGui_ImplWin32_ProcessKeyEventsWorkarounds()
                    if (IsVkDown(VK_LSHIFT) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_LeftShift, is_key_down, VK_LSHIFT, scancode);
                    }
                    if (IsVkDown(VK_RSHIFT) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_RightShift, is_key_down, VK_RSHIFT, scancode);
                    }
                }
                else if (vk == VK_CONTROL)
                {
                    if (IsVkDown(VK_LCONTROL) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_LeftCtrl, is_key_down, VK_LCONTROL, scancode);
                    }
                    if (IsVkDown(VK_RCONTROL) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_RightCtrl, is_key_down, VK_RCONTROL, scancode);
                    }
                }
                else if (vk == VK_MENU)
                {
                    if (IsVkDown(VK_LMENU) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_LeftAlt, is_key_down, VK_LMENU, scancode);
                    }
                    if (IsVkDown(VK_RMENU) == is_key_down)
                    {
                        ImGui_ImplWin32_AddKeyEvent(ImGuiKey_RightAlt, is_key_down, VK_RMENU, scancode);
                    }
                }
            }
            return 0;
        }
        case WM_SETFOCUS:
        case WM_KILLFOCUS:
        io.AddFocusEvent(msg == WM_SETFOCUS);
        return 0;
        case WM_INPUTLANGCHANGE:
        ImGui_ImplWin32_UpdateKeyboardCodePage();
        return 0;
        case WM_CHAR:
        if (::IsWindowUnicode(hwnd))
        {
            // You can also use ToAscii()+GetKeyboardState() to retrieve characters.
            if (wParam > 0 && wParam < 0x10000)
                io.AddInputCharacterUTF16((unsigned short)wParam);
        }
        else
        {
            wchar_t wch = 0;
            ::MultiByteToWideChar(bd->KeyboardCodePage, MB_PRECOMPOSED, (char*)&wParam, 1, &wch, 1);
            io.AddInputCharacter(wch);
        }
        return 0;
        case WM_SETCURSOR:
        // This is required to restore cursor when transitioning from e.g resize borders to client area.
        if (LOWORD(lParam) == HTCLIENT && ImGui_ImplWin32_UpdateMouseCursor())
            return 1;
        return 0;
        case WM_DEVICECHANGE:
#ifndef IMGUI_IMPL_WIN32_DISABLE_GAMEPAD
        if ((UINT)wParam == DBT_DEVNODES_CHANGED)
            bd->WantUpdateHasGamepad = true;
#endif
        return 0;
    }
    return 0;
}

IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (g_pImGuiContextVector)
    {
        for (size_t i = 0; i < g_pImGuiContextVector->size(); ++i)
        {
            ImGui::SetCurrentContext((*g_pImGuiContextVector)[i]);
            if (ImGui_ImplWin32_WndProcHandlerInner(hwnd, msg, wParam, lParam,(int)i))
            {
                return true;
            }
        }
    }
    else
    {
        if (ImGui_ImplWin32_WndProcHandlerInner(hwnd, msg, wParam, lParam,0))
        {
            return true;
        }
    }
    return 0;
}


//--------------------------------------------------------------------------------------------------------
// DPI-related helpers (optional)
//--------------------------------------------------------------------------------------------------------
// - Use to enable DPI awareness without having to create an application manifest.
// - Your own app may already do this via a manifest or explicit calls. This is mostly useful for our examples/ apps.
// - In theory we could call simple functions from Windows SDK such as SetProcessDPIAware(), SetProcessDpiAwareness(), etc.
//   but most of the functions provided by Microsoft require Windows 8.1/10+ SDK at compile time and Windows 8/10+ at runtime,
//   neither we want to require the user to have. So we dynamically select and load those functions to avoid dependencies.
//---------------------------------------------------------------------------------------------------------
// This is the scheme successfully used by GLFW (from which we borrowed some of the code) and other apps aiming to be highly portable.
// ImGui_ImplWin32_EnableDpiAwareness() is just a helper called by main.cpp, we don't call it automatically.
// If you are trying to implement your own backend for your own engine, you may ignore that noise.
//---------------------------------------------------------------------------------------------------------

// Perform our own check with RtlVerifyVersionInfo() instead of using functions from <VersionHelpers.h> as they
// require a manifest to be functional for checks above 8.1. See https://github.com/ocornut/imgui/issues/4200
BOOL _IsWindowsVersionOrGreater(WORD major, WORD minor, WORD)
{
    typedef LONG(WINAPI* PFN_RtlVerifyVersionInfo)(OSVERSIONINFOEXW*, ULONG, ULONGLONG);
    static PFN_RtlVerifyVersionInfo RtlVerifyVersionInfoFn = nullptr;
    if (RtlVerifyVersionInfoFn == nullptr)
        if (HMODULE ntdllModule = ::GetModuleHandleA("ntdll.dll"))
            RtlVerifyVersionInfoFn = (PFN_RtlVerifyVersionInfo)GetProcAddress(ntdllModule, "RtlVerifyVersionInfo");
    if (RtlVerifyVersionInfoFn == nullptr)
        return FALSE;

    RTL_OSVERSIONINFOEXW versionInfo = { };
    ULONGLONG conditionMask = 0;
    versionInfo.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
    versionInfo.dwMajorVersion = major;
    versionInfo.dwMinorVersion = minor;
    VER_SET_CONDITION(conditionMask, VER_MAJORVERSION, VER_GREATER_EQUAL);
    VER_SET_CONDITION(conditionMask, VER_MINORVERSION, VER_GREATER_EQUAL);
    return (RtlVerifyVersionInfoFn(&versionInfo, VER_MAJORVERSION | VER_MINORVERSION, conditionMask) == 0) ? TRUE : FALSE;
}

#define _IsWindowsVistaOrGreater()   _IsWindowsVersionOrGreater(HIBYTE(0x0600), LOBYTE(0x0600), 0) // _WIN32_WINNT_VISTA
#define _IsWindows8OrGreater()       _IsWindowsVersionOrGreater(HIBYTE(0x0602), LOBYTE(0x0602), 0) // _WIN32_WINNT_WIN8
#define _IsWindows8Point1OrGreater() _IsWindowsVersionOrGreater(HIBYTE(0x0603), LOBYTE(0x0603), 0) // _WIN32_WINNT_WINBLUE
#define _IsWindows10OrGreater()      _IsWindowsVersionOrGreater(HIBYTE(0x0A00), LOBYTE(0x0A00), 0) // _WIN32_WINNT_WINTHRESHOLD / _WIN32_WINNT_WIN10

#ifndef DPI_ENUMS_DECLARED
typedef enum { PROCESS_DPI_UNAWARE = 0, PROCESS_SYSTEM_DPI_AWARE = 1, PROCESS_PER_MONITOR_DPI_AWARE = 2 } PROCESS_DPI_AWARENESS;
typedef enum { MDT_EFFECTIVE_DPI = 0, MDT_ANGULAR_DPI = 1, MDT_RAW_DPI = 2, MDT_DEFAULT = MDT_EFFECTIVE_DPI } MONITOR_DPI_TYPE;
#endif
#ifndef _DPI_AWARENESS_CONTEXTS_
DECLARE_HANDLE(DPI_AWARENESS_CONTEXT);
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE    (DPI_AWARENESS_CONTEXT)-3
#endif
#ifndef DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2
#define DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2 (DPI_AWARENESS_CONTEXT)-4
#endif
typedef HRESULT(WINAPI* PFN_SetProcessDpiAwareness)(PROCESS_DPI_AWARENESS);                     // Shcore.lib + dll, Windows 8.1+
typedef HRESULT(WINAPI* PFN_GetDpiForMonitor)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);        // Shcore.lib + dll, Windows 8.1+
typedef DPI_AWARENESS_CONTEXT(WINAPI* PFN_SetThreadDpiAwarenessContext)(DPI_AWARENESS_CONTEXT); // User32.lib + dll, Windows 10 v1607+ (Creators Update)

// Helper function to enable DPI awareness without setting up a manifest
void ImGui_ImplWin32_EnableDpiAwareness()
{
    if (_IsWindows10OrGreater())
    {
        static HINSTANCE user32_dll = ::LoadLibraryA("user32.dll"); // Reference counted per-process
        if (PFN_SetThreadDpiAwarenessContext SetThreadDpiAwarenessContextFn = (PFN_SetThreadDpiAwarenessContext)::GetProcAddress(user32_dll, "SetThreadDpiAwarenessContext"))
        {
            SetThreadDpiAwarenessContextFn(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);
            return;
        }
    }
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
        if (PFN_SetProcessDpiAwareness SetProcessDpiAwarenessFn = (PFN_SetProcessDpiAwareness)::GetProcAddress(shcore_dll, "SetProcessDpiAwareness"))
        {
            SetProcessDpiAwarenessFn(PROCESS_PER_MONITOR_DPI_AWARE);
            return;
        }
    }
#if _WIN32_WINNT >= 0x0600
    ::SetProcessDPIAware();
#endif
}

#if defined(_MSC_VER) && !defined(NOGDI)
#pragma comment(lib, "gdi32")   // Link with gdi32.lib for GetDeviceCaps(). MinGW will require linking with '-lgdi32'
#endif

float ImGui_ImplWin32_GetDpiScaleForMonitor(void* monitor)
{
    UINT xdpi = 96, ydpi = 96;
    if (_IsWindows8Point1OrGreater())
    {
        static HINSTANCE shcore_dll = ::LoadLibraryA("shcore.dll"); // Reference counted per-process
        static PFN_GetDpiForMonitor GetDpiForMonitorFn = nullptr;
        if (GetDpiForMonitorFn == nullptr && shcore_dll != nullptr)
            GetDpiForMonitorFn = (PFN_GetDpiForMonitor)::GetProcAddress(shcore_dll, "GetDpiForMonitor");
        if (GetDpiForMonitorFn != nullptr)
        {
            GetDpiForMonitorFn((HMONITOR)monitor, MDT_EFFECTIVE_DPI, &xdpi, &ydpi);
            IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
            return xdpi / 96.0f;
        }
    }
#ifndef NOGDI
    const HDC dc = ::GetDC(nullptr);
    xdpi = ::GetDeviceCaps(dc, LOGPIXELSX);
    ydpi = ::GetDeviceCaps(dc, LOGPIXELSY);
    IM_ASSERT(xdpi == ydpi); // Please contact me if you hit this assert!
    ::ReleaseDC(nullptr, dc);
#endif
    return xdpi / 96.0f;
}

float ImGui_ImplWin32_GetDpiScaleForHwnd(void* hwnd)
{
    HMONITOR monitor = ::MonitorFromWindow((HWND)hwnd, MONITOR_DEFAULTTONEAREST);
    return ImGui_ImplWin32_GetDpiScaleForMonitor(monitor);
}

//---------------------------------------------------------------------------------------------------------
// Transparency related helpers (optional)
//--------------------------------------------------------------------------------------------------------

#if defined(_MSC_VER)
#pragma comment(lib, "dwmapi")  // Link with dwmapi.lib. MinGW will require linking with '-ldwmapi'
#endif

#elif defined(WASM)

const char* ImGui_ImplSDL2_GetClipboardText(ImGuiContext*)
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    if (bd->ClipboardTextData)
        SDL_free(bd->ClipboardTextData);
    bd->ClipboardTextData = SDL_GetClipboardText();
    return bd->ClipboardTextData;
}

void ImGui_ImplSDL2_SetClipboardText(ImGuiContext*, const char* text)
{
    SDL_SetClipboardText(text);
}

void ImGui_ImplSDL2_PlatformSetImeData(ImGuiContext*, ImGuiViewport*, ImGuiPlatformImeData* data)
{
    if (data->WantVisible)
    {
        SDL_Rect r;
        r.x = (int)data->InputPos.x;
        r.y = (int)data->InputPos.y;
        r.w = 1;
        r.h = (int)data->InputLineHeight;
        SDL_SetTextInputRect(&r);
    }
}
#ifdef USE_SDL2
bool ImGui_ImplSDL2_Init(SDL_Window* window)
#else
bool ImGui_ImplSDL2_Init()
#endif
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    FMLOG("ImGui_ImplSDL2_Init called");
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.BackendPlatformUserData == nullptr && "Already initialized a platform backend!");

    // Check and store if we are on a SDL backend that supports global mouse position
    // ("wayland" and "rpi" don't support it, but we chose to use a white-list instead of a black-list)
    bool mouse_can_use_global_state = false;
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
    const char* sdl_backend = SDL_GetCurrentVideoDriver();
    const char* global_mouse_whitelist[] = { "windows", "cocoa", "x11", "DIVE", "VMAN" };
    for (int n = 0; n < IM_ARRAYSIZE(global_mouse_whitelist); n++)
        if (strncmp(sdl_backend, global_mouse_whitelist[n], strlen(global_mouse_whitelist[n])) == 0)
            mouse_can_use_global_state = true;
#endif

    // Setup backend capabilities flags
    ImGui_ImplOpenGL3_Data* bd = IM_NEW(ImGui_ImplOpenGL3_Data)();
    io.BackendPlatformUserData = (void*)bd;
    io.BackendPlatformName = "imgui_impl_sdl2";
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;       // We can honor GetMouseCursor() values (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;        // We can honor io.WantSetMousePos requests (optional, rarely used)
#ifdef USE_SDL2
        //bd->Window = window;
        //bd->WindowID = SDL_GetWindowID(window);
#endif
    bd->MouseCanUseGlobalState = mouse_can_use_global_state;

    ImGuiPlatformIO& platform_io = ImGui::GetPlatformIO();
    platform_io.Platform_SetClipboardTextFn = ImGui_ImplSDL2_SetClipboardText;
    platform_io.Platform_GetClipboardTextFn = ImGui_ImplSDL2_GetClipboardText;
    platform_io.Platform_ClipboardUserData = nullptr;
    platform_io.Platform_SetImeDataFn = ImGui_ImplSDL2_PlatformSetImeData;
#ifdef __EMSCRIPTEN__
    platform_io.Platform_OpenInShellFn = [](ImGuiContext*, const char* url) { ImGui_ImplSDL2_EmscriptenOpenURL(url); return true; };
#endif

    // Gamepad handling
    //bd->GamepadMode = ImGui_ImplSDL2_GamepadMode_AutoFirst;
    bd->WantUpdateGamepadsList = true;
#ifdef USE_SDL2
    // Load mouse cursors
    bd->MouseCursors[ImGuiMouseCursor_Arrow] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    bd->MouseCursors[ImGuiMouseCursor_TextInput] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    bd->MouseCursors[ImGuiMouseCursor_Hand] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
    SDL_SysWMinfo info;
    SDL_VERSION(&info.version);
    if (SDL_GetWindowWMInfo(window, &info))
    {
#if defined(SDL_VIDEO_DRIVER_WINDOWS)
        main_viewport->PlatformHandleRaw = (void*)info.info.win.window;
#elif defined(__APPLE__) && defined(SDL_VIDEO_DRIVER_COCOA)
        main_viewport->PlatformHandleRaw = (void*)info.info.cocoa.window;
#endif
    }
#else
    bd->MouseCursors[ImGuiMouseCursor_Arrow] = 0;// SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
    bd->MouseCursors[ImGuiMouseCursor_TextInput] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    bd->MouseCursors[ImGuiMouseCursor_ResizeAll] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEALL);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNS] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENS);
    bd->MouseCursors[ImGuiMouseCursor_ResizeEW] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZEWE);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNESW] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENESW);
    bd->MouseCursors[ImGuiMouseCursor_ResizeNWSE] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_SIZENWSE);
    bd->MouseCursors[ImGuiMouseCursor_Hand] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    bd->MouseCursors[ImGuiMouseCursor_NotAllowed] = 0;//SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_NO);
#endif
    // Set platform dependent data in viewport
    // Our mouse update function expect PlatformHandle to be filled for the main viewport
    ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    main_viewport->PlatformHandle = (void*)(intptr_t)bd->WindowID;
    main_viewport->PlatformHandleRaw = nullptr;    

    // From 2.0.5: Set SDL hint to receive mouse click events on window focus, otherwise SDL doesn't emit the event.
    // Without this, when clicking to gain focus, our widgets wouldn't activate even though they showed as hovered.
    // (This is unfortunately a global SDL setting, so enabling it might have a side-effect on your application.
    // It is unlikely to make a difference, but if your app absolutely needs to ignore the initial on-focus click:
    // you can ignore SDL_MOUSEBUTTONDOWN events coming right after a SDL_WINDOWEVENT_FOCUS_GAINED)
#ifdef SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif

    // From 2.0.18: Enable native IME.
    // IMPORTANT: This is used at the time of SDL_CreateWindow() so this will only affects secondary windows, if any.
    // For the main window to be affected, your application needs to call this manually before calling SDL_CreateWindow().
#ifdef SDL_HINT_IME_SHOW_UI
    SDL_SetHint(SDL_HINT_IME_SHOW_UI, "1");
#endif

    // From 2.0.22: Disable auto-capture, this is preventing drag and drop across multiple windows (see #5710)
#ifdef SDL_HINT_MOUSE_AUTO_CAPTURE
    SDL_SetHint(SDL_HINT_MOUSE_AUTO_CAPTURE, "0");
#endif
    return true;
}

//void ImGui_ImplSDL2_CloseGamepads()
//{
//    ImGui_ImplSDL2_Data* bd = ImGui_ImplSDL2_GetBackendData();
//    if (bd->GamepadMode != ImGui_ImplSDL2_GamepadMode_Manual)
//        for (SDL_GameController* gamepad : bd->Gamepads)
//            SDL_GameControllerClose(gamepad);
//    bd->Gamepads.resize(0);
//}
//
//void ImGui_ImplSDL2_SetGamepadMode(ImGui_ImplSDL2_GamepadMode mode, struct _SDL_GameController** manual_gamepads_array, int manual_gamepads_count)
//{
//    ImGui_ImplSDL2_Data* bd = ImGui_ImplSDL2_GetBackendData();
//    ImGui_ImplSDL2_CloseGamepads();
//    if (mode == ImGui_ImplSDL2_GamepadMode_Manual)
//    {
//        IM_ASSERT(manual_gamepads_array != nullptr && manual_gamepads_count > 0);
//        for (int n = 0; n < manual_gamepads_count; n++)
//            bd->Gamepads.push_back(manual_gamepads_array[n]);
//    }
//    else
//    {
//        IM_ASSERT(manual_gamepads_array == nullptr && manual_gamepads_count <= 0);
//        bd->WantUpdateGamepadsList = true;
//    }
//    bd->GamepadMode = mode;
//}

ImGuiViewport* ImGui_ImplSDL2_GetViewportForWindowID(Uint32 window_id)
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    return (window_id == bd->WindowID) ? ImGui::GetMainViewport() : NULL;
}

void ImGui_ImplSDL2_UpdateKeyModifiers(SDL_Keymod sdl_key_mods)
{
    ImGuiIO& io = ImGui::GetIO();
    io.AddKeyEvent(ImGuiMod_Ctrl, (sdl_key_mods & KMOD_CTRL) != 0);
    io.AddKeyEvent(ImGuiMod_Shift, (sdl_key_mods & KMOD_SHIFT) != 0);
    io.AddKeyEvent(ImGuiMod_Alt, (sdl_key_mods & KMOD_ALT) != 0);
    io.AddKeyEvent(ImGuiMod_Super, (sdl_key_mods & KMOD_GUI) != 0);
}


ImGuiKey ImGui_ImplSDL2_KeyEventToImGuiKey(SDL_Keycode keycode, SDL_Scancode scancode)
{
    IM_UNUSED(scancode);
    switch (keycode)
    {
    case SDLK_TAB: return ImGuiKey_Tab;
    case SDLK_LEFT: return ImGuiKey_LeftArrow;
    case SDLK_RIGHT: return ImGuiKey_RightArrow;
    case SDLK_UP: return ImGuiKey_UpArrow;
    case SDLK_DOWN: return ImGuiKey_DownArrow;
    case SDLK_PAGEUP: return ImGuiKey_PageUp;
    case SDLK_PAGEDOWN: return ImGuiKey_PageDown;
    case SDLK_HOME: return ImGuiKey_Home;
    case SDLK_END: return ImGuiKey_End;
    case SDLK_INSERT: return ImGuiKey_Insert;
    case SDLK_DELETE: return ImGuiKey_Delete;
    case SDLK_BACKSPACE: return ImGuiKey_Backspace;
    case SDLK_SPACE: return ImGuiKey_Space;
    case SDLK_RETURN: return ImGuiKey_Enter;
    case SDLK_ESCAPE: return ImGuiKey_Escape;
    case SDLK_QUOTE: return ImGuiKey_Apostrophe;
    case SDLK_COMMA: return ImGuiKey_Comma;
    case SDLK_MINUS: return ImGuiKey_Minus;
    case SDLK_PERIOD: return ImGuiKey_Period;
    case SDLK_SLASH: return ImGuiKey_Slash;
    case SDLK_SEMICOLON: return ImGuiKey_Semicolon;
    case SDLK_EQUALS: return ImGuiKey_Equal;
    case SDLK_LEFTBRACKET: return ImGuiKey_LeftBracket;
    case SDLK_BACKSLASH: return ImGuiKey_Backslash;
    case SDLK_RIGHTBRACKET: return ImGuiKey_RightBracket;
    case SDLK_BACKQUOTE: return ImGuiKey_GraveAccent;
    case SDLK_CAPSLOCK: return ImGuiKey_CapsLock;
    case SDLK_SCROLLLOCK: return ImGuiKey_ScrollLock;
    case SDLK_NUMLOCKCLEAR: return ImGuiKey_NumLock;
    case SDLK_PRINTSCREEN: return ImGuiKey_PrintScreen;
    case SDLK_PAUSE: return ImGuiKey_Pause;
    case SDLK_KP_0: return ImGuiKey_Keypad0;
    case SDLK_KP_1: return ImGuiKey_Keypad1;
    case SDLK_KP_2: return ImGuiKey_Keypad2;
    case SDLK_KP_3: return ImGuiKey_Keypad3;
    case SDLK_KP_4: return ImGuiKey_Keypad4;
    case SDLK_KP_5: return ImGuiKey_Keypad5;
    case SDLK_KP_6: return ImGuiKey_Keypad6;
    case SDLK_KP_7: return ImGuiKey_Keypad7;
    case SDLK_KP_8: return ImGuiKey_Keypad8;
    case SDLK_KP_9: return ImGuiKey_Keypad9;
    case SDLK_KP_PERIOD: return ImGuiKey_KeypadDecimal;
    case SDLK_KP_DIVIDE: return ImGuiKey_KeypadDivide;
    case SDLK_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
    case SDLK_KP_MINUS: return ImGuiKey_KeypadSubtract;
    case SDLK_KP_PLUS: return ImGuiKey_KeypadAdd;
    case SDLK_KP_ENTER: return ImGuiKey_KeypadEnter;
    case SDLK_KP_EQUALS: return ImGuiKey_KeypadEqual;
    case SDLK_LCTRL: return ImGuiKey_LeftCtrl;
    case SDLK_LSHIFT: return ImGuiKey_LeftShift;
    case SDLK_LALT: return ImGuiKey_LeftAlt;
    case SDLK_LGUI: return ImGuiKey_LeftSuper;
    case SDLK_RCTRL: return ImGuiKey_RightCtrl;
    case SDLK_RSHIFT: return ImGuiKey_RightShift;
    case SDLK_RALT: return ImGuiKey_RightAlt;
    case SDLK_RGUI: return ImGuiKey_RightSuper;
    case SDLK_APPLICATION: return ImGuiKey_Menu;
    case SDLK_0: return ImGuiKey_0;
    case SDLK_1: return ImGuiKey_1;
    case SDLK_2: return ImGuiKey_2;
    case SDLK_3: return ImGuiKey_3;
    case SDLK_4: return ImGuiKey_4;
    case SDLK_5: return ImGuiKey_5;
    case SDLK_6: return ImGuiKey_6;
    case SDLK_7: return ImGuiKey_7;
    case SDLK_8: return ImGuiKey_8;
    case SDLK_9: return ImGuiKey_9;
    case SDLK_a: return ImGuiKey_A;
    case SDLK_b: return ImGuiKey_B;
    case SDLK_c: return ImGuiKey_C;
    case SDLK_d: return ImGuiKey_D;
    case SDLK_e: return ImGuiKey_E;
    case SDLK_f: return ImGuiKey_F;
    case SDLK_g: return ImGuiKey_G;
    case SDLK_h: return ImGuiKey_H;
    case SDLK_i: return ImGuiKey_I;
    case SDLK_j: return ImGuiKey_J;
    case SDLK_k: return ImGuiKey_K;
    case SDLK_l: return ImGuiKey_L;
    case SDLK_m: return ImGuiKey_M;
    case SDLK_n: return ImGuiKey_N;
    case SDLK_o: return ImGuiKey_O;
    case SDLK_p: return ImGuiKey_P;
    case SDLK_q: return ImGuiKey_Q;
    case SDLK_r: return ImGuiKey_R;
    case SDLK_s: return ImGuiKey_S;
    case SDLK_t: return ImGuiKey_T;
    case SDLK_u: return ImGuiKey_U;
    case SDLK_v: return ImGuiKey_V;
    case SDLK_w: return ImGuiKey_W;
    case SDLK_x: return ImGuiKey_X;
    case SDLK_y: return ImGuiKey_Y;
    case SDLK_z: return ImGuiKey_Z;
    case SDLK_F1: return ImGuiKey_F1;
    case SDLK_F2: return ImGuiKey_F2;
    case SDLK_F3: return ImGuiKey_F3;
    case SDLK_F4: return ImGuiKey_F4;
    case SDLK_F5: return ImGuiKey_F5;
    case SDLK_F6: return ImGuiKey_F6;
    case SDLK_F7: return ImGuiKey_F7;
    case SDLK_F8: return ImGuiKey_F8;
    case SDLK_F9: return ImGuiKey_F9;
    case SDLK_F10: return ImGuiKey_F10;
    case SDLK_F11: return ImGuiKey_F11;
    case SDLK_F12: return ImGuiKey_F12;
    case SDLK_F13: return ImGuiKey_F13;
    case SDLK_F14: return ImGuiKey_F14;
    case SDLK_F15: return ImGuiKey_F15;
    case SDLK_F16: return ImGuiKey_F16;
    case SDLK_F17: return ImGuiKey_F17;
    case SDLK_F18: return ImGuiKey_F18;
    case SDLK_F19: return ImGuiKey_F19;
    case SDLK_F20: return ImGuiKey_F20;
    case SDLK_F21: return ImGuiKey_F21;
    case SDLK_F22: return ImGuiKey_F22;
    case SDLK_F23: return ImGuiKey_F23;
    case SDLK_F24: return ImGuiKey_F24;
    case SDLK_AC_BACK: return ImGuiKey_AppBack;
    case SDLK_AC_FORWARD: return ImGuiKey_AppForward;
    default: break;
    }
    return ImGuiKey_None;
}


bool ImGui_ImplSDL2_ProcessEvent(const SDL_Event* event)
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplSDL2_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    switch (event->type)
    {
    case SDL_MOUSEMOTION:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->motion.windowID) == NULL)
            return false;
        ImVec2 mouse_pos((float)event->motion.x, (float)event->motion.y);
        io.AddMouseSourceEvent(event->motion.which == SDL_TOUCH_MOUSEID ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
        if (g_bUseMyViewPort)
        {
            io.AddMousePosEvent(cGameApp::m_sMousePosition.x, cGameApp::m_sMousePosition.y);
        }
        else
        {
            io.AddMousePosEvent(mouse_pos.x, mouse_pos.y);
        }
        return true;
    }
    case SDL_MOUSEWHEEL:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->wheel.windowID) == NULL)
            return false;
        //IMGUI_DEBUG_LOG("wheel %.2f %.2f, precise %.2f %.2f\n", (float)event->wheel.x, (float)event->wheel.y, event->wheel.preciseX, event->wheel.preciseY);
#if SDL_VERSION_ATLEAST(2,0,18) // If this fails to compile on Emscripten: update to latest Emscripten!
        float wheel_x = -event->wheel.preciseX;
        float wheel_y = event->wheel.preciseY;
#else
        float wheel_x = -(float)event->wheel.x;
        float wheel_y = (float)event->wheel.y;
#endif
#ifdef __EMSCRIPTEN__
        wheel_x /= 100.0f;
#endif
        io.AddMouseSourceEvent(event->wheel.which == SDL_TOUCH_MOUSEID ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
        io.AddMouseWheelEvent(wheel_x, wheel_y);
        return true;
    }
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->button.windowID) == NULL)
            return false;
        int mouse_button = -1;
        if (event->button.button == SDL_BUTTON_LEFT) { mouse_button = 0; }
        if (event->button.button == SDL_BUTTON_RIGHT) { mouse_button = 1; }
        if (event->button.button == SDL_BUTTON_MIDDLE) { mouse_button = 2; }
        if (event->button.button == SDL_BUTTON_X1) { mouse_button = 3; }
        if (event->button.button == SDL_BUTTON_X2) { mouse_button = 4; }
        if (mouse_button == -1)
            break;
        io.AddMouseSourceEvent(event->button.which == SDL_TOUCH_MOUSEID ? ImGuiMouseSource_TouchScreen : ImGuiMouseSource_Mouse);
        io.AddMouseButtonEvent(mouse_button, (event->type == SDL_MOUSEBUTTONDOWN));
        bd->MouseButtonsDown = (event->type == SDL_MOUSEBUTTONDOWN) ? (bd->MouseButtonsDown | (1 << mouse_button)) : (bd->MouseButtonsDown & ~(1 << mouse_button));
        return true;
    }
    case SDL_TEXTINPUT:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->text.windowID) == NULL)
            return false;
        io.AddInputCharactersUTF8(event->text.text);
        return true;
    }
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->key.windowID) == NULL)
            return false;
        ImGui_ImplSDL2_UpdateKeyModifiers((SDL_Keymod)event->key.keysym.mod);
        ImGuiKey key = ImGui_ImplSDL2_KeyEventToImGuiKey(event->key.keysym.sym, event->key.keysym.scancode);
        io.AddKeyEvent(key, (event->type == SDL_KEYDOWN));
        io.SetKeyEventNativeData(key, event->key.keysym.sym, event->key.keysym.scancode, event->key.keysym.scancode); // To support legacy indexing (<1.87 user code). Legacy backend uses SDLK_*** as indices to IsKeyXXX() functions.
        return true;
    }
    case SDL_WINDOWEVENT:
    {
        if (ImGui_ImplSDL2_GetViewportForWindowID(event->window.windowID) == NULL)
            return false;
        // - When capturing mouse, SDL will send a bunch of conflicting LEAVE/ENTER event on every mouse move, but the final ENTER tends to be right.
        // - However we won't get a correct LEAVE event for a captured window.
        // - In some cases, when detaching a window from main viewport SDL may send SDL_WINDOWEVENT_ENTER one frame too late,
        //   causing SDL_WINDOWEVENT_LEAVE on previous frame to interrupt drag operation by clear mouse position. This is why
        //   we delay process the SDL_WINDOWEVENT_LEAVE events by one frame. See issue #5012 for details.
        Uint8 window_event = event->window.event;
        if (window_event == SDL_WINDOWEVENT_ENTER)
        {
            bd->MouseWindowID = event->window.windowID;
            bd->MouseLastLeaveFrame = 0;
        }
        if (window_event == SDL_WINDOWEVENT_LEAVE)
            bd->MouseLastLeaveFrame = ImGui::GetFrameCount() + 1;
        if (window_event == SDL_WINDOWEVENT_FOCUS_GAINED)
            io.AddFocusEvent(true);
        else if (event->window.event == SDL_WINDOWEVENT_FOCUS_LOST)
            io.AddFocusEvent(false);
        return true;
    }
#ifdef USE_SDL2
    case SDL_CONTROLLERDEVICEADDED:
    case SDL_CONTROLLERDEVICEREMOVED:
    {
        bd->WantUpdateGamepadsList = true;
        return true;
    }
#endif
    }
    //case SDL_FINGERDOWN: 
    //{
    //    float touchX = event.tfinger.x * windowWidth;
    //    float touchY = event.tfinger.y * windowHeight;
    //    ImGui::GetIO().MousePos = ImVec2(touchX, touchY);
    //    ImGui::GetIO().MouseDown[0] = true;  // Simulate left click
    //    break;
    //}
    //case SDL_FINGERMOTION: {
    //    float touchX = event.tfinger.x * windowWidth;
    //    float touchY = event.tfinger.y * windowHeight;
    //    ImGui::GetIO().MousePos = ImVec2(touchX, touchY);
    //    break;
    //}
    //case SDL_FINGERUP:
    //    ImGui::GetIO().MouseDown[0] = false;  // Simulate left click release
    //    break;
    return false;
}


void ImGui_ImplSDL2_UpdateMouseData()
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    ImGuiIO& io = ImGui::GetIO();

    // We forward mouse input when hovered or captured (via SDL_MOUSEMOTION) or when focused (below)
#if SDL_HAS_CAPTURE_AND_GLOBAL_MOUSE
    // SDL_CaptureMouse() let the OS know e.g. that our imgui drag outside the SDL window boundaries shouldn't e.g. trigger other operations outside
    SDL_CaptureMouse((bd->MouseButtonsDown != 0) ? SDL_TRUE : SDL_FALSE);
    SDL_Window* focused_window = SDL_GetKeyboardFocus();
    const bool is_app_focused = (bd->Window == focused_window);
#else
    const bool is_app_focused = (SDL_GetWindowFlags(bd->Window) & SDL_WINDOW_INPUT_FOCUS) != 0; // SDL 2.0.3 and non-windowed systems: single-viewport only
#endif
    if (is_app_focused && 0)
    {
        // (Optional) Set OS mouse position from Dear ImGui if requested (rarely used, only when ImGuiConfigFlags_NavEnableSetMousePos is enabled by user)
        if (io.WantSetMousePos)
            SDL_WarpMouseInWindow(bd->Window, (int)io.MousePos.x, (int)io.MousePos.y);

        // (Optional) Fallback to provide mouse position when focused (SDL_MOUSEMOTION already provides this when hovered or captured)
        if (bd->MouseCanUseGlobalState && bd->MouseButtonsDown == 0)
        {
            int window_x, window_y, mouse_x_global, mouse_y_global;
#ifdef USE_SDL2
            SDL_GetGlobalMouseState(&mouse_x_global, &mouse_y_global);
#else
            SDL_GetMouseState(&mouse_x_global, &mouse_y_global);
#endif
            SDL_GetWindowPosition(bd->Window, &window_x, &window_y);
            io.AddMousePosEvent((float)(mouse_x_global - window_x), (float)(mouse_y_global - window_y));
        }
    }
}

void ImGui_ImplSDL2_UpdateMouseCursor()
{
    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_NoMouseCursorChange)
        return;
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();

    ImGuiMouseCursor imgui_cursor = ImGui::GetMouseCursor();
    if (io.MouseDrawCursor || imgui_cursor == ImGuiMouseCursor_None)
    {
        // Hide OS mouse cursor if imgui is drawing it or if it wants no cursor
        SDL_ShowCursor(SDL_FALSE);
    }
    else
    {
        // Show OS mouse cursor
        SDL_Cursor* expected_cursor = bd->MouseCursors[imgui_cursor] ? bd->MouseCursors[imgui_cursor] : bd->MouseCursors[ImGuiMouseCursor_Arrow];
        if (bd->MouseLastCursor != expected_cursor)
        {
            SDL_SetCursor(expected_cursor); // SDL function doesn't have an early out (see #6113)
            bd->MouseLastCursor = expected_cursor;
        }
        SDL_ShowCursor(SDL_TRUE);
    }
}

void ImGui_ImplSDL2_NewFrame()
{
    ImGui_ImplOpenGL3_Data* bd = ImGui_ImplOpenGL3_GetBackendData();
    IM_ASSERT(bd != nullptr && "Context or backend not initialized! Did you call ImGui_ImplSDL2_Init()?");
    ImGuiIO& io = ImGui::GetIO();

    // Setup display size (every frame to accommodate for window resizing)
    int w, h;
    int display_w, display_h;
    //SDL_GetWindowSize(bd->Window, &w, &h);
    if (SDL_GetWindowFlags(bd->Window) & SDL_WINDOW_MINIMIZED)
    {
        w = h = 0;
    }
//    if (bd->Renderer != nullptr)
//    {
//        SDL_GetRendererOutputSize(bd->Renderer, &display_w, &display_h);
//    }
//#if SDL_HAS_VULKAN
//    else if (SDL_GetWindowFlags(bd->Window) & SDL_WINDOW_VULKAN)
//        SDL_Vulkan_GetDrawableSize(bd->Window, &display_w, &display_h);
//#endif
//    else
#ifdef USE_SDL2
        SDL_GL_GetDrawableSize(bd->Window, &display_w, &display_h);
#endif
    int	l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
    int	l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();


    //w = l_iViewportWidth;
    //h = l_iViewportHeight;
    //display_w = w;
    //display_h = h;
    //io.DisplaySize = ImVec2(display_w, display_h);

    if (g_bUseMyViewPort)
    {
        w = display_w = cGameApp::m_spOpenGLRender->m_vGameResolution.x;
        h = display_h = cGameApp::m_spOpenGLRender->m_vGameResolution.y;
    }
    else
    {
        w = display_w = l_iViewportWidth;
        h = display_h = l_iViewportHeight;
    }

    io.DisplaySize = ImVec2(w, h);

    // Setup time step (we don't use SDL_GetTicks() because it is using millisecond resolution)
    // (Accept SDL_GetPerformanceCounter() not returning a monotonically increasing value. Happens in VMs and Emscripten, see #6189, #6114, #3644)
#ifdef USE_SDL2
    static Uint64 frequency = SDL_GetPerformanceFrequency();
    Uint64 current_time = SDL_GetPerformanceCounter();
    if (current_time <= bd->Time)
        current_time = bd->Time + 1;
    io.DeltaTime = bd->Time > 0 ? (float)((double)(current_time - bd->Time) / frequency) : (float)(1.0f / 60.0f);
    bd->Time = current_time;
#else
    io.DeltaTime = cGameApp::m_sTimeAndFPS.fElpaseTime;//; bd->Time > 0 ? (float)((double)(current_time - bd->Time) / frequency) : (float)(1.0f / 60.0f);
#endif

    if (bd->MouseLastLeaveFrame && bd->MouseLastLeaveFrame >= ImGui::GetFrameCount() && bd->MouseButtonsDown == 0)
    {
        bd->MouseWindowID = 0;
        bd->MouseLastLeaveFrame = 0;
        io.AddMousePosEvent(-FLT_MAX, -FLT_MAX);
    }

    ImGui_ImplSDL2_UpdateMouseData();
    ImGui_ImplSDL2_UpdateMouseCursor();

    // Update game controllers (if enabled and available)
    //ImGui_ImplSDL2_UpdateGamepads();
}

#endif