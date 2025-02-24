/*
**
**	Graphics resource descriptor.
**
		�� layout ���� pool

			�������� ��С������ ��Ҫ�Ż�

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
	 @	����
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
	 @	����
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
	 @	����
	 */
	DescriptorPool::~DescriptorPool() noexcept
		{
			this->Release();
		}

	/*
	 @	���� layout
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
	 @	���� DescriptorSet
	 */
	DescriptorSet* DescriptorPool::Allocate() noexcept
		{
			// ���� impl
			if( auto handle{ this->impl.Allocate() } )
				{
					// ���� superior
					return this->pool.Generate( handle );
				}

			return nullptr;
		}

	/*
	 @	�ͷ� DescriptorSet
	 */
	Void DescriptorPool::Free( DescriptorSet* descriptorSet ) NOEXCEPT
		{
			FC_ASSERT( descriptorSet );

			// �ͷ� impl
			this->impl.Free( descriptorSet->impl );

			// ���� superior
			this->pool.Destroy( descriptorSet );
		}
}



namespace fce
{
	/*
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void DescriptorPool::Release() noexcept
		{
			// ���� superior pool
			this->pool.DestructAll();

			// �ͷ� impl
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
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