/*
**
**	Mesh renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 @	���� Buffer
	 */
	Mesh::Mesh( Entity& entity,
				const Buffer& buf )
		:
			Renderer( entity, buf )
		{
			*this << buf;
		}

	/*
	 @	���� ģ��
	 */
	Mesh::Mesh( Node& node,
				Material* const material,
				VertexBuffer* const vertexBuffer ) NOEXCEPT
		:
			Renderer( node,
					  material ),
			vertexBuffer( vertexBuffer )
		{
			FC_ASSERT( vertexBuffer );
		}


	/*
	 @	Use
	 */
	Void Mesh::Use( inner::window::Surface& surface ) const noexcept
		{
			// Ӧ�� ����
			this->material->Apply( surface );

			// �� ���㻺��
			surface.Bind( this->vertexBuffer );

			// ���� wvp
			surface.PushConstant( matWVP, Shader::TYPE::VERTEX );
		}


	/*
	 @	Present
	 */
	Void Mesh::Present( inner::window::Surface& surface ) const noexcept
		{
			this->Use( surface );

			surface.Draw( this->vertexBuffer->Count(), 1 );
		}
}