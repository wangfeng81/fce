/*
**
**	Multisample.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/graphics/Multisample.h"


/**
 !	Multisample inner kit
 */
namespace fce::inner::multisample
{
	/*
	 @	ππ‘Ï
	 */
	Info::Info( const TYPE		type,
				const Bool		enableSample,
				const Bool		enableAlphaToOne,
				const Bool		enableAlphaToCoverage,
				const Float32	minShading ) noexcept
		:
			type					( type ),
			enableSample			( enableSample ),
			enableAlphaToOne		( enableAlphaToOne ),
			enableAlphaToCoverage	( enableAlphaToCoverage ),
			minSample				( minSample )
		{
		}
}