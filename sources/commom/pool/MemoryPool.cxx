/*
**
**	Memory pool.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	// ��󻺳�ߴ�
	const UInt MemoryPool::MAX_SIZE = 0x100000;

	/*
	 @	����
	 */
	MemoryPool::MemoryPool( const UInt size ) noexcept
		:
			pool( size,
				  AdjustCapacity( size ),
				  AdjustMax( size ) )
		{
		}


	/*
	 @	���� �����ڴ�
	 */
	Bool MemoryPool::Cleanup() noexcept
		{
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			return this->pool.release_memory();
		}


	/*
	 @	��ȡ ��ǰ�����ɵĵ�λ����
	 */
	UInt MemoryPool::GetCapacity() const noexcept
		{
			// ������ȡ ��һ������
			this->mutex.lock();

			UInt next
				{
					this->pool.get_next_size()
				};
			
			this->mutex.unlock();

			// ��ʼ ����
			UInt start
				{
					AdjustCapacity( this->GetCellSize() )
				};

			// ͳ���� ��������
			UInt capacity = 0;

			while( next != start )
				{
					next >>= 1;

					capacity += next;
				}

			return capacity;
		}


	/*
	 @	���� һ����Ԫ
	 */
	Handle MemoryPool::Malloc() noexcept
		{
			std::lock_guard
				< std::recursive_mutex > lock( this->mutex );

			return this->pool.malloc();
		}


	/*
	 @	�ͷ� һ����Ԫ
	 */
	Void MemoryPool::Free( Handle const p ) NOEXCEPT
		{
			FC_ASSERT( p );

			this->mutex.lock();

			this->pool.free( p );

			this->mutex.unlock();
		}


	/*
	 @	���� һ�� �����
	 */
	Handle MemoryPool::NewBlock( const UInt size ) NOEXCEPT
		{
			// Ĭ����ʼ��ַ�� 4k ����
			constexpr UInt ALIGN = 0x1000;

			do {
				// ���ж������
				#ifdef FC_WINDOWS
					Handle p{ _aligned_malloc( size, ALIGN ) };
				#else
					Handle p{ std::aligned_alloc( ALIGN, size ) };
				#endif

				// ����ɹ� �򷵻�
				if( p )
					{
						return p;
					}
				// ���Ϊ�� ������� ����
				else
					{
						MemoryManagement::NewHandler();
					}
			}
			while( true );
		}


	/*
	 @	���� �������
	 */
	FC_INLINE UInt MemoryPool::AdjustAlign( const UInt size ) noexcept
		{
			// �� 64 λ ����Ϊ��׼������������ 4K
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
	 @	���� �Ż���ʼ����
	 */
	FC_INLINE UInt MemoryPool::AdjustCapacity( const UInt size ) noexcept
		{
			// С�ڴ� �ص����
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
			// ���ڴ� �����ݼ��� 1��
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
	 @	���� �Ż��������
	 */
	FC_INLINE UInt MemoryPool::AdjustMax( const UInt size ) noexcept
		{
			// С�ڴ� �ص����
			if( size < 0x1000 )
				{
					return 0x800;
				}
			// ���ڴ� �����ݼ��� 1��
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