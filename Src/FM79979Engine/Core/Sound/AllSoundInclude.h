#pragma once
//now forece open this vecause I am lazy
#ifndef USE_SOUND_CAPTURE
//#define	USE_SOUND_CAPTURE
#endif

#include "OpenAlWavFile.h"
#include "OpenAlOgg.h"
#include "SoundManager.h"
#ifndef LINUX
#include "SoundFile.h"
#endif
#ifndef LINUX
#include "SoundCapture.h"
#endif