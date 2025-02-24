/*
**
**	Name.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Name
	 */
	template< class T = stl::string >

	class FC_API Name final
		:
		public Component
		{
			/*
			 @	Archive
			 */
			COMPONENT_ARCHIVE( Name, Component,
							   ARCHIVE_UNIT( value ) );

		public:
			/*
			 @	Query by ID
			 */
			QUERY_ID( Name )

			/*
			 @	Access
			 */
			FC_INLINE [[nodiscard]] auto& operator * ( this auto& self ) noexcept
				{
					return self.value;
				}

			FC_INLINE [[nodiscard]] auto* operator -> ( this auto& self ) noexcept
				{
					return &self.value;
				}

			/*
			 @	Get from entity
			 */
			FC_INLINE [[nodiscard]] static auto& In( auto&& entity ) NOEXCEPT
				{
					return entity.GetComponent< Name >();
				}

			FC_INLINE [[nodiscard]] static auto& In( auto* const entity ) NOEXCEPT
				{
					FC_ASSERT( entity );

					return In( *entity );
				}

			/*
			 @	Assignment move & copy
			 */
			FC_INLINE Name& operator = ( T&& value ) noexcept
				{
					this->value = std::move( value );

					return *this;
				}

			FC_INLINE Name& operator = ( Name&& name ) noexcept
				{
					this->value = std::move( name.value );

					return *this;
				}

			FC_INLINE Name& operator = ( const auto& value ) NOEXCEPT
				{
					this->value = value;

					return *this;
				}

			FC_INLINE Name& operator = ( const Name& name ) NOEXCEPT
				{
					this->value = name.value;

					return *this;
				}

			/*	Compare
			*/
			FC_INLINE [[nodiscard]] Bool operator == ( const Name& name ) const NOEXCEPT
				{
					return this->value == name.value;
				}

			FC_INLINE [[nodiscard]] Bool operator != ( const Name& name ) const NOEXCEPT
				{
					return this->value != name.value;
				}

			FC_INLINE [[nodiscard]] Bool operator == ( const auto& value ) const NOEXCEPT
				{
					return this->value == value;
				}

			FC_INLINE [[nodiscard]] Bool operator != ( const auto& value ) const NOEXCEPT
				{
					return this->value != value;
				}

		protected:
			/*
			 @	Destructor
			 */
			~Name() noexcept override = default;

			/*
			 @	Constructor Buffer
			 */
			Name( Entity& entity,
				  const Buffer& buf )
				{
					*this << buf;;
				}

			/*
			 @	Constructor Move
			 */
			Name( Name&& name ) noexcept
				:
					value( std::move( name.value ) )
				{
				}

			/*
			 @	Constructor Copy
			 */
			Name( const Name& name ) noexcept
								   ( noexcept( T( *name ) ) )
				:
					value( name.value )
				{
				}

			/*
			 @	Constructor T
			 */
			template< typename ...Args >

			Name( Args&& ...args ) noexcept
								 ( noexcept( T( std::forward< Args >( args )... ) ) )
				requires requires
					{
						T( std::forward< Args >( args )... );
					}
				:
					value( std::forward< Args >( args )... )
				{
				}

		private:
			/*	value
			*/
			T value;
		};
}