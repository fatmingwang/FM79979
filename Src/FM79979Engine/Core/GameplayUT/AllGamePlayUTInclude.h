#pragma once
//camera
#include "Camera/SimpleCamera.h"
#include "Camera/MouseCamera.h"
#include "Camera/OrthogonalCamera.h"
#include "Camera/SplitScreenCamera.h"
#include "Camera/CameraAnimation.h"
#include "Camera/CameraZoomFunction.h"
//CommonRegister
#include "CommonRegister/CommonRegisterManager.h"
//DataCurve
#include "DataCurve/DataCurve.h"
#include "DataCurve/LinerTemplateDataProcess.h"
//FMAnimationRuleEffect
//#include "FMAnimationRuleEffect/FMAnimationRuleEffectManager.h"
//#include "FMAnimationRuleEffect/FMAnimationRuleEffectQueuePlayer.h"
//GameApp
#include "GameApp/GameApp.h"
//GamePad
#include "GamePad/AtgInput.h"
//GamePhase
#include "GamePhase/PhaseControl.h"
#include "GamePhase/PhasePopupMessager.h"
//Language
#include "Language/LanguageFile.h"
//MultiTouch
#include "MultiTouch/MultiTouchPoints.h"
//OpenGL
#include "OpenGL/Glh.h"
//PathAndCurve
#include "PathAndCurve/CurveChanger.h"
#include "PathAndCurve/PathChaser.h"
#include "PathAndCurve/PathFinder.h"
//Physic
#include "Physic/2DImageCollisionData.h"
//Preload
#ifdef WASM
#include "Preload/PreloadResource.h"
#endif
//Render
//ResourceMaintaner
#include "ResourceMaintaner/ObjectXMLParser.h"
#include "ResourceMaintaner/ResourceStamp.h"
//TemplateValueChanger
#include "TemplateValueChanger/ListValueChanger.h"
#include "TemplateValueChanger/RestrictValue.h"




//Render
//CommonRender
#include "Render/CommonRender/SimplePrimitive.h"
#include "Render/CommonRender/BaseImage.h"
#include "Render/CommonRender/NumeralImage.h"
#include "Render/CommonRender/RenderNode.h"
//Font
#include "Render/Font/GlyphFontRender.h"
#include "Render/Font/FreetypeGlypth.h"
//MPDI
#include "Render/MPDI/MPDI.h"
#include "Render/MPDI/MPDINode.h"
//OpenGLObject
#include "Render/OpenGLObject/VertexBufferObject.h"
#include "Render/OpenGLObject/UVAnimationImage.h"
#include "Render/OpenGLObject/FrameBuffer.h"
//Parser
#include "Render/Parser/AnimationParser.h"
#include "Render/Parser/ImageParser.h"
//Particle
#include "Render/Particle/AllParticleInclude.h"
//PuzzleImage
#include "Render/PuzzleImage/PuzzleImage.h"
//RenderFrameEffect
#include "Render/RenderFrameEffect/RenderFrameEffect.h"
//Terrain
#include "Render/Terrain/ShockBoobsPainter.h"
#include "Render/Terrain/TerrianVertex.h"
//Texture
#include "Render/Texture/Texture.h"
#include "Render/Texture/TextureManager.h"
//UI
#include "Render/AllGamePlayUTRenderInclude.h"

//VideoPlayer
//#include "Render/VideoPlayer/AVIPlayer.h"
//#include "Render/VideoPlayer/AndroidVideoPlayer.h"