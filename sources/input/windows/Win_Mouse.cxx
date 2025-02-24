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
	 @	����
	 */
	FC_INLINE Win_Mouse::Win_Mouse( Mouse& mouse ) noexcept
		:
			message( mouse ),
			window( mouse.window )
		{
		}


	/*
	 @	Capture messages outside the window
	 */
	HWND Win_Mouse::Capture() noexcept
		{
			return SetCapture( this->window.impl );
		}
}



/**
 !	���� �¼���Ϣ
 */
namespace fce::inner::win
{
	/*
	 @	����
	 */
	FC_INLINE Int32 Win_Mouse::ProcessWheel( const WPARAM wParam ) noexcept
		{
			return GET_WHEEL_DELTA_WPARAM( wParam ) / 120;
		}


	/*
	 @	����
	 */
	FC_INLINE Point Win_Mouse::ProcessPosition( const LPARAM lParam ) noexcept
		{
			if constexpr( endian_requires_conversion< LPARAM > )
				{
					return Point( static_cast< Int16 >( GET_X_LPARAM( lParam ) ),
								  static_cast< Int16 >( GET_Y_LPARAM( lParam ) ) );
				}
			else
				{
					return reinterpret_cast< const Point& >( lParam );
				}
		}


	/*
	 @	WM_XBUTTONUP	���� x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonUp( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_UP
				 : EVENT::X2_BTN_UP;
		}


	/*
	 @	WM_XBUTTONDOWN	���� x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonDown( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_DOWN
				 : EVENT::X2_BTN_DOWN;
		}


	/*
	 @	WM_XBUTTONDBLCLK	���� x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonDblClk( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_DBLCLK
				 : EVENT::X2_BTN_DBLCLK;
		}


	/*
	 @	������Ϣ ���ͻ�����
	 */
	Bool Win_Mouse::on_WndProc( const UINT uMsg,
								const WPARAM wParam,
								const LPARAM lParam ) noexcept
		{
			// ת�� win ��Ϣ
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
									// �ƶ�
					[[likely]]		case WM_MOUSEMOVE		: e = EVENT::MOVE;			break;

									// ���
					[[likely]]		case WM_LBUTTONDOWN		: e = EVENT::L_BTN_DOWN;	break;
					[[likely]]		case WM_LBUTTONUP		: e = EVENT::L_BTN_UP;		break;
					[[likely]]		case WM_LBUTTONDBLCLK	: e = EVENT::L_BTN_DBLCLK;	break;

									// �Ҽ�
									case WM_RBUTTONDOWN		: e = EVENT::R_BTN_DOWN;	break;
									case WM_RBUTTONUP		: e = EVENT::R_BTN_UP;		break;
									case WM_RBUTTONDBLCLK	: e = EVENT::R_BTN_DBLCLK;	break;

									// �м�
					[[unlikely]]	case WM_MBUTTONDOWN		: e = EVENT::M_BTN_DOWN;	break;
					[[unlikely]]	case WM_MBUTTONUP		: e = EVENT::M_BTN_UP;		break;
					[[unlikely]]	case WM_MBUTTONDBLCLK	: e = EVENT::M_BTN_DBLCLK;	break;

									// ����
									case WM_MOUSEHWHEEL		:
									case WM_MOUSEWHEEL		: e = EVENT::WHEEL;
										{
											this->message.wheelDelta =
												this->ProcessWheel( wParam );

											// WM_MOUSEWHEEL ��������� ����Ļ���꣬�����ظ�����
										}
										goto DISPATCH;

									// x button 1 & 2
					[[unlikely]]	case WM_XBUTTONDOWN:
										{
											e = this->ProcessXButtonDown( wParam );
										}
										break;

					[[unlikely]]	case WM_XBUTTONUP:
										{
											e = this->ProcessXButtonUp( wParam );
										}
										break;

					[[unlikely]]	case WM_XBUTTONDBLCLK:
										{
											e = this->ProcessXButtonDblClk( wParam );
										}
										break;

									// δ������Ϣ ֱ�ӷ��� false
									default:
										{
											return false;
										}
				}

			// ��������
			this->message.position = this->ProcessPosition( lParam );

			// �ַ���Ϣ�� ����
			DISPATCH :

			return this->message.Dispatch();
		}


	/*
	 @	������Ϣ ���ǿͻ�����
	 */
	Bool Win_Mouse::on_NcWndProc( const UINT uMsg,
								  const WPARAM wParam,
								  const LPARAM lParam ) noexcept
		{
			// ���û�� handler ����ǰ�˳�
			if( ! this->handler )
				{
					return false;
				}

			// ת�� win ��Ϣ
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
									// �ƶ�
					[[likely]]		case WM_NCMOUSEMOVE		: e = EVENT::MOVE;			break;

									// ���
					[[likely]]		case WM_NCLBUTTONDOWN	: e = EVENT::L_BTN_DOWN;	break;
					[[likely]]		case WM_NCLBUTTONUP		: e = EVENT::L_BTN_UP;		break;
					[[likely]]		case WM_NCLBUTTONDBLCLK	: e = EVENT::L_BTN_DBLCLK;	break;

									// �Ҽ�
									case WM_NCRBUTTONDOWN	: e = EVENT::R_BTN_DOWN;	break;
									case WM_NCRBUTTONUP		: e = EVENT::R_BTN_UP;		break;
									case WM_NCRBUTTONDBLCLK	: e = EVENT::R_BTN_DBLCLK;	break;

									// �м�
					[[unlikely]]	case WM_NCMBUTTONDOWN	: e = EVENT::M_BTN_DOWN;	break;
					[[unlikely]]	case WM_NCMBUTTONUP		: e = EVENT::M_BTN_UP;		break;
					[[unlikely]]	case WM_NCMBUTTONDBLCLK	: e = EVENT::M_BTN_DBLCLK;	break;

									// x button 1 & 2
					[[unlikely]]	case WM_NCXBUTTONDOWN:
										{
											e = this->ProcessXButtonDown( wParam );
										}
										break;

					[[unlikely]]	case WM_NCXBUTTONUP:
										{
											e = this->ProcessXButtonUp( wParam );
										}
										break;

					[[unlikely]]	case WM_NCXBUTTONDBLCLK:
										{
											e = this->ProcessXButtonDblClk( wParam );
										}
										break;

									// δ������Ϣ ֱ�ӷ���
									default:
										{
											return false;
										}
				}

			// �������� ת����������
			this->message.position = this->window.ScreenToWindow(
										this->ProcessPosition( lParam ) );

			// ������Ϣ
			return this->handler( this->message );
		}
}