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
	 @	构造
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
 !	处理 事件消息
 */
namespace fce::inner::win
{
	/*
	 @	滑轮
	 */
	FC_INLINE Int32 Win_Mouse::ProcessWheel( const WPARAM wParam ) noexcept
		{
			return GET_WHEEL_DELTA_WPARAM( wParam ) / 120;
		}


	/*
	 @	坐标
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
	 @	WM_XBUTTONUP	分离 x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonUp( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_UP
				 : EVENT::X2_BTN_UP;
		}


	/*
	 @	WM_XBUTTONDOWN	分离 x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonDown( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_DOWN
				 : EVENT::X2_BTN_DOWN;
		}


	/*
	 @	WM_XBUTTONDBLCLK	分离 x button 1 | 2
	 */
	FC_INLINE Mouse::EVENT Win_Mouse::ProcessXButtonDblClk( const WPARAM wParam ) noexcept
		{
			WORD fwButton{ GET_XBUTTON_WPARAM( wParam ) };

			return fwButton == XBUTTON1
				 ? EVENT::X1_BTN_DBLCLK
				 : EVENT::X2_BTN_DBLCLK;
		}


	/*
	 @	窗口消息 （客户区）
	 */
	Bool Win_Mouse::on_WndProc( const UINT uMsg,
								const WPARAM wParam,
								const LPARAM lParam ) noexcept
		{
			// 转换 win 消息
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
									// 移动
					[[likely]]		case WM_MOUSEMOVE		: e = EVENT::MOVE;			break;

									// 左键
					[[likely]]		case WM_LBUTTONDOWN		: e = EVENT::L_BTN_DOWN;	break;
					[[likely]]		case WM_LBUTTONUP		: e = EVENT::L_BTN_UP;		break;
					[[likely]]		case WM_LBUTTONDBLCLK	: e = EVENT::L_BTN_DBLCLK;	break;

									// 右键
									case WM_RBUTTONDOWN		: e = EVENT::R_BTN_DOWN;	break;
									case WM_RBUTTONUP		: e = EVENT::R_BTN_UP;		break;
									case WM_RBUTTONDBLCLK	: e = EVENT::R_BTN_DBLCLK;	break;

									// 中键
					[[unlikely]]	case WM_MBUTTONDOWN		: e = EVENT::M_BTN_DOWN;	break;
					[[unlikely]]	case WM_MBUTTONUP		: e = EVENT::M_BTN_UP;		break;
					[[unlikely]]	case WM_MBUTTONDBLCLK	: e = EVENT::M_BTN_DBLCLK;	break;

									// 滑轮
									case WM_MOUSEHWHEEL		:
									case WM_MOUSEWHEEL		: e = EVENT::WHEEL;
										{
											this->message.wheelDelta =
												this->ProcessWheel( wParam );

											// WM_MOUSEWHEEL 的鼠标坐标 是屏幕坐标，毋须重复保存
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

									// 未处理消息 直接返回 false
									default:
										{
											return false;
										}
				}

			// 保存坐标
			this->message.position = this->ProcessPosition( lParam );

			// 分发消息给 引擎
			DISPATCH :

			return this->message.Dispatch();
		}


	/*
	 @	窗口消息 （非客户区）
	 */
	Bool Win_Mouse::on_NcWndProc( const UINT uMsg,
								  const WPARAM wParam,
								  const LPARAM lParam ) noexcept
		{
			// 如果没有 handler 则提前退出
			if( ! this->handler )
				{
					return false;
				}

			// 转换 win 消息
			EVENT& e{ this->message.e };

			switch( uMsg )
				{
									// 移动
					[[likely]]		case WM_NCMOUSEMOVE		: e = EVENT::MOVE;			break;

									// 左键
					[[likely]]		case WM_NCLBUTTONDOWN	: e = EVENT::L_BTN_DOWN;	break;
					[[likely]]		case WM_NCLBUTTONUP		: e = EVENT::L_BTN_UP;		break;
					[[likely]]		case WM_NCLBUTTONDBLCLK	: e = EVENT::L_BTN_DBLCLK;	break;

									// 右键
									case WM_NCRBUTTONDOWN	: e = EVENT::R_BTN_DOWN;	break;
									case WM_NCRBUTTONUP		: e = EVENT::R_BTN_UP;		break;
									case WM_NCRBUTTONDBLCLK	: e = EVENT::R_BTN_DBLCLK;	break;

									// 中键
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

									// 未处理消息 直接返回
									default:
										{
											return false;
										}
				}

			// 保存坐标 转换窗口坐标
			this->message.position = this->window.ScreenToWindow(
										this->ProcessPosition( lParam ) );

			// 发送消息
			return this->handler( this->message );
		}
}