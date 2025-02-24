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
	 @	����
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
	 @	����
	 */
	void VK_WindowSurface::Destroy() noexcept
		{
			// ���� present command chain
			this->presentChain.Destroy();

			// ���� frame buffer chain
			this->frameChain.Destroy();

			// ���� swap chain
			this->swapChain.Destroy();

			// ���� surface
			this->DestroySurface();
		}


	/*
	 @	����
	 */
	VkResult VK_WindowSurface::Create() noexcept
		{
			// ���� surface
			VkResult res
				{
					this->CreateSurface( this->superior.window.impl )
				};

			if( res == VK_SUCCESS )
				{
					// ���� swap chain
					res = this->swapChain.Create( *this );

					if( res == VK_SUCCESS )
						{
							// ���� frame buffer chain
							res = this->frameChain.CreateCompatible( this->swapChain );

							if( res == VK_SUCCESS )
								{
									// ���� present command chain
									res = this->presentChain.Create( this->swapChain );

									if( res == VK_SUCCESS )
										{
											// ��ʼ��
											this->Initialize();
										}
								}
						}
				}

			return res;
		}


	/*
	 @	�ؽ�
	 */
	VkResult VK_WindowSurface::Rebuild() noexcept
		{
			// ����Ƿ��Ѵ��� device�����ȴ��豸����
			VkResult res
				{
					s_VK_Device.WaitIdle()
				};

			if( res == VK_SUCCESS )
				{
					// ����
					this->Destroy();

					// ����
					res = this->Create();

					if( res == VK_SUCCESS )
						{
							// �ؽ� RenderPipeline
							// Ҳ���Կ��� ��ֹ�ֶ����� RenderPipeline�������Ϳ��� ȫ�����¼�����
							// Ŀǰ��ȫ�����¼���
							if( ! s_AssetMgmt.ReloadAll< RenderPipeline >() )
								{
									res = VK_ERROR_UNKNOWN;
								};
						}
				}

			return res;
		}


	/*
	 @	��ʼ��
	 */
	FC_INLINE void VK_WindowSurface::Initialize() noexcept
		{
			// ��ʼ�� ��ǰ sync objects
			this->current.syncObjects = *this->presentChain.curSyncObjects;

			// ��ʼ�� RenderPass begin info
			this->infoBeginRenderPass.renderArea.extent = this->swapChain->extent2D;

			this->infoBeginRenderPass.renderPass = this->frameChain.renderPass->impl;
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� surface
	 */
	FC_INLINE void VK_WindowSurface::DestroySurface() noexcept
		{
			// ���� VkSurfaceKHR
			if( this->vkSurfaceKHR )
				{
					vkDestroySurfaceKHR( s_VK_Instance,
										 this->vkSurfaceKHR,
										 s_VK_Allocator );

					// ���ڿ��� Rebuild ���Ը���
					this->vkSurfaceKHR = VK_NULL_HANDLE;
				}
		}


	/*
	 @	���� ƽ̨���ڵ� չʾҳ��

			�ര��
	 */
	FC_INLINE VkResult VK_WindowSurface::CreateSurface( const auto& implWindow ) noexcept
		{
			// ��ƽ̨�� window surface
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

					// ���� VK_Instance ���� ��������
					return VK_SUCCESS;

			#endif
		}
}



namespace fce::inner::vulkan
{
	/*
	 @	���� clear color
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
	 @	��ȡ mode �б�
	 */
	VkResult VK_WindowSurface::GetModeList( ModeList& arrModes ) const noexcept
		{
			// ��ȡ mode ����
			uint32_t count;

			VkResult res
				{
					vkGetPhysicalDeviceSurfacePresentModesKHR( s_VK_PhyDevice,
															   this->vkSurfaceKHR,
															   &count, nullptr )
				};

			// ��ȡ mode �б�
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
	 @	��ȡ format �б�
	 */
	VkResult VK_WindowSurface::GetFormatList( FormatList& arrFormats ) const noexcept
		{
			// ��ȡ format ����
			uint32_t count;

			VkResult res
				{
					vkGetPhysicalDeviceSurfaceFormatsKHR( s_VK_PhyDevice,
														  this->vkSurfaceKHR,
														  &count, nullptr )
				};

			// ��ȡ format �б�
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
	 @	���� ��ǰ״̬
	 */
	FC_INLINE void VK_WindowSurface::Update() noexcept
		{
			// ���� ��ǰ command buffer
			this->current.commandBuffer = this->presentChain[ this->current.frameIndex ];

			// ���� ��ǰ frame buffer����Ҫ�� swap chain �� view index һ��
			this->infoBeginRenderPass.framebuffer = this->frameChain[ this->current.frameIndex ];
		}


	/*
	 @	���� ��Ⱦ����
	 */
	FC_INLINE VkResult VK_WindowSurface::End() noexcept
		{
			// ���� render pass
			vkCmdEndRenderPass( this->current.commandBuffer );

			// ���� command buffer
			VkResult res
				{
					vkEndCommandBuffer( this->current.commandBuffer )
				};

			if( res == VK_SUCCESS )
				{
					// �ύ Graph ����
					res = vkQueueSubmit( s_VK_Device->queues.graph,
										 1, &this->info.submit,
										 this->current.syncObjects.queueSubmited );

					if( res == VK_SUCCESS )
						{
							// �ύ Present ����
							res = vkQueuePresentKHR( s_VK_Device->queues.present,
													 &this->info.present );
						}
				}

			// ���� ��һ������ sync objects
			this->current.syncObjects = this->presentChain.LoopSyncObjects();

			return res;
		}


	/*
	 @	��ʼ ��Ⱦ����
	 */
	FC_INLINE VkResult VK_WindowSurface::Begin() noexcept
		{
			// �ȴ� ��ǰ���µ� submit fence
			VkResult res
				{
					vkWaitForFences( s_VK_Device,
									 1, &this->current.syncObjects.queueSubmited,
									 VK_TRUE, UInt64_MAX )
				};

			if( res == VK_SUCCESS )
				{
					// ���� ��ǰ���µ� submit fence
					res = vkResetFences( s_VK_Device,
										 1, &this->current.syncObjects.queueSubmited );

					if( res == VK_SUCCESS )
						{
							// ��ȡ swap chain ��һ֡ ����ͼ��
							res = vkAcquireNextImageKHR( s_VK_Device,
														 this->swapChain,
														 UInt64_MAX,
														 this->current.syncObjects.imageAcquired,
														 VK_NULL_HANDLE,
														 &this->current.frameIndex );

							if( res == VK_SUCCESS )
								{
									// ���� current frame objects
									this->Update();

									if( res == VK_SUCCESS )
										{
											// ��ʼ command buffer
											res = vkBeginCommandBuffer( this->current.commandBuffer,
																		&this->info.beginCommand );

											// ��ʼ render pass
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