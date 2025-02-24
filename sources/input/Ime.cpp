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
	 @	����
	 */
	Message::Message( Ime& ime ) noexcept
		:
			e( EVENT::MAXIMUM ),
			character( 0 ),
			ime( ime )
		{
		}


	/*
	 @	 �ַ� ��Ϣ
	 */
	Bool Message::Dispatch() const noexcept
		{
			return this->ime.Dispatch( *this );
		}
}


namespace fce
{
	/*
	 @	����
	 */
	Ime::Ime( Window& window ) noexcept
		:
			window( window ),
			impl( *this )
		{
		}


	/*
	 @	 �ַ���Ϣ
	 */
	FC_INLINE Bool Ime::Dispatch( const Message& msg ) noexcept
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