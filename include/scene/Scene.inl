/*
**
**	Scene.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto Scene::end() const noexcept
		{
			return this->listNodes.cend();
		}

	FC_INLINE auto Scene::rend() const noexcept
		{
			return this->listNodes.crend();
		}

	FC_INLINE auto Scene::begin() const noexcept
		{
			return this->listNodes.cbegin();
		}

	FC_INLINE auto Scene::rbegin() const noexcept
		{
			return this->listNodes.crbegin();
		}

	/*
	 @	Get scene by index
	 */
	FC_INLINE auto& Scene::operator [] ( this auto& self,
										 const UInt idx ) NOEXCEPT
		{
			FC_ASSERT( idx < self.listNodes.size() );

			return std::forward_like< decltype( self ) >
					(
						**std::next( self.begin(), idx )
					);
		}
}



namespace fce
{
	/*
	 @	Move node ( Syntax sugar )
	 */
	FC_INLINE Void Scene::MoveUp( Node& node ) NOEXCEPT
		{
			this->MoveUp( &node );
		}

	FC_INLINE Void Scene::MoveDown( Node& node ) NOEXCEPT
		{
			this->MoveDown( &node );
		}

	FC_INLINE Void Scene::MoveEnd( Node& node ) NOEXCEPT
		{
			this->MoveEnd( &node );
		}

	FC_INLINE Void Scene::MoveBegin( Node& node ) NOEXCEPT
		{
			this->MoveBegin( &node );
		}

	FC_INLINE Void Scene::MoveNode( const Node& dst,
										  Node& src ) NOEXCEPT
		{
			this->MoveNode( &dst, &src );
		}
}



namespace fce
{
	/*
	 @	Sort nodes
	 */
	FC_INLINE Void Scene::SortNodes( auto&& compare ) NOEXCEPT
		{
			stl::sort( this->listNodes, compare );
		}

	/*
	 @	Serialize nodes
	 */
	FC_INLINE Void Scene::SerializeNodes( Buffer& buf ) const NOEXCEPT
		{
			buf << this->listNodes;
		}

	/*
	 @	Count of nodes
	 */
	template< Bool RECURSIVE >

	UInt Scene::CountNodes() const noexcept
		{
			UInt count{ this->listNodes.size() };

			if constexpr( RECURSIVE )
				{
					for( const Node* const root : this->listNodes )
						{
							count += root->CountChildren< true >();
						}
				}

			return count;
		}

	/*
	 @	Travel
	 */
	template< Bool RECURSIVE >

	auto Scene::Travel( this auto&& self,
						auto&& func ) NOEXCEPT
		{
			using Retval = decltype( func( self[ 0 ] ) );

			constexpr bool boolRetval
				{
					std::is_convertible_v< Retval, Bool >
				};

			for( Node* const node : self.listNodes )
				{
					if constexpr( RECURSIVE )
						{
							if constexpr( boolRetval )
								{
									Retval retval
										{
											node->Travel< true >( func )
										};

									if( retval )
										{
											return retval;
										}
								}
							else
								{
									node->Travel< true >( func );
								}
						}
					else
						{
							if constexpr( boolRetval )
								{
									if( Retval retval{ func( *node ) } )
										{
											return retval;
										}
								}
							else
								{
									func( *node );
								}
						}
				}

			if constexpr( boolRetval )
				{
					return Retval( nullptr );
				}
		}
}



namespace fce
{
	/*
	 @	Remove node
	 */
	FC_INLINE Void Scene::RemoveNode( Node& node ) NOEXCEPT
		{
			this->RemoveNode( &node );
		}

	/*
	 @	Remove child by node component
	 */
	FC_INLINE Void Scene::RemoveNode( NodeComponent& nodeCmpt ) NOEXCEPT
		{
			this->RemoveNode( nodeCmpt.node );
		}

	FC_INLINE Void Scene::RemoveNode( NodeComponent* const nodeCmpt ) NOEXCEPT
		{
			FC_ASSERT( nodeCmpt );

			this->RemoveNode( nodeCmpt->node );
		}
}



namespace fce
{
	/*
	 @	Find node
	 */
	FC_INLINE auto Scene::QueryChild( this auto& self,
									  const Node* const node ) noexcept
		{
			return stl::find( self.listNodes, node );
		}

	/*
	 @	Get node index
	 */
	FC_INLINE UInt Scene::GetIndex( const Node& node ) const NOEXCEPT
		{
			return this->GetIndex( &node );
		}

	/*
	 @	Has node
	 */
	template< Bool RECURSIVE >

	Bool Scene::hasNode( const Node* node ) const noexcept
		{
			if constexpr( RECURSIVE )
				{
					node = &node->GetRoot();
				}

			return this->QueryChild( node )
				!= this->end();
		}

	template< Bool RECURSIVE >

	FC_INLINE Bool Scene::hasNode( const Node& node ) const noexcept
		{
			return this->hasNode< RECURSIVE >( &node );
		}
}



namespace fce
{
	/*
	 @	Add root node
	 */
	template< typename ...Args >

	FC_INLINE Scene::Node& Scene::AddNode( Args&& ...args ) NOEXCEPT

	requires requires{ Node( nullptr,
							 std::forward< Args >( args )... ); }
		{
			Node* const node
				{
					this->NewNode
						(
							std::forward< Args >( args )...
						)
				};

			FC_ASSERT( node );

			return *node;
		}

	/*
	 @	New root node
	 */
	template< typename ...Args >

	Scene::Node* Scene::NewNode( Args&& ...args ) noexcept

	requires requires{ Node( nullptr,
							 std::forward< Args >( args )... ); }
		{
			Node* const root
				{
					Node::pool.Generate
						(
							nullptr,
							std::forward< Args >( args )...
						)
				};

			if constexpr( ! noexcept( Node( nullptr,
											std::forward< Args >( args )... ) ) )
				{
					if( ! root )
						{
							return nullptr;
						}
				}

			return this->listNodes.emplace_back( root );
		}
}



namespace fce
{
	/*
	 @	Add node with component
	 */
	template< X_component T, typename ...Args >

	FC_INLINE T& Scene::AddNode( Args&& ...args ) NOEXCEPT
		{
			T* const nodeComponent
				{
					this->NewNode< T >
						(
							std::forward< Args >( args )...
						)
				};

			FC_ASSERT( nodeComponent );

			return *nodeComponent;
		}

	/*
	 @	New node component

			with no transform
	 */
	template< X_component T, typename ...Args >

	T* Scene::NewNode( Args&& ...args ) NOEXCEPT
		{
			Node* const node
				{
					this->NewNode()
				};

			return node->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	New node component

			with transform
	 */
	template< X_component T, typename ...Args >

	T* Scene::NewNode( const Transform& transform,
					   Args&& ...args ) NOEXCEPT
		{
			Node* const node
				{
					this->NewNode( transform )
				};

			return node->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	New node with component

			with pos scale orien
	 */
	template< X_component T, typename ...Args >

	T* Scene::NewNode( const Vector3& pos,
					   const Vector3& scale,
					   const Quaternion& orien,
					   Args&& ...args ) NOEXCEPT
		{
			Node* const node
				{
					this->NewNode( pos, scale, orien )
				};

			return node->NewComponent< T >
						(
							std::forward< Args >( args )...
						);
		}
}



namespace fce
{
	/*
	 @	Get component by Name
	 */
	template< X_component T,
			  Bool RECURSIVE, class Key >

	auto& Scene::GetNode( this auto& self,
						  const Key& key ) NOEXCEPT
		{
			return self.GetNode< RECURSIVE >( key )
					   .GetComponent< T >();
		}

	/*
	 @	Find component by Name
	 */
	template< X_component T,
			  Bool RECURSIVE, class Key >

	auto* Scene::FindNode( this auto& self,
						   const Key& key ) noexcept
		{
			auto* const node
				{
					self.FindNode< RECURSIVE >( key )
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

	FC_INLINE auto& Scene::GetNode( this auto& self,
									const Key& key ) NOEXCEPT
		{
			auto* const child
				{
					self.FindNode< RECURSIVE >( key )
				};

			FC_ASSERT( child );

			return *child;
		}

	/*
	 @	Find child by Name
	 */
	template< Bool RECURSIVE, class Key >

	auto* Scene::FindNode( this auto& self,
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



namespace fce
{
	/*
	 @	Add & Remove camera
	 */
	FC_INLINE Void Scene::Add( Camera* const camera ) noexcept
		{
			this->arrCameras.emplace_back( camera );
		}

	FC_INLINE Void Scene::Remove( Camera* const camera ) noexcept
		{
			stl::remove( this->arrCameras, camera );
		}

	/*
	 @	Add & Remove renderer
	 */
	FC_INLINE Void Scene::Add( Renderer* const renderer ) noexcept
		{
			this->listRenderers.emplace_back( renderer );
		}

	FC_INLINE Void Scene::Remove( Renderer* const renderer ) noexcept
		{
			this->listRenderers.remove( renderer );
		}
}



/**
 !	class SceneGroup
 */
namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto SceneGroup::end() const noexcept
		{
			return this->arrScenes.cend();
		}

	FC_INLINE auto SceneGroup::rend() const noexcept
		{
			return this->arrScenes.crend();
		}

	FC_INLINE auto SceneGroup::begin() const noexcept
		{
			return this->arrScenes.cbegin();
		}

	FC_INLINE auto SceneGroup::rbegin() const noexcept
		{
			return this->arrScenes.crbegin();
		}
}



namespace fce
{
	/*
	 @	Count
	 */
	FC_INLINE UInt SceneGroup::Count() const noexcept
		{
			return this->arrScenes.size();
		}

	/*
	 @	Get scene by index
	 */
	FC_INLINE auto& SceneGroup::operator [] ( this auto& self,
											  const UInt idx ) NOEXCEPT
		{
			FC_ASSERT( idx < self.Count() );

			return std::forward_like< decltype( self ) >
					(
						*self.arrScenes[ idx ]
					);
		}
}



namespace fce
{
	/*
	 @	Remove scene
	 */
	FC_INLINE Void SceneGroup::Remove( Scene& scene ) NOEXCEPT
		{
			this->Remove( &scene );
		}

	FC_INLINE Void SceneGroup::Remove( const UInt idx ) NOEXCEPT
		{
			this->Remove( &( *this )[ idx ] );
		}
}



namespace fce
{
	/*
	 @	Get scene by node
	 */
	template< Bool RECURSIVE >

	Scene& SceneGroup::Get( const Node* node ) const noexcept
		{
			if constexpr( RECURSIVE )
				{
					node = &node->GetRoot();
				}

			Scene* const scene
				{
					this->Find( node )
				};

			FC_ASSERT( scene );

			return *scene;
		}

	template< Bool RECURSIVE >

	FC_INLINE Scene& SceneGroup::Get( const Node& node ) const noexcept
		{
			return this->Get< RECURSIVE >( &node );
		}

	/*
	 @	Find spatial entity by ID component
	 */
	template< class Key >

	FC_INLINE Entity* SceneGroup::FindEntity( const Key& key )
		{
			return ID< Key >::Find( key );
		}

	template< class Key >

	FC_INLINE const Entity* SceneGroup::FindEntity( const Key& key ) const
		{
			return ID< Key >::Find( key );
		}
}