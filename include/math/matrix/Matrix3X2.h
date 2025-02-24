/*
**
**	Matrix 3x2
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Matrix3X2
	 */
	struct FC_API Matrix3X2 final
		{
			/*	Model
			 */
			union
				{
					Vector2 row [ 3 ];
					Float	cell[ 3 ][ 2 ];

					struct
						{
							Float _11, _12,
								  _21, _22,
								  _31, _32;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Matrix3X2, cell )

			XML_ARCHIVE( Matrix3X2, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ),
									ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ),
									ARCHIVE_UNIT( _31 ), ARCHIVE_UNIT( _32 ) )

			/*	Constructor
			*/
			Matrix3X2() = default;

			constexpr Matrix3X2( const Vector2& row1,
								 const Vector2& row2,
								 const Vector2& row3 ) noexcept;

			constexpr Matrix3X2( const Float, const Float,
								 const Float, const Float,
								 const Float, const Float ) noexcept;

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

			/*	Compare
			*/
			Bool operator == ( const Matrix3X2& ) const noexcept;

			Bool operator != ( const Matrix3X2& ) const noexcept;

			/*	Operation
			*/
			Matrix3X2 operator - () const noexcept;

			Matrix3X2 operator * ( const Float ) const noexcept;

			Matrix3X2 operator / ( const Float ) const noexcept;

			Matrix3X2 operator + ( const Matrix3X2& ) const noexcept;

			Matrix3X2 operator - ( const Matrix3X2& ) const noexcept;

			Matrix3   operator * ( const Affine2& ) const noexcept;

			Matrix3X2 operator * ( const Matrix2& ) const noexcept;

			/*	Combinatorial =
			*/
			Matrix3X2& operator *= ( const Float ) noexcept;

			Matrix3X2& operator /= ( const Float ) noexcept;

			Matrix3X2& operator += ( const Matrix3X2& ) noexcept;

			Matrix3X2& operator -= ( const Matrix3X2& ) noexcept;

			Matrix3X2& operator *= ( const Matrix2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Affine2 Transpose() const noexcept;

			/*	Constant
			*/
			constinit static const Matrix3X2 ZERO;
			constinit static const Matrix3X2 IDENTITY;
		};
}



/**
 !	class Matrix3X2
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Matrix3X2::Matrix3X2( const Vector2& row1,
											  const Vector2& row2,
											  const Vector2& row3 ) noexcept
		:
			row{ row1, row2, row3 }
		{
		}

	FC_INLINE constexpr Matrix3X2::Matrix3X2( const Float _11, const Float _12,
											  const Float _21, const Float _22,
											  const Float _31, const Float _32 ) noexcept
		:
			cell
				{
					_11, _12,
					_21, _22,
					_31, _32
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Matrix3X2::operator [] ( this auto& self,
													   const UInt8 row )
		{
			FC_ASSERT( row < 3 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Matrix3X2::operator() ( const UInt8 row,
													  const UInt8 col ) const
		{
			FC_ASSERT( row < 3 && col < 2 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Matrix3X2::Column( const UInt8 col,
												const Vector3& vec3 )
		{
			FC_ASSERT( col < 2 );

			this->row[ 0 ][ col ] = vec3.x;
			this->row[ 1 ][ col ] = vec3.y;
			this->row[ 2 ][ col ] = vec3.z;
		}

	FC_INLINE constexpr const Vector3 Matrix3X2::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 2 );

			return Vector3( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ],
							this->row[ 2 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Matrix3X2 Matrix3X2::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Matrix3X2& Matrix3X2::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Matrix3X2& Matrix3X2::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Matrix3X2& Matrix3X2::operator += ( const Matrix3X2& mat3x2 ) noexcept
		{
			return *this = *this + mat3x2;
		}

	FC_INLINE Matrix3X2& Matrix3X2::operator -= ( const Matrix3X2& mat3x2 ) noexcept
		{
			return *this = *this - mat3x2;
		}

	FC_INLINE Matrix3X2& Matrix3X2::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Matrix3X2::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Matrix3X2::Identity() noexcept
		{
			*this = IDENTITY;
		}
}