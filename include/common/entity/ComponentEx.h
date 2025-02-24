/*
**
**	ComponentEx.
**
		Wrap entity interfaces
		Get & Add & Remove owner component

	Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	ComponentEx
	 */
	class FC_API ComponentEx
		:
		public Component
		{
		public:
			/*	Attached entity
			*/
			Entity& entity;

		public:
			/*
			 @	Access
			 */
			[[nodiscard]] auto* operator -> ( this auto& ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~ComponentEx() = default;

			ComponentEx( Entity& ) noexcept;
		};
}



/**
 !	class ComponentEx
 */
namespace fce
{
	/*
	 @	Constructor
	 */
	FC_INLINE ComponentEx::ComponentEx( Entity& entity ) noexcept
		:
			entity( entity )
		{
		}

	/*
	 @	Access
	 */
	FC_INLINE auto* ComponentEx::operator -> ( this auto& self ) noexcept
		{
			return &self.entity;
		}
}