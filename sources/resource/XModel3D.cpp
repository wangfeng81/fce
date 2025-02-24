/*
**
**	XModel3D.
**
		Copyright (c) WangFeng since 2006...
*/

#include <fbxsdk.h>

#include "../../include/resource/Asset.h"

#include "model3d/Fbx.h"
#include "model3d/Fbx.cxx"


namespace fce
{
	/*
	 !	Load
	 */
	Bool XModel3D::Decode( const Path& path,
						   Scene::Node* const node ) NOEXCEPT
		{
			return Decode( Buffer( path ), node );
		}

	Bool XModel3D::Decode( const Buffer& buf,
						   Scene::Node* const node ) NOEXCEPT
		{
			FC_ASSERT( buf && node );

			return Fbx::instance.Decode( buf, node );
		}
}