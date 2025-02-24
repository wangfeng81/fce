/*
**
**	Index set
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "VertexSet.h"


/*
 !	IndexSet inner kit
 */
namespace fce::inner::indexSet
{
	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			TYPE type;

			UInt8 unitSize;

			Word reserved{};

			UInt32 count;

			UInt32 size;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( type ),
							ARCHIVE_UNIT( unitSize ),
							ARCHIVE_UNIT( reserved ),
							ARCHIVE_UNIT( count ),
							ARCHIVE_UNIT( size ) )

		public:
			/*	Constructor
			*/
			Info() = default;

			Info( const UInt32 count,
				  const TYPE ) NOEXCEPT;

			Info( const UInt32 count ) NOEXCEPT;

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
	class FC_API IndexSet final
		{
			/*	Friends
			*/
			friend IndexBuffer;

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
			using Info = inner::indexSet::Info;

			/*	Attributes
			*/
			[[nodiscard]] TYPE Type() const noexcept;

			[[nodiscard]] UInt32 Size() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] UInt8 UnitSize() const noexcept;

			/*	Access
			*/
			template< typename T = UInt16 >

			[[nodiscard]] auto* Data( this auto& ) noexcept;

			[[nodiscard]] explicit operator Bool() const noexcept;

			[[nodiscard]] const auto& operator * () const noexcept;

			[[nodiscard]] const Info* operator -> () const noexcept;

			/*	Resize
			*/
			Void Resize( const UInt32 count,
						 const TYPE ) NOEXCEPT;

			Void Resize( const Info& ) NOEXCEPT;

			Void Resize( const UInt32 count ) NOEXCEPT;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Save ( Syntax sugar )
			*/
			RESULT Save( const Path& ) const noexcept;

		public:
			/*	Constructor & Destructor
			*/
			IndexSet() = default;

			IndexSet( const Buffer& );

			IndexSet( IndexSet&& ) noexcept;

			IndexSet( const IndexSet& ) noexcept;

			IndexSet( const UInt32 count,
					  const TYPE ) NOEXCEPT;

			IndexSet( const Info& ) NOEXCEPT;

		private:
			/*	Info
			*/
			Info info;
		};
}



/**
 !	class Info
 */
namespace fce::inner::indexSet
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
 !	class IndexSet
 */
namespace fce
{
	/*
	 @	Is valid
	 */
	FC_INLINE IndexSet::operator Bool() const noexcept
		{
			return this->buffer;
		}

	/*
	 @	Attributes
	 */
	FC_INLINE TYPE IndexSet::Type() const noexcept
		{
			return this->info.type;
		}

	FC_INLINE UInt32 IndexSet::Size() const noexcept
		{
			return this->info.size;
		}

	FC_INLINE UInt32 IndexSet::Count() const noexcept
		{
			return this->info.count;
		}

	FC_INLINE UInt8 IndexSet::UnitSize() const noexcept
		{
			return this->info.unitSize;
		}

	/*
	 @	Data
	 */
	template< typename T >

	FC_INLINE auto* IndexSet::Data( this auto& self ) noexcept
		{
			return self.buffer.Data< T >();
		}

	/*
	 @	Buffer
	 */
	FC_INLINE const auto& IndexSet::operator * () const noexcept
		{
			return this->buffer;
		}

	/*
	 @	Get Info
	 */
	FC_INLINE const IndexSet::Info* IndexSet::operator -> () const noexcept
		{
			return &this->info;
		}
}