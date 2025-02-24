/*
**
**	Primitive renderer.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::renderer
{
	/*
	 !	Primitive renderer
	 */
	class FC_API Primitive
		:
		public Renderer
		{
			/*	Friends
			*/
			COMPONENT( Primitive )

		protected:
			/*	Constructor & Destructor
			*/
			~Primitive() override = default;

			Primitive() = default;

			Primitive( Entity&,
					   const Buffer& );

			Primitive( Node&,
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
	 @	PrimitiveRenderer
	 */
	using PrimitiveRenderer = renderer::Primitive;
}