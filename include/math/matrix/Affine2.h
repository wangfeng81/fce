/*
**
**	Affine 2D 
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Affine2
	 */
	struct FC_API Affine2 final
		{
			/*	Model
			 */
			union
				{
					Vector3 row [ 2 ];
					Float	cell[ 2 ][ 3 ];

					struct
						{
							Float _11, _12, _13,
								  _21, _22, _23;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Affine2, cell )

			XML_ARCHIVE( Affine2, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ), ARCHIVE_UNIT( _13 ),
								  ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ), ARCHIVE_UNIT( _23 ) )

			/*	Constructor
			*/
			Affine2() = default;

			Affine2( const Vector2& pos,
					 const Vector2& scale,
					 const Float angle ) noexcept;

			Affine2( const Vector2& pos,
					 const Vector2& scale,
					 const Matrix2& rotation ) noexcept;

			constexpr Affine2( const Vector3& row1,
							   const Vector3& row2 ) noexcept;

			constexpr Affine2( const Float, const Float, const Float,
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
								   const Vector2& );

			constexpr const Vector2 Column( const UInt8 col ) const;

			/*	Conversion
			*/
			operator Matrix2() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const Affine2& ) const noexcept;

			Bool operator != ( const Affine2& ) const noexcept;

			/*	Operation
			*/
			Affine2 operator - () const noexcept;

			Affine2 operator * ( const Float ) const noexcept;

			Affine2 operator / ( const Float ) const noexcept;

			Affine2 operator + ( const Affine2& ) const noexcept;

			Affine2 operator - ( const Affine2& ) const noexcept;

			Affine2 operator * ( const Affine2& ) const noexcept;

			Affine2 operator * ( const Matrix2& ) const noexcept;

			Matrix2 operator * ( const Matrix3X2& ) const noexcept;

			/*	Combinatorial =
			*/
			Affine2& operator *= ( const Float ) noexcept;

			Affine2& operator /= ( const Float ) noexcept;

			Affine2& operator += ( const Affine2& ) noexcept;

			Affine2& operator -= ( const Affine2& ) noexcept;

			Affine2& operator *= ( const Affine2& ) noexcept;

			Affine2& operator *= ( const Matrix2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Matrix3X2 Transpose() const noexcept;

			Void Inverse( Affine2& ) const noexcept;

			Void Invert( const Vector2& position,
						 const Vector2& scale,
						 const Float angle ) noexcept;

			/*	Transform
			*/
			static constexpr Affine2 Translation( const Float x,
												  const Float y ) noexcept;

			static constexpr Matrix2 Scaling( const Float x,
											  const Float y ) noexcept;

			static Matrix2 Rotation( const Float angle ) noexcept;

			static Affine2 Rotation( const Float x,
									 const Float y,
									 const Float angle ) noexcept;

			/*	Constant
			*/
			constinit static const Affine2 ZERO;
			constinit static const Affine2 IDENTITY;
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
	FC_INLINE constexpr Affine2::Affine2( const Vector3& row1,
										  const Vector3& row2 ) noexcept
		:
			row{ row1, row2 }
		{
		}

	FC_INLINE constexpr Affine2::Affine2( const Float _11, const Float _12, const Float _13,
										  const Float _21, const Float _22, const Float _23 ) noexcept
		:
			cell
				{
					_11, _12, _13,
					_21, _22, _23
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Affine2::operator [] ( this auto& self,
													 const UInt8 row )
		{
			FC_ASSERT( row < 2 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Affine2::operator() ( const UInt8 row,
													const UInt8 col ) const
		{
			FC_ASSERT( row < 2 && col < 3 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Affine2::Column( const UInt8 col,
											  const Vector2& vec2 )
		{
			FC_ASSERT( col < 3 );

			this->row[ 0 ][ col ] = vec2.x;
			this->row[ 1 ][ col ] = vec2.y;
		}

	FC_INLINE constexpr const Vector2 Affine2::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 3 );

			return Vector2( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Affine2 Affine2::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Affine2& Affine2::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Affine2& Affine2::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Affine2& Affine2::operator += ( const Affine2& aff2 ) noexcept
		{
			return *this = *this + aff2;
		}

	FC_INLINE Affine2& Affine2::operator -= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this - aff2;
		}

	FC_INLINE Affine2& Affine2::operator *= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this * aff2;
		}

	FC_INLINE Affine2& Affine2::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Affine2::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Affine2::Identity() noexcept
		{
			*this = IDENTITY;
		}

	/*
	 @	Transform
	 */
	FC_INLINE constexpr Affine2 Affine2::Translation( const Float x,
													  const Float y ) noexcept
		{
			return Affine2( 1, 0, x,
							0, 1, y );
		}

	FC_INLINE constexpr Matrix2 Affine2::Scaling( const Float x,
												  const Float y ) noexcept
		{
			return Matrix2( x, 0,
							0, y );
		}
}