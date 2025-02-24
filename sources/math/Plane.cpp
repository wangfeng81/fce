/*
**
**	Plane.
**
		Ax + By + Cz + D = 0
		n��( p - p0 ) = 0
		n��p + d = 0

			ABC Ϊ������
			��һ��ʱ D Ϊƽ�浽ԭ��ľ���
		
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
	
			N��P + d = 0	����
			N��P + d > 0	����
			N��P + d < 0	����

			������� = 0	����
			������� > 0	����
			������� < 0	����
	 */
	Plane::SIDE Plane::Side( const Vector3& point ) const noexcept
		{
			// �������
			Float distance = this->Distance( point );

			// ���� = 0 �ཻ
			if( Equal( distance, 0 ) )
				return SIDE::INSIDE;

			// ���� != 0 ���ж�����
			return distance > 0
				 ? SIDE::POSITIVE
				 : SIDE::NEGATIVE;
		}


	/*
	 @	Side sphere
	 */
	Plane::SIDE Plane::Side( const Sphere& sphere ) const noexcept
		{
			// ���� �� �� ����
			Float distance = this->Distance( sphere.centre );

			// ���� <= �뾶	�ཻ
			if( math::Abs( distance ) <= sphere.radius )
				return SIDE::INSIDE;

			// ���� > �뾶 ���ж�����
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

			�Գ��� ����ͶӰһ���� ���þ��Ե���Ż�
	 */
	Plane::SIDE Plane::Side( const Vector3& centre,
							 const Vector3& halfSize ) const noexcept
		{
			// ���ĵ� �� �� ����
			Float distance = this->Distance( centre );

			// ���Ե�� �ҳ������� �淨�����ͶӰ����
			Float maxAbsDistance = this->normal.AbsDot( halfSize );

			// ����ڷ��� ����������� ��С��������ĸ�ֵ
			if ( distance < -maxAbsDistance )
				return SIDE::NEGATIVE;

			// ��������� ����������� �����������
			if ( distance > +maxAbsDistance )
				return SIDE::POSITIVE;

			// ��� ��������� ��������� ����ֵ֮�� ���ཻ
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
	
			light.w == 0	�����
			light.w == 1	���
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