/*
**
**	Sphere.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Matrix.h"


namespace fce
{
	/*
	 !	Sphere
	 */
	struct FC_API Sphere final
		{
			/*	Model
			*/
			Float radius;

			Vector3 centre;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Sphere,
							ARCHIVE_UNIT( radius ),
							ARCHIVE_INFO( centre, "Centre" ) )

			/*	Constructor
			*/
			Sphere() = default;

			constexpr Sphere( const Vector3& centre,
							  const Float radius ) noexcept;

			/*	Compare
			*/
			Bool operator == ( const Sphere& ) const noexcept;

			Bool operator != ( const Sphere& ) const noexcept;

			/*	Union
			*/
			Sphere& operator |= ( const Sphere& ) noexcept;

			Sphere operator | ( const Sphere& ) const noexcept;

			/*	Contains & Intersects
			*/
			Bool operator && ( const Vector3& ) const noexcept;

			Bool operator && ( const Sphere& ) const noexcept;

			Bool operator && ( const AABox3& ) const noexcept;

			Bool Contains( const Vector3& ) const noexcept;

			Bool Contains( const Sphere& ) const noexcept;

			Bool Intersects( const Sphere& ) const noexcept;

			Bool Intersects( const AABox3& ) const noexcept;

			/*	Common
			*/
			constexpr Float Volume() const noexcept;

			/*	Constant
			*/
			constinit static const Sphere ZERO;
			constinit static const Sphere UNIT;
		};
}



/**
 !	struct Sphere
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Sphere::Sphere( const Vector3& centre,
										const Float radius ) noexcept
		:
			centre( centre ),
			radius( radius )
		{
		}

	/*
	 @	Union
	 */
	FC_INLINE Sphere& Sphere::operator |= ( const Sphere& sphere ) noexcept
		{
			return *this = *this | sphere;
		}

	/*
	 @	Contains & Intersects
	 */
	FC_INLINE Bool Sphere::operator && ( const Vector3& point ) const noexcept
		{
			return this->Contains( point );
		}

	FC_INLINE Bool Sphere::operator && ( const Sphere& sphere ) const noexcept
		{
			return this->Intersects( sphere );
		}

	FC_INLINE Bool Sphere::operator && ( const AABox3& aab ) const noexcept
		{
			return this->Intersects( aab );
		}

	FC_INLINE Bool Sphere::Contains( const Vector3& point ) const noexcept
		{
			return point.Distance2( this->centre )
				<= math::Square( this->radius );
		}

	FC_INLINE Bool Sphere::Contains( const Sphere& sphere ) const noexcept
		{
			return this->centre.Distance2( sphere.centre )
				<= math::Square( this->radius - sphere.radius );
		}

	FC_INLINE Bool Sphere::Intersects( const Sphere& sphere ) const noexcept
		{
			return this->centre.Distance2( sphere.centre )
				<= math::Square( this->radius + sphere.radius );
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Float Sphere::Volume() const noexcept
		{
			return math::PI * 4
				 * math::Square( this->radius );
		}
}