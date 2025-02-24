/*
**
**	Matrix 4x3
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Matrix4X3
	 */
	struct FC_API Matrix4X3 final
		{
			/*	Model
			 */
			union
				{
					Vector3 row [ 4 ];
					Float	cell[ 4 ][ 3 ];

					struct
						{
							Float _11, _12, _13,
								  _21, _22, _23,
								  _31, _32, _33,
								  _41, _42, _43;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Matrix4X3, cell )

			XML_ARCHIVE( Matrix4X3, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ), ARCHIVE_UNIT( _13 ),
									ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ), ARCHIVE_UNIT( _23 ),
									ARCHIVE_UNIT( _31 ), ARCHIVE_UNIT( _32 ), ARCHIVE_UNIT( _33 ),
									ARCHIVE_UNIT( _41 ), ARCHIVE_UNIT( _42 ), ARCHIVE_UNIT( _43 ) )

			/*	Constructor
			*/
			Matrix4X3() = default;

			constexpr Matrix4X3( const Vector3& row1,
								 const Vector3& row2,
								 const Vector3& row3,
								 const Vector3& row4 ) noexcept;

			constexpr Matrix4X3( const Float, const Float, const Float,
								 const Float, const Float, const Float,
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
								   const Vector4& );

			constexpr const Vector4 Column( const UInt8 col ) const;

			/*	Compare
			*/
			Bool operator == ( const Matrix4X3& ) const noexcept;

			Bool operator != ( const Matrix4X3& ) const noexcept;

			/*	Operation
			*/
			Matrix4X3 operator - () const noexcept;

			Matrix4X3 operator * ( const Float ) const noexcept;

			Matrix4X3 operator / ( const Float ) const noexcept;

			Matrix4X3 operator + ( const Matrix4X3& ) const noexcept;

			Matrix4X3 operator - ( const Matrix4X3& ) const noexcept;

			Matrix4   operator * ( const Affine3& ) const noexcept;

			Matrix4X3 operator * ( const Matrix3& ) const noexcept;

			Matrix4X3 operator * ( const Matrix2& ) const noexcept;

			/*	Combinatorial =
			*/
			Matrix4X3& operator *= ( const Float ) noexcept;

			Matrix4X3& operator /= ( const Float ) noexcept;

			Matrix4X3& operator += ( const Matrix4X3& ) noexcept;

			Matrix4X3& operator -= ( const Matrix4X3& ) noexcept;

			Matrix4X3& operator *= ( const Matrix3& ) noexcept;

			Matrix4X3& operator *= ( const Matrix2& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Affine3 Transpose() const noexcept;

			/*	Constant
			*/
			constinit static const Matrix4X3 ZERO;
			constinit static const Matrix4X3 IDENTITY;
		};
}



/**
 !	class Matrix4X3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Matrix4X3::Matrix4X3( const Vector3& row1,
											  const Vector3& row2,
											  const Vector3& row3,
											  const Vector3& row4 ) noexcept
		:
			row{ row1, row2, row3, row4 }
		{
		}

	FC_INLINE constexpr Matrix4X3::Matrix4X3( const Float _11, const Float _12, const Float _13,
											  const Float _21, const Float _22, const Float _23,
											  const Float _31, const Float _32, const Float _33,
											  const Float _41, const Float _42, const Float _43 ) noexcept
		:
			cell
				{
					_11, _12, _13,
					_21, _22, _23,
					_31, _32, _33,
					_41, _42, _43
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Matrix4X3::operator [] ( this auto& self,
													   const UInt8 row )
		{
			FC_ASSERT( row < 4 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Matrix4X3::operator() ( const UInt8 row,
													  const UInt8 col ) const
		{
			FC_ASSERT( row < 4 && col < 3 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Matrix4X3::Column( const UInt8 col,
												const Vector4& vec4 )
		{
			FC_ASSERT( col < 3 );

			this->row[ 0 ][ col ] = vec4.x;
			this->row[ 1 ][ col ] = vec4.y;
			this->row[ 2 ][ col ] = vec4.z;
			this->row[ 3 ][ col ] = vec4.w;
		}

	FC_INLINE constexpr const Vector4 Matrix4X3::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 3 );

			return Vector4( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ],
							this->row[ 2 ][ col ],
							this->row[ 3 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Matrix4X3 Matrix4X3::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Matrix4X3& Matrix4X3::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Matrix4X3& Matrix4X3::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Matrix4X3& Matrix4X3::operator += ( const Matrix4X3& mat4x3 ) noexcept
		{
			return *this = *this + mat4x3;
		}

	FC_INLINE Matrix4X3& Matrix4X3::operator -= ( const Matrix4X3& mat4x3 ) noexcept
		{
			return *this = *this - mat4x3;
		}

	FC_INLINE Matrix4X3& Matrix4X3::operator *= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this * mat3;
		}

	FC_INLINE Matrix4X3& Matrix4X3::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Matrix4X3::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Matrix4X3::Identity() noexcept
		{
			*this = IDENTITY;
		}
}