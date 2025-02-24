/*
**
**	Triangle.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Triangle.h"


namespace fce
{
	/*
	 !	Constant
	 */
	constinit const Triangle3 Triangle3::ZERO( 0, 0, 0 );


	/*
	 @	Compare
	 */
	Bool Triangle3::operator == ( const Triangle3& triangle3 ) const noexcept
		{
			return this->a == triangle3.a
				&& this->b == triangle3.b
				&& this->c == triangle3.c;
		}

	Bool Triangle3::operator != ( const Triangle3& triangle3 ) const noexcept
		{
			return this->a != triangle3.a
				|| this->b != triangle3.b
				|| this->c != triangle3.c;
		}

	/*
	 @	Normal
	 */
	Vector3 Triangle3::Normal() const noexcept
		{
			return ( this->b - this->a )
			 .Cross( this->c - this->a )
			 .Normalized();
		}
}