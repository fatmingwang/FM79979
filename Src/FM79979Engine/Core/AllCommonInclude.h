#pragma once

#include "Common/BinaryFile/BinaryFile.h"
#ifndef _M_CEE//manage code dont support thread
#include "Common/BinaryFile/WriteFileWithThread.h"
#endif
#include "Common/Utility.h"
#include "Common/NamedTypedObject.h"
#include "Common/TimeUT.h"
#include "Common/CommonApp.h"
#include "Common/StringToStructure.h"
#include "Common/CommonDefine.h"
#include "Common/EventSender/MessageSender.h"
#include "Common/Template/TemplateClass.h"
#include "Common/Template/TemplateClass.h"
//#include "Network/GameNetWork.h"
//#include "Network/Network.h"
#include "Sound/AllSoundInclude.h"
#include "XML/XMLLoader.h"
#include "AllMathInclude.h"