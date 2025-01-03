#include "LazyGUIParser.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include "../ImGuiRender.h"
#include "../../Core/AllCoreInclude.h"
#include "../../../include/json.hpp"
using json = nlohmann::json;

cLazyGUIParser::cLazyGUIParser()
{
	m_pMainUIRoot = nullptr;
	ImGui_ImplOpenGL3_Init(cGameApp::m_spOpenGLRender->m_Handle, nullptr);
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	ImGui::GetIO().IniFilename = nullptr;
	ImGui::GetIO().LogFilename = nullptr;
	ImGui::GetIO().FontGlobalScale = 1.5f;
}

cLazyGUIParser::~cLazyGUIParser()
{
	SAFE_DELETE(m_pMainUIRoot);
	ImGui_ImplOpenGL3_Shutdown();
}

bool cLazyGUIParser::ParseUIFile(const char* e_strFileName)
{
	json l_JsonData;
	if (GetJsonFromFile(e_strFileName, l_JsonData))
	{
		auto l_strKEy = l_JsonData.begin().key();
		SAFE_DELETE(this->m_pMainUIRoot);
		this->m_pMainUIRoot = (cMyGuiRootNode*)cImGuiNode::DoUnSerialize(l_JsonData);
		auto l_ChildVector = this->m_pMainUIRoot->GetChildNodeVector();
		return true;
	}
	return false;
}


void cLazyGUIParser::Init()
{

}

void cLazyGUIParser::Update(float e_fElpaseTime)
{

}

void cLazyGUIParser::Render(float* e_pfMatrix, float* e_pfGameResolutoinSize)
{
	float l_fTargetGameResolution[2] = { 1920.f, 1080.f };
	ImGui_StartFrame(e_pfGameResolutoinSize);
	if (this->m_pMainUIRoot)
	{
		m_pMainUIRoot->Render();
	}
	ImGui_EndFrame(e_pfMatrix, e_pfGameResolutoinSize);
	//GLRender::RenderRectangle(l_fTargetGameResolution[0], l_fTargetGameResolution[1], cMatrix44::Identity, Vector4::Red);
	GLRender::RenderRectangle(Vector2(0,0),l_fTargetGameResolution[0], l_fTargetGameResolution[1],Vector4::Red);
}
//
//// Draw a 9-sliced texture
//void DrawNineSlicedTexture(ImTextureID texture, ImVec2 pos, ImVec2 size, ImVec4 borders, ImVec2 uv_min, ImVec2 uv_max)
//{
//	ImDrawList* draw_list = ImGui::GetWindowDrawList();
//
//	// Texture size in pixels
//	float tex_width = size.x;
//	float tex_height = size.y;
//
//	// Border sizes
//	float left = borders.x;
//	float top = borders.y;
//	float right = borders.z;
//	float bottom = borders.w;
//
//	// Define UVs for each region
//	ImVec2 uv_tl = ImVec2(uv_min.x, uv_min.y);                       // Top-left
//	ImVec2 uv_tr = ImVec2(uv_max.x, uv_min.y);                       // Top-right
//	ImVec2 uv_bl = ImVec2(uv_min.x, uv_max.y);                       // Bottom-left
//	ImVec2 uv_br = ImVec2(uv_max.x, uv_max.y);                       // Bottom-right
//
//	ImVec2 uv_center_min = ImVec2(uv_min.x + left / tex_width, uv_min.y + top / tex_height);
//	ImVec2 uv_center_max = ImVec2(uv_max.x - right / tex_width, uv_max.y - bottom / tex_height);
//
//	// Draw 9 parts
//	// Top-left
//	draw_list->AddImage(texture, pos, ImVec2(pos.x + left, pos.y + top), uv_tl, uv_center_min);
//
//	// Top-center
//	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y), ImVec2(pos.x + size.x - right, pos.y + top),
//						ImVec2(uv_center_min.x, uv_min.y), ImVec2(uv_center_max.x, uv_center_min.y));
//
//	// Top-right
//	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y), ImVec2(pos.x + size.x, pos.y + top),
//						ImVec2(uv_center_max.x, uv_min.y), uv_tr);
//
//	// Middle-left
//	draw_list->AddImage(texture, ImVec2(pos.x, pos.y + top), ImVec2(pos.x + left, pos.y + size.y - bottom),
//						ImVec2(uv_min.x, uv_center_min.y), ImVec2(uv_center_min.x, uv_center_max.y));
//
//	// Center
//	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y + top), ImVec2(pos.x + size.x - right, pos.y + size.y - bottom),
//						uv_center_min, uv_center_max);
//
//	// Middle-right
//	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y + top), ImVec2(pos.x + size.x, pos.y + size.y - bottom),
//						ImVec2(uv_center_max.x, uv_center_min.y), ImVec2(uv_max.x, uv_center_max.y));
//
//	// Bottom-left
//	draw_list->AddImage(texture, ImVec2(pos.x, pos.y + size.y - bottom), ImVec2(pos.x + left, pos.y + size.y),
//						uv_bl, ImVec2(uv_center_min.x, uv_max.y));
//
//	// Bottom-center
//	draw_list->AddImage(texture, ImVec2(pos.x + left, pos.y + size.y - bottom), ImVec2(pos.x + size.x - right, pos.y + size.y),
//						ImVec2(uv_center_min.x, uv_center_max.y), ImVec2(uv_center_max.x, uv_max.y));
//
//	// Bottom-right
//	draw_list->AddImage(texture, ImVec2(pos.x + size.x - right, pos.y + size.y - bottom), ImVec2(pos.x + size.x, pos.y + size.y),
//						ImVec2(uv_center_max.x, uv_center_max.y), uv_br);
//}