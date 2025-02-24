/*
**
**	Win_Application.
**
		Windows platform implementation

	Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::win
{
	/*
	 !	Win Application
	 */
	class Win_Application final
		{
			/*	Friends
			*/
			friend Application;

			friend int APIENTRY ::wWinMain( HINSTANCE,
											HINSTANCE,
											LPWSTR, int );

		public:
			/*	WNDPROC handler
			*/
			using Handler = LRESULT( * )( HWND, UINT,
										  WPARAM, LPARAM );

			/*	Set handler
			*/
			VOID SetHandler( Handler ) noexcept;

			/*	Get HINSTANCE
			*/
			operator const HINSTANCE() const noexcept;

			const HINSTANCE& GetHandle() const noexcept;

			/*	Screen shot
			*/
			BOOL Screenshot( Image&,
							 const int x,
							 const int y,
							 const int width,
							 const int height ) const noexcept;

			/*	Execute console command line
			*/
			DWORD Execute( const stl::wstring& cmdLine,
								 stl::string& feedback ) noexcept;

		protected:
			/*	Finish
			*/
			VOID Finish() noexcept;

			/*	Post message
			
					Default maximum queue length 10000
			*/
			BOOL Post( const DWord e,
					   const Object* const ) noexcept;

			/*	Start number of user message
			*/
			static constexpr DWord USER = WM_APP;

			/*	Get screen size
			*/
			SIZE GetScreenSize() const noexcept;

			int GetScreenWidth() const noexcept;

			int GetScreenHeight() const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_Application );

			~Win_Application() noexcept;

			Win_Application() noexcept;

			/*	Create & Destroy & Run
			*/
			BOOL Create( HINSTANCE, LPWSTR, int ) NOEXCEPT;

			VOID Destroy() NOEXCEPT;

			int Run() NOEXCEPT;

			/*	Register
			*/
			static ATOM Register( HINSTANCE ) noexcept;

			/*	Is custom message
			*/
			static Bool isCustomMessage( const UINT ) noexcept;

			/*	Window proc
			*/
			static LRESULT CALLBACK WndProc( HWND,
											 const UINT,
											 const WPARAM,
											 const LPARAM ) noexcept;

			static LRESULT CALLBACK WndProc2( HWND,
											  const UINT,
											  const WPARAM,
											  const LPARAM ) noexcept;

			/*	Main entry point
			*/
			static int Main( HINSTANCE, LPWSTR, int ) noexcept;

		private:
			/*	Handler
			*/
			Handler handler;

			/*	Instance handle
			*/
			HINSTANCE hInstance;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::application
{
	/*
	 @	Application win impl
	 */
	using Impl = win::Win_Application;
	
	/*
	 @	Win_Application singleton shortcut.
	 */
	#define s_Win_App s_App.impl
}



/**
 !	class Win_Application
 */
namespace fce::inner::win
{
	/*
	 @	Get HINSTANCE
	 */
	FC_INLINE Win_Application::operator const HINSTANCE() const noexcept
		{
			return this->hInstance;
		}

	FC_INLINE const HINSTANCE& Win_Application::GetHandle() const noexcept
		{
			return this->hInstance;
		}

	/*
	 @	Set WNDPROC handler

			Only handles messages that
			are not handled by the Win_Application
	 */
	FC_INLINE VOID Win_Application::SetHandler( Handler handler ) noexcept
		{
			this->handler = handler;
		}
}



/**
 !	Get screen size
 */
namespace fce::inner::win
{
	/*
	 @	Screen width
	 */
	FC_INLINE int Win_Application::GetScreenWidth() const noexcept
		{
			return GetSystemMetrics( SM_CXSCREEN );
		}

	/*
	 @	Screen height
	 */
	FC_INLINE int Win_Application::GetScreenHeight() const noexcept
		{
			return GetSystemMetrics( SM_CYSCREEN );
		}

	/*
	 @	Screen size
	 */
	FC_INLINE SIZE Win_Application::GetScreenSize() const noexcept
		{
			return SIZE{ GetSystemMetrics( SM_CXSCREEN ),
						 GetSystemMetrics( SM_CYSCREEN ) };
		}
}


/*
 @	Launch the framework
 */
#define FC_LAUNCH( FRAMEWORK )												\
																			\
		FC_INJECT_FRAMEWORK( FRAMEWORK )									\
																			\
		int APIENTRY wWinMain( _In_		HINSTANCE hInstc,					\
							   _In_opt_	HINSTANCE hPrevInstc,				\
							   _In_		LPWSTR lpCmdLine,					\
							   _In_		int nCmdShow )						\
			{																\
				UNREFERENCED_PARAMETER( hPrevInstc );						\
																			\
				return fce::inner::win::Win_Application::Main( hInstc,		\
															   lpCmdLine,	\
															   nCmdShow );	\
			}