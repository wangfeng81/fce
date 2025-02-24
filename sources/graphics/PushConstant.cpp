/*
**
**	Push constant.
**
		Copyright (c) WangFeng since 2016...
*/

#include "../../include/graphics/PushConstant.h"


/**
 !	PushConstant inner kit
 */
namespace fce::inner::pushConstant
{
	/*
	 @	ππ‘Ï
	 */
	Info::Info( const UInt16 size,
				const std::initializer_list< STAGE >& ilist ) noexcept
		:
			size( size ),
			arrStages( ilist )
		{
		}
}