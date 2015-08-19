#ifndef OPENAL_WAV_FILE_H
#define OPENAL_WAV_FILE_H

#include "BasicSound.h"

namespace FATMING_CORE
{
	ALvoid GetWAVData(const ALbyte *file,ALenum *format,ALvoid **data,ALsizei *size,ALsizei *freq, ALboolean *loop);
	class	cOpanalWAV:public cBasicSound
	{
	public:
		DEFINE_TYPE_INFO();
		//cOpanalWAV(NamedTypedObject*e_pNamedTypedObject):cBasicSound(e_pNamedTypedObject){}
		cOpanalWAV(NamedTypedObject*e_pNamedTypedObject,const char*e_strileName,bool e_bStreaming);
		virtual ~cOpanalWAV();
		virtual	NamedTypedObject*	Clone(){ return 0; }
		virtual bool	OpenFile(const char*e_strileName);
	};
}

#endif