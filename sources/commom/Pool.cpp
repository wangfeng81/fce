/*
**
**	Pool.
**
		需要考虑 对齐分配
		比如 vulkan 要求起始位置 4或8字节对齐

	Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"

#pragma push_macro("new")
#undef new

#include "pool/MemoryPool.cxx"


namespace fce
{
	/*
	 @	构造
	 */
	MemoryManagement::MemoryManagement() noexcept
		{
			// 自定义 new 内存不足 处理函数
			std::set_new_handler( NewHandler );
		}

	/*
	 @	析构
	 */
	MemoryManagement::~MemoryManagement() noexcept
		{
			// 取消 new 处理函数
			std::set_new_handler( nullptr );
		}


	/*
	 @	释放 闲置内存
	 */
	Bool MemoryManagement::Cleanup() noexcept
		{
			Bool released{ false };

			// 加锁后， 清理 未使用的缓存块
			this->mutex.lock();
				{
					for( auto& p : this->mapPools )
						{
							released |= p.second.Cleanup();
						}
				}
			this->mutex.unlock();

			return released;
		}


	/*
	 @	获取 容量
	 */
	UInt MemoryManagement::GetCapacity() const noexcept
		{
			// 加锁
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			// 统计 容量
			UInt capacity{ 0 };

			for( auto& p : this->mapPools )
				{
					const MemoryPool& pool{ p.second };

					capacity += pool.GetCellSize()
							  * pool.GetCapacity();
				}

			// 加上 this->mapPools
			capacity += this->mapPools.size()
					  * sizeof( std::unordered_map
								< UInt, MemoryPool >
								::value_type );

			return capacity;
		}


	/*
	 @	请求 指定大小的 MemoryPool
	 */
	MemoryPool* MemoryManagement::GetPool( UInt size ) NOEXCEPT
		{
			FC_ASSERT_LOG( size,
						   "Cannot request a zero size memory pool");

			// 加锁后 进行查询和插入新建
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			// 对齐合并
			size = MemoryPool::AdjustAlign( size );

			// 如果存在 size 大小的内存池 则直接返回
			const auto p
				{
					this->mapPools.find( size )
				};

			if( p != this->mapPools.end() )
				{
					return &p->second;
				}
			else
				{
					// 不存在 则新建
					const auto pair
						{
							this->mapPools.try_emplace
								(
									size, size
								)
						};

					return &pair.first->second;
				}
		}


	/*
	 @	释放 一个块
	 */
	Void MemoryManagement::Free( Handle const p,
								 MemoryPool* const pool ) NOEXCEPT
		{
			pool->Free( p );

			// 若过大则 Cleanup，大内存尽量不缓存
			if( pool->GetCellSize() > MemoryPool::MAX_SIZE )
				{
					pool->Cleanup();
				}
		}


	/*
	 @	C++ std 内存不足 回调处理函数
	 */
	Void MemoryManagement::NewHandler()
		{
			// 加锁
			std::lock_guard
				< std::recursive_mutex > lock( s_MemMgmt.mutex );

			// 如果清理成功 则继续尝试
			if( s_MemMgmt.Cleanup() )
				{
					return;
				}

			// framework返回true 则继续尝试，此处建议保存 重要文件
			if( s_App->on_Low_Memory() )
				{
					return;
				}

			// 达到尝试次数后，才提示用户
			static UInt32 count{ 0 };

			if( ++count < 32 )
				{
					return;
				}

			// 如果用户确定 则继续尝试
			const WChar* const msg
				{
					L"Out of memory...\n\nFree some memory and\n\n  press \"OK\" to try again or\n\n  press \"Cancel\" to abort !"
				};

			if( s_Window.ConfirmBox( msg ) )
				{
					return;
				}

			// 无计可施 抛出异常
			throw std::bad_alloc();
		}
}


#pragma pop_macro("new")