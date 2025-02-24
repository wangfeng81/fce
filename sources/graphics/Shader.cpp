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
	 @	����
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
	 @	���
	 */
	RESULT Info::Check() const noexcept
		{
			// ��� type
			if( this->type >= TYPE::MAXIMUM )
				{
					return RESULT::DATA_ERROR;
				}
			// ��� name
			else if( ! s_StorageMgmt.Exists( this->name ) )
				{
					return RESULT::OBJECT_NOT_FOUND;
				}
			// ��� entry
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
	 @	����
	 */
	Shader::~Shader() noexcept
		{
			this->Release();
		}

	/*
	 @	����
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
	 @	�ͷ� ( ԭ������ )
	 */
	FC_INLINE Void Shader::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	��ʼ�� ( ԭ������ )
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