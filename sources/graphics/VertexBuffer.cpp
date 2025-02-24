/*
**
**	Vertex buffer.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/graphics/VertexBuffer.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_VertexBuffer.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_VertexBuffer.cxx"
#elif defined  FC_METAL
	#include "metal/MT_VertexBuffer.cxx"
#elif defined  FC_GLES
	#include "gles/ES_VertexBuffer.cxx"
#endif


/**
 !	Vertex inner kit
 */
namespace fce::inner::vertex
{
	/*
	 @	����
	 */
	Info::Info( const Bool restart,
				const PRIMITIVE primitive,
				stl::vector< Binding >&& arrBindings ) noexcept
		:
			restart( restart ),
			primitive( primitive ),
			arrBindings( std::move( arrBindings ) )
		{
		}


	/*
	 @	���� ������Ϣ
	 */
	Void Info::Set( const Bool restart,
					const PRIMITIVE primitive,
					stl::vector< Binding >&& arrBindings ) noexcept
		{
			this->restart = restart;

			this->primitive = primitive;

			this->arrBindings = std::move( arrBindings );
		}


	/*
	 @	��ȡ Ԫ������
	 */
	UInt Info::CountElements() const noexcept
		{
			UInt size{ 0 };

			// ͳ������ Ԫ�ص�����
			for( const Binding& binding : this->arrBindings )
				{
					size += binding.size();
				}

			return size;
		}


	/*
	 @	��ȡ ����ߴ�
	 */
	UInt16 Info::UnitSize() const noexcept
		{
			UInt16 size{ 0 };

			for( const Binding& binding : this->arrBindings )
				{
					for( const TYPE type : binding )
						{
							size += sizeOf[ type ];
						}
				}

			return size;
		}
}



/**
 !	VertexBuffer
 */
namespace fce
{
	/*
	 @	����
	 */
	VertexBuffer::~VertexBuffer() noexcept
		{
			this->Release();
		}

	/*
	 @	���� Buffer
	 */
	VertexBuffer::VertexBuffer( RESULT& res,
								const Buffer& buf )
		:
			VertexBuffer( res,
						  VertexSet( buf ) )
		{
		}

	/*
	 @	���� VertexSet::Info ( ����ռ� )
	 */
	VertexBuffer::VertexBuffer( RESULT& res,
								const UInt32 count,
								const UInt16 unitSize ) NOEXCEPT
		:
			VertexBuffer( res,
						  VertexSet::Info( count, unitSize ) )
		{
		}

	VertexBuffer::VertexBuffer( RESULT& res,
								const VertexSet::Info& info ) NOEXCEPT
		:
			impl(),
			info( info )
		{
			FC_ASSERT( info );

			res = this->Initialize( info.size );
		}

	/*
	 @	���� VertexSet ( ����ռ� & ���� )
	 */
	VertexBuffer::VertexBuffer( RESULT& res,
								const VertexSet& vertexSet ) NOEXCEPT
		:
			VertexBuffer( res,
						  vertexSet.info )
		{
			FC_ASSERT( vertexSet );

			if( res == RESULT::SUCCESS )
				{
					res = this->Update( vertexSet );
				}
		}
}



namespace fce
{
	/*
	 @	����
	 */
	RESULT VertexBuffer::Update( const VertexSet& vertexSet,
								 const UInt32 dstOffset ) NOEXCEPT
		{
			return this->Update( vertexSet,
								 vertexSet.Size(),
								 0, dstOffset );
		}

	/*
	 @	���� ( ԭ������ )
	 */
	RESULT VertexBuffer::Update( const VertexSet& vertexSet,
								 const UInt32 size,
								 const UInt32 srcOffset,
								 const UInt32 dstOffset ) NOEXCEPT
		{
			FC_ASSERT( srcOffset + size <= vertexSet.Size()
					&& dstOffset + size <= this->Size() );

			const auto res
				{
					this->impl.Update( *vertexSet, size,
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
	FC_INLINE Void VertexBuffer::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
	 */
	FC_INLINE RESULT VertexBuffer::Initialize( const UInt32 size ) noexcept
		{
			const auto res
				{
					this->impl.Initialize( size )
				};

			return Graphics::Convert( res );
		}
}