/*
**
**	Clipboard
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Clipboard.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Clipboard.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Clipboard.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Clipboard.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Clipboard.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Clipboard.cxx"
#endif


namespace fce
{
	/*
	 @	清空 剪贴板
	 */
	Bool Clipboard::Clear() noexcept
		{
			return this->impl.Clear();
		}
}



/**
 !	获取和设置 剪贴板对象
 */
namespace fce
{
	/*
	 @	Get 剪贴板 对象
	 */
	Bool Clipboard::Get( String& str ) const noexcept
		{
			return this->impl.Get( str );
		}

	Bool Clipboard::Get( Image& img ) const noexcept
		{
			return this->impl.Get( img );
		}

	Bool Clipboard::Get( Buffer& buf ) const noexcept
		{
			return this->impl.Get( buf );
		}

	/*
	 @	Set 剪贴板 对象
	 */
	Bool Clipboard::Set( const String& str ) noexcept
		{
			return this->impl.Set( str );
		}

	Bool Clipboard::Set( const Image& img ) noexcept
		{
			return this->impl.Set( img );
		}

	Bool Clipboard::Set( const Buffer& buf ) noexcept
		{
			return this->impl.Set( buf );
		}

	/*
	 @	 分发 剪贴板消息

			impl ( this )
	 */
	FC_INLINE Void Clipboard::Dispatch() noexcept
		{
			// 发布消息
			this->NotifyHandlers( this->message.e,
								  this->message );
		}
}