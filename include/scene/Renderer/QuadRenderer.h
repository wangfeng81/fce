/*
**
**	Quad renderer.
**
		Copyright (c) WangFeng since 2006...
*/

/**
 !	Quad inner kit
 */
namespace fce::renderer::quad
{
	/*	Vertex
	 */
	struct FC_API Vertex
		{
			Vector4 position;
		};
}


namespace fce::renderer
{
	/*
	 !	Quad renderer
	 */
	class FC_API Quad
		:
		public Primitive
		{
			/*	Friends
			*/
			friend Camera;

			friend gui::Picture;

			COMPONENT( Quad )

		public:
			/*	Using inner kit
			*/
			using Vertex = quad::Vertex;

			/*	Update
			*/
			Void Update() noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Quad() noexcept override;

			Quad() = default;

			Quad( Entity&,
				  const Buffer& );

			Quad( Node&,
				  Material* const ) NOEXCEPT;
			
			/*	Render
			*/
			Void Use( inner::window::Surface& ) const noexcept;

			Void Present( inner::window::Surface& ) const noexcept;

			/*	On
			*/
			Void on_Constructed() noexcept;

			Void on_Transformed( Node::EVENT,
								 Node& ) noexcept;

		protected:
			/*	Vertex set
			*/
			VertexSet vertexSet;
			//static VertexBuffer vertexBuffer;
		};
}



/*
 !	Implementation
 */
namespace fce
{
	/*
	 @	QuadRenderer
	 */
	using QuadRenderer = renderer::Quad;
}