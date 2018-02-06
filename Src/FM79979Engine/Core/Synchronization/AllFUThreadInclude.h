#pragma once

#include "FUThreadRef.h"
#include "FUCriticalSection.h"
#include "FUSemaphore.h"
#include "FUSynchronized.h"
#include "FUThread.h"
#include "FUThreadPool.h"
//c++11
#ifndef _M_CEE//manage code dont support thread
#include "CPP11Thread.h"
#endif