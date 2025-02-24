/*
**
**	Window.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "../math/Box.h"
#include "../input/Input.h"
#include "../scene/Scene.h"

#include "../graphics/WindowSurface.h"


/**
 !	Window inner kit
 */
namespace fce::inner::window
{
	/*	Mode
	 */
	enum struct MODE
		:
			Byte
		{
			HIDE,

			MAXIMIZE,
			MINIMIZE,

			RESTORE,
			CONFIGURE,

			NORMAL,
			EXCLUSIVE,

			MAXIMUM
		};

	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			PAINT,

			MOVE,
			SIZE,
			MOVING,
			SIZING,

			FILE,
			FOCUS,

			MAXIMUM
		};

	/*
	 !	Config
	 */
	struct FC_API Config final
		{
			/*	Size
			*/
			Size size;

		public:
			/*	Constructor
			*/
			Config() noexcept;
		};

	/*
	 !	Message
	 */
	struct FC_API Message final
		{
			/*	EVENT
			*/
			EVENT e;

			/*	File pos
			*/
			Point ptFiles;

			/*	File list
			*/
			stl::vector< stl::wstring > listFiles;

			/*	Source window
			*/
			Window& window;

		public:
			/*	Constructor
			*/
			Message( Window& ) noexcept;

			/*	Dispatch
			*/
			Void Dispatch() const noexcept;
		};

	/*	Delegate group wrapper
	*/
	using DelegateHandler = InvokerGroupWrapper< UInt8( EVENT::MAXIMUM ),
												 EVENT, const Message& >;
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_Window.h"
#elif defined  FC_ANDROID
	#include "android/Android_Window.h"
#elif defined  FC_LINUX
	#include "linux/Linux_Window.h"
#elif defined  FC_IOS
	#include "ios/Ios_Window.h"
#elif defined  FC_MAC
	#include "mac/Mac_Window.h"
#endif


namespace fce
{
	/*
	 !	Window
	 */
	class Window final
		:
		public Entity,

		public inner::window
					::DelegateHandler
		{
			/*	Friends
			*/
			friend Engine;

			friend Application;

			friend inner::window::Impl;

			friend inner::graphics::Impl;

			friend inner::window::Message;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( Application,
						Window, stl::string );

		public:
			/*	Using inner kit
			*/
			using Impl = inner::window::Impl;

			using MODE = inner::window::MODE;

			using EVENT = inner::window::EVENT;

			using Config = inner::window::Config;

			using Message = inner::window::Message;

			using Surface = inner::window::Surface;

			/*	Implementation
			*/
			Impl impl;

			/*	Input
			*/
			Ime ime;

			Mouse mouse;

			Keyboard keyboard;

			/*	Surface
			*/
			Surface surface;

			/*	Scene group
			*/
			SceneGroup sceneGroup;

		public:
			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Has focus
			*/
			[[nodiscard]] Bool hasFocus() const noexcept;

			/*	Get title
			*/
			[[nodiscard]] String GetTitle() const noexcept;

			/*	Get bounds
			*/
			[[nodiscard]] Size GetSize() const noexcept;

			[[nodiscard]] Point GetPosition() const noexcept;

			[[nodiscard]] Rect GetBounds() const noexcept;

			/*	Transformation
			*/
			[[nodiscard]] Point ScreenToWindow( const Point& ) const noexcept;
			[[nodiscard]] Point WindowToScreen( const Point& ) const noexcept;

			[[nodiscard]] Point ScreenToSurface( const Point& ) const noexcept;
			[[nodiscard]] Point SurfaceToScreen( const Point& ) const noexcept;

			[[nodiscard]] Point WindowToSurface( const Point& ) const noexcept;
			[[nodiscard]] Point SurfaceToWindow( const Point& ) const noexcept;

			/*	Set bounds
			*/
			Void SetSize( const Size& ) noexcept;

			Void SetPosition( const Point& ) noexcept;

			Void SetBounds( const Rect& ) noexcept;

			Void SetBounds( const Point&,
							const Size& ) noexcept;

			/*	Set accept files
			*/
			Void SetAcceptFiles( const Bool accept ) noexcept;

			/*	Set mode
			*/
			Void SetMode( const MODE ) noexcept;

			/*	Set title
			*/
			template< typename Fmt,
					  typename ...Args >

			Bool SetTitle( const Fmt&,
						   const Args&... );

			Bool GetTitle( String& str ) const noexcept;

			/*	Snapshot
			*/
			Bool Snapshot( Image& ) const noexcept;

			/*	System message box
			*/
			template< typename Fmt,
					  typename ...Args >

			Bool ErrorBox( const Fmt&,
						   const Args&... ) const;

			template< typename Fmt,
					  typename ...Args >

			Void PromptBox( const Fmt&,
							const Args&... ) const;

			template< typename Fmt,
					  typename ...Args >

			Bool ConfirmBox( const Fmt&,
							 const Args&... ) const;

			/*	Child window
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt ChildCount() const noexcept;

			Void RemoveChild( const Window* ) noexcept;

			Void RemoveChild( const stl::string& ) noexcept;

			Window* FindChild( const Impl::HANDLE ) const noexcept;

			Window* FindChild( const stl::string& ) const noexcept;

			Window* AddChild( const stl::string&,
							  const Config& = Window::Config() ) noexcept;

		protected:
			/*	Constructor & Destructor
			 */
			~Window() noexcept;

			Window( Window* = nullptr ) noexcept;

			/*	Create & Destroy
			*/
			Bool Create( const Config& = Window::Config() ) noexcept;

			Void Destroy() noexcept;

			/*	Rebuild
			*/
			Bool Rebuild() noexcept;

			/*	Render
			*/
			Void Render() noexcept;

			/*	Dispatch
			*/
			Void Dispatch( const Message& ) noexcept;

		private:
			/*	Parent
			*/
			Window* parent;

			/*	focused		不要缓存数据 用API获取
			*/
			Bool focused;

			/*	Bounds		不要缓存数据 用API获取
			*/
			//Point pos, clientPos;

			//Size size, clientSize;

			/*	Asset pool
			*/
			AssetPool< Application,
					   Window, stl::string > pool;
		};


	/*
	 @	Window singleton shortcut
	 */
	extern FC_API Window& s_Window;
}



/**
 !	class Window
 */
namespace fce
{
	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& Window::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}

	/*
	 @	Set accept files
	 */
	FC_INLINE Void Window::SetAcceptFiles( const Bool accept ) noexcept
		{
			this->impl.SetAcceptFiles( accept );
		}

	/*
	 @	Set mode
	 */
	FC_INLINE Void Window::SetMode( const MODE mode ) noexcept
		{
			this->impl.SetMode( mode );
		}

	/*
	 @	Has focus
	 */
	FC_INLINE Bool Window::hasFocus() const noexcept
		{
			return this->focused;
		}

	/*	Get title
	*/
	FC_INLINE String Window::GetTitle() const noexcept
		{
			String str;

			this->GetTitle( str );

			return str;
		}

	FC_INLINE Bool Window::GetTitle( String& str ) const noexcept
		{
			return this->impl.GetTitle( str );
		}

	/*
	 @	Set title
	 */
	template< typename Fmt,
			  typename ...Args >

	FC_INLINE Bool Window::SetTitle( const Fmt& fmt,
									 const Args& ...args )
		{
			return this->impl.SetTitle( Format( fmt, args... ) );
		}
}



/**
 !	System message box
 */
namespace fce
{
	/*
	 @	Error box
	 */
	template< typename Fmt,
			  typename ...Args >

	FC_INLINE Bool Window::ErrorBox( const Fmt& fmt,
									 const Args& ...args ) const
		{
			return this->impl.ErrorBox( Format( fmt, args... ) );
		}

	/*
	 @	Prompt box
	 */
	template< typename Fmt,
			  typename ...Args >

	FC_INLINE Void Window::PromptBox( const Fmt& fmt,
									  const Args& ...args ) const
		{
			this->impl.PromptBox( Format( fmt, args... ) );
		}

	/*
	 @	Confirm box
	 */
	template< typename Fmt,
			  typename ...Args >

	FC_INLINE Bool Window::ConfirmBox( const Fmt& fmt,
									   const Args& ...args ) const
		{
			return this->impl.ConfirmBox( Format( fmt, args... ) );
		}
}



/**
 !	Child window
 */
namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto Window::end() const noexcept
		{
			return this->pool.end();
		}

	FC_INLINE auto Window::begin() const noexcept
		{
			return this->pool.begin();
		}

	/*
	 @	Count of children
	 */
	FC_INLINE UInt Window::ChildCount() const noexcept
		{
			return this->pool.Count();
		}
}