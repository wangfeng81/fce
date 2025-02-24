/*
**
**	VK_RenderSurface.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#include "VK_FrameBuffer.h"

namespace fce::inner::vulkan
{
	/*
	 !	VK_RenderSurface
	 */
	class FC_API VK_RenderSurface final
		{
			/*	Friends
			*/
			friend VK_WindowSurface;

		public:
			/*	Model
			*/
			VK_FrameBuffer frameBuffer;

			/*	current
			*/
			VkCommandBuffer vkCommandBuffer;

			VkPipelineStageFlags waitStage;

			VkSubmitInfo submitInfo;

			VkCommandBufferBeginInfo cmdBufBegInfo;

		public:
			/*	Get VkSurfaceKHR
			*/
			[[nodiscard]] operator const VkSurfaceKHR() const noexcept;

			[[nodiscard]] const VkSurfaceKHR& GetHandle() const noexcept;

			/*	Bind resource
			*/
			void Bind( const VK_IndexBuffer& ) NOEXCEPT;

			void Bind( const VK_VertexBuffer& ) NOEXCEPT;

			void Bind( const VK_DescriptorSet& ) NOEXCEPT;

			void Bind( const VK_RenderPipeline& ) NOEXCEPT;

			/*	Clear color
			*/
			void SetClearColor( const Color4F& = 0 ) noexcept;

			/*	Push constant
			*/
			void PushConstant( const void* const pValues,
							   const VkShaderStageFlags,
							   const uint32_t offset,
							   const uint32_t size ) NOEXCEPT;

			/*	Update dynamic states
			*/
			void Update_BlendConstants( const Color4F& ) noexcept;

			/*	Draw
			*/
			void Draw( const uint32_t vertCount,
					   const uint32_t instCount,
					   const uint32_t firstVert,
					   const uint32_t firstInst ) NOEXCEPT;

			void DrawIndex( const uint32_t indexCount,
							const uint32_t instanceCount,
							const uint32_t firstIndex,
							const uint32_t firstInstance ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_RenderSurface );

			~VK_RenderSurface() = default;

			VK_RenderSurface( const window::Surface& ) noexcept;

			/*	Create & Destroy & Rebuild
			*/
			void Destroy() noexcept;

			VkResult Create() noexcept;

			VkResult Rebuild() noexcept;

			/*	Create & Destroy surface
			*/
			void DestroySurface() noexcept;

			VkResult CreateSurface() noexcept;

			/*	Create & Destroy FrameChain
			*/
			void DestroyFrameChain() noexcept;

			VkResult CreateFrameChain() noexcept;

			/*	Begin & End & Present
			*/
			void End() noexcept;

			void Begin() noexcept;

			void Present() noexcept;

			/*	On resize
			*/
			bool on_Resize( const Size& size ) noexcept;

		private:
			/*	VkSurfaceKHR
			*/
			VkSurfaceKHR vkSurfaceKHR;

			/*	Window::Surface
			*/
			const window::Surface& superior;

			/*	Current states
			*/
			struct
				{
					const VK_RenderPipeline* renderPipeline;

					VK_FrameChain* frameChain;

					VkPipelineLayout pipelineLayout;
				}
			curStates;
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
	using Impl = vulkan::VK_RenderSurface;
}



/**
 !	class VK_RenderSurface
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkSurfaceKHR
	 */
	FC_INLINE VK_RenderSurface::operator const VkSurfaceKHR() const noexcept
		{
			return this->vkSurfaceKHR;
		}

	FC_INLINE const VkSurfaceKHR& VK_RenderSurface::GetHandle() const noexcept
		{
			return this->vkSurfaceKHR;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Begin & End & Present
	 */
	FC_INLINE void VK_RenderSurface::End() noexcept
		{
			this->cmdPresentation.End();
		}

	FC_INLINE void VK_RenderSurface::Begin() noexcept
		{
			this->cmdPresentation.Begin();
		}

	FC_INLINE void VK_RenderSurface::Present() noexcept
		{
			this->cmdPresentation.Present();
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Bind resource
	 */
	FC_INLINE void VK_RenderSurface::Bind( const VK_IndexBuffer& ib ) NOEXCEPT
		{
			ib.Use( this->cmdPresentation );
		}

	FC_INLINE void VK_RenderSurface::Bind( const VK_VertexBuffer& vb ) NOEXCEPT
		{
			vb.Use( this->cmdPresentation );
		}

	FC_INLINE void VK_RenderSurface::Bind( const VK_DescriptorSet& ds ) NOEXCEPT
		{
			ds.Use( this->cmdPresentation,
					this->curStates.pipelineLayout );
		}

	FC_INLINE void VK_RenderSurface::Bind( const VK_RenderPipeline& rpl ) NOEXCEPT
		{
			if( this->curStates.renderPipeline != &rpl )
				{
					this->curStates.renderPipeline = &rpl;
					this->curStates.pipelineLayout = rpl.GetLayout();
				}

			rpl.Use( this->cmdPresentation );
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
	FC_INLINE void VK_RenderSurface::PushConstant( const void* const pValues,
												   const VkShaderStageFlags stage,
												   const uint32_t offset,
												   const uint32_t size ) NOEXCEPT
		{
			vkCmdPushConstants( this->cmdPresentation,
								this->curStates.pipelineLayout,
								stage, offset, size, pValues );
		}

	/*
	 @	Set clear color
	 */
	FC_INLINE void VK_RenderSurface::SetClearColor( const Color4F& color ) noexcept
		{
			this->cmdPresentation.SetClearColor( color );
		}

	/*
	 @	BLEND_CONSTANTS
	 */
	FC_INLINE void VK_RenderSurface::Update_BlendConstants( const Color4F& color4F ) noexcept
		{
			vkCmdSetBlendConstants( this->cmdPresentation,
									color4F.components );
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	Draw
	 */
	FC_INLINE void VK_RenderSurface::Draw( const uint32_t vertCount,
										   const uint32_t instCount,
										   const uint32_t firstVert,
										   const uint32_t firstInst ) NOEXCEPT
		{
			vkCmdDraw( this->cmdPresentation,
					   vertCount,
					   instCount,
					   firstVert,
					   firstInst );
		}

	/*
	 @	Draw index
	 */
	FC_INLINE void VK_RenderSurface::DrawIndex( const uint32_t indexCount,
												const uint32_t instanceCount,
												const uint32_t firstIndex,
												const uint32_t firstInstance ) NOEXCEPT
		{
			vkCmdDrawIndexed( this->cmdPresentation,
							  indexCount,
							  instanceCount,
							  firstIndex,
							  0,
							  firstInstance );
		}
}