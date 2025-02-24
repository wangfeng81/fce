/*
**
**	Keyboard.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/input/Keyboard.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Keyboard.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Keyboard.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Keyboard.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Keyboard.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Keyboard.cxx"
#endif


/**
 !	Keyboard inner kit
 */
namespace fce::inner::keyboard
{
	/*
	 @	构造
	 */
	Message::Message( Keyboard& keyboard ) noexcept
		:
			e( EVENT::MAXIMUM ),
			key( KEY::ENTER ),
			keyboard( keyboard )
		{
		}


	/*
	 @	 分发 消息
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->keyboard.Dispatch( *this );
		}
}



namespace fce
{
	/*
	 @	构造
	 */
	Keyboard::Keyboard( Window& window ) noexcept
		:
			window( window ),
			impl( *this )
		{
		}


	/*
	 @	分发 消息
	 */
	FC_INLINE Bool Keyboard::Dispatch( const Message& msg ) noexcept
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