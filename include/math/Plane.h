/*
**
**	Plane.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Box.h"
#include "Triangle.h"


namespace fce
{
	/*
	 !	Plane
	 */
	struct FC_API Plane final
		{
			/*	Model
			 */
			union
				{
					Vector3 normal;

					Vector4 vector4;

					struct
						{
							Float a, b, c, d;
						};
				};

			/*	SIDE
			 */
			enum struct SIDE
				:
					Byte
				{
					INSIDE,
					POSITIVE,
					NEGATIVE,

					MAXIMUM
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Plane, vector4 )

			XML_ARCHIVE( Plane, ARCHIVE_UNIT( a ),
								ARCHIVE_UNIT( b ),
								ARCHIVE_UNIT( c ),
								ARCHIVE_UNIT( d ) )

			/*	Constructor
			*/
			Plane() = default;

			constexpr Plane( const Float a,
							 const Float b,
							 const Float c,
							 const Float d ) noexcept;

			constexpr Plane( const Vector4& ) noexcept;

			constexpr Plane( const Vector3& normal,
							 const Float d ) noexcept;

			Plane( const Vector3& normal,
				   const Vector3& point ) noexcept;

			Plane( const Vector3& point0,
				   const Vector3& point1,
				   const Vector3& point2 ) noexcept;

			Plane( const Triangle3& ) noexcept;

			/*	Conversion
			*/
			constexpr operator const Vector4&() const noexcept;

			/*	Is unit
			*/
			Bool isUnit() const noexcept;

			Bool isUnit2() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const Plane& ) const noexcept;

			Bool operator != ( const Plane& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr Plane& operator *= ( const Float ) noexcept;

			constexpr Plane& operator /= ( const Float ) noexcept;

			Plane& operator *= ( const Matrix4& ) noexcept;

			Plane& operator *= ( const Matrix4X3& ) noexcept;

			/*	Operation
			*/
			constexpr Plane operator - () const noexcept;

			constexpr Plane operator * ( const Float ) const noexcept;

			constexpr Plane operator / ( const Float ) const noexcept;

			Plane operator * ( const Matrix4& ) const noexcept;

			Plane operator * ( const Matrix4X3& ) const noexcept;

			/*	Contains & Intersects
			*/
			Bool operator && ( const Vector3& ) const noexcept;

			Bool operator && ( const Sphere& ) const noexcept;

			Bool operator && ( const AABox3& ) const noexcept;

			Bool Contains( const Vector3& ) const noexcept;

			Bool Intersects( const Sphere& ) const noexcept;

			Bool Intersects( const AABox3& ) const noexcept;

			/*	Normalize
			*/
			Plane& Normalize() noexcept;

			Plane Normalized() const noexcept;

			/*	Distance
			*/
			Float Distance( const Vector3& ) const;

			/*	Side
			 */
			SIDE Side( const Vector3& ) const noexcept;

			SIDE Side( const Sphere& ) const noexcept;

			SIDE Side( const AABox3& ) const noexcept;

			SIDE Side( const Vector3& centre,
					   const Vector3& halfSize ) const noexcept;

			/*	Transform
			*/
			Plane Transform( const Matrix4& ) const;

			Plane Transform( const Affine3& ) const;

			/*	Matrix
			 */
			Affine3 Reflection() const;

			Matrix3 Projection() const;

			Matrix4 Shadow( const Vector4& ) const;

			/*	Constant
			*/
			constinit static const Plane ZERO;

			constinit static const Plane UNIT_X;
			constinit static const Plane UNIT_Y;
			constinit static const Plane UNIT_Z;

			constinit static const Plane UNIT_XY;
			constinit static const Plane UNIT_XZ;
			constinit static const Plane UNIT_YZ;
		};
}



/**
 !	struct Plane
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Plane::Plane( const Float a,
									  const Float b,
									  const Float c,
									  const Float d ) noexcept
		:
			vector4( a, b, c, d )
		{
		}

	FC_INLINE constexpr Plane::Plane( const Vector4& vec4 ) noexcept
		:
			vector4( vec4 )
		{
		}

	FC_INLINE constexpr Plane::Plane( const Vector3& normal,
									  const Float d ) noexcept
		:
			vector4( normal, d )
		{
		}

	/*
	 @	Conversion
	 */
	FC_INLINE constexpr Plane::operator const Vector4&() const noexcept
		{
			return this->vector4;
		}

	/*
	 @	Is unit
	 */
	FC_INLINE Bool Plane::isUnit() const noexcept
		{
			return this->normal.isUnit();
		}

	FC_INLINE Bool Plane::isUnit2() const noexcept
		{
			return this->normal.isUnit2();
		}

	/*
	 @	Compare
	 */
	FC_INLINE Bool Plane::operator == ( const Plane& plane ) const noexcept
		{
			return this->vector4 == plane.vector4;
		}

	FC_INLINE Bool Plane::operator != ( const Plane& plane ) const noexcept
		{
			return this->vector4 != plane.vector4;
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Plane& Plane::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE constexpr Plane& Plane::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Plane& Plane::operator *= ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this * mat4;
		}

	FC_INLINE Plane& Plane::operator *= ( const Matrix4X3& mat4x3 ) noexcept
		{
			return *this = *this * mat4x3;
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Plane Plane::operator - () const noexcept
		{
			return -this->vector4;
		}

	FC_INLINE constexpr Plane Plane::operator * ( const Float f ) const noexcept
		{
			return this->vector4 * f;
		}

	FC_INLINE constexpr Plane Plane::operator / ( const Float f ) const noexcept
		{
			return this->vector4 / f;
		}

	FC_INLINE Plane Plane::operator * ( const Matrix4& mat4 ) const noexcept
		{
			return this->vector4 * mat4;
		}

	FC_INLINE Plane Plane::operator * ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return this->vector4 * mat4x3;
		}

	/*
	 @	Contains & Intersects
	 */
	FC_INLINE Bool Plane::operator && ( const Vector3& point ) const noexcept
		{
			return this->Contains( point );
		}

	FC_INLINE Bool Plane::operator && ( const Sphere& sphere ) const noexcept
		{
			return this->Intersects( sphere );
		}

	FC_INLINE Bool Plane::operator && ( const AABox3& aab ) const noexcept
		{
			return this->Intersects( aab );
		}

	FC_INLINE Bool Plane::Contains( const Vector3& point ) const noexcept
		{
			return Equal( this->Distance( point ), 0 );
		}

	FC_INLINE Bool Plane::Intersects( const Sphere& sphere ) const noexcept
		{
			Float dist = this->Distance( sphere.centre );

			return math::Abs( dist ) <= sphere.radius;
		}

	FC_INLINE Bool Plane::Intersects( const AABox3& aab ) const noexcept
		{
			 return this->Side( aab ) == SIDE::INSIDE;
		}

	/*
	 @	Common
	 */
	FC_INLINE Plane& Plane::Normalize() noexcept
		{
			return *this = this->Normalized();
		}

	FC_INLINE Plane Plane::Normalized() const noexcept
		{
			return *this / this->normal.Length();
		}

	FC_INLINE Float Plane::Distance( const Vector3& point ) const
		{
			FC_ASSERT_LOG( this->isUnit2(),
				"The Plane must be normalized..." );

			return this->normal.Dot( point )
				 + this->d;
		}
}