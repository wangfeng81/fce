/*
**
**	Axis-aligned box 2D
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce
{
	/*
	 !	AABox2
	 */
	struct FC_API AABox2 final
		{
			/*	Model
			 */
			union
				{
					struct
						{
							Vector2 min,
									max;
						};

					struct
						{
							Float l, b,
								  r, t;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( AABox2,
							ARCHIVE_UNIT( l ),
							ARCHIVE_UNIT( t ),
							ARCHIVE_UNIT( r ),
							ARCHIVE_UNIT( b ) )

			/*	Constructor
			*/
			AABox2() = default;

			constexpr AABox2( const Vector2& min,
							  const Vector2& max ) noexcept;

			constexpr AABox2( const Float l, const Float t,
							  const Float r, const Float b ) noexcept;

			/*	Set
			*/
			Void Set( const Vector2& pos,
					  const Vector2& size ) noexcept;

			Void Set( const Float x, const Float y,
					  const Float w, const Float h ) noexcept;

			/*	Common
			*/
			Void Width( const Float w ) noexcept;

			Void Height( const Float h ) noexcept;

			Void Size( const Size2& size ) noexcept;

			Void Centre( const Vector2& centre ) noexcept;

			Void CentreSize( const Vector2& centre,
							 const Size2& halfSize ) noexcept;

			constexpr Float Width() const noexcept;

			constexpr Float Height() const noexcept;

			constexpr Size2 Size() const noexcept;

			constexpr Size2 HalfSize() const noexcept;

			constexpr Float Area() const noexcept;

			constexpr Float Aspect() const noexcept;

			constexpr Vector2 Centre() const noexcept;

			Float Distance( const Vector2& ) const noexcept;

			Float Distance2( const Vector2& ) const noexcept;

			/*	Is it valid
			*/
			constexpr explicit operator Bool() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const AABox2& ) const noexcept;

			Bool operator != ( const AABox2& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr AABox2& operator += ( const Vector2& ) noexcept;

			constexpr AABox2& operator -= ( const Vector2& ) noexcept;

			constexpr AABox2& operator *= ( const Vector2& ) noexcept;

			AABox2& operator *= ( const Affine2& ) noexcept;

			/*	Transform
			*/
			constexpr AABox2 operator + ( const Vector2& ) const noexcept;

			constexpr AABox2 operator - ( const Vector2& ) const noexcept;

			constexpr AABox2 operator * ( const Vector2& ) const noexcept;

			AABox2 operator * ( const Affine2& ) const noexcept;

			/*	Contains & Intersects
			*/
			constexpr Bool operator && ( const Vector2& ) const noexcept;

			constexpr Bool operator && ( const AABox2& ) const noexcept;

			constexpr Bool Contains( const Vector2& ) const noexcept;

			constexpr Bool Contains( const AABox2& ) const noexcept;

			constexpr Bool Intersects( const AABox2& ) const noexcept;

			/*	Intersection & Union
			*/
			AABox2& operator &= ( const AABox2& ) noexcept;

			AABox2& operator |= ( const AABox2& ) noexcept;

			AABox2& operator |= ( const Vector2& ) noexcept;

			AABox2 operator & ( const AABox2& ) const noexcept;

			AABox2 operator | ( const AABox2& ) const noexcept;

			AABox2 operator | ( const Vector2& ) const noexcept;

			/*	Constant
			*/
			constinit static const AABox2 ZERO;
		};
}



/**
 !	struct AABox2
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr AABox2::AABox2( const Vector2& min,
										const Vector2& max ) noexcept
		:
			min( min ),
			max( max )
		{
		}

	FC_INLINE constexpr AABox2::AABox2( const Float l, const Float t,
										const Float r, const Float b ) noexcept
		:
			l( l ), b( b ),
			r( r ), t( t )
		{
		}

	/*
	 @	Set
	 */
	FC_INLINE Void AABox2::Set( const Vector2& pos,
								const Vector2& size ) noexcept
		{
			this->min = pos;
			this->max = pos + size;
		}

	FC_INLINE Void AABox2::Set( const Float x, const Float y,
								const Float w, const Float h ) noexcept
		{
			this->Set( { x, y },
					   { w, h } );
		}

	/*
	 @	Common
	 */
	FC_INLINE Void AABox2::Width( const Float w ) noexcept
		{
			this->r = this->l + w;
		}

	FC_INLINE Void AABox2::Height( const Float h ) noexcept
		{
			this->t = this->b + h;
		}

	FC_INLINE Void AABox2::Size( const Size2& size ) noexcept
		{
			this->max = this->min + size;
		}

	FC_INLINE Void AABox2::Centre( const Vector2& centre ) noexcept
		{
			this->CentreSize( centre,
							  this->HalfSize() );
		}

	FC_INLINE constexpr Vector2 AABox2::Centre() const noexcept
		{
			return math::Half( this->min
							 + this->max );
		}

	FC_INLINE constexpr Float AABox2::Width() const noexcept
		{
			return this->r - this->l;
		}

	FC_INLINE constexpr Float AABox2::Height() const noexcept
		{
			return this->t - this->b;
		}

	FC_INLINE constexpr Size2 AABox2::Size() const noexcept
		{
			return this->max - this->min;
		}

	FC_INLINE constexpr Size2 AABox2::HalfSize() const noexcept
		{
			return math::Half( this->Size() );
		}

	FC_INLINE constexpr Float AABox2::Area() const noexcept
		{
			return this->Size().Area();
		}

	FC_INLINE constexpr Float AABox2::Aspect() const noexcept
		{
			return this->Size().Aspect();
		}

	FC_INLINE Float AABox2::Distance( const Vector2& point ) const noexcept
		{
			return math::Sqrt( this->Distance2( point ) );
		}

	/*
	 @	Is it valid
	 */
	FC_INLINE constexpr AABox2::operator Bool() const noexcept
		{
			return this->min < this->max;
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr AABox2& AABox2::operator += ( const Vector2& offset ) noexcept
		{
			return *this = *this + offset;
		}

	FC_INLINE constexpr AABox2& AABox2::operator -= ( const Vector2& offset ) noexcept
		{
			return *this = *this - offset;
		}

	FC_INLINE constexpr AABox2& AABox2::operator *= ( const Vector2& scale ) noexcept
		{
			return *this = *this * scale;
		}

	FC_INLINE AABox2& AABox2::operator *= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this * aff2;
		}

	/*
	 @	Transform
	 */
	FC_INLINE constexpr AABox2 AABox2::operator + ( const Vector2& offset ) const noexcept
		{
			return AABox2( this->min + offset,
						   this->max + offset );
		}

	FC_INLINE constexpr AABox2 AABox2::operator - ( const Vector2& offset ) const noexcept
		{
			return AABox2( this->min - offset,
						   this->max - offset );
		}

	FC_INLINE constexpr AABox2 AABox2::operator * ( const Vector2& scale ) const noexcept
		{
			return AABox2( this->min * scale,
						   this->max * scale );
		}

	/*
	 @	Contains & Intersects
	 */
	FC_INLINE constexpr Bool AABox2::operator && ( const Vector2& point ) const noexcept
		{
			return this->Contains( point );
		}

	FC_INLINE constexpr Bool AABox2::operator && ( const AABox2& aab2 ) const noexcept
		{
			return this->Intersects( aab2 );
		}

	FC_INLINE constexpr Bool AABox2::Contains( const Vector2& point ) const noexcept
		{
			return this->min <= point
				&& this->max >= point;
		}

	FC_INLINE constexpr Bool AABox2::Contains( const AABox2& aab2 ) const noexcept
		{
			return this->min <= aab2.min
				&& this->max >= aab2.max;
		}

	FC_INLINE constexpr Bool AABox2::Intersects( const AABox2& aab2 ) const noexcept
		{
			return ! ( this->l > aab2.r
					|| this->b > aab2.t
					|| this->r < aab2.l
					|| this->t < aab2.b );
		}

	/*
	 @	Intersection & Union
	 */
	FC_INLINE AABox2& AABox2::operator &= ( const AABox2& aab2 ) noexcept
		{
			this->min |= aab2.min;
			this->max &= aab2.max;

			return *this;
		}

	FC_INLINE AABox2& AABox2::operator |= ( const AABox2& aab2 ) noexcept
		{
			this->min &= aab2.min;
			this->max |= aab2.max;

			return *this;
		}

	FC_INLINE AABox2& AABox2::operator |= ( const Vector2& point ) noexcept
		{
			this->min &= point;
			this->max |= point;

			return *this;
		}

	FC_INLINE AABox2 AABox2::operator & ( const AABox2& aab2 ) const noexcept
		{
			return AABox2( this->min | aab2.min,
						   this->max & aab2.max );
		}

	FC_INLINE AABox2 AABox2::operator | ( const AABox2& aab2 ) const noexcept
		{
			return AABox2( this->min & aab2.min,
						   this->max | aab2.max );
		}

	FC_INLINE AABox2 AABox2::operator | ( const Vector2& point ) const noexcept
		{
			return AABox2( this->min & point,
						   this->max | point );
		}
}