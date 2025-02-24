/*
**
**	Triangle.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Vector.h"


namespace fce
{
	/*
	 !	Triangle 3D
	 */
	struct FC_API Triangle3 final
		{
			/*	Model
			 */
			union
				{
					Vector3 vertices[ 3 ];

					struct
						{
							Vector3 a, b, c;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Triangle3,
							ARCHIVE_INFO( a, "V0" ),
							ARCHIVE_INFO( b, "V1" ),
							ARCHIVE_INFO( c, "V2" ) )

			/*	Constructor
			*/
			Triangle3() = default;

			constexpr Triangle3( const Vector3& a,
								 const Vector3& b,
								 const Vector3& c ) noexcept;

			/*	Access
			*/
			constexpr Vector3& operator [] ( const UInt8 );

			constexpr const Vector3& operator [] ( const UInt8 ) const;

			/*	Compare
			*/
			Bool operator == ( const Triangle3& ) const noexcept;

			Bool operator != ( const Triangle3& ) const noexcept;

			/*	Common
			*/
			Vector3 Normal() const noexcept;

			constexpr Vector3 Centre() const noexcept;

			constexpr Triangle3 Invert() const noexcept;

			/*	Constant
			*/
			constinit static const Triangle3 ZERO;
		};
}



/**
 !	struct Triangle3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Triangle3::Triangle3( const Vector3& a,
											  const Vector3& b,
											  const Vector3& c ) noexcept
		:
			vertices{ a, b, c }
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr Vector3& Triangle3::operator [] ( const UInt8 idx )
		{
			FC_ASSERT( idx < 3 );

			return this->vertices[ idx ];
		}

	FC_INLINE constexpr const Vector3& Triangle3::operator [] ( const UInt8 idx ) const
		{
			FC_ASSERT( idx < 3 );

			return this->vertices[ idx ];
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Vector3 Triangle3::Centre() const noexcept
		{
			return ( this->a
				   + this->b
				   + this->c ) / 3;
		}

	FC_INLINE constexpr Triangle3 Triangle3::Invert() const noexcept
		{
			return Triangle3( this->a,
							  this->c,
							  this->b );
		}
}