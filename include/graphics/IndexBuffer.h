/*
**
**	IndexBuffer.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "VertexBuffer.h"

/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_IndexBuffer.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_IndexBuffer.h"
#elif defined  FC_METAL
	#include "metal/MT_IndexBuffer.h"
#elif defined  FC_GLES
	#include "gles/ES_IndexBuffer.h"
#endif


namespace fce
{
	/*
	 !	IndexBuffer
	 */
	class FC_API IndexBuffer final
		:
		public ObjectContainerSugar
			   < IndexBuffer >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						IndexBuffer, Path );

			/*	Allocated by object container
			*/
			OBJECT_CONTAINER_SUGAR( IndexBuffer );

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Using inner kit
			*/
			using Info = inner::indexSet::Info;

			/*	Implementation
			*/
			using Impl = inner::indexBuffer::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Update
			*/
			RESULT Update( const IndexSet&,
						   const UInt32 dstOffset = 0 ) NOEXCEPT;

			RESULT Update( const IndexSet&,
						   const UInt32 size,
						   const UInt32 srcOffset,
						   const UInt32 dstOffset = 0 ) NOEXCEPT;

		public:
			/*	Attributes
			*/
			[[nodiscard]] TYPE Type() const noexcept;

			[[nodiscard]] UInt32 Size() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] UInt8 UnitSize() const noexcept;

			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~IndexBuffer() noexcept;


			IndexBuffer( RESULT&,
						 const Buffer& );

			IndexBuffer( RESULT&,
						 const UInt32 count,
						 const TYPE ) NOEXCEPT;

			IndexBuffer( RESULT&,
						 const Info& ) NOEXCEPT;

			IndexBuffer( RESULT&,
						 const IndexSet& ) NOEXCEPT;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const UInt32 size ) noexcept;

		private:
			/*	Info
			*/
			Info info;
		};
}



/**
 !	class IndexBuffer
 */
namespace fce
{
	/*
	 @	Attributes
	 */
	FC_INLINE TYPE IndexBuffer::Type() const noexcept
		{
			return this->info.type;
		}

	FC_INLINE UInt32 IndexBuffer::Size() const noexcept
		{
			return this->info.size;
		}

	FC_INLINE UInt32 IndexBuffer::Count() const noexcept
		{
			return this->info.count;
		}

	FC_INLINE UInt8 IndexBuffer::UnitSize() const noexcept
		{
			return this->info.unitSize;
		}

	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& IndexBuffer::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}
}