/*
**
**	Win_Ime.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

#pragma comment( lib, "Imm32.lib" )

namespace fce::inner::win
{
	/*
	 @	����
	 */
	FC_INLINE Win_Ime::Win_Ime( Ime& ime ) noexcept
		:
			message( ime ),
			win_Window( ime.window.impl )
		{
		}


	/*
	 @	���� ime ����λ��
	 */
	BOOL Win_Ime::SetPosition( const LONG x,
							   const LONG y ) noexcept
		{
			BOOL res{ FALSE };

			// ��ȡ ime ������
			if( HIMC hIMC{ ImmGetContext( this->win_Window ) } )
				{
					// ��������
					COMPOSITIONFORM cf
						{
							.dwStyle	  { CFS_POINT },
							.ptCurrentPos { x, y },
							.rcArea		  { 0, 0, 0, 0 }
						};

					res = ImmSetCompositionWindow( hIMC, &cf );

					// �ͷ� ime ������
					ImmReleaseContext( this->win_Window, hIMC );

					return res;
				}

			return res;
		}
}



/**
 !	���� �¼���Ϣ
 */
namespace fce::inner::win
{
	/*
	 @	������ַ���
	 */
	FC_INLINE VOID Win_Ime::InputString() noexcept
		{
			// ��ȡ ime ������
			if( HIMC hIMC{ ImmGetContext( this->win_Window ) } )
				{
					// ��ȡ �ַ����ߴ� bytes
					const DWORD size
						{
							( DWORD )ImmGetCompositionStringW( hIMC,
															   GCS_RESULTSTR,
															   NULL, 0 )
						};

					// ���� �ַ������� wchar len
					this->wstr.resize( size >> 1 );

					// ��ȡ�ַ���
					ImmGetCompositionStringW( hIMC, GCS_RESULTSTR,
											  wstr.data(), size );

					// �ͷ� ime ������
					ImmReleaseContext( this->win_Window, hIMC );

					// ����
					this->message.e = EVENT::TEXT;

					this->message.text = wstr;
				}
		}


	/*
	 @	��Ӧ WM_CHAR
	 */
	FC_INLINE Bool Win_Ime::on_WM_CHAR( const WPARAM wParam ) noexcept
		{
			// �޳� �����ַ�
			if( wParam > 0x1F )
				{
					this->message.e = EVENT::CHARACTER;

					this->message.character = Char32( wParam );

					return false;
				}

			return true;
		}


	/*
	 @	ime ��Ϣ
	 */
	Bool Win_Ime::on_WndProc( const UINT uMsg,
							  const WPARAM wParam,
							  const LPARAM lParam ) noexcept
		{
			// ת�� win ��Ϣ
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
					// ����true������Ĭ�� �����
					case WM_IME_STARTCOMPOSITION:
						{
							e = EVENT::START;
						}
						break;

					// ����ַ��� �������
					case WM_IME_ENDCOMPOSITION:
						{
							e = EVENT::END;
						}
						break;

					// ����
					case WM_IME_CHAR:
						return true;

					// ���� ASCII ��
					case WM_CHAR:
						{
							if( this->on_WM_CHAR( wParam ) )
								{
									return false;
								}
						}
						break;

					// ����ַ��� ����
					case WM_IME_COMPOSITION:
						{
							if( lParam & GCS_RESULTSTR ) 
								{
									this->InputString();
								}
							else
								{
									return false;
								}
						}
						break;

					// δ������Ϣ ֱ�ӷ��� false
					default:
						{
							return false;
						}
				}

			// �ַ���Ϣ�� ����
			return this->message.Dispatch();
		}
}