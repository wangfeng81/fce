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
	 @	����
	 */
	FC_INLINE VK_ColorBlend::VK_ColorBlend( const colorBlend::Info& info )
		:
			// ��ת�� vkBlendStates
			arrBlendStates
				(
					stl::transform( info.arrBlends, Convert )
				),

			// ������ create info
			createInfo
				{
					.sType			 = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
					.pNext			 = nullptr,
					.flags			 = 0,
					.logicOpEnable	 = info.logicOpEnable,
					.logicOp		 = ColorBlend::Convert( info.logicOp ),
					.attachmentCount = static_cast< uint32_t >
										(
											this->arrBlendStates.size()
										),
					.pAttachments	 = this->arrBlendStates.data(),
					.blendConstants	{
										info.constant.r,
										info.constant.g,
										info.constant.b,
										info.constant.a
									}
				}
		{
		}


	/*
	 @	ͨ�� Blend ���� AttachmentState
	 */
	FC_INLINE VkPipelineColorBlendAttachmentState VK_ColorBlend::Convert( const colorBlend::Blend& blend )
		{
			return VkPipelineColorBlendAttachmentState
					{
						.blendEnable		 = blend.enable,
						.srcColorBlendFactor = ColorBlend::Convert( blend.srcColor ),
						.dstColorBlendFactor = ColorBlend::Convert( blend.dstColor ),
						.colorBlendOp		 = ColorBlend::Convert( blend.eqColor ),
						.srcAlphaBlendFactor = ColorBlend::Convert( blend.srcAlpha ),
						.dstAlphaBlendFactor = ColorBlend::Convert( blend.dstAlpha ),
						.alphaBlendOp		 = ColorBlend::Convert( blend.eqAlpha ),
						.colorWriteMask		 = ColorBlend::Convert( blend.channel )
					};
		}
}