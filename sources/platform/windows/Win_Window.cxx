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
	 @	����
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
	 @	����
	 */
	Win_Window::~Win_Window() noexcept
		{
			this->Destroy();
		}


	/*
	 @	���� ����
	 */
	BOOL Win_Window::Create( const Config& cfg ) noexcept
		{
			// ���ڷ��
			DWORD style
				{
					WS_VISIBLE | WS_OVERLAPPEDWINDOW
				};

			DWORD styleEx{ 0 };

			// ���ڳߴ�
			RECT rcWnd
				{
					.left	= ( GetSystemMetrics( SM_CXSCREEN ) - cfg.size.x ) >> 1,
					.top	= ( GetSystemMetrics( SM_CYSCREEN ) - cfg.size.y ) >> 1,
					.right	= rcWnd.left + cfg.size.x,
					.bottom	= rcWnd.top + cfg.size.y
				};

			// ��������
			if( this->hWnd = CreateWindowEx( styleEx,
											 FCE_NAME,
											 FCE_NAME, style,
											 rcWnd.left, rcWnd.top,
											 rcWnd.right - rcWnd.left,
											 rcWnd.bottom - rcWnd.top,
											 NULL, NULL, s_Win_App, this ) )
				{
					// ��ʾ����
					return ShowWindow( this->hWnd, SW_SHOWNORMAL );
				}

			return FALSE;
		}


	/*
	 @	���� ����
	 */
	VOID Win_Window::Destroy() noexcept
		{
			// ���ٴ���
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
			// ��ȡ �ͻ��� ��Դ��ڵ� rect
			RECT rcClient;

			if( GetClientRect( this->hWnd,
							   &rcClient ) )
				{
					// ת�� �ͻ���( ���Ͻ� & ���½� ) ����Ļ����
					if( ClientToScreen( this->hWnd,
										LPPOINT( &rcClient ) )

					 && ClientToScreen( this->hWnd,
										LPPOINT( &rcClient.right ) ) )
						{
							return rcClient;
						}
				}

			// ʧ�� ���� �վ���
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
	 @	��ȡ ����
	 */
	int Win_Window::GetTitle( String& str ) const noexcept
		{
			// ��ȡ���ܵ� �ַ������ȣ���Ҫ���� \'0'
			int len
				{
					GetWindowTextLengthW( this->hWnd )
					+ 1
				};

			// ���� wstring ����ȡ �ַ���
			stl::wstring wstr( len, 0 );

			len = GetWindowTextW( this->hWnd,
								  wstr.data(), len );

			// �� ʵ�ʳ��� ���浽 String
			str.Assign( wstr.c_str(), len );

			return len;
		}


	/*
	 @	���� �ͻ��� ��Χ��
	 */
	BOOL Win_Window::SetClientBounds( RECT& rect,
									  const UINT uFlags ) noexcept
		{
			// �� �ͻ��� rect ���㴰�� rect
			if( AdjustWindowRectEx( &rect,
									GetWindowStyle( this->hWnd ),
									GetMenu( this->hWnd ) != NULL,
									GetWindowExStyle( this->hWnd ) ) )
				{
					// �� �ͻ��� rect Ҫ��Ĵ�С ���ô��� rect
					return SetWindowPos( this->hWnd, NULL,
										 rect.left, rect.top,
										 rect.right - rect.left,
										 rect.bottom - rect.top,
										 uFlags );
				}

			return FALSE;
		}


	/*
	 @	���� ������ʾģʽ
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
 !	��ʾ��
 */
namespace fce::inner::win
{
	/*
	 @	ϵͳ ��ʾ��
	 */
	Bool Win_Window::ErrorBox( const stl::wstring& str ) const noexcept
		{
			return IDOK == MessageBoxW( NULL,
										str.c_str(),
										L"fce debug error box",
										MB_TOPMOST | MB_OKCANCEL | MB_ICONERROR );
		}


	/*
	 @	ϵͳ ��ʾ��
	 */
	VOID Win_Window::PromptBox( const stl::wstring& str ) const noexcept
		{
			MessageBoxW( NULL,
						 str.c_str(),
						 L"fce prompt box",
						 MB_TOPMOST | MB_OK | MB_ICONEXCLAMATION );
		}


	/*
	 @	ϵͳ ȷ�Ͽ�
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
 !	���� �¼���Ϣ
 */
namespace fce::inner::win
{
	/*
	 @	��Ӧ WM_CREATE
	 */
	FC_INLINE VOID Win_Window::on_WM_CREATE( const WPARAM wParam,
											 const LPARAM lParam ) noexcept
		{
			/* ��ȡ �����ṹ
			CREATESTRUCT* createStruct
				{
					reinterpret_cast< CREATESTRUCT* >( lParam )
				};

			// ���� ��������
			this->superior.pos =
				{
					static_cast< Int16 >( createStruct->x ),
					static_cast< Int16 >( createStruct->y )
				};

			// ���� ���ڳߴ�
			this->superior.size =
				{
					static_cast< Int16 >( createStruct->cx ),
					static_cast< Int16 >( createStruct->cy )
				};
			*/

			// ���� �����������
			//AddClipboardFormatListener( this->hWnd );
		}


	/*
	 @	��Ӧ WM_MOVE
	 */
	FC_INLINE VOID Win_Window::on_WM_MOVE( const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			/* ���� �ͻ�������
			this->superior.clientPos =
				{
					static_cast< Int16 >( GET_X_LPARAM( lParam ) ),
					static_cast< Int16 >( GET_Y_LPARAM( lParam ) )
				};

			// ��ȡ ��������
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
	 @	��Ӧ WM_SIZE
	 */
	FC_INLINE VOID Win_Window::on_WM_SIZE( const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			/* ���� �ͻ����ߴ�
			this->superior.clientSize =
				{
					static_cast< Int16 >( LOWORD( lParam ) ),
					static_cast< Int16 >( HIWORD( lParam ) )
				};

			// ��ȡ ���ڳߴ�
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
	 @	��Ӧ WM_ACTIVATE
	 */
	FC_INLINE VOID Win_Window::on_WM_ACTIVATE( const WPARAM wParam,
											   const LPARAM lParam ) noexcept
		{
			//this->superior.focused = GET_WM_ACTIVATE_STATE( wParam, lParam );
		}


	/*
	 @	��Ӧ WM_DROPFILES
	 */
	FC_INLINE VOID Win_Window::on_WM_DROPFILES( const WPARAM wParam,
												const LPARAM lParam ) noexcept
		{
			// ��ȡ���
			HDROP hDrop{ reinterpret_cast< HDROP >( wParam ) };

			// ��ȡλ��
			POINT pt;
			DragQueryPoint( hDrop, &pt );

			this->message.ptFiles = pt;

			// ��ȡ �ļ�����
			UINT num{ DragQueryFileW( hDrop, UInt32_MAX, NULL, 0 ) };

			this->message.listFiles.resize( num );

			for( UINT i{ 0 }; i < num; ++i )
				{
					stl::wstring& name{ this->message.listFiles[ i ] };

					// ��ȡ �ļ�������
					UINT len{ DragQueryFileW( hDrop, i, NULL, 0 ) };

					name.resize( len );

					// ��ȡ �ļ�����stl::basic_string ����Ҫ 0 ��β
					DragQueryFileW( hDrop, i, name.data(), len + 1 );
				}

			// �ͷ�
			DragFinish( hDrop );
		}


	/*
	 @	������Ϣ

			����δ��ֵ this->superior.msg.e ������ dispatch������� �ظ�������Ϣ
			����ϰ�� ���õ�Ҫ return
	 */
	FC_INLINE VOID Win_Window::on_WndProc( const UINT uMsg,
										   const WPARAM wParam,
										   const LPARAM lParam ) noexcept
		{
			// ת�� win ��Ϣ
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
					// λ�øı䣨һ�ε��ã�
					case WM_MOVE:
						{
							e = EVENT::MOVE;

							this->on_WM_MOVE( wParam, lParam );
						}
						break;

					// �ߴ�ı䣨һ�ε��ã�
					case WM_SIZE:
						{
							e = EVENT::SIZE;

							this->on_WM_SIZE( wParam, lParam );
						}
						break;

					// λ���϶����������ã�
					case WM_MOVING:
						{
							e = EVENT::MOVING;

							//RECT* rc = ( RECT* )lParam;
						}
						break;

					// �ߴ��϶����������ã�
					case WM_SIZING:
						{
							e = EVENT::SIZING;

							//RECT* rc = ( RECT* )lParam;
						}
						break;

					// �����¼�
					case WM_ACTIVATE:
						{
							e = EVENT::FOCUS;

							this->on_WM_ACTIVATE( wParam, lParam );
						}
						break;

					// ��ý���
					case WM_SETFOCUS:
						{
							// ����δ��ֵ����Ϣ ������ dispatch������� �ظ�������Ϣ
						}
						return;

					// ʧȥ����
					case WM_KILLFOCUS:
						{
						// ����δ��ֵ����Ϣ ������ dispatch������� �ظ�������Ϣ
						}
						return;

					// ������
					case WM_SETCURSOR:
						{
						// ����δ��ֵ����Ϣ ������ dispatch������� �ظ�������Ϣ
						}
						return;

					// �����ļ�
					case WM_DROPFILES:
						{
							e = EVENT::FILE;

							this->on_WM_DROPFILES( wParam, lParam );
						}
						break;

					// �ػ� ��Ϣ
					case WM_PAINT:
						{
							e = EVENT::PAINT;
						}
						break;

					// δ�������Ϣ ����Ҫ return���������� EVENT �ظ� Dispatch
					default:
						{
							return;
						}
				}

			// �ַ���Ϣ�� ����
			this->message.Dispatch();
		}


	/*
	 @	�ַ� ������Ϣ
	 */
	BOOL Win_Window::Dispatch( const UINT uMsg,
							   const WPARAM wParam,
							   const LPARAM lParam ) noexcept
		{
			// ���� �����Ϣ
			if( uMsg >= WM_MOUSEFIRST
			 && uMsg <= WM_MOUSELAST )
				{
					return this->mouse.on_WndProc( uMsg, wParam, lParam );
				}
			// ���� �����Ϣ ���ǿͻ�����
			else if( uMsg >= WM_NCMOUSEMOVE
				  && uMsg <= WM_NCXBUTTONDBLCLK )
				{
					return this->mouse.handler
						&& this->mouse.on_NcWndProc( uMsg, wParam, lParam );
				}
			// ���� ������Ϣ
			else if( uMsg >= WM_KEYFIRST
				  && uMsg <= WM_KEYLAST
				  && uMsg != WM_CHAR )
				{
					return this->keyboard.on_WndProc( uMsg, wParam, lParam );
				}
			// ���� ��Ϣ
			else
				{
					switch( uMsg )
						{
							/*	Ime ��Ϣ
							 */
							case WM_IME_STARTCOMPOSITION:
							case WM_IME_ENDCOMPOSITION:
							case WM_IME_COMPOSITION:

							case WM_IME_CHAR:
							case WM_CHAR:
								{
									return this->ime.on_WndProc( uMsg, wParam, lParam );
								}

							/*	���а� ��Ϣ
							 */
							[[unlikely]]

							case WM_CLIPBOARDUPDATE:
							case WM_DESTROYCLIPBOARD:
								{
									s_Win_Clipboard.on_WndProc( uMsg, wParam, lParam );
								}
								break;

							// ����ձ���
							case WM_ERASEBKGND:
								{
									return true;
								}

							// ��ʾ���øı�
							case WM_DISPLAYCHANGE:
								{
								}
								break;

							// ���� ����
							case WM_CREATE:
								{
									this->on_WM_CREATE( wParam, lParam );
								}
								break;

							// ���� �ر�
							[[unlikely]]

							case WM_CLOSE:
								{
									// �ر� ������ ��Ҫ��ȷ��
									if( s_Win_Window == hWnd )
										{
											s_App.Quit( false );

											return true;
										}
									// �Ӵ��� ��Ҫ���⴦��
									else
										{
											return true;
										}
								}
								break;

							/*	���� Window ��Ϣ
							 */
							default:
								{
									this->on_WndProc( uMsg, wParam, lParam );
								}
						}
				}

			// ��Ϣδ�ض� ϵͳĬ�ϴ���
			return FALSE;
		}
}