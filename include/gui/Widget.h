/*
**
**	Widget.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../scene/Scene.h"


namespace fce::gui
{
	/*	NodeComponent
	*/
	using NodeComponent = Scene::NodeComponent;


	/*
	 !	Widget
	 */
	class FC_API Widget
		:
		public NodeComponent
		{
			/*	Friends
			*/
			//friend Camera;

		public:
			/*	Render
			*/
			//virtual Void Render() const noexcept = 0;

		protected:
			/*	Constructor & Destructor
			*/
			~Widget() noexcept;

			Widget( Node& ) noexcept;

			Widget( Entity& ) NOEXCEPT;
		};
}