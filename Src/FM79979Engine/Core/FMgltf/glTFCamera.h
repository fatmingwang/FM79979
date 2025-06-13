#pragma once

#pragma once
#include <string>
#include <vector>
#include "../AllCoreInclude.h"

class cglTFCamera :public NamedTypedObject
{
public:
	DEFINE_TYPE_INFO();
	virtual ~cglTFCamera();
	enum class eCameraType
	{
		PERSPECTIVE,
		ORTHOGRAPHIC,
		UNKNOWN
	};

	struct sCamera
	{
		std::string name;
		eCameraType type;
		float       yfov;        // Perspective
		float       aspectRatio; // Perspective
		float       znear;
		float       zfar;
		float       xmag;        // Orthographic
		float       ymag;        // Orthographic
		int         nodeIndex = -1;
		cMatrix44   m_StartNodeTransform = cMatrix44::Identity;
		class cglTFNodeData* m_pglTFNodeData = nullptr;
	};

	// List of cameras loaded from glTF
	std::vector<sCamera> m_CameraVector;

	// Load all camera data from glTF model
	void LoadCamerasFromGLTF(const tinygltf::Model& model, std::map<int, class cglTFNodeData*>* e_pNideIndexAndNodeDataMap);
	// Accessors
	size_t GetCameraCount() const;
	const sCamera*	GetCameraByIndex(size_t index) const;
	const sCamera*	GetCameraByName(const std::string& name) const;
	static bool		IsCameraExists(const tinygltf::Model& model);
};



// Controller for managing multiple cameras
//glTF uses a right - handed coordinate system.glTF defines + Y as up, +Z as forward, and -X as right; the front of a glTF asset faces + Z.
class cCameraController : public NamedTypedObject, public cSingltonTemplate<cCameraController>
{
	std::map<std::string,cglTFCamera*>	m_NameAndglTFCameraMap;
	std::vector<std::shared_ptr<cFrameCamera>> m_CameraVector;

	int					m_CurrentCameraIndex = -1;
	int                 m_iLastUsedProgram = -1;
	cCameraController();
	virtual ~cCameraController();
	bool                m_bEnableCotrolCameraByMouse = true;
	cCameraBehaveByMouseBehave*	m_pCameraBehaveByMouseBehave;
	int					m_iDefaultModelCameraIndex = -1;
public:
	DEFINE_TYPE_INFO();
	SINGLETON_BASIC_FUNCTION(cCameraController);

	bool AddglTFCamera(cglTFCamera*e_pglTFCamera);
	bool RemoveglTFCamera(cglTFCamera* e_pglTFCamera);
	// Add a camera to the controller
	bool AddCamera(std::shared_ptr<cFrameCamera> camera);

	// Remove a camera (by pointer)
	void RemoveCamera(std::shared_ptr<cFrameCamera> camera);

	// Remove all cameras
	void ClearCameras();

	// Get number of cameras
	size_t GetCameraCount() const;

	// Get camera by index
	std::shared_ptr<cFrameCamera> GetCamera(size_t idx) const;

	// Get current camera
	std::shared_ptr<cFrameCamera> GetCurrentCamera() const;

	// Switch current camera by index
	bool	SwitchCamera(size_t idx);

	// Switch current camera by pointer
	bool	SwitchCamera(std::shared_ptr<cFrameCamera> camera);
	void	Update(float e_fElpaseTime);
	void    Render(GLuint e_uiProgramID,float*e_pMatrix = nullptr);
	void	RenderGrid();
	void    CreateDefault3DCamera(bool e_bEnableControleByMouse = true);
	auto	GetCurrentCameraIndex() const ->int;
	//auto GetCurrentCameraIndex() const -> int;
};

// Frame-specific camera data
class cCameraFrameData : public cRenderObject
{
	std::shared_ptr<cFrameCamera> m_Camera;
	bool m_bAddIntocCameraController = false;
	//force render for post effect or something?
	bool m_bDoUseThisCamera = false;
	int  m_iOriginalCameraIndex = -1;
public:
	DEFINE_TYPE_INFO();
	cCameraFrameData(cglTFCamera::sCamera e_CameraData);
	virtual ~cCameraFrameData()
	{

	}
	virtual void Update(float e_fElpaseTime)override;
	virtual void Render() override;
	virtual void EndRender() override;
	std::shared_ptr<cFrameCamera> GetCameraData()
	{
		return m_Camera;
	}
};

void g_fSetCameraUniform(GLuint e_uiProgramID, float* e_pMatrix = nullptr);
void g_fCameraDebugRender();
void g_fCameraControllerUpdate(float e_fElpaseTime);