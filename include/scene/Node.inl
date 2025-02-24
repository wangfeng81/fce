/*
**
**	Scene node.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::scene
{
	/*
	 @	Get angles
	 */
	FC_INLINE Float Node::AngleX() const NOEXCEPT
		{
			return this->transform.AngleX();
		}

	FC_INLINE Float Node::AngleY() const NOEXCEPT
		{
			return this->transform.AngleY();
		}

	FC_INLINE Float Node::AngleZ() const NOEXCEPT
		{
			return this->transform.AngleZ();
		}

	/*
	 @	Get axles
	 */
	FC_INLINE Matrix3 Node::Axes() const noexcept
		{
			return this->transform.Axes();
		}

	FC_INLINE Vector3 Node::AxisX() const noexcept
		{
			return this->transform.AxisX();
		}

	FC_INLINE Vector3 Node::AxisY() const noexcept
		{
			return this->transform.AxisY();
		}

	FC_INLINE Vector3 Node::AxisZ() const noexcept
		{
			return this->transform.AxisZ();
		}
}



namespace fce::inner::scene
{
	/*
	 @	Local space
	 */
	FC_INLINE const Vector3& Node::Scale() const noexcept
		{
			return this->transform.scale;
		}

	FC_INLINE const Vector3& Node::Position() const noexcept
		{
			return this->transform.position;
		}

	FC_INLINE const Quaternion& Node::Orientation() const noexcept
		{
			return this->transform.orientation;
		}

	/*
	 @	Global space
	 */
	FC_INLINE const Vector3& Node::GlobalScale() const noexcept
		{
			return this->globalTransform.scale;
		}

	FC_INLINE const Vector3& Node::GlobalPosition() const noexcept
		{
			return this->globalTransform.position;
		}

	FC_INLINE const Quaternion& Node::GlobalOrientation() const noexcept
		{
			return this->globalTransform.orientation;
		}

	FC_INLINE const Transform& Node::GlobalTransform() const noexcept
		{
			return this->globalTransform;
		}

	FC_INLINE const Affine3& Node::GlobalMatrix() const noexcept
		{
			return this->globalMatrix;
		}
}



namespace fce::inner::scene
{
	/*
	 @	Pitch & Yaw & Roll
	 */
	FC_INLINE Void Node::Yaw( const Float angle ) noexcept
		{
			this->Rotate( Vector3::UNIT_Y, angle );
		}

	FC_INLINE Void Node::Roll( const Float angle ) noexcept
		{
			this->Rotate( Vector3::UNIT_Z, angle );
		}

	FC_INLINE Void Node::Pitch( const Float angle ) noexcept
		{
			this->Rotate( Vector3::UNIT_X, angle );
		}
}



namespace fce::inner::scene
{
	/*
	 @	Remove child by node component
	 */
	FC_INLINE Void Node::RemoveChild( NodeComponent& nodeCmpt ) NOEXCEPT
		{
			this->RemoveChild( nodeCmpt.node );
		}

	FC_INLINE Void Node::RemoveChild( NodeComponent* const nodeCmpt ) NOEXCEPT
		{
			FC_ASSERT( nodeCmpt );

			this->RemoveChild( nodeCmpt->node );
		}
}



namespace fce::inner::scene
{
	/*
	 @	Add child with component
	 */
	template< X_component T, typename ...Args >

	FC_INLINE T& Node::AddChild( Args&& ...args ) NOEXCEPT
		{
			T* const childComponent
				{
					this->NewChild< T >
						(
							std::forward< Args >( args )...
						)
				};

			FC_ASSERT( childComponent );

			return *childComponent;
		}

	/*
	 @	New child component

			with no transform
	 */
	template< X_component T, typename ...Args >

	T* Node::NewChild( Args&& ...args ) NOEXCEPT
		{
			Node* const child
				{
					this->NewChild()
				};

			return child->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	New child component

			with transform
	 */
	template< X_component T, typename ...Args >

	T* Node::NewChild( const Transform& transform,
					   Args&& ...args ) NOEXCEPT
		{
			Node* const child
				{
					this->NewChild( transform )
				};

			return child->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	New child with component

			with pos scale orien
	 */
	template< X_component T, typename ...Args >

	T* Node::NewChild( const Vector3& pos,
					   const Vector3& scale,
					   const Quaternion& orien,
					   Args&& ...args ) NOEXCEPT
		{
			Node* const child
				{
					this->NewChild( pos, scale, orien )
				};

			return child->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}
}



namespace fce::inner::scene
{
	/*
	 @	Get component by Name
	 */
	template< X_component T,
			  Bool RECURSIVE, class Key >

	auto& Node::GetChild( this auto& self,
						  const Key& key ) NOEXCEPT
		{
			return self.GetChild< RECURSIVE >( key )
					   .GetComponent< T >();
		}

	/*
	 @	Find component by Name
	 */
	template< X_component T,
			  Bool RECURSIVE, class Key >

	auto* Node::FindChild( this auto& self,
						   const Key& key ) noexcept
		{
			auto* const node
				{
					self.FindChild< RECURSIVE >( key )
				};

			if( node )
				{
					return node->FindComponent< T >();
				}

			return decltype( node->FindComponent< T >() )( nullptr );
		}

	/*
	 @	Get child by Name
	 */
	template< Bool RECURSIVE, class Key >

	FC_INLINE auto& Node::GetChild( this auto& self,
									const Key& key ) NOEXCEPT
		{
			auto* const child
				{
					self.FindChild< RECURSIVE >( key )
				};

			FC_ASSERT( child );

			return *child;
		}

	/*
	 @	Find child by Name
	 */
	template< Bool RECURSIVE, class Key >

	auto* Node::FindChild( this auto& self,
						   const Key& key ) noexcept
		{
			auto f = [ &key ] ( auto& node ) noexcept
				->
					decltype( &node )
				{
					if constexpr( C_string_array< Key >
							   || Stl_string_view< Key > )
						{
							using Key2 = stl::basic_string
										< std::ranges::range_value_t< Key > >;

							auto* const name
								{
									node.FindComponent< Name< Key2 > >()
								};

							if( name && *name == key )
								{
									return &node;
								}
						}
					else if constexpr( C_string_pointer< Key > )
						{
							using Key2 = stl::basic_string
										< stl::remove_all_t< Key > >;

							auto* const name
								{
									node.FindComponent< Name< Key2 > >()
								};

							if( name && *name == key )
								{
									return &node;
								}
						}
					else
						{
							auto* const name
								{
									node.FindComponent< Name< Key > >()
								};

							if( name && *name == key )
								{
									return &node;
								}
						}

					return nullptr;
				};

			return self.Travel< RECURSIVE >( f );
		}
}



namespace fce::inner::scene
{
	/*
	 @	Get scene
	 */
	template< Bool RECURSIVE >

	FC_INLINE auto& Node::GetScene( this auto& self ) noexcept
		{
			return std::forward_like< decltype( self ) >
					(
						s_SceneGroup.Get< RECURSIVE >( self )
					);
		}
}