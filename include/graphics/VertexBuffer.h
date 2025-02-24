/*
**
**	Vertex buffer.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../resource/IndexSet.h"


/*
 !	Vertex inner kit
 */
namespace fce::inner::vertex
{
	/*	Binding
	 */
	using Binding = stl::vector< TYPE >;

	/*	PRIMITIVE
	 */
	enum struct PRIMITIVE
		:
			Byte
		{
			POINT,

			LINE,
			LINE_STRIP,

			TRIANGLE,
			TRIANGLE_STRIP,
			TRIANGLE_FAN,

			LINE_ADJ,
			LINE_STRIP_ADJ,
			TRIANGLE_ADJ,
			TRIANGLE_STRIP_ADJ,
			PATCH,

			MAXIMUM
		};


	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			Bool restart;

			PRIMITIVE primitive;

			stl::vector< Binding > arrBindings;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( restart ),
							ARCHIVE_UNIT( primitive ),
							ARCHIVE_INFO( arrBindings, "Element" ) )

		public:
			/*	Access bindings
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] auto& operator [] ( this auto&,
											  const UInt );

			/*	Size of vertex
			*/
			[[nodiscard]] UInt16 UnitSize() const noexcept;

			/*	Count of elements
			*/
			[[nodiscard]] UInt CountElements() const noexcept;

		public:
			/*	Constructor
			*/
			Info( const Bool restart = false,
				  const PRIMITIVE = PRIMITIVE::TRIANGLE,
				  stl::vector< Binding >&& = {} ) noexcept;

			/*	Set vertex layout
			*/
			Void Set( const Bool restart = false,
					  const PRIMITIVE = PRIMITIVE::TRIANGLE,
					  stl::vector< Binding >&& = {} ) noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_VertexBuffer.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_VertexBuffer.h"
#elif defined  FC_METAL
	#include "metal/MT_VertexBuffer.h"
#elif defined  FC_GLES
	#include "gles/ES_VertexBuffer.h"
#endif


namespace fce
{
	/*
	 !	VertexBuffer
	 */
	class FC_API VertexBuffer final
		:
		public ObjectContainerSugar
			   < VertexBuffer >
		{
			/*	Friends
			*/
			friend Graphics;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						VertexBuffer, Path );

			/*	Allocated by object container
			*/
			OBJECT_CONTAINER_SUGAR( VertexBuffer );

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Using inner kit
			*/
			using Info = inner::vertex::Info;

			using PRIMITIVE = inner::vertex::PRIMITIVE;

			/*	Implementation
			*/
			using Impl = inner::vertexBuffer::Impl;

			friend Impl;

			Impl impl;

		public:
			/*	Update
			*/
			RESULT Update( const VertexSet&,
						   const UInt32 dstOffset = 0 ) NOEXCEPT;

			RESULT Update( const VertexSet&,
						   const UInt32 size,
						   const UInt32 srcOffset,
						   const UInt32 dstOffset = 0 ) NOEXCEPT;

		public:
			/*	Attributes
			*/
			[[nodiscard]] UInt32 Size() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] UInt16 UnitSize() const noexcept;

			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Converts
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

			CONVERT_IMPL_ENUM_VALUE( PRIMITIVE );

		protected:
			/*	Constructor & Destructor
			*/
			~VertexBuffer() noexcept;

			VertexBuffer( RESULT&,
						  const Buffer& );

			VertexBuffer( RESULT&,
						  const VertexSet& ) NOEXCEPT;

			VertexBuffer( RESULT&,
						  const UInt32 count,
						  const UInt16 unitSize ) NOEXCEPT;

			VertexBuffer( RESULT&,
						  const VertexSet::Info& ) NOEXCEPT;

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const UInt32 size ) noexcept;

		private:
			/*	Info
			*/
			VertexSet::Info info;
		};
}



/**
 !	Vertex inner kit
 */
namespace fce::inner::vertex
{
	/*
	 @	Access
	 */
	FC_INLINE auto Info::end() const noexcept
		{
			return this->arrBindings.cend();
		}

	FC_INLINE auto Info::begin() const noexcept
		{
			return this->arrBindings.cbegin();
		}

	FC_INLINE UInt32 Info::Count() const noexcept
		{
			return static_cast< UInt32 >
					(
						this->arrBindings.size()
					);
		}

	FC_INLINE auto& Info::operator [] ( this auto& self,
										const UInt idx )
		{
			FC_ASSERT( idx < this->arrBindings.size() );

			return self.arrBindings[ idx ];
		}
}



/**
 !	class VertexBuffer
 */
namespace fce
{
	/*
	 @	Attributes
	 */
	FC_INLINE UInt32 VertexBuffer::Size() const noexcept
		{
			return this->info.size;
		}

	FC_INLINE UInt32 VertexBuffer::Count() const noexcept
		{
			return this->info.count;
		}

	FC_INLINE UInt16 VertexBuffer::UnitSize() const noexcept
		{
			return this->info.unitSize;
		}

	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& VertexBuffer::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}
}