#pragma once
#include <string>
#include <memory>

// Forward declarations
class cTexture;

/**
 * IBL (Image-Based Lighting) Manager
 * 
 * IMPORTANT: glTF 2.0 does NOT support cubemap textures!
 * All glTF textures are 2D textures only.
 * 
 * IBL textures must be:
 * 1. Generated at runtime from an HDRI image
 * 2. Loaded separately from standalone cubemap files  
 * 3. Pre-computed offline and stored as separate assets
 * 
 * This manager handles the environment maps needed for PBR IBL:
 * - Environment Cubemap: The skybox/environment for reflections
 * - Diffuse Irradiance Map: Pre-convolved cubemap for diffuse lighting
 * - Pre-filtered Specular Map: Mip-mapped cubemap for specular reflections
 * - BRDF LUT: 2D lookup texture for split-sum approximation
 */
class cIBLManager
{
private:
    // Singleton
    static cIBLManager* s_pInstance;
    
    // OpenGL texture IDs
    GLuint m_uiEnvironmentCubemapID = 0;       // Original environment cubemap (skybox)
    GLuint m_uiDiffuseIrradianceMapID = 0;     // Pre-convolved diffuse irradiance
    GLuint m_uiPrefilteredEnvMapID = 0;        // Pre-filtered specular map with mipmaps
    GLuint m_uiBrdfLutID = 0;                  // BRDF Look-Up Table (2D texture)
    
    // Settings
    int m_iEnvironmentMapSize = 512;           // Size of environment cubemap faces
    int m_iIrradianceMapSize = 32;             // Size of irradiance cubemap faces
    int m_iPrefilteredMapSize = 128;           // Size of prefiltered cubemap faces
    int m_iBrdfLutSize = 512;                  // Size of BRDF LUT
    float m_fMaxReflectionLOD = 4.0f;          // Max mip level for prefiltered map
    
    bool m_bIsInitialized = false;
    bool m_bIBLEnabled = false;
    
    // Internal shader programs for IBL generation
    GLuint m_uiEquirectToCubemapShader = 0;
    GLuint m_uiIrradianceShader = 0;
    GLuint m_uiPrefilterShader = 0;
    GLuint m_uiBrdfShader = 0;
    
    // Helper geometry
    GLuint m_uiCubeVAO = 0;
    GLuint m_uiCubeVBO = 0;
    GLuint m_uiQuadVAO = 0;
    GLuint m_uiQuadVBO = 0;
    
    // Private constructor for singleton
    cIBLManager();
    
    // Internal methods
    void SetupCube();
    void SetupQuad();
    void RenderCube();
    void RenderQuad();
    GLuint CreateShaderProgram(const char* vertSrc, const char* fragSrc);
    void CreateIBLShaders();
    
public:
    ~cIBLManager();
    
    // Singleton access
    static cIBLManager* GetInstance();
    static void DestroyInstance();
    
    /**
     * Initialize IBL system
     * Must be called after OpenGL context is created
     */
    void Initialize();
    
    /**
     * Load environment map from an equirectangular HDRI image
     * This will generate all IBL maps (irradiance, prefiltered, BRDF LUT)
     * @param e_strHDRIPath Path to HDR image file
     * @return true if successful
     */
    bool LoadFromHDRI(const char* e_strHDRIPath);
    
    /**
     * Load pre-computed IBL maps from separate cubemap files
     * @param e_strEnvCubemapPath Path to environment cubemap (6 faces or DDS)
     * @param e_strIrradianceMapPath Path to irradiance cubemap
     * @param e_strPrefilteredMapPath Path to prefiltered cubemap
     * @param e_strBrdfLutPath Path to BRDF LUT texture
     * @return true if successful
     */
    bool LoadFromCubemapFiles(const char* e_strEnvCubemapPath,
                               const char* e_strIrradianceMapPath,
                               const char* e_strPrefilteredMapPath,
                               const char* e_strBrdfLutPath);
    
    /**
     * Load environment cubemap from 6 separate face images
     * Faces should be named: posx, negx, posy, negy, posz, negz
     * @param e_strFolderPath Folder containing face images
     * @param e_strExtension File extension (e.g., ".png", ".jpg")
     * @return true if successful
     */
    bool LoadCubemapFromFaces(const char* e_strFolderPath, const char* e_strExtension);
    
    /**
     * Generate IBL maps from currently loaded environment cubemap
     * Call this after loading environment cubemap if not using LoadFromHDRI
     */
    void GenerateIBLMaps();
    
    /**
     * Generate BRDF LUT (only needs to be done once, can be saved to file)
     */
    void GenerateBRDFLut();
    
    /**
     * Bind IBL textures for rendering
     * @param e_uiShaderProgram Shader program ID
     * @param e_uiStartTextureUnit Starting texture unit for IBL textures
     * @return Next available texture unit
     */
    GLuint BindIBLTextures(GLuint e_uiShaderProgram, GLuint e_uiStartTextureUnit);
    
    /**
     * Render skybox using environment cubemap
     * @param e_uiShaderProgram Skybox shader program
     * @param e_pViewMatrix View matrix (3x3 rotation only, no translation)
     * @param e_pProjectionMatrix Projection matrix
     */
    void RenderSkybox(GLuint e_uiShaderProgram, const float* e_pViewMatrix, const float* e_pProjectionMatrix);
    
    // Getters
    bool IsInitialized() const { return m_bIsInitialized; }
    bool IsIBLEnabled() const { return m_bIBLEnabled; }
    GLuint GetEnvironmentCubemapID() const { return m_uiEnvironmentCubemapID; }
    GLuint GetDiffuseIrradianceMapID() const { return m_uiDiffuseIrradianceMapID; }
    GLuint GetPrefilteredEnvMapID() const { return m_uiPrefilteredEnvMapID; }
    GLuint GetBrdfLutID() const { return m_uiBrdfLutID; }
    float GetMaxReflectionLOD() const { return m_fMaxReflectionLOD; }
    
    // Setters
    void SetIBLEnabled(bool e_bEnabled) { m_bIBLEnabled = e_bEnabled; }
    void SetMaxReflectionLOD(float e_fLOD) { m_fMaxReflectionLOD = e_fLOD; }
    
    // Debug/ImGui
    void RenderImGUI();
    
    // Cleanup
    void Cleanup();
};

// Convenience global pointer
extern cIBLManager* g_pIBLManager;
