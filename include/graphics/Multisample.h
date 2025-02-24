/*
**
**	Multisample.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "Graphics.h"


/**
 !	Multisample inner kit
 */
namespace fce::inner::multisample
{
	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			BIT_1,
			BIT_2,
			BIT_4,
			BIT_8,
			BIT_16,
			BIT_32,
			BIT_64,

			MAXIMUM
		};

	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			TYPE type;

			Bool enableSample;

			Bool enableAlphaToOne;

			Bool enableAlphaToCoverage;

			Float32 minSample;

			/*	Sample mask
			*/

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( type ),
							ARCHIVE_UNIT( enableSample ),
							ARCHIVE_UNIT( enableAlphaToOne ),
							ARCHIVE_UNIT( enableAlphaToCoverage ),
							ARCHIVE_UNIT( minSample ) )

		public:
			/*	Constructor
			*/
			Info( const TYPE						= TYPE::BIT_1,
				  const Bool sampleEnable			= false,
				  const Bool alphaToOneEnable		= false,
				  const Bool alphaToCoverageEnable	= false,
				  const Float32 minSample			= 1.0f ) noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Multisample.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Multisample.h"
#elif defined  FC_METAL
	#include "metal/MT_Multisample.h"
#elif defined  FC_GLES
	#include "gles/ES_Multisample.h"
#endif


namespace fce
{
	/*
	 !	Multisample
	 */
	class FC_API Multisample final
		{
			/*	Friends
			*/
			friend RenderPipeline;

		public:
			/*	Implementation
			*/
			using Impl = inner::multisample::Impl;

			/*	Using inner kit
			*/
			using Info = inner::multisample::Info;

			using TYPE = inner::multisample::TYPE;

		public:
			/*	Convert
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Multisample );

			~Multisample() = default;

			Multisample() = default;
		};
}