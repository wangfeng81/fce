/*
**
**	Vector 2D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Vector2
	 */
	struct FC_API Vector2 final
		{
			/*	Model
			 */
			union
				{
					Float components[ 2 ];

					struct
						{
							Float x, y;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Vector2,
							ARCHIVE_UNIT( x ),
							ARCHIVE_UNIT( y ) )

			/*	Constructor
			*/
			Vector2() = default;

			constexpr Vector2( const Float ) noexcept;

			constexpr Vector2( const Float x,
							   const Float y ) noexcept;

			/*	Access
			*/
			constexpr Float& operator [] ( const UInt8 );

			constexpr const Float operator [] ( const UInt8 ) const;

			/*	Operation
			*/
			constexpr Vector2 operator - () const noexcept;

			constexpr Vector2 operator + ( const Float ) const noexcept;

			constexpr Vector2 operator - ( const Float ) const noexcept;

			constexpr Vector2 operator * ( const Float ) const noexcept;

			constexpr Vector2 operator / ( const Float ) const noexcept;

			constexpr Vector2 operator + ( const Vector2& ) const noexcept;

			constexpr Vector2 operator - ( const Vector2& ) const noexcept;

			constexpr Vector2 operator * ( const Vector2& ) const noexcept;

			constexpr Vector2 operator / ( const Vector2& ) const noexcept;

			Vector2 operator * ( const Affine2& ) const noexcept;

			Vector2 operator * ( const Matrix2& ) const noexcept;

			Vector2 operator * ( const Matrix3& ) const noexcept;

			Vector3 operator * ( const Matrix3X2& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr Vector2& operator = ( const Float ) noexcept;

			constexpr Vector2& operator += ( const Float ) noexcept;

			constexpr Vector2& operator -= ( const Float ) noexcept;

			constexpr Vector2& operator *= ( const Float ) noexcept;

			constexpr Vector2& operator /= ( const Float ) noexcept;

			constexpr Vector2& operator += ( const Vector2& ) noexcept;

			constexpr Vector2& operator -= ( const Vector2& ) noexcept;

			constexpr Vector2& operator *= ( const Vector2& ) noexcept;

			constexpr Vector2& operator /= ( const Vector2& ) noexcept;

			Vector2& operator *= ( const Matrix2& ) noexcept;

			Vector2& operator *= ( const Affine2& ) noexcept;

			/*	Make ceil and floor
			*/
			Vector2& operator &= ( const Vector2& ) noexcept;

			Vector2& operator |= ( const Vector2& ) noexcept;

			Vector2 operator & ( const Vector2& ) const noexcept;

			Vector2 operator | ( const Vector2& ) const noexcept;

			/*	Compare
			*/
			constexpr Bool operator > ( const Vector2& ) const noexcept;

			constexpr Bool operator < ( const Vector2& ) const noexcept;

			constexpr Bool operator >= ( const Vector2& ) const noexcept;

			constexpr Bool operator <= ( const Vector2& ) const noexcept;

			Bool operator == ( const Vector2& ) const noexcept;

			Bool operator != ( const Vector2& ) const noexcept;

			/*	Is unit
			*/
			Bool isUnit() const noexcept;

			Bool isUnit2() const noexcept;

			/*	Parallel
			*/
			Bool Parallel( const Vector2& ) const noexcept;

			/*	Length
			*/
			Float Length() const noexcept;

			Float Length2() const noexcept;

			/*	Distance
			*/
			Float Distance( const Vector2& ) const noexcept;

			Float Distance2( const Vector2& ) const noexcept;

			/*	Normalize
			*/
			Vector2& Normalize() noexcept;

			Vector2 Normalized() const noexcept;

			/*	Product
			*/
			Float Dot( const Vector2& ) const noexcept;

			Float AbsDot( const Vector2& ) const noexcept;

			/*	Cross
			*/
			Float Cross( const Vector2& ) const noexcept;

			/*	Common
			*/
			constexpr Float Sum() const noexcept;

			constexpr Float Area() const noexcept;

			constexpr Float Aspect() const noexcept;

			Vector2 Abs() const noexcept;

			Vector2 Inverse( const Float = Float_ONE ) const noexcept;

			Vector2 Reflect( const Vector2& normal ) const noexcept;

			/*	Constant
			*/
			constinit static const Vector2 ONE;
			constinit static const Vector2 ZERO;
			constinit static const Vector2 UNIT_X;
			constinit static const Vector2 UNIT_Y;

			static constexpr UInt8 DIMENSION = 2;
		};
}



/**
 !	class Vector2
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Vector2::Vector2( const Float c ) noexcept
		:
			components{ c, c }
		{
		}

	FC_INLINE constexpr Vector2::Vector2( const Float x,
										  const Float y ) noexcept
		:
			components{ x, y }
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr Float& Vector2::operator [] ( const UInt8 idx )
		{
			FC_ASSERT( idx < 2 );

			return this->components[ idx ];
		}

	FC_INLINE constexpr const Float Vector2::operator [] ( const UInt8 idx ) const
		{
			FC_ASSERT( idx < 2 );

			return this->components[ idx ];
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Vector2 Vector2::operator - () const noexcept
		{
			return Vector2( -this->x,
							-this->y );
		}

	FC_INLINE constexpr Vector2 Vector2::operator + ( const Float f ) const noexcept
		{
			return Vector2( this->x + f,
							this->y + f );
		}

	FC_INLINE constexpr Vector2 Vector2::operator - ( const Float f ) const noexcept
		{
			return Vector2( this->x - f,
							this->y - f );
		}

	FC_INLINE constexpr Vector2 Vector2::operator * ( const Float f ) const noexcept
		{
			return Vector2( this->x * f,
							this->y * f );
		}

	FC_INLINE constexpr Vector2 Vector2::operator / ( const Float f ) const noexcept
		{
			return Vector2( this->x / f,
							this->y / f );
		}

	FC_INLINE constexpr Vector2 Vector2::operator + ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( this->x + vec2.x,
							this->y + vec2.y );
		}

	FC_INLINE constexpr Vector2 Vector2::operator - ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( this->x - vec2.x,
							this->y - vec2.y );
		}

	FC_INLINE constexpr Vector2 Vector2::operator * ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( this->x * vec2.x,
							this->y * vec2.y );
		}

	FC_INLINE constexpr Vector2 Vector2::operator / ( const Vector2& vec2 ) const noexcept
		{
			return Vector2( this->x / vec2.x,
							this->y / vec2.y );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Vector2& Vector2::operator = ( const Float f ) noexcept
		{
			this->x = f;
			this->y = f;

			return *this;
		}

	FC_INLINE constexpr Vector2& Vector2::operator += ( const Float f ) noexcept
		{
			return *this = *this + f;
		}

	FC_INLINE constexpr Vector2& Vector2::operator -= ( const Float f ) noexcept
		{
			return *this = *this - f;
		}

	FC_INLINE constexpr Vector2& Vector2::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE constexpr Vector2& Vector2::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE constexpr Vector2& Vector2::operator += ( const Vector2& vec2 ) noexcept
		{
			return *this = *this + vec2;
		}

	FC_INLINE constexpr Vector2& Vector2::operator -= ( const Vector2& vec2 ) noexcept
		{
			return *this = *this - vec2;
		}

	FC_INLINE constexpr Vector2& Vector2::operator *= ( const Vector2& vec2 ) noexcept
		{
			return *this = *this * vec2;
		}

	FC_INLINE constexpr Vector2& Vector2::operator /= ( const Vector2& vec2 ) noexcept
		{
			return *this = *this / vec2;
		}

	FC_INLINE Vector2& Vector2::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	FC_INLINE Vector2& Vector2::operator *= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this * aff2;
		}

	/*
	 @	Compare
	 */
	FC_INLINE constexpr Bool Vector2::operator > ( const Vector2& vec2 ) const noexcept
		{
			return this->x > vec2.x
				&& this->y > vec2.y;
		}

	FC_INLINE constexpr Bool Vector2::operator < ( const Vector2& vec2 ) const noexcept
		{
			return this->x < vec2.x
				&& this->y < vec2.y;
		}

	FC_INLINE constexpr Bool Vector2::operator >= ( const Vector2& vec2 ) const noexcept
		{
			return this->x >= vec2.x
				&& this->y >= vec2.y;
		}

	FC_INLINE constexpr Bool Vector2::operator <= ( const Vector2& vec2 ) const noexcept
		{
			return this->x <= vec2.x
				&& this->y <= vec2.y;
		}

	/*
	 @	Length
	 */
	FC_INLINE Float Vector2::Length() const noexcept
		{
			return math::Sqrt( this->Length2() );
		}

	FC_INLINE Float Vector2::Length2() const noexcept
		{
			return this->Dot( *this );
		}

	/*
	 @	Distance
	 */
	FC_INLINE Float Vector2::Distance( const Vector2& vec2 ) const noexcept
		{
			return math::Sqrt( this->Distance2( vec2 ) );
		}

	FC_INLINE Float Vector2::Distance2( const Vector2& vec2 ) const noexcept
		{
			return ( *this - vec2 ).Length2();
		}

	/*
	 @	Normalize
	 */
	FC_INLINE Vector2& Vector2::Normalize() noexcept
		{
			return *this = this->Normalized();
		}

	FC_INLINE Vector2 Vector2::Normalized() const noexcept
		{
			return *this / this->Length();
		}

	/*
	 @	Product
	 */
	FC_INLINE Float Vector2::Dot( const Vector2& vec2 ) const noexcept
		{
			return ( *this * vec2 ).Sum();
		}

	FC_INLINE Float Vector2::AbsDot( const Vector2& vec2 ) const noexcept
		{
			return ( *this * vec2 ).Abs().Sum();
		}

	/*
	 @	Cross
	 */
	FC_INLINE Float Vector2::Cross( const Vector2& vec2 ) const noexcept
		{
			return this->x * vec2.y
				 - this->y * vec2.x;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Float Vector2::Sum() const noexcept
		{
			return this->x + this->y;
		}

	FC_INLINE constexpr Float Vector2::Area() const noexcept
		{
			return this->x * this->y;
		}

	FC_INLINE constexpr Float Vector2::Aspect() const noexcept
		{
			return this->x / this->y;
		}

	FC_INLINE Vector2 Vector2::Abs() const noexcept
		{
			return Vector2( math::Abs( this->x ),
							math::Abs( this->y ) );
		}

	FC_INLINE Vector2 Vector2::Inverse( const Float f ) const noexcept
		{
			return Vector2( f / this->x,
							f / this->y );
		}
}