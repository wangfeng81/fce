/*
**
**	Charset inlines
**
		Copyright (c) WangFeng since 2006...
*/

#ifdef FC_WINDOWS
	#include "Icu_Charset.inl"
#elif defined  FC_ANDROID
	#include "Icu_Charset.inl"
#elif defined  FC_LINUX
	#include "Std_Charset.inl"
#elif defined  FC_IOS
	#include "Std_Charset.inl"
#elif defined  FC_MAC
	#include "Std_Charset.inl"
#endif


namespace fce
{
	/*
	 @	Offset determined by character size
	 */
	template< Character T >
	
	FC_INLINE consteval UInt Charset::Offset() noexcept
		{
			return sizeof( T ) >> 1;
		}
	
	/*
	 @	Length of null-terminated string
	 */
	template< Character T >
	
	FC_INLINE constexpr UInt Charset::Length( const T* const sz )
		{
			FC_ASSERT( sz );
	
			return std::char_traits< T >::length( sz );
		}
	
	/*
	 @	Get type
	 */
	template< Character T >

	FC_INLINE consteval Charset::TYPE Charset::GetType() noexcept
		{
			if constexpr( std::same_as< T, Char8 > )
				{
					return TYPE::UTF_8;
				}
			else if constexpr( std::same_as< T, Char16 > )
				{
					return TYPE::UTF_16;
				}
			else if constexpr( std::same_as< T, Char32 > )
				{
					return TYPE::UTF_32;
				}
			else if constexpr( std::same_as< T, Char > )
				{
					return TYPE::MBCS;
				}
			else if constexpr( std::same_as< T, WChar > )
				{
					return TYPE::WCS;
				}
			else
				{
					static_assert( "Missing enum TYPE conversion for character" );
				}
		}

	/*
	 @	Max left shift of conversion size
	 */
	template< Character Src,
			  Character Dst >

	FC_INLINE consteval UInt Charset::MaxRatio() noexcept
		{
			/*	Replace WChar with Char16 or Char32
			*/
			using SrcT = Replace_wchar< Src >;
			using DstT = Replace_wchar< Dst >;

			/*	Fast estimate
			*/
			if constexpr( std::same_as< SrcT, Char32 > )
				{
					if constexpr( std::same_as< DstT, Char >
							   || std::same_as< DstT, Char16 > )
						{
							return 1;
						}
					else if constexpr( std::same_as< DstT, Char8 > )
						{
							return 2;
						}
				}
			else if constexpr( std::same_as< SrcT, Char16 > )
				{
					if constexpr( std::same_as< DstT, Char > )
						{
							return 1;
						}
					else if constexpr( std::same_as< DstT, Char8 > )
						{
							return 2;
						}
				}
			else if constexpr( std::same_as< SrcT, Char > )
				{
					if constexpr( std::same_as< DstT, Char8 > )
						{
							return 1;
						}
				}

			return 0;
		}
	
	/*
	 @	Copy length-limited C_string
	 */
	template< Bool NULL_TERMINATOR,
			  Character T >
	
	UInt Charset::Copy( const T* const src, const UInt srcLen,
							  T* const dst, const UInt dstLen )
		{
			FC_ASSERT( src && dst );

			UInt len;

			if constexpr( NULL_TERMINATOR )
				{
					len = std::min( srcLen, dstLen - 1 );
				}
			else
				{
					len = std::min( srcLen, dstLen );
				}
	
			std::char_traits< T >::copy( dst, src, len );

			if constexpr( NULL_TERMINATOR )
				{
					dst[ len ] = 0;
				}
	
			return len;
		}

	/*
	 @	Max possible count of target string

			char
			wchar_t
			char8_t
			char16_t
			char32_t

		Includes null-terminator and BOM
		Fast algorithm for target length
	 */
	template< Character Src,
			  Character Dst >
	
	FC_INLINE constexpr UInt Charset::TargetMaxLength( const UInt srcLen ) noexcept
		{
			constexpr UInt additional{ std::same_as< Dst, Char8 > ? 4 : 2 };
	
			return ( srcLen << MaxRatio< Src, Dst >() )
				 + additional;
		}
}



namespace fce
{
	/*
	 @	Convert implementation
	 
			length-limited C_string
	 */
	template< Bool NULL_TERMINATOR,
			  Character Src,
			  Character Dst >
	
	UInt Charset::Convert_imp( const Src* const src, const UInt srcLen,
									 Dst* const dst, const UInt dstLen )
		{
			if constexpr( Same_char< Src, Dst > )
				{
					return Copy< NULL_TERMINATOR >( src, srcLen,
													dst, dstLen );
				}
			else
				{
					FC_ASSERT( src && dst );
	
					UInt len
						{
							this->impl.Convert( src, srcLen,
												dst, dstLen )
						};

					if constexpr( NULL_TERMINATOR )
						{
							dst[ len ] = 0;
						}
	
					return len;
				}
		}
	

	/*
	 @	Convert implementation
	 
			From length-limited C_string
			To	 std::basic_string
	 */
	template< Character Src,
			  Stl_string Dst >
	
	Void Charset::Convert_imp( const Src* const src,
							   const UInt srcLen,
									 Dst& dst )
		{
			FC_ASSERT( src );
	
			using CharT = Dst::value_type;

			if constexpr( Same_char< Src, CharT > )
				{
					dst.assign( ( const CharT* )src , srcLen );
				}
			else
				{
					dst.resize( TargetMaxLength< Src, CharT >( srcLen ) );
	
					dst.resize( this->impl.Convert( src,
													srcLen,
													dst.data(),
													dst.length() ) );
				}
		}
}



namespace fce
{
	/*
	 @	Convert From null-terminated C_string
				To	 std::basic_string
	
		Use unsafe methods with caution
	 */
	template< Character Src,
			  Stl_string Dst >
	
	FC_INLINE Void Charset::Convert( const Src* const src,
										   Dst& dst )
		{
			Convert( src, Length( src ),
					 dst );
		}
	

	/*
	 @	Convert From length-limited C_string
				To	 std::basic_string
	 */
	template< Character Src,
			  Stl_string Dst >
	
	FC_INLINE Void Charset::Convert( const Src* const src,
									 const UInt srcLen,
										   Dst& dst )
		{
			s_Charset.Convert_imp( src, srcLen,
								   dst );
		}


	/*
	 @	Convert from std::basic_string
					 std::basic_string_view
				To	 length-limited C_string
	 */
	template< Bool NULL_TERMINATOR,
			  Cpp_string Src,
			  Character Dst >
	
	FC_INLINE UInt Charset::Convert( const Src& src,
										   Dst* const dst,
									 const UInt dstLen )
		{
			return Convert< NULL_TERMINATOR >( src.data(),
											   src.length(),
											   dst, dstLen );
		}
	

	/*
	 @	Convert from std::basic_string
					 std::basic_string_view
				To	 std::basic_string
	
		This safe method is recommended
	 */
	template< Cpp_string Src,
			  Stl_string Dst >
	
	FC_INLINE Void Charset::Convert( const Src& src,
										   Dst& dst ) noexcept
		{
			Convert( src.data(),
					 src.length(),
					 dst );
		}
	

	/*
	 @	Convert from null-terminated C_string
				To	 length-limited C_string
	 */
	template< Bool NULL_TERMINATOR,
			  Character Src,
			  Character Dst >
	
	FC_INLINE UInt Charset::Convert( const Src* const src,
										   Dst* const dst,
									 const UInt dstLen )
		{
			return Convert< NULL_TERMINATOR >( src,
											   Length( src ),
											   dst, dstLen );
		}
	

	/*
	 @	Convert length-limited C_string
	
			char *
			wchar_t *
			char8_t *
			char16_t *
			char32_t *

		return length of target string
	 */
	template< Bool NULL_TERMINATOR,
			  Character Src,
			  Character Dst >
	
	FC_INLINE UInt Charset::Convert( const Src* const src, const UInt srcLen,
										   Dst* const dst, const UInt dstLen )
		{
			return s_Charset.Convert_imp< NULL_TERMINATOR >( src, srcLen,
															 dst, dstLen );
		}
	

	/*
	 @	Convert ( Syntax sugar )
	 */
	template< Character Dst,
			  typename ...Args >
	
	FC_INLINE stl::basic_string< Dst > Charset::Convert( Args&& ...args )
		{
			stl::basic_string< Dst > dst;

			Convert( std::forward< Args >( args )...,
					 dst );

			return dst;
		}
}