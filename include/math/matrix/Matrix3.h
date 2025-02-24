/*
**
**	Matrix 3D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Matrix3
	 */
	struct FC_API Matrix3 final
		{
			/*	Model
			 */
			union
				{
					Vector3 row [ 3 ];
					Float	cell[ 3 ][ 3 ];

					struct
						{
							Float _11, _12, _13,
								  _21, _22, _23,
								  _31, _32, _33;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Matrix3, cell )

			XML_ARCHIVE( Matrix3, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ), ARCHIVE_UNIT( _13 ),
								  ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ), ARCHIVE_UNIT( _23 ),
								  ARCHIVE_UNIT( _31 ), ARCHIVE_UNIT( _32 ), ARCHIVE_UNIT( _33 ) )

			/*	Constructor
			*/
			Matrix3() = default;

			constexpr Matrix3( const Vector3& row1,
							   const Vector3& row2,
							   const Vector3& row3 ) noexcept;

			constexpr Matrix3( const Float, const Float, const Float,
							   const Float, const Float, const Float,
							   const Float, const Float, const Float ) noexcept;

			/*	Access
			*/
			constexpr auto& operator [] ( this auto&,
										  const UInt8 row );

			constexpr Float operator() ( const UInt8 row,
										 const UInt8 col ) const;

			/*	Column
			*/
			constexpr Void Column( const UInt8 col,
								   const Vector3& );

			constexpr const Vector3 Column( const UInt8 col ) const;

			/*	Conversion
			*/
			constexpr operator Affine2&() noexcept;

			constexpr operator const Affine2&() const noexcept;

			constexpr operator Matrix2() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const Matrix3& ) const noexcept;

			Bool operator != ( const Matrix3& ) const noexcept;

			/*	Operation
			*/
			Matrix3 operator - () const noexcept;

			Matrix3 operator * ( const Float ) const noexcept;

			Matrix3 operator / ( const Float ) const noexcept;

			Matrix3 operator + ( const Matrix3& ) const noexcept;

			Matrix3 operator - ( const Matrix3& ) const noexcept;

			Matrix3 operator * ( const Matrix3& ) const noexcept;

			Matrix3 operator * ( const Matrix2& ) const noexcept;

			/*	Combinatorial =
			*/
			Matrix3& operator *= ( const Float ) noexcept;

			Matrix3& operator /= ( const Float ) noexcept;

			Matrix3& operator += ( const Matrix3& ) noexcept;

			Matrix3& operator -= ( const Matrix3& ) noexcept;

			Matrix3& operator *= ( const Matrix3& ) noexcept;

			Matrix3& operator *= ( const Matrix2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Matrix3 Transpose() const noexcept;

			Float Determinant() const noexcept;

			Float Determinant( const Matrix3& adjugate ) const noexcept;

			Float Minor( const UInt8 row,
						 const UInt8 col ) const noexcept;

			Matrix3	Adjugate() const noexcept;

			Matrix3 Inverse() const;

			Bool Inverse( Matrix3& ) const noexcept;

			Void Orthonormalize() noexcept;

			/*	Constant
			*/
			constinit static const Matrix3 ZERO;
			constinit static const Matrix3 IDENTITY;
		};
}



/**
 !	class Matrix3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Matrix3::Matrix3( const Vector3& row1,
										  const Vector3& row2,
										  const Vector3& row3 ) noexcept
		:
			row{ row1, row2, row3 }
		{
		}

	FC_INLINE constexpr Matrix3::Matrix3( const Float _11, const Float _12, const Float _13,
										  const Float _21, const Float _22, const Float _23,
										  const Float _31, const Float _32, const Float _33 ) noexcept
		:
			cell
				{
					_11, _12, _13,
					_21, _22, _23,
					_31, _32, _33
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Matrix3::operator [] ( this auto& self,
													 const UInt8 row )
		{
			FC_ASSERT( row < 3 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Matrix3::operator() ( const UInt8 row,
													const UInt8 col ) const
		{
			FC_ASSERT( row < 3 && col < 3 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Matrix3::Column( const UInt8 col,
											  const Vector3& vec3 )
		{
			FC_ASSERT( col < 3 );

			this->row[ 0 ][ col ] = vec3.x;
			this->row[ 1 ][ col ] = vec3.y;
			this->row[ 2 ][ col ] = vec3.z;
		}

	FC_INLINE constexpr const Vector3 Matrix3::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 3 );

			return Vector3( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ],
							this->row[ 2 ][ col ] );
		}

	/*
	 @	Conversion
	 */
	FC_INLINE constexpr Matrix3::operator Affine2&() noexcept
		{
			return ( Affine2& ) *this;
		}

	FC_INLINE constexpr Matrix3::operator const Affine2&() const noexcept
		{
			return ( Affine2& ) *this;
		}

	FC_INLINE constexpr Matrix3::operator Matrix2() const noexcept
		{
			return Matrix2( this->row[ 0 ].xy,
							this->row[ 1 ].xy );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Matrix3 Matrix3::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Matrix3& Matrix3::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Matrix3& Matrix3::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Matrix3& Matrix3::operator += ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this + mat3;
		}

	FC_INLINE Matrix3& Matrix3::operator -= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this - mat3;
		}

	FC_INLINE Matrix3& Matrix3::operator *= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this * mat3;
		}

	FC_INLINE Matrix3& Matrix3::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Matrix3::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Matrix3::Identity() noexcept
		{
			*this = IDENTITY;
		}
}