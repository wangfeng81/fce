/*
**
**	Vector 3D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Vector3
	 */
	struct FC_API Vector3 final
		{
			/*	Model
			 */
			union
				{
					Float components[ 3 ];

					struct
						{
							Float x, y, z;
						};

					struct
						{
							Vector2 xy; Float w;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Vector3,
							ARCHIVE_UNIT( x ),
							ARCHIVE_UNIT( y ),
							ARCHIVE_UNIT( z ) )

				/*	Constructor
			*/
			Vector3() = default;

			constexpr Vector3( const Float ) noexcept;

			constexpr Vector3( const Float x,
							   const Float y,
							   const Float z ) noexcept;

			constexpr Vector3( const Vector2&,
							   const Float z ) noexcept;

			/*	Access
			*/
			constexpr operator Vector2() const noexcept;

			constexpr Float& operator [] ( const UInt8 );

			constexpr const Float operator [] ( const UInt8 ) const;

			/*	Operation
			*/
			constexpr Vector3 operator - () const noexcept;

			constexpr Vector3 operator + ( const Float ) const noexcept;

			constexpr Vector3 operator - ( const Float ) const noexcept;

			constexpr Vector3 operator * ( const Float ) const noexcept;

			constexpr Vector3 operator / ( const Float ) const noexcept;

			constexpr Vector3 operator + ( const Vector3& ) const noexcept;

			constexpr Vector3 operator - ( const Vector3& ) const noexcept;

			constexpr Vector3 operator * ( const Vector3& ) const noexcept;

			constexpr Vector3 operator / ( const Vector3& ) const noexcept;

			Vector3 operator * ( const Affine3& ) const noexcept;

			Vector3 operator * ( const Matrix3& ) const noexcept;

			Vector3 operator * ( const Matrix4& ) const noexcept;

			Vector3 operator * ( const Matrix3X2& ) const noexcept;

			Vector4 operator * ( const Matrix4X3& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr Vector3& operator = ( const Float ) noexcept;

			constexpr Vector3& operator += ( const Float ) noexcept;

			constexpr Vector3& operator -= ( const Float ) noexcept;

			constexpr Vector3& operator *= ( const Float ) noexcept;

			constexpr Vector3& operator /= ( const Float ) noexcept;

			constexpr Vector3& operator += ( const Vector3& ) noexcept;

			constexpr Vector3& operator -= ( const Vector3& ) noexcept;

			constexpr Vector3& operator *= ( const Vector3& ) noexcept;

			constexpr Vector3& operator /= ( const Vector3& ) noexcept;

			Vector3& operator *= ( const Affine3& ) noexcept;

			Vector3& operator *= ( const Matrix3& ) noexcept;

			Vector3& operator *= ( const Matrix4& ) noexcept;

			/*	Make ceil and floor
			*/
			Vector3& operator &= ( const Vector3& ) noexcept;

			Vector3& operator |= ( const Vector3& ) noexcept;

			Vector3 operator & ( const Vector3& ) const noexcept;

			Vector3 operator | ( const Vector3& ) const noexcept;

			/*	Compare
			*/
			constexpr Bool operator > ( const Vector3& ) const noexcept;

			constexpr Bool operator < ( const Vector3& ) const noexcept;

			constexpr Bool operator >= ( const Vector3& ) const noexcept;

			constexpr Bool operator <= ( const Vector3& ) const noexcept;

			Bool operator == ( const Vector3& ) const noexcept;

			Bool operator != ( const Vector3& ) const noexcept;

			/*	Is unit
			*/
			Bool isUnit() const noexcept;

			Bool isUnit2() const noexcept;

			/*	Parallel
			*/
			Bool Parallel( const Vector3& ) const noexcept;

			/*	Length
			*/
			Float Length() const noexcept;

			Float Length2() const noexcept;

			/*	Distance
			*/
			Float Distance( const Vector3& ) const noexcept;

			Float Distance2( const Vector3& ) const noexcept;

			/*	Normalize
			*/
			Vector3& Normalize() noexcept;

			Vector3 Normalized() const noexcept;

			/*	Homogenize
			*/
			Vector3& Homogenize() noexcept;

			Vector3 Homogenized() const noexcept;

			/*	Product
			*/
			Float Dot( const Vector3& ) const noexcept;

			Float AbsDot( const Vector3& ) const noexcept;

			Vector3 Cross( const Vector3& ) const noexcept;

			/*	Common
			*/
			constexpr Float Sum() const noexcept;

			constexpr Float Volume() const noexcept;

			Vector3 Abs() const noexcept;

			Vector3 Inverse( const Float = Float_ONE ) const noexcept;

			Vector3 Reflect( const Vector3& normal ) const noexcept;

			/*	Constant
			*/
			constinit static const Vector3 ONE;
			constinit static const Vector3 ZERO;
			constinit static const Vector3 UNIT_X;
			constinit static const Vector3 UNIT_Y;
			constinit static const Vector3 UNIT_Z;

			static constexpr UInt8 DIMENSION = 3;
		};
}



/**
 !	class Vector3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Vector3::Vector3( const Float c ) noexcept
		:
			components{ c, c, c }
		{
		}

	FC_INLINE constexpr Vector3::Vector3( const Float x,
										  const Float y,
										  const Float z ) noexcept
		:
			components{ x, y, z }
		{
		}

	FC_INLINE constexpr Vector3::Vector3( const Vector2& vec2,
										  const Float z ) noexcept
		:
			components{ vec2.x, vec2.y, z }
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr Vector3::operator Vector2() const noexcept
		{
			return this->xy / this->w;
		}

	FC_INLINE constexpr Float& Vector3::operator [] ( const UInt8 idx )
		{
			FC_ASSERT( idx < 3 );

			return this->components[ idx ];
		}

	FC_INLINE constexpr const Float Vector3::operator [] ( const UInt8 idx ) const
		{
			FC_ASSERT( idx < 3 );

			return this->components[ idx ];
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Vector3 Vector3::operator - () const noexcept
		{
			return Vector3( -this->x,
							-this->y,
							-this->z );
		}

	FC_INLINE constexpr Vector3 Vector3::operator + ( const Float f ) const noexcept
		{
			return Vector3( this->x + f,
							this->y + f,
							this->z + f );
		}

	FC_INLINE constexpr Vector3 Vector3::operator - ( const Float f ) const noexcept
		{
			return Vector3( this->x - f,
							this->y - f,
							this->z - f );
		}

	FC_INLINE constexpr Vector3 Vector3::operator * ( const Float f ) const noexcept
		{
			return Vector3( this->x * f,
							this->y * f,
							this->z * f );
		}

	FC_INLINE constexpr Vector3 Vector3::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	FC_INLINE constexpr Vector3 Vector3::operator + ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( this->x + vec3.x,
							this->y + vec3.y,
							this->z + vec3.z );
		}

	FC_INLINE constexpr Vector3 Vector3::operator - ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( this->x - vec3.x,
							this->y - vec3.y,
							this->z - vec3.z );
		}

	FC_INLINE constexpr Vector3 Vector3::operator * ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( this->x * vec3.x,
							this->y * vec3.y,
							this->z * vec3.z );
		}

	FC_INLINE constexpr Vector3 Vector3::operator / ( const Vector3& vec3 ) const noexcept
		{
			return Vector3( this->x / vec3.x,
							this->y / vec3.y,
							this->z / vec3.z );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Vector3& Vector3::operator = ( const Float f ) noexcept
		{
			this->x = f;
			this->y = f;
			this->z = f;

			return *this;
		}

	FC_INLINE constexpr Vector3& Vector3::operator += ( const Float f ) noexcept
		{
			return *this = *this + f;
		}

	FC_INLINE constexpr Vector3& Vector3::operator -= ( const Float f ) noexcept
		{
			return *this = *this - f;
		}

	FC_INLINE constexpr Vector3& Vector3::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE constexpr Vector3& Vector3::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE constexpr Vector3& Vector3::operator += ( const Vector3& vec3 ) noexcept
		{
			return *this = *this + vec3;
		}

	FC_INLINE constexpr Vector3& Vector3::operator -= ( const Vector3& vec3 ) noexcept
		{
			return *this = *this - vec3;
		}

	FC_INLINE constexpr Vector3& Vector3::operator *= ( const Vector3& vec3 ) noexcept
		{
			return *this = *this * vec3;
		}

	FC_INLINE constexpr Vector3& Vector3::operator /= ( const Vector3& vec3 ) noexcept
		{
			return *this = *this / vec3;
		}

	FC_INLINE Vector3& Vector3::operator *= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this * aff3;
		}

	FC_INLINE Vector3& Vector3::operator *= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this * mat3;
		}

	FC_INLINE Vector3& Vector3::operator *= ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this * mat4;
		}

	/*
	 @	Compare
	 */
	FC_INLINE constexpr Bool Vector3::operator > ( const Vector3& vec3 ) const noexcept
		{
			return this->x > vec3.x
				&& this->y > vec3.y
				&& this->z > vec3.z;
		}

	FC_INLINE constexpr Bool Vector3::operator < ( const Vector3& vec3 ) const noexcept
		{
			return this->x < vec3.x
				&& this->y < vec3.y
				&& this->z < vec3.z;
		}

	FC_INLINE constexpr Bool Vector3::operator >= ( const Vector3& vec3 ) const noexcept
		{
			return this->x >= vec3.x
				&& this->y >= vec3.y
				&& this->z >= vec3.z;
		}

	FC_INLINE constexpr Bool Vector3::operator <= ( const Vector3& vec3 ) const noexcept
		{
			return this->x <= vec3.x
				&& this->y <= vec3.y
				&& this->z <= vec3.z;
		}

	/*
	 @	Length
	 */
	FC_INLINE Float Vector3::Length() const noexcept
		{
			return math::Sqrt( this->Length2() );
		}

	FC_INLINE Float Vector3::Length2() const noexcept
		{
			return this->Dot( *this );
		}

	/*
	 @	Distance
	 */
	FC_INLINE Float Vector3::Distance( const Vector3& vec3 ) const noexcept
		{
			return math::Sqrt( this->Distance2( vec3 ) );
		}

	FC_INLINE Float Vector3::Distance2( const Vector3& vec3 ) const noexcept
		{
			return ( *this - vec3 ).Length2();
		}

	/*
	 @	Normalize
	 */
	FC_INLINE Vector3& Vector3::Normalize() noexcept
		{
			return *this = this->Normalized();
		}

	FC_INLINE Vector3 Vector3::Normalized() const noexcept
		{
			return *this / this->Length();
		}

	/*
	 @	Homogenize
	 */
	FC_INLINE Vector3& Vector3::Homogenize() noexcept
		{
			return *this = this->Homogenized();
		}

	FC_INLINE Vector3 Vector3::Homogenized() const noexcept
		{
			return Vector3( *this, 1 );
		}

	/*
	 @	Product
	 */
	FC_INLINE Float Vector3::Dot( const Vector3& vec3 ) const noexcept
		{
			return ( *this * vec3 ).Sum();
		}

	FC_INLINE Float Vector3::AbsDot( const Vector3& vec3 ) const noexcept
		{
			return ( *this * vec3 ).Abs().Sum();
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Float Vector3::Sum() const noexcept
		{
			return this->x + this->y + this->z;
		}

	FC_INLINE constexpr Float Vector3::Volume() const noexcept
		{
			return this->x * this->y * this->z;
		}

	FC_INLINE Vector3 Vector3::Abs() const noexcept
		{
			return Vector3( math::Abs( this->x ),
							math::Abs( this->y ),
							math::Abs( this->z ) );
		}

	FC_INLINE Vector3 Vector3::Inverse( const Float f ) const noexcept
		{
			return Vector3( f / this->x,
							f / this->y,
							f / this->z );
		}
}