/*
**
**	Primitive renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 @	构造 Buffer
	 */
	Primitive::Primitive( Entity& entity,
						  const Buffer& buf )
		:
			Renderer( entity, buf )
		{
			*this << buf;
		}

	/*
	 @	构造 模型
	 */
	Primitive::Primitive( Node& node,
						  Material* const material,
						  VertexBuffer* const vertexBuffer ) NOEXCEPT
		:
			Renderer( node, material ),
			vertexBuffer( vertexBuffer )
		{
			FC_ASSERT( vertexBuffer );
		}


	/*
	 @	Use
	 */
	Void Primitive::Use( inner::window::Surface& surface ) const noexcept
		{
			// 应用 材质
			this->material->Apply( surface );

			// 绑定 顶点缓冲
			surface.Bind( this->vertexBuffer );

			// 推送 wvp
			surface.PushConstant( matWVP, Shader::TYPE::VERTEX );
		}


	/*
	 @	Present
	 */
	Void Primitive::Present( inner::window::Surface& surface ) const noexcept
		{
			this->Use( surface );

			surface.Draw( this->vertexBuffer->Count(), 1 );
		}
}