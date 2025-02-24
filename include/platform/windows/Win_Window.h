/*
**
**	Win_Window.
**
		Windows platform implementation

	Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::win
{
	/*
	 !	Win window
	 */
	class Win_Window final
		{
			/*	Friends
			*/
			friend Window;

			friend WindowSurface;

			friend Win_Application;

			/*	Using inner kit
			*/
			using HANDLE = HWND;

			using MODE = window::MODE;

			using EVENT = window::EVENT;

			using Config = window::Config;

			using Message = window::Message;

		public:
			/*	Input
			*/
			Win_Ime& ime;

			Win_Mouse& mouse;

			Win_Keyboard& keyboard;

			/*	Get HWND
			*/
			operator const HWND() const noexcept;

			const HWND& GetHandle() const noexcept;

		protected:
			/*	Create & Destroy
			*/
			BOOL Create( const Config& ) noexcept;

			VOID Destroy() noexcept;

			/*	Get bounds
			*/
			SIZE GetSize() const noexcept;

			POINT GetPosition() const noexcept;

			RECT GetBounds() const noexcept;

			/*	Get client bounds
			*/
			SIZE GetClientSize() const noexcept;

			POINT GetClientPosition() const noexcept;

			RECT GetClientBounds() const noexcept;

			/*	Set bounds
			*/
			BOOL SetSize( const Size& ) noexcept;

			BOOL SetPosition( const Point& ) noexcept;

			BOOL SetBounds( const Rect& ) noexcept;

			BOOL SetBounds( const Point&,
							const Size&,
							const UINT = SWP_NOZORDER ) noexcept;

			/*	Set client bounds
			*/
			BOOL SetClientSize( const Size& ) noexcept;

			BOOL SetClientPosition( const Point& ) noexcept;

			BOOL SetClientBounds( const Rect& ) noexcept;

			BOOL SetClientBounds( const Point&,
								  const Size& ) noexcept;

			BOOL SetClientBounds( RECT&,
								  const UINT = SWP_NOZORDER ) noexcept;

			/*	Get & Set title
			*/
			int GetTitle( String& ) const noexcept;

			BOOL SetTitle( const stl::wstring& ) noexcept;

			/*	Common
			*/
			BOOL SetMode( const MODE ) noexcept;

			VOID SetAcceptFiles( const BOOL accept ) noexcept;

			/*	System message box
			*/
			Bool ErrorBox( const stl::wstring& ) const noexcept;

			VOID PromptBox( const stl::wstring& ) const noexcept;

			Bool ConfirmBox( const stl::wstring& ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Window );

			~Win_Window() noexcept;

			Win_Window( Window& ) noexcept;

			/*	Handle messages
			*/
			VOID on_WM_CREATE( const WPARAM,
							   const LPARAM ) noexcept;

			VOID on_WM_MOVE( const WPARAM,
							 const LPARAM ) noexcept;

			VOID on_WM_SIZE( const WPARAM,
							 const LPARAM ) noexcept;

			VOID on_WM_ACTIVATE( const WPARAM,
								 const LPARAM ) noexcept;

			VOID on_WM_DROPFILES( const WPARAM,
								  const LPARAM ) noexcept;

			VOID on_WndProc( const UINT,
							 const WPARAM,
							 const LPARAM ) noexcept;

			BOOL Dispatch( const UINT,
						   const WPARAM,
						   const LPARAM ) noexcept;

		private:
			/*	HWND
			*/
			HWND hWnd;

			/*	Message
			*/
			Message message;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::window
{
	/*
	 @	Window win impl
	 */
	using Impl = win::Win_Window;

	/*
	 @	Main Win_Window singleton shortcut.
	 */
	#define s_Win_Window s_Window.impl
}



/**
 !	class Win_Window
 */
namespace fce::inner::win
{
	/*
	 @	Get HWND
	 */
	FC_INLINE Win_Window::operator const HWND() const noexcept
		{
			return this->hWnd;
		}

	FC_INLINE const HWND& Win_Window::GetHandle() const noexcept
		{
			return this->hWnd;
		}
}



/**
 !	Common
 */
namespace fce::inner::win
{
	/*
	 @	Set accept files
	 */
	FC_INLINE VOID Win_Window::SetAcceptFiles( const BOOL accept ) noexcept
		{
			DragAcceptFiles( this->hWnd, accept );
		}

	/*
	 @	Set title
	 */
	FC_INLINE BOOL Win_Window::SetTitle( const stl::wstring& wstr ) noexcept
		{
			return SetWindowTextW( this->hWnd, wstr.c_str() );
		}
}