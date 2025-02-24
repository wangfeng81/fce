/*
**
**	Crc32.
**
		Copyright (c) WangFeng since 2006...
*/

#include <boost/crc.hpp>

namespace fce
{
	/*
	 !	Crc32
	 */
	class Crc32 final
		{
		public:
			/*	Constructor
			*/
			Crc32() = default;

			Crc32( const auto&,
				   const auto&... ) NOEXCEPT;

			/*	Checksum
			*/
			[[nodiscard]] operator DWord() const noexcept;

			/*	Process group
			*/
			Crc32& Group( const auto&... ) NOEXCEPT;

		public:
			/*	Process composite params
			*/
			Crc32& operator() ( const Void* const, UInt ) NOEXCEPT;

			Crc32& operator() ( const Void* const beg,
								const Void* const end ) NOEXCEPT;

			Crc32& operator() ( const auto& ) NOEXCEPT;

			/*	Process single param <<
			*/
			Crc32& operator << ( const auto& ) noexcept;

			Crc32& operator << ( const Path& ) noexcept;

			Crc32& operator << ( const String& ) noexcept;

			Crc32& operator << ( const Cpp_string auto& ) noexcept;

			Crc32& operator << ( const C_string_pointer auto& ) NOEXCEPT;

			Crc32& operator << ( const Stl_tuple auto& ) NOEXCEPT;

			Crc32& operator << ( const Stl_pointer auto& ) NOEXCEPT;

			Crc32& operator << ( const Container_object auto& ) noexcept;

			template< class T >

			requires requires( const T& obj, Crc32& crc32 )
				{
					{ obj >> crc32 } -> std::same_as< Crc32& >;
				}

			Crc32& operator << ( const T& ) NOEXCEPT;

		private:
			/*	boost crc32
			*/
			boost::crc_32_type crc32;
		};
}



namespace fce
{
	/*
	 @	Constructor
	 */
	Crc32::Crc32( const auto& arg,
				  const auto&... args ) NOEXCEPT
		{
			( *this )( arg, args... );
		}

	/*
	 @	Checksum
	 */
	FC_INLINE Crc32::operator DWord() const noexcept
		{
			return this->crc32.checksum();
		}

	/*
	 @	Process	group
	 */
	FC_INLINE Crc32& Crc32::Group( const auto&... args ) NOEXCEPT
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

	/*
	 @	Process composite params
	 */
	FC_INLINE Crc32& Crc32::operator() ( const Void* const p,
											   UInt size ) NOEXCEPT
		{
			FC_ASSERT( p || !size );

			this->crc32.process_bytes( p, size );

			return *this;
		}

	FC_INLINE Crc32& Crc32::operator() ( const Void* const beg,
										 const Void* const end ) NOEXCEPT
		{
			FC_ASSERT( beg <= end );

			return ( *this )( beg, ( const Byte* )end -
								   ( const Byte* )beg );
		}

	FC_INLINE Crc32& Crc32::operator() ( const auto& obj ) NOEXCEPT
		{
			return *this << obj;
		}

	/*
	 @	Process single param <<
	 */
	FC_INLINE Crc32& Crc32::operator << ( const auto& obj ) noexcept
		{
			return ( *this )( &obj, sizeof( obj ) );
		}

	FC_INLINE Crc32& Crc32::operator << ( const Path& path ) noexcept
		{
			return *this << path.native();
		}

	FC_INLINE Crc32& Crc32::operator << ( const String& str ) noexcept
		{
			return *this << str.Native();
		}

	FC_INLINE Crc32& Crc32::operator << ( const Cpp_string auto& str ) noexcept
		{
			return ( *this )( str.data(),
							  str.size() );
		}

	FC_INLINE Crc32& Crc32::operator << ( const Stl_pointer auto& ptr ) NOEXCEPT
		{
			FC_ASSERT( ptr );

			return *this << *ptr;
		}

	FC_INLINE Crc32& Crc32::operator << ( const C_string_pointer auto& sz ) NOEXCEPT
		{
			return ( *this )( sz, String::Length( sz ) );
		}

	FC_INLINE Crc32& Crc32::operator << ( const Stl_tuple auto& tp ) NOEXCEPT
		{
			auto f = [ this ]( const auto&... args ) NOEXCEPT -> Crc32&
				{
					return this->Group( args... );
				};

			return std::apply( f, tp );
		}

	FC_INLINE Crc32& Crc32::operator << ( const Container_object auto& container ) noexcept
		{
			for( const auto& val : container )
				{
					*this << val;
				}

			return *this;
		}

	template< class T >

	requires requires( const T& obj, Crc32& crc32 )
		{
			{ obj >> crc32 } -> std::same_as< Crc32& >;
		}

	FC_INLINE Crc32& Crc32::operator << ( const T& obj ) NOEXCEPT
		{
			return obj >> *this;
		}
}



/*
 @	Archive crc32
 */
#define ARCHIVE_CRC32( ... )																		\
																									\
		friend ::fce::Crc32;																		\
																									\
		::fce::Crc32& operator >> ( ::fce::Crc32& crc32 ) const NOEXCEPT							\
			{																						\
				using Archive = ::fce::Buffer;														\
																									\
				return crc32.Group( __VA_ARGS__ );													\
			}
