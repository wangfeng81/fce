/*
**
**	Shader.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Graphics.h"


/*
 !	Shader inner kit
 */
namespace fce::inner::shader
{
	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			VERTEX,
			CONTROL,
			EVALUATION,
			GEOMETRY,
			FRAGMENT,

			COMPUTE,
			GRAPHICS,
			ALL,

			MAXIMUM,

			PIXEL = FRAGMENT
		};


	/*	Specialization
	 */
	class FC_API Specialization final
		{
		protected:
			/*	data
			*/
			Buffer data;

			/*	Element group
			*/
			stl::forward_list< UInt8 > listElements;
		};


	/*	Info
	 */
	struct FC_API Info final
		{
			/*	TYPE
			*/
			TYPE type;

			/*	Asset name
			*/
			Path name;

			/*	Entry name
			*/
			stl::string entry;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( type ),
							ARCHIVE_UNIT( name ),
							ARCHIVE_UNIT( entry ) )

		public:
			/*	Constructor
			*/
			Info( const TYPE		 = TYPE::MAXIMUM,
				  const Path&		 = "",
				  const stl::string& = "main" ) noexcept;

			/*	Check
			*/
			RESULT Check() const noexcept;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Shader.h"
#elif defined  FC_DIRECTX
	#include "directx/DX_Shader.h"
#elif defined  FC_METAL
	#include "metal/MT_Shader.h"
#elif defined  FC_GLES
	#include "gles/ES_Shader.h"
#endif


namespace fce
{
	/*
	 !	Shader
	 */
	class FC_API Shader final
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Shader, Path );

		public:
			/*	Asset ( Syntax sugar )
			*/
			ASSET_SYNTAX_SUGAR_DECL;

			/*	Using inner kit
			*/
			using Info = inner::shader::Info;

			using TYPE = inner::shader::TYPE;

			using Specialization = inner::shader::
									Specialization;

			/*	Implementation
			*/
			using Impl = inner::shader::Impl;

			Impl impl;

		public:
			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Convert & Combine TYPE
			*/
			CONVERT_IMPL_ENUM_VALUE( TYPE );

			COMBINE_IMPL_ENUM_VALUE( TYPE );

		protected:
			/*	Constructor & Destructor
			*/
			~Shader() noexcept;

			Shader( RESULT&,
					const Buffer& );

			/*	Initialize & Release
			*/
			Void Release() noexcept;

			RESULT Initialize( const Buffer& );
		};
}



/**
 !	class Shader
 */
namespace fce
{
	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& Shader::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}
}