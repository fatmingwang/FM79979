#pragma once
#include "../CommonRender/RenderObject.h"
#include <functional>
namespace FATMING_CORE
{
	enum eRenderCommandType
	{
		eRCT_RENDER_QUAD = 0,
		eRCT_RENDER_TRIANGLE,
		eRCT_RENDER_PRIMITIVE,//line,points,sphere,panel,cube....
		eRCT_VIEWPORT_SETTING,//also scissor
		eRCT_FBO_OBJECT,
		eRCT_VBO_OBJECT,
		eRCT_VIEW_PROJECTION_SETTING,//camea and shader
		eRCT_CUSTOM_RENDERING,//use callback function(f_CustomRenderfunction)
		eRCT_MAX
	};
	typedef std::function<void(void*)>	f_CustomRenderfunction;
	//cBaseImage
	//cUIIMage
	//cPuzzleImage
	//cMPDI
	//cSubMPDI
	//FrameBuffer
	//Viewport
	//Shader
	//Mesh
	//AnimationMesh
	//cNumeralImage
	//RenderLine
	//DrawPrimitive
	class cRenderCommand :public NamedTypedObject
	{
	public:
		cRenderCommand();
		~cRenderCommand();
		virtual void	SetUpdate(void*e_pData) = 0;
		virtual void	Render(void*e_pData) const  = 0;
	};

	class cRenderCommandForPrimitive :public NamedTypedObject
	{
		float*		m_pfVertices;
		float*		m_pfUV;
		float*		m_pfColor;
		void		CreateQuadData();
		void		CreateTrianglesData(int e_iNumTriangle);
		bool		m_bBlending;
	public:
	};

	//class cRenderCommandForPrimitive :public NamedTypedObject
	//{

	//};
}