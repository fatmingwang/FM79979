#pragma once
#include "Shader.h"
//https://developer.nvidia.com/sites/default/files/akamai/gamedev/docs/OpenGL%204.x%20and%20Beyond.pdf
// page20.
//
//class	cBaseShader :public NamedTypedObject
//{
//public:
//	std::string m_strShaderLog;
//protected:
//	//to check camera or light updated or something else...not necessary
//	bool	m_bDataUpdated;
//	GLuint	m_uiVS;
//	GLuint	m_uiFS;
//	GLuint	m_uiProgram;
//	bool	CreateVS(const char* e_strVS);
//	bool	CreateFS(const char* e_strPS);
//	bool	CreateProgram(const char* e_strVS, const char* e_strPS, bool e_bTexture);
//	bool	m_bTexture;
//public:
//	GLuint	m_uiMatrixVPLoacation;
//	GLuint	m_uiMatrixWLoacation;
//	GLuint	m_uiTexLoacation;
//	GLuint	m_uiAttribArray[TOTAL_FVF];
//	//for non texture,directly setup pixel shader's color
//	GLuint	m_uiColorLoacation;
//	//for skinning
//	GLuint	m_uiBonesLocation;
//	//for Point Size
//	GLuint	m_uiPointSize;
//	//if e_bTexture is true it texture coordinate attribute and texture UV will be assign.
//	cBaseShader(const wchar_t* e_strName, bool e_bTexture = true);
//	cBaseShader(const char* e_strVS, const char* e_strPS, const wchar_t* e_strName, bool e_bTexture = true);
//	//for opengl es1
//	cBaseShader(const wchar_t* e_strName, bool* e_pbClientState);
//	cBaseShader();
//	virtual	NamedTypedObject* Clone() { return 0; }
//	virtual ~cBaseShader();
//	virtual	void				Use(bool e_bUseLastWVPMatrix = true);
//	//do something if shader need to do in each frame update.
//	virtual	void				Update(float e_fElpaseTime) { m_bDataUpdated = false; }
//	void						Disable();
//	GLuint						GetUniFormLocationByName(const char* e_strName);
//	virtual	void				DebugRender();
//};