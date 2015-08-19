#include "../AllCoreInclude.h"
extern std::vector<std::string>	*g_strParsingDataList;

#if defined(WIN32)
#elif defined(ANDROID)
 //#include <cstdarg>
	#ifndef OPENGLES_2_X
	#define OPENGLES_1_X
	#endif
#elif defined(IOS)

//#ifndef IOS
//#define	IOS
//#endif
	#ifndef OPENGLES_2_X
	#define OPENGLES_1_X
	#endif

#endif