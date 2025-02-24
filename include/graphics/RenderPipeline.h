/*
**
**	Render pipeline.
**
		Copyright (c) WangFeng since 2016...
*/

#pragma once

#include "Viewport.h"

#include "IndexBuffer.h"
#include "Descriptor.h"
#include "PushConstant.h"
#include "DepthStencil.h"

#include "RenderPass.h"
#include "Multisample.h"
#include "Rasterization.h"
#include "ColorBlend.h"


/**
 !	RenderPipeline inner kit
 */
namespace fce::inner::renderPipeline
{
	/*	Dynamic state
	 */
	enum struct DYNAMIC_STATE
		:
			Byte
		{
			VIEWPORT,
			SCISSOR,
			VIEWPORT_WITH_COUNT,
			SCISSOR_WITH_COUNT,

			DEPTH_BIAS,
			DEPTH_BOUNDS,
			DEPTH_COMPARE_OP,
			DEPTH_BIAS_ENABLE,
			DEPTH_TEST_ENABLE,
			DEPTH_WRITE_ENABLE,
			DEPTH_BOUNDS_TEST_ENABLE,

			STENCIL_OP,
			STENCIL_REFERENCE,
			STENCIL_WRITE_MASK,
			STENCIL_COMPARE_MASK,
			STENCIL_TEST_ENABLE,

			PRIMITIVE_TOPOLOGY,
			PRIMITIVE_RESTART_ENABLE,
			VERTEX_INPUT_BINDING_STRIDE,

			CULL_MODE,
			FRONT_FACE,
			LINE_WIDTH,
			BLEND_CONSTANTS,
			RASTERIZER_DISCARD_ENABLE,

			MAXIMUM
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_RenderPipeline.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_RenderPipeline.h"
#elif defined  FC_METAL
	#include "metal/MT_RenderPipeline.h"
#elif defined  FC_GLES
	#include "gles/ES_RenderPipeline.h"
#endif


namespace fce::inner::renderPipeline
{
	/*	Info
	 */
	struct FC_API Info final
		{
			/*	Friends
			*/
			friend RenderPipeline;

			friend renderPipeline::Impl;

			/*	RawObject
			*/
			template< class T >

			using RawObject = RawObject< T, Info >;

		public:
			/*	Render pass
			*/
			Path							renderPass;

			/*	Infos
			*/
			VertexBuffer::Info				vertexDescribe;

			Rasterization::Info				rasterization;

			Multisample::Info				multisample;

			ColorBlend::Info				colorBlend;

			RawObject< DepthStencil::Info >	depthStencil;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( renderPass ),
							ARCHIVE_INFO( arrShaders,		"Shader"		 ),
							ARCHIVE_INFO( arrDescriptors,	"Descriptor"	 ),
							ARCHIVE_INFO( arrPushConstants,	"PushConstant"	 ),
							ARCHIVE_INFO( arrScissors,		"Scissor"		 ),
							ARCHIVE_INFO( arrViewports,		"Viewport"		 ),
							ARCHIVE_INFO( arrDynamicStates,	"DynamicState"	 ),
							ARCHIVE_INFO( vertexDescribe,	"VertexDescribe" ),
							ARCHIVE_INFO( rasterization,	"Rasterization"	 ),
							ARCHIVE_INFO( depthStencil,		"DepthStencil"	 ),
							ARCHIVE_INFO( multisample,		"Multisample"	 ),
							ARCHIVE_INFO( colorBlend,		"ColorBlend"	 ) )

		public:
			/*	Constructor & Destructor
			*/
			Info() = default;

			/*	Update
			*/
			Info& Update() noexcept;

			/*	Check
			*/
			RESULT Check() const noexcept;

		public:
			/*	Add & Set
			*/
			template< class T, typename ...Args >

			auto& Add( Args&&... ) noexcept;

			template< class T, typename Value >

			Void Set( const std::initializer_list< Value >& ) noexcept;

		private:
			/*	Get container
			*/
			template< class T >

			constexpr auto& GetContainer() noexcept;

		private:
			/*	Info arrays
			*/
			stl::vector< Shader::Info >			arrShaders;

			stl::vector< Descriptor::Info >		arrDescriptors;

			stl::vector< PushConstant::Info >	arrPushConstants;

			stl::vector< Scissor >				arrScissors;

			stl::vector< Viewport >				arrViewports;

			stl::vector< DYNAMIC_STATE >		arrDynamicStates;
		};
}



namespace fce
{
	/*
	 !	RenderPipeline
	 */
	class FC_API RenderPipeline final
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						RenderPipeline, Path );

			ASSET_POOL2( Graphics,
						 RenderPipeline );

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Using inner kit
			*/
			using Info = inner::renderPipeline::Info;

			using DYNAMIC_STATE = inner::renderPipeline
									   ::DYNAMIC_STATE;

			/*	Implementation
			*/
			using Impl = inner::renderPipeline::Impl;

			friend Impl;

			Impl impl;

			/*	Vertex size
			*/
			const UInt vertexSize;

			/*	Render pass
			*/
			RenderPass* const renderPass;

			/*	Descriptor pool
			*/
			DescriptorPool* const descriptorPool;

		public:
			/*	Allocate & Free		resource
			*/
			template< class T >

			Void Free( T* ) noexcept;

			template< class T >

			[[nodiscard]] T* Allocate() noexcept;

			/*	Size of vertex
			*/
			[[nodiscard]] UInt VertexSize() const noexcept;

			/*	Convert
			*/
			[[nodiscard]] static constexpr auto Convert( const DYNAMIC_STATE );

		protected:
			/*	Constructor & Destructor
			*/
			~RenderPipeline() noexcept;

			RenderPipeline( RESULT&,
							const Buffer& );

			RenderPipeline( RESULT&,
							const Info& );

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Info& );

			/*	Get resource object pool
			*/
			template< class T >

			constexpr auto* GetPool() noexcept;
		};
}



/**
 !	class Info
 */
namespace fce::inner::renderPipeline
{
	/*
	 @	Add
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE auto& Info::Add( Args&& ...args ) noexcept
		{
			return this->GetContainer< T >().emplace_back
					(
						std::forward< Args >( args )...
					);
		}

	/*
	 @	Set
	 */
	template< class T,
			  typename Value >

	FC_INLINE Void Info::Set( const std::initializer_list< Value >& ilist ) noexcept
		{
			this->GetContainer< T >() = ilist;
		}

	/*
	 @	Get container
	 */
	template< class T >

	FC_INLINE constexpr auto& Info::GetContainer() noexcept
		{
			if constexpr( std::same_as< Shader, T > )
				{
					return this->arrShaders;
				}
			else if constexpr( std::same_as< Descriptor, T > )
				{
					return this->arrDescriptors;
				}
			else if constexpr( std::same_as< PushConstant, T > )
				{
					return this->arrPushConstants;
				}
			else if constexpr( std::same_as< Scissor, T > )
				{
					return this->arrScissors;
				}
			else if constexpr( std::same_as< Viewport, T > )
				{
					return this->arrViewports;
				}
			else if constexpr( std::same_as< DYNAMIC_STATE, T > )
				{
					return this->arrDynamicStates;
				}
			else
				{
					static_assert( "No container defined..." );
				}
		}
}



/**
 !	class RenderPipeline
 */
namespace fce
{
	/*
	 @	Allocate resource
	 */
	template< class T >

	FC_INLINE T* RenderPipeline::Allocate() noexcept
		{
			FC_ASSERT( this->GetPool< T >() );

			return this->GetPool< T >()->Allocate();
		}

	/*
	 @	Free resource
	 */
	template< class T >

	FC_INLINE Void RenderPipeline::Free( T* obj ) noexcept
		{
			FC_ASSERT( this->GetPool< T >() );

			this->GetPool< T >()->Free( obj );
		}

	/*
	 @	Size of vertex
	 */
	FC_INLINE UInt RenderPipeline::VertexSize() const noexcept
		{
			return this->vertexSize;
		}

	/*
	 @	Get pool of the resource
	 */
	template< class T >

	FC_INLINE constexpr auto* RenderPipeline::GetPool() noexcept
		{
			if constexpr( std::same_as< DescriptorSet, T > )
				{
					return this->descriptorPool;
				}
			else
				{
					static_assert( "No pool defined..." );
				}
		}

	/*
	 @	Convert dynamic state
	 */
	FC_INLINE constexpr auto RenderPipeline::Convert( const DYNAMIC_STATE dynState )
		{
			FC_ASSERT( dynState < DYNAMIC_STATE::MAXIMUM );

			return inner::dynamicState::Impl::STATE_LIST[ UInt8( dynState ) ];
		}
}