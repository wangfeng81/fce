/*
**
**	XModel3D.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Mesh.h"
#include "../scene/Scene.h"


/**
 !	XModel3D inner kit
 */
namespace fce::inner::model3d
{
	/*	Types
	 */
	enum struct TYPE
		:
			Byte
		{
			FBX,
			MAX,
			OBJ,
			DAE,

			MAXIMUM
		};
}



namespace fce
{
	/*
	!	XModel3D
	*/
	class FC_API XModel3D final
		{
			/*	Friends
			*/
			friend Engine;

		public:
			/*	TYPE
			*/
			using TYPE = inner::model3d::TYPE;

			/*	Decode
			*/
			Bool Decode( const Path&,
						 Scene::Node* const ) NOEXCEPT;

			Bool Decode( const Buffer&,
						 Scene::Node* const ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( XModel3D );

			~XModel3D() = default;

			XModel3D() = default;

		};


	/*
	 @	XModel3D singleton shortcut
	 */
	extern FC_API XModel3D& s_XModel3D;
}