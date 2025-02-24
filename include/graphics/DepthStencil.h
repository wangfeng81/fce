/*
**
**	Depth stencil.
**
		Copyright (c) WangFeng since 2016...
*/

#pragma once

#include "Graphics.h"


/**
 !	DepthStencil inner kit
 */
namespace fce::inner::depthStencil
{
	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			DEPTH,
			STENCIL,
			DEPTH_STENCIL,

			MAXIMUM
		};

	/*	STENCIL
	 */
	enum struct STENCIL
		:
			Byte
		{
			KEEP,
			ZERO,
			REPLACE,
			INCREMENT_AND_CLAMP,
			DECREMENT_AND_CLAMP,
			INVERT,
			INCREMENT_AND_WRAP,
			DECREMENT_AND_WRAP,

			MAXIMUM
		};

	/*
	 !	Stencil
	 */
	struct FC_API Stencil final
		{
			/*	Model
			*/
			STENCIL	failOp;
			STENCIL	passOp;
			STENCIL	depthFailOp;

			COMPARE	compareOp;

			UInt32	compareMask;
			UInt32	writeMask;
			UInt32	reference;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Stencil,
							ARCHIVE_UNIT( failOp ),
							ARCHIVE_UNIT( passOp ),
							ARCHIVE_UNIT( depthFailOp ),
							ARCHIVE_UNIT( compareOp ),
							ARCHIVE_UNIT( compareMask ),
							ARCHIVE_UNIT( writeMask ),
							ARCHIVE_UNIT( reference ) )

		public:
			/*	Constructor
			*/
			Stencil( const STENCIL failOp		= STENCIL::KEEP,
					 const STENCIL passOp		= STENCIL::KEEP,
					 const STENCIL depthFailOp	= STENCIL::KEEP,
					 const COMPARE compareOp	= COMPARE::NEVER,
					 const UInt32 compareMask	= 0,
					 const UInt32 writeMask		= 0,
					 const UInt32 reference		= 0 ) noexcept;

			/*	Convert
			*/
			#ifdef FC_VULKAN

				[[nodiscard]] operator VkStencilOpState() const;

			#endif
		};

	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			Bool enableDepth;
			Bool enableStencil;
			Bool enableWrite;
			Bool enableBounds;

			COMPARE	compare;

			Float32	minDepth;
			Float32	maxDepth;

			Stencil	front;
			Stencil	back;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( enableDepth ),
							ARCHIVE_UNIT( enableStencil ),
							ARCHIVE_UNIT( enableWrite ),
							ARCHIVE_UNIT( enableBounds ),
							ARCHIVE_UNIT( compare ),
							ARCHIVE_UNIT( minDepth ),
							ARCHIVE_UNIT( maxDepth ),
							ARCHIVE_UNIT( front ),
							ARCHIVE_UNIT( back ) )

		public:
			/*	Constructor
			*/
			Info( const Bool depth		= false,
				  const Bool stencil	= false,
				  const Bool write		= false,
				  const Bool bounds		= false,
				  const COMPARE			= COMPARE::NEVER,
				  const Float32	min		= Float_ZERO,
				  const Float32	max		= Float_ONE,
				  const Stencil front	= Stencil(),
				  const Stencil back	= Stencil() ) noexcept;

			/*	Convert
			*/
			#ifdef FC_VULKAN

				[[nodiscard]] operator VkPipelineDepthStencilStateCreateInfo() const;

			#endif
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_DepthStencil.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_DepthStencil.h"
#elif defined  FC_METAL
	#include "metal/MT_DepthStencil.h"
#elif defined  FC_GLES
	#include "gles/ES_DepthStencil.h"
#endif


namespace fce
{
	/*
	 !	DepthStencil
	 */
	class FC_API DepthStencil final
		:
		public ObjectContainerSugar
			   < DepthStencil >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by object container
			*/
			OBJECT_CONTAINER_SUGAR( DepthStencil );

		public:
			/*	Implementation
			*/
			using Impl = inner::depthStencil::Impl;

			/*	Using inner kit
			*/
			using Info = inner::depthStencil::Info;

			using TYPE = inner::depthStencil::TYPE;

			using STENCIL = inner::depthStencil::STENCIL;

			/*	Implementation
			*/
			using Impl = inner::depthStencil::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Convert
			*/
			CONVERT_IMPL_ENUM_VALUE( STENCIL );

		protected:
			/*	Constructor & Destructor
			*/
			~DepthStencil() noexcept;

			DepthStencil( RESULT&,
						  const TYPE,
						  const Size& ) noexcept;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const TYPE,
							   const Size& ) noexcept;
		};
}