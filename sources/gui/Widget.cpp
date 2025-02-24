/*
**
**	Widget.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/gui/Widget.h"


namespace fce::gui
{
	/*
	 @	����
	 */
	Widget::~Widget() noexcept
		{
		}

	/*
	 @	���� Ĭ��
	 */
	Widget::Widget( Node& node ) noexcept
		:
			NodeComponent( node )
		{
		}

	Widget::Widget( Entity& entity ) NOEXCEPT
		:
			Widget( static_cast< Node& >( entity ) )
		{
		}
}