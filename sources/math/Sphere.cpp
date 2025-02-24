/*
**
**	Sphere.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Box.h"


namespace fce
{
	/*
	 !	Constant
	 */
	constinit const Sphere Sphere::ZERO( 0, 0 );
	constinit const Sphere Sphere::UNIT( 0, 1 );


	/*
	 @	Compare
	 */
	Bool Sphere::operator == ( const Sphere& sphere ) const noexcept
		{
			return this->centre == sphere.centre
				&& Equal( this->radius, sphere.radius );
		}


	Bool Sphere::operator != ( const Sphere& sphere ) const noexcept
		{
			return this->centre != sphere.centre
				|| NotEqual( this->radius, sphere.radius );
		}


	/*
	 @	Intersects AABox3
	 */
	Bool Sphere::Intersects( const AABox3& aab ) const noexcept
		{
			// Arvo's algorithm
			Float s, d = 0;

			for( UInt8 i{ 0 }; i < 3; ++i )
				{
					if( this->centre[ i ] < aab.min[ i ] )
						{
							s = this->centre[ i ] - aab.min[ i ];
							d += s * s;
						}
					else if( this->centre[ i ] > aab.max[ i ] )
						{
							s = this->centre[ i ] - aab.max[ i ];
							d += s * s;
						}
				}

			return d <= math::Square( this->radius );
		}


	/*
	 @	Union sphere
	 */
	Sphere Sphere::operator | ( const Sphere& sphere ) const noexcept
		{
			// ���Ĳ� �� �뾶��
			Vector3 centreDiff = sphere.centre - this->centre;
			Float radiusDiff = sphere.radius - this->radius;

			// ���ľ����ƽ�����ɼ����ж� ��ȫ����
			Float distance2 = centreDiff.Length2();

			// ��� ���ľ��� < �뾶�����ȫ����
			if( distance2 <= math::Square( radiusDiff )  )
				{
					// ���ݰ뾶������ �ж�˭����˭
					return radiusDiff <= 0
						 ? *this : sphere;
				}

			// ���ľ���
			Float distance = math::Sqrt( distance2 );

			Float t = ( distance + radiusDiff )
					/ math::Double( distance );

			return Sphere( this->centre + centreDiff * t,
						   math::Half( distance
									 + this->radius
									 + sphere.radius ) );
		}
}