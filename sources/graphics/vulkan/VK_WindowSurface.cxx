/*
**
**	VK_WindowSurface.
**
		Vulkan implementation

	Copyright (c) WangFeng since 2020...
*/

#include "VK_SwapChain.cxx"
#include "VK_FrameBuffer.cxx"
#include "VK_PresentChain.cxx"

namespace fce::inner::vulkan
{
	/*
	 @	构造
	 */
	FC_INLINE VK_WindowSurface::VK_WindowSurface( const window::Surface& superior ) noexcept
		:
			current{},
			superior( superior ),
			vkSurfaceKHR( VK_NULL_HANDLE ),

			// VkClearValue array
			arrClearValues
				{
					{
						.color
						{
							.float32
							{
								0.0f, 0.0f, 0.0f, 1.0f
							}
						}
					},
					{
						.depthStencil
						{
							.depth		= 1.0f,
							.stencil	= 0
						}
					}
				},

			// VkRenderPassBeginInfo
			infoBeginRenderPass
				{
					.sType				= VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
					.pNext				= nullptr,
					.renderPass			= VK_NULL_HANDLE,
					.framebuffer		= VK_NULL_HANDLE,
					.renderArea			= { { 0, 0 }, { 0, 0 } },
					.clearValueCount	= 2,
					.pClearValues		= arrClearValues
				},

			// Info objects
			info
				{
					// VkSubmitInfo
					.submit
						{
							.sType					= VK_STRUCTURE_TYPE_SUBMIT_INFO,
							.pNext					= nullptr,
							.waitSemaphoreCount		= 1,
							.pWaitSemaphores		= &current.syncObjects.imageAcquired,
							.pWaitDstStageMask		= &info.waitStage,
							.commandBufferCount		= 1,
							.pCommandBuffers		= &current.commandBuffer,
							.signalSemaphoreCount	= 1,
							.pSignalSemaphores		= &current.syncObjects.renderFinished
						},

					// VkPresentInfoKHR
					.present
						{
							.sType					= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
							.pNext					= nullptr,
							.waitSemaphoreCount		= 1,
							.pWaitSemaphores		= &current.syncObjects.renderFinished,
							.swapchainCount			= 1,
							.pSwapchains			= &swapChain.GetHandle(),
							.pImageIndices			= &current.frameIndex,
							.pResults				= nullptr
						},

					// Wait dst stage mask
					.waitStage						= VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,

					// VkCommandBufferBeginInfo
					.beginCommand
						{
							.sType					= VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
							.pNext					= nullptr,
							.flags					= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
							.pInheritanceInfo		= nullptr
						}
				}
		{
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁
	 */
	void VK_WindowSurface::Destroy() noexcept
		{
			// 销毁 present command chain
			this->presentChain.Destroy();

			// 销毁 frame buffer chain
			this->frameChain.Destroy();

			// 销毁 swap chain
			this->swapChain.Destroy();

			// 销毁 surface
			this->DestroySurface();
		}


	/*
	 @	创建
	 */
	VkResult VK_WindowSurface::Create() noexcept
		{
			// 创建 surface
			VkResult res
				{
					this->CreateSurface( this->superior.window.impl )
				};

			if( res == VK_SUCCESS )
				{
					// 创建 swap chain
					res = this->swapChain.Create( *this );

					if( res == VK_SUCCESS )
						{
							// 创建 frame buffer chain
							res = this->frameChain.CreateCompatible( this->swapChain );

							if( res == VK_SUCCESS )
								{
									// 创建 present command chain
									res = this->presentChain.Create( this->swapChain );

									if( res == VK_SUCCESS )
										{
											// 初始化
											this->Initialize();
										}
								}
						}
				}

			return res;
		}


	/*
	 @	重建
	 */
	VkResult VK_WindowSurface::Rebuild() noexcept
		{
			// 检测是否已创建 device，并等待设备空闲
			VkResult res
				{
					s_VK_Device.WaitIdle()
				};

			if( res == VK_SUCCESS )
				{
					// 销毁
					this->Destroy();

					// 创建
					res = this->Create();

					if( res == VK_SUCCESS )
						{
							// 重建 RenderPipeline
							// 也可以考虑 禁止手动创建 RenderPipeline，这样就可以 全部重新加载了
							// 目前是全部重新加载
							if( ! s_AssetMgmt.ReloadAll< RenderPipeline >() )
								{
									res = VK_ERROR_UNKNOWN;
								};
						}
				}

			return res;
		}


	/*
	 @	初始化
	 */
	FC_INLINE void VK_WindowSurface::Initialize() noexcept
		{
			// 初始化 当前 sync objects
			this->current.syncObjects = *this->presentChain.curSyncObjects;

			// 初始化 RenderPass begin info
			this->infoBeginRenderPass.renderArea.extent = this->swapChain->extent2D;

			this->infoBeginRenderPass.renderPass = this->frameChain.renderPass->impl;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	销毁 surface
	 */
	FC_INLINE void VK_WindowSurface::DestroySurface() noexcept
		{
			// 销毁 VkSurfaceKHR
			if( this->vkSurfaceKHR )
				{
					vkDestroySurfaceKHR( s_VK_Instance,
										 this->vkSurfaceKHR,
										 s_VK_Allocator );

					// 由于可能 Rebuild 所以赋空
					this->vkSurfaceKHR = VK_NULL_HANDLE;
				}
		}


	/*
	 @	创建 平台窗口的 展示页面

			多窗口
	 */
	FC_INLINE VkResult VK_WindowSurface::CreateSurface( const auto& implWindow ) noexcept
		{
			// 各平台的 window surface
			#ifdef FC_WINDOWS
				{
					const VkWin32SurfaceCreateInfoKHR createInfo
						{
							.sType		= VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
							.pNext		= nullptr,
							.flags		= 0,
							.hinstance	= s_Win_App,
							.hwnd		= implWindow
						};

					return vkCreateWin32SurfaceKHR( s_VK_Instance,
													&createInfo,
													s_VK_Allocator,
													&this->vkSurfaceKHR );
				}
			#elif defined FC_LINUX
				{
					const VkXcbSurfaceCreateInfoKHR createInfo
						{
							.sType		= VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR,
							.pNext		= nullptr,
							.flags		= 0,
							.connection	= s_Win_App,
							.window		= implWindow
						};

					return vkCreateXcbSurfaceKHR( s_VK_Instance,
												  &createInfo,
												  s_VK_Allocator,
												  &this->vkSurfaceKHR );
				}
			#elif defined( VK_USE_PLATFORM_WAYLAND_KHR )
				{
					const VkWaylandSurfaceCreateInfoKHR createInfo
						{
							.sType		= VK_STRUCTURE_TYPE_WAYLAND_SURFACE_CREATE_INFO_KHR,
							.pNext		= nullptr,
							.flags		= 0,
							.display	= s_Win_App,
							.surface	= implWindow
						};

					return vkCreateWaylandSurfaceKHR( s_VK_Instance,
													  &createInfo,
													  s_VK_Allocator,
													  &this->vkSurfaceKHR );
				}
			#else

					// 已在 VK_Instance 创建 单件窗口
					return VK_SUCCESS;

			#endif
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	设置 clear color
	 */
	FC_INLINE void VK_WindowSurface::SetClearColor( const Color4F& color ) noexcept
		{
			this->arrClearValues[ 0 ].color =
				{
					.float32
					{
						color.r,
						color.g,
						color.b,
						color.a
					}
				};
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	获取 mode 列表
	 */
	VkResult VK_WindowSurface::GetModeList( ModeList& arrModes ) const noexcept
		{
			// 获取 mode 数量
			uint32_t count;

			VkResult res
				{
					vkGetPhysicalDeviceSurfacePresentModesKHR( s_VK_PhyDevice,
															   this->vkSurfaceKHR,
															   &count, nullptr )
				};

			// 获取 mode 列表
			if( res == VK_SUCCESS && count )
				{
					arrModes.resize( count );

					res = vkGetPhysicalDeviceSurfacePresentModesKHR( s_VK_PhyDevice,
																	 this->vkSurfaceKHR,
																	 &count, arrModes.data() );
				}

			return res;
		}


	/*
	 @	获取 format 列表
	 */
	VkResult VK_WindowSurface::GetFormatList( FormatList& arrFormats ) const noexcept
		{
			// 获取 format 数量
			uint32_t count;

			VkResult res
				{
					vkGetPhysicalDeviceSurfaceFormatsKHR( s_VK_PhyDevice,
														  this->vkSurfaceKHR,
														  &count, nullptr )
				};

			// 获取 format 列表
			if( res == VK_SUCCESS && count )
				{
					arrFormats.resize( count );

					res = vkGetPhysicalDeviceSurfaceFormatsKHR( s_VK_PhyDevice,
																this->vkSurfaceKHR,
																&count, arrFormats.data() );
				}

			return res;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	更新 当前状态
	 */
	FC_INLINE void VK_WindowSurface::Update() noexcept
		{
			// 更新 当前 command buffer
			this->current.commandBuffer = this->presentChain[ this->current.frameIndex ];

			// 更新 当前 frame buffer，需要和 swap chain 的 view index 一致
			this->infoBeginRenderPass.framebuffer = this->frameChain[ this->current.frameIndex ];
		}


	/*
	 @	结束 渲染命令
	 */
	FC_INLINE VkResult VK_WindowSurface::End() noexcept
		{
			// 结束 render pass
			vkCmdEndRenderPass( this->current.commandBuffer );

			// 结束 command buffer
			VkResult res
				{
					vkEndCommandBuffer( this->current.commandBuffer )
				};

			if( res == VK_SUCCESS )
				{
					// 提交 Graph 队列
					res = vkQueueSubmit( s_VK_Device->queues.graph,
										 1, &this->info.submit,
										 this->current.syncObjects.queueSubmited );

					if( res == VK_SUCCESS )
						{
							// 提交 Present 队列
							res = vkQueuePresentKHR( s_VK_Device->queues.present,
													 &this->info.present );
						}
				}

			// 更新 下一个环形 sync objects
			this->current.syncObjects = this->presentChain.LoopSyncObjects();

			return res;
		}


	/*
	 @	开始 渲染命令
	 */
	FC_INLINE VkResult VK_WindowSurface::Begin() noexcept
		{
			// 等待 当前最新的 submit fence
			VkResult res
				{
					vkWaitForFences( s_VK_Device,
									 1, &this->current.syncObjects.queueSubmited,
									 VK_TRUE, UInt64_MAX )
				};

			if( res == VK_SUCCESS )
				{
					// 重置 当前最新的 submit fence
					res = vkResetFences( s_VK_Device,
										 1, &this->current.syncObjects.queueSubmited );

					if( res == VK_SUCCESS )
						{
							// 获取 swap chain 下一帧 空闲图像
							res = vkAcquireNextImageKHR( s_VK_Device,
														 this->swapChain,
														 UInt64_MAX,
														 this->current.syncObjects.imageAcquired,
														 VK_NULL_HANDLE,
														 &this->current.frameIndex );

							if( res == VK_SUCCESS )
								{
									// 更新 current frame objects
									this->Update();

									if( res == VK_SUCCESS )
										{
											// 开始 command buffer
											res = vkBeginCommandBuffer( this->current.commandBuffer,
																		&this->info.beginCommand );

											// 开始 render pass
											if( res == VK_SUCCESS )
												{
													vkCmdBeginRenderPass( this->current.commandBuffer,
																		  &this->infoBeginRenderPass,
																		  VK_SUBPASS_CONTENTS_INLINE );
												}
										}
								}
						}
				}

			return res;
		}
}