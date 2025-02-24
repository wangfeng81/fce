/*
**
**	ID.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	ID
			Unique key
	 */
	template< class Key = stl::string >

	class FC_API ID final
		:
		public Component
		{
			/*
			 @	Archive
			 */
			COMPONENT_ARCHIVE( ID, Component,
							   ARCHIVE_UNIT( key ) );

		public:
			/*
			 @	Swap
			 */
			FC_INLINE Void Swap( ID& id ) noexcept
				{
					Swap( id, *this );
				}

			Void static Swap( ID& id,
							  ID& id2 ) noexcept
				{
					std::swap( id.key, id2.key );

					std::swap( mapEntities[ id.key ],
							   mapEntities[ id2.key ] );
				}

			/*
			 @	Assignment
			 */
			FC_INLINE ID& operator = ( ID&& id ) noexcept
				{
					this->Swap( id );

					return *this;
				}

			FC_INLINE ID& operator = ( Key&& key ) NOEXCEPT
				{
					this->Set( std::forward< Key >( key ) );

					return *this;
				}

			Void Set( Key&& key ) NOEXCEPT
				{
					Entity& entity
						{
							this->Get()
						};

					mapEntities.erase( this->key );

					this->key = std::forward< Key >( key );

					this->Link( entity );
				}

			/*
			 @	Get component
			 */
			template< X_component T >

			FC_INLINE [[nodiscard]] T& Get() NOEXCEPT
				{
					return Get< T >( this->key );
				}

			template< X_component T >

			[[nodiscard]] static T& Get( const Key& key ) NOEXCEPT
				{
					return Get( key ).GetComponent< T >();
				}

			/*
			 @	Find component
			 */
			template< X_component T >

			FC_INLINE [[nodiscard]] T* Find() noexcept
				{
					return Find< T >( this->key );
				}

			template< X_component T >

			[[nodiscard]] static T* Find( const Key& key ) noexcept
				{
					Entity* const entity{ Find( key ) };

					if( entity )
						{
							return entity->FindComponent< T >();
						}

					return nullptr;
				}

			/*
			 @	Find entity
			 */
			template< X_entity T = Entity >

			FC_INLINE [[nodiscard]] Entity* Find() noexcept
				{
					return Find< T >( this->key );
				}

			[[nodiscard]] static Entity* Find( const Key& key ) noexcept
				{
					const auto p{ mapEntities.find( key ) };

					if( p != mapEntities.end() )
						{
							return p->second;
						}

					return nullptr;
				}

			template< X_entity T >

			FC_INLINE [[nodiscard]] static T* Find( const Key& key ) NOEXCEPT
				{
					Entity* const entity{ Find( key ) };

					FC_ASSERT( UInt( entity )
							== UInt( dynamic_cast< T* >( entity ) ) );

					return static_cast< T* >( entity );
				}

			/*
			 @	Get entity
			 */
			template< X_entity T = Entity >

			FC_INLINE [[nodiscard]] Entity& Get() NOEXCEPT
				{
					return Get< T >( this->key );
				}

			[[nodiscard]] static Entity& Get( const Key& key ) NOEXCEPT
				{
					const auto p{ mapEntities.find( key ) };

					FC_ASSERT( p != mapEntities.end() );

					return *p->second;
				}

			template< X_entity T >

			FC_INLINE [[nodiscard]] static T& Get( const Key& key ) NOEXCEPT
				{
					Entity& entity{ Get( key ) };

					FC_ASSERT( UInt( &entity )
							   == UInt( dynamic_cast< T* >( &entity ) ) );

					return static_cast< T& >( entity );
				}

			/*
			 @	Access
			 */
			FC_INLINE [[nodiscard]] static Bool Exists( const Key& key ) noexcept
				{
					return mapEntities.contains( key );
				}

			FC_INLINE [[nodiscard]] auto& operator * ( this auto& self ) noexcept
				{
					return self.key;
				}

			FC_INLINE [[nodiscard]] auto* operator -> ( this auto& self ) noexcept
				{
					return &self.key;
				}

			/*
			 @	Get from entity
			 */
			FC_INLINE [[nodiscard]] static auto& In( auto&& entity ) NOEXCEPT
				{
					return entity.GetComponent< ID >();
				}

			FC_INLINE [[nodiscard]] static auto& In( auto* const entity ) NOEXCEPT
				{
					FC_ASSERT( entity );

					return In( *entity );
				}

			/*	Compare
			*/
			FC_INLINE [[nodiscard]] Bool operator == ( const ID& id ) const NOEXCEPT
				{
					return this->key == id.key;
				}

			FC_INLINE [[nodiscard]] Bool operator != ( const ID& id ) const NOEXCEPT
				{
					return this->key != id.key;
				}

			FC_INLINE [[nodiscard]] Bool operator == ( const auto& key ) const NOEXCEPT
				{
					return this->key == key;
				}

			FC_INLINE [[nodiscard]] Bool operator != ( const auto& key ) const NOEXCEPT
				{
					return this->key != key;
				}

		protected:
			/*
			 @	Destructor
			 */
			~ID() noexcept override
				{
					mapEntities.erase( this->key );
				}

			/*
			 @	Constructor Buffer
			 */
			ID( Entity& entity,
				const Buffer& buf )
				{
					*this << buf;

					this->Link( entity );
				}

			/*
			 @	Constructor Move
			 */
			ID( Entity& entity,
				ID&& id ) noexcept
				:
					key( std::move( id.key ) )
				{
					this->Link( entity );
				}

			/*
			 @	Constructor T
			 */
			template< typename ...Args >

			ID( Entity& entity,
				Args&& ...args ) NOEXCEPT

				requires requires
					{
						Key( std::forward< Args >( args )... );
					}
				:
					key( std::forward< Args >( args )... )
				{
					this->Link( entity );
				}

		private:
			/*
			 @	Link
			 */
			FC_INLINE Void Link( Entity& entity ) NOEXCEPT
				{
					FC_ASSERT_LOG( ! Exists( this->key ),
								"The ID has exists..." );

					mapEntities.emplace( this->key,
										 &entity );
				}

		private:
			/*	Key
			*/
			Key key;

			/*	Entities map
			*/
			static stl::unordered_map< Key, Entity* > mapEntities;
		};

	/*
	 @	ID map
	 */
	template< class Key >

	stl::unordered_map< Key, Entity* > ID< Key >::mapEntities;
}



/*
 @	Query by ID
 */
#define QUERY_ID( T )																					\
																										\
		template< class Key >																			\
																										\
		FC_INLINE [[nodiscard]] static T& Get( const Key& key ) NOEXCEPT								\
			{																							\
				return fce::ID< Key >::Get< T >( key );													\
			}																							\
																										\
		template< class Key >																			\
																										\
		FC_INLINE [[nodiscard]] static T* Find( const Key& key ) noexcept								\
			{																							\
				return fce::ID< Key >::Find< T >( key );												\
			}																							\
																										\
		template< Character Key >																		\
																										\
		FC_INLINE [[nodiscard]] static T& Get( const Key* const key ) NOEXCEPT							\
			{																							\
				return Get( fce::stl::basic_string< Key >( key ) );										\
			}																							\
																										\
		template< Character Key >																		\
																										\
		FC_INLINE [[nodiscard]] static T* Find( const Key* const key ) noexcept							\
			{																							\
				return Find( fce::stl::basic_string< Key >( key ) );									\
			}																							\
																										\
		template< Character Key >																		\
																										\
		FC_INLINE [[nodiscard]] static T& Get( const std::basic_string_view< Key >& key ) NOEXCEPT		\
			{																							\
				return Get( fce::stl::basic_string< Key >( key ) );										\
			}																							\
																										\
		template< Character Key >																		\
																										\
		FC_INLINE [[nodiscard]] static T* Find( const std::basic_string_view< Key >& key ) noexcept		\
			{																							\
				return Find( fce::stl::basic_string< Key >( key ) );									\
			}