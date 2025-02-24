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
	 @	析构
	 */
	IndexBuffer::~IndexBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 Buffer
	 */
	IndexBuffer::IndexBuffer( RESULT& res,
							  const Buffer& buf )
		:
			IndexBuffer( res,
						 IndexSet( buf ) )
		{
		}

	/*
	 @	构造 IndexSet::Info ( 分配空间 )
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
	 @	构造 IndexSet ( 分配空间 & 数据 )
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
	 @	更新
	 */
	RESULT IndexBuffer::Update( const IndexSet& indexSet,
								const UInt32 dstOffset ) NOEXCEPT
		{
			return this->Update( indexSet,
								 indexSet.Size(),
								 0, dstOffset );
		}

	/*
	 @	更新 ( 原生调用 )
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
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void IndexBuffer::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
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