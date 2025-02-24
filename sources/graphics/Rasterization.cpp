/*
**
**	Rasterization.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/graphics/Rasterization.h"


/**
 !	Rasterization inner kit
 */
namespace fce::inner::rasterization
{
	/*
	 @	构造 默认值
	 */
	Info::Info( const FILL		fill,
				const CULL		cull,
				const Bool		discard,
				const Bool		clockwise,
				const Float32	lineWidth,
				const Float32	depthBias,
				const Float32	depthBiasSlope,
				const Float32	depthBiasClamp ) noexcept
		:
			fill			( fill ),
			cull			( cull ),
			discard			( discard ),
			clockwise		( clockwise ),
			lineWidth		( lineWidth ),
			depthBias		( depthBias ),
			depthBiasSlope	( depthBiasSlope ),
			depthBiasClamp	( depthBiasClamp )
		{
		}
}