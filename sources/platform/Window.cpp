/*
**
**	Window.
**
		���� ���ܵ��� impl::Destroy()
		Window::Destroy() ���� ���ߵ� ����

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
	 @	����
	 */
	Config::Config() noexcept
		:
			size( 1024, 768 )
		{
		}

	/*
	 @	����
	 */
	Message::Message( Window& window ) noexcept
		:
			e( EVENT::MAXIMUM ),
			window( window )
		{
		}

	/*
	 @	 �ַ� ��Ϣ
	 */
	Void Message::Dispatch() const noexcept
		{
			this->window.Dispatch( *this );
		}
}



namespace fce
{
	/*
	 @	����
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
	 @	����
	 */
	Window::~Window() noexcept
		{
		}


	/*
	 @	���� ����
	 */
	Bool Window::Create( const Config& cfg ) noexcept
		{
			// ���� impl
			if( this->impl.Create( cfg ) )
				{
					// ���� surface
					return this->surface.Create() == RESULT::SUCCESS;
				}

			return false;
		}


	/*
	 @	���� ����
	 */
	Void Window::Destroy() noexcept
		{
			// ���� �Ӵ���
			for( const auto& p : this->pool )
				{
					Window* wnd{ p.second };

					wnd->Destroy();

					//this->pool.Destroy( wnd );
				}

			// ��� �������
			this->ClearComponents();

			// ��� ����
			this->sceneGroup.Clear();

			// ���� surface
			this->surface.Destroy();

			// ���� impl
			this->impl.Destroy();
		}


	/*
	 @	�ؽ� ���� suface
	 */
	Bool Window::Rebuild() noexcept
		{
			// �ݹ��ؽ� �Ӵ��� surface
			for( const auto& p : this->pool )
				{
					if( ! p.second->Rebuild() )
						{
							return false;
						}
				}

			// �ؽ� surface
			return this->surface.Rebuild() == RESULT::SUCCESS;
		}


	/*
	 @	��ȡ ���ڿ���
	 */
	Bool Window::Snapshot( Image& img ) const noexcept
		{
			return s_App.Screenshot( img,
									 this->GetBounds() );
		}


	/*
	 @	��Ⱦ
	 */
	Void Window::Render() noexcept
		{
			// ��� ��С����������Ҫ�Ż�
			if( this->surface.visible )
				{
					// ��ʼ
					this->surface.Begin();

					// ��Ⱦ
					this->sceneGroup.Render();

					// ����
					this->surface.End();
				}

			// �ݹ���Ⱦ �Ӵ���
			for( const auto& p : this->pool )
				{
					// ���ǣ��ж� pool �Ƿ��� �Ӵ��ڣ����� renderPipe �� viewport ����
					p.second->Render();
				}
		}


	/*
	 @	�ַ� ������Ϣ
	 */
	FC_INLINE Void Window::Dispatch( const Message& msg ) noexcept
		{
			// ����Ӧ�ڲ�ϵͳ
			switch( msg.e )
				{
					// �ߴ� �ı�
					case EVENT::SIZE:
						{
							// ��Ӧ suface on_Resize
							this->surface.on_Resize();
						}
						break;
				}

			// ������Ϣ
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
 !	�Ӵ���
 */
namespace fce
{
	/*
	 @	�Ƴ� �Ӵ���
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
	 @	���� �Ӵ���
	 */
	Window* Window::FindChild( const stl::string& name ) const noexcept
		{
			// �Ȳ��� ֱϵ
			Window* wnd{ this->pool.Find( name ) };

			// û�ҵ� ��ݹ�
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
			// ���� �Ӵ���
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
	 @	��� �Ӵ���
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