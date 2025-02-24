/*
**
**	Pool.
**
		Never allocate a null pointer

		Common objects have no special requirements.

		But polymorphic objects need to inherit from the fce::Object
		otherwise, its equivalent to calling new & delete

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../platform/Debug.h"

#pragma push_macro("new")
#undef new

#include "pool/MemoryPool.h"


namespace fce
{
	/*
	 !	Memory management
	 */
	class FC_API MemoryManagement final
		{
			/*	Friends
			*/
			friend Buffer;

			friend Singleton;

			friend MemoryPool;

		public:
			/*	Request ObjectPool by type
			*/
			template< class T >

			[[nodiscard]] auto& GetPool() noexcept;

			/*	Request MemoryPool by template size
			*/
			template< UInt size >

			[[nodiscard]] MemoryPool* GetPool() noexcept;

			/*	Request MemoryPool by size
			*/
			[[nodiscard]] MemoryPool* GetPool( UInt size ) NOEXCEPT;

			/*	Get current capacity
			*/
			[[nodiscard]] UInt GetCapacity() const noexcept;

			/*	Malloc a block with no construct
			*/
			template< typename T = Byte >

			[[nodiscard]] T* Malloc( const UInt n = 1 ) NOEXCEPT;

			/*	Generate an obj
			*/
			template< class T,
					  typename ...Args >

			[[nodiscard]] T* Generate( Args&&... ) noexcept;

			/*	Free a block with no destruct
			*/
			template< typename T = Byte >

			Void Free( T* const,
					   const UInt n = 1 ) NOEXCEPT;

			/*	Destroy an obj
			*/
			template< class T >

			Void Destroy( T* const ) NOEXCEPT;

			/*	Destruct an object
			*/
			template< class T >

			static Void Destruct( T* const ) NOEXCEPT;

			/*
			 @	Construct an object
			 */
			template< class T,
					  typename ...Args >

			static T* Construct( Handle const,
								 Args&& ... ) NOEXCEPT;

			/*	Rebuild an object
			*/
			template< class T,
					  typename ...Args >

			static Void Rebuild( T* const,
								 Args&&... ) NOEXCEPT;

			/*	Release idle memory
			*/
			Bool Cleanup() noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( MemoryManagement );

			~MemoryManagement() noexcept;

			MemoryManagement() noexcept;

			/*	Free with MemoryPool*
			*/
			Void Free( Handle const,
					   MemoryPool* const ) NOEXCEPT;

			/*	std::set_new_handler
			*/
			static Void NewHandler();

		private:
			/*	Pools map
			*/
			std::unordered_map
			< UInt, MemoryPool > mapPools;

			/*	recursive mutex
			*/
			mutable std::recursive_mutex mutex;
		};


	/*
	 @	s_MemMgmt
	 */
	SINGLETON_SHORTCUT_EX( MemoryManagement,
						   MemMgmt );
}


/*
 !	Extended pools
 */
#include "pool/Allocator.h"
#include "pool/RawObject.h"
#include "pool/ObjectPool.h"
#include "pool/PolymorphicPool.h"
#include "pool/AssetPool.h"
#include "pool/ObjectContainer.h"


/**
 !	class MemoryManagement
 */
namespace fce
{
	/*
	 @	Request an object pool by type
	 */
	template< class T >

	FC_INLINE auto& MemoryManagement::GetPool() noexcept
		{
			static ObjectPool< T, MemoryManagement > pool;

			return pool;
		}

	/*
	 @	Request a memory pool by template size
	 */
	template< UInt size >

	FC_INLINE MemoryPool* MemoryManagement::GetPool() noexcept
		{
			static_assert( size, "Cannot request a zero size memory pool" );

			static MemoryPool* pool{ this->GetPool( size ) };

			return pool;
		}

	/*
	 @	Free a block with no destruct
	 */
	template< typename T >

	FC_INLINE Void MemoryManagement::Free( T* const p,
										   const UInt n ) NOEXCEPT
		{
			this->Free( p, this->GetPool( sizeof( T ) * n ) );
		}

	/*
	 @	Malloc a block with no construct
	 */
	template< typename T >

	FC_INLINE T* MemoryManagement::Malloc( const UInt n ) NOEXCEPT
		{
			return reinterpret_cast< T* >
					(
						this->GetPool( sizeof( T ) * n )->Malloc()
					);
		}

	/*
	 @	Destruct an object
	 */
	template< class T  >

	FC_INLINE Void MemoryManagement::Destruct( T* const obj ) NOEXCEPT
		{
			FC_ASSERT( obj );

			obj->~T();
		}

	/*
	 @	Construct an object
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE T* MemoryManagement::Construct( Handle const p,
											  Args&& ...args ) NOEXCEPT
		{
			ObjectPool< T >::Construct
				(
					p,
					std::forward< Args >( args )...
				);
		}

	/*
	 @	Destroy an obj
	 */
	template< class T >

	FC_INLINE Void MemoryManagement::Destroy( T* const obj ) NOEXCEPT
		{
			if constexpr( std::has_virtual_destructor< T >::value )
				{
					if constexpr( std::derived_from< T, Object > )
						{
							PolymorphicPool<>::Destroy( obj );
						}
					else
						{
							delete obj;
						}
				}
			else
				{
					this->GetPool< T >().Destroy( obj );
				}
		}

	/*
	 @	Generate an obj
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE T* MemoryManagement::Generate( Args&& ...args ) noexcept
		{
			if constexpr( std::has_virtual_destructor< T >::value )
				{
					if constexpr( std::derived_from< T, Object > )
						{
							return PolymorphicPool<>::Generate< T >
								(
									std::forward< Args >( args )...
								);
						}
					else
						{
							return new T( std::forward< Args >( args )... );
						}
				}
			else
				{
					return this->GetPool< T >().Generate
							(
								std::forward< Args >( args )...
							);
				}
		}

	/*
	 @	Rebuild an object

			Parent pointer is not allowed
			Non-Thread-Safe
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE Void MemoryManagement::Rebuild( T* const obj,
											  Args&& ...args ) NOEXCEPT
		{
			if constexpr( std::derived_from< T, Object > )
				{
					PolymorphicPool<>::Rebuild
						(
							obj,
							std::forward< Args >( args )...
						);
				}
			else
				{
					ObjectPool< T >::Rebuild
						(
							obj,
							std::forward< Args >( args )...
						);
				}
		}
}


#pragma pop_macro("new")