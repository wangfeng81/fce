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
	 !	VK_Shader
	 */
	class FC_API VK_Shader final
		{
			/*	Friends
			*/
			friend Shader;

		public:
			/*	Get VkShaderModule
			*/
			[[nodiscard]] operator const VkShaderModule() const noexcept;

			[[nodiscard]] const VkShaderModule& GetHandle() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Shader );

			~VK_Shader() = default;

			VK_Shader() noexcept;

			/*	Initialize & Release
			*/
			void Release() noexcept;

			VkResult Initialize( const Buffer& );

		private:
			/*	Shader module
			*/
			VkShaderModule vkShaderModule;

			/*	Shader type list
			*/
			static constexpr VkShaderStageFlagBits TYPE_LIST[]
				{
					VK_SHADER_STAGE_VERTEX_BIT,
					VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT,
					VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT,
					VK_SHADER_STAGE_GEOMETRY_BIT,
					VK_SHADER_STAGE_FRAGMENT_BIT,
					VK_SHADER_STAGE_COMPUTE_BIT,

					VK_SHADER_STAGE_ALL_GRAPHICS,
					VK_SHADER_STAGE_ALL
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::shader
{
	/*
	 @	Shader vulkan impl
	 */
	using Impl = vulkan::VK_Shader;
}



/**
 !	class VK_Shader
 */
namespace fce::inner::vulkan
{
	/*
	 @	Get VkShaderModule
	 */
	FC_INLINE VK_Shader::operator const VkShaderModule() const noexcept
		{
			return this->vkShaderModule;
		}

	FC_INLINE const VkShaderModule& VK_Shader::GetHandle() const noexcept
		{
			return this->vkShaderModule;
		}
}