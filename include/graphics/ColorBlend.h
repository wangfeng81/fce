/*
**
**	ColorBlend.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "Graphics.h"


/**
 !	ColorBlend inner kit
 */
namespace fce::inner::colorBlend
{
	/*	CHANNEL
	*/
	using CHANNEL = Color::CHANNEL;

	/*	EQUATION
	 */
	enum struct EQUATION
		:
			Byte
		{
			MIN,
			MAX,

			ADD,
			SUBTRACT,
			REVERSE_SUBTRACT,

			MAXIMUM
		};

	/*	FACTOR
	 */
	enum struct FACTOR
		:
			Byte
		{
			ZERO,
			ONE,

			SRC_COLOR,
			SRC_ALPHA,

			DST_COLOR,
			DST_ALPHA,

			CONSTANT_COLOR,
			CONSTANT_ALPHA,

			ONE_MINUS_SRC_COLOR,
			ONE_MINUS_SRC_ALPHA,

			ONE_MINUS_DST_COLOR,
			ONE_MINUS_DST_ALPHA,

			ONE_MINUS_CONSTANT_COLOR,
			ONE_MINUS_CONSTANT_ALPHA,

			SRC_ALPHA_SATURATE,

			MAXIMUM
		};

	/*	Logic operation
	 */
	enum struct LOGIC
		:
			Byte
		{
			CLEAR,
			AND,
			AND_REVERSE,
			COPY,
			AND_INVERTED,
			NO_OP,
			XOR,
			OR,
			NOR,
			EQUIVALENT,
			INVERT,
			OR_REVERSE,
			COPY_INVERTED,
			OR_INVERTED,
			NAND,
			SET,

			MAXIMUM
		};

	/*	Blend
	 */
	struct FC_API Blend final
		{
			/*	Model
			*/
			union
				{
					QWord value;

					struct
						{
							/*	Enable blend
							*/
							Bool enable;

							/*	Blend equation
							*/
							EQUATION eqColor;

							EQUATION eqAlpha;

							/*	Color factor
							*/
							FACTOR srcColor;

							FACTOR dstColor;

							/*	Alpha factor
							*/
							FACTOR srcAlpha;

							FACTOR dstAlpha;

							/*	Color mask
							*/
							CHANNEL channel;
						};
				};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Blend,
							ARCHIVE_UNIT( enable ),
							ARCHIVE_UNIT( eqColor ),
							ARCHIVE_UNIT( eqAlpha ),
							ARCHIVE_UNIT( srcColor ),
							ARCHIVE_UNIT( dstColor ),
							ARCHIVE_UNIT( srcAlpha ),
							ARCHIVE_UNIT( dstAlpha ),
							ARCHIVE_UNIT( channel ) )

		public:
			/*	Constructor
			*/
			Blend( const Bool		enable		= false,
				   const EQUATION	eqColor		= EQUATION::ADD,
				   const EQUATION	eqAlpha		= EQUATION::ADD,
				   const FACTOR		srcColor	= FACTOR::SRC_ALPHA,
				   const FACTOR		dstColor	= FACTOR::ONE_MINUS_SRC_ALPHA,
				   const FACTOR		srcAlpha	= FACTOR::DST_ALPHA,
				   const FACTOR		dstAlpha	= FACTOR::ZERO,
				   const CHANNEL				= CHANNEL::RGBA ) noexcept;

			/*	Check
			*/
			RESULT Check() const noexcept;
		};

	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			Bool logicOpEnable;

			LOGIC logicOp;

			Color4F constant;

			stl::vector< Blend > arrBlends;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( logicOpEnable ),
							ARCHIVE_UNIT( logicOp ),
							ARCHIVE_UNIT( constant ),
							ARCHIVE_INFO( arrBlends, "Blend" ) )

		public:
			/*	Access
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] auto& operator [] ( this auto&,
											  const UInt );

		public:
			/*	Constructor
			*/
			Info( const Bool		logicOpEnable		= false,
				  const LOGIC		logicOp				= LOGIC::COPY,
				  const Color4F&	constant			= 0,
				  const std::initializer_list< Blend >& = {} ) noexcept;

			/*	Add Blend
			*/
			Blend& Add( const Bool		enable		= false,
						const EQUATION	eqColor		= EQUATION::ADD,
						const EQUATION	eqAlpha		= EQUATION::ADD,
						const FACTOR	srcColor	= FACTOR::ONE,
						const FACTOR	dstColor	= FACTOR::ZERO,
						const FACTOR	srcAlpha	= FACTOR::ONE,
						const FACTOR	dstAlpha	= FACTOR::ZERO,
						const CHANNEL				= CHANNEL::RGBA ) noexcept;

			/*	Check
			*/
			RESULT Check() const noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_ColorBlend.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_ColorBlend.h"
#elif defined  FC_METAL
	#include "metal/MT_ColorBlend.h"
#elif defined  FC_GLES
	#include "gles/ES_ColorBlend.h"
#endif


namespace fce
{
	/*
	 !	Color blend
	 */
	class FC_API ColorBlend final
		{
			/*	Friends
			*/
			friend RenderPipeline;

			friend inner::colorBlend::Impl;

		public:
			/*	Implementation
			*/
			using Impl = inner::colorBlend::Impl;

			/*	Using inner kit
			*/
			using CHANNEL = Color::CHANNEL;

			using Info = inner::colorBlend::Info;

			using Blend = inner::colorBlend::Blend;

			using LOGIC = inner::colorBlend::LOGIC;

			using FACTOR = inner::colorBlend::FACTOR;

			using EQUATION = inner::colorBlend::EQUATION;

		public:
			/*	Converts
			*/
			CONVERT_IMPL_ENUM_VALUE( LOGIC );

			CONVERT_IMPL_ENUM_VALUE( FACTOR );

			CONVERT_IMPL_ENUM_VALUE( CHANNEL );

			CONVERT_IMPL_ENUM_VALUE( EQUATION );

		public:
			/*	Add
			*/
			template< class SrcColor,
					  class DstColor,
					  typename SrcFactor,
					  typename DstFactor >

			static auto Add( const SrcColor& srcColor,
							 const DstColor& dstColor,
							 const SrcFactor& srcFactor,
							 const DstFactor& dstFactor ) noexcept;
		
			/*	Subtract
			*/
			template< class SrcColor,
					  class DstColor,
					  typename SrcFactor,
					  typename DstFactor >

			static auto Subtract( const SrcColor& srcColor,
								  const DstColor& dstColor,
								  const SrcFactor& srcFactor,
								  const DstFactor& dstFactor ) noexcept;
		
			/*	Multiply
			*/
			template< class SrcColor,
					  class DstColor,
					  typename SrcFactor,
					  typename DstFactor >

			static auto Multiply( const SrcColor& srcColor,
								  const DstColor& dstColor,
								  const SrcFactor& srcFactor,
								  const DstFactor& dstFactor ) noexcept;
		
			/*	Min
			*/
			template< class SrcColor,
					  class DstColor,
					  typename SrcFactor,
					  typename DstFactor >

			static auto Min( const SrcColor& srcColor,
							 const DstColor& dstColor,
							 const SrcFactor& srcFactor,
							 const DstFactor& dstFactor ) noexcept;
		
			/*	Max
			*/
			template< class SrcColor,
					  class DstColor,
					  typename SrcFactor,
					  typename DstFactor >

			static auto Max( const SrcColor& srcColor,
							 const DstColor& dstColor,
							 const SrcFactor& srcFactor,
							 const DstFactor& dstFactor ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( ColorBlend );

			~ColorBlend() = default;

			ColorBlend() = default;
		};
}



/**
 !	class Info
 */
namespace fce::inner::colorBlend
{
	/*
	 @	Access
	 */
	FC_INLINE auto Info::end() const noexcept
		{
			return this->arrBlends.cend();
		}

	FC_INLINE auto Info::begin() const noexcept
		{
			return this->arrBlends.cbegin();
		}

	FC_INLINE UInt32 Info::Count() const noexcept
		{
			return static_cast< UInt32 >
					(
						this->arrBlends.size()
					);
		}

	FC_INLINE auto& Info::operator [] ( this auto& self,
										const UInt idx )
		{
			return self.arrBlends[ idx ];
		}
}



/**
 !	ColorBlend operations
 */
namespace fce
{
	/*
	 @	Add
	 */
	template< class SrcColor,
			  class DstColor,
			  typename SrcFactor,
			  typename DstFactor >

	auto ColorBlend::Add( const SrcColor& srcColor,
						  const DstColor& dstColor,
						  const SrcFactor& srcFactor,
						  const DstFactor& dstFactor ) noexcept
		{
			return srcColor * srcFactor
				 + dstColor * dstFactor;
		}
	
	/*
	 @	Subtract
	 */
	template< class SrcColor,
			  class DstColor,
			  typename SrcFactor,
			  typename DstFactor >

	auto ColorBlend::Subtract( const SrcColor& srcColor,
							   const DstColor& dstColor,
							   const SrcFactor& srcFactor,
							   const DstFactor& dstFactor ) noexcept
		{
			return srcColor * srcFactor
				 - dstColor * dstFactor;
		}
	
	/*
	 @	Multiply
	 */
	template< class SrcColor,
			  class DstColor,
			  typename SrcFactor,
			  typename DstFactor >

	auto ColorBlend::Multiply( const SrcColor& srcColor,
							   const DstColor& dstColor,
							   const SrcFactor& srcFactor,
							   const DstFactor& dstFactor ) noexcept
		{
			return srcColor * srcFactor
				 * dstColor * dstFactor;
		}
	
	/*
	 @	Min
	 */
	template< class SrcColor,
			  class DstColor,
			  typename SrcFactor,
			  typename DstFactor >

	auto ColorBlend::Min( const SrcColor& srcColor,
						  const DstColor& dstColor,
						  const SrcFactor& srcFactor,
						  const DstFactor& dstFactor ) noexcept
		{
			return ( srcColor * srcFactor ).
				Min( dstColor * dstFactor );
		}
	
	/*
	 @	Max
	 */
	template< class SrcColor,
			  class DstColor,
			  typename SrcFactor,
			  typename DstFactor >

	auto ColorBlend::Max( const SrcColor& srcColor,
						  const DstColor& dstColor,
						  const SrcFactor& srcFactor,
						  const DstFactor& dstFactor ) noexcept
		{
			return ( srcColor * srcFactor ).
				Max( dstColor * dstFactor );
		}
}