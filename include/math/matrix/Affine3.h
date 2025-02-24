/*
**
**	Affine 3D
**
		Copyright (c) WangFeng since 1999...
*/

namespace fce
{
	/*
	 !	Affine3
	 */
	struct FC_API Affine3 final
		{
			/*	Model
			 */
			union
				{
					Vector4 row [ 3 ];
					Float	cell[ 3 ][ 4 ];

					struct
						{
							Float _11, _12, _13, _14,
								  _21, _22, _23, _24,
								  _31, _32, _33, _34;
						};
				};

			/*	Archive
			*/
			BIN_ARCHIVE( Affine3, cell )

			XML_ARCHIVE( Affine3, ARCHIVE_UNIT( _11 ), ARCHIVE_UNIT( _12 ), ARCHIVE_UNIT( _13 ), ARCHIVE_UNIT( _14 ),
								  ARCHIVE_UNIT( _21 ), ARCHIVE_UNIT( _22 ), ARCHIVE_UNIT( _23 ), ARCHIVE_UNIT( _24 ),
								  ARCHIVE_UNIT( _31 ), ARCHIVE_UNIT( _32 ), ARCHIVE_UNIT( _33 ), ARCHIVE_UNIT( _34 ) )

			/*	Constructor
			*/
			constexpr Affine3() noexcept;

			Affine3( const Vector3& position,
					 const Vector3& scale,
					 const Vector3& angles ) noexcept;

			Affine3( const Vector3& position,
					 const Vector3& scale,
					 const Vector3& axis,
					 const Float angle ) noexcept;

			Affine3( const Vector3& position,
					 const Vector3& scale,
					 const Matrix3& rotation ) noexcept;

			constexpr Affine3( const Vector4& row1,
							   const Vector4& row2,
							   const Vector4& row3 ) noexcept;

			constexpr Affine3( const Float, const Float, const Float, const Float,
							   const Float, const Float, const Float, const Float,
							   const Float, const Float, const Float, const Float ) noexcept;

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
			operator Matrix2() const noexcept;

			operator Matrix3() const noexcept;

			/*	Compare
			*/
			Bool operator == ( const Affine3& ) const noexcept;

			Bool operator != ( const Affine3& ) const noexcept;

			/*	Operation
			*/
			Affine3 operator - () const noexcept;

			Affine3 operator * ( const Float ) const noexcept;

			Affine3 operator / ( const Float ) const noexcept;

			Affine3 operator + ( const Affine3& ) const noexcept;

			Affine3 operator - ( const Affine3& ) const noexcept;

			Affine3 operator * ( const Affine3& ) const noexcept;

			Affine3 operator * ( const Affine2& ) const noexcept;

			Affine3 operator * ( const Matrix2& ) const noexcept;

			Affine3 operator * ( const Matrix3& ) const noexcept;

			Matrix4 operator * ( const Matrix4& ) const noexcept;

			Matrix3 operator * ( const Matrix4X3& ) const noexcept;

			/*	Combinatorial =
			*/
			Affine3& operator *= ( const Float ) noexcept;

			Affine3& operator /= ( const Float ) noexcept;

			Affine3& operator += ( const Affine3& ) noexcept;

			Affine3& operator -= ( const Affine3& ) noexcept;

			Affine3& operator *= ( const Affine3& ) noexcept;

			Affine3& operator *= ( const Affine2& ) noexcept;

			Affine3& operator *= ( const Matrix2& ) noexcept;

			Affine3& operator *= ( const Matrix3& ) noexcept;

			/*	Common
			*/
			constexpr Void Zero() noexcept;

			constexpr Void Identity() noexcept;

			Matrix4X3 Transpose() const noexcept;

			Bool Inverse( Affine3& ) const noexcept;

			Void Invert( const Vector3& pos,
						 const Vector3& scale,
						 const Quaternion& orien );

			/*	Update
			*/
			Void UpdateTranslation( const Vector3& ) noexcept;

			Void UpdateRotScale( const Matrix3& rot,
								 const Vector3& scale ) noexcept;

			/*	Transform
			*/
			static constexpr Affine3 Translation( const Float x,
												  const Float y,
												  const Float z ) noexcept;

			static constexpr Matrix3 Scaling( const Float x,
											  const Float y,
											  const Float z ) noexcept;

			static Matrix3 Rotation( const Float x,
									 const Float y,
									 const Float z ) noexcept;

			static Matrix3 RotationX( const Float angle ) noexcept;

			static Matrix3 RotationY( const Float angle ) noexcept;

			static Matrix3 RotationZ( const Float angle ) noexcept;

			static Matrix3 Rotation( const Vector3& angles ) noexcept
				;
			static Matrix3 Rotation( const Vector3& axis,
									 const Float angle ) noexcept;

			/*	Constant
			*/
			constinit static const Affine3 ZERO;
			constinit static const Affine3 IDENTITY;
		};
}



/**
 !	class Affine3
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE constexpr Affine3::Affine3() noexcept
		:
			Affine3( IDENTITY )
		{
		}

	FC_INLINE constexpr Affine3::Affine3( const Vector4& row1,
										  const Vector4& row2,
										  const Vector4& row3 ) noexcept
		:
			row{ row1, row2, row3 }
		{
		}

	FC_INLINE constexpr Affine3::Affine3( const Float _11, const Float _12, const Float _13, const Float _14,
										  const Float _21, const Float _22, const Float _23, const Float _24,
										  const Float _31, const Float _32, const Float _33, const Float _34 ) noexcept
		:
			cell
				{
					_11, _12, _13, _14,
					_21, _22, _23, _24,
					_31, _32, _33, _34
				}
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE constexpr auto& Affine3::operator [] ( this auto& self,
													 const UInt8 row )
		{
			FC_ASSERT( row < 3 );

			return self.row[ row ];
		}

	FC_INLINE constexpr Float Affine3::operator() ( const UInt8 row,
													const UInt8 col ) const
		{
			FC_ASSERT( row < 3 && col < 4 );

			return this->cell[ row ][ col ];
		}

	/*
	 @	Column
	 */
	FC_INLINE constexpr Void Affine3::Column( const UInt8 col,
											  const Vector3& vec3 )
		{
			FC_ASSERT( col < 4 );

			this->row[ 0 ][ col ] = vec3.x;
			this->row[ 1 ][ col ] = vec3.y;
			this->row[ 2 ][ col ] = vec3.z;
		}

	FC_INLINE constexpr const Vector3 Affine3::Column( const UInt8 col ) const
		{
			FC_ASSERT( col < 4 );

			return Vector3( this->row[ 0 ][ col ],
							this->row[ 1 ][ col ],
							this->row[ 2 ][ col ] );
		}

	/*
	 @	Operation
	 */
	FC_INLINE Affine3 Affine3::operator / ( const Float f ) const noexcept
		{
			return *this * math::Inverse( f );
		}

	/*
	 @	Combinatorial =
	 */
	FC_INLINE Affine3& Affine3::operator *= ( const Float f ) noexcept
		{
			return *this = *this * f;
		}

	FC_INLINE Affine3& Affine3::operator /= ( const Float f ) noexcept
		{
			return *this = *this / f;
		}

	FC_INLINE Affine3& Affine3::operator += ( const Affine3& aff3 ) noexcept
		{
			return *this = *this + aff3;
		}

	FC_INLINE Affine3& Affine3::operator -= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this - aff3;
		}

	FC_INLINE Affine3& Affine3::operator *= ( const Affine3& aff3 ) noexcept
		{
			return *this = *this * aff3;
		}

	FC_INLINE Affine3& Affine3::operator *= ( const Affine2& aff2 ) noexcept
		{
			return *this = *this * aff2;
		}

	FC_INLINE Affine3& Affine3::operator *= ( const Matrix2& mat2 ) noexcept
		{
			return *this = *this * mat2;
		}

	FC_INLINE Affine3& Affine3::operator *= ( const Matrix3& mat3 ) noexcept
		{
			return *this = *this * mat3;
		}

	/*
	 @	Update translation
	 */
	FC_INLINE Void Affine3::UpdateTranslation( const Vector3& pos ) noexcept
		{
			this->Column( 3, pos );
		}

	/*
	 @	Common
	 */
	FC_INLINE constexpr Void Affine3::Zero() noexcept
		{
			*this = ZERO;
		}

	FC_INLINE constexpr Void Affine3::Identity() noexcept
		{
			*this = IDENTITY;
		}

	/*
	 @	Transform
	 */
	FC_INLINE constexpr Affine3 Affine3::Translation( const Float x,
													  const Float y,
													  const Float z ) noexcept
		{
			return Affine3( 1, 0, 0, x,
							0, 1, 0, y,
							0, 0, 1, z );
		}

	FC_INLINE constexpr Matrix3 Affine3::Scaling( const Float x,
												  const Float y,
												  const Float z ) noexcept
		{
			return Matrix3( x, 0, 0,
							0, y, 0,
							0, 0, z );
		}

	FC_INLINE Matrix3 Affine3::Rotation( const Float x,
										 const Float y,
										 const Float z ) noexcept
		{
			return Rotation( Vector3( x, y, z ) );
		}
}