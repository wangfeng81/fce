/*
**
**	Cursor
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Cursor.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Cursor.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Cursor.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Cursor.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Cursor.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Cursor.cxx"
#endif


namespace fce
{
	/*
	 @	Çå¿Õ ¼ôÌù°å
	 */
	Bool Cursor::Clear()
		{
			return this->impl.Clear();
		}
}