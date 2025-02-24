/*
**
**	VK_RenderPass.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Subpass
	 */
	class FC_API VK_Subpass final
		{
			/*	Friends
			*/
			friend class VK_SubpassGroup;

		public:
			/*	Constructor & Destructor
			*/
			~VK_Subpass() = default;

			VK_Subpass( const renderPass::Subpass& );

			/*	Converts
			*/
			[[nodiscard]] operator VkSubpassDescription() const noexcept;

			[[nodiscard]] static VkAttachmentReference Convert( const renderPass::Reference& );

			/*	Choose
			*/
			[[nodiscard]] static const VkAttachmentReference* Choose( const VkAttachmentReference& ) noexcept;

		private:
			/*	Resolve & depthStencil attachment reference
			*/
			const VkAttachmentReference resolve;

			const VkAttachmentReference depthStencil;

			/*	Attachment reference array
			*/
			const stl::vector< uint32_t > arrPreserves;

			const stl::vector< VkAttachmentReference > arrInputs;

			const stl::vector< VkAttachmentReference > arrColors;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_AttachmentGroup
	 */
	class FC_API VK_AttachmentGroup final
		{
			/*	Friends
			*/
			friend VK_RenderPass;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_AttachmentGroup );

			~VK_AttachmentGroup() = default;

			VK_AttachmentGroup( const stl::vector< renderPass::Attachment >& );

			/*	Access
			*/
			[[nodiscard]] uint32_t Count() const noexcept;

			[[nodiscard]] const VkAttachmentDescription* Data() const noexcept;

			/*	Convert
			*/
			[[nodiscard]] static VkAttachmentDescription Convert( const renderPass::Attachment& );

		private:
			/*	Attachment description array
			*/
			const stl::vector< VkAttachmentDescription > arrDescriptions;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_DependencyGroup
	 */
	class FC_API VK_DependencyGroup final
		{
			/*	Friends
			*/
			friend VK_RenderPass;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_DependencyGroup );

			~VK_DependencyGroup() = default;

			VK_DependencyGroup( const stl::vector< renderPass::Dependency >& );

			/*	Access
			*/
			[[nodiscard]] uint32_t Count() const noexcept;

			[[nodiscard]] const VkSubpassDependency* Data() const noexcept;

			/*	Convert
			*/
			[[nodiscard]] static VkSubpassDependency Convert( const renderPass::Dependency& );

		private:
			/*	Subpass dependency array
			*/
			const stl::vector< VkSubpassDependency > arrDescriptions;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_SubpassGroup
	 */
	class FC_API VK_SubpassGroup final
		{
			/*	Friends
			*/
			friend VK_RenderPass;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_SubpassGroup );

			~VK_SubpassGroup() = default;

			VK_SubpassGroup( const stl::vector< renderPass::Subpass >& );

			/*	Access
			*/
			[[nodiscard]] uint32_t Count() const noexcept;

			[[nodiscard]] const VkSubpassDescription* Data() const noexcept;

		private:
			/*	VK_Subpass array
			*/
			const stl::vector< VK_Subpass > vk_subpasses;

			/*	Subpass description array
			*/
			const stl::vector< VkSubpassDescription > arrDescriptions;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_RenderPass
	 */
	class FC_API VK_RenderPass final
		{
			/*	Friends
			*/
			friend RenderPass;

			friend VK_WindowSurface;

		public:
			/*	Get VkRenderPass
			*/
			[[nodiscard]] operator const VkRenderPass() const noexcept;

			/*	Convert
			*/
			[[nodiscard]] static VkFormat Convert( const renderPass::FORMAT );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_RenderPass );

			~VK_RenderPass() = default;

			VK_RenderPass() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const renderPass::Info& );

		private:
			/*	VkRenderPass
			*/
			VkRenderPass vkRenderPass;

			/*	VkAttachmentLoadOp list
			*/
			static constexpr VkAttachmentLoadOp LOAD_LIST[]
				{
					VK_ATTACHMENT_LOAD_OP_LOAD,
					VK_ATTACHMENT_LOAD_OP_CLEAR,
					VK_ATTACHMENT_LOAD_OP_DONT_CARE
				};

			/*	VkAttachmentStoreOp list
			*/
			static constexpr VkAttachmentStoreOp STORE_LIST[]
				{
					VK_ATTACHMENT_STORE_OP_STORE,
					VK_ATTACHMENT_STORE_OP_DONT_CARE
				};

			/*	VkImageLayout list
			*/
			static constexpr VkImageLayout LAYOUT_LIST[]
				{
					VK_IMAGE_LAYOUT_UNDEFINED,
					VK_IMAGE_LAYOUT_GENERAL,
					VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
					VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL,
					VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
					VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					VK_IMAGE_LAYOUT_PREINITIALIZED,

					VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
				};

			/*	VkPipelineStageFlagBits list
			*/
			static constexpr VkPipelineStageFlagBits STAGE_LIST[]
				{
					VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_DRAW_INDIRECT_BIT,
					VK_PIPELINE_STAGE_VERTEX_INPUT_BIT,
					VK_PIPELINE_STAGE_VERTEX_SHADER_BIT,
					VK_PIPELINE_STAGE_TESSELLATION_CONTROL_SHADER_BIT,
					VK_PIPELINE_STAGE_TESSELLATION_EVALUATION_SHADER_BIT,
					VK_PIPELINE_STAGE_GEOMETRY_SHADER_BIT,
					VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
					VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
					VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
					VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
					VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
					VK_PIPELINE_STAGE_TRANSFER_BIT,
					VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
					VK_PIPELINE_STAGE_HOST_BIT,
					VK_PIPELINE_STAGE_ALL_GRAPHICS_BIT,
					VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
					VK_PIPELINE_STAGE_NONE
				};

			/*	VkAccessFlagBits list
			*/
			static constexpr VkAccessFlagBits ACCESS_LIST[]
				{
					VK_ACCESS_INDIRECT_COMMAND_READ_BIT,
					VK_ACCESS_INDEX_READ_BIT,
					VK_ACCESS_VERTEX_ATTRIBUTE_READ_BIT,
					VK_ACCESS_UNIFORM_READ_BIT,
					VK_ACCESS_INPUT_ATTACHMENT_READ_BIT,
					VK_ACCESS_SHADER_READ_BIT,
					VK_ACCESS_SHADER_WRITE_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
					VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
					VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
					VK_ACCESS_TRANSFER_READ_BIT,
					VK_ACCESS_TRANSFER_WRITE_BIT,
					VK_ACCESS_HOST_READ_BIT,
					VK_ACCESS_HOST_WRITE_BIT,
					VK_ACCESS_MEMORY_READ_BIT,
					VK_ACCESS_MEMORY_WRITE_BIT,
					VK_ACCESS_NONE
				};

			/*	VkDependencyFlagBits list
			*/
			static constexpr VkDependencyFlagBits DEPENDENCY_LIST[]
				{
					VK_DEPENDENCY_BY_REGION_BIT,
					VK_DEPENDENCY_DEVICE_GROUP_BIT,
					VK_DEPENDENCY_VIEW_LOCAL_BIT,
					VK_DEPENDENCY_FEEDBACK_LOOP_BIT_EXT
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::renderPass
{
	/*
	 @	RenderPass vulkan impl
	 */
	using Impl = vulkan::VK_RenderPass;
}



/**
 !	class VK_RenderPass
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkRenderPass
	 */
	FC_INLINE VK_RenderPass::operator const VkRenderPass() const noexcept
		{
			return this->vkRenderPass;
		}
}