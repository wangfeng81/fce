/*
**
**	Vector
**
		点乘：U·V = |u||v|cos<u,v>	（0°≤θ≤180°）

				1.Dot = 0		θ= 90° 直角
				2.Dot > 0		θ< 90° 锐角
				3.Dot < 0		θ> 90° 钝角
				4.Dot = +|u||v|	θ= 0°  同向共线
				5.Dot = -|u||v|	θ= 180°反向共线

				若其中一个为单位向量n
				则点积为另一个在n上的投影长度

		叉乘：|U×V| = |u||v|sin<u,v> （0°≤θ≤180°）

				c⊥u c⊥v	叉乘的模为平行四边形面积

		列向量 矩阵相乘 数据是连续的
		行向量 编码可以用连等
		所以将 列向量左乘矩阵 是一个语法上的方便

	Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Matrix.h"


/**
 !	Vector2
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Vector2 Vector2::ONE( 1 );
	constinit const Vector2 Vector2::ZERO( 0 );
	constinit const Vector2 Vector2::UNIT_X( 1, 0 );
	constinit const Vector2 Vector2::UNIT_Y( 0, 1 );


	/*
	 @	Make ceil and floor
	 */
	Vector2& Vector2::operator &= ( const Vector2& vec2 ) noexcept
		{
			if( this->x > vec2.x ) this->x = vec2.x;
			if( this->y > vec2.y ) this->y = vec2.y;

			return *this;
		}

	Vector2& Vector2::operator |= ( const Vector2& vec2 ) noexcept
		{
			if( this->x < vec2.x ) this->x = vec2.x;
			if( this->y < vec2.y ) this->y = vec2.y;

			return *this;
		}

	Vector2 Vector2::operator & ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( math::Min( this->x, vec2.x ),
							math::Min( this->y, vec2.y ) );
		}

	Vector2 Vector2::operator | ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( math::Max( this->x, vec2.x ),
							math::Max( this->y, vec2.y ) );
		}


	/*
	 @	Compare
	 */
	Bool Vector2::operator == ( const Vector2& vec2 ) const noexcept
		{
			return Equal( this->x, vec2.x )
				&& Equal( this->y, vec2.y );
		}

	Bool Vector2::operator != ( const Vector2& vec2 ) const noexcept
		{
			return NotEqual( this->x, vec2.x )
				|| NotEqual( this->y, vec2.y );
		}


	/*
	 @	Is unit
	 */
	Bool Vector2::isUnit() const noexcept
		{
			return Equal( this->Length(), 1,
						  Float_PRECISION );
		}

	Bool Vector2::isUnit2() const noexcept
		{
			return Equal( this->Length2(), 1,
						  Float_PRECISION );
		}


	/*
	 @	Parallel
	 */
	Bool Vector2::Parallel( const Vector2& vec2 ) const noexcept
		{
			return Equal( this->x * vec2.y,
						  this->y * vec2.x,
						  Float_PRECISION );
		}


	/*
	 @	Reflect
	 */
	Vector2 Vector2::Reflect( const Vector2& normal ) const noexcept
		{
			return *this - normal * math::Double
									(
										this->Dot( normal )
									);
		}


	/*
	 @	operator * Affine2
	 */
	Vector2 Vector2::operator * ( const Affine2& aff2 ) const noexcept
		{
			return Vector2( this->Dot( aff2[ 0 ].xy ) + aff2._13,
							this->Dot( aff2[ 1 ].xy ) + aff2._23 );
		}


	/*
	 @	operator * Matrix2
	 */
	Vector2 Vector2::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Vector2( this->Dot( mat2[ 0 ] ),
							this->Dot( mat2[ 1 ] ) );
		}


	/*
	 @	operator * Matrix3
	 */
	Vector2 Vector2::operator * ( const Matrix3& mat3 ) const noexcept
		{
			Float w = this->Dot( mat3[ 2 ].xy )
					+ mat3._33;

			return Vector2( ( this->Dot( mat3[ 0 ].xy ) + mat3._13 ) / w,
							( this->Dot( mat3[ 1 ].xy ) + mat3._23 ) / w );
		}


	/*
	 @	operator * Matrix3X2
	 */
	Vector3 Vector2::operator * ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return Vector3( this->Dot( mat3x2[ 0 ] ),
							this->Dot( mat3x2[ 1 ] ),
							this->Dot( mat3x2[ 2 ] ) );
		}
}



/**
 !	Vector3
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Vector3 Vector3::ONE( 1 );
	constinit const Vector3 Vector3::ZERO( 0 );
	constinit const Vector3 Vector3::UNIT_X( 1, 0, 0 );
	constinit const Vector3 Vector3::UNIT_Y( 0, 1, 0 );
	constinit const Vector3 Vector3::UNIT_Z( 0, 0, 1 );


	/*
	 @	Make ceil and floor
	 */
	Vector3& Vector3::operator &= ( const Vector3& vec3 ) noexcept
		{
			if( this->x > vec3.x ) this->x = vec3.x;
			if( this->y > vec3.y ) this->y = vec3.y;
			if( this->z > vec3.z ) this->z = vec3.z;

			return *this;
		}

	Vector3& Vector3::operator |= ( const Vector3& vec3 ) noexcept
		{
			if( this->x < vec3.x ) this->x = vec3.x;
			if( this->y < vec3.y ) this->y = vec3.y;
			if( this->z < vec3.z ) this->z = vec3.z;

			return *this;
		}

	Vector3 Vector3::operator & ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( math::Min( this->x, vec3.x ),
							math::Min( this->y, vec3.y ),
							math::Min( this->z, vec3.z ) );
		}

	Vector3 Vector3::operator | ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( math::Max( this->x, vec3.x ),
							math::Max( this->y, vec3.y ),
							math::Max( this->z, vec3.z ) );
		}


	/*
	 @	Compare
	 */
	Bool Vector3::operator == ( const Vector3& vec3 ) const noexcept
		{
			return Equal( this->x, vec3.x )
				&& Equal( this->y, vec3.y )
				&& Equal( this->z, vec3.z );
		}

	Bool Vector3::operator != ( const Vector3& vec3 ) const noexcept
		{
			return NotEqual( this->x, vec3.x )
				|| NotEqual( this->y, vec3.y )
				|| NotEqual( this->z, vec3.z );
		}


	/*
	 @	Is unit
	 */
	Bool Vector3::isUnit() const noexcept
		{
			return Equal( this->Length(), 1,
						  Float_PRECISION );
		}

	Bool Vector3::isUnit2() const noexcept
		{
			return Equal( this->Length2(), 1,
						  Float_PRECISION );
		}

	/*
	 @	Parallel
	 */
	Bool Vector3::Parallel( const Vector3& vec3 ) const noexcept
		{
			return Equal( this->x * vec3.y,
						  this->y * vec3.x,
						  Float_PRECISION )

				&& Equal( this->x * vec3.z,
						  this->z * vec3.x,
						  Float_PRECISION );
		}


	/*
	 @	Reflect
	 */
	Vector3 Vector3::Reflect( const Vector3& normal ) const noexcept
		{
			return *this - normal * math::Double
									(
										this->Dot( normal )
									);
		}


	/*
	 @	operator * Affine3
	 */
	Vector3 Vector3::operator * ( const Affine3& aff3 ) const noexcept
		{
			return Vector3( this->Dot( aff3[ 0 ].xyz ) + aff3._14,
							this->Dot( aff3[ 1 ].xyz ) + aff3._24,
							this->Dot( aff3[ 2 ].xyz ) + aff3._34 );
		}


	/*
	 @	operator * Matrix3
	 */
	Vector3 Vector3::operator * ( const Matrix3& mat3 ) const noexcept
		{
			return Vector3( this->Dot( mat3[ 0 ] ),
							this->Dot( mat3[ 1 ] ),
							this->Dot( mat3[ 2 ] ) );
		}


	/*
	 @	operator * Matrix4
	 */
	Vector3 Vector3::operator * ( const Matrix4& mat4 ) const noexcept
		{
			Float invW = math::Inverse
						(
							this->Dot( mat4[ 3 ].xyz ) + mat4._44
						);

			return Vector3( ( this->Dot( mat4[ 0 ].xyz ) + mat4._14 ) * invW,
							( this->Dot( mat4[ 1 ].xyz ) + mat4._24 ) * invW,
							( this->Dot( mat4[ 2 ].xyz ) + mat4._34 ) * invW );
		}


	/*
	 @	operator * Matrix3X2
	 */
	Vector3 Vector3::operator * ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return Vector3( mat3x2[ 0 ].Dot( this->xy ),
							mat3x2[ 1 ].Dot( this->xy ),
							mat3x2[ 2 ].Dot( this->xy ) + this->w );
		}


	/*
	 @	operator * Matrix4X3
	 */
	Vector4 Vector3::operator * ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return Vector4( this->Dot( mat4x3[ 0 ] ),
							this->Dot( mat4x3[ 1 ] ),
							this->Dot( mat4x3[ 2 ] ),
							this->Dot( mat4x3[ 3 ] ) );
		}


	/*
	 @	Cross
	 */
	Vector3 Vector3::Cross( const Vector3& vec3 ) const noexcept
		{
			return Vector3( this->y * vec3.z - this->z * vec3.y,
							this->z * vec3.x - this->x * vec3.z,
							this->x * vec3.y - this->y * vec3.x );
		}
}



/**
 !	Vector4
 */
namespace fce
{
	/*
	 @	Compare
	 */
	Bool Vector4::operator == ( const Vector4& vec4 ) const noexcept
		{
			return Equal( this->x, vec4.x )
				&& Equal( this->y, vec4.y )
				&& Equal( this->z, vec4.z )
				&& Equal( this->w, vec4.w );
		}

	Bool Vector4::operator != ( const Vector4& vec4 ) const noexcept
		{
			return NotEqual( this->x, vec4.x )
				|| NotEqual( this->y, vec4.y )
				|| NotEqual( this->z, vec4.z )
				|| NotEqual( this->w, vec4.w );
		}


	/*
	 @	Is unit
	 */
	Bool Vector4::isUnit() const noexcept
		{
			return Equal( this->Length(), 1,
						  Float_PRECISION );
		}

	Bool Vector4::isUnit2() const noexcept
		{
			return Equal( this->Length2(), 1,
						  Float_PRECISION );
		}


	/*
	 @	operator * Affine3
	 */
	Vector4 Vector4::operator * ( const Affine3& aff3 ) const noexcept
		{
			return Vector4( this->Dot( aff3[ 0 ] ),
							this->Dot( aff3[ 1 ] ),
							this->Dot( aff3[ 2 ] ),
							this->w );
		}


	/*
	 @	operator * Affine3
	 */
	Vector4 Vector4::operator * ( const Matrix4& mat4 ) const noexcept
		{
			return Vector4( this->Dot( mat4[ 0 ] ),
							this->Dot( mat4[ 1 ] ),
							this->Dot( mat4[ 2 ] ),
							this->Dot( mat4[ 3 ] ) );
		}


	/*
	 @	operator * Matrix4X3
	 */
	Vector4 Vector4::operator * ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return Vector4( mat4x3[ 0 ].Dot( this->xyz ),
							mat4x3[ 1 ].Dot( this->xyz ),
							mat4x3[ 2 ].Dot( this->xyz ),
							mat4x3[ 3 ].Dot( this->xyz ) + this->w );
		}


	/*
	 @	Cross
	 */
	Vector4 Vector4::Cross( const Vector4& vec4 ) const noexcept
		{
			return Vector4( this->w * vec4.x + this->x * vec4.w + this->y * vec4.z - this->z * vec4.y,
							this->w * vec4.y - this->x * vec4.z + this->y * vec4.w + this->z * vec4.x,
							this->w * vec4.z + this->x * vec4.y - this->y * vec4.x + this->z * vec4.w,
							this->w * vec4.w - this->x * vec4.x - this->y * vec4.y - this->z * vec4.z );
		}
}