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
	 @	ππ‘Ï
	 */
	FC_INLINE VK_Multisample::VK_Multisample( const multisample::Info& info )
		:
			createInfo
				{
					.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
					.pNext					= nullptr,
					.flags					= 0,
					.rasterizationSamples	= Multisample::Convert( info.type ),
					.sampleShadingEnable	= info.enableSample,
					.minSampleShading		= info.minSample,
					.pSampleMask			= nullptr,
					.alphaToCoverageEnable	= info.enableAlphaToCoverage,
					.alphaToOneEnable		= info.enableAlphaToOne
				}
		{
		}
}