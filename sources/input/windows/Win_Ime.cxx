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
	 @	构造
	 */
	FC_INLINE Win_Ime::Win_Ime( Ime& ime ) noexcept
		:
			message( ime ),
			win_Window( ime.window.impl )
		{
		}


	/*
	 @	设置 ime 窗口位置
	 */
	BOOL Win_Ime::SetPosition( const LONG x,
							   const LONG y ) noexcept
		{
			BOOL res{ FALSE };

			// 获取 ime 上下文
			if( HIMC hIMC{ ImmGetContext( this->win_Window ) } )
				{
					// 设置坐标
					COMPOSITIONFORM cf
						{
							.dwStyle	  { CFS_POINT },
							.ptCurrentPos { x, y },
							.rcArea		  { 0, 0, 0, 0 }
						};

					res = ImmSetCompositionWindow( hIMC, &cf );

					// 释放 ime 上下文
					ImmReleaseContext( this->win_Window, hIMC );

					return res;
				}

			return res;
		}
}



/**
 !	处理 事件消息
 */
namespace fce::inner::win
{
	/*
	 @	输入的字符串
	 */
	FC_INLINE VOID Win_Ime::InputString() noexcept
		{
			// 获取 ime 上下文
			if( HIMC hIMC{ ImmGetContext( this->win_Window ) } )
				{
					// 获取 字符串尺寸 bytes
					const DWORD size
						{
							( DWORD )ImmGetCompositionStringW( hIMC,
															   GCS_RESULTSTR,
															   NULL, 0 )
						};

					// 设置 字符串长度 wchar len
					this->wstr.resize( size >> 1 );

					// 获取字符串
					ImmGetCompositionStringW( hIMC, GCS_RESULTSTR,
											  wstr.data(), size );

					// 释放 ime 上下文
					ImmReleaseContext( this->win_Window, hIMC );

					// 保存
					this->message.e = EVENT::TEXT;

					this->message.text = wstr;
				}
		}


	/*
	 @	响应 WM_CHAR
	 */
	FC_INLINE Bool Win_Ime::on_WM_CHAR( const WPARAM wParam ) noexcept
		{
			// 剔除 控制字符
			if( wParam > 0x1F )
				{
					this->message.e = EVENT::CHARACTER;

					this->message.character = Char32( wParam );

					return false;
				}

			return true;
		}


	/*
	 @	ime 消息
	 */
	Bool Win_Ime::on_WndProc( const UINT uMsg,
							  const WPARAM wParam,
							  const LPARAM lParam ) noexcept
		{
			// 转换 win 消息
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
					// 返回true则屏蔽默认 输入框
					case WM_IME_STARTCOMPOSITION:
						{
							e = EVENT::START;
						}
						break;

					// 组合字符串 输入结束
					case WM_IME_ENDCOMPOSITION:
						{
							e = EVENT::END;
						}
						break;

					// 屏蔽
					case WM_IME_CHAR:
						return true;

					// 键盘 ASCII 码
					case WM_CHAR:
						{
							if( this->on_WM_CHAR( wParam ) )
								{
									return false;
								}
						}
						break;

					// 组合字符串 输入
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

					// 未处理消息 直接返回 false
					default:
						{
							return false;
						}
				}

			// 分发消息给 引擎
			return this->message.Dispatch();
		}
}