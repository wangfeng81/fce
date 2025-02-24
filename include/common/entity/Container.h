/*
**
**	Container.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Container
	 */
	template< class T >

	class FC_API Container final
		:
		public Component
		{
			/*
			 @	Archive
			 */
			COMPONENT_ARCHIVE( Container, Component,
							   ARCHIVE_INFO( obj, "Object" ) );

		public:
			/*
			 @	Query by ID
			 */
			QUERY_ID( Container )

			/*
			 @	Access
			 */
			FC_INLINE [[nodiscard]] auto& operator * ( this auto& self ) noexcept
				{
					return self.obj;
				}

			FC_INLINE [[nodiscard]] auto* operator -> ( this auto& self ) noexcept
				{
					return &self.obj;
				}

			/*
			 @	Get from entity
			 */
			FC_INLINE [[nodiscard]] static auto& In( auto&& entity ) NOEXCEPT
				{
					return entity.GetComponent< Container >();
				}

			FC_INLINE [[nodiscard]] static auto& In( auto* const entity ) NOEXCEPT
				{
					FC_ASSERT( entity );

					return In( *entity );
				}

			/*
			 @	Assignment
			 */
			FC_INLINE Container& operator = ( T&& obj ) noexcept
				{
					this->obj = std::move( obj );

					return *this;
				}

			FC_INLINE Container& operator = ( const T& obj ) NOEXCEPT
				{
					this->obj = obj;

					return *this;
				}

			FC_INLINE Container& operator = ( Container&& container ) noexcept
				{
					this->obj = std::move( container.obj );

					return *this;
				}

			FC_INLINE Container& operator = ( const Container& container ) NOEXCEPT
				{
					this->obj = container.obj;

					return *this;
				}

		protected:
			/*
			 @	Destructor
			 */
			~Container() noexcept override = default;

			/*
			 @	Constructor Buffer
			 */
			Container( Entity& entity,
					   const Buffer& buf )
				{
					buf >> this->obj;
				}

			/*
			 @	Constructor Move
			 */
			Container( Container&& container ) noexcept
				:
					obj( std::move( container.obj ) )
				{
				}

			/*
			 @	Constructor Copy
			 */
			Container( const Container& container ) noexcept
												  ( noexcept( T( *container ) ) )
				:
					obj( container.obj )
				{
				}

			/*
			 @	Constructor T
			 */
			template< typename ...Args >

			Container( Args&& ...args ) noexcept
									  ( noexcept( T( std::forward< Args >( args )... ) ) )
				requires requires
					{
						T( std::forward< Args >( args )... );
					}
				:
					obj( std::forward< Args >( args )... )
				{
				}

		private:
			/*	Object
			*/
			T obj;
		};


	/*	concept		Wrapped component
	 */
	template< class T >

	using Wrapped_component = std::conditional_t< X_component< T >,
												  T, Container< T > >;
}