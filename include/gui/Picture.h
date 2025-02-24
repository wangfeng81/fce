/*
**
**	Picture.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Widget.h"


namespace fce::gui
{
	/*
	 !	Picture
	 */
	class FC_API Picture
		:
		public NodeComponent
		{
			/*	Friends
			*/
			//friend Camera;

			/*
			 @	Archive
			 */
			//COMPONENT_ARCHIVE( Picture,
			//				   NodeComponent,
			//				   ARCHIVE_UNIT( path ) );

		public:
			/*	Render
			*/
			//virtual Void Render() const noexcept = 0;

		protected:
			/*	Constructor & Destructor
			*/
			~Picture() noexcept;

			//Picture( Entity&,
			//		 const Buffer& );

			//Picture( Node&,
			//		 const Path& ) NOEXCEPT;

			//Picture( Node&,
			//		 const Path&,
			//		 const Rect& ) NOEXCEPT;

			//Picture( Node&,
			//		 const Image&,
			//		 const Rect& ) NOEXCEPT;

			//Picture( Node&,
			//		 const Rect&,
			//		 Texture2* const ) NOEXCEPT;

			//Picture( Node&,
			//		 const Rect&,
			//		 material::Texture* const ) NOEXCEPT;

		private:
			/*	Path
			*/
			Path path;

			/*	Clipped rect
			*/
			Rect rcCliped;

			/*	VertexSet
			*/
			VertexSet vertexSet;

			/*	Renderer
			*/
			//renderer::Quad renderer;
		};
}