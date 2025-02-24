/*
**
**	VK_ColorBlend.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK Color blend
	 */
	class FC_API VK_ColorBlend final
		{
			/*	Friends
			*/
			friend ColorBlend;

			friend VK_RenderPipeline;

		public:
			/*	Convert
			*/
			[[nodiscard]] static VkPipelineColorBlendAttachmentState Convert( const colorBlend::Blend& );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_ColorBlend );

			~VK_ColorBlend() = default;

			VK_ColorBlend( const colorBlend::Info& );

		private:
			/*	Blend list
			*/
			const stl::vector< VkPipelineColorBlendAttachmentState > arrBlendStates;

			/*	Create info
			*/
			const VkPipelineColorBlendStateCreateInfo createInfo;

		private:
			/*	Equation list
			*/
			static constexpr VkBlendOp EQUATION_LIST[]
				{
					VK_BLEND_OP_MIN,
					VK_BLEND_OP_MAX,

					VK_BLEND_OP_ADD,
					VK_BLEND_OP_SUBTRACT,
					VK_BLEND_OP_REVERSE_SUBTRACT
				};

			/*	Factor list
			*/
			static constexpr VkBlendFactor FACTOR_LIST[]
				{
					VK_BLEND_FACTOR_ZERO,
					VK_BLEND_FACTOR_ONE,

					VK_BLEND_FACTOR_SRC_COLOR,
					VK_BLEND_FACTOR_SRC_ALPHA,

					VK_BLEND_FACTOR_DST_COLOR,
					VK_BLEND_FACTOR_DST_ALPHA,

					VK_BLEND_FACTOR_CONSTANT_COLOR,
					VK_BLEND_FACTOR_CONSTANT_ALPHA,

					VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
					VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,

					VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
					VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,

					VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
					VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,

					VK_BLEND_FACTOR_SRC_ALPHA_SATURATE
				};

			/*	Logic operation list
			*/
			static constexpr VkLogicOp LOGIC_LIST[]
				{
					VK_LOGIC_OP_CLEAR,
					VK_LOGIC_OP_AND,
					VK_LOGIC_OP_AND_REVERSE,
					VK_LOGIC_OP_COPY,
					VK_LOGIC_OP_AND_INVERTED,
					VK_LOGIC_OP_NO_OP,
					VK_LOGIC_OP_XOR,
					VK_LOGIC_OP_OR,
					VK_LOGIC_OP_NOR,
					VK_LOGIC_OP_EQUIVALENT,
					VK_LOGIC_OP_INVERT,
					VK_LOGIC_OP_OR_REVERSE,
					VK_LOGIC_OP_COPY_INVERTED,
					VK_LOGIC_OP_OR_INVERTED,
					VK_LOGIC_OP_NAND,
					VK_LOGIC_OP_SET
				};

			/*	CHANNEL list
			*/
			static constexpr VkColorComponentFlags CHANNEL_LIST[]
				{
					VK_COLOR_COMPONENT_R_BIT,
					VK_COLOR_COMPONENT_G_BIT,
					VK_COLOR_COMPONENT_B_BIT,
					VK_COLOR_COMPONENT_A_BIT,

					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT,
					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT,
					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT,
					VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
					VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT,
					VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,

					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT,
					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_A_BIT,
					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
					VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,

					VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::colorBlend
{
	/*
	 @	ColorBlend vulkan implementation
	 */
	using Impl = vulkan::VK_ColorBlend;
}