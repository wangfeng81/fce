/*
**
**	Scene node.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/scene/Scene.h"


/**
 !	class Node component
 */
namespace fce::inner::scene
{
	/*
	 @	���� Node
	 */
	NodeComponent::NodeComponent( Node& node ) noexcept
		:
			node( node )
		{
		}

	/*
	 @	���� Entity
	 */
	NodeComponent::NodeComponent( Entity& entity ) NOEXCEPT
		:
			node( static_cast< Node& >( entity ) )
		{
			FC_ASSERT( typeid( entity ) == typeid( Node ) );
		}
}



/**
 !	class Node
 */
namespace fce::inner::scene
{
	/*
	 @	���� Ĭ��
	 */
	Node::Node( Node* const parent ) noexcept
		:
			TreeNode( parent )
		{
		}

	/*
	 @	���� buffer
	 */
	Node::Node( Node* const parent,
				const Buffer& buf )
		:
			TreeNode( parent ),
			transform( buf )
		{
			this->Update();

			this->DeserializeChildren( buf );

			this->DeserializeComponents( buf );
		}

	/*
	 @	���� copy
	 */
	Node::Node( Node* const parent,
				const Node& node ) NOEXCEPT
		:
			TreeNode( parent ),
			transform( node.transform )
		{
			this->Update();

			this->CopyChildren( node );

			this->CopyComponents( node );
		}

	/*
	 @	���� move
	 */
	Node::Node( Node* const parent,
				Node&& node ) noexcept
		:
			TreeNode( parent ),
			transform( std::move( transform ) )
		{
			this->MoveChildren( std::move( node ) );

			this->Update();

			this->MoveComponents( std::move( node ) );
		}

	/*
	 @	���� transform
	 */
	Node::Node( Node* const parent,
				const Vector3& pos,
				const Vector3& scale,
				const Quaternion& orien ) noexcept
		:
			TreeNode( parent ),
			transform( pos, scale, orien )
		{
			this->Update();
		}

	Node::Node( Node* const parent,
				const Transform& transform ) noexcept
		:
			TreeNode( parent ),
			transform( transform )
		{
			this->Update();
		}
}



namespace fce::inner::scene
{
	/*
	 @	��ֵ move
	 */
	Node& Node::operator = ( Node&& node ) noexcept
		{
			this->transform = std::move( node.transform );

			TreeNode::operator = ( std::move( node ) );

			Entity::operator = ( std::move( node ) );

			return *this;
		}

	/*
	 @	��ֵ copy
	 */
	Node& Node::operator = ( const Node& node ) NOEXCEPT
		{
			this->transform = node.transform;

			TreeNode::operator = ( node );

			Entity::operator = ( node );

			return *this;
		}

	/*
	 @	���л�
	 */
	Buffer& Node::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			buf << this->transform;

			this->SerializeChildren( buf );

			return this->SerializeComponents( buf );
		}
}



namespace fce::inner::scene
{
	/*
	 @	����
	 */
	Void Node::Destroy() noexcept
		{
			if( this->parent )
				{
					this->parent->RemoveChild( this );
				}
			else
				{
					this->GetScene< false >().RemoveNode( this );
				}
		}

	/*
	 @	��ȡ ����
	 */
	UInt Node::GetIndex() const noexcept
		{
			if( this->parent )
				{
					return this->parent->GetIndex( this );
				}
			else
				{
					return this->GetScene< false >().GetIndex( this );
				}
		}
}



namespace fce::inner::scene
{
	/*
	 @	�ƶ� ǰһ��
	 */
	Void Node::MoveUp() noexcept
		{
			if( this->parent )
				{
					this->parent->MoveUp( this );
				}
			else
				{
					this->GetScene< false >().MoveUp( this );
				}
		}

	/*
	 @	�ƶ� ��һ��
	 */
	Void Node::MoveDown() noexcept
		{
			if( this->parent )
				{
					this->parent->MoveDown( this );
				}
			else
				{
					this->GetScene< false >().MoveDown( this );
				}
		}

	/*
	 @	�ƶ� β��
	 */
	Void Node::MoveEnd() noexcept
		{
			if( this->parent )
				{
					this->parent->MoveEnd( this );
				}
			else
				{
					this->GetScene< false >().MoveEnd( this );
				}
		}

	/*
	 @	�ƶ� ͷ��
	 */
	Void Node::MoveBegin() noexcept
		{
			if( this->parent )
				{
					this->parent->MoveBegin( this );
				}
			else
				{
					this->GetScene< false >().MoveBegin( this );
				}
		}

	/*
	 @	���� Parent
	 */
	Void Node::SetParent( Node* const newParent ) noexcept
		{
			if( this->parent == newParent )
				{
					return;
				}

			if( this->parent )
				{
					this->parent->EraseChild( this );
				}
			else
				{
					this->GetScene< false >().EraseNode( this );
				}

			if( newParent )
				{
					newParent->PushChild( this );
				}
			else
				{
					this->GetScene< true >().PushNode( this );
				}

			this->Update();
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� local x y z
	 */
	Void Node::SetX( const Float x ) noexcept
		{
			this->transform.position.x = x;

			this->Update( EVENT::TRANSLATION );
		}

	Void Node::SetY( const Float y ) noexcept
		{
			this->transform.position.y = y;

			this->Update( EVENT::TRANSLATION );
		}

	Void Node::SetZ( const Float z ) noexcept
		{
			this->transform.position.z = z;

			this->Update( EVENT::TRANSLATION );
		}

	/*
	 @	���� local position
	 */
	Void Node::SetPosition( const Float x,
							const Float y ) noexcept
		{
			this->SetPosition( Vector2( x, y ) );
		}

	Void Node::SetPosition( const Float x,
							const Float y,
							const Float z ) noexcept
		{
			this->SetPosition( Vector3( x, y, z ) );
		}

	Void Node::SetPosition( const Vector2& pos ) noexcept
		{
			this->transform.position.xy = pos;

			this->Update( EVENT::TRANSLATION );
		}

	Void Node::SetPosition( const Vector3& pos ) noexcept
		{
			this->transform.position = pos;

			this->Update( EVENT::TRANSLATION );
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� local scale x y z
	 */
	Void Node::SetScaleX( const Float x ) noexcept
		{
			this->transform.scale.x = x;

			this->Update( EVENT::SCALE );
		}

	Void Node::SetScaleY( const Float y ) noexcept
		{
			this->transform.scale.y = y;

			this->Update( EVENT::SCALE );
		}

	Void Node::SetScaleZ( const Float z ) noexcept
		{
			this->transform.scale.z = z;

			this->Update( EVENT::SCALE );
		}

	/*
	 @	���� local scale
	 */
	Void Node::SetScale( const Float s ) noexcept
		{
			this->SetScale( Vector3( s ) );
		}

	Void Node::SetScale( const Float x,
						 const Float y ) noexcept
		{
			this->SetScale( Vector2( x, y ) );
		}

	Void Node::SetScale( const Float x,
						 const Float y,
						 const Float z ) noexcept
		{
			this->SetScale( Vector3( x, y, z ) );
		}

	Void Node::SetScale( const Vector2& scale ) noexcept
		{
			this->transform.scale.xy = scale;

			this->Update( EVENT::SCALE );
		}

	Void Node::SetScale( const Vector3& scale ) noexcept
		{
			this->transform.scale = scale;

			this->Update( EVENT::SCALE );
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� local angles
	 */
	Void Node::SetAngleX( const Float x ) noexcept
		{
			this->transform.orientation = Quaternion::AngleX( x );

			this->Update( EVENT::ROTATION );
		}

	Void Node::SetAngleY( const Float y ) noexcept
		{
			this->transform.orientation = Quaternion::AngleY( y );

			this->Update( EVENT::ROTATION );
		}

	Void Node::SetAngleZ( const Float z ) noexcept
		{
			this->transform.orientation = Quaternion::AngleZ( z );

			this->Update( EVENT::ROTATION );
		}

	Void Node::SetAngles( const Float x,
						  const Float y,
						  const Float z ) noexcept
		{
			this->SetAngles( Vector3( x, y, z ) );
		}

	Void Node::SetAngles( const Vector3& angles ) noexcept
		{
			this->transform.orientation = angles;

			this->Update( EVENT::ROTATION );
		}

	/*
	 @	���� local orientation
	 */
	Void Node::SetOrientation( const Vector3& axis,
							   const Float angle ) noexcept
		{
			this->transform.orientation = { axis, angle };

			this->Update( EVENT::ROTATION );
		}

	Void Node::SetOrientation( const Vector3& xAxis,
							   const Vector3& yAxis,
							   const Vector3& zAxis ) noexcept
		{
			this->transform.orientation = { xAxis, yAxis, zAxis };

			this->Update( EVENT::ROTATION );
		}

	Void Node::SetOrientation( const Quaternion& orien ) noexcept
		{
			this->transform.orientation = orien.Normalized();

			this->Update( EVENT::ROTATION );
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� transform
	 */
	Void Node::SetTransform( const Vector3& pos,
							 const Vector3& scale,
							 const Quaternion& orien ) noexcept
		{
			this->transform.Combine( pos, scale, orien );

			this->Update( EVENT::COMBINATION );
		};

	Void Node::SetTransform( const Transform& transform ) noexcept
		{
			this->transform = transform;

			this->Update( EVENT::COMBINATION );
		};
}



namespace fce::inner::scene
{
	/*
	 @	λ��
	 */
	Void Node::Translate( const Float x,
						  const Float y ) noexcept
		{
			this->Translate( Vector2( x, y ) );
		}

	Void Node::Translate( const Float x,
						  const Float y,
						  const Float z ) noexcept
		{
			this->Translate( Vector3( x, y, z ) );
		}

	Void Node::Translate( const Vector2& offset ) noexcept
		{
			this->SetPosition( this->transform.position.xy + offset );
		}

	Void Node::Translate( const Vector3& offset ) noexcept
		{
			this->SetPosition( this->transform.position + offset );
		}
}



namespace fce::inner::scene
{
	/*
	 @	��ת
	 */
	Void Node::RotateX( const Float angle ) noexcept
		{
			this->Rotate( Quaternion::AngleX( angle ) );
		}

	Void Node::RotateY( const Float angle ) noexcept
		{
			this->Rotate( Quaternion::AngleY( angle ) );
		}

	Void Node::RotateZ( const Float angle ) noexcept
		{
			this->Rotate( Quaternion::AngleZ( angle ) );
		}

	Void Node::Rotate( const Float xAngle,
					   const Float yAngle,
					   const Float zAngle ) noexcept
		{
			this->Rotate( Quaternion( xAngle, yAngle, zAngle ) );
		}

	Void Node::Rotate( const Vector3& axis,
					   const Float angle ) noexcept
		{
			this->Rotate( Quaternion( axis, angle ) );
		}

	Void Node::Rotate( const Quaternion& q ) noexcept
		{
			this->SetOrientation( this->transform.orientation * q );
		}
}



namespace fce::inner::scene
{
	/*
	 @	�任 ȫ�� ����
	 */
	Vector3 Node::PosToLocal( const Vector3& global ) const noexcept
		{
			return this->GlobalOrientation().UnitInverse()
				 * ( global - this->GlobalPosition() )
				 / this->GlobalScale();
		}

	Vector3 Node::PosToGlobal( const Vector3& local ) const noexcept
		{
			return local * this->globalMatrix;
		}

	Vector3 Node::ScaleToLocal( const Vector3& global ) const noexcept
		{
			return global / this->GlobalScale();
		}

	Vector3 Node::ScaleToGlobal( const Vector3& local ) const noexcept
		{
			return local * this->GlobalScale();
		}

	Quaternion Node::OrienToLocal( const Quaternion& global ) const noexcept
		{
			return this->GlobalOrientation().UnitInverse()
				 * global;
		}

	Quaternion Node::OrienToGlobal( const Quaternion& local ) const noexcept
		{
			return this->GlobalOrientation() * local;
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� ȫ�� pos scale orient
	 */
	Void Node::SetGlobalScale( const Vector3& scale ) noexcept
		{
			if( this->parent )
				{
					this->globalTransform.scale = scale;

					this->UpdateGlobalSub( EVENT::SCALE );
				}
			else
				{
					this->SetScale( scale );
				}
		}

	Void Node::SetGlobalPosition( const Vector3& pos ) noexcept
		{
			if( this->parent )
				{
					this->globalTransform.position = pos;

					this->UpdateGlobalSub( EVENT::TRANSLATION );
				}
			else
				{
					this->SetPosition( pos );
				}
		}

	Void Node::SetGlobalOrientation( const Quaternion& orien ) noexcept
		{
			if( this->parent )
				{
					this->globalTransform.orientation = orien;

					this->UpdateGlobalSub( EVENT::ROTATION );
				}
			else
				{
					this->SetOrientation( orien );
				}
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� ȫ�� pos scale orient ( Syntax sugar )
	 */
	Void Node::SetGlobalScale( const Float x,
							   const Float y,
							   const Float z ) noexcept
		{
			this->SetGlobalScale( Vector3( x, y, z ) );
		}

	Void Node::SetGlobalPosition( const Float x,
								  const Float y,
								  const Float z ) noexcept
		{
			this->SetGlobalPosition( Vector3( x, y, z ) );
		}

	Void Node::SetGlobalOrientation( const Float xAngle,
									 const Float yAngle,
									 const Float zAngle ) noexcept
		{
			this->SetGlobalOrientation( Vector3( xAngle, yAngle, zAngle ) );
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� ȫ�� Transform
	 */
	Void Node::SetGlobalTransform( const Transform& transform ) noexcept
		{
			this->SetGlobalTransform( transform.position,
									  transform.scale,
									  transform.orientation );
		}

	Void Node::SetGlobalTransform( const Vector3& pos,
								   const Vector3& scale,
								   const Quaternion& orien ) noexcept
		{
			if( this->parent )
				{
					this->globalTransform.Combine( pos, scale, orien );

					this->UpdateGlobalSub( EVENT::COMBINATION );
				}
			else
				{
					this->SetTransform( pos, scale, orien );
				}
		}
}



namespace fce::inner::scene
{
	/*
	 @	���� transform
	 */
	Void Node::Update( const EVENT e ) noexcept
		{
			// ����ȫ�ֱ任
			if( this->parent )
				{
					this->UpdateSub( e );
				}
			else
				{
					this->UpdateRoot( e );
				}
		};

	/*
	 @	���� global transform
	 */
	Void Node::UpdateGlobal( const EVENT e ) noexcept
		{
			// ���±��ر任
			if( this->parent )
				{
					this->UpdateGlobalSub( e );
				}
			else
				{
					this->UpdateRoot( e );
				}
		};

	/*
	 @	���� ȫ�ֱ任 ( �޸��ڵ� )
	 */
	Void Node::UpdateRoot( const EVENT e ) noexcept
		{
			switch( e )
				{
					[[unlikely]]	case EVENT::SCALE:
										{
											this->globalTransform.scale = this->Scale();

											// ���� �任����
											this->globalMatrix.UpdateRotScale( this->GlobalOrientation(),
																			   this->GlobalScale() );
										}
										break;

									case EVENT::ROTATION:
										{
											this->globalTransform.orientation = this->Orientation();

											// ���� �任����
											this->globalMatrix.UpdateRotScale( this->GlobalOrientation(),
																			   this->GlobalScale() );
										}
										break;

					[[likely]]		case EVENT::TRANSLATION:
										{
											this->globalTransform.position = this->Position();

											// ���� �任����
											this->globalMatrix.UpdateTranslation( this->GlobalPosition() );
										}
										break;

					[[likely]]		case EVENT::COMBINATION:
										{
											this->globalTransform = this->transform;

											// �ϳ� �任����
											this->globalMatrix = *this->globalTransform;
										}
										break;

					// ��Ӧ�� δ���������
					default: std::unreachable();
				}

			// ͨ�ø���
			this->UpdateCommon( e );
		};

	/*
	 @	���� ȫ�ֱ任 ( �и��ڵ� )
	 */
	Void Node::UpdateSub( const EVENT e ) noexcept
		{
			switch( e )
				{
					[[unlikely]]	case EVENT::SCALE:
										{
											this->globalTransform.scale = this->parent->GlobalScale()
																	    * this->Scale();
										}
										break;

									case EVENT::ROTATION:
										{
											this->globalTransform.orientation = this->parent->GlobalOrientation()
																			  * this->Orientation();

											this->globalTransform.orientation.Normalize();
										}
										break;

					[[likely]]		case EVENT::TRANSLATION:
										{
										}
										break;

					[[likely]]		case EVENT::COMBINATION:
										{
											this->globalTransform.scale = this->parent->GlobalScale()
																	    * this->Scale();

											this->globalTransform.orientation = this->parent->GlobalOrientation()
																			  * this->Orientation();

											this->globalTransform.orientation.Normalize();
										}
										break;

					// ��Ӧ�� δ���������
					default: std::unreachable();
				}

			// ���� λ��
			this->globalTransform.position = this->parent->GlobalPosition()
										   + this->parent->GlobalOrientation()
										   * ( this->parent->GlobalScale() * this->Position() );

			// �ϳɱ任����
			this->globalMatrix = *this->globalTransform;

			// ͨ�ø���
			this->UpdateCommon( e );
		};

	/*
	 @	���� ���ر任 ( �и��ڵ� )
	 */
	Void Node::UpdateGlobalSub( const EVENT e ) noexcept
		{
			switch( e )
				{
					[[unlikely]]	case EVENT::SCALE:
										{
											this->transform.scale = this->parent->ScaleToLocal( this->GlobalScale() );
										}
										break;

									case EVENT::ROTATION:
										{
											this->transform.orientation = this->parent->OrienToLocal( this->GlobalOrientation() );

											this->transform.orientation.Normalize();
										}
										break;

					[[likely]]		case EVENT::TRANSLATION:
										{
										}
										break;

					[[likely]]		case EVENT::COMBINATION:
										{
											this->transform.scale = this->parent->ScaleToLocal( this->GlobalScale() );

											this->transform.orientation = this->parent->OrienToLocal( this->GlobalOrientation() );

											this->transform.orientation.Normalize();
										}
										break;

					// ��Ӧ�� δ���������
					default: std::unreachable();
				}

			// ���� λ��
			this->transform.position = this->parent->PosToLocal( this->GlobalPosition() );

			// �ϳɱ任����
			this->globalMatrix = *this->globalTransform;

			// ͨ�ø���
			this->UpdateCommon( e );
		};

	/*
	 @	ͨ�ø���
	 */
	Void Node::UpdateCommon( const EVENT e ) noexcept
		{
			this->NotifyHandlers( e, *this );

			// �����ӽڵ�
			for( Node* const child : this->children )
				{
					child->UpdateSub( e );
				}
		};
}



namespace fce::inner::scene
{
	/*
	 !	Editor mode
	 */
	#ifdef FC_EDITOR

		/*
		 @	���� xml
		 */
		Node::Node( Node* const parent,
					const Xml::Deserialization& des ) NOEXCEPT
			:
				TreeNode( parent )
			{
				*this << des;
			}

		/*
		 @	Serialize xml
		 */
		Void Node::operator >> ( Xml::Serialization& ser ) const NOEXCEPT
			{
				ser.Serialize( this->transform,
							   Transform::ARCHIVE_NAME );

				this->SerializeComponents( ser );

				this->SerializeChildren( ser );
			}

		/*
		 @	Deserialize xml
		 */
		Void Node::operator << ( const Xml::Deserialization& des ) NOEXCEPT
			{
				des.Deserialize( this->transform,
								 Transform::ARCHIVE_NAME );

				this->DeserializeChildren( des );

				this->DeserializeComponents( des );
			}

	#endif
}