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
	 @	���� Model
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
	 @	���� Buffer
	 */
	VertexSet::VertexSet( const Buffer& buf )
		:
			VertexSet( Info( buf ) )
		{
			// ��������� ���ݽ���
			buf.Read( this->Data(),
					  this->info.size );
		}

	/*
	 @	���� move
	 */
	VertexSet::VertexSet( VertexSet&& rv ) noexcept
		:
			info( std::move( rv.info ) ),
			buffer( std::move( rv.buffer ) )
		{
		}

	/*
	 @	���� copy
	 */
	VertexSet::VertexSet( const VertexSet& lv ) noexcept
		:
			info( lv.info ),
			buffer( lv.buffer )
		{
		}

	/*
	 @	���� Info
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
	 @	���л�
	 */
	Buffer& VertexSet::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			// ��������� ���ݱ���
			return buf << this->info
					   << this->buffer;
		}


	/*
	 @	���� �ļ�
	 */
	RESULT VertexSet::Save( const Path& path ) const noexcept
		{
			// ���뵽 type ���͵�ͼ��
			Buffer buf( *this );

			return buf.Save( path );
		}
}