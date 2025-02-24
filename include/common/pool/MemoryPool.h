/*
**
**	Memory pool.
**
		Never allocate a null pointer

	Copyright (c) WangFeng since 2006...
*/

#include <boost/pool/pool.hpp>

namespace fce
{
	 /*
	  !	Memory pool
	  */
	class FC_API MemoryPool final
		{
			/*	Friends
			*/
			friend MemoryManagement;

			friend std::unordered_map
				   < UInt, MemoryPool >
				   ::value_type;

		public:
			/*	Cleanup
			*/
			Bool Cleanup() noexcept;

			/*	Malloc
			*/
			[[nodiscard]]
			
			Handle Malloc() noexcept;

			/*	Free
			*/
			Void Free( Handle const ) NOEXCEPT;

			/*	Get actual cell size
			*/
			UInt GetCellSize() const noexcept;

			/*	Get capacity
			*/
			UInt GetCapacity() const noexcept;

			/*	Alignment
			*/
			static constexpr UInt Align( const UInt size,
											   UInt align ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( MemoryPool );

			~MemoryPool() = default;

			explicit MemoryPool( const UInt size ) noexcept;

			/*	new & delete a aligned block
			*/
			[[nodiscard]]
			
			static Handle NewBlock( const UInt size ) NOEXCEPT;

			static Void DeleteBlock( Handle const ) NOEXCEPT;

			/*	Adjust Align
			*/
			static UInt AdjustAlign( const UInt size ) noexcept;

			/*	Adjust max
			*/
			static UInt AdjustMax( const UInt size ) noexcept;

			/*	Adjust capacity
			*/
			static UInt AdjustCapacity( const UInt size ) noexcept;

		private:
			/*	boost::pool allocator
			*/
			struct FC_API allocator final
				{
					/*	Types
					*/
					typedef std::size_t size_type;

					typedef std::ptrdiff_t difference_type;

					/*	free
					*/
					static void free( void* const ) NOEXCEPT;

					/*	malloc
					*/
					static char* malloc( const size_type ) NOEXCEPT;
				};

		private:
			/*	recursive mutex
			*/
			mutable std::recursive_mutex mutex;

			/*	boost pool
			*/
			boost::pool< allocator > pool;

			/*	Max size
			*/
			static const UInt MAX_SIZE;
		};
}



/**	
 !	boost::pool	

		allocator => new_delete
 */
namespace fce
{
	/*
	 @	free a block
	 */
	FC_INLINE void MemoryPool::allocator::free( void* const p ) NOEXCEPT
		{
			DeleteBlock( p );
		}

	/*
	 @	malloc a block
	 */
	FC_INLINE char* MemoryPool::allocator::malloc( const size_type size ) NOEXCEPT
		{
			return static_cast< char* >( NewBlock( size ) );
		}
}



/**
 !	class MemoryPool
 */
namespace fce
{
	/*
	 @	Delete a aligned block
	 */
	FC_INLINE Void MemoryPool::DeleteBlock( Handle const p ) NOEXCEPT
		{
			#ifdef FC_WINDOWS
				_aligned_free( p );
			#else
				std::free( p );
			#endif
		}

	/*
	 @	Get actual cell size
	 */
	FC_INLINE UInt MemoryPool::GetCellSize() const noexcept
		{
			return this->pool.get_requested_size();
		}

	/*
	 @	Calculate alignment dimensions
	 */
	FC_INLINE constexpr UInt MemoryPool::Align( const UInt size,
													  UInt align ) noexcept
		{
			return --align, ( size + align ) & ~align;
		}
}