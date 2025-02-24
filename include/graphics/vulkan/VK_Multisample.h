/*
**
**	VK_Multisample.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 !	VK_Multisample
	 */
	class FC_API VK_Multisample final
		{
			/*	Friends
			*/
			friend Multisample;

			friend VK_RenderPipeline;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Multisample );

			~VK_Multisample() = default;

			VK_Multisample( const multisample::Info& );

		private:
			/*	Create info
			*/
			const VkPipelineMultisampleStateCreateInfo createInfo;

			/*	Sample type list
			*/
			static constexpr VkSampleCountFlagBits TYPE_LIST[]
				{
					VK_SAMPLE_COUNT_1_BIT,
					VK_SAMPLE_COUNT_2_BIT,
					VK_SAMPLE_COUNT_4_BIT,
					VK_SAMPLE_COUNT_8_BIT,
					VK_SAMPLE_COUNT_16_BIT,
					VK_SAMPLE_COUNT_32_BIT,
					VK_SAMPLE_COUNT_64_BIT
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::multisample
{
	/*
	 @	Multisample vulkan impl
	 */
	using Impl = vulkan::VK_Multisample;
}