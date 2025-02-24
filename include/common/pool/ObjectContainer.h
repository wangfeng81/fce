/*
**
**	Object container.
**
		Syntax sugar

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Object container sugar
	 */
	template< class T >

	class ObjectContainerSugar
		{
			/*	Friends
			*/
			friend typename T;

		public:
			/*	Pool
			*/
			using Pool = AssetPool
					   < ObjectContainerSugar< T >, T >;

			/*
			 @	Destroy an object
			 */
			FC_INLINE Void Destroy() NOEXCEPT
				{
					pool.Destroy( static_cast< T* >( this ) );
				}

			/*
			 @	Rebuild an object
			 */
			template< typename ...Args >

			FC_INLINE T* Rebuild( Args&& ...args ) NOEXCEPT
				{
					return pool.Rebuild
							(
								static_cast< T* >( this ),
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Generate an object
			 */
			template< typename ...Args >

			FC_INLINE [[nodiscard]] static T* Generate( Args&& ...args ) NOEXCEPT
				{
					return pool.Generate
							(
								std::forward< Args >( args )...
							);
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( ObjectContainerSugar );

			~ObjectContainerSugar() = default;

			ObjectContainerSugar() = default;

			/*
			 @	Clear all
			 */
			FC_INLINE static Void ClearAll() noexcept
				{
					pool.Clear();
				}

			/*
			 @	Destruct all
			 */
			FC_INLINE static Void DestructAll() noexcept
				{
					pool.DestructAll();
				}

		private:
			/*	Pool
			*/
			static Pool pool;
		};

	/*
	 @	Asset pool instance
	 */
	template< class T >

	ObjectContainerSugar< T >::Pool ObjectContainerSugar< T >::pool;

	/*
	 @	Requirements of container pool
	 */
	#define OBJECT_CONTAINER_SUGAR( T )	ASSET_POOL2( ObjectContainerSugar< T >, T )
}



namespace fce
{
	/*
	 !	Object hash container sugar
	 */
	template< class T >

	class ObjectHashContainerSugar
		{
			/*	Friends
			*/
			friend typename T;

		public:
			/*	Pool
			*/
			using Pool = AssetPool
					   < ObjectHashContainerSugar< T >, T, DWord >;

			/*
			 @	Destroy an object
			 */
			FC_INLINE Void Destroy() NOEXCEPT
				{
					pool.Destroy( static_cast< T* >( this ) );
				}

			/*
			 @	Request an object
			 */
			template< typename ...Args >

			[[nodiscard]] static T* Request( Args&& ...args ) NOEXCEPT
				{
					const DWord key
						{
							Crc32().Group( args... )
						};

					if( T* const obj{ pool.Find( key ) } )
						{
							return obj;
						}
					else
						{
							return pool.Generate( key,
												  std::forward< Args >( args )... );
						}
				}

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( ObjectHashContainerSugar );

			~ObjectHashContainerSugar() = default;

			ObjectHashContainerSugar() = default;

			/*
			 @	Clear all
			 */
			FC_INLINE static Void ClearAll() noexcept
				{
					pool.Clear();
				}

			/*
			 @	Destruct all
			 */
			FC_INLINE static Void DestructAll() noexcept
				{
					pool.DestructAll();
				}

		private:
			/*	Pool
			*/
			static Pool pool;
		};

	/*
	 @	Asset pool instance
	 */
	template< class T >

	ObjectHashContainerSugar< T >::Pool ObjectHashContainerSugar< T >::pool;

	/*
	 @	Requirements of container pool
	 */
	#define OBJECT_HASH_CONTAINER_SUGAR( T )	ASSET_POOL( ObjectHashContainerSugar< T >, T, DWord )
}