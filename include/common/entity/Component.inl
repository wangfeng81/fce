/*
**
**	Component inlines
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 @	DependentOn reference ( Syntax sugar )
	 */
	FC_INLINE Bool Component::DependentOn( this const auto& self,
										   const Entity& entity ) noexcept
		{
			return self.DependentOn( &entity );
		}

	FC_INLINE Bool Component::DependentOn( this const auto& self,
										   const Component& cmpt ) noexcept
		{
			return self.DependentOn( &cmpt );
		}

	/*
	 @	Dependent on entity & component
	 */
	FC_INLINE Bool Component::DependentOn( const Entity* const entity ) const noexcept
		{
			return false;
		}

	FC_INLINE Bool Component::DependentOn( const Component* const cmpt ) const noexcept
		{
			return false;
		}
}