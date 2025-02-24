/*
**
**	Shader.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/resource/Asset.h"


/**
 !	Shader inner kit
 */
namespace fce::inner::shader
{
	/*
	 @	构造
	 */
	Info::Info( const TYPE type,
				const Path& name,
				const stl::string& entry ) noexcept
		:
			type( type ),
			name( name ),
			entry( entry )
		{
		}


	/*
	 @	检测
	 */
	RESULT Info::Check() const noexcept
		{
			// 检测 type
			if( this->type >= TYPE::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			// 检测 name
			else if( ! s_StorageMgmt.Exists( this->name ) )
				{
					return RESULT::OBJECT_NOT_FOUND;
				}
			// 检测 entry
			else if( this->entry.empty() )
				{
					return RESULT::DATA_ERROR;
				}

			return RESULT::SUCCESS;
		}
}


/**
 !	Cross platform
 */
#ifdef FC_VULKAN
	#include "vulkan/VK_Shader.cxx"
#elif defined  FC_DIRECTX
	#include "directx/DX_Shader.cxx"
#elif defined  FC_METAL
	#include "metal/MT_Shader.cxx"
#elif defined  FC_GLES
	#include "gles/ES_Shader.cxx"
#endif


/**
 !	Shader
 */
namespace fce
{
	/*
	 @	析构
	 */
	Shader::~Shader() noexcept
		{
			this->Release();
		}

	/*
	 @	构造
	 */
	Shader::Shader( RESULT& res,
					const Buffer& buf )
		:
			impl()
		{
			res = this->Initialize( buf );
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void Shader::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT Shader::Initialize( const Buffer& buf )
		{
			const auto res
				{
					this->impl.Initialize( buf )
				};

			return Graphics::Convert( res );
		}
}