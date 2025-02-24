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
			// 索引数据类型
			this->type = VertexBuffer::ConvertDataType(
												info.type );

			// 图元类型
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
			// 创建 索引缓冲
			glGenBuffers( 1, &this->id );

			FC_ASSERT( this->id );

			// 绑定 索引缓冲
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER,
						  this->id );

			// 加载 索引缓冲
			glBufferData( GL_ELEMENT_ARRAY_BUFFER,
						  indices.Size(), *indices,
						  GL_STATIC_DRAW );
		}
}