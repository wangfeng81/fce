/*
**
**	VK_RenderPipeline.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_ShaderProgram
	 */
	class FC_API VK_ShaderProgram final
		{
			/*	Friends
			*/
			friend VK_RenderPipeline;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_ShaderProgram );

			~VK_ShaderProgram() = default;

			VK_ShaderProgram( const stl::vector< shader::Info >& );

			/*	Access
			*/
			[[nodiscard]] uint32_t Count() const noexcept;

			[[nodiscard]] const VkPipelineShaderStageCreateInfo* Data() const noexcept;

			/*	Convert
			*/
			[[nodiscard]] static VkPipelineShaderStageCreateInfo Convert( const shader::Info& );

		private:
			/*	Create info array
			*/
			const stl::vector< VkPipelineShaderStageCreateInfo > arrCreateInfos;
		};
}


namespace fce::inner::vulkan
{
	/*
	 !	VK_VertexDescribe
	 */
	class FC_API VK_VertexDescribe final
		{
			/*	Friends
			*/
			friend VK_RenderPipeline;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_VertexDescribe );

			~VK_VertexDescribe() = default;

			VK_VertexDescribe( const vertex::Info& );

			/*	Set binding create info
			*/
			uint32_t AddAttributes( const uint32_t,
									const vertex::Binding& );

			void AddBindings( const stl::vector< vertex::Binding >& );

			VkPipelineVertexInputStateCreateInfo SetCreateInfo( const vertex::Info& );

		private:
			/*	Binding list
			*/
			stl::vector< VkVertexInputBindingDescription > arrBindings;

			/*	Attribute list
			*/
			stl::vector< VkVertexInputAttributeDescription > arrAttributes;

			/*	Binding create info
			*/
			const VkPipelineVertexInputStateCreateInfo bindingCreateInfo;

			/*	Assembly create info
			*/
			const VkPipelineInputAssemblyStateCreateInfo assemblyCreateInfo;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_Viewport
	 */
	class FC_API VK_Viewport final
		{
			/*	Friends
			*/
			friend VK_RenderPipeline;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Viewport );

			~VK_Viewport() = default;

			VK_Viewport( const stl::vector< Scissor >&,
						 const stl::vector< Viewport >& ) noexcept;

		private:
			/*	Vk scissor & viewport array
			*/
			const stl::vector< VkRect2D > vkScissors;

			const stl::vector< VkViewport > vkViewports;

			/*	Create info
			*/
			const VkPipelineViewportStateCreateInfo createInfo;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_DynamicState
	 */
	class FC_API VK_DynamicState final
		{
			/*	Friends
			*/
			friend RenderPipeline;

			/*	Allocated by raw object
			*/
			RAW_OBJECT( VK_DynamicState,
						VK_RenderPipeline );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_DynamicState );

			~VK_DynamicState() = default;

			VK_DynamicState( const stl::vector< renderPipeline::DYNAMIC_STATE >& );

		private:
			/*	Dynamic state list
			*/
			const stl::vector< VkDynamicState > vkDynamicStates;

			/*	Create info
			*/
			const VkPipelineDynamicStateCreateInfo createInfo;

			/*	Dynamic stage list
			*/
			static constexpr VkDynamicState STATE_LIST[]
				{
					VK_DYNAMIC_STATE_VIEWPORT,
					VK_DYNAMIC_STATE_SCISSOR,
					VK_DYNAMIC_STATE_VIEWPORT_WITH_COUNT,
					VK_DYNAMIC_STATE_SCISSOR_WITH_COUNT,

					VK_DYNAMIC_STATE_DEPTH_BIAS,
					VK_DYNAMIC_STATE_DEPTH_BOUNDS,
					VK_DYNAMIC_STATE_DEPTH_COMPARE_OP,
					VK_DYNAMIC_STATE_DEPTH_BIAS_ENABLE,
					VK_DYNAMIC_STATE_DEPTH_TEST_ENABLE,
					VK_DYNAMIC_STATE_DEPTH_WRITE_ENABLE,
					VK_DYNAMIC_STATE_DEPTH_BOUNDS_TEST_ENABLE,

					VK_DYNAMIC_STATE_STENCIL_OP,
					VK_DYNAMIC_STATE_STENCIL_REFERENCE,
					VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
					VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
					VK_DYNAMIC_STATE_STENCIL_TEST_ENABLE,

					VK_DYNAMIC_STATE_PRIMITIVE_TOPOLOGY,
					VK_DYNAMIC_STATE_PRIMITIVE_RESTART_ENABLE,
					VK_DYNAMIC_STATE_VERTEX_INPUT_BINDING_STRIDE,

					VK_DYNAMIC_STATE_CULL_MODE,
					VK_DYNAMIC_STATE_FRONT_FACE,
					VK_DYNAMIC_STATE_LINE_WIDTH,
					VK_DYNAMIC_STATE_BLEND_CONSTANTS,
					VK_DYNAMIC_STATE_RASTERIZER_DISCARD_ENABLE,
				};
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_PipelineLayout
	 */
	class FC_API VK_PipelineLayout final
		:
		protected ObjectHashContainerSugar
				  < VK_PipelineLayout >
		{
			/*	Friends
			*/
			friend VK_RenderPipeline;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( VK_Graphics,
						VK_PipelineLayout, DWord );

			/*	Allocated by object container
			*/
			OBJECT_HASH_CONTAINER_SUGAR( VK_PipelineLayout );

		public:
			/*	Get VkPipelineLayout
			*/
			[[nodiscard]] const VkPipelineLayout GetHandle() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_PipelineLayout );

			~VK_PipelineLayout() noexcept;

			VK_PipelineLayout( RESULT&,
							   const VkDescriptorSetLayout,
							   const stl::vector< pushConstant::Info >& );

			/*	Convert
			*/
			static void Convert( const stl::vector< pushConstant::Info >&,
									   stl::vector< VkPushConstantRange >& );

		private:
			/*	VkPipelineLayout
			*/
			VkPipelineLayout vkPipelineLayout;
		};
}



namespace fce::inner::vulkan
{
	/*
	 !	VK_RenderPipeline
	 */
	class FC_API VK_RenderPipeline final
		{
			/*	Friends
			*/
			friend RenderPipeline;

			/*	RawObject
			*/
			template< class T >

			using RawObject = RawObject< T, VK_RenderPipeline >;

			using Raw_DynamicState = RawObject< VK_DynamicState >;

			using Raw_DepthStencil = RawObject< VkPipelineDepthStencilStateCreateInfo >;

		public:
			/*	Get VkPipeline
			*/
			[[nodiscard]] operator const VkPipeline() const noexcept;

			[[nodiscard]] const VkPipelineLayout GetLayout() const noexcept;

			/*	Use
			*/
			void Use( const VkCommandBuffer ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_RenderPipeline );

			~VK_RenderPipeline() = default;

			VK_RenderPipeline() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const renderPipeline::Info&,
								 const RenderPipeline& );

			/*	Request VkPipelineLayout
			*/
			VkPipelineLayout RequestLayout( const renderPipeline::Info&,
											const RenderPipeline& );

		private:
			/*	Handles
			*/
			VkPipeline vkPipeline;

			VkPipelineLayout vkPipelineLayout;
		};
}



namespace fce::inner::dynamicState
{
	/*
	 @	DynamicState vulkan impl
	 */
	using Impl = vulkan::VK_DynamicState;
}

namespace fce::inner::renderPipeline
{
	/*
	 @	RenderPipeline vulkan impl
	 */
	using Impl = vulkan::VK_RenderPipeline;
}



/**
 !	class VK_RenderPipeline
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkPipeline
	 */
	FC_INLINE VK_RenderPipeline::operator const VkPipeline() const noexcept
		{
			return this->vkPipeline;
		}

	FC_INLINE const VkPipelineLayout VK_RenderPipeline::GetLayout() const noexcept
		{
			return this->vkPipelineLayout;
		}

	/*
	 @	Bind
	 */
	FC_INLINE void VK_RenderPipeline::Use( const VkCommandBuffer vkCommandBuffer ) const noexcept
		{
			vkCmdBindPipeline( vkCommandBuffer,
							   VK_PIPELINE_BIND_POINT_GRAPHICS,
							   this->vkPipeline );
		}
}