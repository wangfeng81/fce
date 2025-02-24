/*
**
**	VK_WindowSurface.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#include "VK_SwapChain.h"
#include "VK_FrameBuffer.h"
#include "VK_PresentChain.h"

namespace fce::inner::vulkan
{
	/*
	 !	VK_WindowSurface
	 */
	class FC_API VK_WindowSurface final
		{
			/*	Friends
			*/
			friend VK_Instance;

			friend window::Surface;

		public:
			/*	List types
			*/
			using ModeList = stl::vector
							 < VkPresentModeKHR >;

			using FormatList = stl::vector
							   < VkSurfaceFormatKHR >;

			/*	Model
			*/
			VK_SwapChain swapChain;

			VK_FrameChain frameChain;

			VK_PresentChain presentChain;

			/*	Superior
			*/
			const window::Surface& superior;

		public:
			/*	Get VkSurfaceKHR
			*/
			[[nodiscard]] operator const VkSurfaceKHR() const noexcept;

			[[nodiscard]] const VkSurfaceKHR& GetHandle() const noexcept;

			/*	Get lists
			*/
			VkResult GetModeList( ModeList& ) const noexcept;

			VkResult GetFormatList( FormatList& ) const noexcept;

			/*	Bind resource
			*/
			void Bind( const VK_IndexBuffer& ) noexcept;

			void Bind( const VK_VertexBuffer& ) noexcept;

			void Bind( const VK_DescriptorSet& ) noexcept;

			void Bind( const VK_RenderPipeline& ) noexcept;

			/*	Clear color
			*/
			void SetClearColor( const Color4F& = 0 ) noexcept;

			/*	Push constant
			*/
			void PushConstant( const void* const pValues,
							   const VkShaderStageFlags,
							   const uint32_t offset,
							   const uint32_t size ) noexcept;

			/*	Update dynamic states
			*/
			void Update_BlendConstants( const Color4F& ) noexcept;

			/*	Draw
			*/
			void Draw( const uint32_t vertCount,
					   const uint32_t instCount,
					   const uint32_t firstVert,
					   const uint32_t firstInst ) noexcept;

			void DrawIndex( const uint32_t indexCount,
							const uint32_t instanceCount,
							const uint32_t firstIndex,
							const uint32_t firstInstance ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_WindowSurface );

			~VK_WindowSurface() = default;

			VK_WindowSurface( const window::Surface& ) noexcept;

			/*	Begin & End & Update
			*/
			void Update() noexcept;

			VkResult End() noexcept;

			VkResult Begin() noexcept;

			/*	Create & Destroy & Rebuild
			*/
			void Destroy() noexcept;

			void Initialize() noexcept;

			VkResult Create() noexcept;

			VkResult Rebuild() noexcept;

			/*	Create & Destroy surface
			*/
			void DestroySurface() noexcept;

			VkResult CreateSurface( const auto& ) noexcept;

		private:
			/*	VkSurfaceKHR
			*/
			VkSurfaceKHR vkSurfaceKHR;

			/*	Render pass
			*/
			VkClearValue arrClearValues[ 2 ];

			VkRenderPassBeginInfo infoBeginRenderPass;

			/*	Info objects
			*/
			struct
				{
					VkSubmitInfo submit;

					VkPresentInfoKHR present;

					VkPipelineStageFlags waitStage;

					VkCommandBufferBeginInfo beginCommand;
				}
			const info;

			/*	Current frame objects
			*/
			struct
				{
					uint32_t frameIndex;

					VkCommandBuffer commandBuffer;

					VkPipelineLayout pipelineLayout;

					VK_PresentChain::SyncObjects syncObjects;
				}
			current;
		};
}



/**
 !	Implementation
 */
namespace fce::inner::window::surface
{
	/*
	 @	Window surface impl
	 */
	using Impl = vulkan::VK_WindowSurface;
}



/**
 !	class VK_WindowSurface
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkSurfaceKHR
	 */
	FC_INLINE VK_WindowSurface::operator const VkSurfaceKHR() const noexcept
		{
			return this->vkSurfaceKHR;
		}

	FC_INLINE const VkSurfaceKHR& VK_WindowSurface::GetHandle() const noexcept
		{
			return this->vkSurfaceKHR;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Bind resource
	 */
	FC_INLINE void VK_WindowSurface::Bind( const VK_IndexBuffer& ib ) noexcept
		{
			ib.Use( this->current.commandBuffer );
		}

	FC_INLINE void VK_WindowSurface::Bind( const VK_VertexBuffer& vb ) noexcept
		{
			vb.Use( this->current.commandBuffer );
		}

	FC_INLINE void VK_WindowSurface::Bind( const VK_DescriptorSet& ds ) noexcept
		{
			ds.Use( this->current.commandBuffer,
					this->current.pipelineLayout );
		}

	FC_INLINE void VK_WindowSurface::Bind( const VK_RenderPipeline& rpl ) noexcept
		{
			this->current.pipelineLayout = rpl.GetLayout();

			rpl.Use( this->current.commandBuffer );
		}
}



/**
 !	Update dynamic states
 */
namespace fce::inner::vulkan
{
	/*
	 @	Push constant
	 */
	FC_INLINE void VK_WindowSurface::PushConstant( const void* const pValues,
												   const VkShaderStageFlags stage,
												   const uint32_t offset,
												   const uint32_t size ) noexcept
		{
			vkCmdPushConstants( this->current.commandBuffer,
								this->current.pipelineLayout,
								stage, offset, size, pValues );
		}

	/*
	 @	BLEND_CONSTANTS
	 */
	FC_INLINE void VK_WindowSurface::Update_BlendConstants( const Color4F& color4F ) noexcept
		{
			vkCmdSetBlendConstants( this->current.commandBuffer,
									color4F.components );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Draw
	 */
	FC_INLINE void VK_WindowSurface::Draw( const uint32_t vertCount,
										   const uint32_t instCount,
										   const uint32_t firstVert,
										   const uint32_t firstInst ) noexcept
		{
			vkCmdDraw( this->current.commandBuffer,
					   vertCount,
					   instCount,
					   firstVert,
					   firstInst );
		}

	/*
	 @	Draw index
	 */
	FC_INLINE void VK_WindowSurface::DrawIndex( const uint32_t indexCount,
												const uint32_t instanceCount,
												const uint32_t firstIndex,
												const uint32_t firstInstance ) noexcept
		{
			vkCmdDrawIndexed( this->current.commandBuffer,
							  indexCount,
							  instanceCount,
							  firstIndex,
							  0,
							  firstInstance );
		}
}