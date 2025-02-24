/*
**
**	Memory pool.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	// 最大缓冲尺寸
	const UInt MemoryPool::MAX_SIZE = 0x100000;

	/*
	 @	构造
	 */
	MemoryPool::MemoryPool( const UInt size ) noexcept
		:
			pool( size,
				  AdjustCapacity( size ),
				  AdjustMax( size ) )
		{
		}


	/*
	 @	清理 空闲内存
	 */
	Bool MemoryPool::Cleanup() noexcept
		{
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			return this->pool.release_memory();
		}


	/*
	 @	获取 当前能容纳的单位数量
	 */
	UInt MemoryPool::GetCapacity() const noexcept
		{
			// 加锁获取 下一次容量
			this->mutex.lock();

			UInt next
				{
					this->pool.get_next_size()
				};
			
			this->mutex.unlock();

			// 起始 容量
			UInt start
				{
					AdjustCapacity( this->GetCellSize() )
				};

			// 统计已 分配容量
			UInt capacity = 0;

			while( next != start )
				{
					next >>= 1;

					capacity += next;
				}

			return capacity;
		}


	/*
	 @	分配 一个单元
	 */
	Handle MemoryPool::Malloc() noexcept
		{
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			return this->pool.malloc();
		}


	/*
	 @	释放 一个单元
	 */
	Void MemoryPool::Free( Handle const p ) NOEXCEPT
		{
			FC_ASSERT( p );

			this->mutex.lock();

			this->pool.free( p );

			this->mutex.unlock();
		}


	/*
	 @	分配 一个 对齐块
	 */
	Handle MemoryPool::NewBlock( const UInt size ) NOEXCEPT
		{
			// 默认起始地址按 4k 对齐
			constexpr UInt ALIGN = 0x1000;

			do {
				// 进行对齐分配
				#ifdef FC_WINDOWS
					Handle p{ _aligned_malloc( size, ALIGN ) };
				#else
					Handle p{ std::aligned_alloc( ALIGN, size ) };
				#endif

				// 如果成功 则返回
				if( p )
					{
						return p;
					}
				// 如果为空 则清理后 重试
				else
					{
						MemoryManagement::NewHandler();
					}
			}
			while( true );
		}


	/*
	 @	调整 对齐策略
	 */
	FC_INLINE UInt MemoryPool::AdjustAlign( const UInt size ) noexcept
		{
			// 以 64 位 对齐为基准，倍数递增到 4K
			if( size <= 8 )
				{
					return 8;
				}
			else if( size <= 16 )
				{
					return 16;
				}
			else if( size <= 32 )
				{
					return 32;
				}
			else if( size <= 64 )
				{
					return 64;
				}
			else if( size <= 128 )
				{
					return 128;
				}
			else if( size <= 256 )
				{
					return 256;
				}
			else if( size <= 512 )
				{
					return 512;
				}
			else if( size <= 1024 )
				{
					return 1024;
				}
			else if( size <= 0x800 )
				{
					return 0x800;
				}
			else if( size <= 0x1000 )
				{
					return 0x1000;
				}
			else if( size <= 0x2000 )
				{
					return 0x2000;
				}
			else if( size <= 0x4000 )
				{
					return 0x4000;
				}
			else if( size <= 0x8000 )
				{
					return 0x8000;
				}

			return Align( size, 0x10000 );
		}


	/*
	 @	调整 优化初始容量
	 */
	FC_INLINE UInt MemoryPool::AdjustCapacity( const UInt size ) noexcept
		{
			// 小内存 重点分配
			if( size < 1024 )
				{
					return 1024;
				}
			else if( size < 2048 )
				{
					return 512;
				}
			else if( size < 0x1000 )
				{
					return 256;
				}
			// 大内存 倍数递减到 1个
			else if( size < 0x10000 )
				{
					return 32;
				}
			else if( size < 0x20000 )
				{
					return 16;
				}
			else if( size < 0x40000 )
				{
					return 8;
				}
			else if( size < 0x80000 )
				{
					return 4;
				}
			else if( size < 0x100000 )
				{
					return 2;
				}

			return 1;
		}


	/*
	 @	调整 优化最大数量
	 */
	FC_INLINE UInt MemoryPool::AdjustMax( const UInt size ) noexcept
		{
			// 小内存 重点分配
			if( size < 0x1000 )
				{
					return 0x800;
				}
			// 大内存 倍数递减到 1个
			else if( size < 0x10000 )
				{
					return 128;
				}
			else if( size < 0x20000 )
				{
					return 64;
				}
			else if( size < 0x40000 )
				{
					return 32;
				}
			else if( size < 0x80000 )
				{
					return 16;
				}
			else if( size < 0x100000 )
				{
					return 8;
				}

			return 1;
		}
}