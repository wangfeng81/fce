/*
**
**	Transform.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../math/Quaternion.h"


namespace fce
{
	/*
	 !	Transform
	 */
	struct FC_API Transform final
		{
			/*	Model
			*/
			Vector3	scale;

			Vector3	position;

			Quaternion orientation;

			/*	Constant
			*/
			static const Transform IDENTITY;

		public:
			/*	Combine
			*/
			Void Combine( const Vector3& pos,
						  const Vector3& scale,
						  const Quaternion& ) noexcept;

			/*
			 @	Archive
			 */
			OBJECT_ARCHIVE( Transform,
							ARCHIVE_UNIT( scale ),
							ARCHIVE_UNIT( position ),
							ARCHIVE_UNIT( orientation ) );

		public:
			/*	Angles
			*/
			[[nodiscard]] Float AngleX() const NOEXCEPT;

			[[nodiscard]] Float AngleY() const NOEXCEPT;

			[[nodiscard]] Float AngleZ() const NOEXCEPT;

			/*	Axles
			*/
			[[nodiscard]] Matrix3 Axes() const noexcept;

			[[nodiscard]] Vector3 AxisX() const noexcept;

			[[nodiscard]] Vector3 AxisY() const noexcept;

			[[nodiscard]] Vector3 AxisZ() const noexcept;

		public:
			/*	Conversion
			*/
			[[nodiscard]] Affine3 operator * () const noexcept;

			[[nodiscard]] Transform operator - () const noexcept;

			/*	Assignment
			*/
			Transform& operator = ( const Transform& ) noexcept;

			/*	operation
			*/
			Transform& operator += ( const Transform& ) noexcept;

			Transform& operator -= ( const Transform& ) noexcept;

			Transform operator + ( const Transform& ) const noexcept;

			Transform operator - ( const Transform& ) const noexcept;

		public:
			/*	Constructor & Destructor
			*/
			~Transform() = default;

			Transform() noexcept;

			Transform( const Transform& ) noexcept;

			Transform( const Vector3& pos,
					   const Vector3& scale	= Vector3::ONE,
					   const Quaternion& = Quaternion::IDENTITY ) noexcept;
		};
}



/*
 !	class Transform
 */
namespace fce
{
	/*
	 @	Get angles
	 */
	FC_INLINE Float Transform::AngleX() const NOEXCEPT
		{
			return this->orientation.AngleX();
		}

	FC_INLINE Float Transform::AngleY() const NOEXCEPT
		{
			return this->orientation.AngleY();
		}

	FC_INLINE Float Transform::AngleZ() const NOEXCEPT
		{
			return this->orientation.AngleZ();
		}

	/*
	 @	Get axles
	 */
	FC_INLINE Matrix3 Transform::Axes() const noexcept
		{
			return this->orientation.Axes();
		}

	FC_INLINE Vector3 Transform::AxisX() const noexcept
		{
			return this->orientation.AxisX();
		}

	FC_INLINE Vector3 Transform::AxisY() const noexcept
		{
			return this->orientation.AxisY();
		}

	FC_INLINE Vector3 Transform::AxisZ() const noexcept
		{
			return this->orientation.AxisZ();
		}
}



namespace fce
{
	/*
	 @	Combinatorial =
	 */
	FC_INLINE Transform& Transform::operator += ( const Transform& transform ) noexcept
		{
			return *this = *this + transform;
		}

	FC_INLINE Transform& Transform::operator -= ( const Transform& transform ) noexcept
		{
			return *this = *this - transform;
		}
}