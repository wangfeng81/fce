/*
**
**	Matrix 2D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Matrix2
	 */
	struct FC_API Matrix2 final
		{
			/*	Model
			 */
			union
				{
					Vector2 row [ 2 ];
					Float	cell[ 2 ][ 2 ];

					struct
						{
							Float _11, _12,
								  _21, _22;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Matrix2, cell )

			XML_ARCHIVE( Matrix2, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ),
								  ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ) )

			/*	Constructor
			*/
			Matrix2() = default;

			constexpr Matrix2( const Vector2& row1,
							   const Vector2& row2 ) noexcept;

			constexpr Matrix2( const Float, const Float,
							   const Float, const Float  ) noexcept;

			/*	Access
			*/
			constexpr auto& operator [] ( this auto&,
										  const UInt8 row );

			constexpr Float operator () ( const UInt8 row,
										  const UInt8 col ) const;

			/*	Column
			*/
			constexpr Void Column( const UInt8 col,
								   const Vector2& );

			constexpr const Vector2 Column( const UInt8 col ) const;

			/*	Compare
			*/
			Bool operator == ( const Matrix2& ) const noexcept;

			Bool operator != ( const Matrix2& ) const noexcept;

			/*	Operation
			*/
			Matrix2 operator - () const noexcept;

			Matrix2 operator * ( const Float ) const noexcept;

			Matrix2 operator / ( const Float ) const noexcept;

			Matrix2 operator + ( const Matrix2& ) const noexcept;

			Matrix2 operator - ( const Matrix2& ) const noexcept;

			Matrix2 operator * ( const Matrix2& ) const noexcept;

			/*	Combinatorial =
			*/
			Matrix2& operator *= ( const Float ) noexcept;

			Matrix2& operator /= ( const Float ) noexcept;

			Matrix2& operator += ( const Matrix2& ) noexcept;

			Matrix2& operator -= ( const Matrix2& ) noexcept;

			Matrix2& operator *= ( const Matrix2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			constexpr Matrix2 Transpose() const noexcept;

			constexpr Float Determinant() const noexcept;

			constexpr Float Minor( const UInt8 row,
								   const UInt8 col ) const noexcept;

			constexpr Matrix2 Adjugate() const noexcept;

			Matrix2 Inverse() const;

			/*	Inverse ( optimized )
			*/
			Bool Inverse( Matrix2& ) const noexcept;

			/*	Constant
			*/
			constinit static const Matrix2 ZERO;
			constinit static const Matrix2 IDENTITY;
		};
}



/**
 !	class Matrix2
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Matrix2::Matrix2( const Vector2& row1,
										  const Vector2& row2 ) noexcept
		:
			row{ row1, row2 }
		{
		}

	FC_INLINE constexpr Matrix2::Matrix2( const Float _11, const Float _12,
										  const Float _21, const Float _22  ) noexcept
		:
			cell
				{
					_11, _12,
					_21, _22
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Matrix2::operator [] ( this auto& self,
													 const UInt8 row )
		{
			FC_ASSERT( row < 2 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Matrix2::operator () ( const UInt8 row,
													 const UInt8 col ) const
		{
			FC_ASSERT( row < 2 && col < 2 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Matrix2::Column( const UInt8 col,
											  const Vector2& vec2 )
		{
			FC_ASSERT( col < 2 );

			this->row[ 0 ][ col ] = vec2.x;
			this->row[ 1 ][ col ] = vec2.y;
		}

	FC_INLINE constexpr const Vector2 Matrix2::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 2 );

			return Vector2( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Matrix2 Matrix2::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Matrix2& Matrix2::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Matrix2& Matrix2::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Matrix2& Matrix2::operator += ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this + mat2;
		}

	FC_INLINE Matrix2& Matrix2::operator -= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this - mat2;
		}

	FC_INLINE Matrix2& Matrix2::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Matrix2::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Matrix2::Identity() noexcept
		{
			*this = IDENTITY;
		}

	FC_INLINE constexpr Matrix2 Matrix2::Transpose() const noexcept
		{
			return Matrix2( this->_11, this->_21,
							this->_12, this->_22 );
		}

	FC_INLINE constexpr Float Matrix2::Determinant() const noexcept
		{
			return this->_11 * this->_22
				 - this->_12 * this->_21;
		}

	FC_INLINE constexpr Float Matrix2::Minor( const UInt8 row,
											  const UInt8 col ) const noexcept
		{
			return this->cell[ row ? 0 : 1 ]
							 [ col ? 0 : 1 ];
		}

	FC_INLINE constexpr Matrix2 Matrix2::Adjugate() const noexcept
		{
			return Matrix2( this->_22, -this->_12,
						   -this->_21,  this->_11 );
		}
}