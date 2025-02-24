/*
**
**	Win_Ime.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
	 !	Windows Ime.
	 */
	class FC_API Win_Ime final
		{
			/*	Friends
			*/
			friend Ime;

			friend Win_Window;

			/*	Using inner kit
			*/
			using EVENT = ime::EVENT;

			using Message = ime::Message;

		public:
			/*	Implementation
			*/
			BOOL SetPosition( const LONG x,
							  const LONG y ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Ime );

			~Win_Ime() = default;

			Win_Ime( Ime& ) noexcept;

			/*	Handle messages
			*/
			VOID InputString() noexcept;

			Bool on_WM_CHAR( const WPARAM ) noexcept;

			Bool on_WndProc( const UINT,
							 const WPARAM,
							 const LPARAM ) noexcept;

		private:
			/*	Win_Window
			*/
			Win_Window& win_Window;

			/*	Input string
			*/
			stl::wstring wstr;

			/*	Message
			*/
			Message message;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::ime
{
	/*
	 @	Ime win impl
	 */
	using Impl = win::Win_Ime;

	/*
	 @	Win_Ime singleton shortcut.
	 */
	#define s_Win_Ime s_Ime.impl
}



/**
 !	class Win_Ime
 */
namespace fce::inner::win
{
}