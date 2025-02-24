/*
**
**	Image::Info
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::image
{
	/*
	 @	构造
	 */
	Info::Info() noexcept
		:
			value( 0 )
		{
		}

	/*
	 @	构造
	 */
	Info::Info( const Info& info ) noexcept
		:
			value( info.value )
		{
		}

	/*
	 @	构造
	 */
	Info::Info( const Size size,
				const COLOR color ) noexcept
		:
			size( size ),
			color( color ),
			reserves{}
		{
		}

	/*
	 @	构造
	 */
	Info::Info( const Int16 width,
				const Int16 height,
				const COLOR color ) noexcept
		:
			Info( { width, height }, color )
		{
		}

	/*
	 @	构造 反序列化
	 */
	Info::Info( const Buffer& buf )
		{
			*this << buf;
		}

	/*
	 @	检查 有效性
	 */
	Info::operator Bool() const noexcept
		{
			return this->width > 0
				&& this->height > 0
				&& this->color > COLOR::UNKNOWN
				&& this->color < COLOR::MAXIMUM;
		}
}