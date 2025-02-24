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
	 @	构造
	 */
	FC_INLINE Win_Application::Win_Application() noexcept
		:
			handler( NULL ),
			hInstance( NULL )
		{
		}

	/*
	 @	析构
	 */
	FC_INLINE Win_Application::~Win_Application() noexcept
		{
			// 注销 窗口类
			if( this->hInstance )
				{
					UnregisterClass( FCE_NAME,
									 this->hInstance );
				}
		}


	/*
	 @	Post custom message
	 */
	FC_INLINE BOOL Win_Application::Post( const DWord e,
										  const Object* const obj ) noexcept
		{
			return PostMessage( s_Win_Window, e, NULL,
								reinterpret_cast< LPARAM >( obj ) );
		}


	/*
	 @	屏幕截图

			下一步考虑用DX的 Magnification
			或 desktop duplication
	 */
	BOOL Win_Application::Screenshot( Image& img,
									  const int x,
									  const int y,
									  const int width,
									  const int height ) const noexcept
		{
			BOOL res{ FALSE };

			// 获取 屏幕 hdc
			if( HDC hdcScreen{ GetDC( NULL ) } )
				{
					// 创建 截图 hdc
					if( HDC hdc{ CreateCompatibleDC( hdcScreen ) } )
						{
							// 创建 截图 HBITMAP
							if( HBITMAP hBitmap{ CreateCompatibleBitmap( hdcScreen,
																		 width, height ) } )
								{
									// 选取 截图
									SelectBitmap( hdc, hBitmap );

									// 复制 屏幕截图
									if( BitBlt( hdc, 0, 0, width, height,
												hdcScreen, x, y, SRCCOPY ) )
										{
											res = image::Bitmap::Convert( hdc,
																		  hBitmap, img );
										}

									// 销毁 截图
									DeleteBitmap( hBitmap );
								}

							// 销毁 截图 hdc
							DeleteDC( hdc );
						}

					// 释放 屏幕 hdc
					ReleaseDC( NULL, hdcScreen );
				}

			return res;
		}


	/*
	 @	执行 系统命令
	 */
	FC_INLINE DWORD Win_Application::Execute( const stl::wstring& cmdLine,
													stl::string& feedback ) noexcept
		{
			DWORD exitCode{ DWORD( - 1 ) };

			HANDLE hRead{ NULL },
				   hWrite{ NULL };

			// 设置 security attributes
			SECURITY_ATTRIBUTES sa
				{
					.nLength				= sizeof( SECURITY_ATTRIBUTES ),
					.lpSecurityDescriptor	= NULL,
					.bInheritHandle			= TRUE
				};

			// 创建 pipe
			if( CreatePipe( &hRead, &hWrite, &sa, 0 ) )
				{
					PROCESS_INFORMATION pi;

					// 设置 startup info
					STARTUPINFOW si;

					GetStartupInfoW( &si );
						{
							// 绑定 std error & output
							si.hStdError = hWrite;
							si.hStdOutput = hWrite;

							// 启用 std io
							si.dwFlags = STARTF_USESTDHANDLES;
						}

					// 启动进程 执行 cmd
					if( CreateProcessW( NULL, LPWSTR( cmdLine.c_str() ),
										NULL, NULL, TRUE,
										CREATE_NO_WINDOW,
										NULL, NULL, &si, &pi ) )
						{
							// 等待进程 完成 cmd
							if( WaitForSingleObject( pi.hProcess, INFINITE ) == WAIT_OBJECT_0 )
								{
									// 获取 exit code
									if( GetExitCodeProcess( pi.hProcess, &exitCode ) )
										{
											// 关闭 thread & process
											CloseHandle( pi.hThread );
											CloseHandle( pi.hProcess );

											// 关闭 pipe write
											CloseHandle( hWrite );

											// 循环读取 管道信息
											char buf[ 1024 ];

											DWORD bytesRead;

											while( ReadFile( hRead, buf,
															 sizeof( buf ),
															 &bytesRead, NULL ) ) 
												{
													feedback.append( buf, bytesRead );
												}

											// 关闭 pipe read
											CloseHandle( hRead );

											// 返回 exit code
											return exitCode;
										}
								}

							// 失败后 关闭 thread & process
							CloseHandle( pi.hThread );
							CloseHandle( pi.hProcess );
						}

					// 失败后 关闭 read & write
					CloseHandle( hRead );
					CloseHandle( hWrite );
				}

			// 失败返回 -1
			return exitCode;
		}


	/*
	 @	注册 窗口类
	 */
	FC_INLINE ATOM Win_Application::Register( HINSTANCE hInstc ) noexcept
		{
			// 可配置
			UINT style{ CS_DBLCLKS };

			// 注册 App class，名字要改 太单一
			const WNDCLASSEX wcex
				{
					.cbSize			= sizeof( WNDCLASSEX ),
					.style			= style,
					.lpfnWndProc	= WndProc,
					.cbClsExtra		= 0,
					.cbWndExtra		= 0,
					.hInstance		= hInstc,
					.hIcon			= LoadIcon( hInstc, TEXT( "" ) ),
					.hCursor		= LoadCursor( NULL, IDC_HAND ),
					.hbrBackground	= NULL,
					.lpszMenuName	= NULL,
					.lpszClassName	= FCE_NAME,
					.hIconSm		= LoadIcon( hInstc, TEXT( "" ) )
				};

			return RegisterClassEx( &wcex );
		}


	/*
	 @	创建
	 */
	FC_INLINE BOOL Win_Application::Create( HINSTANCE hInstc,
											LPWSTR lpCmdLine,
											int nCmdShow ) NOEXCEPT
		{
			// 注册 窗口类
			if( Register( hInstc ) )
				{
					// 保存 程序实例句柄
					this->hInstance = hInstc;

					// 响应 s_App 创建
					return s_App.on_Create( lpCmdLine );
				}

			return FALSE;
		}


	/*
	 @	销毁

			1.创建失败时调用
			2.响应 EVENT::EXIT
	 */
	FC_INLINE VOID Win_Application::Destroy() NOEXCEPT
		{
			// 触发 s_App 销毁
			s_App.on_Destroy();
		}


	/*
	 @	消息循环
	 */
	FC_INLINE int Win_Application::Run() NOEXCEPT
		{
			// 主消息循环:
			MSG msg;

			while( true )
				{
					// 捕获 作业时 异常
					#ifdef FC_SECURITY
					try	{
					#endif

						// 只取 hwnd 为本窗口 的消息
						if( PeekMessage( &msg, NULL,
										 0, 0, PM_REMOVE ) )
							{
								// 转换并分发 msg 到 WndProc
								TranslateMessage( &msg );
								DispatchMessage( &msg );

								// 检查 退出标志
								if( msg.message == WM_QUIT )
									{
										break;
									}
							}
						else
							{
								// 触发 s_App 帧更新
								s_App.on_Frame();

								// 等待 新消息
								WaitMessage();
							}

					#ifdef FC_SECURITY
						}
					catch( ... )
						{
							s_App.on_Exception();
						}
					#endif
				}

			// 返回 exit code
			return int( msg.wParam );
		}


	/*
	 @	清空 消息
	 
			需要在销毁窗口前 清理完 Object
			一旦窗口销毁 将退出消息循环 导致泄漏
	 */
	FC_INLINE VOID Win_Application::Finish() noexcept
		{
			// 首先：切换到 WndProc 2 终止 app 原有消息处理逻辑
			SetWindowLongPtr( s_Win_Window,
							  GWLP_WNDPROC,
							  LONG_PTR( WndProc2 ) );

			// 然后：将残留消息 全部转发到 WndProc 2 进行处理
			MSG msg;

			while( PeekMessage( &msg, NULL,
								0, 0, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
		}


	/*
	 @	是否 自定义消息
	 */
	FC_INLINE Bool Win_Application::isCustomMessage( const UINT uMsg ) noexcept
		{
			return uMsg >= WM_APP && uMsg <= 0xBFFF;
		}


	/*
	 @	处理 窗口的消息

			hWnd		主窗口句柄
			uMsg		消息结构
			wParam		消息标识
			lParam		消息数据
	 */
	LRESULT CALLBACK Win_Application::WndProc( HWND hWnd,
											   const UINT uMsg,
											   const WPARAM wParam,
											   const LPARAM lParam ) noexcept
		{
			// 自定义 消息
			if( isCustomMessage( uMsg ) )
				{
					s_App.Dispatch( Application::EVENT( uMsg ),
									reinterpret_cast< Object* >( lParam ) );

					return 0;
				}
			// 主窗口 消息
			else if( s_Win_Window == hWnd )
				{
					if( s_Win_Window.Dispatch( uMsg, wParam, lParam ) )
						{
							return 0;
						}
				}
			// 子窗口 消息
			else if( Window* child = s_Window.FindChild( hWnd ) )
				{
					if( child->impl.Dispatch( uMsg, wParam, lParam ) )
						{
							return 0;
						}
				}
			// 窗口创建 消息
			else if( uMsg == WM_CREATE )
				{
					// 暂时在这里 设置剪贴板监听
					AddClipboardFormatListener( hWnd );
				}
			// 其它 消息
			else if( s_Win_App.handler )
				{
					return s_Win_App.handler( hWnd, uMsg,
											  wParam, lParam );
				}

			// 系统 默认
			return DefWindowProc( hWnd, uMsg,
								  wParam, lParam );
		}


	/*
	 @	执行退出后, 用于善后的 WndProc2
	 */
	LRESULT CALLBACK Win_Application::WndProc2( HWND hWnd,
												const UINT uMsg,
												const WPARAM wParam,
												const LPARAM lParam ) noexcept
		{
			// 清理 自定义消息
			if( isCustomMessage( uMsg ) )
				{
					// 析构 object
					if( Object* const obj =
							reinterpret_cast< Object* >( lParam ) )
						{
							MemoryManagement::Destruct( obj );
						}
				}
			// 退出消息循环
			else if( uMsg == WM_DESTROY )
				{
					PostQuitMessage( 0 );
				}

			// 系统默认处理
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}


	/*
	 @	fc 主函数

			hInstc		程序实例句柄
			lpCmdLine	命令行参数
			nCmdShow	窗口的初始显示方式
	 */
	int Win_Application::Main( HINSTANCE hInstc,
							   LPWSTR lpCmdLine,
							   int nCmdShow ) noexcept
		{
			// 捕获异常，主要是 创建 和 销毁 阶段
			#ifdef FC_SECURITY
			try	{
			#endif

				// 创建 win app
				if( s_Win_App.Create( hInstc,
									  lpCmdLine,
									  nCmdShow ) )
					{
						// 进入 主消息循环
						return s_Win_App.Run();
					}
				else
					{
						// 创建失败 销毁
						s_Win_App.Destroy();
					}

			#ifdef FC_SECURITY
				}
			catch( ... )
				{
					std::terminate();
				}
			#endif

			return 1;
		}
}