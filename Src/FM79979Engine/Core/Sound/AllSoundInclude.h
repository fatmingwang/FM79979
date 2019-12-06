//now forece open this vecause I am lazy
#ifndef USE_SOUND_CAPTURE
#define	USE_SOUND_CAPTURE
#endif

#include "OpenAlWavFile.h"
#include "OpenAlOgg.h"
#include "../XML/XMLLoader.h"
#include "SoundManager.h"
#ifndef LINUX
#include "SoundFile.h"
#endif
#if defined(USE_SOUND_CAPTURE)
#include "SoundCapture.h"
#endif