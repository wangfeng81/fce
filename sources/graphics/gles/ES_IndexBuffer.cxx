/*
**
**	ES_IndexBuffer.
**
		Gles implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Constructor.
	 */
	ES_IndexBuffer::Info::Info( const Indices::Info& info )
		:
			count( info.count )
		{
			// ������������
			this->type = VertexBuffer::ConvertDataType(
												info.type );

			// ͼԪ����
			this->mode = VertexBuffer::ConvertPrimitive(
												info.primitive );
		}


	/*
	 @	Constructor.
	 */
	ES_IndexBuffer::ES_IndexBuffer( const Indices& indices )
		:
			info( indices.GetInfo() )
		{
			// ���� ��������
			glGenBuffers( 1, &this->id );

			FC_ASSERT( this->id );

			// �� ��������
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
						  this->id );

			// ���� ��������
			glBufferData( GL_ELEMENT_ARRAY_BUFFER,
						  indices.Size(), *indices,
						  GL_STATIC_DRAW );
		}
}