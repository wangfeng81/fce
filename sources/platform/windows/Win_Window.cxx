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
	 @	构造
	 */
	Win_Window::Win_Window( Window& window ) noexcept
		:
			hWnd( NULL ),
			message( window ),

			ime( window.ime.impl ),
			mouse( window.mouse.impl ),
			keyboard( window.keyboard.impl )
		{
		}

	/*
	 @	析构
	 */
	Win_Window::~Win_Window() noexcept
		{
			this->Destroy();
		}


	/*
	 @	创建 窗口
	 */
	BOOL Win_Window::Create( const Config& cfg ) noexcept
		{
			// 窗口风格
			DWORD style
				{
					WS_VISIBLE | WS_OVERLAPPEDWINDOW
				};

			DWORD styleEx{ 0 };

			// 窗口尺寸
			RECT rcWnd
				{
					.left	= ( GetSystemMetrics( SM_CXSCREEN ) - cfg.size.x ) >> 1,
					.top	= ( GetSystemMetrics( SM_CYSCREEN ) - cfg.size.y ) >> 1,
					.right	= rcWnd.left + cfg.size.x,
					.bottom	= rcWnd.top + cfg.size.y
				};

			// 创建窗口
			if( this->hWnd = CreateWindowEx( styleEx,
											 FCE_NAME,
											 FCE_NAME, style,
											 rcWnd.left, rcWnd.top,
											 rcWnd.right - rcWnd.left,
											 rcWnd.bottom - rcWnd.top,
											 NULL, NULL, s_Win_App, this ) )
				{
					// 显示窗口
					return ShowWindow( this->hWnd, SW_SHOWNORMAL );
				}

			return FALSE;
		}


	/*
	 @	销毁 窗口
	 */
	VOID Win_Window::Destroy() noexcept
		{
			// 销毁窗口
			if( this->hWnd )
				{
					DestroyWindow( this->hWnd );

					this->hWnd = NULL;
				}
		}
}



/**
 !	Get bounds
 */
namespace fce::inner::win
{
	/*
	 @	Size
	 */
	FC_INLINE SIZE Win_Window::GetSize() const noexcept
		{
			const RECT rect
				{
					this->GetBounds()
				};

			return SIZE{ rect.right - rect.left,
						 rect.bottom - rect.top };
		}

	/*
	 @	Position
	 */
	FC_INLINE POINT Win_Window::GetPosition() const noexcept
		{
			const RECT rect
				{
					this->GetBounds()
				};

			return reinterpret_cast< const POINT& >( rect );
		}

	/*
	 @	Bounds
	 */
	FC_INLINE RECT Win_Window::GetBounds() const noexcept
		{
			RECT rect;

			#if WINVER >= _WIN32_WINNT_VISTA
				{
					if( DwmGetWindowAttribute( this->hWnd,
											   DWMWA_EXTENDED_FRAME_BOUNDS,
											   &rect, sizeof( RECT ) ) == S_OK )
						{
							return rect;
						}
				}
			#else
				{
					if( GetWindowRect( this->hWnd, &rect ) )
						{
							return rect;
						}
				}
			#endif

			return { 0 };
		}
}



/**
 !	Get client bounds
 */
namespace fce::inner::win
{
	/*
	 @	Client size
	 */
	SIZE Win_Window::GetClientSize() const noexcept
		{
			RECT rect;

			if( GetClientRect( this->hWnd, &rect ) )
				{
					return SIZE{ rect.right - rect.left,
								 rect.bottom - rect.top };
				}

			return { 0 };
		}

	/*
	 @	Client position
	 */
	POINT Win_Window::GetClientPosition() const noexcept
		{
			const RECT rect
				{
					this->GetClientBounds()
				};

			return reinterpret_cast< const POINT& >( rect );
		}

	/*
	 @	Client bounds
	 */
	RECT Win_Window::GetClientBounds() const noexcept
		{
			// 获取 客户区 相对窗口的 rect
			RECT rcClient;

			if( GetClientRect( this->hWnd,
							   &rcClient ) )
				{
					// 转换 客户区( 左上角 & 右下角 ) 到屏幕坐标
					if( ClientToScreen( this->hWnd,
										LPPOINT( &rcClient ) )

					 && ClientToScreen( this->hWnd,
										LPPOINT( &rcClient.right ) ) )
						{
							return rcClient;
						}
				}

			// 失败 返回 空矩形
			return { 0 };
		}
}



/**
 !	Set window bounds
 */
namespace fce::inner::win
{
	/*
	 @	Size
	 */
	FC_INLINE BOOL Win_Window::SetSize( const Size& size ) noexcept
		{
			return this->SetBounds( 0, size,
									SWP_NOZORDER | SWP_NOMOVE );
		}

	/*
	 @	Position
	 */
	FC_INLINE BOOL Win_Window::SetPosition( const Point& pt ) noexcept
		{
			return this->SetBounds( pt, 0,
									SWP_NOZORDER | SWP_NOSIZE );
		}

	/*
	 @	Rect
	 */
	FC_INLINE BOOL Win_Window::SetBounds( const Rect& rc ) noexcept
		{
			return this->SetBounds( rc.min,
									rc.Size() );
		}

	/*
	 @	Bounds
	 */
	FC_INLINE BOOL Win_Window::SetBounds( const Point& pt,
										  const Size& size,
										  const UINT uFlags ) noexcept
		{
			return SetWindowPos( this->hWnd, NULL,
								 pt.x,
								 pt.y,
								 size.x,
								 size.y,
								 uFlags );
		}
}



/**
 !	Set client bounds
 */
namespace fce::inner::win
{
	/*
	 @	Client size
	 */
	BOOL Win_Window::SetClientSize( const Size& size ) noexcept
		{
			RECT rect{ 0, 0, size.x, size.y };

			return this->SetClientBounds( rect,
										  SWP_NOZORDER | SWP_NOMOVE  );
		}

	/*
	 @	Client position
	 */
	BOOL Win_Window::SetClientPosition( const Point& pt ) noexcept
		{
			RECT rect{ pt.x, pt.y, 0, 0 };

			return this->SetClientBounds( rect,
										  SWP_NOZORDER | SWP_NOSIZE );
		}

	/*
	 @	Client rect
	 */
	BOOL Win_Window::SetClientBounds( const Rect& rc ) noexcept
		{
			RECT rect( rc );

			return this->SetClientBounds( rect );
		}

	/*
	 @	Client bounds
	 */
	BOOL Win_Window::SetClientBounds( const Point& pt,
									  const Size& size ) noexcept
		{
			RECT rect{ pt.x, pt.y,
					   pt.x + size.x,
					   pt.y + size.y };

			return this->SetClientBounds( rect );
		}
}



/**
 !	Common
 */
namespace fce::inner::win
{
	/*
	 @	获取 标题
	 */
	int Win_Window::GetTitle( String& str ) const noexcept
		{
			// 获取可能的 字符串长度，需要加上 \'0'
			int len
				{
					GetWindowTextLengthW( this->hWnd )
					+ 1
				};

			// 分配 wstring 并获取 字符串
			stl::wstring wstr( len, 0 );

			len = GetWindowTextW( this->hWnd,
								  wstr.data(), len );

			// 按 实际长度 保存到 String
			str.Assign( wstr.c_str(), len );

			return len;
		}


	/*
	 @	设置 客户区 范围框
	 */
	BOOL Win_Window::SetClientBounds( RECT& rect,
									  const UINT uFlags ) noexcept
		{
			// 按 客户区 rect 计算窗口 rect
			if( AdjustWindowRectEx( &rect,
									GetWindowStyle( this->hWnd ),
									GetMenu( this->hWnd ) != NULL,
									GetWindowExStyle( this->hWnd ) ) )
				{
					// 用 客户区 rect 要求的大小 重置窗口 rect
					return SetWindowPos( this->hWnd, NULL,
										 rect.left, rect.top,
										 rect.right - rect.left,
										 rect.bottom - rect.top,
										 uFlags );
				}

			return FALSE;
		}


	/*
	 @	设置 窗口显示模式
	 */
	BOOL Win_Window::SetMode( const MODE mode ) noexcept
		{
			switch( mode )
				{
					case MODE::HIDE:
						{
							return ShowWindow( this->hWnd, SW_HIDE );
						}

					case MODE::MAXIMIZE:
						{
							return ShowWindow( this->hWnd, SW_MAXIMIZE );
						}

					case MODE::MINIMIZE:
						{
							return ShowWindow( this->hWnd, SW_MINIMIZE );
						}

					case MODE::RESTORE:
						{
							return ShowWindow( this->hWnd, SW_RESTORE );
						}

					case MODE::CONFIGURE:
						{
						}
						break;

					case MODE::NORMAL:
						{
						}
						break;

					case MODE::EXCLUSIVE:
						{
						}
						break;
				}

			return FALSE;
		}
}



/**
 !	提示框
 */
namespace fce::inner::win
{
	/*
	 @	系统 提示框
	 */
	Bool Win_Window::ErrorBox( const stl::wstring& str ) const noexcept
		{
			return IDOK == MessageBoxW( NULL,
										str.c_str(),
										L"fce debug error box",
										MB_TOPMOST | MB_OKCANCEL | MB_ICONERROR );
		}


	/*
	 @	系统 提示框
	 */
	VOID Win_Window::PromptBox( const stl::wstring& str ) const noexcept
		{
			MessageBoxW( NULL,
						 str.c_str(),
						 L"fce prompt box",
						 MB_TOPMOST | MB_OK | MB_ICONEXCLAMATION );
		}


	/*
	 @	系统 确认框
	 */
	Bool Win_Window::ConfirmBox( const stl::wstring& str ) const noexcept
		{
			return IDOK == MessageBoxW( NULL,
										str.c_str(),
										L"fce confirm box",
										MB_TOPMOST | MB_OKCANCEL | MB_ICONQUESTION );
		}
}



/**
 !	处理 事件消息
 */
namespace fce::inner::win
{
	/*
	 @	响应 WM_CREATE
	 */
	FC_INLINE VOID Win_Window::on_WM_CREATE( const WPARAM wParam,
											 const LPARAM lParam ) noexcept
		{
			/* 获取 创建结构
			CREATESTRUCT* createStruct
				{
					reinterpret_cast< CREATESTRUCT* >( lParam )
				};

			// 保存 窗口坐标
			this->superior.pos =
				{
					static_cast< Int16 >( createStruct->x ),
					static_cast< Int16 >( createStruct->y )
				};

			// 保存 窗口尺寸
			this->superior.size =
				{
					static_cast< Int16 >( createStruct->cx ),
					static_cast< Int16 >( createStruct->cy )
				};
			*/

			// 开启 剪贴板监听器
			//AddClipboardFormatListener( this->hWnd );
		}


	/*
	 @	响应 WM_MOVE
	 */
	FC_INLINE VOID Win_Window::on_WM_MOVE( const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			/* 保存 客户区坐标
			this->superior.clientPos =
				{
					static_cast< Int16 >( GET_X_LPARAM( lParam ) ),
					static_cast< Int16 >( GET_Y_LPARAM( lParam ) )
				};

			// 获取 窗口坐标
			RECT rc;
			GetWindowRect( this->hWnd, &rc );

			this->superior.pos =
				{
					static_cast< Int16 >( rc.left ),
					static_cast< Int16 >( rc.top )
				};
			*/
		}


	/*
	 @	响应 WM_SIZE
	 */
	FC_INLINE VOID Win_Window::on_WM_SIZE( const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			/* 保存 客户区尺寸
			this->superior.clientSize =
				{
					static_cast< Int16 >( LOWORD( lParam ) ),
					static_cast< Int16 >( HIWORD( lParam ) )
				};

			// 获取 窗口尺寸
			RECT rc;
			GetWindowRect( this->hWnd, &rc );

			this->superior.size =
				{
					static_cast< Int16 >( rc.right - rc.left ),
					static_cast< Int16 >( rc.bottom - rc.top )
				};
			*/
		}


	/*
	 @	响应 WM_ACTIVATE
	 */
	FC_INLINE VOID Win_Window::on_WM_ACTIVATE( const WPARAM wParam,
											   const LPARAM lParam ) noexcept
		{
			//this->superior.focused = GET_WM_ACTIVATE_STATE( wParam, lParam );
		}


	/*
	 @	响应 WM_DROPFILES
	 */
	FC_INLINE VOID Win_Window::on_WM_DROPFILES( const WPARAM wParam,
												const LPARAM lParam ) noexcept
		{
			// 获取句柄
			HDROP hDrop{ reinterpret_cast< HDROP >( wParam ) };

			// 获取位置
			POINT pt;
			DragQueryPoint( hDrop, &pt );

			this->message.ptFiles = pt;

			// 获取 文件数量
			UINT num{ DragQueryFileW( hDrop, UInt32_MAX, NULL, 0 ) };

			this->message.listFiles.resize( num );

			for( UINT i{ 0 }; i < num; ++i )
				{
					stl::wstring& name{ this->message.listFiles[ i ] };

					// 获取 文件名长度
					UINT len{ DragQueryFileW( hDrop, i, NULL, 0 ) };

					name.resize( len );

					// 获取 文件名，stl::basic_string 不需要 0 结尾
					DragQueryFileW( hDrop, i, name.data(), len + 1 );
				}

			// 释放
			DragFinish( hDrop );
		}


	/*
	 @	窗口消息

			所有未赋值 this->superior.msg.e 都不能 dispatch，否则会 重复处理消息
			养成习惯 不用的要 return
	 */
	FC_INLINE VOID Win_Window::on_WndProc( const UINT uMsg,
										   const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			// 转换 win 消息
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
					// 位置改变（一次调用）
					case WM_MOVE:
						{
							e = EVENT::MOVE;

							this->on_WM_MOVE( wParam, lParam );
						}
						break;

					// 尺寸改变（一次调用）
					case WM_SIZE:
						{
							e = EVENT::SIZE;

							this->on_WM_SIZE( wParam, lParam );
						}
						break;

					// 位置拖动（持续调用）
					case WM_MOVING:
						{
							e = EVENT::MOVING;

							//RECT* rc = ( RECT* )lParam;
						}
						break;

					// 尺寸拖动（持续调用）
					case WM_SIZING:
						{
							e = EVENT::SIZING;

							//RECT* rc = ( RECT* )lParam;
						}
						break;

					// 激活事件
					case WM_ACTIVATE:
						{
							e = EVENT::FOCUS;

							this->on_WM_ACTIVATE( wParam, lParam );
						}
						break;

					// 获得焦点
					case WM_SETFOCUS:
						{
							// 所有未赋值的消息 都不能 dispatch，否则会 重复处理消息
						}
						return;

					// 失去焦点
					case WM_KILLFOCUS:
						{
						// 所有未赋值的消息 都不能 dispatch，否则会 重复处理消息
						}
						return;

					// 光标进入
					case WM_SETCURSOR:
						{
						// 所有未赋值的消息 都不能 dispatch，否则会 重复处理消息
						}
						return;

					// 拖入文件
					case WM_DROPFILES:
						{
							e = EVENT::FILE;

							this->on_WM_DROPFILES( wParam, lParam );
						}
						break;

					// 重绘 消息
					case WM_PAINT:
						{
							e = EVENT::PAINT;
						}
						break;

					// 未处理的消息 都需要 return，否则会造成 EVENT 重复 Dispatch
					default:
						{
							return;
						}
				}

			// 分发消息给 引擎
			this->message.Dispatch();
		}


	/*
	 @	分发 窗口消息
	 */
	BOOL Win_Window::Dispatch( const UINT uMsg,
							   const WPARAM wParam,
							   const LPARAM lParam ) noexcept
		{
			// 检索 鼠标消息
			if( uMsg >= WM_MOUSEFIRST
			 && uMsg <= WM_MOUSELAST )
				{
					return this->mouse.on_WndProc( uMsg, wParam, lParam );
				}
			// 检索 鼠标消息 （非客户区）
			else if( uMsg >= WM_NCMOUSEMOVE
				  && uMsg <= WM_NCXBUTTONDBLCLK )
				{
					return this->mouse.handler
						&& this->mouse.on_NcWndProc( uMsg, wParam, lParam );
				}
			// 检索 键盘消息
			else if( uMsg >= WM_KEYFIRST
				  && uMsg <= WM_KEYLAST
				  && uMsg != WM_CHAR )
				{
					return this->keyboard.on_WndProc( uMsg, wParam, lParam );
				}
			// 其它 消息
			else
				{
					switch( uMsg )
						{
							/*	Ime 消息
							 */
							case WM_IME_STARTCOMPOSITION:
							case WM_IME_ENDCOMPOSITION:
							case WM_IME_COMPOSITION:

							case WM_IME_CHAR:
							case WM_CHAR:
								{
									return this->ime.on_WndProc( uMsg, wParam, lParam );
								}

							/*	剪切板 消息
							 */
							[[unlikely]]

							case WM_CLIPBOARDUPDATE:
							case WM_DESTROYCLIPBOARD:
								{
									s_Win_Clipboard.on_WndProc( uMsg, wParam, lParam );
								}
								break;

							// 不清空背景
							case WM_ERASEBKGND:
								{
									return true;
								}

							// 显示配置改变
							case WM_DISPLAYCHANGE:
								{
								}
								break;

							// 窗口 创建
							case WM_CREATE:
								{
									this->on_WM_CREATE( wParam, lParam );
								}
								break;

							// 窗口 关闭
							[[unlikely]]

							case WM_CLOSE:
								{
									// 关闭 主窗口 需要先确认
									if( s_Win_Window == hWnd )
										{
											s_App.Quit( false );

											return true;
										}
									// 子窗口 需要额外处理
									else
										{
											return true;
										}
								}
								break;

							/*	其它 Window 消息
							 */
							default:
								{
									this->on_WndProc( uMsg, wParam, lParam );
								}
						}
				}

			// 消息未截断 系统默认处理
			return FALSE;
		}
}