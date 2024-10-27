#include "stdafx.h"
#include "MyImGui.h"
#include "../../imgui/imgui.h"
#include "../../imgui/ImGuiRender.h"
ImFont* g_pFont_title = nullptr;
ImFont* g_pFont_title2 = nullptr;

void cMyImGuiMenu::Init()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;   // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;    // Enable Gamepad Controls
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	AddImGuiFonts({ "Chinese.ttf" }, { 23 });
	//g_pFont_title = GetFontByFileName("NotoSansTC-VariableFont_wght.ttf");
	g_pFont_title2 = GetFontByFileName("Chinese.ttf");
}

void cMyImGuiMenu::Update(float e_fElpaseTime)
{
	ImGui::SetNextWindowSize({ static_cast<float>(1280 - 16) + 1000, (float)250 });
	ImGui::SetNextWindowPos({ 0, 0 });
	ImGui::Begin("BUILDER", nullptr, ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_MenuBar);
	if (ImGui::BeginMenuBar())
	{

		if (ImGui::BeginMenu("Project"))
		{
			if (ImGui::MenuItem("Save"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("SaveProjectFileDlgKey", "Save File", ".builder", RegeditGetPath("ImGuiBuilderPath"), "project");
			}

			if (ImGui::MenuItem("Open"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("OpenProjectFileDlgKey", "Open File", ".builder", RegeditGetPath("ImGuiBuilderPath"), "project");
			}

			if (ImGui::MenuItem("Generate Code"))
			{
				//ImGuiFileDialog::Instance()->OpenDialog("GenCodeProjectFileDlgKey", "Open File", ".cpp,.h,.hpp", RegeditGetPath("ImGuiBuilderPath"), "imgui_builder");
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Editor"))
		{
			if (ImGui::MenuItem("Color"))
			{
				//m_color_menu = !m_color_menu;
			}

			if (ImGui::MenuItem("Style"))
			{
				//m_style_menu = !m_style_menu;
			}				

			if (ImGui::MenuItem("Font"))
			{
				//m_font_menu = !m_font_menu;
			}

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();
}

// Variables to control the clipping region
ImVec2 g_clipMin(0, 0);
ImVec2 g_clipMax(500, 500);

void MyCustomClippingWindow()
{
	//ImVec2 l_vStartPoint(cGameApp::m_spOpenGLRender->m_vViewPortSize.x, cGameApp::m_spOpenGLRender->m_vViewPortSize.y);
	ImVec2 l_vStartPoint(100, 100);
	//ImVec2 l_clipMin(g_clipMin.x, g_clipMin.y);
	//ImVec2 l_clipMax(g_clipMax.x, g_clipMax.y);
	//ImVec2 windowPos(1920/2, 1080/2); // Set desired X and Y position
	//ImGui::SetNextWindowPos(windowPos);
	ImGui::Begin("Clipping Example");

	// Make sure l_clipMax is always greater than l_clipMin
	if (g_clipMax.x < g_clipMin.x) g_clipMax.x = g_clipMin.x + 1;
	if (g_clipMax.y < g_clipMin.y) g_clipMax.y = g_clipMin.y + 1;

	// Set up the clipping rectangle based on the current values of l_clipMin and l_clipMax
	auto l_vStartPos = ImVec2(g_clipMin.x + l_vStartPoint.x, g_clipMin.y + l_vStartPoint.y);
	auto l_vEndPos = ImVec2(g_clipMax.x + l_vStartPoint.x, g_clipMax.y + l_vStartPoint.y);
	ImGui::GetWindowDrawList()->PushClipRect(l_vStartPos, l_vEndPos, true); // true to intersect with current clip rect

	ImGui::DragFloat2("Clip Min", (float*)&g_clipMin, 1.0f, 0.0f, 1500.0f, "%.0f");
	ImGui::DragFloat2("Clip Max", (float*)&g_clipMax, 1.0f, 0.0f, 1500.0f, "%.0f");



	// Draw some content that will be clipped
	ImGui::Text("This text may be clipped based on the clip region.");
	ImGui::Text("Only part of this text will be visible.");

	// Draw some shapes for visualization
	ImDrawList* drawList = ImGui::GetWindowDrawList();
	drawList->AddRect(l_vStartPoint, l_vStartPos, IM_COL32(255, 0, 0, 255)); // Red rectangle outside clipping
	drawList->AddRect(l_vStartPoint, l_vEndPos, IM_COL32(0, 255, 0, 255)); // Green rectangle showing the clip region

	//Restore the clipping
	ImGui::GetWindowDrawList()->PopClipRect();

	ImGui::End();
}

void	ShowMyGUIDebugInfo()
{
	int	l_iOpenGLViewportX = cGameApp::m_spOpenGLRender->m_vViewPortSize.Width();
	int	l_iOpenGLViewportY = cGameApp::m_spOpenGLRender->m_vViewPortSize.Height();
	int fb_width = (int)(ImGui::GetDrawData()->DisplaySize.x * ImGui::GetDrawData()->FramebufferScale.x);
	int fb_height = (int)(ImGui::GetDrawData()->DisplaySize.y * ImGui::GetDrawData()->FramebufferScale.y);
#ifdef WASM
	int l_iBrowserWidth = EMSDK::EMSDK_GetBrowserWidth();
	int l_iBrowserHeight = EMSDK::EMSDK_GetBrowserHeight();
	int	l_iViewportWidth = EMSDK::EMSDK_GetViewportWidth();
	int	l_iViewportHeight = EMSDK::EMSDK_GetViewportHeight();
	int	l_iCanvasPosX = EMSDK::EMSDK_GetCanvasPosX();
	int	l_iCanvasPosY = EMSDK::EMSDK_GetCanvasPosY();
	int windowWidth, windowHeight;
	emscripten_get_canvas_element_size("#canvas", &windowWidth, &windowHeight);

	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nBrowserSize:%d,%d\nCavansSize:%d,%d\nCanvasPos:%d,%d\nOpenGLViewport:%d,%d\nEMSCanvans:%d,%d",
		fb_width, fb_height,
		l_iBrowserWidth, l_iBrowserHeight,
		l_iViewportWidth, l_iViewportHeight,
		l_iCanvasPosX, l_iCanvasPosY,
		l_iOpenGLViewportX, l_iOpenGLViewportY,
		windowWidth, windowHeight).c_str());
#endif
	cGameApp::RenderFont(0, 300, UT::ComposeMsgByFormat(L"FBSize:%d,:%d\nOpenGLViewport:%d,%d",
		fb_width, fb_height,
		l_iOpenGLViewportX, l_iOpenGLViewportY).c_str());
}

// Variables for text box
char inputText[256] = "";

void MyImGuiTest()
{
	auto io = ImGui::GetIO();
	bool show_demo_window = false;
	bool show_another_window = false;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
#ifdef WIN32
	ImGui_ImplWin32_NewFrame();
#elif defined(WASM)
	ImGui_ImplSDL2_NewFrame();
#endif
	ImGui::NewFrame();
	// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
	if (show_demo_window)
		ImGui::ShowDemoWindow(&show_demo_window);
	//2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
	{
		static float f = 0.0f;
		static int counter = 0;

		ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
		cMyImGuiMenu::Update(0.016f);
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
		ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
		ImGui::Checkbox("Another Window", &show_another_window);

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
		ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::PushFont(g_pFont_title2);
		// Create a text box (input field)
		ImGui::InputText("Input Text", inputText, IM_ARRAYSIZE(inputText));

		// Display the current text
		ImGui::Text("You typed: %s", inputText);
		ImGui::PopFont();

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
		ImGui::End();
	}
	//MyCustomClippingWindow();// 
	// Rendering
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	bool l_bShowMyGuiDebugInfo = false;
	if (l_bShowMyGuiDebugInfo)
	{
		ShowMyGUIDebugInfo();
	}
}
