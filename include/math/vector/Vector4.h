/*
**
**	Vector 4D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Vector4
	 */
	struct FC_API Vector4 final
		{
			/*	Model
			 */
			union
				{
					Vector2 xy;
					Vector3 xyz;
					Float components[ 4 ];

					struct
						{
							Float x, y, z, w;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Vector4,
							ARCHIVE_UNIT( x ),
							ARCHIVE_UNIT( y ),
							ARCHIVE_UNIT( z ),
							ARCHIVE_UNIT( w ) )

			/*	Constructor
			*/
			Vector4() = default;

			constexpr Vector4( const Float ) noexcept;

			constexpr Vector4( const Float x,
							   const Float y,
							   const Float z,
							   const Float w ) noexcept;

			constexpr Vector4( const Vector2&,
							   const Float z,
							   const Float w ) noexcept;

			constexpr Vector4( const Vector3&,
							   const Float w ) noexcept;

			/*	Access
			*/
			constexpr operator Vector3() const noexcept;

			constexpr Float& operator [] ( const UInt8 );

			constexpr const Float operator [] ( const UInt8 ) const;

			/*	Operation
			*/
			constexpr Vector4 operator - () const noexcept;

			constexpr Vector4 operator + ( const Float ) const noexcept;

			constexpr Vector4 operator - ( const Float ) const noexcept;

			constexpr Vector4 operator * ( const Float ) const noexcept;

			constexpr Vector4 operator / ( const Float ) const noexcept;

			constexpr Vector4 operator + ( const Vector4& ) const noexcept;

			constexpr Vector4 operator - ( const Vector4& ) const noexcept;

			constexpr Vector4 operator * ( const Vector4& ) const noexcept;

			constexpr Vector4 operator / ( const Vector4& ) const noexcept;

			Vector4 operator * ( const Affine3& ) const noexcept;

			Vector4 operator * ( const Matrix4& ) const noexcept;

			Vector4 operator * ( const Matrix4X3& ) const noexcept;

			/*	Combinatorial =
			*/
			constexpr Vector4& operator = ( const Float ) noexcept;

			constexpr Vector4& operator += ( const Float ) noexcept;

			constexpr Vector4& operator -= ( const Float ) noexcept;

			constexpr Vector4& operator *= ( const Float ) noexcept;

			constexpr Vector4& operator /= ( const Float ) noexcept;

			constexpr Vector4& operator += ( const Vector4& ) noexcept;

			constexpr Vector4& operator -= ( const Vector4& ) noexcept;

			constexpr Vector4& operator *= ( const Vector4& ) noexcept;

			constexpr Vector4& operator /= ( const Vector4& ) noexcept;

			Vector4& operator *= ( const Affine3& ) noexcept;

			Vector4& operator *= ( const Matrix4& ) noexcept;

			Vector4& operator *= ( const Matrix4X3& ) noexcept;

			/*	Compare
			*/
			constexpr Bool operator > ( const Vector4& ) const noexcept;

			constexpr Bool operator < ( const Vector4& ) const noexcept;

			constexpr Bool operator >= ( const Vector4& ) const noexcept;

			constexpr Bool operator <= ( const Vector4& ) const noexcept;

			Bool operator == ( const Vector4& ) const noexcept;

			Bool operator != ( const Vector4& ) const noexcept;

			/*	Is unit
			*/
			Bool isUnit() const noexcept;

			Bool isUnit2() const noexcept;

			/*	Length
			*/
			Float Length() const noexcept;

			Float Length2() const noexcept;

			/*	Distance
			*/
			Float Distance( const Vector4& ) const noexcept;

			Float Distance2( const Vector4& ) const noexcept;

			/*	Normalize
			*/
			Vector4& Normalize() noexcept;

			Vector4 Normalized() const noexcept;

			/*	Homogenize
			*/
			Vector4& Homogenize() noexcept;

			Vector4 Homogenized() const noexcept;

			/*	Product
			*/
			Float Dot( const Vector4& ) const noexcept;

			Float AbsDot( const Vector4& ) const noexcept;

			Vector4 Cross( const Vector4& ) const noexcept;

			/*	Common
			*/
			constexpr Float Sum() const noexcept;

			Vector4 Abs() const noexcept;

			/*	Constant
			*/
			static constexpr UInt8 DIMENSION = 4;
		};
}



/**
 !	class Vector4
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Vector4::Vector4( const Float c ) noexcept
		:
			components{ c, c, c, c }
		{
		}

	FC_INLINE constexpr Vector4::Vector4( const Float x,
										  const Float y,
										  const Float z,
										  const Float w ) noexcept
		:
			components{ x, y, z , w }
		{
		}

	FC_INLINE constexpr Vector4::Vector4( const Vector2& vec2,
										  const Float z,
										  const Float w ) noexcept
		:
			components{ vec2.x, vec2.y, z, w }
		{
		}

	FC_INLINE constexpr Vector4::Vector4( const Vector3& vec3,
										  const Float w ) noexcept
		:
			components{ vec3.x, vec3.y, vec3.z, w }
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr Vector4::operator Vector3() const noexcept
		{
			return this->xyz / this->w;
		}

	FC_INLINE constexpr Float& Vector4::operator [] ( const UInt8 idx )
		{
			FC_ASSERT( idx < 4 );

			return this->components[ idx ];
		}

	FC_INLINE constexpr const Float Vector4::operator [] ( const UInt8 idx ) const
		{
			FC_ASSERT( idx < 4 );

			return this->components[ idx ];
		}

	/*
	 @	Operation
	 */
	FC_INLINE constexpr Vector4 Vector4::operator - () const noexcept
		{
			return Vector4( -this->x,
							-this->y,
							-this->z,
							-this->w );
		}

	FC_INLINE constexpr Vector4 Vector4::operator + ( const Float f ) const noexcept
		{
			return Vector4( this->x + f,
							this->y + f,
							this->z + f,
							this->w + f );
		}

	FC_INLINE constexpr Vector4 Vector4::operator - ( const Float f ) const noexcept
		{
			return Vector4( this->x - f,
							this->y - f,
							this->z - f,
							this->w - f );
		}

	FC_INLINE constexpr Vector4 Vector4::operator * ( const Float f ) const noexcept
		{
			return Vector4( this->x * f,
							this->y * f,
							this->z * f,
							this->w * f );
		}

	FC_INLINE constexpr Vector4 Vector4::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	FC_INLINE constexpr Vector4 Vector4::operator + ( const Vector4& vec4 ) const noexcept
		{
			return Vector4( this->x + vec4.x,
							this->y + vec4.y,
							this->z + vec4.z,
							this->w + vec4.w );
		}

	FC_INLINE constexpr Vector4 Vector4::operator - ( const Vector4& vec4 ) const noexcept
		{
			return Vector4( this->x - vec4.x,
							this->y - vec4.y,
							this->z - vec4.z,
							this->w - vec4.w );
		}

	FC_INLINE constexpr Vector4 Vector4::operator * ( const Vector4& vec4 ) const noexcept
		{
			return Vector4( this->x * vec4.x,
							this->y * vec4.y,
							this->z * vec4.z,
							this->w * vec4.w );
		}

	FC_INLINE constexpr Vector4 Vector4::operator / ( const Vector4& vec4 ) const noexcept
		{
			return Vector4( this->x / vec4.x,
							this->y / vec4.y,
							this->z / vec4.z,
							this->w / vec4.w );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE constexpr Vector4& Vector4::operator = ( const Float f ) noexcept
		{
			this->x = f;
			this->y = f;
			this->z = f;
			this->w = f;

			return *this;
		}

	FC_INLINE constexpr Vector4& Vector4::operator += ( const Float f ) noexcept
		{
			return *this = *this + f;
		}

	FC_INLINE constexpr Vector4& Vector4::operator -= ( const Float f ) noexcept
		{
			return *this = *this - f;
		}

	FC_INLINE constexpr Vector4& Vector4::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE constexpr Vector4& Vector4::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE constexpr Vector4& Vector4::operator += ( const Vector4& vec4 ) noexcept
		{
			return *this = *this + vec4;
		}

	FC_INLINE constexpr Vector4& Vector4::operator -= ( const Vector4& vec4 ) noexcept
		{
			return *this = *this - vec4;
		}

	FC_INLINE constexpr Vector4& Vector4::operator *= ( const Vector4& vec4 ) noexcept
		{
			return *this = *this * vec4;
		}

	FC_INLINE constexpr Vector4& Vector4::operator /= ( const Vector4& vec4 ) noexcept
		{
			return *this = *this / vec4;
		}

	FC_INLINE Vector4& Vector4::operator *= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this * aff3;
		}

	FC_INLINE Vector4& Vector4::operator *= ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this * mat4;
		}

	FC_INLINE Vector4& Vector4::operator *= ( const Matrix4X3& mat4x3 ) noexcept
		{
			return *this = *this * mat4x3;
		}

	/*
	 @	Compare
	 */
	FC_INLINE constexpr Bool Vector4::operator > ( const Vector4& vec4 ) const noexcept
		{
			return this->x > vec4.x
				&& this->y > vec4.y
				&& this->z > vec4.z
				&& this->w > vec4.w;
		}

	FC_INLINE constexpr Bool Vector4::operator < ( const Vector4& vec4 ) const noexcept
		{
			return this->x < vec4.x
				&& this->y < vec4.y
				&& this->z < vec4.z
				&& this->w < vec4.w;
		}

	FC_INLINE constexpr Bool Vector4::operator >= ( const Vector4& vec4 ) const noexcept
		{
			return this->x >= vec4.x
				&& this->y >= vec4.y
				&& this->z >= vec4.z
				&& this->w >= vec4.w;
		}

	FC_INLINE constexpr Bool Vector4::operator <= ( const Vector4& vec4 ) const noexcept
		{
			return this->x <= vec4.x
				&& this->y <= vec4.y
				&& this->z <= vec4.z
				&& this->w <= vec4.w;
		}

	/*
	 @	Length
	 */
	FC_INLINE Float Vector4::Length() const noexcept
		{
			return math::Sqrt( this->Length2() );
		}

	FC_INLINE Float Vector4::Length2() const noexcept
		{
			return this->Dot( *this );
		}

	/*
	 @	Distance
	 */
	FC_INLINE Float Vector4::Distance( const Vector4& vec4 ) const noexcept
		{
			return math::Sqrt( this->Distance2( vec4 ) );
		}

	FC_INLINE Float Vector4::Distance2( const Vector4& vec4 ) const noexcept
		{
			return ( *this - vec4 ).Length2();
		}

	/*
	 @	Normalize
	 */
	FC_INLINE Vector4& Vector4::Normalize() noexcept
		{
			return *this = this->Normalized();
		}

	FC_INLINE Vector4 Vector4::Normalized() const noexcept
		{
			return *this / this->Length();
		}

	/*
	 @	Homogenize
	 */
	FC_INLINE Vector4& Vector4::Homogenize() noexcept
		{
			return *this = this->Homogenized();
		}

	FC_INLINE Vector4 Vector4::Homogenized() const noexcept
		{
			return Vector4( *this, 1 );
		}

	/*
	 @	Product
	 */
	FC_INLINE Float Vector4::Dot( const Vector4& vec4 ) const noexcept
		{
			return ( *this * vec4 ).Sum();
		}

	FC_INLINE Float Vector4::AbsDot( const Vector4& vec4 ) const noexcept
		{
			return ( *this * vec4 ).Abs().Sum();
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Float Vector4::Sum() const noexcept
		{
			return this->x + this->y + this->z + this->w;
		}

	FC_INLINE Vector4 Vector4::Abs() const noexcept
		{
			return Vector4( math::Abs( this->x ),
							math::Abs( this->y ),
							math::Abs( this->z ),
							math::Abs( this->w ) );
		}
}