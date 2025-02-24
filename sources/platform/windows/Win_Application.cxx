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
	 @	����
	 */
	FC_INLINE Win_Application::Win_Application() noexcept
		:
			handler( NULL ),
			hInstance( NULL )
		{
		}

	/*
	 @	����
	 */
	FC_INLINE Win_Application::~Win_Application() noexcept
		{
			// ע�� ������
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
	 @	��Ļ��ͼ

			��һ��������DX�� Magnification
			�� desktop duplication
	 */
	BOOL Win_Application::Screenshot( Image& img,
									  const int x,
									  const int y,
									  const int width,
									  const int height ) const noexcept
		{
			BOOL res{ FALSE };

			// ��ȡ ��Ļ hdc
			if( HDC hdcScreen{ GetDC( NULL ) } )
				{
					// ���� ��ͼ hdc
					if( HDC hdc{ CreateCompatibleDC( hdcScreen ) } )
						{
							// ���� ��ͼ HBITMAP
							if( HBITMAP hBitmap{ CreateCompatibleBitmap( hdcScreen,
																		 width, height ) } )
								{
									// ѡȡ ��ͼ
									SelectBitmap( hdc, hBitmap );

									// ���� ��Ļ��ͼ
									if( BitBlt( hdc, 0, 0, width, height,
												hdcScreen, x, y, SRCCOPY ) )
										{
											res = image::Bitmap::Convert( hdc,
																		  hBitmap, img );
										}

									// ���� ��ͼ
									DeleteBitmap( hBitmap );
								}

							// ���� ��ͼ hdc
							DeleteDC( hdc );
						}

					// �ͷ� ��Ļ hdc
					ReleaseDC( NULL, hdcScreen );
				}

			return res;
		}


	/*
	 @	ִ�� ϵͳ����
	 */
	FC_INLINE DWORD Win_Application::Execute( const stl::wstring& cmdLine,
													stl::string& feedback ) noexcept
		{
			DWORD exitCode{ DWORD( - 1 ) };

			HANDLE hRead{ NULL },
				   hWrite{ NULL };

			// ���� security attributes
			SECURITY_ATTRIBUTES sa
				{
					.nLength				= sizeof( SECURITY_ATTRIBUTES ),
					.lpSecurityDescriptor	= NULL,
					.bInheritHandle			= TRUE
				};

			// ���� pipe
			if( CreatePipe( &hRead, &hWrite, &sa, 0 ) )
				{
					PROCESS_INFORMATION pi;

					// ���� startup info
					STARTUPINFOW si;

					GetStartupInfoW( &si );
						{
							// �� std error & output
							si.hStdError = hWrite;
							si.hStdOutput = hWrite;

							// ���� std io
							si.dwFlags = STARTF_USESTDHANDLES;
						}

					// �������� ִ�� cmd
					if( CreateProcessW( NULL, LPWSTR( cmdLine.c_str() ),
										NULL, NULL, TRUE,
										CREATE_NO_WINDOW,
										NULL, NULL, &si, &pi ) )
						{
							// �ȴ����� ��� cmd
							if( WaitForSingleObject( pi.hProcess, INFINITE ) == WAIT_OBJECT_0 )
								{
									// ��ȡ exit code
									if( GetExitCodeProcess( pi.hProcess, &exitCode ) )
										{
											// �ر� thread & process
											CloseHandle( pi.hThread );
											CloseHandle( pi.hProcess );

											// �ر� pipe write
											CloseHandle( hWrite );

											// ѭ����ȡ �ܵ���Ϣ
											char buf[ 1024 ];

											DWORD bytesRead;

											while( ReadFile( hRead, buf,
															 sizeof( buf ),
															 &bytesRead, NULL ) ) 
												{
													feedback.append( buf, bytesRead );
												}

											// �ر� pipe read
											CloseHandle( hRead );

											// ���� exit code
											return exitCode;
										}
								}

							// ʧ�ܺ� �ر� thread & process
							CloseHandle( pi.hThread );
							CloseHandle( pi.hProcess );
						}

					// ʧ�ܺ� �ر� read & write
					CloseHandle( hRead );
					CloseHandle( hWrite );
				}

			// ʧ�ܷ��� -1
			return exitCode;
		}


	/*
	 @	ע�� ������
	 */
	FC_INLINE ATOM Win_Application::Register( HINSTANCE hInstc ) noexcept
		{
			// ������
			UINT style{ CS_DBLCLKS };

			// ע�� App class������Ҫ�� ̫��һ
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
	 @	����
	 */
	FC_INLINE BOOL Win_Application::Create( HINSTANCE hInstc,
											LPWSTR lpCmdLine,
											int nCmdShow ) NOEXCEPT
		{
			// ע�� ������
			if( Register( hInstc ) )
				{
					// ���� ����ʵ�����
					this->hInstance = hInstc;

					// ��Ӧ s_App ����
					return s_App.on_Create( lpCmdLine );
				}

			return FALSE;
		}


	/*
	 @	����

			1.����ʧ��ʱ����
			2.��Ӧ EVENT::EXIT
	 */
	FC_INLINE VOID Win_Application::Destroy() NOEXCEPT
		{
			// ���� s_App ����
			s_App.on_Destroy();
		}


	/*
	 @	��Ϣѭ��
	 */
	FC_INLINE int Win_Application::Run() NOEXCEPT
		{
			// ����Ϣѭ��:
			MSG msg;

			while( true )
				{
					// ���� ��ҵʱ �쳣
					#ifdef FC_SECURITY
					try	{
					#endif

						// ֻȡ hwnd Ϊ������ ����Ϣ
						if( PeekMessage( &msg, NULL,
										 0, 0, PM_REMOVE ) )
							{
								// ת�����ַ� msg �� WndProc
								TranslateMessage( &msg );
								DispatchMessage( &msg );

								// ��� �˳���־
								if( msg.message == WM_QUIT )
									{
										break;
									}
							}
						else
							{
								// ���� s_App ֡����
								s_App.on_Frame();

								// �ȴ� ����Ϣ
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

			// ���� exit code
			return int( msg.wParam );
		}


	/*
	 @	��� ��Ϣ
	 
			��Ҫ�����ٴ���ǰ ������ Object
			һ���������� ���˳���Ϣѭ�� ����й©
	 */
	FC_INLINE VOID Win_Application::Finish() noexcept
		{
			// ���ȣ��л��� WndProc 2 ��ֹ app ԭ����Ϣ�����߼�
			SetWindowLongPtr( s_Win_Window,
							  GWLP_WNDPROC,
							  LONG_PTR( WndProc2 ) );

			// Ȼ�󣺽�������Ϣ ȫ��ת���� WndProc 2 ���д���
			MSG msg;

			while( PeekMessage( &msg, NULL,
								0, 0, PM_REMOVE ) )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
		}


	/*
	 @	�Ƿ� �Զ�����Ϣ
	 */
	FC_INLINE Bool Win_Application::isCustomMessage( const UINT uMsg ) noexcept
		{
			return uMsg >= WM_APP && uMsg <= 0xBFFF;
		}


	/*
	 @	���� ���ڵ���Ϣ

			hWnd		�����ھ��
			uMsg		��Ϣ�ṹ
			wParam		��Ϣ��ʶ
			lParam		��Ϣ����
	 */
	LRESULT CALLBACK Win_Application::WndProc( HWND hWnd,
											   const UINT uMsg,
											   const WPARAM wParam,
											   const LPARAM lParam ) noexcept
		{
			// �Զ��� ��Ϣ
			if( isCustomMessage( uMsg ) )
				{
					s_App.Dispatch( Application::EVENT( uMsg ),
									reinterpret_cast< Object* >( lParam ) );

					return 0;
				}
			// ������ ��Ϣ
			else if( s_Win_Window == hWnd )
				{
					if( s_Win_Window.Dispatch( uMsg, wParam, lParam ) )
						{
							return 0;
						}
				}
			// �Ӵ��� ��Ϣ
			else if( Window* child = s_Window.FindChild( hWnd ) )
				{
					if( child->impl.Dispatch( uMsg, wParam, lParam ) )
						{
							return 0;
						}
				}
			// ���ڴ��� ��Ϣ
			else if( uMsg == WM_CREATE )
				{
					// ��ʱ������ ���ü��������
					AddClipboardFormatListener( hWnd );
				}
			// ���� ��Ϣ
			else if( s_Win_App.handler )
				{
					return s_Win_App.handler( hWnd, uMsg,
											  wParam, lParam );
				}

			// ϵͳ Ĭ��
			return DefWindowProc( hWnd, uMsg,
								  wParam, lParam );
		}


	/*
	 @	ִ���˳���, �����ƺ�� WndProc2
	 */
	LRESULT CALLBACK Win_Application::WndProc2( HWND hWnd,
												const UINT uMsg,
												const WPARAM wParam,
												const LPARAM lParam ) noexcept
		{
			// ���� �Զ�����Ϣ
			if( isCustomMessage( uMsg ) )
				{
					// ���� object
					if( Object* const obj =
							reinterpret_cast< Object* >( lParam ) )
						{
							MemoryManagement::Destruct( obj );
						}
				}
			// �˳���Ϣѭ��
			else if( uMsg == WM_DESTROY )
				{
					PostQuitMessage( 0 );
				}

			// ϵͳĬ�ϴ���
			return DefWindowProc( hWnd, uMsg, wParam, lParam );
		}


	/*
	 @	fc ������

			hInstc		����ʵ�����
			lpCmdLine	�����в���
			nCmdShow	���ڵĳ�ʼ��ʾ��ʽ
	 */
	int Win_Application::Main( HINSTANCE hInstc,
							   LPWSTR lpCmdLine,
							   int nCmdShow ) noexcept
		{
			// �����쳣����Ҫ�� ���� �� ���� �׶�
			#ifdef FC_SECURITY
			try	{
			#endif

				// ���� win app
				if( s_Win_App.Create( hInstc,
									  lpCmdLine,
									  nCmdShow ) )
					{
						// ���� ����Ϣѭ��
						return s_Win_App.Run();
					}
				else
					{
						// ����ʧ�� ����
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