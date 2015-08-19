#include "StdAfx.h"
#include "MeshViewer.h"


void	DrawAxisArrow(cFrameCamera*	e_pFrameCamera)
{
	Vector3 P = Vector3(100,980,0);
	Vector3 D1 = Vector3(100,0,0);
	glLineWidth(5);
	//x	
	float	l_fLength = D1.Length();
	cMatrix44	l_mat = cMatrix44::TranslationMatrix(P.x, P.y, 0.0f)*cMatrix44::ScaleMatrix(Vector3(l_fLength,l_fLength, l_fLength))*cMatrix44::StripPosFromMatrix(e_pFrameCamera->GetWorldView());
	float	l_fLinesX[] = {	0,		0,
							1,		0,
							1,		0,
							0.9f,	-0.05f,
							1,		0,
							0.9f,	0.05f};

	GLRender::RenderLine(&l_fLinesX[0],6,Vector4(1,0,0,1),2,l_mat);
	float	l_fLinesY[] = {	0,		0,
							0,		-1,
							0,		-1,
							-0.05f,	-0.9f,
							0,		-1,
							0.05f,	-0.9f};

	GLRender::RenderLine(&l_fLinesY[0],6,Vector4(0,1,0,1),2,l_mat);
	float	l_fLinesZ[] = {	0,		0,0,
							0,		0,1,
							-0.05f,	0,0.9f,
							0,		0,1,
							0,		0,1,
							0.05f,	0,0.9f};
	GLRender::RenderLine(&l_fLinesZ[0],6,Vector4(0,0,1,1),3,l_mat);
}