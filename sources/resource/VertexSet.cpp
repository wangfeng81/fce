/*
**
**	VertexSet
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/resource/VertexSet.h"


namespace fce::inner::vertexSet
{
	/*
	 @	构造 Model
	 */
	Info::Info( const UInt32 count,
				const UInt16 unitSize ) NOEXCEPT
		:
			count( count ),
			unitSize( unitSize ),
			size( unitSize * count )
		{
			FC_ASSERT( *this );
		}
}



namespace fce
{
	/*
	 @	构造 Buffer
	 */
	VertexSet::VertexSet( const Buffer& buf )
		:
			VertexSet( Info( buf ) )
		{
			// 这里可以做 数据解码
			buf.Read( this->Data(),
					  this->info.size );
		}

	/*
	 @	构造 move
	 */
	VertexSet::VertexSet( VertexSet&& rv ) noexcept
		:
			info( std::move( rv.info ) ),
			buffer( std::move( rv.buffer ) )
		{
		}

	/*
	 @	构造 copy
	 */
	VertexSet::VertexSet( const VertexSet& lv ) noexcept
		:
			info( lv.info ),
			buffer( lv.buffer )
		{
		}

	/*
	 @	构造 Info
	 */
	VertexSet::VertexSet( const Info& info ) NOEXCEPT
		:
			info( info ),
			buffer( info.size )
		{
			FC_ASSERT( info );
		}

	VertexSet::VertexSet( const UInt32 count,
						  const UInt16 unitSize ) NOEXCEPT
		:
			VertexSet( Info( count, unitSize ) )
		{
		}
}



namespace fce
{
	/*
	 @	Resize
	 */
	Void VertexSet::Resize( const Info& info ) NOEXCEPT
		{
			this->info = info;

			this->buffer.Resize( info.size );
		}

	Void VertexSet::Resize( const UInt32 count ) NOEXCEPT
		{
			this->Resize( count, this->UnitSize() );
		}

	Void VertexSet::Resize( const UInt32 count,
							const UInt16 unitSize ) NOEXCEPT
		{
			this->Resize( Info( count, unitSize ) );
		}

	/*
	 @	序列化
	 */
	Buffer& VertexSet::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			// 这里可以做 数据编码
			return buf << this->info
					   << this->buffer;
		}


	/*
	 @	保存 文件
	 */
	RESULT VertexSet::Save( const Path& path ) const noexcept
		{
			// 编码到 type 类型的图像
			Buffer buf( *this );

			return buf.Save( path );
		}
}