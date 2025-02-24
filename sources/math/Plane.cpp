/*
**
**	Plane.
**
		Ax + By + Cz + D = 0
		n·( p - p0 ) = 0
		n·p + d = 0

			ABC 为法向量
			归一化时 D 为平面到原点的距离
		
	Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Plane.h"


namespace fce
{
	/*
	 !	Constant
	 */
	constinit const Plane Plane::ZERO( 0 );

	constinit const Plane Plane::UNIT_X( 1, 0, 0,  0 );
	constinit const Plane Plane::UNIT_Y( 0, 1, 0,  0 );
	constinit const Plane Plane::UNIT_Z( 0, 0, 1,  0 );

	constinit const Plane Plane::UNIT_XY( 1, 1, 0,  0 );
	constinit const Plane Plane::UNIT_XZ( 1, 0, 1,  0 );
	constinit const Plane Plane::UNIT_YZ( 0, 1, 1,  0 );


	/*
	 @	Constructor
	 */
	Plane::Plane( const Vector3& normal,
				  const Vector3& point ) noexcept
		:
			Plane( normal,
				  -normal.Dot( point ) )
		{
		}


	Plane::Plane( const Vector3& point0,
				  const Vector3& point1,
				  const Vector3& point2 ) noexcept
		:
			Plane( Triangle3( point0,
							  point1,
							  point2 ) )
		{
		}


	Plane::Plane( const Triangle3& triangle3 ) noexcept
		:
			Plane( triangle3.Normal(),
				   triangle3[ 0 ] )
		{
		}


	/*
	 @	Side point
	
			N·P + d = 0	共面
			N·P + d > 0	正面
			N·P + d < 0	反面

			点面距离 = 0	共面
			点面距离 > 0	正面
			点面距离 < 0	反面
	 */
	Plane::SIDE Plane::Side( const Vector3& point ) const noexcept
		{
			// 点面距离
			Float distance = this->Distance( point );

			// 距离 = 0 相交
			if( Equal( distance, 0 ) )
				return SIDE::INSIDE;

			// 距离 != 0 再判断正反
			return distance > 0
				 ? SIDE::POSITIVE
				 : SIDE::NEGATIVE;
		}


	/*
	 @	Side sphere
	 */
	Plane::SIDE Plane::Side( const Sphere& sphere ) const noexcept
		{
			// 球心 到 面 距离
			Float distance = this->Distance( sphere.centre );

			// 距离 <= 半径	相交
			if( math::Abs( distance ) <= sphere.radius )
				return SIDE::INSIDE;

			// 距离 > 半径 再判断正反
			return distance > 0
				 ? SIDE::POSITIVE
				 : SIDE::NEGATIVE;
		}


	/*
	 @	Side AABox
	 */
	Plane::SIDE Plane::Side( const AABox3& aab ) const noexcept
		{
			Float minDot, maxDot;

			const Vector3& min = aab.min;
			const Vector3& max = aab.max;

			if( this->normal.x > 0 )
				{
					minDot = this->normal.x * min.x;
					maxDot = this->normal.x * max.x;
				}
			else
				{
					minDot = this->normal.x * max.x;
					maxDot = this->normal.x * min.x;
				}

			if( this->normal.y > 0 )
				{
					minDot += this->normal.y * min.y;
					maxDot += this->normal.y * max.y;
				}
			else
				{
					minDot += this->normal.y * max.y;
					maxDot += this->normal.y * min.y;
				}

			if( this->normal.z > 0 )
				{
					minDot += this->normal.z * min.z;
					maxDot += this->normal.z * max.z;
				}
			else
				{
					minDot += this->normal.z * max.z;
					maxDot += this->normal.z * min.z;
				}

			if( minDot >= this->d )
				return SIDE::POSITIVE;

			if( maxDot <= this->d )
				return SIDE::POSITIVE;

			return SIDE::INSIDE;
		}


	/*
	 @	Side

			对称轴 两边投影一样长 可用绝对点积优化
	 */
	Plane::SIDE Plane::Side( const Vector3& centre,
							 const Vector3& halfSize ) const noexcept
		{
			// 中心点 到 面 距离
			Float distance = this->Distance( centre );

			// 绝对点积 找出顶点在 面法线上最长投影距离
			Float maxAbsDistance = this->normal.AbsDot( halfSize );

			// 如果在反面 则中心面距离 会小于最大距离的负值
			if ( distance < -maxAbsDistance )
				return SIDE::NEGATIVE;

			// 如果在正面 则中心面距离 会大于最大距离
			if ( distance > +maxAbsDistance )
				return SIDE::POSITIVE;

			// 如果 中心面距离 在最大距离的 正负值之间 则相交
			return SIDE::INSIDE;
		}


	/*
	 @	Transform
	 */
	Plane Plane::Transform( const Matrix4& mat4 ) const
		{
			FC_ASSERT_LOG( this->isUnit2(),
				"The Plane must be normalized..." );

			Matrix4 inv;

			Bool res = mat4.Inverse( inv );

			FC_ASSERT_LOG( res,
				"The inverse matrix does not exist!" );

			return this->vector4 * inv.Transpose();
		}

	Plane Plane::Transform( const Affine3& aff3 ) const
		{
			FC_ASSERT_LOG( this->isUnit2(),
				"The Plane must be normalized..." );

			Affine3 inv;

			aff3.Inverse( inv );

			return this->vector4 * inv.Transpose();
		}


	/*
	 @	Projection
	 */
	Matrix3 Plane::Projection() const
		{
			FC_ASSERT_LOG( this->normal.isUnit2(),
						   "The Plane must be normalized..." );

			return Matrix3( 1.0f - normal.x * normal.x,
								 - normal.x * normal.y,
								 - normal.x * normal.z,

								 - normal.y * normal.x,
							1.0f - normal.y * normal.y,
								 - normal.y * normal.z,

								 - normal.z * normal.x,
								 - normal.z * normal.y,
							1.0f - normal.z * normal.z );
		}


	/*
	 @	Reflection
	 
	 		-2 * P.a * P.a + 1	-2 * P.a * P.b		-2 * P.a * P.c		-2 * P.a * P.d
	 		-2 * P.b * P.a		-2 * P.b * P.b + 1	-2 * P.b * P.c		-2 * P.b * P.d
	 		-2 * P.c * P.a		-2 * P.c * P.b		-2 * P.c * P.c + 1	-2 * P.c * P.d
	 */
	Affine3 Plane::Reflection() const
		{
			FC_ASSERT_LOG( this->normal.isUnit2(),
						   "The Plane must be normalized..." );

			Affine3 aff3;

			Vector3 normal2( -math::Double( this->normal ) );

			aff3[ 0 ] = *this * normal2.x;
			aff3[ 1 ] = *this * normal2.y;
			aff3[ 2 ] = *this * normal2.z;

			++ aff3._11;
			++ aff3._22;
			++ aff3._33;

			return aff3;
		}


	/*
	 @	Shadow
	
			P = normalize( Plane );
			L = Light;
			d = Dot( P, L )
	
				P.a * L.x + d	P.b * L.x		P.c * L.x		P.d * L.x
				P.a * L.y		P.b * L.y + d	P.c * L.y		P.d * L.y
				P.a * L.z		P.b * L.z		P.c * L.z + d	P.d * L.z
				P.a * L.w		P.b * L.w		P.c * L.w		P.d * L.w + d
	
			light.w == 0	方向光
			light.w == 1	点光
	 */
	Matrix4 Plane::Shadow( const Vector4& light ) const
		{
			FC_ASSERT_LOG( this->normal.isUnit2(),
						   "The Plane must be normalized..." );

			Matrix4 mat4;

			Float dot = this->normal.Dot( light.xyz );

			mat4[ 0 ] = *this * light.x;
			mat4[ 1 ] = *this * light.y;
			mat4[ 2 ] = *this * light.z;
			mat4[ 3 ] = *this * light.w;

			mat4._11 += dot;
			mat4._22 += dot;
			mat4._33 += dot;
			mat4._44 += dot;

			return mat4;
		}
}