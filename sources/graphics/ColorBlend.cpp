/*
**
**	ColorBlend.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/graphics/ColorBlend.h"


/**
 !	Blend inner kit
 */
namespace fce::inner::colorBlend
{
	/*
	 @	构造
	 */
	Blend::Blend( const Bool		enable,
				  const EQUATION	eqColor,
				  const EQUATION	eqAlpha,
				  const FACTOR		srcColor,
				  const FACTOR		dstColor,
				  const FACTOR		srcAlpha,
				  const FACTOR		dstAlpha,
				  const CHANNEL		channel ) noexcept
		:
			enable		( enable ),
			eqColor		( eqColor ),
			eqAlpha		( eqAlpha ),
			srcColor	( srcColor ),
			dstColor	( dstColor ),
			srcAlpha	( srcAlpha ),
			dstAlpha	( dstAlpha ),
			channel		( channel )
		{
		}


	/*
	 @	检测
	 */
	RESULT Blend::Check() const noexcept
		{
			if( this->channel >= CHANNEL::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->eqColor >= EQUATION::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->eqAlpha >= EQUATION::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->srcColor >= FACTOR::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->dstColor >= FACTOR::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->srcAlpha >= FACTOR::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			else if( this->dstAlpha >= FACTOR::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}

			return RESULT::SUCCESS;
		}
}



/**
 !	Info inner kit
 */
namespace fce::inner::colorBlend
{
	/*
	 @	构造
	 */
	Info::Info( const Bool								logicOpEnable,
				const LOGIC								logicOp,
				const Color4F&							constant,
				const std::initializer_list< Blend >&	ilist ) noexcept
		:
			logicOpEnable	( logicOpEnable ),
			logicOp			( logicOp ),
			constant		( constant ),
			arrBlends		( ilist )
		{
		}

	/*
	 @	添加 Blend
	 */
	Blend& Info::Add( const Bool		enable,
					  const EQUATION	eqColor,
					  const EQUATION	eqAlpha,
					  const FACTOR		srcColor,
					  const FACTOR		dstColor,
					  const FACTOR		srcAlpha,
					  const FACTOR		dstAlpha,
					  const CHANNEL		channel ) noexcept
		{
			return this->arrBlends.emplace_back( enable,
												 eqColor,
												 eqAlpha,
												 srcColor,
												 dstColor,
												 srcAlpha,
												 dstAlpha,
												 channel );
		}


	/*
	 @	检测
	 */
	RESULT Info::Check() const noexcept
		{
			if( this->logicOp >= LOGIC::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}

			return RESULT::SUCCESS;
		}
}