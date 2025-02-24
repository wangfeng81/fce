/*
**
**	Cursor.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../resource/XImage.h"
#include "../platform/Window.h"


/**
 !	Cursor inner kit
 */
namespace fce::inner::cursor
{
	/*	Types
	 */
	enum struct TYPE
		:
			Byte
		{
			TEXT,
			IMAGE,

			DATA,

			MAXIMUM
		};
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Cursor.h"
#elif defined  FC_ANDROID
	#include "android/Android_Cursor.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Cursor.h"
#elif defined  FC_IOS
	#include "ios/Ios_Cursor.h"
#elif defined  FC_MAC
	#include "mac/Mac_Cursor.h"
#endif


namespace fce
{
	/*
	 !	Cursor
	 */
	class Cursor final
		:
		public Component
		{
			/*	Friends
			*/
			friend inner::cursor::Impl;

			REFLECTION( Cursor );

		public:
			/*	Using inner kit
			*/
			using Impl = inner::cursor::Impl;

			using TYPE = inner::cursor::TYPE;

			/*	Implementation
			*/
			Impl impl;

		public:
			/*	Set
			*/
			Bool Set( TYPE );

		protected:
			/*	Constructor & Destructor
			*/
			Cursor() = default;

			~Cursor() = default;

			/*	Convert type
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

		private:
			/*	Message
			*/
			Message	message;
		};
}