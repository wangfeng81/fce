/*
**
**	Bounding box.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Box.h"


/**
 !	AABox2
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const AABox2 AABox2::ZERO( 0, 0 );


	/*
	 @	Compare
	 */
	Bool AABox2::operator == ( const AABox2& aab2 ) const noexcept
		{
			return this->min == aab2.min
				&& this->max == aab2.max;
		}


	Bool AABox2::operator != ( const AABox2& aab2 ) const noexcept
		{
			return this->min != aab2.min
				|| this->max != aab2.max;
		}


	/*
	 @	operator *
	 */
	AABox2 AABox2::operator * ( const Affine2& aff2 ) const noexcept
		{
			Size2 halfSize( this->HalfSize() );

			Size2 newHalfSize( halfSize.AbsDot( aff2[ 0 ].xy ),
							   halfSize.AbsDot( aff2[ 1 ].xy ) );

            Vector2 newCentre( this->Centre() * aff2 );

			return AABox2( newCentre - newHalfSize,
						   newCentre + newHalfSize );
		}


	/*
	 @	Distance2
	 */
	Float AABox2::Distance2( const Vector2& point ) const noexcept
		{
			if( this->Contains( point ) )
				return 0;

			Vector2 maxDistance( 0 );

			if( point.x < this->l )
				maxDistance.x = this->l - point.x;
			else if( point.x > this->r )
				maxDistance.x = point.x - this->r;

			if( point.y < this->b )
				maxDistance.y = this->b - point.y;
			else if( point.y > this->t )
				maxDistance.y = point.y - this->t;

			return maxDistance.Length2();
		}


	/*
	 @	Centre size
	 */
	Void AABox2::CentreSize( const Vector2& centre,
							 const Size2& halfSize ) noexcept
		{
			this->min = centre - halfSize;
			this->max = centre + halfSize;
		}
}



/**
 !	AABox3
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const AABox3 AABox3::ZERO( 0, 0 );


	/*
	 @	Compare
	 */
	Bool AABox3::operator == ( const AABox3& aab3 ) const noexcept
		{
			return this->min == aab3.min
				&& this->max == aab3.max;
		}


	Bool AABox3::operator != ( const AABox3& aab3 ) const noexcept
		{
			return this->min != aab3.min
				|| this->max != aab3.max;
		}


	/*
	 @	operator *
	 */
	AABox3 AABox3::operator * ( const Affine3& aff3 ) const noexcept
		{
			Size3 halfSize( this->HalfSize() );

			// 以中心为原点，8方向半径的 最大长度
			Size3 newHalfSize( halfSize.AbsDot( aff3[ 0 ].xyz ),
							   halfSize.AbsDot( aff3[ 1 ].xyz ),
							   halfSize.AbsDot( aff3[ 2 ].xyz ) );

			// 变换 中心
            Vector3 newCentre( this->Centre() * aff3 );

			return AABox3( newCentre - newHalfSize,
						   newCentre + newHalfSize );
		}


	/*
	 @	Distance2
	 */
	Float AABox3::Distance2( const Vector3& point ) const noexcept
		{
			if( this->Contains( point ) )
				return 0;

			Vector3 maxDistance( 0 );

			if( point.x < this->l )
				maxDistance.x = this->l - point.x;
			else if( point.x > this->r )
				maxDistance.x = point.x - this->r;

			if( point.y < this->b )
				maxDistance.y = this->b - point.y;
			else if( point.y > this->t )
				maxDistance.y = point.y - this->t;

			if( point.z < this->n )
				maxDistance.z = this->n - point.z;
			else if( point.z > this->f )
				maxDistance.z = point.z - this->f;

			return maxDistance.Length2();
		}


	/*
	 @	Centre size
	 */
	Void AABox3::CentreSize( const Vector3& centre,
							 const Size3& halfSize ) noexcept
		{
			this->min = centre - halfSize;
			this->max = centre + halfSize;
		}
}