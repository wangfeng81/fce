/*
**
**	Charset.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Pool.h"


/**
 !	Charset inner kit
 */
namespace fce::inner::charset
{
	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			UTF_8,
			UTF_16,
			UTF_32,

			MBCS,
			WCS,

			MAXIMUM
		};
}


/**
 !	Charset concept
 */
namespace fce
{
	/*	Mapping WChar to Char16 or Char32
	 */
	using Mapping_wchar = std::conditional
							<
								sizeof( WChar ) == sizeof( Char16 ),
								Char16, Char32
							>
							::type;

	/*	Replace WChar with Char16 or Char32
	 */
	template< typename T >

	using Replace_wchar = std::conditional
							<
								std::same_as< T, WChar >,
								Mapping_wchar, T
							>
							::type;

	/*	Same type of char
	 */
	template< typename T, typename U >

	concept Same_char = std::same_as
						<
							Replace_wchar< T >,
							Replace_wchar< U >
						>;
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS

	#if WINVER >= _WIN32_WINNT_WIN10
		#include "charset/Icu_Charset.h"
	#else
		#include "charset/Std_Charset.h"
	#endif

#elif defined  FC_ANDROID
	#include "charset/Icu_Charset.h"
#elif defined  FC_LINUX
	#include "charset/Std_Charset.h"
#elif defined  FC_IOS
	#include "charset/Std_Charset.h"
#elif defined  FC_MAC
	#include "charset/Std_Charset.h"
#endif


namespace fce
{
	/*
	 !	Charset.

			Association of character types

			char		=> MBCS

			char8_t		=> UTF_8
			char16_t	=> UTF_16
			char32_t	=> UTF_32

			wchar_t		=> UTF_16 ( 2 Bytes ) 
						   UTF_32 ( 4 Bytes ) 
	 */
	class Charset final
		{
			/*	Friends
			*/
			friend String;

			friend Singleton;

		public:
			/*	Using inner kit
			*/
			using Impl = inner::charset::Impl;

			using TYPE = inner::charset::TYPE;

			/*	Implementation
			*/
			friend Impl;

			Impl impl;

		public:
			/*	Get char type
			*/
			template< Character T >

			static consteval TYPE GetType() noexcept;

			/*	Convert between C_string & Cpp_string
			*/
			template< Character Src,
					  Stl_string Dst >

			static Void Convert( const Src* const,
									   Dst& );

			template< Character Src,
					  Stl_string Dst >

			static Void Convert( const Src* const,
								 const UInt srcLen,
									   Dst& );

			template< Bool NULL_TERMINATOR = true,
					  Cpp_string Src,
					  Character Dst >

			static UInt Convert( const Src&,
									   Dst* const,
								 const UInt dstLen );

			/*	Convert Cpp_string
			*/
			template< Cpp_string Src,
					  Stl_string Dst >

			static Void Convert( const Src&,
									   Dst& ) noexcept;

			/*	Convert C_string
			*/
			template< Bool NULL_TERMINATOR = true,
					  Character Src,
					  Character Dst >

			static UInt Convert( const Src* const,
									   Dst* const,
								 const UInt dstLen );

			template< Bool NULL_TERMINATOR = true,
					  Character Src,
					  Character Dst >

			static UInt Convert( const Src* const, const UInt srcLen,
									   Dst* const, const UInt dstLen );

			/*	Convert ( Syntax sugar )
			*/
			template< Character Dst,
					  typename ...Args >

			static stl::basic_string< Dst > Convert( Args&&... );

		protected:
			/*	Constructor & Destructor
			*/
			Charset() noexcept;

			~Charset() noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize() noexcept;

			/*	Convert implementation
			*/
			template< Character Src,
					  Stl_string Dst >

			Void Convert_imp( const Src* const,
							  const UInt srcLen,
									Dst& );

			template< Bool NULL_TERMINATOR,
					  Character Src,
					  Character Dst >

			UInt Convert_imp( const Src* const, const UInt srcLen,
									Dst* const, const UInt dstLen );

		private:
			/*	Offset determined by character size
			*/
			template< Character T >

			static consteval UInt Offset() noexcept;

			/*	Max ratio of conversion size
			*/
			template< Character Src,
					  Character Dst >

			static consteval UInt MaxRatio() noexcept;

			/*	Length of null-terminated C_string
			*/
			template< Character T >

			static constexpr UInt Length( const T* const );

			/*	Copy string
			*/
			template< Bool NULL_TERMINATOR = true,
					  Character T >

			static UInt Copy( const T* const src, const UInt srcLen,
									T* const dst, const UInt dstLen );

			/*	Max possible count of target string
			*/
			template< Character Src,
					  Character Dst >

			static constexpr UInt TargetMaxLength( const UInt srcLen ) noexcept;
		};


	/*
	 @	s_Charset
	 */
	SINGLETON_SHORTCUT( Charset );
}


/**
 !	Inlines
 */
#include "charset/Charset.inl"