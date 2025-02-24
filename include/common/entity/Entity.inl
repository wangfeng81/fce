/*
**
**	Entity inlines
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 @	Access
	 */
	FC_INLINE auto Entity::end() const noexcept
		{
			return this->arrComponents.cend();
		}

	FC_INLINE auto Entity::begin() const noexcept
		{
			return this->arrComponents.cbegin();
		}

	FC_INLINE UInt Entity::CountComponents() const noexcept
		{
			return this->arrComponents.size();
		}

	FC_INLINE auto& Entity::operator [] ( this auto& self,
										  const UInt idx ) NOEXCEPT
		{
			return std::forward_like< decltype( self ) >
					(
						*self.arrComponents[ idx ]
					);
		}

	/*
	 @	Serialization bin
	 */
	FC_INLINE const Buffer& Entity::operator << ( const Buffer& buf )
		{
			return this->DeserializeComponents( buf );
		}

	FC_INLINE Buffer& Entity::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			return this->SerializeComponents( buf );
		}

	FC_INLINE Buffer& Entity::SerializeComponents( Buffer& buf ) const NOEXCEPT
		{
			return buf << this->arrComponents;
		}

	/*
	 !	Editor mode
	 */
	#ifdef FC_EDITOR

		/*
		 @	Serialization xml
		 */
		FC_INLINE Void Entity::operator >> ( Xml::Serialization& ser ) const NOEXCEPT
			{
				this->SerializeComponents( ser );
			}

		FC_INLINE Void Entity::operator << ( const Xml::Deserialization& des ) NOEXCEPT
			{
				this->DeserializeComponents( des );
			}

	#endif
}



namespace fce
{
	/*
	 @	Has component
	 */
	template< X_component T >

	FC_INLINE Bool Entity::hasComponent() const noexcept
		{
			return this->FindComponent< T >();
		}

	FC_INLINE Bool Entity::hasComponent( const Component& cmpt ) const noexcept
		{
			return this->hasComponent( &cmpt );
		}

	FC_INLINE Bool Entity::hasComponent( const Component* const cmpt ) const noexcept
		{
			return this->QueryComponent( cmpt ) != this->end();
		}

	/*
	 @	Has dependency
	 */
	FC_INLINE Bool Entity::hasDependency( const Entity& entity ) const noexcept
		{
			return this->hasDependency( &entity );
		}

	FC_INLINE Bool Entity::hasDependency( const Component& cmpt ) const noexcept
		{
			return this->hasDependency( &cmpt );
		}

	FC_INLINE Bool Entity::hasDependency( const Entity* const entity ) const noexcept
		{
			return this->QueryDependency( entity ) != this->end();
		}

	FC_INLINE Bool Entity::hasDependency( const Component* const cmpt ) const noexcept
		{
			return this->QueryDependency( cmpt ) != this->end();
		}
}



namespace fce
{
	/*
	 @	Get component by index
	 */
	template< X_component T >

	FC_INLINE auto& Entity::GetComponent( this auto& self,
										  const UInt idx ) NOEXCEPT
		{
			FC_ASSERT_LOG( typeid( T ) == typeid( self[ idx ] ),
						   "This component is not %s",
						   typeid( T ).name() );

			return *self.castComponent< T >
					(
						self.arrComponents[ idx ]
					);
		}

	/*
	 @	Get component by type
	 */
	template< X_component T >

	FC_INLINE auto& Entity::GetComponent( this auto& self ) NOEXCEPT
		{
			auto* const cmpt
				{
					self.FindComponent< T >()
				};

			FC_ASSERT( cmpt );

			return *cmpt;
		}

	/*
	 @	Cast component
	 */
	template< X_component T >

	FC_INLINE auto* Entity::castComponent( this auto& self,
										   Component* const cmpt ) noexcept
		{
			if constexpr( std::is_const_v< std::remove_reference_t< decltype( self ) > > )
				{
					return static_cast< const T* >( cmpt );
				}
			else
				{
					return static_cast< T* >( cmpt );
				}
		}
}



namespace fce
{
	/*
	 @	Find component by CLASS_HASH
	 */
	template< X_component T >

	requires requires{ T::CLASS_HASH; }

	FC_INLINE auto* Entity::FindComponent( this auto& self ) noexcept
		{
			return self.castComponent< T >
					(
						self.FindComponent( T::CLASS_HASH )
					);
		}

	/*
	 @	Find component by type
	 */
	template< X_component T >

	FC_INLINE auto* Entity::FindComponent( this auto& self ) noexcept
		{
			return self.castComponent< T >
					(
						self.FindComponent( typeid( T ) )
					);
		}
}



namespace fce
{
	/*
	 @	Query component iterator
	 */
	template< X_component T >

	requires requires{ T::CLASS_HASH; }

	FC_INLINE Entity::Iterator Entity::QueryComponent() const noexcept
		{
			return this->QueryComponent( T::CLASS_HASH );
		}

	template< X_component T >

	FC_INLINE Entity::Iterator Entity::QueryComponent() const noexcept
		{
			return this->QueryComponent( typeid( T ) );
		}

	FC_INLINE Entity::Iterator Entity::QueryComponent( const Component* const cmpt ) const noexcept
		{
			return stl::find( this->arrComponents, cmpt );
		}

	/*
	 @	Get the dependent components
	 */
	FC_INLINE Entity::ComponentGroup Entity::GetDependencies( const Entity& entity ) const noexcept
		{
			return this->GetDependencies( &entity );
		}

	FC_INLINE Entity::ComponentGroup Entity::GetDependencies( const Component& cmpt ) const noexcept
		{
			return this->GetDependencies( &cmpt );
		}

	/*
	 @	Get the requirement components
	 */
	FC_INLINE Entity::ComponentGroup Entity::GetRequirements( const Component* const cmpt ) const NOEXCEPT
		{
			FC_ASSERT( cmpt );

			return this->GetRequirements( *cmpt );
		}
}



namespace fce
{
	/*
	 @	Remove component by type
	 */
	template< X_component T >

	Void Entity::RemoveComponent() NOEXCEPT
		{
			const Iterator p
				{
					this->QueryComponent< T >()
				};

			this->RemoveComponent( p );
		}

	/*
	 @	Remove component by reference
	 */
	FC_INLINE Void Entity::RemoveComponent( Component& cmpt ) NOEXCEPT
		{
			this->RemoveComponent( &cmpt );
		}
}



namespace fce
{
	/*
	 @	Request	component by type
	 */
	template< X_component T >

	T& Entity::RequestComponent( this auto& self ) NOEXCEPT
		{
			if( T* const cmpt{ self.FindComponent< T >() } )
				{
					return *cmpt;
				}
			else
				{
					return self.AddComponent< T >();
				}
		}
}



namespace fce
{
	/*
	 @	Add	component by type
	 */
	template< X_component T,
			  typename ...Args >

	FC_INLINE T& Entity::AddComponent( this auto& self,
									   Args&& ...args ) NOEXCEPT
		{
			T* const cmpt
				{
					self.NewComponent< T >
						(
							std::forward< Args >( args )...
						)
				};

			FC_ASSERT( cmpt );

			return *cmpt;
		}
}



namespace fce
{
	/*
	 @	New	component by type
	 */
	template< X_component T,
			  typename ...Args >

	T* Entity::NewComponent( this auto& self,
							 Args&& ...args ) NOEXCEPT
		{
			return self.NewComponent< T >
					(
						self,
						std::forward< Args >( args )...
					);
		}

	/*
	 @	New	component by type with entity
	 */
	template< X_component T,
			  typename ...Args >

	T* Entity::NewComponent( this auto& self,
							 Args&& ...args ) NOEXCEPT
		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			FC_ASSERT_LOG( ! self.hasComponent< T >(),
						   "%s component added repeatedly...",
						   typeid( T ).name() );

			T* const cmpt
				{
					Component::reflection.Generate< T >
						(
							std::forward< Args >( args )...
						)
				};

			FC_ASSERT_LOG( UInt( cmpt ) == UInt( dynamic_cast< Component* >( cmpt ) ),
						   "The first base class of %s must be fce::Component...",
						   typeid( T ).name() );

			self.AddComponent( cmpt );

			return cmpt;
		}
}



namespace fce
{
	/*
	 @	Update component by type
	 */
	template< X_component T,
			  typename ...Args >

	T* Entity::UpdateComponent( this auto& self,
								Args&& ...args ) NOEXCEPT
		{
			return self.UpdateComponent
					(
						self.FindComponent< T >(),
						std::forward< Args >( args )...
					);
		}

	/*
	 @	Update component by pointer with entity
	 */
	template< X_component T,
			  typename ...Args >

	FC_INLINE T* Entity::UpdateComponent( this auto& self,
										  T* const cmpt,
										  Args&& ...args ) NOEXCEPT
		{
			return self.UpdateComponent
					(
						cmpt,
						self,
						std::forward< Args >( args )...
					);
		}

	/*
	 @	Update component by pointer
	 */
	template< X_component T,
			  typename ...Args >

	FC_INLINE T* Entity::UpdateComponent( this auto& self,
										  T* const cmpt,
										  Args&& ...args ) NOEXCEPT
		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			FC_ASSERT( typeid( T ) == typeid( *cmpt ) );

			return Component::reflection.Rebuild
					(
						cmpt,
						std::forward< Args >( args )...
					);
		}
}



namespace fce
{
	/*
	 @	Copy component reference ( Syntax sugar )
	 */
	FC_INLINE Component& Entity::CopyComponent( const Component& src ) NOEXCEPT
		{
			Component* const cmpt
				{
					this->CopyComponent( &src )
				};

			FC_ASSERT( cmpt );

			return *cmpt;
		}

	/*
	 @	Insert	component by type
	 */
	template< X_component T,
			  typename ...Args >

	T* Entity::InsertComponent( this auto& self,
								Component* const dependency,
								Args&& ...args ) NOEXCEPT
		{
			FC_ASSERT_LOG( ! self.hasComponent< T >(),
						   "%s component added repeatedly...",
						   typeid( T ).name() );

			T* const cmpt
				{
					Component::PolymorphicPool::Generate< T >
						(
							self,
							std::forward< Args >( args )...
						)
				};

			self.InsertComponent( dependency, cmpt );

			return cmpt;
		}
}



namespace fce
{
	/*
	 @	Remove name
	 */
	template< class T >

	FC_INLINE Void Entity::RemoveName() NOEXCEPT
		{
			this->RemoveComponent< Name< T > >();
		}

	/*
	 @	Has name
	 */
	template< class T >

	FC_INLINE Bool Entity::hasName() NOEXCEPT
		{
			return this->hasComponent< Name< T > >();
		}

	/*
	 @	Get name
	 */
	template< class T >

	FC_INLINE auto& Entity::GetName( this auto& self ) NOEXCEPT
		{
			return *self.GetComponent< Name< T > >();
		}

	/*
	 @	Add name component
	 */
	template< class T, typename ...Args >

	FC_INLINE auto& Entity::AddName( Args&& ...args ) NOEXCEPT

		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			return this->AddComponent< Name< T > >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	Set name component
	 */
	template< class T, typename ...Args >

	FC_INLINE auto& Entity::SetName( Args&& ...args ) NOEXCEPT

		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			if( auto* const cmpt{ this->FindComponent< Name< T > >() } )
				{
					return *cmpt = T( std::forward< Args >( args )... );
				}
			else
				{
					return this->AddName< T >
								(
									std::forward< Args >( args )...
								);
				}
		}
}



namespace fce
{
	/*
	 @	Remove id
	 */
	template< class T >

	FC_INLINE Void Entity::RemoveID() NOEXCEPT
		{
			this->RemoveComponent< ID< T > >();
		}

	/*
	 @	Has id
	 */
	template< class T >

	FC_INLINE Bool Entity::hasID() NOEXCEPT
		{
			return this->hasComponent< ID< T > >();
		}

	/*
	 @	Get id
	 */
	template< class T >

	FC_INLINE auto& Entity::GetID( this auto& self ) NOEXCEPT
		{
			return *self.GetComponent< ID< T > >();
		}

	/*
	 @	Add id component
	 */
	template< class T, typename ...Args >

	FC_INLINE auto& Entity::AddID( Args&& ...args ) NOEXCEPT

		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			return this->AddComponent< ID< T > >
						(
							std::forward< Args >( args )...
						);
		}

	/*
	 @	Set id component
	 */
	template< class T, typename ...Args >

	FC_INLINE auto& Entity::SetID( Args&& ...args ) NOEXCEPT

		requires requires
			{
				T( std::forward< Args >( args )... );
			}
		{
			if( auto* const cmpt{ this->FindComponent< ID< T > >() } )
				{
					return *cmpt = T( std::forward< Args >( args )... );
				}
			else
				{
					return this->AddID< T >
								(
									std::forward< Args >( args )...
								);
				}
		}
}