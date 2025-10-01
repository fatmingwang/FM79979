#pragma once
#include "glTFCamera.h"
#include "glTFLight.h"
#include <memory>
#include <string>
#include "ShadowMap.h"

class cglTFScene : public Frame
{
    cShadowMap  m_ShadowMap;
private:
    std::unique_ptr<cglTFNodeData> m_pRootFrame;
    //std::unique_ptr<cCameraController> m_pCameraController;
    //cLighController* m_pLightController; // Singleton, do not own
    Frame* FindFrameByNameRecursive(Frame* pFrame, const std::wstring& name) const;
public:
    cglTFScene();
    virtual ~cglTFScene();
    Frame* GetRootFrame() const;
    //cCameraController* GetCameraController() const;
    //cLighController* GetLightController() const;
    // Recursive update
    void Update(float e_fElpaseTime);

    // Recursive render
    void Render();

    // Find Frame by name (recursive)
    Frame*  FindFrameByName(const std::wstring& name) const;
	bool    AddFrame(Frame* pFrame);
};