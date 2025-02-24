/*
**
**	Axis-aligned box 3D
**
		Copyright (c) WangFeng since 2000...
*/

namespace fce
{
	/*
	 !	AABox3
	 */
	struct FC_API AABox3 final
		{
			/*	Model
			 */
			union
				{
					struct
						{
							Vector3 min,
									max;
						};

					struct
						{
							Float l, b, n,
								  r, t, f;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( AABox3,
							ARCHIVE_UNIT( l ),
							ARCHIVE_UNIT( t ),
							ARCHIVE_UNIT( r ),
							ARCHIVE_UNIT( b ),
							ARCHIVE_UNIT( n ),
							ARCHIVE_UNIT( f ) )

			/*	Constructor
			*/
			AABox3() = default;

			constexpr AABox3( const Vector3& min,
							  const Vector3& max ) noexcept;

			constexpr AABox3( const Float l, const Float t,
							  const Float r, const Float b,
							  const Float n, const Float f ) noexcept;

			/*	Set
			*/
			Void Set( const Vector3& pos,
					  const Vector3& size ) noexcept;

			Void Set( const Float x, const Float y, const Float z,
					  const Float w, const Float h, const Float d ) noexcept;

			/*	Common
			*/
			Void Width( const Float w ) noexcept;

			Void Height( const Float h ) noexcept;

			Void Depth( const Float d ) noexcept;

			Void Size( const Size3& size ) noexcept;

			Void Centre( const Vector3& centre ) noexcept;

			Void CentreSize( const Vector3& centre,
							 const Size3& halfSize ) noexcept;

			constexpr Float Width() const noexcept;

			constexpr Float Height() const noexcept;

			constexpr Float Depth() const noexcept;

			constexpr Size3 Size() const noexcept;

			constexpr Size3 HalfSize() const noexcept;

			constexpr Float Volume() const noexcept;

			constexpr Vector3 Centre() const noexcept;

			Float Distance( const Vector3& ) const noexcept;

			Float Distance2( const Vector3& ) const noexcept;

			/*	Is it valid
			*/
			constexpr explicit operator Bool() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const AABox3& ) const noexcept;

			Bool operator != ( const AABox3& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr AABox3& operator += ( const Vector3& ) noexcept;

			constexpr AABox3& operator -= ( const Vector3& ) noexcept;

			constexpr AABox3& operator *= ( const Vector3& ) noexcept;

			AABox3& operator *= ( const Affine3& ) noexcept;

			/*	Transform
			*/
			constexpr AABox3 operator + ( const Vector3& ) const noexcept;

			constexpr AABox3 operator - ( const Vector3& ) const noexcept;

			constexpr AABox3 operator * ( const Vector3& ) const noexcept;

			AABox3 operator * ( const Affine3& ) const noexcept;

			/*	Contains & Intersects
			*/
			constexpr Bool operator && ( const Vector3& ) const noexcept;

			constexpr Bool operator && ( const AABox3& ) const noexcept;

			constexpr Bool Contains( const Vector3& ) const noexcept;

			constexpr Bool Contains( const AABox3& ) const noexcept;

			constexpr Bool Intersects( const AABox3& ) const noexcept;

			Bool operator && ( const Sphere& ) const noexcept;

			Bool Intersects( const Sphere& ) const noexcept;

			/*	Intersection & Union
			*/
			AABox3& operator &= ( const AABox3& ) noexcept;

			AABox3& operator |= ( const AABox3& ) noexcept;

			AABox3& operator |= ( const Vector3& ) noexcept;

			AABox3 operator & ( const AABox3& ) const noexcept;

			AABox3 operator | ( const AABox3& ) const noexcept;

			AABox3 operator | ( const Vector3& ) const noexcept;

			/*	Constant
			*/
			constinit static const AABox3 ZERO;
		};
}



/**
 !	struct AABox3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr AABox3::AABox3( const Vector3& min,
										const Vector3& max ) noexcept
		:
			min( min ),
			max( max )
		{
		}

	FC_INLINE constexpr AABox3::AABox3( const Float l, const Float t,
										const Float r, const Float b,
										const Float n, const Float f ) noexcept
		:
			l( l ), b( b ), n( n ),
			r( r ), t( t ), f( f )
		{
		}

	/*
	 @	Set
	 */
	FC_INLINE Void AABox3::Set( const Vector3& pos,
								const Vector3& size ) noexcept
		{
			this->min = pos;
			this->max = pos + size;
		}

	FC_INLINE Void AABox3::Set( const Float x, const Float y, const Float z,
								const Float w, const Float h, const Float d ) noexcept
		{
			this->Set( { x, y, z },
					   { w, h, d } );
		}


	/*
	 @	Common
	 */
	FC_INLINE Void AABox3::Width( const Float w ) noexcept
		{
			this->r = this->l + w;
		}

	FC_INLINE Void AABox3::Height( const Float h ) noexcept
		{
			this->t = this->b + h;
		}

	FC_INLINE Void AABox3::Depth( const Float d ) noexcept
		{
			this->f = this->n + d;
		}

	FC_INLINE Void AABox3::Size( const Size3& size ) noexcept
		{
			this->max = this->min + size;
		}

	FC_INLINE Void AABox3::Centre( const Vector3& centre ) noexcept
		{
			this->CentreSize( centre,
							  this->HalfSize() );
		}

	FC_INLINE constexpr Vector3 AABox3::Centre() const noexcept
		{
			return math::Half( this->min
							 + this->max );
		}

	FC_INLINE constexpr Float AABox3::Width() const noexcept
		{
			return this->r - this->l;
		}

	FC_INLINE constexpr Float AABox3::Height() const noexcept
		{
			return this->t - this->b;
		}

	FC_INLINE constexpr Float AABox3::Depth() const noexcept
		{
			return this->f - this->n;
		}

	FC_INLINE constexpr Size3 AABox3::Size() const noexcept
		{
			return this->max - this->min;
		}

	FC_INLINE constexpr Size3 AABox3::HalfSize() const noexcept
		{
			return math::Half( this->Size() );
		}

	FC_INLINE constexpr Float AABox3::Volume() const noexcept
		{
			return this->Size().Volume();
		}

	FC_INLINE Float AABox3::Distance( const Vector3& point ) const noexcept
		{
			return math::Sqrt( this->Distance2( point ) );
		}

	/*
	 @	Is it valid
	 */
	FC_INLINE constexpr AABox3::operator Bool() const noexcept
		{
			return this->min < this->max;
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr AABox3& AABox3::operator += ( const Vector3& offset ) noexcept
		{
			return *this = *this + offset;
		}

	FC_INLINE constexpr AABox3& AABox3::operator -= ( const Vector3& offset ) noexcept
		{
			return *this = *this - offset;
		}

	FC_INLINE constexpr AABox3& AABox3::operator *= ( const Vector3& scale ) noexcept
		{
			return *this = *this * scale;
		}

	FC_INLINE AABox3& AABox3::operator *= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this * aff3;
		}

	/*
	 @	Transform
	 */
	FC_INLINE constexpr AABox3 AABox3::operator + ( const Vector3& offset ) const noexcept
		{
			return AABox3( this->min + offset,
						   this->max + offset );
		}

	FC_INLINE constexpr AABox3 AABox3::operator - ( const Vector3& offset ) const noexcept
		{
			return AABox3( this->min - offset,
						   this->max - offset );
		}

	FC_INLINE constexpr AABox3 AABox3::operator * ( const Vector3& scale ) const noexcept
		{
			return AABox3( this->min * scale,
						   this->max * scale );
		}

	/*
	 @	Contains & Intersects
	 */
	FC_INLINE constexpr Bool AABox3::operator && ( const Vector3& point ) const noexcept
		{
			return this->Contains( point );
		}

	FC_INLINE constexpr Bool AABox3::operator && ( const AABox3& aab3 ) const noexcept
		{
			return this->Intersects( aab3 );
		}

	FC_INLINE constexpr Bool AABox3::Contains( const Vector3& point ) const noexcept
		{
			return this->min <= point
				&& this->max >= point;
		}

	FC_INLINE constexpr Bool AABox3::Contains( const AABox3& aab3 ) const noexcept
		{
			return this->min <= aab3.min
				&& this->max >= aab3.max;
		}

	FC_INLINE constexpr Bool AABox3::Intersects( const AABox3& aab3 ) const noexcept
		{
			return ! ( this->l > aab3.r
					|| this->b > aab3.t
					|| this->n > aab3.f
					|| this->r < aab3.l
					|| this->t < aab3.b
					|| this->f < aab3.n );
		}

	FC_INLINE Bool AABox3::operator && ( const Sphere& sphere ) const noexcept
		{
			return this->Intersects( sphere );
		}

	FC_INLINE Bool AABox3::Intersects( const Sphere& sphere ) const noexcept
		{
			return sphere.Intersects( *this );
		}

	/*
	 @	Intersection & Union
	 */
	FC_INLINE AABox3& AABox3::operator &= ( const AABox3& aab3 ) noexcept
		{
			this->min |= aab3.min;
			this->max &= aab3.max;

			return *this;
		}

	FC_INLINE AABox3& AABox3::operator |= ( const AABox3& aab3 ) noexcept
		{
			this->min &= aab3.min;
			this->max |= aab3.max;

			return *this;
		}

	FC_INLINE AABox3& AABox3::operator |= ( const Vector3& point ) noexcept
		{
			this->min &= point;
			this->max |= point;

			return *this;
		}

	FC_INLINE AABox3 AABox3::operator & ( const AABox3& aab3 ) const noexcept
		{
			return AABox3( this->min | aab3.min,
						   this->max & aab3.max );
		}

	FC_INLINE AABox3 AABox3::operator | ( const AABox3& aab3 ) const noexcept
		{
			return AABox3( this->min & aab3.min,
						   this->max | aab3.max );
		}

	FC_INLINE AABox3 AABox3::operator | ( const Vector3& point ) const noexcept
		{
			return AABox3( this->min & point,
						   this->max | point );
		}
}