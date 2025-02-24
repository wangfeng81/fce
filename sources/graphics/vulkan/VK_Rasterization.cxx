/*
**
**	VK_Rasterization.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Rasterization::VK_Rasterization( const rasterization::Info& info )
		:
			createInfo
				{
					.sType					 = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
					.pNext					 = nullptr,
					.flags					 = 0,
					.depthClampEnable		 = VK_FALSE,
					.rasterizerDiscardEnable = info.discard,
					.polygonMode			 = Rasterization::Convert( info.fill ),
					.cullMode				 = Rasterization::Convert( info.cull ),
					.frontFace				 = Convert( info.clockwise ),
					.depthBiasEnable		 = CheckDepthBias( info ),
					.depthBiasConstantFactor = info.depthBias,
					.depthBiasClamp			 = info.depthBiasClamp,
					.depthBiasSlopeFactor	 = info.depthBiasSlope,
					.lineWidth				 = info.lineWidth
				}
		{
		}


	/*
	 @	检测 是否需要开启 depthBias
	 */
	FC_INLINE VkBool32 VK_Rasterization::CheckDepthBias( const rasterization::Info& info ) noexcept
		{
			return info.depthBias != 0
				|| info.depthBiasSlope != 0;
		}


	/*
	 @	转换 Front face
	 */
	FC_INLINE VkFrontFace VK_Rasterization::Convert( const bool clockwise ) noexcept
		{
			return clockwise ? VK_FRONT_FACE_CLOCKWISE
							 : VK_FRONT_FACE_COUNTER_CLOCKWISE;
		}
}