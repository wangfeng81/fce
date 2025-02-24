/*
**
**	Quaternion.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Matrix.h"

namespace fce
{
	/*
	 !	Quaternion
	 */
	struct FC_API Quaternion final
		{
			/*	Model
			 */
			union
				{
					Vector3 v;
					Vector4 vector4;

					struct
						{
							Float x, y, z, w;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Quaternion, vector4 )

			XML_ARCHIVE( Quaternion, ARCHIVE_UNIT( x ),
									 ARCHIVE_UNIT( y ),
									 ARCHIVE_UNIT( z ),
									 ARCHIVE_UNIT( w ) )

			/*	Constructor
			*/
			Quaternion() = default;

			Quaternion( const Float xAngle,
						const Float yAngle,
						const Float zAngle ) noexcept;

			Quaternion( const Vector3& xAxis,
						const Vector3& yAxis,
						const Vector3& zAxis ) noexcept;

			Quaternion( const Matrix3& rot3 ) noexcept;

			Quaternion( const Vector3& angles ) noexcept;

			Quaternion( const Vector3& axis,
						const Float angle );

			constexpr Quaternion( const Float x,
								  const Float y,
								  const Float z,
								  const Float w ) noexcept;

			constexpr Quaternion( const Float w,
								  const Vector3& v ) noexcept;

			constexpr Quaternion( const Vector4& ) noexcept;

			/*	Generator
			*/
			static Quaternion AngleX( const Float angle ) noexcept;

			static Quaternion AngleY( const Float angle ) noexcept;

			static Quaternion AngleZ( const Float angle ) noexcept;

			/*	Conversion
			*/
			operator Matrix3() const noexcept;

			/*	Access
			*/
			constexpr Float& operator [] ( const UInt8 );

			constexpr const Float operator [] ( const UInt8 ) const;

			/*	Combinatorial =
			*/
			constexpr Quaternion& operator *= ( const Float ) noexcept;

			constexpr Quaternion& operator /= ( const Float ) noexcept;

			constexpr Quaternion& operator += ( const Quaternion& ) noexcept;

			constexpr Quaternion& operator -= ( const Quaternion& ) noexcept;

			constexpr Quaternion& operator *= ( const Quaternion& ) noexcept;

			/*	Operation
			*/
			constexpr Quaternion operator - () const noexcept;

			constexpr Quaternion operator * ( const Float ) const noexcept;

			constexpr Quaternion operator / ( const Float ) const noexcept;

			constexpr Quaternion operator + ( const Quaternion& ) const noexcept;

			constexpr Quaternion operator - ( const Quaternion& ) const noexcept;

			constexpr Quaternion operator * ( const Quaternion& ) const noexcept;

			Vector3 operator * ( const Vector3& ) const;

			/*	Compare
			*/
			Bool operator == ( const Quaternion& ) const noexcept;

			Bool operator != ( const Quaternion& ) const noexcept;

			Bool isUnit() const noexcept;

			Bool isUnit2() const noexcept;

			/*	Common
			*/
			Float Length() const noexcept;

			Float Length2() const noexcept;

			Float Dot( const Quaternion& ) const noexcept;

			Quaternion& Normalize() noexcept;

			Quaternion Normalized() const noexcept;

			Quaternion Conjugate() const noexcept;

			Quaternion Inverse() const noexcept;

			Quaternion UnitInverse() const;

			/*	Get
			*/
			Float AngleX() const NOEXCEPT;
			Float AngleY() const NOEXCEPT;
			Float AngleZ() const NOEXCEPT;

			Vector3 AxisX() const noexcept;
			Vector3 AxisY() const noexcept;
			Vector3 AxisZ() const noexcept;

			Matrix3 Axes() const noexcept;

			Quaternion AxisAngle() const noexcept;

			/*	Lerp
			*/
			Quaternion Exp() const noexcept;

			Quaternion Log() const noexcept;

			static Quaternion Slerp( const Float t,
									 const Quaternion& p,
									 const Quaternion& q ) noexcept;

			static Quaternion Squad( const Float t,
									 const Quaternion& p,
									 const Quaternion& a,
									 const Quaternion& b,
									 const Quaternion& q ) noexcept;

			static Quaternion Intermediate( const Quaternion& q0,
											const Quaternion& q1,
											const Quaternion& q2 );

			/*	Constant
			*/
			constinit static const Quaternion ZERO;

			constinit static const Quaternion IDENTITY;
		};
}



/**
 !	struct Quaternion
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Quaternion::Quaternion( const Float x,
												const Float y,
												const Float z,
												const Float w ) noexcept
		:
			vector4( x, y, z, w )
		{
		}

	FC_INLINE constexpr Quaternion::Quaternion( const Float w,
												const Vector3& v ) noexcept
		:
			vector4( v, w )
		{
		}

	FC_INLINE constexpr Quaternion::Quaternion( const Vector4& vec4 ) noexcept
		:
			vector4( vec4 )
		{
		}

	FC_INLINE Quaternion::Quaternion( const Float xAngle,
									  const Float yAngle,
									  const Float zAngle ) noexcept
		:
			Quaternion( Vector3( xAngle,
								 yAngle,
								 zAngle ) )
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr Float& Quaternion::operator [] ( const UInt8 idx )
		{
			return this->vector4[ idx ];
		}

	FC_INLINE constexpr const Float Quaternion::operator [] ( const UInt8 idx ) const
		{
			return this->vector4[ idx ];
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Quaternion& Quaternion::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE constexpr Quaternion& Quaternion::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE constexpr Quaternion& Quaternion::operator += ( const Quaternion& q ) noexcept
		{
			return *this = *this + q;
		}

	FC_INLINE constexpr Quaternion& Quaternion::operator -= ( const Quaternion& q ) noexcept
		{
			return *this = *this - q;
		}

	FC_INLINE constexpr Quaternion& Quaternion::operator *= ( const Quaternion& q ) noexcept
		{
			return *this = *this * q;
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Quaternion Quaternion::operator - () const noexcept
		{
			return -this->vector4;
		}

	FC_INLINE constexpr Quaternion Quaternion::operator * ( const Float f ) const noexcept
		{
			return this->vector4 * f;
		}

	FC_INLINE constexpr Quaternion Quaternion::operator / ( const Float f ) const noexcept
		{
			return this->vector4 / f;
		}

	FC_INLINE constexpr Quaternion Quaternion::operator + ( const Quaternion& q ) const noexcept
		{
			return this->vector4 + q.vector4;
		}

	FC_INLINE constexpr Quaternion Quaternion::operator - ( const Quaternion& q ) const noexcept
		{
			return this->vector4 - q.vector4;
		}

	FC_INLINE constexpr Quaternion Quaternion::operator * ( const Quaternion& q ) const noexcept
		{
			return this->vector4.Cross( q.vector4 );
		}

	/*
	 @	Compare
	 */
	FC_INLINE Bool Quaternion::operator == ( const Quaternion& q ) const noexcept
		{
			return this->vector4 == q.vector4;
		}

	FC_INLINE Bool Quaternion::operator != ( const Quaternion& q ) const noexcept
		{
			return this->vector4 != q.vector4;
		}

	FC_INLINE Bool Quaternion::isUnit() const noexcept
		{
			return this->vector4.isUnit();
		}

	FC_INLINE Bool Quaternion::isUnit2() const noexcept
		{
			return this->vector4.isUnit2();
		}

	/*
	 @	Common
	 */
	FC_INLINE Float Quaternion::Length() const noexcept
		{
			return this->vector4.Length();
		}

	FC_INLINE Float Quaternion::Length2() const noexcept
		{
			return this->vector4.Length2();
		}

	FC_INLINE Float Quaternion::Dot( const Quaternion& q ) const noexcept
		{
			return this->vector4.Dot( q.vector4 );
		}

	FC_INLINE Quaternion& Quaternion::Normalize() noexcept
		{
			return *this = this->Normalized();
		}

	FC_INLINE Quaternion Quaternion::Normalized() const noexcept
		{
			return this->vector4.Normalized();
		}

	FC_INLINE Quaternion Quaternion::Conjugate() const noexcept
		{
			return Quaternion( this->w,
							  -this->v );
		}

	FC_INLINE Quaternion Quaternion::Inverse() const noexcept
		{
			return this->Normalized().Conjugate();
		}

	FC_INLINE Quaternion Quaternion::UnitInverse() const
		{
			FC_ASSERT_LOG( this->isUnit2(),
						  "The quaternion must be normalized...");

			return this->Conjugate();
		}

	/*
	 @	Get axes
	 */
	FC_INLINE Matrix3 Quaternion::Axes() const noexcept
		{
			return Matrix3( *this ).Transpose();
		}
}