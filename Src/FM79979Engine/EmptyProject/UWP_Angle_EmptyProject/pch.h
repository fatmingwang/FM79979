#pragma once

#include <wrl.h>
#include <wrl/client.h>
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>
#include <collection.h>
#include "App.xaml.h"




#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES
#endif
// OpenGL ES includes
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

// EGL includes
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglplatform.h>

#include <angle_windowsstore.h>

//#define UWP
//#include "../../Core/AllCoreInclude.h"
#include "../../Core/Common/BinaryFile/BinaryFile.h"
#ifndef _M_CEE//manage code dont support thread
#include "../../Core/Common/BinaryFile/WriteFileWithThread.h"
#endif
#include "../../Core/Common/Utility.h"
#include "../../Core/Common/NamedTypedObject.h"
#include "../../Core/Common/TimeUT.h"
#include "../../Core/Common/CommonApp.h"
#include "../../Core/Common/StringToStructure.h"
#include "../../Core/Common/CommonDefine.h"
#include "../../Core/Common/EventSender/MessageSender.h"
#include "../../Core/Common/Template/TemplateClass.h"
#include "../../Core/Common/Template/TemplateClass.h"
//#include "Network/GameNetWork.h"
//#include "Network/Network.h"

#include "../../Core/XML/XMLLoader.h"
#include "../../Core/AllMathInclude.h"
//#ifndef IOS
#include "../../Core/Sound/AllSoundInclude.h"
//#endif
//#ifndef IOS
//#include "AVIPlayer/AVIPlayer.h"
//#endif
#include "../../Core/GameplayUT/AllGamePlayUTInclude.h"