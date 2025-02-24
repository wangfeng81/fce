/*
**
**	IndexBuffer.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/graphics/IndexBuffer.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_IndexBuffer.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_IndexBuffer.cxx"
#elif defined  FC_METAL
	#include "metal/MT_IndexBuffer.cxx"
#elif defined  FC_GLES
	#include "gles/ES_IndexBuffer.cxx"
#endif


namespace fce
{
	/*
	 @	����
	 */
	IndexBuffer::~IndexBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	���� Buffer
	 */
	IndexBuffer::IndexBuffer( RESULT& res,
							  const Buffer& buf )
		:
			IndexBuffer( res,
						 IndexSet( buf ) )
		{
		}

	/*
	 @	���� IndexSet::Info ( ����ռ� )
	 */
	IndexBuffer::IndexBuffer( RESULT& res,
							  const UInt32 count,
							  const TYPE type ) NOEXCEPT
		:
			IndexBuffer( res,
						 Info( count, type ) )
		{
		}

	IndexBuffer::IndexBuffer( RESULT& res,
							  const Info& info ) NOEXCEPT
		:
			impl( Impl::Convert( info.type ) ),
			info( info )
		{
			FC_ASSERT( info );

			res = this->Initialize( info.size );
		}

	/*
	 @	���� IndexSet ( ����ռ� & ���� )
	 */
	IndexBuffer::IndexBuffer( RESULT& res,
							  const IndexSet& indexSet ) NOEXCEPT
		:
			IndexBuffer( res,
						 indexSet.info )
		{
			FC_ASSERT( indexSet );

			if( res == RESULT::SUCCESS )
				{
					res = this->Update( indexSet );
				}
		}
}



namespace fce
{
	/*
	 @	����
	 */
	RESULT IndexBuffer::Update( const IndexSet& indexSet,
								const UInt32 dstOffset ) NOEXCEPT
		{
			return this->Update( indexSet,
								 indexSet.Size(),
								 0, dstOffset );
		}

	/*
	 @	���� ( ԭ������ )
	 */
	RESULT IndexBuffer::Update( const IndexSet& indexSet,
								const UInt32 size,
								const UInt32 srcOffset,
								const UInt32 dstOffset ) NOEXCEPT
		{
			FC_ASSERT( srcOffset + size <= indexSet.Size()
					&& dstOffset + size <= this->Size() );

			const auto res
				{
					this->impl.Update( *indexSet, size,
												  srcOffset,
												  dstOffset )
				};

			return Graphics::Convert( res );
		}
}



namespace fce
{
	/*
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void IndexBuffer::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
	 */
	FC_INLINE RESULT IndexBuffer::Initialize( const UInt32 size ) noexcept
		{
			const auto res
				{
					this->impl.Initialize( size )
				};

			return Graphics::Convert( res );
		}
}