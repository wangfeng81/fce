/*
**
**	Graphics resource descriptor.
**
		用 layout 创建 pool

			描述符池 大小的问题 需要优化

		Copyright (c) WangFeng since 2016...
*/

#include "../../include/graphics/RenderPipeline.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Descriptor.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_Descriptor.cxx"
#elif defined  FC_METAL
	#include "metal/MT_Descriptor.cxx"
#elif defined  FC_GLES
	#include "gles/ES_Descriptor.cxx"
#endif


/**
 !	Descriptor inner kit
 */
namespace fce::inner::descriptor
{
	/*
	 @	构造
	 */
	Info::Info( const TYPE type,
				const UInt16 count,
				const std::initializer_list< STAGE >& ilist ) noexcept
		:
			type( type ),
			count( count ),
			arrStages( ilist )
		{
		}
}



/**
 !	DescriptorSet
 */
namespace fce
{
	/*
	 @	构造
	 */
	DescriptorSet::DescriptorSet( const auto handle ) noexcept
		:
			impl( handle )
		{
		}
}



/**
 !	DescriptorPool
 */
namespace fce
{
	/*
	 @	析构
	 */
	DescriptorPool::~DescriptorPool() noexcept
		{
			this->Release();
		}

	/*
	 @	构造 layout
	 */
	DescriptorPool::DescriptorPool( RESULT& res,
									const Info& info ) noexcept
		:
			impl()
		{
			res = this->Initialize( info );
		}
}



namespace fce
{
	/*
	 @	分配 DescriptorSet
	 */
	DescriptorSet* DescriptorPool::Allocate() noexcept
		{
			// 分配 impl
			if( auto handle{ this->impl.Allocate() } )
				{
					// 生成 superior
					return this->pool.Generate( handle );
				}

			return nullptr;
		}

	/*
	 @	释放 DescriptorSet
	 */
	Void DescriptorPool::Free( DescriptorSet* descriptorSet ) NOEXCEPT
		{
			FC_ASSERT( descriptorSet );

			// 释放 impl
			this->impl.Free( descriptorSet->impl );

			// 销毁 superior
			this->pool.Destroy( descriptorSet );
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void DescriptorPool::Release() noexcept
		{
			// 析构 superior pool
			this->pool.DestructAll();

			// 释放 impl
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT DescriptorPool::Initialize( const Info& info ) noexcept
		{
			const auto res
				{
					this->impl.Initialize( info )
				};

			return Graphics::Convert( res );
		}
}