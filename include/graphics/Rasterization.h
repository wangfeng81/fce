/*
**
**	Rasterization.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "Graphics.h"


/**
 !	Rasterization inner kit
 */
namespace fce::inner::rasterization
{
	/*	Fill mode
	 */
	enum struct FILL
		:
			Byte
		{
			SOLID,
			LINE,
			POINT,

			MAXIMUM
		};

	/*	Cull mode
	 */
	enum struct CULL
		:
			Byte
		{
			NONE,
			FRONT,
			BACK,
			ALL,

			MAXIMUM
		};

	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			FILL fill;

			CULL cull;

			Bool discard;

			Bool clockwise;

			Float32 lineWidth;

			Float32 depthBias;

			Float32 depthBiasSlope;

			Float32 depthBiasClamp;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( fill ),
							ARCHIVE_UNIT( cull ),
							ARCHIVE_UNIT( discard ),
							ARCHIVE_UNIT( clockwise ),
							ARCHIVE_UNIT( lineWidth ),
							ARCHIVE_UNIT( depthBias ),
							ARCHIVE_UNIT( depthBiasSlope ),
							ARCHIVE_UNIT( depthBiasClamp ) )

		public:
			/*	Constructor
			*/
			Info( const FILL					= FILL::SOLID,
				  const CULL					= CULL::BACK,
				  const Bool discard			= false,
				  const Bool clockwise			= false,
				  const Float32 lineWidth		= 1.0f,
				  const Float32 depthBias		= 0.0f,
				  const Float32 depthBiasSlope	= 0.0f,
				  const Float32 depthBiasClamp	= 0.0f ) noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Rasterization.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Rasterization.h"
#elif defined  FC_METAL
	#include "metal/MT_Rasterization.h"
#elif defined  FC_GLES
	#include "gles/ES_Rasterization.h"
#endif


namespace fce
{
	/*
	 !	Rasterization
	 */
	class FC_API Rasterization final
		{
			/*	Friends
			*/
			friend RenderPipeline;

		public:
			/*	Implementation
			*/
			using Impl = inner::rasterization::Impl;

			/*	Using inner kit
			*/
			using Info = inner::rasterization::Info;

			using FILL = inner::rasterization::FILL;

			using CULL = inner::rasterization::CULL;

		public:
			/*	Convert
			*/
			CONVERT_IMPL_ENUM_VALUE( FILL );

			CONVERT_IMPL_ENUM_VALUE( CULL );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Rasterization );

			~Rasterization() = default;

			Rasterization() = default;
		};
}