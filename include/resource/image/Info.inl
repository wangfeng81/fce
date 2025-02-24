/*
**
**	Image::Info	 inlines
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::image
{
	/*
	 @	Operater =
	 */
	FC_INLINE constexpr Info& Info::operator = ( const Info& info ) noexcept
		{
			this->value = info.value;

			return *this;
		}

	/*
	 @	Pixel bit depth
	 */
	FC_INLINE UInt8 Info::Depth() const noexcept
		{
			return Color::Depth( this->color );
		}

	/*
	 @	Count of pixels
	 */
	FC_INLINE Int32 Info::Area() const noexcept
		{
			return this->size.Area();
		}

	/*
	 @	Memory size for image width
	 */
	FC_INLINE Int32 Info::Pitch() const noexcept
		{
			return this->width
				 * UInt8( this->color );
		}

	FC_INLINE Int32 Info::Pitch( const UInt8 align ) const noexcept
		{
			return Int32( MemoryPool::Align( this->Pitch(), align ) );
		}

	/*
	 @	Memory size
	 */
	FC_INLINE UInt32 Info::Capacity() const noexcept
		{
			return this->Area()
				 * UInt8( this->color );
		}

	FC_INLINE UInt32 Info::Capacity( const UInt8 align ) const noexcept
		{
			return this->Pitch( align )
				 * this->height;
		}

	/*
	 @	Serialize
	 */
	FC_INLINE Buffer& Info::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			return buf << this->value;
		}

	/*
	 @	Deserialize
	 */
	FC_INLINE const Buffer& Info::operator << ( const Buffer& buf )
		{
			return buf >> this->value;
		}
}