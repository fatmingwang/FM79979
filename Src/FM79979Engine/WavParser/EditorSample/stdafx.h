// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
#pragma once

// TODO: reference additional headers your program requires here
#pragma warning( disable : 4945 )
#ifdef WIN32
#include "windows.h"
#include "../../../include/glew.h"
#endif
#include <stdlib.h>
//#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <map>
#include <vector>
#include <list>
#include <xstring>
#include "assert.h"
#include "../../Core/AllCoreInclude.h"
#include "../../core/AVIPlayer/AVIPlayer.h"
//#include "../../Core/AllPhysicInclude.h"
#include "../kiss_fft130/kiss_fft.h"
//#include "../../kiss_fft.h/kiss_fft.h"

#include "../MusicGame/AllPhaseName.h"
#include "../MusicGame/Parameters.h"
#include "../MusicGame/SoundTimeLineData.h"
#ifdef WIN32
#include "../../DotNetCommon/AllDNCTInclude.h"
using namespace DNCT;
#endif

//#define	PARSE_TEST_SOUND