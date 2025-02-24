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
	 @	����
	 */
	Message::Message( Mouse& mouse ) noexcept
		:
			e( EVENT::MAXIMUM ),
			wheelDelta( 0 ),
			mouse( mouse )
		{
		}


	/*
	 @	 �ַ� ��Ϣ
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->mouse.Dispatch( *this );
		}
}


namespace fce
{
	/*
	 @	����
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
	 @	�ַ���Ϣ
	 */
	FC_INLINE Bool Mouse::Dispatch( const Message& msg ) noexcept
		{
			// ������Ϣ��������� true �����κ���
			if( this->NotifyHandlers( msg.e,
									  msg ) )
				{
					return true;
				}

			// ����UI

			return false;
		}
}