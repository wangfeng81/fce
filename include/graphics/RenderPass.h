/*
**
**	RenderPass.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Multisample.h"


/*
 !	RenderPass inner kit
 */
namespace fce::inner::renderPass
{
	/*	Attachment format
	 */
	enum struct FORMAT
		:
			Byte
		{
			DEPTH,
			STENCIL,
			DEPTH_STENCIL,

			SWAP_CHAIN,

			MAXIMUM
		};

	/*	Attachment load op
	 */
	enum struct LOAD
		:
			Byte
		{
			LOAD,
			CLEAR,
			DONT_CARE,

			MAXIMUM
		};

	/*	Attachment store op
	 */
	enum struct STORE
		:
			Byte
		{
			STORE,
			DONT_CARE,

			MAXIMUM
		};

	/*	Image layout
	 */
	enum struct LAYOUT
		:
			Byte
		{
			UNDEFINED,
			GENERAL,
			COLOR_OPTIMAL,
			DEPTH_STENCIL_OPTIMAL,
			DEPTH_STENCIL_READ_ONLY_OPTIMAL,
			SHADER_READ_ONLY_OPTIMAL,
			TRANSFER_SRC_OPTIMAL,
			TRANSFER_DST_OPTIMAL,
			PREINITIALIZED,

			PRESENT_SRC_KHR,

			MAXIMUM
		};

	/*	Pipeline stage
	 */
	enum struct STAGE
		:
			Byte
		{
			TOP_OF_PIPE,
			DRAW_INDIRECT,
			VERTEX_INPUT,
			VERTEX_SHADER,
			CONTROL_SHADER,
			EVALUATION_SHADER,
			GEOMETRY_SHADER,
			FRAGMENT_SHADER,
			EARLY_FRAGMENT_TESTS,
			LATE_FRAGMENT_TESTS,
			COLOR_OUTPUT,
			COMPUTE_SHADER,
			TRANSFER,
			BOTTOM_OF_PIPE,
			HOST,
			ALL_GRAPHICS,
			ALL_COMMANDS,
			NONE,

			MAXIMUM
		};

	/*	ACCESS
	 */
	enum struct ACCESS
		:
			Byte
		{
			INDIRECT_COMMAND_READ,
			INDEX_READ,
			VERTEX_READ,
			UNIFORM_READ,
			INPUT_READ,
			SHADER_READ,
			SHADER_WRITE,
			COLOR_READ,
			COLOR_WRITE,
			DEPTH_STENCIL_READ,
			DEPTH_STENCIL_WRITE,
			TRANSFER_READ,
			TRANSFER_WRITE,
			HOST_READ,
			HOST_WRITE,
			MEMORY_READ,
			MEMORY_WRITE,
			NONE,

			MAXIMUM
		};

	/*	Dependency
	 */
	enum struct DEPENDENCY
		:
			Byte
		{
			BY_REGION,
			DEVICE_GROUP,
			VIEW_LOCAL,
			FEEDBACK_LOOP_EXT,

			MAXIMUM
		};


	/*	Attachment reference
	 */
	struct FC_API Reference final
		{
			/*	Model
			*/
			UInt32 index;

			LAYOUT layout;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Reference,
							ARCHIVE_UNIT( index ),
							ARCHIVE_UNIT( layout ) )

		public:
			/*	Constructor
			*/
			Reference( const UInt32 = UInt32_MAX,
					   const LAYOUT = LAYOUT::UNDEFINED ) noexcept;
		};


	/*	Attachment description
	 */
	struct FC_API Attachment final
		{
			/*	Model
			*/
			Multisample::TYPE	sample;
			FORMAT              format;
			LOAD				load;
			STORE				store;
			LOAD				stencilLoad;
			STORE				stencilStore;
			LAYOUT				initialLayout;
			LAYOUT				finalLayout;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Attachment,
							ARCHIVE_UNIT( sample ),
							ARCHIVE_UNIT( format ),
							ARCHIVE_UNIT( load ),
							ARCHIVE_UNIT( store ),
							ARCHIVE_UNIT( stencilLoad ),
							ARCHIVE_UNIT( stencilStore ),
							ARCHIVE_UNIT( initialLayout ),
							ARCHIVE_UNIT( finalLayout ) )

		public:
			/*	Constructor
			*/
			Attachment( const Multisample::TYPE			= Multisample::TYPE::BIT_1,
						const FORMAT					= FORMAT::SWAP_CHAIN,
						const LOAD						= LOAD::DONT_CARE,
						const STORE						= STORE::DONT_CARE,
						const LOAD		stencilLoad		= LOAD::DONT_CARE,
						const STORE		stencilStore	= STORE::DONT_CARE,
						const LAYOUT	initial			= LAYOUT::UNDEFINED,
						const LAYOUT	final			= LAYOUT::UNDEFINED ) noexcept;
		};


	/*	Subpass description
	 */
	struct FC_API Subpass final
		{
			/*	Model
			*/
			Reference resolve;

			Reference depthStencil;

			stl::vector< Reference > arrInputs;

			stl::vector< Reference > arrColors;

			stl::vector< UInt32 > arrPreserves;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Subpass,
							ARCHIVE_UNIT( resolve ),
							ARCHIVE_UNIT( depthStencil ),
							ARCHIVE_INFO( arrInputs,	"Input" ),
							ARCHIVE_INFO( arrColors,	"Color" ),
							ARCHIVE_INFO( arrPreserves,	"Preserve" ) )

		public:
			/*	Constructor
			*/
			Subpass() = default;
		};


	/*	Subpass dependency
	 */
	struct FC_API Dependency final
		{
			/*	Model
			*/
			UInt32 srcSubpass;

			UInt32 dstSubpass;

			stl::vector< STAGE > srcStages;

			stl::vector< STAGE > dstStages;

			stl::vector< ACCESS > srcAccesses;

			stl::vector< ACCESS > dstAccesses;

			stl::vector< DEPENDENCY > arrTypes;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Dependency,
							ARCHIVE_UNIT( srcSubpass ),
							ARCHIVE_UNIT( dstSubpass ),
							ARCHIVE_INFO( arrTypes,		"Type" ),
							ARCHIVE_INFO( srcStages,	"SrcStage" ),
							ARCHIVE_INFO( dstStages,	"DstStage" ),
							ARCHIVE_INFO( srcAccesses,	"SrcAccess" ),
							ARCHIVE_INFO( dstAccesses,	"DstAccess" ) )

		public:
			/*	Constructor
			*/
			Dependency( const UInt32 srcSubpass = 0,
						const UInt32 dstSubpass = 0 ) noexcept;
		};


	/*	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			stl::vector< Subpass >		arrSubpasses;

			stl::vector< Attachment >	arrAttachments;

			stl::vector< Dependency >	arrDependencies;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_INFO( arrSubpasses,		Subpass::ARCHIVE_NAME ),
							ARCHIVE_INFO( arrAttachments,	Attachment::ARCHIVE_NAME ),
							ARCHIVE_INFO( arrDependencies,	Dependency::ARCHIVE_NAME ) )

		public:
			/*	Constructor
			*/
			Info() = default;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_RenderPass.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_RenderPass.h"
#elif defined  FC_METAL
	#include "metal/MT_RenderPass.h"
#elif defined  FC_GLES
	#include "gles/ES_RenderPass.h"
#endif


namespace fce
{
	/*
	 !	RenderPass
	 */
	class FC_API RenderPass final
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						RenderPass, Path );

		public:
			/*	Using inner kit
			*/
			using Info = inner::renderPass::Info;

			using LOAD = inner::renderPass::LOAD;

			using STORE = inner::renderPass::STORE;

			using STAGE = inner::renderPass::STAGE;

			using FORMAT = inner::renderPass::FORMAT;

			using LAYOUT = inner::renderPass::LAYOUT;

			using ACCESS = inner::renderPass::ACCESS;

			using DEPENDENCY = inner::renderPass::DEPENDENCY;

			/*	Implementation
			*/
			using Impl = inner::renderPass::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Converts
			*/
			CONVERT_IMPL_ENUM_VALUE( LOAD );

			CONVERT_IMPL_ENUM_VALUE( STORE );

			CONVERT_IMPL_ENUM_VALUE( LAYOUT );

			CONVERT_IMPL_ENUM_VALUE( STAGE );

			CONVERT_IMPL_ENUM_VALUE( ACCESS );

			CONVERT_IMPL_ENUM_VALUE( DEPENDENCY );

			/*	Combines
			*/
			COMBINE_IMPL_ENUM_VALUE( STAGE );

			COMBINE_IMPL_ENUM_VALUE( ACCESS );

			COMBINE_IMPL_ENUM_VALUE( DEPENDENCY );

		protected:
			/*	Constructor & Destructor
			*/
			~RenderPass() noexcept;

			RenderPass( RESULT&,
						const Buffer& );

			RenderPass( RESULT&,
						const Info& );

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Info& );
		};
}