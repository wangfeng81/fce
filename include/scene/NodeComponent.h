/*
**
**	Scene node component.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::scene
{
	/*
	 !	Node component
	 */
	class FC_API NodeComponent
		:
		public Component
		{
		public:
			/*	Using inner kit
			*/
			using Node = Node;

			/*	Attached node
			*/
			Node& node;

		public:
			/*
			 @	Access
			 */
			[[nodiscard]] auto* operator -> ( this auto& ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~NodeComponent() = default;

			NodeComponent( Node& ) noexcept;

			NodeComponent( Entity& ) NOEXCEPT;
		};
}



namespace fce
{
	/*
	 @	SceneNodeComponent
	 */
	using SceneNodeComponent = inner::scene::NodeComponent;
}



/**
 !	class NodeComponent
 */
namespace fce::inner::scene
{
	/*
	 @	Access
	 */
	FC_INLINE auto* NodeComponent::operator -> ( this auto& self ) noexcept
		{
			return &self.node;
		}
}