/*
**
**	Matrix.
**
		行列式		determinant		任一行( 或列 )的 m( n )个元素 乘以各自的 cofactor 的总和
		余子式		minor			删除任一行 i 和列 j 后 剩下矩阵的 determinant
		代数余子式	cofactor		minor 根据自己的行列号之和的奇偶取正负 (-1)^(m+n)
		伴随矩阵	adjugate		每个元素的 cofactor 构成的矩阵 的转置矩阵
		逆矩阵		inverse			adjugate / determinant	( det != 0 )

		优化：高阶由低阶组成 对矩阵分块 可减少重复计算

	Copyright (c) WangFeng since 1999...
*/

#include "../../include/math/Quaternion.h"


/**
 !	Matrix2
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Matrix2 Matrix2::ZERO( 0, 0,
										   0, 0 );

	constinit const Matrix2 Matrix2::IDENTITY( 1, 0,
											   0, 1 );
    

	/*
	 @	Compare
	 */
	Bool Matrix2::operator == ( const Matrix2& mat2 ) const noexcept
		{
			return ( *this )[ 0 ] == mat2[ 0 ]
				&& ( *this )[ 1 ] == mat2[ 1 ];
		}

	Bool Matrix2::operator != ( const Matrix2& mat2 ) const noexcept
		{
			return ( *this )[ 0 ] != mat2[ 0 ]
				|| ( *this )[ 1 ] != mat2[ 1 ];
		}


	/*
	 @	Operation
	 */
	Matrix2 Matrix2::operator - () const noexcept
		{
			return Matrix2( -( *this )[ 0 ],
							-( *this )[ 1 ] );
		}

	Matrix2 Matrix2::operator * ( const Float f ) const noexcept
		{
			return Matrix2( ( *this )[ 0 ] * f,
							( *this )[ 1 ] * f );
		}

	Matrix2 Matrix2::operator + ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix2( ( *this )[ 0 ] + mat2[ 0 ],
							( *this )[ 1 ] + mat2[ 1 ] );
		}

	Matrix2 Matrix2::operator - ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix2( ( *this )[ 0 ] - mat2[ 0 ],
							( *this )[ 1 ] - mat2[ 1 ] );
		}


	/*
	 @	operator * Matrix2
	 */
	Matrix2 Matrix2::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix2( this->_11 * mat2._11 + this->_12 * mat2._21,
							this->_11 * mat2._12 + this->_12 * mat2._22,

							this->_21 * mat2._11 + this->_22 * mat2._21,
							this->_21 * mat2._12 + this->_22 * mat2._22 );
		}


	/*
	 @	Inverse
	 */
	Matrix2 Matrix2::Inverse() const
		{
			Float det{ this->Determinant() };

			FC_ASSERT_LOG( NotEqual( det, 0 ),
						   "The inverse matrix does not exist!" );

			return this->Adjugate() / det;
		}

	Bool Matrix2::Inverse( Matrix2& mat2 ) const noexcept
		{
			Float det{ this->Determinant() };

			if( NotEqual( det, 0 ) )
				{
					mat2 = this->Adjugate()
						 / det;

					return true;
				}

			return false;
		}
}



/**
 !	Matrix3
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Matrix3 Matrix3::ZERO( 0, 0, 0,
										   0, 0, 0,
										   0, 0, 0 );

	constinit const Matrix3 Matrix3::IDENTITY( 1, 0, 0,
											   0, 1, 0,
											   0, 0, 1 );


	/*
	 @	Compare
	 */
	Bool Matrix3::operator == ( const Matrix3& mat3 ) const noexcept
		{
			return ( *this )[ 0 ] == mat3[ 0 ]
				&& ( *this )[ 1 ] == mat3[ 1 ]
				&& ( *this )[ 2 ] == mat3[ 2 ];
		}


	Bool Matrix3::operator != ( const Matrix3& mat3 ) const noexcept
		{
			return ( *this )[ 0 ] != mat3[ 0 ]
				|| ( *this )[ 1 ] != mat3[ 1 ]
				|| ( *this )[ 2 ] != mat3[ 2 ];
		}


	/*
	 @	Operation
	 */
	Matrix3 Matrix3::operator - () const noexcept
		{
			return Matrix3( -( *this )[ 0 ],
							-( *this )[ 1 ],
							-( *this )[ 2 ] );
		}

	Matrix3 Matrix3::operator * ( const Float f ) const noexcept
		{
			return Matrix3( ( *this )[ 0 ] * f,
							( *this )[ 1 ] * f,
							( *this )[ 2 ] * f );
		}

	Matrix3 Matrix3::operator + ( const Matrix3& mat3 ) const noexcept
		{
			return Matrix3( ( *this )[ 0 ] + mat3[ 0 ],
							( *this )[ 1 ] + mat3[ 1 ],
							( *this )[ 2 ] + mat3[ 2 ] );
		}

	Matrix3 Matrix3::operator - ( const Matrix3& mat3 ) const noexcept
		{
			return Matrix3( ( *this )[ 0 ] - mat3[ 0 ],
							( *this )[ 1 ] - mat3[ 1 ],
							( *this )[ 2 ] - mat3[ 2 ] );
		}


	/*
	 @	operator * Matrix3
	 */
	Matrix3 Matrix3::operator * ( const Matrix3& mat3 ) const noexcept
		{
			return Matrix3( this->_11 * mat3._11 + this->_12 * mat3._21 + this->_13 * mat3._31,
							this->_11 * mat3._12 + this->_12 * mat3._22 + this->_13 * mat3._32,
							this->_11 * mat3._13 + this->_12 * mat3._23 + this->_13 * mat3._33,

							this->_21 * mat3._11 + this->_22 * mat3._21 + this->_23 * mat3._31,
							this->_21 * mat3._12 + this->_22 * mat3._22 + this->_23 * mat3._32,
							this->_21 * mat3._13 + this->_22 * mat3._23 + this->_23 * mat3._33,

							this->_31 * mat3._11 + this->_32 * mat3._21 + this->_33 * mat3._31,
							this->_31 * mat3._12 + this->_32 * mat3._22 + this->_33 * mat3._32,
							this->_31 * mat3._13 + this->_32 * mat3._23 + this->_33 * mat3._33 );
		}


	/*
	 @	operator * Matrix2
	 */
	Matrix3 Matrix3::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix3( this->_11 * mat2._11 + this->_12 * mat2._21,
							this->_11 * mat2._12 + this->_12 * mat2._22,
							this->_13,

							this->_21 * mat2._11 + this->_22 * mat2._21,
							this->_21 * mat2._12 + this->_22 * mat2._22,
							this->_23,

							this->_31 * mat2._11 + this->_32 * mat2._21,
							this->_31 * mat2._12 + this->_32 * mat2._22,
							this->_33 );
		}


	/*
	 @	Transpose
	 */
	Matrix3 Matrix3::Transpose() const noexcept
		{
			return Matrix3( this->_11, this->_21, this->_31,
							this->_12, this->_22, this->_32,
							this->_13, this->_23, this->_33 );
		}


	/*
	 @	Determinant
	 */
	Float Matrix3::Determinant( const Matrix3& adjugate ) const noexcept
		{
			return this->_11 * adjugate._11
				 + this->_12 * adjugate._21
				 + this->_13 * adjugate._31;
		}

	Float Matrix3::Determinant() const noexcept
		{
			return this->_11 * this->Minor( 0, 0 )
				 - this->_12 * this->Minor( 0, 1 )
				 + this->_13 * this->Minor( 0, 2 );
		}
    

	/*
	 @	Adjugate
	 */
	Matrix3 Matrix3::Adjugate() const noexcept
		{
			/*
				 Minor( 0, 0 ) -Minor( 1, 0 )  Minor( 2, 0 )
				-Minor( 0, 1 )  Minor( 1, 1 ) -Minor( 2, 1 )
				 Minor( 0, 2 ) -Minor( 1, 2 )  Minor( 2, 2 )
			*/
			return Matrix3( this->_22 * this->_33 - this->_23 * this->_32,
							this->_13 * this->_32 - this->_12 * this->_33,
							this->_12 * this->_23 - this->_13 * this->_22,

							this->_23 * this->_31 - this->_21 * this->_33,
							this->_11 * this->_33 - this->_13 * this->_31,
							this->_13 * this->_21 - this->_11 * this->_23,

							this->_21 * this->_32 - this->_22 * this->_31,
							this->_12 * this->_31 - this->_11 * this->_32,
							this->_11 * this->_22 - this->_12 * this->_21 );
		}


	/*
	 @	Minor
	 */
	Float Matrix3::Minor( const UInt8 row,
						  const UInt8 col ) const noexcept
		{
			// sub indices
			constexpr UInt8 sub[ 3 ][ 2 ]
				{
					1, 2,
					0, 2,
					0, 1,
				};

			// sub-matrix row & col
			const UInt8 row1{ sub[ row ][ 0 ] },
						row2{ sub[ row ][ 1 ] };

			const UInt8 col1{ sub[ col ][ 0 ] },
						col2{ sub[ col ][ 1 ] };

			// sub-matrix determinant
			return Matrix2( this->cell[ row1 ][ col1 ],
							this->cell[ row1 ][ col2 ],
							this->cell[ row2 ][ col1 ],
							this->cell[ row2 ][ col2 ] ).Determinant();
		}


	/*
	 @	Inverse
	 */
	Matrix3 Matrix3::Inverse() const
		{
			Matrix3 adjugate{ this->Adjugate() };

			Float det{ this->Determinant( adjugate ) };

			FC_ASSERT_LOG( NotEqual( det, 0 ),
						   "The inverse matrix does not exist!" );

			return adjugate / det;
		}


	/*
	 @	Inverse
	 */
	Bool Matrix3::Inverse( Matrix3& mat3 ) const noexcept
		{
			// adjugate matrix
			mat3 = { this->_22 * this->_33 - this->_23 * this->_32,
					 this->_13 * this->_32 - this->_12 * this->_33,
					 this->_12 * this->_23 - this->_13 * this->_22,
					 
					 this->_23 * this->_31 - this->_21 * this->_33,
					 this->_11 * this->_33 - this->_13 * this->_31,
					 this->_13 * this->_21 - this->_11 * this->_23,
					 
					 this->_21 * this->_32 - this->_22 * this->_31,
					 this->_12 * this->_31 - this->_11 * this->_32,
					 this->_11 * this->_22 - this->_12 * this->_21 };

			// determinant
			Float det{ this->_11 * mat3._11
					 + this->_12 * mat3._21
					 + this->_13 * mat3._31 };

			// adjugate / det
			if( NotEqual( det, 0 ) )
				{
					mat3 /= det;

					return true;
				}

			return false;
		}


	/*
	 @	Orthonormalize
	 */
	Void Matrix3::Orthonormalize() noexcept
		{
			// Gram-Schmidt orthogonalization

			// col 1
			this->Column( 0, this->Column( 0 )
							.Normalized() );

			// col 2
			Float dot12 = this->Column( 1 )
						 .Dot( this->Column( 0 ) );

			this->Column( 1, ( this->Column( 1 )
								- this->Column( 0 )
								* dot12 ).Normalized() );

			// col 3
			Float dot13 = this->Column( 2 )
						 .Dot( this->Column( 0 ) );

			Float dot23 = this->Column( 2 )
						 .Dot( this->Column( 1 ) );

			this->Column( 2, ( this->Column( 2 )
								- ( this->Column( 1 ) * dot23
								  + this->Column( 0 ) * dot13 ) )
								.Normalized() );
		}
}



/**
 !	Matrix4
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Matrix4 Matrix4::ZERO( 0, 0, 0, 0,
										   0, 0, 0, 0,
										   0, 0, 0, 0,
										   0, 0, 0, 0 );

	constinit const Matrix4 Matrix4::IDENTITY( 1, 0, 0, 0,
											   0, 1, 0, 0,
											   0, 0, 1, 0,
											   0, 0, 0, 1 );


	/*
	 @	Compare
	 */
	Bool Matrix4::operator == ( const Matrix4& mat4 ) const noexcept
		{
			return ( *this )[ 0 ] == mat4[ 0 ]
				&& ( *this )[ 1 ] == mat4[ 1 ]
				&& ( *this )[ 2 ] == mat4[ 2 ]
				&& ( *this )[ 3 ] == mat4[ 3 ];
		}


	Bool Matrix4::operator != ( const Matrix4& mat4 ) const noexcept
		{
			return ( *this )[ 0 ] != mat4[ 0 ]
				|| ( *this )[ 1 ] != mat4[ 1 ]
				|| ( *this )[ 2 ] != mat4[ 2 ]
				|| ( *this )[ 3 ] != mat4[ 3 ];
		}


	/*
	 @	Operation
	 */
	Matrix4 Matrix4::operator - () const noexcept
		{
			return Matrix4( -( *this )[ 0 ],
							-( *this )[ 1 ],
							-( *this )[ 2 ],
							-( *this )[ 3 ] );
		}

	Matrix4 Matrix4::operator * ( const Float f ) const noexcept
		{
			return Matrix4( ( *this )[ 0 ] * f,
							( *this )[ 1 ] * f,
							( *this )[ 2 ] * f,
							( *this )[ 3 ] * f );
		}

	Matrix4 Matrix4::operator + ( const Matrix4& mat4 ) const noexcept
		{
			return Matrix4( ( *this )[ 0 ] + mat4[ 0 ],
							( *this )[ 1 ] + mat4[ 1 ],
							( *this )[ 2 ] + mat4[ 2 ],
							( *this )[ 3 ] + mat4[ 3 ] );
		}

	Matrix4 Matrix4::operator - ( const Matrix4& mat4 ) const noexcept
		{
			return Matrix4( ( *this )[ 0 ] - mat4[ 0 ],
							( *this )[ 1 ] - mat4[ 1 ],
							( *this )[ 2 ] - mat4[ 2 ],
							( *this )[ 3 ] - mat4[ 3 ] );
		}


	/*
	 @	operator * Matrix4
	 */
	Matrix4 Matrix4::operator * ( const Matrix4& mat4 ) const noexcept
		{
			return Matrix4( this->_11 * mat4._11 + this->_12 * mat4._21 + this->_13 * mat4._31 + this->_14 * mat4._41,
							this->_11 * mat4._12 + this->_12 * mat4._22 + this->_13 * mat4._32 + this->_14 * mat4._42,
							this->_11 * mat4._13 + this->_12 * mat4._23 + this->_13 * mat4._33 + this->_14 * mat4._43,
							this->_11 * mat4._14 + this->_12 * mat4._24 + this->_13 * mat4._34 + this->_14 * mat4._44,

							this->_21 * mat4._11 + this->_22 * mat4._21 + this->_23 * mat4._31 + this->_24 * mat4._41,
							this->_21 * mat4._12 + this->_22 * mat4._22 + this->_23 * mat4._32 + this->_24 * mat4._42,
							this->_21 * mat4._13 + this->_22 * mat4._23 + this->_23 * mat4._33 + this->_24 * mat4._43,
							this->_21 * mat4._14 + this->_22 * mat4._24 + this->_23 * mat4._34 + this->_24 * mat4._44,

							this->_31 * mat4._11 + this->_32 * mat4._21 + this->_33 * mat4._31 + this->_34 * mat4._41,
							this->_31 * mat4._12 + this->_32 * mat4._22 + this->_33 * mat4._32 + this->_34 * mat4._42,
							this->_31 * mat4._13 + this->_32 * mat4._23 + this->_33 * mat4._33 + this->_34 * mat4._43,
							this->_31 * mat4._14 + this->_32 * mat4._24 + this->_33 * mat4._34 + this->_34 * mat4._44,

							this->_41 * mat4._11 + this->_42 * mat4._21 + this->_43 * mat4._31 + this->_44 * mat4._41,
							this->_41 * mat4._12 + this->_42 * mat4._22 + this->_43 * mat4._32 + this->_44 * mat4._42,
							this->_41 * mat4._13 + this->_42 * mat4._23 + this->_43 * mat4._33 + this->_44 * mat4._43,
							this->_41 * mat4._14 + this->_42 * mat4._24 + this->_43 * mat4._34 + this->_44 * mat4._44 );
		}


	/*
	 @	operator * Affine3
	 */
	Matrix4 Matrix4::operator * ( const Affine3& aff3 ) const noexcept
		{
			return Matrix4( this->_11 * aff3._11 + this->_12 * aff3._21 + this->_13 * aff3._31,
							this->_11 * aff3._12 + this->_12 * aff3._22 + this->_13 * aff3._32,
							this->_11 * aff3._13 + this->_12 * aff3._23 + this->_13 * aff3._33,
							this->_11 * aff3._14 + this->_12 * aff3._24 + this->_13 * aff3._34 + this->_14,

							this->_21 * aff3._11 + this->_22 * aff3._21 + this->_23 * aff3._31,
							this->_21 * aff3._12 + this->_22 * aff3._22 + this->_23 * aff3._32,
							this->_21 * aff3._13 + this->_22 * aff3._23 + this->_23 * aff3._33,
							this->_21 * aff3._14 + this->_22 * aff3._24 + this->_23 * aff3._34 + this->_24,

							this->_31 * aff3._11 + this->_32 * aff3._21 + this->_33 * aff3._31,
							this->_31 * aff3._12 + this->_32 * aff3._22 + this->_33 * aff3._32,
							this->_31 * aff3._13 + this->_32 * aff3._23 + this->_33 * aff3._33,
							this->_31 * aff3._14 + this->_32 * aff3._24 + this->_33 * aff3._34 + this->_34,

							this->_41 * aff3._11 + this->_42 * aff3._21 + this->_43 * aff3._31,
							this->_41 * aff3._12 + this->_42 * aff3._22 + this->_43 * aff3._32,
							this->_41 * aff3._13 + this->_42 * aff3._23 + this->_43 * aff3._33,
							this->_41 * aff3._14 + this->_42 * aff3._24 + this->_43 * aff3._34 + this->_44 );
		}


	/*
	 @	operator * Affine2
	 */
	Matrix4 Matrix4::operator * ( const Affine2& aff2 ) const noexcept
		{
			return Matrix4( this->_11 * aff2._11 + this->_12 * aff2._21,
							this->_11 * aff2._12 + this->_12 * aff2._22,
							this->_13,
							this->_11 * aff2._13 + this->_12 * aff2._23 + this->_14,

							this->_21 * aff2._11 + this->_22 * aff2._21,
							this->_21 * aff2._12 + this->_22 * aff2._22,
							this->_23,
							this->_21 * aff2._13 + this->_22 * aff2._23 + this->_24,

							this->_31 * aff2._11 + this->_32 * aff2._21,
							this->_31 * aff2._12 + this->_32 * aff2._22,
							this->_33,
							this->_31 * aff2._13 + this->_32 * aff2._23 + this->_34,

							this->_41 * aff2._11 + this->_42 * aff2._21,
							this->_41 * aff2._12 + this->_42 * aff2._22,
							this->_43,
							this->_41 * aff2._13 + this->_42 * aff2._23 + this->_44 );
		}


	/*
	 @	operator * Matrix3
	 */
	Matrix4 Matrix4::operator * ( const Matrix3& mat3 ) const noexcept
		{
			return Matrix4( this->_11 * mat3._11 + this->_12 * mat3._21 + this->_13 * mat3._31,
							this->_11 * mat3._12 + this->_12 * mat3._22 + this->_13 * mat3._32,
							this->_11 * mat3._13 + this->_12 * mat3._23 + this->_13 * mat3._33,
							this->_14,

							this->_21 * mat3._11 + this->_22 * mat3._21 + this->_23 * mat3._31,
							this->_21 * mat3._12 + this->_22 * mat3._22 + this->_23 * mat3._32,
							this->_21 * mat3._13 + this->_22 * mat3._23 + this->_23 * mat3._33,
							this->_24,

							this->_31 * mat3._11 + this->_32 * mat3._21 + this->_33 * mat3._31,
							this->_31 * mat3._12 + this->_32 * mat3._22 + this->_33 * mat3._32,
							this->_31 * mat3._13 + this->_32 * mat3._23 + this->_33 * mat3._33,
							this->_34,

							this->_41 * mat3._11 + this->_42 * mat3._21 + this->_43 * mat3._31,
							this->_41 * mat3._12 + this->_42 * mat3._22 + this->_43 * mat3._32,
							this->_41 * mat3._13 + this->_42 * mat3._23 + this->_43 * mat3._33,
							this->_44 );
		}


	/*
	 @	operator * Matrix2
	 */
	Matrix4 Matrix4::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix4( this->_11 * mat2._11 + this->_12 * mat2._21,
							this->_11 * mat2._12 + this->_12 * mat2._22,
							this->_13,
							this->_14,

							this->_21 * mat2._11 + this->_22 * mat2._21,
							this->_21 * mat2._12 + this->_22 * mat2._22,
							this->_23,
							this->_24,

							this->_31 * mat2._11 + this->_32 * mat2._21,
							this->_31 * mat2._12 + this->_32 * mat2._22,
							this->_33,
							this->_34,

							this->_41 * mat2._11 + this->_42 * mat2._21,
							this->_41 * mat2._12 + this->_42 * mat2._22,
							this->_43,
							this->_44 );
		}


	/*
	 @	Transpose
	 */
	Matrix4 Matrix4::Transpose() const noexcept
		{
			return Matrix4( this->_11, this->_21, this->_31, this->_41,
							this->_12, this->_22, this->_32, this->_42,
							this->_13, this->_23, this->_33, this->_43,
							this->_14, this->_24, this->_34, this->_44 );
		}


	/*
	 @	Determinant
	 */
	Float Matrix4::Determinant( const Matrix4& adjugate ) const noexcept
		{
			return this->_11 * adjugate._11
				 + this->_12 * adjugate._21
				 + this->_13 * adjugate._31
				 + this->_14 * adjugate._41;
		}

	Float Matrix4::Determinant() const noexcept
		{
			return this->_11 * this->Minor( 0, 0 )
				 - this->_12 * this->Minor( 0, 1 )
				 + this->_13 * this->Minor( 0, 2 )
				 - this->_14 * this->Minor( 0, 3 );
		}


	/*
	 @	Adjugate
	 */
	Matrix4 Matrix4::Adjugate() const noexcept
		{
			/*
				 Minor( 0, 0 ) -Minor( 1, 0 )  Minor( 2, 0 ) -Minor( 3, 0 )
				-Minor( 0, 1 )  Minor( 1, 1 ) -Minor( 2, 1 )  Minor( 3, 1 )
				 Minor( 0, 2 ) -Minor( 1, 2 )  Minor( 2, 2 ) -Minor( 3, 2 )
				-Minor( 0, 3 )  Minor( 1, 3 ) -Minor( 2, 3 )  Minor( 3, 3 )
			*/
			Matrix4 mat4;

			// sub-matrix2 determinant for adjugate col 1 & col 2
			Float s0 = this->_31 * this->_42 - this->_32 * this->_41,
				  s1 = this->_31 * this->_43 - this->_33 * this->_41,
				  s2 = this->_31 * this->_44 - this->_34 * this->_41,
				  s3 = this->_32 * this->_43 - this->_33 * this->_42,
				  s4 = this->_32 * this->_44 - this->_34 * this->_42,
				  s5 = this->_33 * this->_44 - this->_34 * this->_43;

			// adjugate matrix col 1
			mat4._11 = s5 * this->_22 - s4 * this->_23 + s3 * this->_24;
			mat4._21 = s2 * this->_23 - s5 * this->_21 - s1 * this->_24;
			mat4._31 = s4 * this->_21 - s2 * this->_22 + s0 * this->_24;
			mat4._41 = s1 * this->_22 - s3 * this->_21 - s0 * this->_23;

			// adjugate matrix col 2	inverse
			mat4._12 = ( s4 * this->_13 - s5 * this->_12 - s3 * this->_14 );
			mat4._22 = ( s5 * this->_11 - s2 * this->_13 + s1 * this->_14 );
			mat4._32 = ( s2 * this->_12 - s4 * this->_11 - s0 * this->_14 );
			mat4._42 = ( s3 * this->_11 - s1 * this->_12 + s0 * this->_13 );

			// sub-matrix2 determinant for adjugate col 3
			s0 = this->_21 * this->_42 - this->_22 * this->_41;
			s1 = this->_21 * this->_43 - this->_23 * this->_41;
			s2 = this->_21 * this->_44 - this->_24 * this->_41;
			s3 = this->_22 * this->_43 - this->_23 * this->_42;
			s4 = this->_22 * this->_44 - this->_24 * this->_42;
			s5 = this->_23 * this->_44 - this->_24 * this->_43;

			// adjugate matrix col 3	inverse
			mat4._13 = ( s5 * this->_12 - s4 * this->_13 + s3 * this->_14 );
			mat4._23 = ( s2 * this->_13 - s5 * this->_11 - s1 * this->_14 );
			mat4._33 = ( s4 * this->_11 - s2 * this->_12 + s0 * this->_14 );
			mat4._43 = ( s1 * this->_12 - s3 * this->_11 - s0 * this->_13 );

			// sub-matrix2 determinant for adjugate col 4
			s0 = this->_32 * this->_21 - this->_31 * this->_22;
			s1 = this->_33 * this->_21 - this->_31 * this->_23;
			s2 = this->_34 * this->_21 - this->_31 * this->_24;
			s3 = this->_33 * this->_22 - this->_32 * this->_23;
			s4 = this->_34 * this->_22 - this->_32 * this->_24;
			s5 = this->_34 * this->_23 - this->_33 * this->_24;

			// adjugate matrix col 4	inverse
			mat4._14 = ( s4 * this->_13 - s5 * this->_12 - s3 * this->_14 );
			mat4._24 = ( s5 * this->_11 - s2 * this->_13 + s1 * this->_14 );
			mat4._34 = ( s2 * this->_12 - s4 * this->_11 - s0 * this->_14 );
			mat4._44 = ( s3 * this->_11 - s1 * this->_12 + s0 * this->_13 );

			return mat4;
		}


	/*
	 @	Minor
	 */
	Float Matrix4::Minor( const UInt8 row,
						  const UInt8 col ) const noexcept
		{
			// sub indices
			constexpr UInt8 sub[ 4 ][ 3 ]
				{
					1, 2, 3,
					0, 2, 3,
					0, 1, 3,
					0, 1, 2,
				};

			// sub-matrix row & col
			const UInt8 row1 = sub[ row ][ 0 ],
						row2 = sub[ row ][ 1 ],
						row3 = sub[ row ][ 2 ];

			const UInt8 col1 = sub[ col ][ 0 ],
						col2 = sub[ col ][ 1 ],
						col3 = sub[ col ][ 2 ];

			// sub-matrix determinant
			return Matrix3( this->cell[ row1 ][ col1 ],
							this->cell[ row1 ][ col2 ],
							this->cell[ row1 ][ col3 ],

							this->cell[ row2 ][ col1 ],
							this->cell[ row2 ][ col2 ],
							this->cell[ row2 ][ col3 ],

							this->cell[ row3 ][ col1 ],
							this->cell[ row3 ][ col2 ],
							this->cell[ row3 ][ col3 ] ).Determinant();
		}


	/*
	 @	Inverse
	 */
	Matrix4 Matrix4::Inverse() const
		{
			Matrix4 adjugate = this->Adjugate();

			Float det = this->Determinant( adjugate );

			FC_ASSERT_LOG( NotEqual( det, 0 ),
						   "The inverse matrix does not exist!" );

			return adjugate / det;
		}


	/*
	 @	Inverse
	 */
	Bool Matrix4::Inverse( Matrix4& mat4 ) const noexcept
		{
			// sub-matrix2 determinant for adjugate col 1 & col 2
			Float s0 = this->_31 * this->_42 - this->_32 * this->_41,
				  s1 = this->_31 * this->_43 - this->_33 * this->_41,
				  s2 = this->_31 * this->_44 - this->_34 * this->_41,
				  s3 = this->_32 * this->_43 - this->_33 * this->_42,
				  s4 = this->_32 * this->_44 - this->_34 * this->_42,
				  s5 = this->_33 * this->_44 - this->_34 * this->_43;

			// adjugate matrix col 1
			mat4._11 = s5 * this->_22 - s4 * this->_23 + s3 * this->_24;
			mat4._21 = s2 * this->_23 - s5 * this->_21 - s1 * this->_24;
			mat4._31 = s4 * this->_21 - s2 * this->_22 + s0 * this->_24;
			mat4._41 = s1 * this->_22 - s3 * this->_21 - s0 * this->_23;

			// matrix4 determinant
			Float det = this->_11 * mat4._11
					  + this->_12 * mat4._21
					  + this->_13 * mat4._31
					  + this->_14 * mat4._41;

			if( Equal( det, 0 ) )
				{
					return false;
				}

			Float invDet = math::Inverse( det );

			mat4._11 *= invDet;
			mat4._21 *= invDet;
			mat4._31 *= invDet;
			mat4._41 *= invDet;

			// adjugate matrix col 2	inverse
			mat4._12 = ( s4 * this->_13 - s5 * this->_12 - s3 * this->_14 ) * invDet;
			mat4._22 = ( s5 * this->_11 - s2 * this->_13 + s1 * this->_14 ) * invDet;
			mat4._32 = ( s2 * this->_12 - s4 * this->_11 - s0 * this->_14 ) * invDet;
			mat4._42 = ( s3 * this->_11 - s1 * this->_12 + s0 * this->_13 ) * invDet;

			// sub-matrix2 determinant for adjugate col 3
			s0 = this->_21 * this->_42 - this->_22 * this->_41;
			s1 = this->_21 * this->_43 - this->_23 * this->_41;
			s2 = this->_21 * this->_44 - this->_24 * this->_41;
			s3 = this->_22 * this->_43 - this->_23 * this->_42;
			s4 = this->_22 * this->_44 - this->_24 * this->_42;
			s5 = this->_23 * this->_44 - this->_24 * this->_43;

			// adjugate matrix col 3	inverse
			mat4._13 = ( s5 * this->_12 - s4 * this->_13 + s3 * this->_14 ) * invDet;
			mat4._23 = ( s2 * this->_13 - s5 * this->_11 - s1 * this->_14 ) * invDet;
			mat4._33 = ( s4 * this->_11 - s2 * this->_12 + s0 * this->_14 ) * invDet;
			mat4._43 = ( s1 * this->_12 - s3 * this->_11 - s0 * this->_13 ) * invDet;

			// sub-matrix2 determinant for adjugate col 4
			s0 = this->_32 * this->_21 - this->_31 * this->_22;
			s1 = this->_33 * this->_21 - this->_31 * this->_23;
			s2 = this->_34 * this->_21 - this->_31 * this->_24;
			s3 = this->_33 * this->_22 - this->_32 * this->_23;
			s4 = this->_34 * this->_22 - this->_32 * this->_24;
			s5 = this->_34 * this->_23 - this->_33 * this->_24;

			// adjugate matrix col 4	inverse
			mat4._14 = ( s4 * this->_13 - s5 * this->_12 - s3 * this->_14 ) * invDet;
			mat4._24 = ( s5 * this->_11 - s2 * this->_13 + s1 * this->_14 ) * invDet;
			mat4._34 = ( s2 * this->_12 - s4 * this->_11 - s0 * this->_14 ) * invDet;
			mat4._44 = ( s3 * this->_11 - s1 * this->_12 + s0 * this->_13 ) * invDet;

			return true;
		}
}



/**
 !	Matrix3X2
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Matrix3X2 Matrix3X2::ZERO( 0, 0,
											   0, 0,
											   0, 0 );

	constinit const Matrix3X2 Matrix3X2::IDENTITY( 1, 0,
												   0, 1,
												   0, 0 );


	/*
	 @	Compare
	 */
	Bool Matrix3X2::operator == ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return ( *this )[ 0 ] == mat3x2[ 0 ]
				&& ( *this )[ 1 ] == mat3x2[ 1 ]
				&& ( *this )[ 2 ] == mat3x2[ 2 ];
		}


	Bool Matrix3X2::operator != ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return ( *this )[ 0 ] != mat3x2[ 0 ]
				|| ( *this )[ 1 ] != mat3x2[ 1 ]
				|| ( *this )[ 2 ] != mat3x2[ 2 ];
		}


	/*
	 @	Operation
	 */
	Matrix3X2 Matrix3X2::operator - () const noexcept
		{
			return Matrix3X2( -( *this )[ 0 ],
							  -( *this )[ 1 ],
							  -( *this )[ 2 ] );
		}

	Matrix3X2 Matrix3X2::operator * ( const Float f ) const noexcept
		{
			return Matrix3X2( ( *this )[ 0 ] * f,
							  ( *this )[ 1 ] * f,
							  ( *this )[ 2 ] * f );
		}

	Matrix3X2 Matrix3X2::operator + ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return Matrix3X2( ( *this )[ 0 ] + mat3x2[ 0 ],
							  ( *this )[ 1 ] + mat3x2[ 1 ],
							  ( *this )[ 2 ] + mat3x2[ 2 ] );
		}

	Matrix3X2 Matrix3X2::operator - ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return Matrix3X2( ( *this )[ 0 ] - mat3x2[ 0 ],
							  ( *this )[ 1 ] - mat3x2[ 1 ],
							  ( *this )[ 2 ] - mat3x2[ 2 ] );
		}

	/*
	 @	operator * Affine2
	 */
	Matrix3 Matrix3X2::operator * ( const Affine2& aff2 ) const noexcept
		{
			return Matrix3( this->_11 * aff2._11 + this->_12 * aff2._21,
							this->_11 * aff2._12 + this->_12 * aff2._22,
							this->_11 * aff2._13 + this->_12 * aff2._23,

							this->_21 * aff2._11 + this->_22 * aff2._21,
							this->_21 * aff2._12 + this->_22 * aff2._22,
							this->_21 * aff2._13 + this->_22 * aff2._23,

							this->_31 * aff2._11 + this->_32 * aff2._21,
							this->_31 * aff2._12 + this->_32 * aff2._22,
							this->_31 * aff2._13 + this->_32 * aff2._23 );
		}


	/*
	 @	operator * Matrix2
	 */
	Matrix3X2 Matrix3X2::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix3X2( this->_11 * mat2._11 + this->_12 * mat2._21,
							  this->_11 * mat2._12 + this->_12 * mat2._22,

							  this->_21 * mat2._11 + this->_22 * mat2._21,
							  this->_21 * mat2._12 + this->_22 * mat2._22,

							  this->_31 * mat2._11 + this->_32 * mat2._21,
							  this->_31 * mat2._12 + this->_32 * mat2._22 );
		}
    

	/*
	 @	Transpose
	 */
	Affine2 Matrix3X2::Transpose() const noexcept
		{
			return Affine2( this->_11, this->_21, this->_31,
							this->_12, this->_22, this->_32 );
		}
}



/**
 !	Matrix4X3
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Matrix4X3 Matrix4X3::ZERO( 0, 0, 0,
											   0, 0, 0,
											   0, 0, 0,
											   0, 0, 0 );

	constinit const Matrix4X3 Matrix4X3::IDENTITY( 1, 0, 0,
												   0, 1, 0,
												   0, 0, 1,
												   0, 0, 0 );


	/*
	 @	Compare
	 */
	Bool Matrix4X3::operator == ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return ( *this )[ 0 ] == mat4x3[ 0 ]
				&& ( *this )[ 1 ] == mat4x3[ 1 ]
				&& ( *this )[ 2 ] == mat4x3[ 2 ]
				&& ( *this )[ 3 ] == mat4x3[ 3 ];
		}

	Bool Matrix4X3::operator != ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return ( *this )[ 0 ] != mat4x3[ 0 ]
				|| ( *this )[ 1 ] != mat4x3[ 1 ]
				|| ( *this )[ 2 ] != mat4x3[ 2 ]
				|| ( *this )[ 3 ] != mat4x3[ 3 ];
		}


	/*
	 @	Operation
	 */
	Matrix4X3 Matrix4X3::operator - () const noexcept
		{
			return Matrix4X3( -( *this )[ 0 ],
							  -( *this )[ 1 ],
							  -( *this )[ 2 ],
							  -( *this )[ 3 ] );
		}

	Matrix4X3 Matrix4X3::operator * ( const Float f ) const noexcept
		{
			return Matrix4X3( ( *this )[ 0 ] * f,
							  ( *this )[ 1 ] * f,
							  ( *this )[ 2 ] * f,
							  ( *this )[ 3 ] * f );
		}

	Matrix4X3 Matrix4X3::operator + ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return Matrix4X3( ( *this )[ 0 ] + mat4x3[ 0 ],
							  ( *this )[ 1 ] + mat4x3[ 1 ],
							  ( *this )[ 2 ] + mat4x3[ 2 ],
							  ( *this )[ 3 ] + mat4x3[ 3 ] );
		}

	Matrix4X3 Matrix4X3::operator - ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return Matrix4X3( ( *this )[ 0 ] - mat4x3[ 0 ],
							  ( *this )[ 1 ] - mat4x3[ 1 ],
							  ( *this )[ 2 ] - mat4x3[ 2 ],
							  ( *this )[ 3 ] - mat4x3[ 3 ] );
		}


	/*
	 @	operator * Affine3
	 */
	Matrix4 Matrix4X3::operator * ( const Affine3& aff3 ) const noexcept
		{
			return Matrix4( this->_11 * aff3._11 + this->_12 * aff3._21 + this->_13 * aff3._31,
							this->_11 * aff3._12 + this->_12 * aff3._22 + this->_13 * aff3._32,
							this->_11 * aff3._13 + this->_12 * aff3._23 + this->_13 * aff3._33,
							this->_11 * aff3._14 + this->_12 * aff3._24 + this->_13 * aff3._34,

							this->_21 * aff3._11 + this->_22 * aff3._21 + this->_23 * aff3._31,
							this->_21 * aff3._12 + this->_22 * aff3._22 + this->_23 * aff3._32,
							this->_21 * aff3._13 + this->_22 * aff3._23 + this->_23 * aff3._33,
							this->_21 * aff3._14 + this->_22 * aff3._24 + this->_23 * aff3._34,

							this->_31 * aff3._11 + this->_32 * aff3._21 + this->_33 * aff3._31,
							this->_31 * aff3._12 + this->_32 * aff3._22 + this->_33 * aff3._32,
							this->_31 * aff3._13 + this->_32 * aff3._23 + this->_33 * aff3._33,
							this->_31 * aff3._14 + this->_32 * aff3._24 + this->_33 * aff3._34,

							this->_41 * aff3._11 + this->_42 * aff3._21 + this->_43 * aff3._31,
							this->_41 * aff3._12 + this->_42 * aff3._22 + this->_43 * aff3._32,
							this->_41 * aff3._13 + this->_42 * aff3._23 + this->_43 * aff3._33,
							this->_41 * aff3._14 + this->_42 * aff3._24 + this->_43 * aff3._34 );
		}


	/*
	 @	operator * Matrix3
	 */
	Matrix4X3 Matrix4X3::operator * ( const Matrix3& mat3 ) const noexcept
		{
			return Matrix4X3( this->_11 * mat3._11 + this->_12 * mat3._21 + this->_13 * mat3._31,
							  this->_11 * mat3._12 + this->_12 * mat3._22 + this->_13 * mat3._32,
							  this->_11 * mat3._13 + this->_12 * mat3._23 + this->_13 * mat3._33,

							  this->_21 * mat3._11 + this->_22 * mat3._21 + this->_23 * mat3._31,
							  this->_21 * mat3._12 + this->_22 * mat3._22 + this->_23 * mat3._32,
							  this->_21 * mat3._13 + this->_22 * mat3._23 + this->_23 * mat3._33,

							  this->_31 * mat3._11 + this->_32 * mat3._21 + this->_33 * mat3._31,
							  this->_31 * mat3._12 + this->_32 * mat3._22 + this->_33 * mat3._32,
							  this->_31 * mat3._13 + this->_32 * mat3._23 + this->_33 * mat3._33,

							  this->_41 * mat3._11 + this->_42 * mat3._21 + this->_43 * mat3._31,
							  this->_41 * mat3._12 + this->_42 * mat3._22 + this->_43 * mat3._32,
							  this->_41 * mat3._13 + this->_42 * mat3._23 + this->_43 * mat3._33 );
		}


	/*
	 @	operator * Matrix2
	 */
	Matrix4X3 Matrix4X3::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Matrix4X3( this->_11 * mat2._11 + this->_12 * mat2._21,
							  this->_11 * mat2._12 + this->_12 * mat2._22,
							  this->_13,

							  this->_21 * mat2._11 + this->_22 * mat2._21,
							  this->_21 * mat2._12 + this->_22 * mat2._22,
							  this->_23,

							  this->_31 * mat2._11 + this->_32 * mat2._21,
							  this->_31 * mat2._12 + this->_32 * mat2._22,
							  this->_33,

							  this->_41 * mat2._11 + this->_42 * mat2._21,
							  this->_41 * mat2._12 + this->_42 * mat2._22,
							  this->_43 );
		}


	/*
	 @	Transpose
	 */
	Affine3 Matrix4X3::Transpose() const noexcept
		{
			return Affine3( this->_11, this->_21, this->_31, this->_41,
							this->_12, this->_22, this->_32, this->_42,
							this->_13, this->_23, this->_33, this->_43 );
		}
}



/**
 !	Affine2
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Affine2 Affine2::ZERO( 0, 0, 0,
										   0, 0, 0 );

	constinit const Affine2 Affine2::IDENTITY( 1, 0, 0,
											   0, 1, 0 );


	/*
	 @	Constructor

			Rotate => Scale => Translate
	 */
	Affine2::Affine2( const Vector2& position,
					  const Vector2& scale,
					  const Float angle ) noexcept
		:
			Affine2( position, scale,
					 Rotation( angle ) )
		{
		}


	/*
	 @	Constructor

			Rotate => Scale => Translate
	 */
	Affine2::Affine2( const Vector2& position,
					  const Vector2& scale,
					  const Matrix2& rotation ) noexcept
		:
			row
				{
					{ rotation[ 0 ] * scale, position.x },
					{ rotation[ 1 ] * scale, position.y }
				}
		{
		}


	/*
	 @	Conversion
	 */
	Affine2::operator Matrix2() const noexcept
		{
			return Matrix2( ( *this )[ 0 ].xy,
							( *this )[ 1 ].xy );
		}


	/*
	 @	Compare
	 */
	Bool Affine2::operator == ( const Affine2& aff2 ) const noexcept
		{
			return ( *this )[ 0 ] == aff2[ 0 ]
				&& ( *this )[ 1 ] == aff2[ 1 ];
		}


	Bool Affine2::operator != ( const Affine2& aff2 ) const noexcept
		{
			return ( *this )[ 0 ] != aff2[ 0 ]
				|| ( *this )[ 1 ] != aff2[ 1 ];
		}


	/*
	 @	Operation
	 */
	Affine2 Affine2::operator - () const noexcept
		{
			return Affine2( -( *this )[ 0 ],
							-( *this )[ 1 ] );
		}

	Affine2 Affine2::operator * ( const Float f ) const noexcept
		{
			return Affine2( ( *this )[ 0 ] * f,
							( *this )[ 1 ] * f );
		}

	Affine2 Affine2::operator + ( const Affine2& aff2 ) const noexcept
		{
			return Affine2( ( *this )[ 0 ] + aff2[ 0 ],
							( *this )[ 1 ] + aff2[ 1 ] );
		}

	Affine2 Affine2::operator - ( const Affine2& aff2 ) const noexcept
		{
			return Affine2( ( *this )[ 0 ] - aff2[ 0 ],
							( *this )[ 1 ] - aff2[ 1 ] );
		}


	/*
	 @	operator * Affine2
	 */
	Affine2 Affine2::operator * ( const Affine2& aff2 ) const noexcept
		{
			return Affine2( this->_11 * aff2._11 + this->_12 * aff2._21,
							this->_11 * aff2._12 + this->_12 * aff2._22,
							this->_11 * aff2._13 + this->_12 * aff2._23 + this->_13,

							this->_21 * aff2._11 + this->_22 * aff2._21,
							this->_21 * aff2._12 + this->_22 * aff2._22,
							this->_21 * aff2._13 + this->_22 * aff2._23 + this->_23 );
		}


	/*
	 @	operator * Matrix2
	 */
	Affine2 Affine2::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Affine2( this->_11 * mat2._11 + this->_12 * mat2._21,
							this->_11 * mat2._12 + this->_12 * mat2._22,
							this->_13,

							this->_21 * mat2._11 + this->_22 * mat2._21,
							this->_21 * mat2._12 + this->_22 * mat2._22,
							this->_23 );
		}


	/*
	 @	operator * Matrix3X2
	 */
	Matrix2 Affine2::operator * ( const Matrix3X2& mat3x2 ) const noexcept
		{
			return Matrix2( this->_11 * mat3x2._11 + this->_12 * mat3x2._21 + this->_13 * mat3x2._31,
							this->_11 * mat3x2._12 + this->_12 * mat3x2._22 + this->_13 * mat3x2._32,

							this->_21 * mat3x2._11 + this->_22 * mat3x2._21 + this->_23 * mat3x2._31,
							this->_21 * mat3x2._12 + this->_22 * mat3x2._22 + this->_23 * mat3x2._32 );
		}


	/*
	 @	Transpose
	 */
	Matrix3X2 Affine2::Transpose() const noexcept
		{
			return Matrix3X2( this->_11, this->_21,
							  this->_12, this->_22,
							  this->_13, this->_23 );
		}


	/*
	 @	Inverse
	 */
	Void Affine2::Inverse( Affine2& aff2 ) const noexcept
		{
			// Inverse matrix2	rot & scale
			Matrix2 inv( Matrix2( this->_11, this->_12,
								  this->_21, this->_22 ).Inverse() );

			// Inverse pos
			Vector2 pos( this->Column( 2 ) * inv );

			aff2 = { inv._11, inv._12, -pos.x,
					 inv._21, inv._22, -pos.y };
		}


	/*
	 @	Inverse
	 */
	Void Affine2::Invert( const Vector2& position,
						  const Vector2& scale,
						  const Float angle ) noexcept
		{
			// Inverse the  rot & scale & pos
			Matrix2 invRot( Rotation( -angle ) );

			Vector2 invScale( scale.Inverse() );

			Vector2 invPos( -position * invScale * invRot );

			// Combine rot & scale & pos
			this->row[ 0 ] = { invRot[ 0 ] * invScale.x, invPos.x };
			this->row[ 1 ] = { invRot[ 1 ] * invScale.y, invPos.y };
		}


	/*
	 @	Rotation
	 */
	Matrix2 Affine2::Rotation( const Float angle ) noexcept
		{
			Float sinA = math::Sin( angle );
			Float cosA = math::Cos( angle );

			return Matrix2( cosA, -sinA,
							sinA,  cosA );
		}


	/*
	 @	Rotation
	 */
	Affine2 Affine2::Rotation( const Float x,
							   const Float y,
							   const Float angle ) noexcept
		{
			Float sinA = math::Sin( angle );
			Float cosA = math::Cos( angle );

			Float cosAInc = 1 - cosA;

			return Affine2( cosA, -sinA, x * cosAInc + y * sinA,
							sinA,  cosA, y * cosAInc - x * sinA );
		}
}



/**
 !	Affine3
 */
namespace fce
{
	/*
	 @	Constant
	 */
	constinit const Affine3 Affine3::ZERO( 0, 0, 0, 0,
										   0, 0, 0, 0,
										   0, 0, 0, 0 );

	constinit const Affine3 Affine3::IDENTITY( 1, 0, 0, 0,
											   0, 1, 0, 0,
											   0, 0, 1, 0 );


	/*
	 @	Constructor
	 */
	Affine3::Affine3( const Vector3& position,
					  const Vector3& scale,
					  const Vector3& angles ) noexcept
		:
			Affine3( position, scale,
					 Rotation( angles ) )
		{
		}

	Affine3::Affine3( const Vector3& position,
					  const Vector3& scale,
					  const Vector3& axis,
					  const Float angle ) noexcept
		:
			Affine3( position, scale,
					 Rotation( axis, angle ) )
		{
		}

	/*
	 @	Constructor

			Rotate => Scale => Translate
	 */
	Affine3::Affine3( const Vector3& position,
					  const Vector3& scale,
					  const Matrix3& rotation ) noexcept
		:
			row
				{
					{ rotation[ 0 ] * scale, position.x },
					{ rotation[ 1 ] * scale, position.y },
					{ rotation[ 2 ] * scale, position.z }
				}
		{
		}


	/*
	 @	Conversion
	 */
	Affine3::operator Matrix2() const noexcept
		{
			return Matrix2( ( *this )[ 0 ].xy,
							( *this )[ 1 ].xy );
		}

	Affine3::operator Matrix3() const noexcept
		{
			return Matrix3( ( *this )[ 0 ].xyz,
							( *this )[ 1 ].xyz,
							( *this )[ 2 ].xyz );
		}


	/*
	 @	Compare
	 */
	Bool Affine3::operator == ( const Affine3& aff3 ) const noexcept
		{
			return ( *this )[ 0 ] == aff3[ 0 ]
				&& ( *this )[ 1 ] == aff3[ 1 ]
				&& ( *this )[ 2 ] == aff3[ 2 ];
		}

	Bool Affine3::operator != ( const Affine3& aff3 ) const noexcept
		{
			return ( *this )[ 0 ] != aff3[ 0 ]
				|| ( *this )[ 1 ] != aff3[ 1 ]
				|| ( *this )[ 2 ] != aff3[ 2 ];
		}


	/*
	 @	Operation
	 */
	Affine3 Affine3::operator - () const noexcept
		{
			return Affine3( -( *this )[ 0 ],
							-( *this )[ 1 ],
							-( *this )[ 2 ] );
		}

	Affine3 Affine3::operator * ( const Float f ) const noexcept
		{
			return Affine3( ( *this )[ 0 ] * f,
							( *this )[ 1 ] * f,
							( *this )[ 2 ] * f );
		}

	Affine3 Affine3::operator + ( const Affine3& aff3 ) const noexcept
		{
			return Affine3( ( *this )[ 0 ] + aff3[ 0 ],
							( *this )[ 1 ] + aff3[ 1 ],
							( *this )[ 2 ] + aff3[ 2 ] );
		}

	Affine3 Affine3::operator - ( const Affine3& aff3 ) const noexcept
		{
			return Affine3( ( *this )[ 0 ] - aff3[ 0 ],
							( *this )[ 1 ] - aff3[ 1 ],
							( *this )[ 2 ] - aff3[ 2 ] );
		}


	/*
	 @	operator * Affine3
	 */
	Affine3 Affine3::operator * ( const Affine3& aff3 ) const noexcept
		{
			return Affine3( this->_11 * aff3._11 + this->_12 * aff3._21 + this->_13 * aff3._31,
							this->_11 * aff3._12 + this->_12 * aff3._22 + this->_13 * aff3._32,
							this->_11 * aff3._13 + this->_12 * aff3._23 + this->_13 * aff3._33,
							this->_11 * aff3._14 + this->_12 * aff3._24 + this->_13 * aff3._34 + this->_14,

							this->_21 * aff3._11 + this->_22 * aff3._21 + this->_23 * aff3._31,
							this->_21 * aff3._12 + this->_22 * aff3._22 + this->_23 * aff3._32,
							this->_21 * aff3._13 + this->_22 * aff3._23 + this->_23 * aff3._33,
							this->_21 * aff3._14 + this->_22 * aff3._24 + this->_23 * aff3._34 + this->_24,

							this->_31 * aff3._11 + this->_32 * aff3._21 + this->_33 * aff3._31,
							this->_31 * aff3._12 + this->_32 * aff3._22 + this->_33 * aff3._32,
							this->_31 * aff3._13 + this->_32 * aff3._23 + this->_33 * aff3._33,
							this->_31 * aff3._14 + this->_32 * aff3._24 + this->_33 * aff3._34 + this->_34 );
		}


	/*
	 @	operator * Affine2
	 */
	Affine3 Affine3::operator * ( const Affine2& aff2 ) const noexcept
		{
			return Affine3( this->_11 * aff2._11 + this->_12 * aff2._21,
							this->_11 * aff2._12 + this->_12 * aff2._22,
							this->_13,
							this->_11 * aff2._13 + this->_12 * aff2._23 + this->_14,

							this->_21 * aff2._11 + this->_22 * aff2._21,
							this->_21 * aff2._12 + this->_22 * aff2._22,
							this->_23,
							this->_21 * aff2._13 + this->_22 * aff2._23 + this->_24,

							this->_31 * aff2._11 + this->_32 * aff2._21,
							this->_31 * aff2._12 + this->_32 * aff2._22,
							this->_33,
							this->_31 * aff2._13 + this->_32 * aff2._23 + this->_34 );
		}


	/*
	 @	operator * Matrix2
	 */
	Affine3 Affine3::operator * ( const Matrix2& mat2 ) const noexcept
		{
			return Affine3( this->_11 * mat2._11 + this->_12 * mat2._21,
							this->_11 * mat2._12 + this->_12 * mat2._22,
							this->_13,
							this->_14,

							this->_21 * mat2._11 + this->_22 * mat2._21,
							this->_21 * mat2._12 + this->_22 * mat2._22,
							this->_23,
							this->_24,

							this->_31 * mat2._11 + this->_32 * mat2._21,
							this->_31 * mat2._12 + this->_32 * mat2._22,
							this->_33,
							this->_34 );
		}


	/*
	 @	operator * Matrix3
	 */
	Affine3 Affine3::operator * ( const Matrix3& mat3 ) const noexcept
		{
			return Affine3( this->_11 * mat3._11 + this->_12 * mat3._21 + this->_13 * mat3._31,
							this->_11 * mat3._12 + this->_12 * mat3._22 + this->_13 * mat3._32,
							this->_11 * mat3._13 + this->_12 * mat3._23 + this->_13 * mat3._33,
							this->_14,

							this->_21 * mat3._11 + this->_22 * mat3._21 + this->_23 * mat3._31,
							this->_21 * mat3._12 + this->_22 * mat3._22 + this->_23 * mat3._32,
							this->_21 * mat3._13 + this->_22 * mat3._23 + this->_23 * mat3._33,
							this->_24,

							this->_31 * mat3._11 + this->_32 * mat3._21 + this->_33 * mat3._31,
							this->_31 * mat3._12 + this->_32 * mat3._22 + this->_33 * mat3._32,
							this->_31 * mat3._13 + this->_32 * mat3._23 + this->_33 * mat3._33,
							this->_34 );
		}


	/*
	 @	operator * Matrix4
	 */
	Matrix4 Affine3::operator * ( const Matrix4& mat4 ) const noexcept
		{
			return Matrix4( this->_11 * mat4._11 + this->_12 * mat4._21 + this->_13 * mat4._31 + this->_14 * mat4._41,
							this->_11 * mat4._12 + this->_12 * mat4._22 + this->_13 * mat4._32 + this->_14 * mat4._42,
							this->_11 * mat4._13 + this->_12 * mat4._23 + this->_13 * mat4._33 + this->_14 * mat4._43,
							this->_11 * mat4._14 + this->_12 * mat4._24 + this->_13 * mat4._34 + this->_14 * mat4._44,

							this->_21 * mat4._11 + this->_22 * mat4._21 + this->_23 * mat4._31 + this->_24 * mat4._41,
							this->_21 * mat4._12 + this->_22 * mat4._22 + this->_23 * mat4._32 + this->_24 * mat4._42,
							this->_21 * mat4._13 + this->_22 * mat4._23 + this->_23 * mat4._33 + this->_24 * mat4._43,
							this->_21 * mat4._14 + this->_22 * mat4._24 + this->_23 * mat4._34 + this->_24 * mat4._44,

							this->_31 * mat4._11 + this->_32 * mat4._21 + this->_33 * mat4._31 + this->_34 * mat4._41,
							this->_31 * mat4._12 + this->_32 * mat4._22 + this->_33 * mat4._32 + this->_34 * mat4._42,
							this->_31 * mat4._13 + this->_32 * mat4._23 + this->_33 * mat4._33 + this->_34 * mat4._43,
							this->_31 * mat4._14 + this->_32 * mat4._24 + this->_33 * mat4._34 + this->_34 * mat4._44,

							mat4._41,
							mat4._42,
							mat4._43,
							mat4._44 );
		}


	/*
	 @	operator * Matrix4X3
	 */
	Matrix3 Affine3::operator * ( const Matrix4X3& mat4x3 ) const noexcept
		{
			return Matrix3( this->_11 * mat4x3._11 + this->_12 * mat4x3._21 + this->_13 * mat4x3._31 + this->_14 * mat4x3._41,
							this->_11 * mat4x3._12 + this->_12 * mat4x3._22 + this->_13 * mat4x3._32 + this->_14 * mat4x3._42,
							this->_11 * mat4x3._13 + this->_12 * mat4x3._23 + this->_13 * mat4x3._33 + this->_14 * mat4x3._43,

							this->_21 * mat4x3._11 + this->_22 * mat4x3._21 + this->_23 * mat4x3._31 + this->_24 * mat4x3._41,
							this->_21 * mat4x3._12 + this->_22 * mat4x3._22 + this->_23 * mat4x3._32 + this->_24 * mat4x3._42,
							this->_21 * mat4x3._13 + this->_22 * mat4x3._23 + this->_23 * mat4x3._33 + this->_24 * mat4x3._43,

							this->_31 * mat4x3._11 + this->_32 * mat4x3._21 + this->_33 * mat4x3._31 + this->_34 * mat4x3._41,
							this->_31 * mat4x3._12 + this->_32 * mat4x3._22 + this->_33 * mat4x3._32 + this->_34 * mat4x3._42,
							this->_31 * mat4x3._13 + this->_32 * mat4x3._23 + this->_33 * mat4x3._33 + this->_34 * mat4x3._43 );
		}


	/*
	 @	Transpose
	 */
	Matrix4X3 Affine3::Transpose() const noexcept
		{
			return Matrix4X3( this->_11, this->_21, this->_31,
							  this->_12, this->_22, this->_32,
							  this->_13, this->_23, this->_33,
							  this->_14, this->_24, this->_34 );
		}


	/*
	 @	Inverse
	 */
	Bool Affine3::Inverse( Affine3& aff3 ) const noexcept
		{
			// adjugate matrix3 col 1
			Float t00 = this->_33 * this->_22 - this->_32 * this->_23,
				  t10 = this->_31 * this->_23 - this->_33 * this->_21,
				  t20 = this->_32 * this->_21 - this->_31 * this->_22;

			// adjugate matrix3 determinant
			Float det = this->_11 * t00
					  + this->_12 * t10
					  + this->_13 * t20;

			if( Equal( det, 0 ) )
				{
					return false;
				}

			Float invDet = math::Inverse( det );

			// Shortcut operation
			Float m00 = this->_11 * invDet,
				  m01 = this->_12 * invDet,
				  m02 = this->_13 * invDet;

			// inverse matrix3
			Float r00 = t00 * invDet,
				  r01 = m02 * this->_32 - m01 * this->_33,
				  r02 = m01 * this->_23 - m02 * this->_22;

			Float r10 = t10 * invDet,
				  r11 = m00 * this->_33 - m02 * this->_31,
				  r12 = m02 * this->_21 - m00 * this->_23;

			Float r20 = t20 * invDet,
				  r21 = m01 * this->_31 - m00 * this->_32,
				  r22 = m00 * this->_22 - m01 * this->_21;

			// inverse col 4
			Float r03 = -( r00 * this->_14 + r01 * this->_24 + r02 * this->_34 ),
				  r13 = -( r10 * this->_14 + r11 * this->_24 + r12 * this->_34 ),
				  r23 = -( r20 * this->_14 + r21 * this->_24 + r22 * this->_34 );

			aff3 = { r00, r01, r02, r03,
					 r10, r11, r12, r13,
					 r20, r21, r22, r23 };

			return true;
		}


	/*
	 @	Invert
	 */
	Void Affine3::Invert( const Vector3& pos,
						  const Vector3& scale,
						  const Quaternion& orien )
		{
			FC_ASSERT_LOG( orien.isUnit2(),
							"The quaternion must be normalized...");

			// Inverse the  rot & scale & pos
			Quaternion invOrient( orien.UnitInverse() );

			Vector3 invScale( scale.Inverse() );

			Vector3 invPos( invOrient * -pos * invScale );

			// Combine rot & scale & pos
			Matrix3 invRot( invOrient );

			this->row[ 0 ] = { invRot[ 0 ] * invScale.x, invPos.x };
			this->row[ 1 ] = { invRot[ 1 ] * invScale.y, invPos.y };
			this->row[ 2 ] = { invRot[ 2 ] * invScale.z, invPos.z };
		}


	/*
	 @	Update scale & rotation
	 */
	Void Affine3::UpdateRotScale( const Matrix3& rot,
								  const Vector3& scale ) noexcept
		{
			this->row[ 0 ].xyz = rot[ 0 ] * scale;
			this->row[ 1 ].xyz = rot[ 1 ] * scale;
			this->row[ 2 ].xyz = rot[ 2 ] * scale;
		}


	/*
	 @	Rotation X
	 */
	Matrix3 Affine3::RotationX( const Float angle ) noexcept
		{
			Float sinX = math::Sin( angle );
			Float cosX = math::Cos( angle );

			return Matrix3( 1,    0,	 0,
							0, cosX, -sinX,
							0, sinX,  cosX );
		}


	/*
	 @	Rotation Y
	 */
	Matrix3 Affine3::RotationY( const Float angle ) noexcept
		{
			Float sinY = math::Sin( angle );
			Float cosY = math::Cos( angle );

			return Matrix3( cosY, 0, sinY,
							   0, 1,	0,
						   -sinY, 0, cosY );
		}


	/*
	 @	Rotation Z
	 */
	Matrix3 Affine3::RotationZ( const Float angle ) noexcept
		{
			Float sinZ = math::Sin( angle );
			Float cosZ = math::Cos( angle );

			return Matrix3( cosZ, -sinZ, 0,
							sinZ,  cosZ, 0,
							   0,     0, 1 );
		}


	/*
	 @	Rotation
	 */
	Matrix3 Affine3::Rotation( const Vector3& angles ) noexcept
		{
			Vector3 sin( math::Sin( angles.x ),
						 math::Sin( angles.y ),
						 math::Sin( angles.z ) );

			Vector3 cos( math::Cos( angles.x ),
						 math::Cos( angles.y ),
						 math::Cos( angles.z ) );

			return Matrix3( cos.x * cos.z - sin.x * cos.y * sin.z,
						   -cos.x * sin.z - sin.x * cos.y * cos.z,
						    sin.x * sin.y,
						   
						    cos.x * cos.y * sin.z + sin.x * cos.z,
						    cos.x * cos.y * cos.z - sin.x * sin.z,
						   -cos.x * sin.y,
						   
						    sin.y * sin.z,
						    sin.y * cos.z,
						    cos.y );
		}


	/*
	 @	Rotation
	 */
	Matrix3 Affine3::Rotation( const Vector3& axis,
							   const Float angle ) noexcept
		{
			Float sinA = math::Sin( angle );
			Float cosA = math::Cos( angle );
			Float cosAInc = 1 - cosA;

			return Matrix3( cosAInc * axis.x * axis.x + cosA,
							cosAInc * axis.y * axis.x + sinA * axis.z,
							cosAInc * axis.z * axis.x - sinA * axis.y,
							
							cosAInc * axis.x * axis.y - sinA * axis.z,
							cosAInc * axis.y * axis.y + cosA,
							cosAInc * axis.z * axis.y + sinA * axis.x,

							cosAInc * axis.x * axis.z + sinA * axis.y,
							cosAInc * axis.y * axis.z - sinA * axis.x,
							cosAInc * axis.z * axis.z + cosA );
		}


	/*
	 @	Perspective
	
			xScale	0		0			 0
			0		yScale	0			 0
			0		0		(n+f)/(n-f)	 2*n*f/(n-f)
			0		0		-1			 0
	
		yScale = cot( fovY / 2 )
		xScale = yScale / aspect ratio


	 Matrix4 Affine3::Perspective( Float fovy,
								  Float aspect,
								  const Range& zRange )
		{
			Float inv = - Float_ONE / zRange.Size();
			Float zScale = ( zRange.min + zRange.max ) * inv;
			Float zOffst = math::Double( zRange.min * zRange.max ) * inv;

			Float yScale = math::Cot( math::Half( fovy ) );
			Float xScale = yScale / aspect;

			return Matrix4( xScale, 0,		0,		0,
							0,		yScale, 0,		0,
							0,		0,		zScale, zOffst,
							0,		0,		-1,		0	   );
		}
	 */
}