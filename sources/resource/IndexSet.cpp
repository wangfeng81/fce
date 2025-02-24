/*
**
**	IndexSet
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/resource/IndexSet.h"


namespace fce::inner::indexSet
{
	/*
	 @	���� Model
	 */
	Info::Info( const UInt32 count,
				const TYPE type ) NOEXCEPT
		:
			type( type ),
			unitSize( sizeOf[ type ] ),
			count( count ),
			size( unitSize * count )
		{
			FC_ASSERT( *this );
		}

	Info::Info( const UInt32 count ) NOEXCEPT
		:
			Info( count,
				  count < UInt16_MAX
				? TYPE::UINT16 : TYPE::UINT32 )
		{
		}
}



namespace fce
{
	/*
	 @	���� Buffer
	 */
	IndexSet::IndexSet( const Buffer& buf )
		:
			IndexSet( Info( buf ) )
		{
			// ��������� ���ݽ���
			buf.Read( this->Data(),
					  this->info.size );
		}

	/*
	 @	���� move
	 */
	IndexSet::IndexSet( IndexSet&& rv ) noexcept
		:
			info( std::move( rv.info ) ),
			buffer( std::move( rv.buffer ) )
		{
		}

	/*
	 @	���� copy
	 */
	IndexSet::IndexSet( const IndexSet& lv ) noexcept
		:
			info( lv.info ),
			buffer( lv.buffer )
		{
		}

	/*
	 @	���� Info
	 */
	IndexSet::IndexSet( const UInt32 count,
						const TYPE type ) NOEXCEPT
		:
			IndexSet( Info( count, type ) )
		{
		}

	IndexSet::IndexSet( const Info& info ) NOEXCEPT
		:
			info( info ),
			buffer( info.size )
		{
			FC_ASSERT( info );
		}
}



namespace fce
{
	/*
	 @	Resize
	 */
	Void IndexSet::Resize( const Info& info ) NOEXCEPT
		{
			this->info = info;

			this->buffer.Resize( info.size );
		}

	Void IndexSet::Resize( const UInt32 count ) NOEXCEPT
		{
			this->Resize( count, this->Type() );
		}

	Void IndexSet::Resize( const UInt32 count,
						   const TYPE type ) NOEXCEPT
		{
			this->Resize( Info( count, type ) );
		}

	/*
	 @	���л�
	 */
	Buffer& IndexSet::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			buf << this->info;

			// ��������� ���ݱ���
			return buf.Write( this->Data(),
							  this->info.size );
		}


	/*
	 @	���� �ļ�
	 */
	RESULT IndexSet::Save( const Path& path ) const noexcept
		{
			// ���뵽 type ���͵�ͼ��
			Buffer buf( *this );

			return buf.Save( path );
		}
}