/*
**
**	String inlines
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE String::String( const String& str ) noexcept
		:
			u32str( str.u32str )
		{
		}

	FC_INLINE String::String( String&& str ) noexcept
		:
			u32str( std::move( str.u32str ) )
		{
		}

	/*
	 @	Constructor
	 
			Char32 *
			std::u32string
			std::u32string_view

			std::filesystem::path
	 */
	FC_INLINE String::String( const Char32* const sz32 )
		:
			u32str( sz32 )
		{
		}

	FC_INLINE String::String( const Char32* const sz32,
							  const UInt len )
		:
			u32str( sz32, len )
		{
		}

	FC_INLINE String::String( const stl::u32string& u32str ) noexcept
		:
			u32str( u32str )
		{
		}

	FC_INLINE String::String( stl::u32string&& u32str ) noexcept
		:
			u32str( std::move( u32str ) )
		{
		}

	FC_INLINE String::String( const std::u32string_view& u32v ) noexcept
		:
			u32str( u32v )
		{
		}

	FC_INLINE String::String( const std::filesystem::path& path ) noexcept
		:
			String( path.native() )
		{
		}

	/*
	 @	Constructor
	
			Convert charset
			from a std::basic_string
			  or a std::basic_string_view
			  or a null-terminated string
	 */
	FC_INLINE String::String( const Character auto* const sz )
		{
			Charset::Convert( sz, this->u32str );
		}

	FC_INLINE String::String( const Character auto* const sz,
							  const UInt len )
		{
			Charset::Convert( sz, len, this->u32str );
		}

	FC_INLINE String::String( const Stl_string auto& bs ) noexcept
		{
			Charset::Convert( bs, this->u32str );
		}

	FC_INLINE String::String( const Stl_string_view auto& bsv ) noexcept
		{
			Charset::Convert( bsv, this->u32str );
		}
}



/**
 !	Conversion
 */
namespace fce
{
	/*
	 @	Char32 *
	 */
	FC_INLINE const Char32* const String::operator * () const noexcept
		{
			return this->u32str.c_str();
		}

	/*
	 @	stl::u32string
	 */
	FC_INLINE String::operator stl::u32string& () noexcept
		{
			return this->u32str;
		}

	FC_INLINE String::operator const stl::u32string& () const noexcept
		{
			return this->u32str;
		}

	/*
	 @	std::basic_string
	 */
	template< Character T >

	FC_INLINE String::operator stl::basic_string< T >() const noexcept
		{
			return Charset::Convert< T >( this->u32str );
		}
}



/**
 !	Assignment
 */
namespace fce
{
	/*
	 @	Assign
	 */
	FC_INLINE String& String::Assign( const Char32* const sz32,
									  const UInt len )
		{
			this->u32str.assign( sz32, len );

			return *this;
		}

	FC_INLINE String& String::Assign( const Character auto* const sz,
									  const UInt len )
		{
			Charset::Convert( sz, len, this->u32str );

			return *this;
		}

	/*
	 @	operator =
	 */
	FC_INLINE String& String::operator = ( const String& str ) noexcept
		{
			this->u32str = str.u32str;

			return *this;
		}

	FC_INLINE String& String::operator = ( String&& str ) noexcept
		{
			this->u32str = std::move( str.u32str );

			return *this;
		}

	/*
	 @	operator =
	
			Char32
			Char32 *
			std::u32string
			std::u32string_view
			std::filesystem::path
			std::initializer_list< Char32 >
	 */
	FC_INLINE String& String::operator = ( const Char32 ch32 ) noexcept
		{
			this->u32str = ch32;

			return *this;
		}

	FC_INLINE String& String::operator = ( const Char32* const sz32 )
		{
			this->u32str = sz32;

			return *this;
		}

	FC_INLINE String& String::operator = ( const stl::u32string& u32str ) noexcept
		{
			this->u32str = u32str;

			return *this;
		}

	FC_INLINE String& String::operator = ( stl::u32string&& u32str ) noexcept
		{
			this->u32str = std::move( u32str );

			return *this;
		}

	FC_INLINE String& String::operator = ( const std::u32string_view& u32v ) noexcept
		{
			this->u32str = u32v;

			return *this;
		}

	FC_INLINE String& String::operator = ( const std::filesystem::path& path ) noexcept
		{
			return *this = path.native();
		}

	FC_INLINE String& String::operator = ( const std::initializer_list< Char32 >& ilist ) noexcept
		{
			this->u32str = ilist;

			return *this;
		}

	/*
	 @	operator =

			Convert charset
			from a std::basic_string
			  or a std::basic_string_view
			  or a null-terminated string

			  or a integer
			  or a floating number
	 */
	FC_INLINE String& String::operator = ( const Character auto* const sz )
		{
			Charset::Convert( sz, this->u32str );

			return *this;
		}

	FC_INLINE String& String::operator = ( const Stl_string auto& bs ) noexcept
		{
			Charset::Convert( bs, this->u32str );

			return *this;
		}

	FC_INLINE String& String::operator = ( const Stl_string_view auto& bsv ) noexcept
		{
			Charset::Convert( bsv, this->u32str );

			return *this;
		}

	String& String::operator = ( const Real_number auto rn ) noexcept
		{
			stl::string r( std::to_string( rn ) );

			auto size{ r.size() };

			this->u32str.resize( size );

			while( size-- )
				{
					this->u32str[ size ] = r[ size ];
				}

			return *this;
		}

	/*
	 @	operator +=

			Append a String

			Include convert charset
			from a std::basic_string
			  or a std::basic_string_view
			  or a null-terminated string

			  or a integer
			  or a floating number
	 */
	FC_INLINE String& String::operator += ( const String& str ) noexcept
		{
			this->u32str += str.u32str;

			return *this;
		}

	String& String::operator += ( const Real_number auto rn ) noexcept
		{
			const stl::string r( std::to_string( rn ) );

			const auto size{ this->u32str.size() };

			this->u32str.resize( r.size() + size );

			Char32* p{ this->u32str.data() + size };

			for( Char ch : r )
				{
					*p++ = ch;
				}

			return *this;
		}

	/*
	 @	operator +=

			Char32
			Char32 *
			std::u32string
			std::u32string_view
	 */
	FC_INLINE String& String::operator += ( const Char32 ch32 ) noexcept
		{
			this->u32str += ch32;

			return *this;
		}

	FC_INLINE String& String::operator += ( const Char32* const sz32 )
		{
			this->u32str += sz32;

			return *this;
		}

	FC_INLINE String& String::operator += ( const stl::u32string& u32str ) noexcept
		{
			this->u32str += u32str;

			return *this;
		}

	FC_INLINE String& String::operator += ( const std::u32string_view& u32v ) noexcept
		{
			this->u32str += u32v;

			return *this;
		}

	FC_INLINE String& String::operator += ( const std::initializer_list< Char32 >& ilist ) noexcept
		{
			this->u32str += ilist;

			return *this;
		}
}



/**
 !	Common
 */
namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto String::end( this auto& self ) noexcept
		{
			return self.u32str.end();
		}

	FC_INLINE auto String::begin( this auto& self ) noexcept
		{
			return self.u32str.begin();
		}

	/*
	 @	Access
	 */
	FC_INLINE auto String::operator -> ( this auto& self ) noexcept
		{
			return &self.u32str;
		}

	FC_INLINE auto& String::operator [] ( this auto& self, const UInt idx )
		{
			return self.u32str[ idx ];
		}

	/*
	 @	Empty
	 */
	FC_INLINE constexpr const auto& String::Native() const noexcept
		{
			return this->u32str;
		}

	/*
	 @	Empty
	 */
	FC_INLINE constexpr Bool String::Empty() const noexcept
		{
			return this->u32str.empty();
		}

	/*
	 @	Clear
	 */
	FC_INLINE constexpr Void String::Clear() noexcept
		{
			this->u32str.clear();
		}

	/*
	 @	Length
	 */
	FC_INLINE constexpr UInt String::Length() const noexcept
		{
			return this->u32str.length();
		}

	FC_INLINE constexpr UInt String::Length( const Character auto* const sz )
		{
			return Charset::Length( sz );
		}

	/*
	 @	Size
	
			Includes '\0'
	 */
	FC_INLINE constexpr UInt String::Size() const noexcept
		{
			return this->Length() + 1;
		}

	FC_INLINE constexpr UInt String::Size( const Character auto* const sz )
		{
			return Length( sz ) + 1;
		}
	
	/*
	 @	Memory size
	 */
	template< Bool NULL_TERMINATOR >
	
	FC_INLINE constexpr UInt String::Capacity() const noexcept
		{
			return Capacity< NULL_TERMINATOR >( this->u32str );
		}
	
	/*
	 @	Calculate buffer size with the null-terminated string
	 */
	template< Character T >
	
	FC_INLINE constexpr UInt String::Capacity( const T* const sz )
		{
			return Capacity< T >( Length( sz ) + 1 );
		}
	
	/*
	 @	Calculate buffer size with the std::basic_string
	 */
	template< Bool NULL_TERMINATOR,
			  Stl_string T >
	
	FC_INLINE constexpr UInt String::Capacity( const T& str ) noexcept
		{
			using CharT = T::value_type;

			return Capacity< CharT >( str.length()
									  + NULL_TERMINATOR );
		}
	
	/*
	 @	Calculate buffer size with the std::basic_string_view
	 */
	template< Bool NULL_TERMINATOR,
			  Stl_string_view T >
	
	FC_INLINE constexpr UInt String::Capacity( const T& str ) noexcept
		{
			using CharT = T::value_type;

			return Capacity< CharT >( str.length()
									  + NULL_TERMINATOR );
		}
	
	/*
	 @	Calculate buffer size with the length
	 */
	template< Character T >
	
	FC_INLINE constexpr UInt String::Capacity( const UInt len ) noexcept
		{
			return len << Charset::Offset< T >();
		}

	/*
	 @	Copy length-limited string
	 */
	template< Character T >
	
	FC_INLINE Void String::Copy( const T* src,
									   T* dst )
		{
			FC_ASSERT( src && dst );

			while( *dst++ = *src++ );
		}

	/*
	 @	Copy length-limited string
	 */
	template< Character T >
	
	FC_INLINE auto String::Copy( const T* const src,
									   T* const dst,
								 const UInt count )
		{
			FC_ASSERT( src && dst );

			return std::char_traits< T >::copy( dst, src, count );
		}

	/*
	 @	Move length-limited string
	 */
	template< Character T >
	
	FC_INLINE auto String::Move( const T* const src,
									   T* const dst,
								 const UInt count )
		{
			FC_ASSERT( src && dst );

			return std::char_traits< T >::move( dst, src, count );
		}
}



/**
 !	Global overload
 */
namespace fce
{
	/*
	 @	Overload operator +
	 */
	FC_INLINE String operator + ( const String& left,
								  const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				 + ( const stl::u32string& )right;
		}

	FC_INLINE String operator + ( const String&& left,
								  const String&& right ) noexcept
		{
			return std::move( ( stl::u32string )left )
				 + std::move( ( stl::u32string )right );
		}

	/*
	 @	Overload operator +	( left String& )

			Char32
			Char32 *
			std::u32string
			std::u32string_view

			integer
			floating number
	 */
	FC_INLINE String operator + ( const String& str,
								  const Char32 ch32 ) noexcept
		{
			return ( const stl::u32string& )str + ch32;
		}

	FC_INLINE String operator + ( const String& str,
								  const Char32* const sz32 )
		{
			return ( const stl::u32string& )str + sz32;
		}

	FC_INLINE String operator + ( const String& str,
								  const stl::u32string& u32str ) noexcept
		{
			return ( const stl::u32string& )str + u32str;
		}

	FC_INLINE String operator + ( const String& str,
								  const std::u32string_view& u32v ) noexcept
		{
			return ( const stl::u32string& )str + u32v.data();
		}

	FC_INLINE String operator + ( const String& str,
								  const Real_number auto rn ) noexcept
		{
			return String( str ) += rn;
		}

	/*
	 @	Overload operator +	( right String& )
	 */
	FC_INLINE String operator + ( const Char32 ch32,
								  const String& str ) noexcept
		{
			return ch32 + ( const stl::u32string& )str;
		}

	FC_INLINE String operator + ( const Char32* const sz32,
								  const String& str )
		{
			return sz32 + ( const stl::u32string& )str;
		}

	FC_INLINE String operator + ( const stl::u32string& u32str,
								  const String& str ) noexcept
		{
			return u32str + ( const stl::u32string& )str;
		}

	FC_INLINE String operator + ( const std::u32string_view& u32v,
								  const String& str ) noexcept
		{
			return u32v.data() + ( const stl::u32string& )str;
		}

	FC_INLINE String operator + ( const Real_number auto rn,
								  const String& str ) noexcept
		{
			String strn;

			return strn = rn, strn += str;
		}

	/*
	 @	Overload compare
	 */
	FC_INLINE [[nodiscard]] Bool operator == ( const String& left,
											   const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				== ( const stl::u32string& )right;
		}

	FC_INLINE [[nodiscard]] Bool operator != ( const String& left,
											   const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				!= ( const stl::u32string& )right;
		}

	FC_INLINE [[nodiscard]] Bool operator >= ( const String& left,
											   const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				>= ( const stl::u32string& )right;
		}

	FC_INLINE [[nodiscard]] Bool operator <= ( const String& left,
											   const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				<= ( const stl::u32string& )right;
		}

	FC_INLINE [[nodiscard]] Bool operator > ( const String& left,
											  const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				 > ( const stl::u32string& )right;
		}

	FC_INLINE [[nodiscard]] Bool operator < ( const String& left,
											  const String& right ) noexcept
		{
			return ( const stl::u32string& )left
				 < ( const stl::u32string& )right;
		}
}