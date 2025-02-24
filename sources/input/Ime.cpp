/*
**
**	Ime.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Window.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Ime.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Ime.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Ime.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Ime.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Ime.cxx"
#endif


/**
 !	Ime inner kit
 */
namespace fce::inner::ime
{
	/*
	 @	构造
	 */
	Message::Message( Ime& ime ) noexcept
		:
			e( EVENT::MAXIMUM ),
			character( 0 ),
			ime( ime )
		{
		}


	/*
	 @	 分发 消息
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->ime.Dispatch( *this );
		}
}


namespace fce
{
	/*
	 @	构造
	 */
	Ime::Ime( Window& window ) noexcept
		:
			window( window ),
			impl( *this )
		{
		}


	/*
	 @	 分发消息
	 */
	FC_INLINE Bool Ime::Dispatch( const Message& msg ) noexcept
		{
			// 发布消息，如果返回 true 则屏蔽后续
			if( this->NotifyHandlers( msg.e,
									  msg ) )
				{
					return true;
				}

			// 调用UI

			return false;
		}
}