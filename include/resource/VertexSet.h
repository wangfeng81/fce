/*
**
**	Vertex set
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../graphics/Graphics.h"


/*
 !	VertexSet inner kit
 */
namespace fce::inner::vertexSet
{
	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			UInt32 size;

			UInt32 count;

			UInt16 unitSize;

			Word reserved{};

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( size ),
							ARCHIVE_UNIT( count ),
							ARCHIVE_UNIT( unitSize ),
							ARCHIVE_UNIT( reserved ) )

		public:
			/*	Constructor
			*/
			Info() = default;

			Info( const UInt32 count,
				  const UInt16 unitSize ) NOEXCEPT;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;
		};
}



namespace fce
{
	/*
	 !	Vertex set
	 */
	class FC_API VertexSet final
		{
			/*	Friends
			*/
			friend VertexBuffer;

			/*	Cross platform Buffer
			*/
			#ifdef FC_VULKAN
				inner::vulkan::VK_TempBuffer buffer;
			#else
				Buffer buffer;
			#endif

		public:
			/*	Using inner kit
			*/
			using Info = inner::vertexSet::Info;

			/*	Attributes
			*/
			[[nodiscard]] UInt32 Size() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] UInt16 UnitSize() const noexcept;

			/*	Access
			*/
			template< typename T = Byte >

			[[nodiscard]] auto* Data( this auto& ) noexcept;

			[[nodiscard]] explicit operator Bool() const noexcept;

			[[nodiscard]] const auto& operator * () const noexcept;

			[[nodiscard]] const Info* operator -> () const noexcept;

			/*	Resize
			*/
			Void Resize( const Info& ) NOEXCEPT;

			Void Resize( const UInt32 count ) NOEXCEPT;

			Void Resize( const UInt32 count,
						 const UInt16 unitSize ) NOEXCEPT;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Save ( Syntax sugar )
			*/
			RESULT Save( const Path& ) const noexcept;

		public:
			/*	Constructor & Destructor
			*/
			VertexSet() = default;

			VertexSet( const Buffer& );

			VertexSet( VertexSet&& ) noexcept;

			VertexSet( const VertexSet& ) noexcept;

			VertexSet( const Info& ) NOEXCEPT;

			VertexSet( const UInt32 count,
					   const UInt16 unitSize ) NOEXCEPT;

		private:
			/*	Info
			*/
			Info info;
		};
}



/**
 !	class Info
 */
namespace fce::inner::vertexSet
{
	/*
	 @	Check whether the buffer is valid
	 */
	FC_INLINE Info::operator Bool() const noexcept
		{
			return this->size
				&& this->size == this->count * this->unitSize;
		}
}



/**
 !	class VertexSet
 */
namespace fce
{
	/*
	 @	Is valid
	 */
	FC_INLINE VertexSet::operator Bool() const noexcept
		{
			return this->buffer;
		}

	/*
	 @	Attributes
	 */
	FC_INLINE UInt32 VertexSet::Size() const noexcept
		{
			return this->info.size;
		}

	FC_INLINE UInt32 VertexSet::Count() const noexcept
		{
			return this->info.count;
		}

	FC_INLINE UInt16 VertexSet::UnitSize() const noexcept
		{
			return this->info.unitSize;
		}

	/*
	 @	Data
	 */
	template< typename T >

	FC_INLINE auto* VertexSet::Data( this auto& self ) noexcept
		{
			return self.buffer.Data< T >();
		}

	/*
	 @	Buffer
	 */
	FC_INLINE const auto& VertexSet::operator * () const noexcept
		{
			return this->buffer;
		}

	/*
	 @	Get Info
	 */
	FC_INLINE const VertexSet::Info* VertexSet::operator -> () const noexcept
		{
			return &this->info;
		}
}