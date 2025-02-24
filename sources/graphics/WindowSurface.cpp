/*
**
**	Window surface.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/platform/Application.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_WindowSurface.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_WindowSurface.cxx"
#elif defined  FC_METAL
	#include "metal/MT_WindowSurface.cxx"
#elif defined  FC_GLES
	#include "gles/ES_WindowSurface.cxx"
#endif


namespace fce::inner::window
{
	/*
	 @	构造
	 */
	Surface::Surface( Window& window ) noexcept
		:
			current{},
			impl( *this ),
			visible( true ),
			window( window )
		{
		}
}



namespace fce::inner::window
{
	/*
	 @	销毁
	 */
	Void Surface::Destroy() noexcept
		{
			// 调用 impl 销毁
			this->impl.Destroy();
		}

	/*
	 @	创建
	 */
	RESULT Surface::Create() noexcept
		{
			// 调用 impl 创建
			RESULT res
				{
					Graphics::Convert
						(
							this->impl.Create()
						)
				};

			return res;
		}

	/*
	 @	重建
	 */
	RESULT Surface::Rebuild() noexcept
		{
			// 调用 impl 重建
			RESULT res
				{
					Graphics::Convert
						(
							this->impl.Rebuild()
						)
				};

			return res;
		}
}



namespace fce::inner::window
{
	/*
	 @	响应 size
	 */
	Bool Surface::on_Resize() noexcept
		{
			// 获取 新尺寸
			Size size
				{
					this->GetSize()
				};

			// size 不能为 0，比如 窗口最小化 时
			if( this->visible = size.x && size.y )
				{
					// 重建
					return this->Rebuild() == RESULT::SUCCESS;
				}

			return false;
		}

	/*
	 @	截取 快照
	 */
	Bool Surface::Snapshot( Image& img ) const noexcept
		{
			return s_App.Screenshot( img,
									 this->GetBounds() );
		}
}



namespace fce::inner::window
{
	/*
	 @	End
	 */
	Void Surface::End() noexcept
		{
			this->impl.End();
		}

	/*
	 @	Begin
	 */
	Void Surface::Begin() noexcept
		{
			this->current = {};

			this->impl.Begin();
		}
}



namespace fce::inner::window
{
	/*
	 @	Set clear color
	 */
	Void Surface::SetClearColor( const Color4F& color ) noexcept
		{
			this->impl.SetClearColor( color );
		}

	/*
	 @	Bind render pipeline
	 */
	Void Surface::Bind( const RenderPipeline* const rpl ) NOEXCEPT
		{
			FC_ASSERT( rpl );

			if( this->current.renderPipeline != rpl )
				{
					this->current.renderPipeline = rpl;

					this->impl.Bind( rpl->impl );
				}
		}
}



/**
 !	Bounds
 */
namespace fce::inner::window
{
	/*
	 @	Get client bounds
	 */
	Size Surface::GetSize() const noexcept
		{
			return this->window.impl.GetClientSize();
		}

	Point Surface::GetPosition() const noexcept
		{
			return this->window.impl.GetClientPosition();
		}

	Rect Surface::GetBounds() const noexcept
		{
			return this->window.impl.GetClientBounds();
		}

	/*
	 @	Set client bounds
	 */
	Void Surface::SetSize( const Size& size ) noexcept
		{
			this->window.impl.SetClientSize( size );
		}

	Void Surface::SetPosition( const Point& pt ) noexcept
		{
			this->window.impl.SetClientPosition( pt );
		}

	Void Surface::SetBounds( const Rect& rc ) noexcept
		{
			this->window.impl.SetClientBounds( rc );
		}

	Void Surface::SetBounds( const Point& pt,
							 const Size& size ) noexcept
		{
			this->window.impl.SetClientBounds( pt, size );
		}
}