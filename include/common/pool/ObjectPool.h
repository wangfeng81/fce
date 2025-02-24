/*
**
**	Object pool.
**
		Never allocate a null pointer

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Object pool

			Malloc		& Free
			Generate	& Destroy
			Construct	& Destruct
			
			Rebuild
	 */
	template< typename T,
			  class Frnd >

	class ObjectPool final
		{
			/*	Friends
			*/
			friend typename T;

			friend typename Frnd;

		public:
			/*
			 @	Destroy an object

					Avoid passing in derived objects
			 */
			template< std::same_as< T > U >

			Void Destroy( U* const obj ) NOEXCEPT
				{
					Destruct( obj );

					this->Free( obj );
				}

			/*
			 @	Generate an object
			 
					with constructor
					
						( highest priority )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( Args&& ...args ) noexcept

			requires requires{ T( std::forward< Args >( args )... ); }
				{
					return this->Generate_with_constructor
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Generate an object
			 
					with a result value

						( secondary priority )
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate( Args&& ...args ) NOEXCEPT
				{
					return this->Generate_with_result
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Rebuild an object

					Non-Thread-Safe
					Failed return null
			 */
			template< std::same_as< T > U,
					  typename ...Args >

			static T* Rebuild( U* const obj,
							   Args&& ...args ) NOEXCEPT
				{
					Destruct( obj );

					return Construct
							(
								obj,
								std::forward< Args >( args )...
							);
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( ObjectPool );

			~ObjectPool() = default;

			ObjectPool() noexcept
				:
					pool( s_MemMgmt.GetPool< sizeof( T ) >() )
				{
				}

		protected:
			/*
			 @	Free the memory
			 */
			Void Free( Handle const p ) NOEXCEPT
				{
					this->pool->Free( p );
				}

			/*
			 @	Allocate memory
			 */
			[[nodiscard]] Handle Malloc() noexcept
				{
					return this->pool->Malloc();
				}

		protected:
			/*
			 @	Destruct an object
			 */
			template< std::same_as< T > U >

			static Void Destruct( U* const obj ) NOEXCEPT
				{
					FC_ASSERT( obj );

					assert( typeid( T ) == typeid( *obj ) );

					obj->~U();
				}

			/*
			 @	Construct an object
			 
					with constructor
			 */
			template< typename ...Args >

			[[nodiscard]] static T* Construct( Handle const p,
											   Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); }
				{
					return Construct_with_constructor
							(
								p,
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Construct an object

					with a result value
			 */
			template< typename ...Args >

			[[nodiscard]] static T* Construct( Handle const p,
											   Args&& ...args ) NOEXCEPT
				{
					return Construct_with_result
							(
								p,
								std::forward< Args >( args )...
							);
				}

		protected:
			/*
			 @	Construct an object
			 
					with constructor ( noexcept )
			 */
			template< typename ...Args >

			[[nodiscard]] static T* Construct_with_constructor( Handle const p,
																Args&& ...args ) NOEXCEPT

			requires requires{ { T( std::forward< Args >( args )... ) } noexcept; }
				{
					FC_ASSERT( p );

					return ::new( p ) T
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Try to construct an object
			 
					with constructor ( except )
			 */
			template< typename ...Args >

			[[nodiscard]] static T* Construct_with_constructor( Handle const p,
																Args&& ...args ) noexcept
				{
					try {
							return ::new( p ) T
									(
										std::forward< Args >( args )...
									);
						}
					catch( ... )
						{
							return nullptr;
						}
				}

			/*
			 @	Construct an object

					with a result value
			 */
			template< typename ...Args >

			requires requires( RESULT& res,
							   Args&& ...args )
				{
					T( res, std::forward< Args >( args )... );
				}

			[[nodiscard]] static T* Construct_with_result( Handle const p,
														   Args&& ...args ) NOEXCEPT
				{
					RESULT res{ RESULT::SUCCESS };

					T* const obj
						{
							Construct_with_constructor
								(
									p, res,
									std::forward< Args >( args )...
								)
						};

					if( res == RESULT::SUCCESS )
						{
							return obj;
						}
					else
						{
							FC_ASSERT( obj );

							Destruct( obj );

							return nullptr;
						}
				}

		protected:
			/*
			 @	Generate an object with constructor ( noexcept )

					Never return a null pointer

					If there is not enough memory
					it will keep trying until succeeds
					or terminates the program
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate_with_constructor( Args&& ...args ) noexcept

			requires requires{ { T( std::forward< Args >( args )... ) } noexcept; }
				{
					return ::new( this->Malloc() ) T
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Try to generate an object with constructor ( except )

					If the constructor throws an exception
					it will reclaim memory and return a null pointer
			 */
			template< typename ...Args >

			[[nodiscard]] T* Generate_with_constructor( Args&& ...args ) noexcept
				{
					Handle p{ this->Malloc() };

					try {
							return ::new( p ) T
									(
										std::forward< Args >( args )...
									);
						}
					catch( ... )
						{
							this->Free( p );

							return nullptr;
						}
				}

			/*
			 @	Generate an object
			 
					with a result value
			 */
			template< typename ...Args >

			requires requires( RESULT& res,
							   Args&& ...args )
				{
					T( res, std::forward< Args >( args )... );
				}

			[[nodiscard]] T* Generate_with_result( Args&& ...args ) NOEXCEPT
				{
					RESULT res{ RESULT::SUCCESS };

					T* const obj
						{
							this->Generate_with_constructor
								(
									res,
									std::forward< Args >( args )...
								)
						};

					if( res == RESULT::SUCCESS )
						{
							return obj;
						}
					else
						{
							// The constructor of the %s should return immediately when RESULT error...
							FC_ASSERT( obj );

							this->Destroy( obj );

							return nullptr;
						}
				}

		private:
			/*	Memory pool
			*/
			MemoryPool * const pool;
		};


	/*
	 @	Requirements of object pool
	 */
	#define OBJECT_POOL( T, F )		friend F;							\
																		\
									friend ::fce::ObjectPool< T, F >;
}