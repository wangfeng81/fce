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
	 @	���� ��һ��
	 */
	Transform::Transform() noexcept
		:
			Transform( IDENTITY )
		{
		}

	/*
	 @	���� Rotate => Scale => Translate
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
	 @	���� copy
	 */
	Transform::Transform( const Transform& transform ) noexcept
		:
			scale( transform.scale ),
			position( transform.position ),
			orientation( transform.orientation )
		{
		}

	/*
	 @	��ֵ copy
	 */
	Transform& Transform::operator = ( const Transform& transform ) noexcept
		{
			this->scale = transform.scale;
			this->position = transform.position;
			this->orientation = transform.orientation;

			return *this;
		}

	/*
	 @	��ϱ任
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
	 @	ת�� �������
	 */
	Affine3 Transform::operator * () const noexcept
		{
			return Affine3( this->position,
							this->scale,
							this->orientation );
		}

	/*
	 @	������
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
	 @	���� +
	 */
	Transform Transform::operator + ( const Transform& transform ) const noexcept
		{
			return Transform( this->position	+ transform.position,
							  this->scale		* transform.scale,
							  this->orientation	* transform.orientation );
		}

	/*
	 @	���� -
	 */
	Transform Transform::operator - ( const Transform& transform ) const noexcept
		{
			return Transform( this->position	- transform.position,
							  this->scale		/ transform.scale,
							  this->orientation	* transform.orientation );
		}
}