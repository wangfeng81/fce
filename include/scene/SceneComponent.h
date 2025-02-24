/*
**
**	Scene component.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../scene/Scene.h"


namespace fce
{
	/*
	 !	Scene component
	 */
	class FC_API SceneComponent
		:
		public Component
		{
		public:
			/*	Scene
			*/
			Scene& scene;

			/*	Insert component
			*/
			template< X_component T,
					  typename ...Args >

			T* InsertComponent( Args&&... ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			~SceneComponent() = default;

			SceneComponent( Entity& );
		};
}



/**
 !	class SceneComponent
 */
namespace fce
{
	/*	concept		Derived component
	 */
	template< class T >

	concept Scene_component = std::derived_from< T, SceneComponent >;

	/*
	 @	Constructor
	 */
	FC_INLINE SceneComponent::SceneComponent( Entity& entity )
		:
			scene( static_cast< Scene& >( scene ) )
		{
			FC_ASSERT_LOG( typeid( Scene ) == typeid( entity ),
						   "The %s can only be added to Scene...",
						   typeid( *this ).name() );
		}

	/*
	 @	Insert	component
	 */
	template< X_component T,
			  typename ...Args >

	FC_INLINE T* SceneComponent::InsertComponent( Args&& ...args ) NOEXCEPT
		{
			return this->entity.InsertComponent< T >
					(
						this,
						std::forward< Args >( args )...
					);
		}
}