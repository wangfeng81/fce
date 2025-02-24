/*
**
**	VK_Shader.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_Shader::VK_Shader() noexcept
		:
			vkShaderModule( VK_NULL_HANDLE )
		{
		}


	/*
	 @	释放 shader
	 */
	FC_INLINE void VK_Shader::Release() noexcept
		{
			if( this->vkShaderModule )
				{
					vkDestroyShaderModule( s_VK_Device,
										   this->vkShaderModule,
										   s_VK_Allocator );
				}
		}


	/*
	 @	初始化 shader
	 */
	FC_INLINE VkResult VK_Shader::Initialize( const Buffer& buf )
		{
			VkResult res;

			// 设置 Create info
			const VkShaderModuleCreateInfo createInfo
				{
					.sType	  = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
					.pNext	  = nullptr,
					.flags	  = 0,
					.codeSize = buf.Size(),
					.pCode	  = buf.Ptr< uint32_t >()
				};

			// 创建 shader module
			VK_ERR_GUARD( vkCreateShaderModule( s_VK_Device,
												&createInfo,
												s_VK_Allocator,
												&this->vkShaderModule ) );

			return res;
		}
}