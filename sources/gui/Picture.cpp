/*
**
**	Picture.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/gui/Picture.h"


namespace fce::gui
{
	/*
	 @	����
	 */
	Picture::~Picture() noexcept
		{
			//Material::Destroy( renderer.material );
		}

	///*
	// @	���� Buffer
	// */
	//Picture::Picture( Entity& entity,
	//				  const Buffer& buf )
	//	:
	//		NodeComponent( entity )
	//	{
	//		*this << buf;
	//	}

	///*
	// @	���� Path
	// */
	//Picture::Picture( Node& node,
	//				  const Path& path ) NOEXCEPT
	//	:
	//		Picture( node, path, Rect() )
	//	{
	//		material::Texture* texture2
	//			{
	//				(static_cast< material::Texture* >
	//				( this->renderer.material ))->texture
	//			};
	//		this->rcCliped.Set( this->renderer.material->t );
	//	}

	///*
	// @	���� Rect
	// */
	//Picture::Picture( Node& node,
	//				  const Path& path,
	//				  const Rect& rcCliped ) NOEXCEPT
	//	:
	//		Picture
	//			(
	//				node, rcCliped,
	//				Texture2::Load( path )
	//			)
	//	{
	//	}

	///*
	// @	���� Material
	// */
	//Picture::Picture( Node& node,
	//				  const Rect& rcCliped,
	//				  Texture2* const texture2 ) NOEXCEPT
	//	:
	//		Picture
	//			(
	//				node, rcCliped,
	//				Material::Generate< material::Texture >
	//					(
	//						"renderPipeline/Texture2.rpl",
	//						texture2
	//					)
	//			)
	//	{
	//	}

	///*
	// @	���� Material
	// */
	//Picture::Picture( Node& node,
	//				  const Rect& rcCliped,
	//				  material::Texture* const material ) NOEXCEPT
	//	:
	//		NodeComponent( node ),
	//		rcCliped( rcCliped ),
	//		renderer( node, material )
	//	{
	//	}

}