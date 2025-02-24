/*
**
**	Mouse.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/platform/Window.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Mouse.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Mouse.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Mouse.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Mouse.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Mouse.cxx"
#endif


/**
 !	Mouse inner kit
 */
namespace fce::inner::mouse
{
	/*
	 @	构造
	 */
	Message::Message( Mouse& mouse ) noexcept
		:
			e( EVENT::MAXIMUM ),
			wheelDelta( 0 ),
			mouse( mouse )
		{
		}


	/*
	 @	 分发 消息
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->mouse.Dispatch( *this );
		}
}


namespace fce
{
	/*
	 @	构造
	 */
	Mouse::Mouse( Window& window ) noexcept
		:
			window( window ),
			impl( *this )
		{
		}


	/*
	 @	Get cursor position
	 */
	Point Mouse::GetPosition() const noexcept
		{
			const auto pt
				{
					this->impl.GetPosition()
				};

			return this->window.ScreenToSurface( pt );
		}

	/*
	 @	Set cursor position
	 */
	Void Mouse::SetPosition( const Point& pt ) noexcept
		{
			const Point ptScr
				{
					this->window.SurfaceToScreen( pt )
				};

			this->impl.SetPosition( ptScr.x,
									ptScr.y );
		}

	/*
	 @	分发消息
	 */
	FC_INLINE Bool Mouse::Dispatch( const Message& msg ) noexcept
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