/*
**
**	Quaternion.
**
		a + bk+ cj + di

		共轭：反向角位移 单位四元数的逆 = 共轭

		四元数乘积的逆：等于各个四元数的逆以相反的顺序相乘

	Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Quaternion.h"


namespace fce
{
	/*
	!	Constant
	*/
	constinit const Quaternion Quaternion::ZERO( 0, 0, 0, 0 );
	constinit const Quaternion Quaternion::IDENTITY( 0, 0, 0, 1 );


	/*
	 @	From axes

			Matrix3( xAxis,
					 yAxis,
					 zAxis ).Transpose()
	 */
	Quaternion::Quaternion( const Vector3& xAxis,
							const Vector3& yAxis,
							const Vector3& zAxis ) noexcept
		:
			Quaternion( Matrix3( xAxis.x, yAxis.x, zAxis.x,
								 xAxis.y, yAxis.y, zAxis.y,
								 xAxis.z, yAxis.z, zAxis.z ) )
		{
		}


	/*
	 @	From rotation Matrix3
	 
	 		Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	 		article "Quaternion Calculus and Fast Animation".
	 */
	Quaternion::Quaternion( const Matrix3& rot3 ) noexcept
		{
			Float root;

			const Float trace = rot3._11
							  + rot3._22
							  + rot3._33;

			if( trace > 0 )
				{
					// |w| > 1/2, may as well choose w > 1/2
					root	= math::Sqrt( trace + 1 );	// 2w
					this->w	= 0.5f * root;
					root	= 0.5f / root;				// 1/(4w)

					this->v = { ( rot3._32 - rot3._23 ) * root,
								( rot3._13 - rot3._31 ) * root,
								( rot3._21 - rot3._12 ) * root };
				}
			else
				{
					// |w| <= 1/2
					constexpr UInt8 next[] = { 1, 2, 0 };

					UInt8 i = 0;

					if( rot3._22 > rot3._11 )
						i = 1;
					if( rot3._33 > rot3[ i ][ i ] )
						i = 2;

					UInt8 j = next[ i ];
					UInt8 k = next[ j ];

					root = math::Sqrt( rot3[ i ][ i ]
									 - rot3[ j ][ j ]
									 - rot3[ k ][ k ]
									 + 1 );

					( *this )[ i ] = 0.5f * root;
					root		   = 0.5f / root;

					this->w		   = ( rot3[ k ][ j ] - rot3[ j ][ k ] ) * root;
					( *this )[ j ] = ( rot3[ j ][ i ] + rot3[ i ][ j ] ) * root;
					( *this )[ k ] = ( rot3[ k ][ i ] + rot3[ i ][ k ] ) * root;
				}
		}


	/*
	 @	From axis-angle
	 
	 		axis 是单位长度
	 		q = cos( A/2 ) + sin( A/2 ) * ( x*i + y*j + z*k )
	 */
	Quaternion::Quaternion( const Vector3& axis,
							const Float angle )
		{
			FC_ASSERT_LOG( axis.isUnit2(),
							"The axis must be normalized...");

			const Float halfAngle{ math::Half( angle ) };

			*this = { math::Cos( halfAngle ),
					  axis * math::Sin( halfAngle )	};
		}


	/*
	 @	From angles xyz
	 */
	Quaternion::Quaternion( const Vector3& angles ) noexcept
		{
			const Vector3 halfAngles( math::Half( angles ) );

			const Vector3 sinA( math::Sin( halfAngles.x ),
								math::Sin( halfAngles.y ),
								math::Sin( halfAngles.z ) );

			const Vector3 cosA( math::Cos( halfAngles.x ),
								math::Cos( halfAngles.y ),
								math::Cos( halfAngles.z ) );

			*this = { -sinA.z * sinA.y * cosA.x + cosA.z * cosA.y * sinA.x,
					   sinA.z * cosA.y * sinA.x + cosA.z * sinA.y * cosA.x,
					   sinA.z * cosA.y * cosA.x - cosA.z * sinA.y * sinA.x,
					   sinA.z * sinA.y * sinA.x + cosA.z * cosA.y * cosA.x };
		}


	/*
	 @	From Angle X
	 */
	Quaternion Quaternion::AngleX( const Float angle ) noexcept
		{
			const Float halfAngleX = math::Half( angle );

			return Quaternion( math::Sin( halfAngleX ),
							   0,
							   0,
							   math::Cos( halfAngleX ) );
		}


	/*
	 @	From Angle Y
	 */
	Quaternion Quaternion::AngleY( const Float angle ) noexcept
		{
			const Float halfAngleY = math::Half( angle );

			return Quaternion( 0,
							   math::Sin( halfAngleY ),
							   0,
							   math::Cos( halfAngleY ) );
		}


	/*
	 @	From Angle Z
	 */
	Quaternion Quaternion::AngleZ( const Float angle ) noexcept
		{
			const Float halfAngleZ = math::Half( angle );

			return Quaternion( 0,
							   0,
							   math::Sin( halfAngleZ ),
							   math::Cos( halfAngleZ ) );
		}


	/*
	 @	To Matrix3
	 */
	Quaternion::operator Matrix3() const noexcept
		{
			const Vector3 d( math::Double( this->v ) );

			const Vector3 dw( d * this->w );
			const Vector3 dx( d * this->x );

			const Float dyy = d.y * this->y;
			const Float dyz = d.z * this->y;
			const Float dzz = d.z * this->z;

			return Matrix3( 1 - ( dyy + dzz ),	dx.y - dw.z,		dx.z + dw.y,
							dx.y + dw.z,		1 - ( dx.x + dzz ),	dyz - dw.x,
							dx.z - dw.y,		dyz + dw.x,			1 - ( dx.x + dyy ) );
		}


	/*
	 @	Rotate point
	 */
	Vector3 Quaternion::operator * ( const Vector3& p ) const
		{
			FC_ASSERT_LOG( this->isUnit2(),
							"The quaternion must be normalized...");

			// nVidia SDK implementation
			Vector3 uv, uuv;

			uv = this->v.Cross( p );
			uuv = this->v.Cross( uv );

			uv *= w + w;
			uuv += uuv;

			return p + uv + uuv;
		}


	/*
	 @	Get axis-angle
	 */
	Quaternion Quaternion::AxisAngle() const noexcept
		{
			const Float length = this->v.Length();

			if( Equal( length, 0 ) )
				return ZERO;

			return Quaternion( math::Double( math::ACos( this->w ) ),
							   this->v / length );
		}


	/*
	 @	Get x-axis
	 */
	Vector3 Quaternion::AxisX() const noexcept
		{
			const Float dy = math::Double( this->y );
			const Float dz = math::Double( this->z );

			const Float dyw = dy * this->w;
			const Float dzw = dz * this->w;
			const Float dyx = dy * this->x;
			const Float dzx = dz * this->x;
			const Float dyy = dy * this->y;
			const Float dzz = dz * this->z;

			return Vector3( 1 - ( dyy + dzz ),
							dyx + dzw,
							dzx - dyw );
		}


	/*
	 @	Get y-axis
	 */
	Vector3 Quaternion::AxisY() const noexcept
		{
			const Vector3 d = math::Double( this->v );

			const Float dxw = d.x * this->w;
			const Float dzw = d.z * this->w;
			const Float dxx = d.x * this->x;
			const Float dyx = d.y * this->x;
			const Float dzy = d.z * this->y;
			const Float dzz = d.z * this->z;

			return Vector3( dyx - dzw,
							1 - ( dxx + dzz ),
							dzy + dxw );
		}


	/*
	 @	Get z-axis
	 */
	Vector3 Quaternion::AxisZ() const noexcept
		{
			const Vector3 d = math::Double( this->v );

			const Float dxw = d.x * this->w;
			const Float dyw = d.y * this->w;
			const Float dxx = d.x * this->x;
			const Float dzx = d.z * this->x;
			const Float dyy = d.y * this->y;
			const Float dzy = d.z * this->y;

			return Vector3( dzx + dyw,
							dzy - dxw,
							1 - ( dxx + dyy ) );
		}


	/*
	 @	Pitch angle
	 */
	Float Quaternion::AngleX() const NOEXCEPT
		{
			FC_ASSERT_LOG( this->isUnit2(),
							"The quaternion must be normalized...");

			return math::ATan( math::Double( y * z + w * x ),
							   w * w - x * x - y * y + z * z );
		}


	/*
	 @	Yaw angle
	 */
	Float Quaternion::AngleY() const NOEXCEPT
		{
			FC_ASSERT_LOG( this->isUnit2(),
							"The quaternion must be normalized...");

			return math::ASin( - math::Double( x * z - w * y ) );
		}


	/*
	 @	Roll angle
	 */
	Float Quaternion::AngleZ() const NOEXCEPT
		{
			FC_ASSERT_LOG( this->isUnit2(),
							"The quaternion must be normalized...");

			return math::ATan( math::Double( x * y + w * z ),
							   w * w + x * x - y * y - z * z );
		}


	/*
	 @	Exp
	 */
	Quaternion Quaternion::Exp() const noexcept
		{
			const Float angle = this->v.Length();
			const Float sinA = math::Sin( angle );
			const Float cosA = math::Cos( angle );

			if( Equal( sinA, 0 ) )
				{
					return Quaternion( cosA, this->v );
				}

			const Float coeff = sinA / angle;
			return Quaternion( cosA, this->v * coeff );
		}


	/*
	 @	Log
	 */
	Quaternion Quaternion::Log() const noexcept
		{
			const Float angle = math::ACos( this->w );
			const Float sinA = math::Sin( angle );

			if( Equal( sinA, 0 ) )
				{
					return Quaternion( 0, this->v );
				}

			const Float coeff = angle / sinA;
			return Quaternion( 0, this->v * coeff );
		}


	/*
	 @	Slerp
	 */
	Quaternion Quaternion::Slerp( const Float t,
								  const Quaternion& p,
								  const Quaternion& q ) noexcept
		{
			Quaternion rk;

			Float cosA = p.Dot( q );

			if( cosA < 0 )
				{
					cosA = -cosA;
					rk = -q;
				}
			else
				{
					rk = q;
				}

			const Float angle = math::ACos( cosA );
			const Float sinA = math::Sin( angle );

			if( NotEqual( sinA, 0 ) )
				{
					Float invSinA = math::Inverse( sinA );
					Float coeff0 = math::Sin( angle * ( 1 - t ) ) * invSinA;
					Float coeff1 = math::Sin( angle * t ) * invSinA;

					rk = p * coeff0 + rk * coeff1;
				}
			else
				{
					rk = math::Lerp( p, rk, t );
				}

			return rk.Normalized();
		}


	/*
	 @	Intermediate
	 */
	Quaternion Quaternion::Intermediate( const Quaternion& q0,
										 const Quaternion& q1,
										 const Quaternion& q2 )
		{
			FC_ASSERT_LOG( q0.isUnit2() && q1.isUnit2() && q2.isUnit2(),
							"The quaternion must be normalized...");

			const Quaternion inv( q1.UnitInverse() );

			const Quaternion exp( ( ( ( inv * q0 ).Log()
									+ ( inv * q2 ).Log() )
									 * -0.25f ).Exp() );

			return q1 * exp;
		}


	/*
	 @	Squad
	 */
	Quaternion Quaternion::Squad( const Float t,
								  const Quaternion& p,
								  const Quaternion& a,
								  const Quaternion& b,
								  const Quaternion& q ) noexcept
		{
			return Slerp( ( t + t ) * ( 1 - t ),
						  Slerp( t, p, q ),
						  Slerp( t, a, b ) );
		}
}