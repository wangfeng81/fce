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
	 !	VK_Rasterization
	 */
	class FC_API VK_Rasterization final
		{
			/*	Friends
			*/
			friend Rasterization;

			friend VK_RenderPipeline;

		public:
			/*	Check enable depthBias
			*/
			[[nodiscard]] static VkBool32 CheckDepthBias( const rasterization::Info& ) noexcept;

			/*	Convert
			*/
			[[nodiscard]] static VkFrontFace Convert( const bool ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VK_Rasterization );

			~VK_Rasterization() = default;

			VK_Rasterization( const rasterization::Info& );

		private:
			/*	Create info
			*/
			const VkPipelineRasterizationStateCreateInfo createInfo;

			/*	Polygon mode list
			*/
			static constexpr VkPolygonMode FILL_LIST[]
				{
					VK_POLYGON_MODE_FILL,
					VK_POLYGON_MODE_LINE,
					VK_POLYGON_MODE_POINT
				};

			/*	Cull mode list
			*/
			static constexpr VkCullModeFlags CULL_LIST[]
				{
					VK_CULL_MODE_NONE,
					VK_CULL_MODE_FRONT_BIT,
					VK_CULL_MODE_BACK_BIT,
					VK_CULL_MODE_FRONT_AND_BACK
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::rasterization
{
	/*
	 @	Rasterization vulkan impl
	 */
	using Impl = vulkan::VK_Rasterization;
}