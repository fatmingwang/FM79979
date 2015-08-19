// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#ifdef WIN32
#include <windows.h>
#include <tchar.h>
#endif
// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently



#include "../../Core/AllCoreInclude.h"
#include "../../Core/Particle/AllParticleInclude.h"
#include "../../Core/FatmingCollada/AllColladaInclude.h"
#include "../../Core/FatmingAI/AllFatmingAIInclude.h"
#include "../../Core/AllPhysicInclude.h"


cbtShapeCollision*GetbtShapeCollision(TiXmlElement*e_pElement);