/*
**
**	Matrix 4D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Matrix4
	 */
	struct FC_API Matrix4 final
		{
			/*	Model
			 */
			union
				{
					Vector4 row [ 4 ];
					Float	cell[ 4 ][ 4 ];

					struct
						{
							Float _11, _12, _13, _14,
								  _21, _22, _23, _24,
								  _31, _32, _33, _34,
								  _41, _42, _43, _44;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Matrix4, cell )

			XML_ARCHIVE( Matrix4, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ), ARCHIVE_UNIT( _13 ), ARCHIVE_UNIT( _14 ),
								  ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ), ARCHIVE_UNIT( _23 ), ARCHIVE_UNIT( _24 ),
								  ARCHIVE_UNIT( _31 ), ARCHIVE_UNIT( _32 ), ARCHIVE_UNIT( _33 ), ARCHIVE_UNIT( _34 ),
								  ARCHIVE_UNIT( _41 ), ARCHIVE_UNIT( _42 ), ARCHIVE_UNIT( _43 ), ARCHIVE_UNIT( _44 ) )

			/*	Constructor
			*/
			Matrix4() = default;

			constexpr Matrix4( const Vector4& row1,
							   const Vector4& row2,
							   const Vector4& row3,
							   const Vector4& row4 ) noexcept;

			constexpr Matrix4( const Float, const Float, const Float, const Float,
							   const Float, const Float, const Float, const Float,
							   const Float, const Float, const Float, const Float,
							   const Float, const Float, const Float, const Float ) noexcept;

			/*	Conversion
			*/
			constexpr operator Affine3&() noexcept;

			constexpr operator const Affine3&() const noexcept;

			constexpr operator Matrix2() const noexcept;

			constexpr operator Matrix3() const noexcept;

			/*	Access
			*/
			constexpr auto& operator [] ( this auto&,
										  const UInt8 row );

			constexpr Float operator() ( const UInt8 row,
										 const UInt8 col ) const;

			/*	Column
			*/
			constexpr Void Column( const UInt8 col,
								   const Vector4& );

			constexpr const Vector4 Column( const UInt8 col ) const;

			/*	Compare
			*/
			Bool operator == ( const Matrix4& ) const noexcept;

			Bool operator != ( const Matrix4& ) const noexcept;

			/*	Operation
			*/
			Matrix4 operator - () const noexcept;

			Matrix4 operator * ( const Float ) const noexcept;

			Matrix4 operator / ( const Float ) const noexcept;

			Matrix4 operator + ( const Matrix4& ) const noexcept;

			Matrix4 operator - ( const Matrix4& ) const noexcept;

			Matrix4 operator * ( const Matrix4& ) const noexcept;

			Matrix4 operator * ( const Matrix3& ) const noexcept;

			Matrix4 operator * ( const Matrix2& ) const noexcept;

			Matrix4 operator * ( const Affine3& ) const noexcept;

			Matrix4 operator * ( const Affine2& ) const noexcept;

			/*	Combinatorial =
			*/
			Matrix4& operator *= ( const Float ) noexcept;

			Matrix4& operator /= ( const Float ) noexcept;

			Matrix4& operator += ( const Matrix4& ) noexcept;

			Matrix4& operator -= ( const Matrix4& ) noexcept;

			Matrix4& operator *= ( const Matrix4& ) noexcept;

			Matrix4& operator *= ( const Matrix3& ) noexcept;

			Matrix4& operator *= ( const Matrix2& ) noexcept;

			Matrix4& operator *= ( const Affine3& ) noexcept;

			Matrix4& operator *= ( const Affine2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Matrix4 Transpose() const noexcept;

			Float Determinant( const Matrix4& adjugate ) const noexcept;

			Float Determinant() const noexcept;

			Float Minor( const UInt8 row,
						 const UInt8 col ) const noexcept;

			Matrix4	Adjugate() const noexcept;

			Matrix4 Inverse() const;

			/*	Inverse ( optimized )
			*/
			Bool Inverse( Matrix4& ) const noexcept;

			/*	Constant
			*/
			constinit static const Matrix4 ZERO;
			constinit static const Matrix4 IDENTITY;
		};
}



/**
 !	class Matrix4
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Matrix4::Matrix4( const Vector4& row1,
										  const Vector4& row2,
										  const Vector4& row3,
										  const Vector4& row4 ) noexcept
		:
			row{ row1, row2, row3, row4 }
		{
		}

	FC_INLINE constexpr Matrix4::Matrix4( const Float _11, const Float _12, const Float _13, const Float _14,
										  const Float _21, const Float _22, const Float _23, const Float _24,
										  const Float _31, const Float _32, const Float _33, const Float _34,
										  const Float _41, const Float _42, const Float _43, const Float _44 ) noexcept
		:
			cell
				{
					_11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34,
					_41, _42, _43, _44
				}
		{
		}

	/*
	 @	Conversion
	 */
	FC_INLINE constexpr Matrix4::operator Affine3&() noexcept
		{
			return ( Affine3& ) *this;
		}

	FC_INLINE constexpr Matrix4::operator const Affine3&() const noexcept
		{
			return ( Affine3& ) *this;
		}

	FC_INLINE constexpr Matrix4::operator Matrix2() const noexcept
		{
			return Matrix2( this->row[ 0 ].xy,
							this->row[ 1 ].xy );
		}

	FC_INLINE constexpr Matrix4::operator Matrix3() const noexcept
		{
			return Matrix3( this->row[ 0 ].xyz,
							this->row[ 1 ].xyz,
							this->row[ 2 ].xyz );
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Matrix4::operator [] ( this auto& self,
													 const UInt8 row )
		{
			FC_ASSERT( row < 4 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Matrix4::operator() ( const UInt8 row,
													const UInt8 col ) const
		{
			FC_ASSERT( row < 4 && col < 4 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Matrix4::Column( const UInt8 col,
											  const Vector4& vec4 )
		{
			FC_ASSERT( col < 4 );

			this->row[ 0 ][ col ] = vec4.x;
			this->row[ 1 ][ col ] = vec4.y;
			this->row[ 2 ][ col ] = vec4.z;
			this->row[ 3 ][ col ] = vec4.w;
		}

	FC_INLINE constexpr const Vector4 Matrix4::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 4 );

			return Vector4( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ],
							this->row[ 2 ][ col ],
							this->row[ 3 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Matrix4 Matrix4::operator / ( const  Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Matrix4& Matrix4::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Matrix4& Matrix4::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Matrix4& Matrix4::operator += ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this + mat4;
		}

	FC_INLINE Matrix4& Matrix4::operator -= ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this - mat4;
		}

	FC_INLINE Matrix4& Matrix4::operator *= ( const Matrix4& mat4 ) noexcept
		{
			return *this = *this * mat4;
		}

	FC_INLINE Matrix4& Matrix4::operator *= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this * mat3;
		}

	FC_INLINE Matrix4& Matrix4::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	FC_INLINE Matrix4& Matrix4::operator *= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this * aff3;
		}

	FC_INLINE Matrix4& Matrix4::operator *= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this * aff2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Matrix4::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Matrix4::Identity() noexcept
		{
			*this = IDENTITY;
		}
}