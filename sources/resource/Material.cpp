/*
**
**	Material.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/resource/Asset.h"


namespace fce
{
	/*
	 @	���� Buffer
	 */
	Material::Material( const Buffer& buf )
		:
			Material( buf.Get< Path >() )
		{
		}

	/*
	 @	���� copy
	 */
	Material::Material( const Material& lv ) noexcept
		:
			renderPipeline( lv.renderPipeline )
		{
		}

	/*
	 @	���� Path
	 */
	Material::Material( const Path& rpl ) NOEXCEPT
		:
			Material( RenderPipeline::Load( rpl ) )
		{
		}

	/*
	 @	���� RenderPipeline
	 */
	Material::Material( RenderPipeline* const rpl ) NOEXCEPT
		:
			renderPipeline( rpl )
		{
			FC_ASSERT( rpl );
		}

	/*
	 @	Ӧ�� �� surface
	 */
	Void Material::Apply( WindowSurface& surface ) noexcept
		{
			surface.Bind( this->renderPipeline );
		}

	/*
	 @	����
	 */
	Material* Material::Reflect( const Buffer& buf )
		{
			return reflection.Reflect( buf.Get< DWord >(),
									   buf );
		}
}



namespace fce
{
	/*
	 @	���� RenderPipeline
	 */
	MaterialEx::MaterialEx( RenderPipeline* const rpl ) NOEXCEPT
		:
			Material( rpl ),
			descriptorSet( rpl->Allocate< DescriptorSet >() )
		{
			FC_ASSERT( this->descriptorSet );
		}

	/*
	 @	���� copy
	 */
	MaterialEx::MaterialEx( const MaterialEx& lv ) noexcept
		:
			Material( lv ),
			descriptorSet( lv.descriptorSet )
		{
		}

	/*
	 @	Ӧ�� �� surface
	 */
	Void MaterialEx::Apply( WindowSurface& surface ) noexcept
		{
			Material::Apply( surface );

			surface.Bind( this->descriptorSet );
		}
}



namespace fce::material
{
	/*
	 @	���� copy
	 */
	Texture::Texture( const Texture& lv ) noexcept
		:
			Texture( lv.renderPipeline,
					 lv.texture )
		{
		}

	/*
	 @	���� Path
	 */
	Texture::Texture( const Path& rpl,
					  const Path& texture2 ) NOEXCEPT
		:
			Texture( RenderPipeline::Load( rpl ),
					 Texture2::Load( texture2 ) )
		{
		}

	/*
	 @	���� RenderPipeline
	 */
	Texture::Texture( RenderPipeline* const rpl,
					  Texture2* const texture2 ) NOEXCEPT
		:
			MaterialEx( rpl ),
			texture( texture2 )
		{
			FC_ASSERT( texture2 );

			this->descriptorSet->Update( this->texture );
		}
}



namespace fce::material
{
	/*
	 @	���� copy
	 */
	Mesh::Mesh( const Mesh& lv ) noexcept
		:
			Mesh( lv.renderPipeline,
				  lv.texture )
		{
		}

	/*
	 @	���� Path
	 */
	Mesh::Mesh( const Path& rpl,
				const Path& texture2 ) NOEXCEPT
		:
			Mesh( RenderPipeline::Load( rpl ),
				  Texture2::Load( texture2 ) )
		{
		}

	/*
	 @	���� RenderPipeline
	 */
	Mesh::Mesh( RenderPipeline* const rpl,
				Texture2* const texture2 ) NOEXCEPT
		:
			MaterialEx( rpl ),
			texture( texture2 )
		{
			FC_ASSERT( texture2 );

			this->descriptorSet->Update( this->texture );
		}
}




namespace fce
{
	/*
	 @	����
	 */
	MaterialManagement::~MaterialManagement() noexcept
		{
			for( Material* material : this->container )
				{
					Material::reflection.Destroy( material );
				}
		}


	/*
	 @	Add Material
	 */
	Material* MaterialManagement::Add( const stl::string& fileName )
		{
			// ���ļ�����

			return nullptr;
		}


	/*
	 @	Request Material
	 */
	Material* MaterialManagement::Request( const stl::string& name )
		{
			if( Material* material = this->Get( name ) )
				return material;

			return this->Add( name );
		}


	/*
	 @	Get Material
	 */
	Material* MaterialManagement::Get( const stl::string& name ) const
		{
			//const auto p
			//	{
			//		this->container.find( name )
			//	};

			//if( p != this->container.end() )
			//	{
			//		return p->second;
			//	}

			return nullptr;
		}


	/*
	 @	Remove Material
	 */
	Void MaterialManagement::Remove( const stl::string& name )
		{
			//auto p{ this->container.find( name ) };

			//if( p != this->container.end() )
			//	{
			//		//Material::reflection.Destroy( p->second );

			//		this->container.erase( p );

			//		return;
			//	}

			FC_ASSERT( "The material doesn't exists..." );
		}


	/*
	 @	Destroy Material
	 */
	Void MaterialManagement::Destroy( Material* material )
		{
			for( auto p = this->container.begin();
				 p != this->container.end();
				 ++p )
				{
					if( *p == material )
						{
							this->container.erase( p );

							Material::reflection.Destroy( material );

							return;
						}
				}

			FC_ASSERT( "The material doesn't exists..." );
		}
}