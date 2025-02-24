/*
**
**	Buffer inlines
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 @	Constructor
	 */
	template< class T >

	requires requires( const T& obj, Buffer& buf )
		{
			{ obj >> buf } -> std::same_as< Buffer& >;
		}

	FC_INLINE Buffer::Buffer( const T& obj ) NOEXCEPT
		:
			Buffer( 0 )
		{
			obj >> *this;

			this->SeekBeg();
		}

	/*
	 @	Check whether the buffer is empty
	 */
	FC_INLINE Bool Buffer::Empty() const noexcept
		{
			return this->beg == this->end;
		}

	/*
	 @	Get last pointer
	 */
	FC_INLINE auto* Buffer::Last( this auto&& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*( self.end - 1 )
					);
		}

	/*
	 @	Get base pointer
	 */
	template< typename T >

	FC_INLINE auto* Buffer::Data( this auto&& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*reinterpret_cast< T* >( self.beg )
					);
		}

	/*
	 @	Get current pointer
	 */
	FC_INLINE auto* Buffer::operator * ( this auto&& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*self.cur
					);
		}

	/*
	 @	Get current pointer by type

			Warning: Avoid different byte order
	 */
	template< typename T >

	FC_INLINE auto* Buffer::Ptr( this auto&& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*reinterpret_cast< T* >( self.cur )
					);
		}

	/*
	 @	Check whether the buffer is valid
	 */
	FC_INLINE Buffer::operator Bool() const noexcept
		{
			return this->beg;
		}

	/*
	 @	Zero
	 */
	FC_INLINE Void Buffer::Zero() noexcept
		{
			memset( this->Data(), 0,
					this->Size() );
		}

	/*
	 @	Gets the current pointer position
	 */
	FC_INLINE UInt Buffer::Pos() const noexcept
		{
			return this->cur - this->beg;
		}

	/*
	 @	Get buffer size
	 */
	FC_INLINE UInt Buffer::Size() const noexcept
		{
			return this->end - this->beg;
		}

	/*
	 @	Get current size
	 */
	FC_INLINE UInt Buffer::CurSize() const noexcept
		{
			return this->end - this->cur;
		}

	/*
	 @	Get capacity
	 */
	FC_INLINE UInt Buffer::Capacity() const noexcept
		{
			return this->pool
				 ? this->pool->GetCellSize() : 0;
		}

	/*
	 @	Info ( Syntax sugar )
	 */
	FC_INLINE auto& Buffer::Info( auto&& arg,
								  const auto&... ) noexcept
		{
			return arg;
		}

	/*
	 @	Serialize	group
	 */
	FC_INLINE Buffer& Buffer::SerializeGroup( const auto&... args ) NOEXCEPT
		{
			if constexpr( sizeof...( args ) )
				{ 
					return ( ( *this << args ), ... );
				}
			else
				{
					return *this;
				}
		}

	FC_INLINE const Buffer& Buffer::DeserializeGroup( auto&... args ) const
		{
			if constexpr( sizeof...( args ) )
				{ 
					return ( ( *this >> args ), ... );
				}
			else
				{
					return *this;
				}
		}
}



/**
 !	Move cursor operation
 */
namespace fce
{
	/*
	 @	Seek	from current
	 */
	FC_INLINE auto&& Buffer::Seek( this auto&& self,
								   const Integer auto offset ) NOEXCEPT
		{
			self.cur += offset;

			if constexpr( Signed_integer< decltype( offset ) > )
				{
					FC_ASSERT( self.CheckBounds() );
				}
			else
				{
					FC_ASSERT( self.CheckEnd() );
				}

			return std::forward< decltype( self ) >( self );
		}

	/*
	 @	Seek	from begin
	 */
	FC_INLINE auto&& Buffer::SeekBeg( this auto&& self,
									  const UInt offset ) NOEXCEPT
		{
			self.cur = self.beg + offset;

			FC_ASSERT( self.CheckEnd() );

			return std::forward< decltype( self ) >( self );
		}

	/*
	 @	Seek	from the end
	 */
	FC_INLINE auto&& Buffer::SeekEnd( this auto&& self,
									  const UInt offset ) NOEXCEPT
		{
			self.cur = self.end - offset;

			FC_ASSERT( self.CheckBeg() );

			return std::forward< decltype( self ) >( self );
		}
}



/**
 !	Get data
 */
namespace fce
{
	/*
	 @	Get value T ( Syntax sugar )
	 */
	template< typename T >

	FC_INLINE T Buffer::Get() const
		{
			T val;

			*this >> val;

			return val;
		}

	/*
	 @	Get the string directly from the buffer

			little-endian

				Return string pointer

				The lifecycle of the value
				is consistent with the buffer

			big-endian

				Return std::basic_string
				if it is a wide character
	 */
	template< Character T >

	FC_INLINE auto Buffer::GetString() const
		{
			if constexpr( endian_requires_conversion< T > )
				{
					return this->Get
							<
								stl::basic_string< T >
							>
							();
				}
			else
				{
					const T* const sz
						{
							this->Ptr< T >()
						};

					this->Seek( String::Capacity( sz ) );

					return sz;
				}
		}

	FC_INLINE auto Buffer::GetStringA() const
		{
			return this->GetString< Char >();
		}

	FC_INLINE auto Buffer::GetStringW() const
		{
			return this->GetString< WChar >();
		}

	FC_INLINE auto Buffer::GetString8() const
		{
			return this->GetString< Char8 >();
		}

	FC_INLINE auto Buffer::GetString16() const
		{
			return this->GetString< Char16 >();
		}

	FC_INLINE auto Buffer::GetString32() const
		{
			return this->GetString< Char32 >();
		}


	/*
	 @	Read data	from memory pointer
	 */
	FC_INLINE const Buffer& Buffer::Read( Void* const dst,
										  const UInt size ) const
		{
			FC_ASSERT( ! size || ( size && dst ) );

			std::memcpy( dst, this->cur, size );

			return this->Seek( size );
		}

	/*
	 @	Read data	from the std::ostream
	 */
	FC_INLINE const Buffer& Buffer::Read( std::ostream& os,
										  const UInt size ) const
		{
			os.write( ( const Char* )this->cur, size );

			return this->Seek( size );
		}
}



/**
 !	Serialize & Deserialize		
 */
namespace fce
{
	/*
	 @	left object ( Syntax sugar )
	 */
	template< class T >

	requires requires( const T& obj, Buffer& buf )
		{
			{ obj >> buf } -> std::same_as< Buffer& >;
		}

	FC_INLINE Buffer& Buffer::operator << ( const T& obj ) NOEXCEPT
		{
			return obj >> *this;
		}

	/*
	 @	left object ( Syntax sugar )
	 */
	template< class T >

	requires requires( T& obj, const Buffer& buf )
		{
			{ obj << buf } -> std::same_as< const Buffer& >;
		}

	FC_INLINE const Buffer& Buffer::operator >> ( T& obj ) const
		{
			return obj << *this;
		}

	/*
	 @	pointer object
	 */
	FC_INLINE Buffer& Buffer::operator << ( const Archive_pointer auto& obj ) NOEXCEPT
		{
			FC_ASSERT( obj );

			if constexpr( std::derived_from< std::remove_cvref_t< decltype( *obj ) >,
											 ObjectEx > )
				{
					return obj->Serialize( *this );
				}
			else
				{
					return *this << *obj;
				}
		}

	FC_INLINE const Buffer& Buffer::operator >> ( Archive_pointer auto& obj ) const
		{
			FC_ASSERT( obj );

			return *this >> *obj;
		}


	/*
	 @	Serialize	numerical value
	 */
	template< Arithmetic T >

	FC_INLINE Buffer& Buffer::operator << ( const T val ) noexcept
		{
			this->Adjust( sizeof( T ) );

			T*& rp{ reinterpret_cast< T*& >( this->cur ) };

			if constexpr( endian_requires_conversion< T > )
				{
					this->EndianSave( val );
				}
			else
				{
					*rp = val;
				}

			++rp;

			return *this;
		}

	/*
	 @	Deserialize	numerical value
	 */
	template< Arithmetic T >

	FC_INLINE const Buffer& Buffer::operator >> ( T& val ) const
		{
			FC_ASSERT( this->cur );

			T*& rp{ reinterpret_cast< T*& >( this->cur ) };

			if constexpr( endian_requires_conversion< T > )
				{
					this->EndianLoad( val );
				}
			else
				{
					val = *rp;
				}

			++rp;

			FC_ASSERT( this->CheckEnd() );

			return *this;
		}


	/*
	 @	String	stored in utf-16
	 */
	FC_INLINE Buffer& Buffer::operator << ( const String& str ) noexcept
		{
			return *this << str.operator stl::u16string();
		}

	FC_INLINE const Buffer& Buffer::operator >> ( String& str ) const
		{
			str = this->GetString16();

			return *this;
		}


	/*
	 @	Serialize	null-terminated string
	 */
	template< Character T >

	Buffer& Buffer::operator << ( const T* sz ) NOEXCEPT
		{
			this->Adjust( sizeof( T )
						* String::Size( sz ) );

			T*& rp{ reinterpret_cast< T*& >( this->cur ) };

			if constexpr( endian_requires_conversion< T > )
				{
					do	{
							this->EndianSave( *sz++ );
						}
					while( *rp++ );
				}
			else
				{
					while( *rp++ = *sz++ );
				}

			return *this;
		}

	/*
	 @	Deserialize	null-terminated string
	 */
	template< Character T >

	const Buffer& Buffer::operator >> ( T* sz ) const
		{
			FC_ASSERT( this->cur );

			T*& rp{ reinterpret_cast< T*& >( this->cur ) };

			if constexpr( endian_requires_conversion< T > )
				{
					do	{
							this->EndianLoad( *sz++ );
						}
					while( *rp++ );
				}
			else
				{
					while( *sz++ = *rp++ );
				}

			FC_ASSERT( this->CheckEnd() );

			return *this;
		}


	/*
	 @	Raw_object
	 */
	template< Raw_object T >

	Buffer& Buffer::operator << ( const T& raw ) NOEXCEPT
		{
			if( raw )
				{
					return *this << true
								 << *raw;
				}

			return *this << false;
		}

	template< Raw_object T >

	const Buffer& Buffer::operator >> ( T& raw ) const
		{
			if( this->Get< Bool >() )
				{
					if constexpr( std::constructible_from< typename T::Value,
														   const Buffer& > )
						{
							raw.Construct( *this );
						}
					else
						{
							*this >> *raw.Construct();
						}
				}

			return *this;
		}
}



/**
 !	Serialize & Deserialize		Color object
 */
namespace fce
{
	/*
	 @	Color2
	 */
	FC_INLINE Buffer& Buffer::operator << ( const Color2& color2 ) noexcept
		{
			return *this << color2.value;
		}

	FC_INLINE const Buffer& Buffer::operator >> ( Color2& color2 ) const
		{
			return *this >> color2.value;
		}

	/*
	 @	Color4
	 */
	FC_INLINE Buffer& Buffer::operator << ( const Color4& color4 ) noexcept
		{
			return *this << color4.value;
		}

	FC_INLINE const Buffer& Buffer::operator >> ( Color4& color4 ) const
		{
			return *this >> color4.value;
		}

	/*
	 @	Color_object
	 */
	Buffer& Buffer::operator << ( const Color_object auto& color ) noexcept
		{
			return *this << Color4( color );
		}

	const Buffer& Buffer::operator >> ( Color_object auto& color ) const
		{
			color = this->Get< Color4 >();

			return *this;
		}
}



/**
 !	Serialize & Deserialize		stl object
 */
namespace fce
{
	/*
	 @	std::pointer
	 */
	template< Stl_pointer T >

	FC_INLINE Buffer& Buffer::operator << ( const T& ptr ) NOEXCEPT
		{
			FC_ASSERT( ptr );

			return *this << *ptr;
		}

	template< Stl_pointer T >

	FC_INLINE const Buffer& Buffer::operator >> ( T& ptr ) const
		{
			FC_ASSERT( ptr );

			return *this >> *ptr;
		}


	/*
	 @	std::pair
	 */
	template< class T, class U >

	FC_INLINE Buffer& Buffer::operator << ( const std::pair< T, U >& pair ) NOEXCEPT
		{
			return *this << pair.first << pair.second;
		}

	template< class T, class U >

	FC_INLINE const Buffer& Buffer::operator >> ( std::pair< T, U >& pair ) const
		{
			return *this >> const_cast< std::remove_const_t< T >& >( pair.first )
						 >> const_cast< std::remove_const_t< U >& >( pair.second );
		}


	/*
	 @	std::filesystem::path	stored in utf-8
	 */
	FC_INLINE Buffer& Buffer::operator << ( const std::filesystem::path& path ) noexcept
		{
			return *this << path.u8string();
		}

	FC_INLINE const Buffer& Buffer::operator >> ( std::filesystem::path& path ) const
		{
			path.assign( this->GetString8() );

			return *this;
		}


	/*
	 @	Serialize	std::tuple
	 */
	template< typename ...Args >

	FC_INLINE Buffer& Buffer::operator << ( const std::tuple< Args... >& tp ) NOEXCEPT
		{
			auto f = [ this ]( const auto&... args ) NOEXCEPT -> Buffer&
				{
					return this->SerializeGroup( args... );
				};

			return std::apply( f, tp );
		}

	/*
	 @	Deserialize	std::tuple
	 */
	template< typename ...Args >

	FC_INLINE const Buffer& Buffer::operator >> ( std::tuple< Args... >& tp ) const
		{
			auto f = [ this ]( auto&... args ) NOEXCEPT -> const Buffer&
				{
					return this->DeserializeGroup( args... );
				};

			return std::apply( f, tp );
		}
}



/**
 !	Serialize & Deserialize		stl::string
 */
namespace fce
{
	/*
	 @	Serialize	std::basic_string
	 */
	template< typename ...Args >

	FC_INLINE Buffer& Buffer::operator << ( const std::basic_string< Args... >& str ) noexcept
		{
			return *this << str.c_str();
		}

	/*
	 @	Deserialize	std::basic_string
	 */
	template< typename ...Args >

	const Buffer& Buffer::operator >> ( std::basic_string< Args... >& str ) const
		{
			using CharT = std::basic_string< Args... >::value_type;

			str.resize( String::Length( this->Ptr< CharT >() ) );

			return *this >> str.data();
		}


	/*
	 @	Serialize	std::basic_string_view
	 */
	template< typename ...Args >

	Buffer& Buffer::operator << ( const std::basic_string_view< Args... >& bsv ) noexcept
		{
			for( const auto c : bsv )
				{
					if( !c ) break;

					*this << c;
				}

			*this << std::basic_string_view< Args... >::value_type( 0 );

			return *this;
		}
}



/**
 !	Serialize & Deserialize		stl::ranges
 */
namespace fce
{
	/*
	 @	Serialize	Container_object
	 */
	template< Container_object T >

	Buffer& Buffer::operator << ( const T& container ) NOEXCEPT
		{
			UInt n;

			// sized_range
			if constexpr( std::ranges::sized_range< T > )
				{
					n = std::ranges::size( container );
				}
			// Unknown size
			else
				{
					n = std::distance( container );
				}

			// There is little need for more than 32 bits
			FC_ASSERT( n < UInt32_MAX );

			*this << static_cast< UInt32 >( n );

			for( const auto& val : container )
				{
					*this << val;
				}

			return *this;
		}

	/*
	 @	Deserialize	ranges >> array
	 */
	template< Fixed_array T >

	const Buffer& Buffer::operator >> ( T& arr ) const
		{
			UInt32 n;

			*this >> n;

			FC_ASSERT( n <= std::ranges::size( arr ) );

			for( UInt32 i{ 0 }; i < n; ++i )
				{
					*this >> arr[ i ];
				}

			return *this;
		}

	/*
	 @	Deserialize	ranges >> T::resize
	 */
	template< Container_with_resize T >

	const Buffer& Buffer::operator >> ( T& container ) const
		{
			UInt32 n;

			*this >> n;

			using Value = std::ranges::range_value_t< T >;

			if constexpr( std::constructible_from< Value, const Buffer& > )
				{
					if constexpr( Container_with_reserve< T > )
						{
							container.reserve( n );
						}

					while( n-- )
						{
							if constexpr( Template< T, std::forward_list > )
								{
									container.emplace_front( *this );
								}
							else
								{
									container.emplace_back( *this );
								}
						}
				}
			else
				{
					container.resize( n );

					for( auto& val : container )
						{
							*this >> val;
						}
				}

			return *this;
		}

	/*
	 @	Deserialize	ranges >> T::emplace
	 */
	template< Container_with_emplace T >

	const Buffer& Buffer::operator >> ( T& container ) const
		{
			UInt32 n;

			*this >> n;

			using Value = std::ranges::range_value_t< T >;

			while( n-- )
				{
					if constexpr( std::constructible_from< Value, const Buffer& > )
						{
							container.emplace( *this );
						}
					else
						{
							container.emplace( this->Get< Value >() );
						}
				}

			return *this;
		}
}



/**
 !	Byte order compatible operation
 */
namespace fce
{
	/*
	 @	Check	big or little endian
	 */
	template< Arithmetic T >

	FC_INLINE consteval Bool Buffer::CheckEndian() noexcept
		{
			return CheckEndian()
				&& sizeof( T ) > 1;
		}

	FC_INLINE consteval Bool Buffer::CheckEndian() noexcept
		{
			return std::endian::native
				!= std::endian::little;
		}

	/*
	 @	Load	in byte order
	 */
	template< Arithmetic T >

	FC_INLINE Void Buffer::EndianLoad( T& val ) const noexcept
		{
			val = boost::endian::endian_load
					<
						T, sizeof( T ),
						boost::endian::order::little
					>
					( this->cur );
		}

	/*
	 @	Save	in byte order
	 */
	template< Arithmetic T >

	FC_INLINE Void Buffer::EndianSave( const T& val ) noexcept
		{
			boost::endian::endian_store
				<
					T, sizeof( T ),
					boost::endian::order::little
				>
				( this->cur, val );
		}
}



/**
 !	Detect access crossing boundaries
 */
namespace fce
{
	/*
	 @	Check begin
	 */
	FC_INLINE Bool Buffer::CheckBeg() const noexcept
		{
			return this->cur >= this->beg;
		}

	/*
	 @	Check end
	 */
	FC_INLINE Bool Buffer::CheckEnd() const noexcept
		{
			return this->cur <= this->end;
		}

	/*
	 @	Check both begin and end
	 */
	FC_INLINE Bool Buffer::CheckBounds() const noexcept
		{
			return this->CheckBeg()
				&& this->CheckEnd();
		}
}



/*
 @	Bin archive
 */
#define BIN_ARCHIVE( T, ... )															\
																						\
		friend ::fce::Buffer;															\
																						\
		T( const ::fce::Buffer& buffer )												\
			{																			\
				*this << buffer;														\
			}																			\
																						\
		const ::fce::Buffer& operator << ( const ::fce::Buffer& buffer )				\
			{																			\
				using Archive = ::fce::Buffer;											\
																						\
				return buffer.DeserializeGroup( __VA_ARGS__ );							\
			}																			\
																						\
		::fce::Buffer& operator >> ( ::fce::Buffer& buffer ) const NOEXCEPT				\
			{																			\
				using Archive = ::fce::Buffer;											\
																						\
				return buffer.SerializeGroup( __VA_ARGS__ );							\
			}


/*
 @	Bin derived archive
 */
#define BIN_DERIVED_ARCHIVE( P, ... )													\
																						\
		const ::fce::Buffer& operator << ( const ::fce::Buffer& buffer ) override		\
			{																			\
				using Archive = ::fce::Buffer;											\
																						\
				P::operator << ( buffer );												\
																						\
				return buffer.DeserializeGroup( __VA_ARGS__ );							\
			}																			\
																						\
		::fce::Buffer& operator >> ( ::fce::Buffer& buffer ) const NOEXCEPT override	\
			{																			\
				using Archive = ::fce::Buffer;											\
																						\
				P::operator >> ( buffer );												\
																						\
				return buffer.SerializeGroup( __VA_ARGS__ );							\
			}