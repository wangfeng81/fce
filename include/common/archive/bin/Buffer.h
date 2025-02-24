/*
**
**	Buffer.
**
		For variable length memory block management

		It can also be used for serialization
		compatible big & little endian

	Copyright (c) WangFeng since 1999...
*/

#include <boost/endian/conversion.hpp>



namespace fce
{
	/*
	 !	Buffer
	 */
	class Buffer final
		{
			/*	Friends
			*/
			friend Crc32;

		public:
			/*	Constructor & Destructor
			*/
			~Buffer() noexcept;

			Buffer() noexcept;

			Buffer( Buffer&& ) noexcept;

			Buffer( const Buffer& ) noexcept;

			explicit Buffer( MemoryPool& ) noexcept;

			explicit Buffer( const Path& ) noexcept;

			explicit Buffer( RESULT&,
							 const Path& ) noexcept;

			explicit Buffer( const UInt size ) noexcept;

			explicit Buffer( const Void* const,
							 const UInt size ) NOEXCEPT;

			template< class T >

			requires requires( const T& obj, Buffer& buf )
				{
					{ obj >> buf } -> std::same_as< Buffer& >;
				}

			explicit Buffer( const T& ) NOEXCEPT;

			/*	Assignment
			*/
			Buffer& operator = ( const Buffer& ) noexcept;

			Buffer& operator = ( Buffer&& ) noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Get base pointer
			*/
			template< typename T = Byte >

			[[nodiscard]] auto* Data( this auto&& ) noexcept;

			/*	Get current pointer
			*/
			[[nodiscard]] auto* operator * ( this auto&& ) noexcept;

			/*	Get current pointer by type
			*/
			template< typename T = Byte >

			[[nodiscard]] auto* Ptr( this auto&& ) noexcept;

			/*	Get last pointer
			*/
			[[nodiscard]] auto* Last( this auto&& ) noexcept;

			/*	Get value T
			*/
			template< typename T >

			[[nodiscard]] T Get() const;

			/*	Get string directly from the buffer
			*/
			template< Character T >

			[[nodiscard]] auto GetString() const;

			[[nodiscard]] auto GetStringA() const;

			[[nodiscard]] auto GetStringW() const;

			[[nodiscard]] auto GetString8() const;

			[[nodiscard]] auto GetString16() const;

			[[nodiscard]] auto GetString32() const;

			/*	Attributes
			*/
			[[nodiscard]] UInt Pos() const noexcept;

			[[nodiscard]] UInt Size() const noexcept;

			[[nodiscard]] Bool Empty() const noexcept;

			[[nodiscard]] UInt CurSize() const noexcept;

			[[nodiscard]] UInt Capacity() const noexcept;

			/*	Common
			*/
			Void Zero() noexcept;

			Void Clear() noexcept;

			Void Adjust( const UInt delta ) noexcept;

			Void Resize( const UInt size,
						 const Bool retained = false ) noexcept;

			Void Set( const Void* const,
					  const UInt size ) NOEXCEPT;

			/*	Load & Save file
			*/
			RESULT Load( const Path& ) noexcept;

			RESULT Save( const Path& ) const noexcept;

			/*	Seek	current | begin | end
			*/
			auto&& Seek( this auto&&,
						 const Integer auto offset ) NOEXCEPT;

			auto&& SeekBeg( this auto&&,
							const UInt offset = 0 ) NOEXCEPT;

			auto&& SeekEnd( this auto&&,
							const UInt offset = 0 ) NOEXCEPT;

			/*	Read & Write
			*/
			Buffer& Write( const TYPE,
						   const Buffer& ) noexcept;

			Buffer& Write( const Void* const,
						   const UInt size ) NOEXCEPT;

			Buffer& Write( std::istream&,
						   const UInt size ) NOEXCEPT;

			const Buffer& Read( const TYPE,
								Buffer& ) const;

			const Buffer& Read( Void* const,
								const UInt size ) const;

			const Buffer& Read( std::ostream&,
								const UInt size ) const;

			/*	Serialize	Buffer
			*/
			Buffer& operator << ( const Buffer& ) noexcept;

			const Buffer& operator >> ( Buffer& ) const noexcept;

			/*	Serialize	String
			*/
			Buffer& operator << ( const String& ) noexcept;

			const Buffer& operator >> ( String& ) const;

			/*	Serialize	Color_object
			*/
			Buffer& operator << ( const Color2& ) noexcept;

			Buffer& operator << ( const Color4& ) noexcept;

			Buffer& operator << ( const Color_object auto& ) noexcept;

			const Buffer& operator >> ( Color2& ) const;

			const Buffer& operator >> ( Color4& ) const;

			const Buffer& operator >> ( Color_object auto& ) const;

			/*	Serialize	std::filesystem::path
			*/
			Buffer& operator << ( const std::filesystem::path& ) noexcept;

			const Buffer& operator >> ( std::filesystem::path& ) const;

			/*	Serialize	arithmetic
			*/
			template< Arithmetic T >

			Buffer& operator << ( const T ) noexcept;

			template< Arithmetic T >

			const Buffer& operator >> ( T& ) const;

			/*	Serialize	null-terminated string
			*/
			template< Character T >

			Buffer& operator << ( const T* ) NOEXCEPT;

			template< Character T >

			const Buffer& operator >> ( T* ) const;

			/*	Serialize	Raw_object
			*/
			template< Raw_object T >

			Buffer& operator << ( const T& ) NOEXCEPT;

			template< Raw_object T >

			const Buffer& operator >> ( T& ) const;

			/*	Serialize	std::pointer
			*/
			template< Stl_pointer T >

			Buffer& operator << ( const T& ) NOEXCEPT;

			template< Stl_pointer T >

			const Buffer& operator >> ( T& ) const;

			/*	Serialize	std::basic_string
			*/
			template< typename ...Args >

			Buffer& operator << ( const std::basic_string< Args... >& ) noexcept;

			template< typename ...Args >

			const Buffer& operator >> ( std::basic_string< Args... >& ) const;

			/*	Serialize	std::basic_string_view
			*/
			template< typename ...Args >

			Buffer& operator << ( const std::basic_string_view< Args... >& ) noexcept;

			/*	Serialize	std::pair
			*/
			template< class T, class U >

			Buffer& operator << ( const std::pair< T, U >& ) NOEXCEPT;

			template< class T, class U >

			const Buffer& operator >> ( std::pair< T, U >& ) const;

			/*	Serialize	std::tuple
			*/
			template< typename ...Args >

			Buffer& operator << ( const std::tuple< Args... >& ) NOEXCEPT;

			template< typename ...Args >

			const Buffer& operator >> ( std::tuple< Args... >& ) const;

			/*	Serialize	ranges
			*/
			template< Container_object T >

			Buffer& operator << ( const T& ) NOEXCEPT;

			/*	Deserialize	array
			*/
			template< Fixed_array T >

			const Buffer& operator >> ( T& ) const;

			/*	Deserialize	T::resize
			*/
			template< Container_with_resize T >

			const Buffer& operator >> ( T& ) const;

			/*	Deserialize	T::emplace
			*/
			template< Container_with_emplace T >

			const Buffer& operator >> ( T& ) const;

			/*	Serialize	pointer object
			*/
			const Buffer& operator >> ( Archive_pointer auto& ) const;

			Buffer& operator << ( const Archive_pointer auto& ) NOEXCEPT;

			/*	Serialize	left object
			*/
			template< class T >

			requires requires( const T& obj, Buffer& buf )
				{
					{ obj >> buf } -> std::same_as< Buffer& >;
				}

			Buffer& operator << ( const T& ) NOEXCEPT;

			template< class T >

			requires requires( T& obj, const Buffer& buf )
				{
					{ obj << buf } -> std::same_as< const Buffer& >;
				}

			const Buffer& operator >> ( T& ) const;

			/*	Serialize	group
			*/
			Buffer& SerializeGroup( const auto&... ) NOEXCEPT;

			const Buffer& DeserializeGroup( auto&... ) const;

			/*	Info ( Syntax sugar )
			*/
			static auto& Info( auto&&, const auto&... ) noexcept;

		private:
			/*	Byte order	Load & Save & Check
			*/
			template< Arithmetic T >

			Void EndianLoad( T& ) const noexcept;

			template< Arithmetic T >

			Void EndianSave( const T& ) noexcept;

			template< Arithmetic T >

			static consteval Bool CheckEndian() noexcept;

			static consteval Bool CheckEndian() noexcept;

			/*	Check bounds
			*/
			Bool CheckBeg() const noexcept;

			Bool CheckEnd() const noexcept;

			Bool CheckBounds() const noexcept;

			/*	Download file
			*/
			Void Download( const Path& ) NOEXCEPT;

			/*	Free
			*/
			Void Free() noexcept;

			/*	Crc32
			*/
			Crc32& operator >> ( Crc32& ) const noexcept;

		private:
			/*	Memory pool
			*/
			MemoryPool* pool;

			/*	Pos pointer
			*/
			mutable Byte *beg, *end, *cur;
		};
}