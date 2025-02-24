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
	 @	����
	 */
	Message::Message( Keyboard& keyboard ) noexcept
		:
			e( EVENT::MAXIMUM ),
			key( KEY::ENTER ),
			keyboard( keyboard )
		{
		}


	/*
	 @	 �ַ� ��Ϣ
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->keyboard.Dispatch( *this );
		}
}



namespace fce
{
	/*
	 @	����
	 */
	Keyboard::Keyboard( Window& window ) noexcept
		:
			window( window ),
			impl( *this )
		{
		}


	/*
	 @	�ַ� ��Ϣ
	 */
	FC_INLINE Bool Keyboard::Dispatch( const Message& msg ) noexcept
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