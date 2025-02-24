/*
**
**	Image	 inlines
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce
{
	/*
	 @	Clear
	 */
	FC_INLINE Void Image::Clear() noexcept
		{
			this->buffer.Zero();
		}

	/*
	 @	Set Image
	 */
	FC_INLINE Void Image::Set( const Int16 width,
							   const Int16 height,
							   const COLOR color ) noexcept
		{
			this->Set( { width, height, color } );
		}

	/*
	 @	Buffer offset
	 */
	FC_INLINE Int32 Image::Offset( const Int16 x,
								   const Int16 y ) const noexcept
		{
			return ( this->Width() * y + x )
				 * UInt8( this->Color() );
		}

	FC_INLINE Int32 Image::Offset( const Point& pt ) const noexcept
		{
			return this->Offset( pt.x, pt.y );
		}

	/*
	 @	Get data pointer
	 */
	FC_INLINE const auto& Image::operator * () const noexcept
		{
			return this->buffer;
		}

	/*
	 @	Get pixels pointer
	 */
	template< Color_integer T >

	FC_INLINE auto* Image::Pixels( this auto& self )
		{
			return self.buffer.Data< T >();
		}

	/*
	 @	Get row pointer
	 */
	template< Color_integer T >

	FC_INLINE auto* Image::Row( this auto& self,
								const Int16 row )
		{
			return self.Pixel< T >( 0, row );
		}

	/*
	 @	Get pixel pointer
	 */
	template< Color_integer T >

	FC_INLINE auto* Image::Pixel( this auto& self,
								  const Int16 x,
								  const Int16 y )
		{
			FC_ASSERT( x >= 0 && y >= 0
					&& x < self.Width()
					&& y < self.Height() );

			auto* p
				{
					self.buffer.Data()
				  + self.Offset( x, y )
				};

			if constexpr( Const_of< decltype( self ) > )
				{
					return reinterpret_cast< const T* >( p );
				}
			else
				{
					return reinterpret_cast< T* >( p );
				}
		}
}



/**
 !	Attributes
 */
namespace fce
{
	/*
	 @	Size
	 */
	FC_INLINE const Size& Image::Size() const noexcept
		{
			return this->info.size;
		}

	FC_INLINE Int16 Image::Height() const noexcept
		{
			return this->info.height;
		}

	FC_INLINE Int16 Image::Width() const noexcept
		{
			return this->info.width;
		}

	/*
	 @	Count of pixels
	 */
	FC_INLINE Int32 Image::Area() const noexcept
		{
			return this->info.Area();
		}

	/*
	 @	Color format & depth
	 */
	FC_INLINE COLOR Image::Color() const noexcept
		{
			return this->info.color;
		}

	FC_INLINE UInt8 Image::Depth() const noexcept
		{
			return this->info.Depth();
		}

	/*
	 @	Memory size for image width
	 */
	FC_INLINE Int32 Image::Pitch() const noexcept
		{
			return this->info.Pitch();
		}

	FC_INLINE Int32 Image::Pitch( const UInt8 align ) const noexcept
		{
			return this->info.Pitch( align );
		}

	/*
	 @	Memory size
	 */
	FC_INLINE UInt Image::Capacity() const noexcept
		{
			return this->buffer.Size();
		}

	FC_INLINE UInt Image::Capacity( const UInt8 align ) const noexcept
		{
			return this->info.Capacity( align );
		}

	/*
	 @	Get Info
	 */
	FC_INLINE Image::operator const Info&() const noexcept
		{
			return this->info;
		}

	FC_INLINE const Image::Info* Image::operator -> () const noexcept
		{
			return &this->info;
		}
}