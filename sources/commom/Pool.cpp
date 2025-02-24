/*
**
**	Pool.
**
		��Ҫ���� �������
		���� vulkan Ҫ����ʼλ�� 4��8�ֽڶ���

	Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"

#pragma push_macro("new")
#undef new

#include "pool/MemoryPool.cxx"


namespace fce
{
	/*
	 @	����
	 */
	MemoryManagement::MemoryManagement() noexcept
		{
			// �Զ��� new �ڴ治�� ������
			std::set_new_handler( NewHandler );
		}

	/*
	 @	����
	 */
	MemoryManagement::~MemoryManagement() noexcept
		{
			// ȡ�� new ������
			std::set_new_handler( nullptr );
		}


	/*
	 @	�ͷ� �����ڴ�
	 */
	Bool MemoryManagement::Cleanup() noexcept
		{
			Bool released{ false };

			// ������ ���� δʹ�õĻ����
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
	 @	��ȡ ����
	 */
	UInt MemoryManagement::GetCapacity() const noexcept
		{
			// ����
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			// ͳ�� ����
			UInt capacity{ 0 };

			for( auto& p : this->mapPools )
				{
					const MemoryPool& pool{ p.second };

					capacity += pool.GetCellSize()
							  * pool.GetCapacity();
				}

			// ���� this->mapPools
			capacity += this->mapPools.size()
					  * sizeof( std::unordered_map
								< UInt, MemoryPool >
								::value_type );

			return capacity;
		}


	/*
	 @	���� ָ����С�� MemoryPool
	 */
	MemoryPool* MemoryManagement::GetPool( UInt size ) NOEXCEPT
		{
			FC_ASSERT_LOG( size,
						   "Cannot request a zero size memory pool");

			// ������ ���в�ѯ�Ͳ����½�
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			// ����ϲ�
			size = MemoryPool::AdjustAlign( size );

			// ������� size ��С���ڴ�� ��ֱ�ӷ���
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
					// ������ ���½�
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
	 @	�ͷ� һ����
	 */
	Void MemoryManagement::Free( Handle const p,
								 MemoryPool* const pool ) NOEXCEPT
		{
			pool->Free( p );

			// �������� Cleanup�����ڴ澡��������
			if( pool->GetCellSize() > MemoryPool::MAX_SIZE )
				{
					pool->Cleanup();
				}
		}


	/*
	 @	C++ std �ڴ治�� �ص�������
	 */
	Void MemoryManagement::NewHandler()
		{
			// ����
			std::lock_guard
				< std::recursive_mutex > lock( s_MemMgmt.mutex );

			// �������ɹ� ���������
			if( s_MemMgmt.Cleanup() )
				{
					return;
				}

			// framework����true ��������ԣ��˴����鱣�� ��Ҫ�ļ�
			if( s_App->on_Low_Memory() )
				{
					return;
				}

			// �ﵽ���Դ����󣬲���ʾ�û�
			static UInt32 count{ 0 };

			if( ++count < 32 )
				{
					return;
				}

			// ����û�ȷ�� ���������
			const WChar* const msg
				{
					L"Out of memory...\n\nFree some memory and\n\n  press \"OK\" to try again or\n\n  press \"Cancel\" to abort !"
				};

			if( s_Window.ConfirmBox( msg ) )
				{
					return;
				}

			// �޼ƿ�ʩ �׳��쳣
			throw std::bad_alloc();
		}
}


#pragma pop_macro("new")