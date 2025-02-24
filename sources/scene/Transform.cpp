/*
**
**	Transform.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/scene/Transform.h"


namespace fce
{
	/*
	!	Constant
	*/
	const Transform Transform::IDENTITY( Vector3::ZERO,
										 Vector3::ONE,
										 Quaternion::IDENTITY );

	/*
	 @	构造 归一化
	 */
	Transform::Transform() noexcept
		:
			Transform( IDENTITY )
		{
		}

	/*
	 @	构造 Rotate => Scale => Translate
	 */
	Transform::Transform( const Vector3& pos,
						  const Vector3& scale,
						  const Quaternion& orien ) noexcept
		:
			scale( scale ),
			position( pos ),
			orientation( orien )
		{
		}

	/*
	 @	构造 copy
	 */
	Transform::Transform( const Transform& transform ) noexcept
		:
			scale( transform.scale ),
			position( transform.position ),
			orientation( transform.orientation )
		{
		}

	/*
	 @	赋值 copy
	 */
	Transform& Transform::operator = ( const Transform& transform ) noexcept
		{
			this->scale = transform.scale;
			this->position = transform.position;
			this->orientation = transform.orientation;

			return *this;
		}

	/*
	 @	组合变换
	 */
	Void Transform::Combine( const Vector3& pos,
							 const Vector3& scale,
							 const Quaternion& orien ) noexcept
		{
			this->scale = scale;
			this->position = pos;
			this->orientation = orien;
		};
}



namespace fce
{
	/*
	 @	转换 仿射矩阵
	 */
	Affine3 Transform::operator * () const noexcept
		{
			return Affine3( this->position,
							this->scale,
							this->orientation );
		}

	/*
	 @	负运算
	 */
	Transform Transform::operator - () const noexcept
		{
			return Transform( -this->position,
							  -this->scale,
							  -this->orientation );
		}
}



namespace fce
{
	/*
	 @	运算 +
	 */
	Transform Transform::operator + ( const Transform& transform ) const noexcept
		{
			return Transform( this->position	+ transform.position,
							  this->scale		* transform.scale,
							  this->orientation	* transform.orientation );
		}

	/*
	 @	运算 -
	 */
	Transform Transform::operator - ( const Transform& transform ) const noexcept
		{
			return Transform( this->position	- transform.position,
							  this->scale		/ transform.scale,
							  this->orientation	* transform.orientation );
		}
}