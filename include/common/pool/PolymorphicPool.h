/*
**
**	Polymorphic object pool.
**
		Never allocate a null pointer

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Polymorphic pool

			Generate & Destroy & Rebuild
	 */
	template< typename Frnd = MemoryManagement >

	class PolymorphicPool final
		{
			/*	Friends
			*/
			friend typename Frnd;

			template< class Base,
					  typename ...Args >

			friend class Reflection;

			/*	Object pool
			*/
			template< class T >

			using ObjectPool = ObjectPool< T, PolymorphicPool >;

		protected:
			/*
			 @	Request an object pool
			 */
			template< class T >

			static auto& GetPool() noexcept
				{
					static ObjectPool< T > pool;

					return pool;
				}

			/*
			 @	Destroy an object
			 */
			template< class T >

			requires requires{ T::memoryPool; }

			static Void Destroy( T* const obj ) NOEXCEPT
				{
					FC_ASSERT( obj && obj->memoryPool );

					MemoryPool* const pool
						{
							obj->memoryPool
						};

					obj->~T();

					pool->Free( obj );
				}

			/*
			 @	Generate an object
			 */
			template< class T,
					  typename ...Args >

			requires requires{ T::memoryPool; }

			[[nodiscard]] static T* Generate( Args&& ...args ) noexcept
				{
					auto& op = GetPool< T >();

					T* obj
						{
							op.Generate
								(
									std::forward< Args >( args )...
								)
						};

					if constexpr( noexcept( T( std::forward< Args >( args )... ) ) )
						{
							obj->memoryPool = op.pool;
						}
					else
						{
							if( obj )
								{
									obj->memoryPool = op.pool;
								}
						}

					return obj;
				}

			/*
			 @	Rebuild an object

					Non-Thread-Safe
			 */
			template< class T,
					  typename ...Args >

			requires requires{ T::memoryPool; }

			static T* Rebuild( T* const obj,
							   Args&& ...args ) NOEXCEPT
				{
					auto& op = GetPool< T >();

					T* obj2
						{
							op.Rebuild
								(
									obj,
									std::forward< Args >( args )...
								)
						};

					if constexpr( noexcept( T( std::forward< Args >( args )... ) ) )
						{
							obj2->memoryPool = op.pool;
						}
					else
						{
							if( obj2 )
								{
									obj2->memoryPool = op.pool;
								}
						}

					return obj2;
				}

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( PolymorphicPool );

			~PolymorphicPool() = delete;

			PolymorphicPool() = delete;
		};


	/*
	 @	Requirements of polymorphic pool
	 */
	#define POLYMORPHIC_POOL( T, F )	OBJECT_POOL( T, F );										\
																									\
										using PolymorphicPool = fce::PolymorphicPool< class F >;	\
																									\
										friend fce::ObjectPool< T, PolymorphicPool >;				\
																									\
										friend typename PolymorphicPool

}



namespace fce
{
	/*
	 !	Object
	 */
	class FC_API Object
		{
			/*	Allocated by polymorphic pool
			*/
			POLYMORPHIC_POOL( Object,
							  MemoryManagement );

		protected:
			/*	Constructor & Destructor
			*/
			virtual ~Object() = default;

			Object() noexcept;

			Object( Object&& ) noexcept;

			Object( const Object& ) noexcept;

			/*	Assignment
			*/
			Object& operator = ( Object&& ) noexcept;

			Object& operator = ( const Object& ) noexcept;

		protected:
			/*	Memory pool
			*/
			MemoryPool * memoryPool;
		};


	/*	concept		Derived object
	 */
	template< class T >

	concept X_object = std::derived_from< T, Object >;
}