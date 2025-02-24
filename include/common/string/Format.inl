/*
**
**	Format inlines
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 @	Constructor
	 */
	template< class Fmt,
			  typename ...Args >

	FC_INLINE Format::Format( const Fmt& fmt,
							  const Args& ...args )
		{
			( *this )( fmt, args... );
		}

	/*
	 @	Constructor
	 */
	template< class Fmt,
			  typename ...Args >

	FC_INLINE Format::Format( String& str,
							  const Fmt& fmt,
							  const Args& ...args )
		{
			str = ( *this )( fmt, args... );
		}

	/*
	 @	Conversion
	 */
	FC_INLINE Format::operator String() const noexcept
		{
			return this->fmt.str();
		}

	FC_INLINE Format::operator stl::wstring() const noexcept
		{
			return this->fmt.str();
		}

	FC_INLINE Format::operator std::wstring() const noexcept
		{
			return std::wstring( this->fmt.str() );
		}

	/*
	 @	Conversion
	 */
	template< Character T >

	FC_INLINE Format::operator stl::basic_string< T >() const noexcept
		{
			return Charset::Convert< T >( this->fmt.str() ) ;
		}

	/*	Convert char set
	*/
	template< typename T >

	FC_INLINE auto Format::Convert( const T& arg )
		{
			if constexpr( Any_of< std::remove_cvref_t< T >,
								  String, std::wstring, std::wstring_view > )
				{
					return stl::wstring( arg );
				}
			else if constexpr( Character_string< T >
							&& ! Stl_wstring< T >
							&& ! Any_of< std::remove_cvref_t< T >,
										 WChar*, const WChar* > )
				{
					return Charset::Convert< WChar >( arg );
				}
			else
				{
					return arg;
				}
		}

	/*
	 @	Parse format
	 */
	template< class Fmt,
			  typename ...Args >

	stl::wstring Format::operator() ( const Fmt& strFmt,
									  const Args& ...args )
		{
			this->fmt.parse( Convert( strFmt ) );

			FC_ASSERT( sizeof...( Args )
					== this->fmt.expected_args() );

			( ( this->fmt % Convert( args ) ), ... );

			return this->fmt.str();
		}
}