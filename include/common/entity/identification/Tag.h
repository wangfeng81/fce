/*
**
**	Tag.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Tag
	 */
	template< class Value =
				stl::string >

	class FC_API Tag final
		:
		public Component
		{
			/*
			 @	Archive
			 */
			COMPONENT_ARCHIVE( Tag, Component,
							   ARCHIVE_UNIT( value ) );

		public:
			/*	entity
			*/
			Entity& entity;

			/*
			 @	Operation
			 */
			[[nodiscard]] operator const Value&() const noexcept
				{
					return this->value;
				}

			Tag& operator = ( const Tag& tag )
				{
					return *this = tag.value;
				}

			template< class Key >

			Tag& operator = ( const Key& key )
				{
					this->Alter( key );

					return *this;
				}

			/*
			 @	Alter tag
			 */
			template< class Key >

			Void Alter( const Key& key )
				{
					this->Remove();

					this->Add( this->entity, key );

				}

			/*
			 @	Alter tag group
			 */
			template< class Key >

			static Void Alter( const Key& src,
							   const Key& dst )
				{
					FC_ASSERT( src != dst );

					for( auto entity : mapEntities[ src ] )
						In( entity )->Add( entity, dst );

					mapEntities.erase( src );
				}

			/*
			 @	Remove tag group
			 */
			template< class Key >

			static Void Remove( const Key& key )
				{
					stl::vector< Entity* > allocator( std::move(
								mapEntities[ key ] ) );

					for( auto p : allocator )
						( *p )->RemoveComponent< Tag >();

					mapEntities.erase( key );
				}

			/*
			 @	Get tag group
			 */
			template< class Key >

			static const stl::vector< Entity* >& Group( const Key& key )
				{
					const auto p = mapEntities.find( key );

					FC_ASSERT_LOG( p != mapEntities.end(),
						"The tag doesn't exists..." );

					return p->second;
				}

			/*
			 @	Get from entity
			 */
			[[nodiscard]] static auto In( const Entity* entity )
				{
					FC_ASSERT( entity );

					return entity->FindComponent
								   < Tag >();
				}

		protected:
			/*
			 @	Deserialize
			 */
			Tag( Entity& entity,
				 const Buffer& buf )
				:
					entity( entity )
				{
					buf >> this->value;

					this->Add( entity, this->value );
				}

			/*
			 @	Constructor & Destructor
			 */
			template< class Key >

			Tag( Entity& entity,
				 const Key& key )
				:
					entity( entity )
				{
					this->Add( entity, key );
				}

			~Tag() noexcept override
				{
					this->Remove();
				}

		private:
			/*
			 @	Add tag to map
			 */
			template< class Key >

			Void Add( Entity& entity, const Key& key )
				{
					using EntityType =
							stl::vector< Entity* >::template value_type;

					FC_ASSERT( dynamic_cast
							   < EntityType >( &entity ) );

					this->value = key;

					auto& listEntities =
							mapEntities[ this->value ];

					listEntities.insert( listEntities.end(),
						static_cast< EntityType >( &entity ) );
				}

			/*
			 @	Remove tag from map
			 */
			Void Remove()
				{
					auto& listEntities =
							mapEntities[ this->value ];

					const auto p
						{
							stl::find( listEntities,
									   &this->entity )
						};

					if( p != listEntities.end() )
						{
							listEntities.erase( p );
						}
				}

		private:
			/*	value
			*/
			Value value;

			/*	Tags map
			*/
			static stl::unordered_map< Value, stl::vector< Entity* > > mapEntities;
		};

	/*
	 @	Tags map
	*/
	template< class Value >

	stl::unordered_map< Value, stl::vector< Entity* > > Tag< Value >::mapEntities;
}