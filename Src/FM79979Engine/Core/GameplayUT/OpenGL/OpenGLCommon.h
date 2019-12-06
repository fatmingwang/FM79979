#pragma once

#include "../../AllMathInclude.h"
#include "../GameUTDefine.h"

//inline Vector3	WorldToScreen(float objX,float objY,float objZ,float *e_pfProjectionMatrix = 0,float *e_pfModelViewMatrix = 0)
Vector3				WorldToScreen(float objX, float objY, float objZ, float *e_pfProjectionMatrix, float *e_pfModelViewMatrix);
//the view port here original start always be X=0,Y=0,however we have changed it,
//it should be look like this.
//int	l_iViewPort[4] = {0,0,ResolutionWidth,ResolutionHeight};
Vector3				WorldToScreen(float objX, float objY, float objZ, float*e_pmatProjection, float*e_pmatMV, int *e_piViewPort);
//e_matVP  =	ProjectionTransform*ViewTransform();
//e_vScreenPos.z is betwnne 0 to 1
Vector3				ScreenToWorld(Vector3 e_vScreenPos, cMatrix44 e_matVP, Vector2 e_vViewPortSize);

Vector4				ViewRectToOpenGLScissor(Vector4 e_v2DViewRange, Vector4 e_vViewPort, Vector2 e_vGameResolution, eDeviceDirection e_eDeviceDirection);
Vector4				ViewRectToOpenGLScissor(Vector4 e_v2DViewRange);