/*
**
**	Clipboard.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../resource/XImage.h"
#include "../platform/Window.h"


/**
 !	Clipboard inner kit
 */
namespace fce::inner::clipboard
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

	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			CLEAR,
			UPDATE,

			MAXIMUM
		};

	/*
	 !	Message
	 */
	class FC_API Message final
		{
			/*	Friends
			*/
			friend Clipboard;

		public:
			/*	EVENT
			*/
			EVENT e;

		protected:
			/*	Constructor & Destructor
			*/
			Message() = default;

			~Message() = default;
		};

	/*	Delegate group wrapper
	*/
	using DelegateHandler = InvokerGroupWrapper
							<
								UInt8( EVENT::MAXIMUM ),
								EVENT, const Message&
							>;
}


/**
 !	Clipboard concept
 */
namespace fce
{
	/*	Clipboard object
	 */
	template< typename T >

	concept Clipboard_object = Any_of< std::remove_cvref_t< T >,
									   String, Image, Buffer >;
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Clipboard.h"
#elif defined  FC_ANDROID
	#include "android/Android_Clipboard.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Clipboard.h"
#elif defined  FC_IOS
	#include "ios/Ios_Clipboard.h"
#elif defined  FC_MAC
	#include "mac/Mac_Clipboard.h"
#endif


namespace fce
{
	/*
	 !	Clipboard
	 */
	class FC_API Clipboard final
		:
		public inner::clipboard
					::DelegateHandler
		{
			/*	Friends
			*/
			friend Engine;

		public:
			/*	Using inner kit
			*/
			using Impl = inner::clipboard::Impl;

			using TYPE = inner::clipboard::TYPE;

			using EVENT = inner::clipboard::EVENT;

			using Message = inner::clipboard::Message;

			/*	Implementation
			*/
			friend Impl;

			Impl impl;

		public:
			/*	Clear
			*/
			Bool Clear() noexcept;

			/*	Get & Set object
			*/
			Bool Get( String& ) const noexcept;

			Bool Get( Image& ) const noexcept;

			Bool Get( Buffer& ) const noexcept;

			Bool Set( const String& ) noexcept;

			Bool Set( const Image& ) noexcept;

			Bool Set( const Buffer& ) noexcept;

			/*	Get & Set ( Alias )
			*/
			Bool GetText( String& ) const noexcept;

			Bool GetImage( Image& ) const noexcept;

			Bool GetData( Buffer& ) const noexcept;

			Bool SetText( const String& ) noexcept;

			Bool SetImage( const Image& ) noexcept;

			Bool SetData( const Buffer& ) noexcept;

			/*	Get directly ( Syntax sugar )
			*/
			template< Clipboard_object T >

			[[nodiscard]] T Get() const noexcept;

			[[nodiscard]] String GetText() const noexcept;

			[[nodiscard]] Image GetImage() const noexcept;

			[[nodiscard]] Buffer GetData() const noexcept;

			/*	Has ( Syntax sugar )
			*/
			template< Clipboard_object T >

			[[nodiscard]] Bool Has() const noexcept;

			[[nodiscard]] Bool hasText() const noexcept;

			[[nodiscard]] Bool hasImage() const noexcept;

			[[nodiscard]] Bool hasData() const noexcept;

			/*	Has object
			*/
			[[nodiscard]] Bool Has( const TYPE ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			Clipboard() = default;

			~Clipboard() = default;

			/*	Dispatch
			*/
			Void Dispatch() noexcept;

			/*	Get type
			*/
			template< Clipboard_object T >

			static consteval TYPE GetType() noexcept;

			/*	Convert type
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

		private:
			/*	Message
			*/
			Message	message;
		};


	/*
	 @	Clipboard singleton shortcut
	 */
	extern FC_API Clipboard& s_Clipboard;
}



/**
 !	Get type
 */
namespace fce
{
	/*
	 @	Get object type
	 */
	template< Clipboard_object T >

	FC_INLINE consteval Clipboard::TYPE Clipboard::GetType() noexcept
		{
			if constexpr( std::same_as< T, String > )
				{
					return TYPE::TEXT;
				}
			else if constexpr( std::same_as< T, Image > )
				{
					return TYPE::IMAGE;
				}
			else if constexpr( std::same_as< T, Buffer > )
				{
					return TYPE::DATA;
				}
			else
				{
					static_assert( "Missing enum TYPE conversion for clipboard object" );
				}
		}
}



/**
 !	Get & Set ( Alias )
 */
namespace fce
{
	/*
	 @	Get object ( Alias )
	 */
	FC_INLINE Bool Clipboard::GetText( String& str ) const noexcept
		{
			return this->Get( str );
		}

	FC_INLINE Bool Clipboard::GetImage( Image& img ) const noexcept
		{
			return this->Get( img );
		}

	FC_INLINE Bool Clipboard::GetData( Buffer& buf ) const noexcept
		{
			return this->Get( buf );
		}

	/*
	 @	Set object ( Alias )
	 */
	FC_INLINE Bool Clipboard::SetText( const String& str ) noexcept
		{
			return this->Set( str );
		}

	FC_INLINE Bool Clipboard::SetImage( const Image& img ) noexcept
		{
			return this->Set( img );
		}

	FC_INLINE Bool Clipboard::SetData( const Buffer& buf ) noexcept
		{
			return this->Set( buf );
		}
}



/**
 !	Get directly ( Syntax sugar )
 */
namespace fce
{
	/*
	 @	Get object directly
	 */
	template< Clipboard_object T >

	FC_INLINE T Clipboard::Get() const noexcept
		{
			T obj;

			this->Get( obj );

			return obj;
		}

	/*
	 @	Get object directly ( Alias )
	 */
	FC_INLINE String Clipboard::GetText() const noexcept
		{
			return this->Get< String >();
		}

	FC_INLINE Image Clipboard::GetImage() const noexcept
		{
			return this->Get< Image >();
		}

	FC_INLINE Buffer Clipboard::GetData() const noexcept
		{
			return this->Get< Buffer >();
		}
}



/**
 !	Has object
 */
namespace fce
{
	/*
	 @	Has object ( Alias )
	 */
	FC_INLINE Bool Clipboard::hasText() const noexcept
		{
			return this->Has( TYPE::TEXT );
		}

	FC_INLINE Bool Clipboard::hasImage() const noexcept
		{
			return this->Has( TYPE::IMAGE );
		}

	FC_INLINE Bool Clipboard::hasData() const noexcept
		{
			return this->Has( TYPE::DATA );
		}

	/*
	 @	Has object
	 */
	template< Clipboard_object T >

	FC_INLINE Bool Clipboard::Has() const noexcept
		{
			return this->Has( GetType< T >() );
		}

	FC_INLINE Bool Clipboard::Has( const TYPE type ) const noexcept
		{
			return this->impl.Has( Convert( type ) );
		}
}