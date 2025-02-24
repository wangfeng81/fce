/*
**
**	Graphics resource descriptor.
**
		Copyright (c) WangFeng since 2016...
*/

#pragma once

#include "Shader.h"
#include "Texture.h"
#include "UniformBuffer.h"


/*
 !	Descriptor inner kit
 */
namespace fce::inner::descriptor
{
	/*	STAGE
	 */
	using STAGE = Shader::TYPE;

	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			SAMPLER,
			COMBINED_IMAGE_SAMPLER,
			SAMPLED_IMAGE,
			STORAGE_IMAGE,
			UNIFORM_TEXEL_BUFFER,
			STORAGE_TEXEL_BUFFER,
			UNIFORM_BUFFER,
			STORAGE_BUFFER,
			UNIFORM_BUFFER_DYNAMIC,
			STORAGE_BUFFER_DYNAMIC,
			INPUT_ATTACHMENT,
			INLINE_UNIFORM_BLOCK,
			ACCELERATION_STRUCTURE_KHR,
			ACCELERATION_STRUCTURE_NV,
			MUTABLE_VALVE,
			INLINE_UNIFORM_BLOCK_EXT,

			MAXIMUM
		};


	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			TYPE type;

			UInt16 count;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( type ),
							ARCHIVE_UNIT( count ),
							ARCHIVE_INFO( arrStages, "Stage" ) )

		public:
			/*	Access element
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] auto GetStage() const noexcept;

			[[nodiscard]] STAGE operator [] ( const UInt ) const;

		public:
			/*	Constructor
			*/
			Info( const TYPE = TYPE::SAMPLER,
				  const UInt16 count = 1,
				  const std::initializer_list< STAGE >& = {} ) noexcept;

		private:
			/*	Stage list
			*/
			stl::vector< STAGE > arrStages;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Descriptor.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Descriptor.h"
#elif defined  FC_METAL
	#include "metal/MT_Descriptor.h"
#elif defined  FC_GLES
	#include "gles/ES_Descriptor.h"
#endif


namespace fce
{
	/*
	 !	Descriptor
	 */
	class FC_API Descriptor final
		{
			/*	Friends
			*/
			friend DescriptorSet;

		public:
			/*	Using inner kit
			*/
			using Info = inner::descriptor::Info;

			using TYPE = inner::descriptor::TYPE;

			using STAGE = inner::descriptor::STAGE;

			/*	Implementation
			*/
			using Impl = inner::descriptor::Impl;

		public:
			/*	Convert TYPE
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Descriptor );

			~Descriptor() = default;

			Descriptor() = default;
		};
}



namespace fce
{
	/*
	 !	Descriptor set
	 */
	class FC_API DescriptorSet final
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL2( DescriptorPool,
						 DescriptorSet );

		public:
			/*	Implementation
			*/
			using Impl = inner::descriptorSet::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Update one
			*/
			template< class T >

			Void Update( const T* const,
						 const UInt32 binding = 0,
						 const UInt32 arrayIndex = 0 ) NOEXCEPT;

			/*	Update a binding group
			*/
			template< std::ranges::range Range >

			Void Update( const Range&,
						 const UInt32 binding = 0 ) NOEXCEPT;

			template< class T >

			Void Update( const std::initializer_list< const T* >&,
						 const UInt32 binding = 0 ) NOEXCEPT;

			/*	Update all
			*/
			template< std::ranges::range ...Args >

			Void Update( const Args&... ) NOEXCEPT;

			template< typename ...T >

			Void Update( const std::initializer_list< const T* >&... ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~DescriptorSet() = default;

			DescriptorSet( const auto ) noexcept;
		};
}



namespace fce
{
	/*
	 !	Descriptor pool
	 */
	class FC_API DescriptorPool final
		:
		protected ObjectHashContainerSugar
				  < DescriptorPool >
		{
			/*	Friends
			*/
			friend Graphics;

			friend RenderPipeline;

			/*	Allocated by object container
			*/
			OBJECT_HASH_CONTAINER_SUGAR( DescriptorPool );

			using Info = stl::vector< Descriptor::Info >;

		public:
			/*	Implementation
			*/
			using Impl = inner::descriptorPool::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Allocate & Free
			*/
			DescriptorSet* Allocate() noexcept;

			Void Free( DescriptorSet* ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~DescriptorPool() noexcept;

			DescriptorPool( RESULT&,
							const Info& ) noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Info& ) noexcept;

		private:
			/*	Descriptor set pool
			*/
			AssetPool< DescriptorPool, DescriptorSet > pool;
		};
}



/**
 !	Descriptor inner kit
 */
namespace fce::inner::descriptor
{
	/*
	 @	Access
	 */
	FC_INLINE auto Info::end() const noexcept
		{
			return this->arrStages.cend();
		}

	FC_INLINE auto Info::begin() const noexcept
		{
			return this->arrStages.cbegin();
		}

	FC_INLINE UInt32 Info::Count() const noexcept
		{
			return static_cast< UInt32 >
					(
						this->arrStages.size()
					);
		}

	FC_INLINE auto Info::GetStage() const noexcept
		{
			return Shader::Combine( this->arrStages );
		}

	FC_INLINE STAGE Info::operator [] ( const UInt idx ) const
		{
			return this->arrStages[ idx ];
		}
}



/**
 !	class DescriptorSet
 */
namespace fce
{
	/*	Update one
	*/
	template< typename T >

	Void DescriptorSet::Update( const T* const obj,
								const UInt32 binding,
								const UInt32 arrayIndex ) NOEXCEPT
		{
			FC_ASSERT( obj );

			this->impl.Update( obj->impl, binding, arrayIndex );
		}

	/*	Update a binding group
	*/
	template< std::ranges::range Range >

	Void DescriptorSet::Update( const Range& range,
								const UInt32 binding ) NOEXCEPT
		{
			this->impl.Update( range, binding );
		}

	template< typename T >

	Void DescriptorSet::Update( const std::initializer_list< const T* >& ilist,
								const UInt32 binding ) NOEXCEPT
		{
			this->impl.Update( ilist, binding );
		}

	/*	Update all
	*/
	template< std::ranges::range ...Args >

	Void DescriptorSet::Update( const Args& ...args ) NOEXCEPT
		{
			this->impl.Update( std::index_sequence_for< Args... >{},
							   args... );
		}

	template< typename ...T >

	Void DescriptorSet::Update( const std::initializer_list< const T* >& ...args ) NOEXCEPT
		{
			this->impl.Update( std::index_sequence_for< T... >{},
							   args... );
		}
}