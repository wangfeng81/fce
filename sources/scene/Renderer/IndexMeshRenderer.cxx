/*
**
**	Index mesh renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 @	构造 Buffer
	 */
	IndexMesh::IndexMesh( Entity& entity,
						  const Buffer& buf )
		:
			renderer::Mesh( entity, buf )
		{
			*this << buf;
		}

	/*
	 @	构造 模型
	 */
	IndexMesh::IndexMesh( Node& node,
						  Material* const material,
						  IndexBuffer* const indexBuffer,
						  VertexBuffer* const vertexBuffer ) NOEXCEPT
		:
			renderer::Mesh( node,
							material,
							vertexBuffer ),
			indexBuffer( indexBuffer )
		{
			FC_ASSERT( indexBuffer );
		}


	/*
	 @	Use
	 */
	Void IndexMesh::Use( inner::window::Surface& surface ) const noexcept
		{
			// use mesh
			Mesh::Use( surface );

			// 绑定 索引缓冲
			surface.Bind( this->indexBuffer );
		}


	/*
	 @	Present
	 */
	Void IndexMesh::Present( inner::window::Surface& surface ) const noexcept
		{
			this->Use( surface );

			surface.DrawIndex( this->indexBuffer->Count(), 1 );
		}
}