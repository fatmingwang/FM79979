#ifndef ALL_GAME_PLAY_UT_INCLUDE_H
#define ALL_GAME_PLAY_UT_INCLUDE_H

#include "Language/LanguageFile.h"
#include "BinaryFile.h"
#include "PhaseControl.h"
#include "PhasePopupMessager.h"
//#include "Event\EventInstance.h"
#include "PathChaser.h"
#include "GameApp.h"
#include "CurveChanger.h"
#include "Mouse_Touch.h"
#include "MultiSelectOptionByMPDI.h"
#include "AtgInput.h"
#include "3DScroller.h"
#include "Scroller.h"
#include "ImageWithSlider.h"
#include "MultiPage.h"
#include "Sliderbar.h"
#include "PathFinder.h"
#include "FontFrame.h"
#include "RestrictValue.h"
#include "MultiTouchPoints.h"
#include "FMAnimationRuleEffectManager.h"
#include "ResourceStamp.h"
#include "AssembleImageButton.h"
#include "RenderObjectVector.h"
#include "ObjectXMLParser.h"
#include "CameraZoomFunction.h"
#include "ClickBehavior/ClickBehavior.h"
#include "EventSender/MessageSender.h"
#ifndef _M_CEE//manage code dont support thread
#include "WriteFile\WriteFileWithThread.h"
#endif
#ifdef WASM
#include "Preload/PreloadResource.h"
#endif
#endif