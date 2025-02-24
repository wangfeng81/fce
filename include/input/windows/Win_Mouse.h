/*
**
**	Win_Mouse.
**
		Windows platform implementation

	Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::win
{
	/*
	 !	Win Mouse.

			Void( const Message& )
	 */
	class FC_API Win_Mouse final
		:
		/*	Nonclient area handler
		 */
		public DelegateWrapper
			   < Bool, const mouse::Message& >
		{
			/*	Friends
			*/
			friend Mouse;

			friend Win_Window;

			/*	Using inner kit
			*/
			using EVENT = mouse::EVENT;

			using Message = mouse::Message;

		public:
			/*	Capture messages
			*/
			HWND Capture() noexcept;

			BOOL Uncapture() noexcept;

			/*	Show | Hide  cursor
			*/
			int ShowCursor() noexcept;

			int HideCursor() noexcept;

			/*	Get | Set  position
			*/
			POINT GetPosition() const noexcept;

			BOOL SetPosition( const int x,
							  const int y ) noexcept;

			/*	Check button status
			*/
			Bool isPressed( const int vk ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Mouse );

			~Win_Mouse() = default;

			Win_Mouse( Mouse& ) noexcept;

			/*	Handle messages
			*/
			Bool on_WndProc( const UINT,
							 const WPARAM,
							 const LPARAM ) noexcept;

			Bool on_NcWndProc( const UINT,
							   const WPARAM,
							   const LPARAM ) noexcept;

			Int32 ProcessWheel( const WPARAM ) noexcept;

			Point ProcessPosition( const LPARAM ) noexcept;

			EVENT ProcessXButtonUp( const WPARAM ) noexcept;

			EVENT ProcessXButtonDown( const WPARAM ) noexcept;

			EVENT ProcessXButtonDblClk( const WPARAM ) noexcept;

		private:
			/*	Attached window
			*/
			Window& window;

			/*	Message
			*/
			Message message;

			/*	Key code list
			*/
			static constexpr int KEY_LIST[]
				{
					VK_LBUTTON,
					VK_RBUTTON,
					VK_MBUTTON,

					VK_XBUTTON1,
					VK_XBUTTON2
				};
		};
}



/*
 !	Implementation
 */
namespace fce::inner::mouse
{
	/*
	 @	Mouse win impl
	 */
	using Impl = win::Win_Mouse;

	/*
	 @	Win_Mouse singleton shortcut.
	 */
	#define s_Win_Mouse s_Mouse.impl
}



/**
 !	class Win_Mouse
 */
namespace fce::inner::win
{
	/*
	 @	Cancel the message capture outside the window
	 */
	FC_INLINE BOOL Win_Mouse::Uncapture() noexcept
		{
			return ReleaseCapture();
		}

	/*
	 @	Show the cursor
	 */
	FC_INLINE int Win_Mouse::ShowCursor() noexcept
		{
			return ::ShowCursor( TRUE );
		}

	/*
	 @	Hide the cursor
	 */
	FC_INLINE int Win_Mouse::HideCursor() noexcept
		{
			return ::ShowCursor( FALSE );
		}

	/*
	 @	Get cursor position
	 */
	FC_INLINE POINT Win_Mouse::GetPosition() const noexcept
		{
			POINT pt;

			GetCursorPos( &pt );

			return pt;
		}

	/*
	 @	Set cursor position
	 */
	FC_INLINE BOOL Win_Mouse::SetPosition( const int x,
										   const int y ) noexcept
		{
			return SetCursorPos( x, y );
		}

	/*
	 @	Check whether the key is pressed
	 */
	FC_INLINE Bool Win_Mouse::isPressed( const int vk ) const noexcept
		{
			return GetKeyState( vk ) & 0x8000;
		}
}