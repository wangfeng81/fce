/*
**
**	Mesh renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 !	Mesh renderer
	 */
	class FC_API Mesh
		:
		public Renderer
		{
			/*	Friends
			*/
			friend Camera;

			COMPONENT( Mesh )

		protected:
			/*	Constructor & Destructor
			*/
			~Mesh() override = default;

			Mesh( Entity&,
				  const Buffer& );

			Mesh( Node&,
				  Material* const,
				  VertexBuffer* const ) NOEXCEPT;

			/*	Render
			*/
			Void Use( inner::window::Surface& ) const noexcept;

			Void Present( inner::window::Surface& ) const noexcept;

		protected:
			/*	Vertex buffer
			*/
			VertexBuffer* vertexBuffer;
		};
}



/*
 !	Implementation
 */
namespace fce
{
	/*
	 @	MeshRenderer
	 */
	using MeshRenderer = renderer::Mesh;
}