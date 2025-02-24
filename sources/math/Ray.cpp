/*
**
**	Ray.
**
		p(t) = p0 + tu
		t = ( p - p0 )·u
		t[ 0, ∞ )

	Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Ray.h"


namespace fce
{
	/*
	 !	Ray 3D
	 */


	/*
	 @	Compare
	 */
	Bool Ray3::operator == ( const Ray3& ray3 ) const noexcept
		{
			return this->origin == ray3.origin
				&& this->direction == ray3.direction;
		}


	Bool Ray3::operator != ( const Ray3& ray3 ) const noexcept
		{
			return this->origin != ray3.origin
				|| this->direction != ray3.direction;
		}

	/*
	 @	Distance2
	 */
	Float Ray3::Distance2( const Vector3& point ) const
		{
			FC_ASSERT_LOG( this->direction.isUnit2(),
				"The direction must be normalized..." );

			Vector3 diff( point - this->origin );
			Float proj = diff.Dot( this->direction );

			return diff.Length2() - math::Square( proj );
		}


	/*
	 @	Contains	point
	 */
	Bool Ray3::Contains( const Vector3& point ) const noexcept
		{
			Vector3 diff( point - this->origin );
			Float t = diff.Dot( this->direction );

			// 提前退出
			if( t < 0 )
				{
					return false;
				}

			return point == ( *this )[ t ];
		}


	/*
	 @	Intersects	Plane
	 */
	Float Ray3::Intersects( const Plane& plane ) const noexcept
		{
			Float dn = this->direction.Dot( plane.normal );

			// 射线平行于面
			if( Equal( dn, 0 ) )
				{
					return -1;
				}

			Float on = this->origin.Dot( plane.normal );

			return -( on + plane.d ) / dn;
		}


	/*
	 @	Intersects	Sphere
			
			球心到射线距离，包括射线反向线情况

				distance < radius	两个交点
				distance = radius	一个切点
				distance > radius	没有交点
	 */
	Float Ray3::Intersects( const Sphere& sphere ) const noexcept
		{
			// 代入射线 球体方程
			Vector3 rs( this->origin - sphere.centre );

			Float a = this->direction.Length2();
			Float b = math::Double( rs.Dot( this->direction ) );
			Float c = rs.Length2() - math::Square( sphere.radius );

			Float d = math::Square( b ) - a * c * 4;

			// 没有交点
			if( d < 0 )
				{
					return -1;
				}

			// t = ( -b +/- sqrt( b*b + 4ac ) ) / 2a
			Float d2 = math::Sqrt( d );
			Float a2 = math::Double( a );

			Float t = -( b + d2 ) / a2;

			// 如果第一个为负值，则取第二个
			if( t < 0 )
				{
					t = ( d2 - b ) / a2;
				}

			return t;
		}


	/*
	 @	&&	AABox3
	 */
	Bool Ray3::operator &&( const AABox3& aab ) const
		{
			FC_ASSERT_LOG( aab,
						"The AABox3 is invalid..." );

			// 检查射线起点 是否在box内
			if( aab.Contains( this->origin ) )
				{
					return true;
				}

			// 检查box最近的3个面
			const Vector3& min = aab.min;
			const Vector3& max = aab.max;

			Vector3 hitPoint;

			Float t;

			// Min x
			if( this->origin.x <= min.x
			 && this->direction.x > 0 )
				{
					t = ( min.x - this->origin.x )
					  / this->direction.x;

					hitPoint = ( *this )[ t ];

					if( hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z )
						{
							return true;
						}
				}

			// Max x
			if( this->origin.x >= max.x
			 && this->direction.x < 0 )
				{
					t = ( max.x - this->origin.x )
					  / this->direction.x;

					hitPoint = ( *this )[ t ];

					if( hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z )
						{
							return true;
						}
				}

			// Min y
			if( this->origin.y <= min.y
			 && this->direction.y > 0 )
				{
					t = ( min.y - this->origin.y )
					  / this->direction.y;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z )
						{
							return true;
						}
				}

			// Max y
			if( this->origin.y >= max.y
			 && this->direction.y < 0 )
				{
					t = ( max.y - this->origin.y )
					  / this->direction.y;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z )
						{
							return true;
						}
				}

			// Min z
			if( this->origin.z <= min.z && this->direction.z > 0 )
				{
					t = ( min.z - this->origin.z )
					  / this->direction.z;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.y >= min.y
					 && hitPoint.y <= max.y )
						{
							return true;
						}
				}

			// Max z
			if( this->origin.z >= max.z && this->direction.z < 0 )
				{
					t = ( max.z - this->origin.z )
					  / this->direction.z;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.y >= min.y
					 && hitPoint.y <= max.y )
						{
							return true;
						}
				}

			return false;
		}


	/*
	 @	Intersects	AABox3
	 */
	Float Ray3::Intersects( const AABox3& aab ) const
		{
			FC_ASSERT_LOG( aab,
				"The AABox3 is invalid..." );

			// 检查射线起点 是否在box内
			if( aab.Contains( this->origin ) )
				{
					return 0;
				}

			// 检查box最近的3个面
			const Vector3& min = aab.min;
			const Vector3& max = aab.max;

			Vector3 hitPoint;

			Bool hit = false;
			Float t, tLow = 0;

			// Min x
			if( this->origin.x <= min.x
			 && this->direction.x > 0 )
				{
					t = ( min.x - this->origin.x )
					  / this->direction.x;

					hitPoint = ( *this )[ t ];

					if( hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			// Max x
			if( this->origin.x >= max.x
			 && this->direction.x < 0 )
				{
					t = ( max.x - this->origin.x )
					  / this->direction.x;

					hitPoint = ( *this )[ t ];

					if( hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			// Min y
			if( this->origin.y <= min.y
			 && this->direction.y > 0 )
				{
					t = ( min.y - this->origin.y )
					  / this->direction.y;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			// Max y
			if( this->origin.y >= max.y
			 && this->direction.y < 0 )
				{
					t = ( max.y - this->origin.y )
					  / this->direction.y;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.z >= min.z
					 && hitPoint.z <= max.z
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			// Min z
			if( this->origin.z <= min.z && this->direction.z > 0 )
				{
					t = ( min.z - this->origin.z )
					  / this->direction.z;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			// Max z
			if( this->origin.z >= max.z && this->direction.z < 0 )
				{
					t = ( max.z - this->origin.z )
					  / this->direction.z;

					hitPoint = ( *this )[ t ];

					if( hitPoint.x >= min.x
					 && hitPoint.x <= max.x
					 && hitPoint.y >= min.y
					 && hitPoint.y <= max.y
					 && ( !hit || t < tLow ) )
						{
							hit = true;
							tLow = t;
						}
				}

			return tLow;
		}


	/*
	 @	Intersects	Triangle3
	 */
	Float Ray3::Intersects( const Triangle3& triangle ) const noexcept
		{
			//// Winding must be consistent from all edges for point to be inside
			//Vector3 v1, v2;
			//Real dot[ 3 ];
			//bool zeroDot[ 3 ];

			//v1 = b - a;
			//v2 = p - a;

			//// Note we don't care about normalisation here since sign is all we need
			//// It means we don't have to worry about magnitude of cross products either
			//dot[ 0 ] = v1.Cross( v2 ).Dot( normal );
			//zeroDot[ 0 ] = math::RealEqual( dot[ 0 ], 0.0f, 1e-3 );


			//v1 = c - b;
			//v2 = p - b;

			//dot[ 1 ] = v1.Cross( v2 ).Dot( normal );
			//zeroDot[ 1 ] = math::RealEqual( dot[ 1 ], 0.0f, 1e-3 );

			//// Compare signs (ignore colinear / coincident points)
			//if( !zeroDot[ 0 ] && !zeroDot[ 1 ]
			//	&& math::Sign( dot[ 0 ] ) != math::Sign( dot[ 1 ] ) )
			//	{
			//	return false;
			//	}

			//v1 = a - c;
			//v2 = p - c;

			//dot[ 2 ] = v1.Cross( v2 ).Dot( normal );
			//zeroDot[ 2 ] = math::RealEqual( dot[ 2 ], 0.0f, 1e-3 );
			//// Compare signs (ignore colinear / coincident points)
			//if( ( !zeroDot[ 0 ] && !zeroDot[ 2 ]
			//	&& math::Sign( dot[ 0 ] ) != math::Sign( dot[ 2 ] ) ) ||
			//	( !zeroDot[ 1 ] && !zeroDot[ 2 ]
			//	&& math::Sign( dot[ 1 ] ) != math::Sign( dot[ 2 ] ) ) )
			//	{
			//	return false;
			//	}


			return true;
		}
}