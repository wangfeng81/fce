/*
**
**	Ray.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Plane.h"


namespace fce
{
	/*
	 !	Ray 3D
	 */
	struct FC_API Ray3 final
		{
			/*	Model
			*/
			Vector3 origin,
					direction;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Ray3,
							ARCHIVE_INFO( origin, "Origin" ),
							ARCHIVE_INFO( direction, "Direction" ) )

			/*	Constructor
			*/
			Ray3() = default;

			constexpr Ray3( const Vector3& origin,
							const Vector3& direction ) noexcept;

			/*	Common
			*/
			constexpr Ray3 Invert() const noexcept;

			constexpr Ray3 operator - () const noexcept;

			constexpr const Vector3 operator [] ( const Float t ) const noexcept;

			/*	Distance
			*/
			Float Distance( const Vector3& ) const;

			Float Distance2( const Vector3& ) const;

			/*	Compare
			*/
			Bool operator == ( const Ray3& ) const noexcept;

			Bool operator != ( const Ray3& ) const noexcept;

			/*	Is t valid
			*/
			constexpr Bool Valid( const Float t ) const noexcept;

			/*	Contains & Intersects
			*/
			Bool operator && ( const Vector3& ) const noexcept;

			Bool operator && ( const Plane& ) const noexcept;

			Bool operator && ( const Sphere& ) const noexcept;

			Bool operator && ( const AABox3& ) const;

			Bool Contains( const Vector3& ) const noexcept;

			Float Intersects( const Plane& ) const noexcept;

			Float Intersects( const Sphere& ) const noexcept;

			Float Intersects( const AABox3& ) const;

			Float Intersects( const Triangle3& ) const noexcept;
		};
}



/**
 !	struct Ray3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Ray3::Ray3( const Vector3& origin,
									const Vector3& direction ) noexcept
		:
			origin( origin ),
			direction( direction )
		{
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Ray3 Ray3::Invert() const noexcept
		{
			return Ray3( this->origin,
						-this->direction );
		}

	FC_INLINE constexpr Ray3 Ray3::operator - () const noexcept
		{
			return Ray3( -this->origin,
						 -this->direction );
		}

	FC_INLINE constexpr const Vector3 Ray3::operator [] ( const Float t ) const noexcept
		{
			return this->origin
				 + this->direction * t;
		}

	/*
	 @	Distance
	 */
	FC_INLINE Float Ray3::Distance( const Vector3& point ) const
		{
			return math::Sqrt( this->Distance2( point ) );
		}

	/*
	 @	Is t valid
	 */
	FC_INLINE constexpr Bool Ray3::Valid( const Float t ) const noexcept
		{
			return t >= 0;
		}

	/*
	 @	Contains & Intersects
	 */
	FC_INLINE Bool Ray3::operator && ( const Vector3& point ) const noexcept
		{
			return this->Contains( point );
		}

	FC_INLINE Bool Ray3::operator && ( const Plane& plane ) const noexcept
		{
			return this->Intersects( plane ) >= 0;
		}

	FC_INLINE Bool Ray3::operator && ( const Sphere& sphere ) const noexcept
		{
			return this->Intersects( sphere ) >= 0;
		}
}