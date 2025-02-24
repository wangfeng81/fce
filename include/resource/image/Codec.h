/*
**
**	Image::Codec
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Info.h"


namespace fce::inner::image
{
	/*
	 !	Codec
	 */
	class FC_API Codec
		{
		public:
			/*	Decode & Encode interface
			*/
			virtual Bool Decode( const Buffer&,
									   Image& ) = 0;

			virtual Bool Encode( Buffer&,
								 const Image& ) = 0;

			/*	Get info interface
			*/
			[[nodiscard]] virtual Bool GetInfo( const Buffer&,
												Info& ) const = 0;

			[[nodiscard]] virtual Bool CheckType( const Buffer& ) const = 0;

		protected:
			/*	Destructor
			*/
			virtual ~Codec() = default;
		};
}