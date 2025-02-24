/*
**
**	Win_Keyboard.
**
		Windows platform implementation

	Copyright (c) WangFeng since 1999...
*/

namespace fce::inner::win
{
	/*
	 @	构造
	 */
	FC_INLINE Win_Keyboard::Win_Keyboard( Keyboard& keyboard ) noexcept
		:
			message( keyboard )
		{
		}


	/*
	 @	Convert VK to KEY
	 */
	FC_INLINE constexpr keyboard::KEY Win_Keyboard::Convert( const int vk ) noexcept
		{
			switch( vk )
				{
					[[likely]]		case '0'			: return KEY::_0;
					[[likely]]		case '1'			: return KEY::_1;
					[[likely]]		case '2'			: return KEY::_2;
					[[likely]]		case '3'			: return KEY::_3;
					[[likely]]		case '4'			: return KEY::_4;
					[[likely]]		case '5'			: return KEY::_5;
					[[likely]]		case '6'			: return KEY::_6;
					[[likely]]		case '7'			: return KEY::_7;
					[[likely]]		case '8'			: return KEY::_8;
					[[likely]]		case '9'			: return KEY::_9;

					[[likely]]		case 'A'			: return KEY::_A;
					[[likely]]		case 'B'			: return KEY::_B;
					[[likely]]		case 'C'			: return KEY::_C;
					[[likely]]		case 'D'			: return KEY::_D;
					[[likely]]		case 'E'			: return KEY::_E;
					[[likely]]		case 'F'			: return KEY::_F;
					[[likely]]		case 'G'			: return KEY::_G;
					[[likely]]		case 'H'			: return KEY::_H;
					[[likely]]		case 'I'			: return KEY::_I;
					[[likely]]		case 'J'			: return KEY::_J;
					[[likely]]		case 'K'			: return KEY::_K;
					[[likely]]		case 'L'			: return KEY::_L;
					[[likely]]		case 'M'			: return KEY::_M;
					[[likely]]		case 'N'			: return KEY::_N;
					[[likely]]		case 'O'			: return KEY::_O;
					[[likely]]		case 'P'			: return KEY::_P;
					[[likely]]		case 'Q'			: return KEY::_Q;
					[[likely]]		case 'R'			: return KEY::_R;
					[[likely]]		case 'S'			: return KEY::_S;
					[[likely]]		case 'T'			: return KEY::_T;
					[[likely]]		case 'U'			: return KEY::_U;
					[[likely]]		case 'V'			: return KEY::_V;
					[[likely]]		case 'W'			: return KEY::_W;
					[[likely]]		case 'X'			: return KEY::_X;
					[[likely]]		case 'Y'			: return KEY::_Y;
					[[likely]]		case 'Z'			: return KEY::_Z;

									case VK_F1			: return KEY::F1;	
									case VK_F2			: return KEY::F2;	
									case VK_F3			: return KEY::F3;	
									case VK_F4			: return KEY::F4;	
									case VK_F5			: return KEY::F5;	
									case VK_F6			: return KEY::F6;	
									case VK_F7			: return KEY::F7;	
									case VK_F8			: return KEY::F8;	
									case VK_F9			: return KEY::F9;	
									case VK_F10			: return KEY::F10;
									case VK_F11			: return KEY::F11;
									case VK_F12			: return KEY::F12;

					[[likely]]		case VK_UP			: return KEY::UP;
					[[likely]]		case VK_DOWN		: return KEY::DOWN;
					[[likely]]		case VK_LEFT		: return KEY::LEFT;
					[[likely]]		case VK_RIGHT		: return KEY::RIGHT;	

									case VK_INSERT		: return KEY::INS;
					[[likely]]		case VK_DELETE		: return KEY::DEL;
									case VK_HOME		: return KEY::HOME;
									case VK_END			: return KEY::END;
					[[likely]]		case VK_PRIOR		: return KEY::PRIOR;
					[[likely]]		case VK_NEXT		: return KEY::NEXT;

					[[likely]]		case VK_ESCAPE		: return KEY::ESC;
									case VK_TAB			: return KEY::TAB;
					[[likely]]		case VK_BACK		: return KEY::BACK;
					[[likely]]		case VK_RETURN		: return KEY::ENTER;
					[[likely]]		case VK_SPACE		: return KEY::SPACE;
					[[unlikely]]	case VK_PAUSE		: return KEY::PAUSE;
									case VK_SNAPSHOT	: return KEY::SNAPSHOT;

					[[unlikely]]	case VK_LMENU		: return KEY::ALT_L;
					[[unlikely]]	case VK_RMENU		: return KEY::ALT_R;
					[[likely]]		case VK_LCONTROL	: return KEY::CTRL_L;
									case VK_RCONTROL	: return KEY::CTRL_R;
					[[likely]]		case VK_LSHIFT		: return KEY::SHIFT_L;
									case VK_RSHIFT		: return KEY::SHIFT_R;

					[[unlikely]]	case VK_NUMLOCK		: return KEY::NUM_LOCK;
									case VK_CAPITAL		: return KEY::CAPS_LOCK;
					[[unlikely]]	case VK_SCROLL		: return KEY::SCROLL_LOCK;

					[[likely]]		case VK_NUMPAD0		: return KEY::NUM_PAD_0;
					[[likely]]		case VK_NUMPAD1		: return KEY::NUM_PAD_1;
					[[likely]]		case VK_NUMPAD2		: return KEY::NUM_PAD_2;
					[[likely]]		case VK_NUMPAD3		: return KEY::NUM_PAD_3;
					[[likely]]		case VK_NUMPAD4		: return KEY::NUM_PAD_4;
					[[likely]]		case VK_NUMPAD5		: return KEY::NUM_PAD_5;
					[[likely]]		case VK_NUMPAD6		: return KEY::NUM_PAD_6;
					[[likely]]		case VK_NUMPAD7		: return KEY::NUM_PAD_7;
					[[likely]]		case VK_NUMPAD8		: return KEY::NUM_PAD_8;
					[[likely]]		case VK_NUMPAD9		: return KEY::NUM_PAD_9;
									case VK_ADD			: return KEY::NUM_PAD_ADD;
									case VK_SUBTRACT	: return KEY::NUM_PAD_SUB;
									case VK_MULTIPLY	: return KEY::NUM_PAD_MUL;
									case VK_DIVIDE		: return KEY::NUM_PAD_DIV;
									case VK_DECIMAL		: return KEY::NUM_PAD_DEC;
									case VK_CLEAR		: return KEY::NUM_PAD_CLEAR;

									case VK_OEM_1		: return KEY::OEM_1;
									case VK_OEM_2		: return KEY::OEM_2;
									case VK_OEM_3		: return KEY::OEM_3;
									case VK_OEM_4		: return KEY::OEM_4;
									case VK_OEM_5		: return KEY::OEM_5;
									case VK_OEM_6		: return KEY::OEM_6;
									case VK_OEM_7		: return KEY::OEM_7;
									case VK_OEM_8		: return KEY::OEM_8;
									case VK_OEM_PLUS	: return KEY::OEM_PLUS;
									case VK_OEM_MINUS	: return KEY::OEM_MINUS;
									case VK_OEM_COMMA	: return KEY::OEM_COMMA;
									case VK_OEM_PERIOD	: return KEY::OEM_PERIOD;

					[[unlikely]]	default				: return KEY( vk );
				}
		}
}



/**
 !	处理 事件消息
 */
namespace fce::inner::win
{
	/*
	 @	 按键消息
	 */
	FC_INLINE keyboard::KEY Win_Keyboard::ProcessKey( const WPARAM wParam ) noexcept
		{
			// 先区分 部分左右按键
			switch( wParam )
				{
					[[likely]]	case VK_SHIFT:
									{
										return GetKeyState( VK_LSHIFT ) < 0
											 ? KEY::SHIFT_L
											 : KEY::SHIFT_R;
									}

					[[likely]]	case VK_CONTROL:
									{
										return GetKeyState( VK_LCONTROL ) < 0
											 ? KEY::CTRL_L
											 : KEY::CTRL_R;
									}

								case VK_MENU:
									{
										return GetKeyState( VK_LMENU ) < 0
											 ? KEY::ALT_L
											 : KEY::ALT_R;
									}
				}

			// 转换其它 按键
			return this->Convert( int( wParam ) );
		}


	/*
	 @	 窗口键盘消息
	 */
	Bool Win_Keyboard::on_WndProc( const UINT uMsg,
								   const WPARAM wParam,
								   const LPARAM lParam ) noexcept
		{
			// 转换 win 消息
			EVENT& e{ this->message.e };

			// sys 主要响应 Alt 键
			switch( uMsg )
				{
					case WM_KEYDOWN:
					case WM_SYSKEYDOWN:
						{
							// 避免重复发送
							if( lParam & 0x40000000 )
								{
									return false;
								}
							else
								{
									e = EVENT::KEY_DOWN;
								}
						}
						break;

					case WM_KEYUP:
					case WM_SYSKEYUP:
						{
							e = EVENT::KEY_UP;
						}
						break;

					// 未处理消息 直接返回 false
					default:
						{
							return false;
						}
				}

			// 保存键值
			this->message.key = this->ProcessKey( wParam );

			// 分发消息给 引擎
			return this->message.Dispatch();
		}
}