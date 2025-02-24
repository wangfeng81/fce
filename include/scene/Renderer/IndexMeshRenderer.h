/*
**
**	Index mesh renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 !	IndexMesh renderer
	 */
	class FC_API IndexMesh
		:
		public renderer::Mesh
		{
			/*	Friends
			*/
			friend Camera;

			COMPONENT( IndexMesh )

		protected:
			/*	Constructor & Destructor
			*/
			~IndexMesh() override = default;

			IndexMesh( Entity&,
					   const Buffer& );

			IndexMesh( Node&,
					   Material* const,
					   IndexBuffer* const,
					   VertexBuffer* const ) NOEXCEPT;

			/*	Render
			*/
			Void Use( inner::window::Surface& ) const noexcept;

			Void Present( inner::window::Surface& ) const noexcept;

		protected:
			/*	Index buffer
			*/
			IndexBuffer* indexBuffer;
		};
}



/*
 !	Implementation
 */
namespace fce
{
	/*
	 @	IndexMeshRenderer
	 */
	using IndexMeshRenderer = renderer::IndexMesh;
}