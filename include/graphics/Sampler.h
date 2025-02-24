/*
**
**	Sampler.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Graphics.h"


/*
 !	Sampler inner kit
 */
namespace fce::inner::sampler
{
	/*	MIPMAP
	 */
	enum struct MIPMAP
		:
			Byte
		{
			NEAREST,
			LINEAR,

			MAXIMUM
		};

	/*	FILTER
	 */
	enum struct FILTER
		:
			Byte
		{
			NEAREST,
			LINEAR,

			CUBIC,

			MAXIMUM
		};

	/*	ADDRESS
	 */
	enum struct ADDRESS
		:
			Byte
		{
			REPEAT,
			MIRRORED_REPEAT,
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER,
			MIRROR_CLAMP_TO_EDGE,

			MAXIMUM
		};


	/*	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			MIPMAP mipmap;

			FILTER filterMag;
			FILTER filterMin;

			ADDRESS addressU;
			ADDRESS addressV;
			ADDRESS addressW;

			COMPARE compare;
			Bool compareEnable;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( mipmap ),
							ARCHIVE_UNIT( filterMag ),
							ARCHIVE_UNIT( filterMin ),
							ARCHIVE_UNIT( addressU ),
							ARCHIVE_UNIT( addressV ),
							ARCHIVE_UNIT( addressW ),
							ARCHIVE_UNIT( compare ),
							ARCHIVE_UNIT( compareEnable ) )

		public:
			/*	Constructor
			*/
			Info( const MIPMAP		= MIPMAP::LINEAR,
				  const FILTER mag	= FILTER::LINEAR,
				  const FILTER min	= FILTER::LINEAR,
				  const ADDRESS u	= ADDRESS::REPEAT,
				  const ADDRESS v	= ADDRESS::REPEAT,
				  const ADDRESS w	= ADDRESS::REPEAT,
				  const COMPARE		= COMPARE::NEVER,
				  const Bool enable = false ) noexcept;

			/*	Check
			*/
			RESULT Check() const noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Sampler.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Sampler.h"
#elif defined  FC_METAL
	#include "metal/MT_Sampler.h"
#elif defined  FC_GLES
	#include "gles/ES_Sampler.h"
#endif


namespace fce
{
	/*
	 !	Sampler
	 */
	class FC_API Sampler final
		:
		public ObjectHashContainerSugar
			   < Sampler >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Sampler, Path );

			/*	Allocated by object container
			*/
			OBJECT_HASH_CONTAINER_SUGAR( Sampler );

		public:
			/*	Using inner kit
			*/
			using Info = inner::sampler::Info;

			using MIPMAP = inner::sampler::MIPMAP;

			using FILTER = inner::sampler::FILTER;

			using ADDRESS = inner::sampler::ADDRESS;

			/*	Implementation
			*/
			using Impl = inner::sampler::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Default
			*/
			[[nodiscard]] static Sampler* Default() noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Sampler() noexcept;

			Sampler( RESULT&,
					 const Buffer& );

			Sampler( RESULT&,
					 const Info& );

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Info& );

			/*	Convert
			*/
			CONVERT_IMPL_ENUM_VALUE( MIPMAP );

			CONVERT_IMPL_ENUM_VALUE( FILTER );

			CONVERT_IMPL_ENUM_VALUE( ADDRESS );
		};
}