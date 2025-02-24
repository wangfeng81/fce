/*
**
**	Window.
**
		析构 不能调用 impl::Destroy()
		Window::Destroy() 调用 两者的 销毁

	Copyright (c) WangFeng since 1999...
*/

#include "../../include/platform/Application.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Window.cxx"
#elif defined  FC_ANDROID
	#include "android/Android_Window.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_Window.cxx"
#elif defined  FC_IOS
	#include "ios/Ios_Window.cxx"
#elif defined  FC_MAC
	#include "mac/Mac_Window.cxx"
#endif


/**
 !	Window inner kit
 */
namespace fce::inner::window
{
	/*
	 @	构造
	 */
	Config::Config() noexcept
		:
			size( 1024, 768 )
		{
		}

	/*
	 @	构造
	 */
	Message::Message( Window& window ) noexcept
		:
			e( EVENT::MAXIMUM ),
			window( window )
		{
		}

	/*
	 @	 分发 消息
	 */
	Void Message::Dispatch() const noexcept
		{
			this->window.Dispatch( *this );
		}
}



namespace fce
{
	/*
	 @	构造
	 */
	Window::Window( Window* parent ) noexcept
		:
			impl( *this ),
			surface( *this ),

			ime( *this ),
			mouse( *this ),
			keyboard( *this ),
			sceneGroup( this->surface ),

			focused( false ),
			parent( parent )
		{
		}

	/*
	 @	析构
	 */
	Window::~Window() noexcept
		{
		}


	/*
	 @	创建 窗口
	 */
	Bool Window::Create( const Config& cfg ) noexcept
		{
			// 创建 impl
			if( this->impl.Create( cfg ) )
				{
					// 创建 surface
					return this->surface.Create() == RESULT::SUCCESS;
				}

			return false;
		}


	/*
	 @	销毁 窗口
	 */
	Void Window::Destroy() noexcept
		{
			// 销毁 子窗口
			for( const auto& p : this->pool )
				{
					Window* wnd{ p.second };

					wnd->Destroy();

					//this->pool.Destroy( wnd );
				}

			// 清空 所有组件
			this->ClearComponents();

			// 清空 场景
			this->sceneGroup.Clear();

			// 销毁 surface
			this->surface.Destroy();

			// 销毁 impl
			this->impl.Destroy();
		}


	/*
	 @	重建 所有 suface
	 */
	Bool Window::Rebuild() noexcept
		{
			// 递归重建 子窗口 surface
			for( const auto& p : this->pool )
				{
					if( ! p.second->Rebuild() )
						{
							return false;
						}
				}

			// 重建 surface
			return this->surface.Rebuild() == RESULT::SUCCESS;
		}


	/*
	 @	截取 窗口快照
	 */
	Bool Window::Snapshot( Image& img ) const noexcept
		{
			return s_App.Screenshot( img,
									 this->GetBounds() );
		}


	/*
	 @	渲染
	 */
	Void Window::Render() noexcept
		{
			// 检查 最小化，可能需要优化
			if( this->surface.visible )
				{
					// 开始
					this->surface.Begin();

					// 渲染
					this->sceneGroup.Render();

					// 结束
					this->surface.End();
				}

			// 递归渲染 子窗口
			for( const auto& p : this->pool )
				{
					// 考虑：判断 pool 是否有 子窗口，进行 renderPipe 的 viewport 设置
					p.second->Render();
				}
		}


	/*
	 @	分发 窗口消息
	 */
	FC_INLINE Void Window::Dispatch( const Message& msg ) noexcept
		{
			// 先响应内部系统
			switch( msg.e )
				{
					// 尺寸 改变
					case EVENT::SIZE:
						{
							// 响应 suface on_Resize
							this->surface.on_Resize();
						}
						break;
				}

			// 发布消息
			this->NotifyHandlers( msg.e,
								  msg );
		}
}



/**
 !	Bounds
 */
namespace fce
{
	/*
	 @	Get bounds
	 */
	Size Window::GetSize() const noexcept
		{
			return this->impl.GetSize();
		}

	Point Window::GetPosition() const noexcept
		{
			return this->impl.GetPosition();
		}

	Rect Window::GetBounds() const noexcept
		{
			return this->impl.GetBounds();
		}

	/*
	 @	Set bounds
	 */
	Void Window::SetSize( const Size& size ) noexcept
		{
			this->impl.SetSize( size );
		}

	Void Window::SetPosition( const Point& pt ) noexcept
		{
			this->impl.SetPosition( pt );
		}

	Void Window::SetBounds( const Rect& rc ) noexcept
		{
			this->impl.SetBounds( rc );
		}

	Void Window::SetBounds( const Point& pt,
							const Size& size ) noexcept
		{
			this->impl.SetBounds( pt, size );
		}
}



/**
 !	Transformation
 */
namespace fce
{
	/*
	 @	Screen <=> Window
	 */
	Point Window::ScreenToWindow( const Point& pt ) const noexcept
		{
			return pt - this->GetPosition();
		}

	Point Window::WindowToScreen( const Point& pt ) const noexcept
		{
			return pt + this->GetPosition();
		}

	/*
	 @	Screen <=> Surface
	 */
	Point Window::ScreenToSurface( const Point& pt ) const noexcept
		{
			return pt - this->surface.GetPosition();
		}

	Point Window::SurfaceToScreen( const Point& pt ) const noexcept
		{
			return pt + this->surface.GetPosition();
		}

	/*
	 @	Surface <=> Window
	 */
	Point Window::SurfaceToWindow( const Point& pt ) const noexcept
		{
			return this->ScreenToWindow
						(
							this->SurfaceToScreen( pt )
						);
		}

	Point Window::WindowToSurface( const Point& pt ) const noexcept
		{
			return this->ScreenToSurface
						(
							this->WindowToScreen( pt )
						);
		}
}



/**
 !	子窗口
 */
namespace fce
{
	/*
	 @	移除 子窗口
	 */
	Void Window::RemoveChild( const Window* wnd ) noexcept
		{
			this->pool.Destroy( wnd );
		}

	Void Window::RemoveChild( const stl::string& name ) noexcept
		{
			this->pool.Destroy( name );
		}


	/*
	 @	查找 子窗口
	 */
	Window* Window::FindChild( const stl::string& name ) const noexcept
		{
			// 先查找 直系
			Window* wnd{ this->pool.Find( name ) };

			// 没找到 则递归
			if( wnd == nullptr )
				{
					for( const auto& p : this->pool )
						{
							if( wnd = p.second->FindChild( name ) )
								{
									return wnd;
								}
						}
				}

			return wnd;
		}

	Window* Window::FindChild( const Impl::HANDLE handle ) const noexcept
		{
			// 遍历 子窗口
			for( const auto& p : this->pool )
				{
					Window* wnd{ p.second };

					if( wnd->impl == handle )
						{
							return wnd;
						}
					else if( wnd = wnd->FindChild( handle ) )
						{
							return wnd;
						}
				}

			return nullptr;
		}


	/*
	 @	添加 子窗口
	 */
	Window* Window::AddChild( const stl::string& name,
							  const Config& cfg ) noexcept
		{
			if( Window* wnd{ this->pool.Generate( name ) } )
				{
					if( wnd->Create( cfg ) )
						{
							return wnd;
						}
				}

			return nullptr;
		}
}