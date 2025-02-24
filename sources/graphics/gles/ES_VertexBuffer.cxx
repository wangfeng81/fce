/*
**
**	ES_VertexBuffer.
**
		Gles implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 @	Describe::Constructor.ת�� �Զ��� �������� �� GL��ʽ
	 */
	FC_INLINE ES_VertexBuffer::Describe::Element::Element( const Vertices::Describe::Element& element )
		:
			index( element.slot ),
			size( element.count ),
			offset( element.offset )
		{
		  this->type = VertexBuffer::ConvertDataType( element.type );
		}


	/*
	 @	Describe::Constructor.ת�� �Զ��� �������� �� GL��ʽ
	 */
	ES_VertexBuffer::Describe::Describe( const Vertices::Describe& describe )
		:
			stride( describe.stride ),
			listElements( describe.GetCount() )
		{
			for( UInt8 c = 0; c < describe.GetCount(); ++c )
				{
					this->listElements[ c ] = describe[ c ];
				}
		}


	/*
	 @	Use.
	 */
	GLvoid ES_VertexBuffer::Describe::Use() const
		{
			for( auto& p : this->listElements )
				{
					glVertexAttribPointer( p.index, p.size, p.type,
										   GL_FALSE, this->stride,
										   ( const GLvoid* )( Int64 )p.offset );
				}
		}


	/*
	 @	Constructor.
	 */
	ES_VertexBuffer::ES_VertexBuffer( const Vertices::Describe& describe,
									  const Vertices& vertices )
		:
			describe( describe ),
			count( vertices.Count() )
		{
			// ��������
			glGenBuffers( 1, &this->id );

			FC_ASSERT( this->id );

			// �󶨻���
			glBindBuffer( GL_ARRAY_BUFFER,
						  this->id );

			// ���ػ�������
			glBufferData( GL_ARRAY_BUFFER,
						  vertices.Size(),
						 *vertices,
						  GL_STATIC_DRAW );

			// ͼԪ����
			this->mode = VertexBuffer::ConvertPrimitive(
									vertices.Primitive() );

		}
}