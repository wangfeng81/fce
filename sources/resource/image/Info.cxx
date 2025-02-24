/*
**
**	Image::Info
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::image
{
	/*
	 @	����
	 */
	Info::Info() noexcept
		:
			value( 0 )
		{
		}

	/*
	 @	����
	 */
	Info::Info( const Info& info ) noexcept
		:
			value( info.value )
		{
		}

	/*
	 @	����
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
	 @	����
	 */
	Info::Info( const Int16 width,
				const Int16 height,
				const COLOR color ) noexcept
		:
			Info( { width, height }, color )
		{
		}

	/*
	 @	���� �����л�
	 */
	Info::Info( const Buffer& buf )
		{
			*this << buf;
		}

	/*
	 @	��� ��Ч��
	 */
	Info::operator Bool() const noexcept
		{
			return this->width > 0
				&& this->height > 0
				&& this->color > COLOR::UNKNOWN
				&& this->color < COLOR::MAXIMUM;
		}
}