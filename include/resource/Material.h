/*
**
**	Material.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../graphics/WindowSurface.h"
#include "../graphics/RenderPipeline.h"


namespace fce
{
	/*
	 !	Material
	 */
	class FC_API Material
		:
		public ObjectEx
		{
			/*	Friends
			*/
			friend Renderer;

			friend MaterialManagement;

		public:
			/*	Apply
			*/
			virtual Void Apply( WindowSurface& ) noexcept;

			/*	Asset ( Syntax sugar )
			*/
			template< std::derived_from< Material > T = Material,
					  typename ...Args >

			static T* Generate( Args&&... );

			static Void Destroy( Material* const );

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( Material );

			/*	Constructor & Destructor
			*/
			~Material() = default;

			Material( const Buffer& );

			Material( const Material& ) noexcept;

			Material( RenderPipeline* const ) NOEXCEPT;

			Material( const Path& ) NOEXCEPT;

			/*	Reflect
			*/
			static Material* Reflect( const Buffer& );

		protected:
			/*	Render pipeline
			*/
			RenderPipeline* renderPipeline;
		};


	/*	concept		Derived Material
	 */
	template< class T >

	concept X_material = std::derived_from< T, Material >;
}



namespace fce
{
	/*
	 !	Material ex
	 */
	class FC_API MaterialEx
		:
		public Material
		{
			/*	Friends
			*/
			friend MaterialManagement;

		public:
			/*	Apply
			*/
			virtual Void Apply( WindowSurface& ) noexcept;

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( MaterialEx );

			/*	Constructor & Destructor
			*/
			~MaterialEx() = default;

			MaterialEx( const Buffer& );

			MaterialEx( const MaterialEx& ) noexcept;

			MaterialEx( RenderPipeline* const ) NOEXCEPT;

		protected:
			/*	Descriptor set
			*/
			DescriptorSet* descriptorSet;
		};
}



namespace fce::material
{
	/*
	 !	Material
	 */
	class FC_API Texture
		:
		public MaterialEx
		{
			/*	Friends
			*/
			friend MaterialManagement;

		public:
			/*	Set & Get Texture
			*/
			auto* GetTexture( this auto& ) noexcept;

			Void SetTexture( Texture2* const ) NOEXCEPT;

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( Texture );

			/*	Constructor & Destructor
			*/
			~Texture() = default;

			Texture( const Buffer& );

			Texture( const Path&,
					 const Path& ) NOEXCEPT;

			Texture( RenderPipeline* const,
					 Texture2* const ) NOEXCEPT;

			Texture( const Texture& ) noexcept;

		protected:
			/*	Texture2
			*/
			Texture2* texture;
		};
}



namespace fce::material
{
	/*
	 !	Mesh
	 */
	class FC_API Mesh
		:
		public MaterialEx
		{
			/*	Friends
			*/
			friend MaterialManagement;

		public:
			/*	Set & Get Texture
			*/
			auto* GetTexture( this auto& ) noexcept;

			Void SetTexture( Texture2* const ) NOEXCEPT;

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( Mesh );

			/*	Constructor & Destructor
			*/
			~Mesh() = default;

			Mesh( const Buffer& );

			Mesh( const Mesh& ) noexcept;

			Mesh( const Path&,
				  const Path& ) NOEXCEPT;

			Mesh( RenderPipeline* const,
				  Texture2* const ) NOEXCEPT;

		protected:
			/*	Texture2
			*/
			Texture2* texture;
		};
}




namespace fce
{
	/*
	 !	StandardMaterial
	 */
	class FC_API StandardMaterial
		:
		public Material
		{
			//REFLECTION( StandardMaterial );

		public:
			/*	Texture2
			*/
			Texture2* texture;

			/*	Use
			*/
			virtual Void Use()
				{
					//this->shaderProgram->Use();

					//if( this->texture )
					//	this->texture->Use();
				}

		protected:
			/*	Constructor & Destructor
			*/
			~StandardMaterial() = default;

			StandardMaterial( const stl::string& textureName );

			StandardMaterial( const Buffer& buf )
				:
					Material( buf )
				{
				}
		};
}



namespace fce
{
	/*
	 !	Material management
	 */
	class FC_API MaterialManagement final
		{
			friend Engine;

		public:
			/*	Iterator
			*/
			[[nodiscard]]

			auto end() const noexcept
				{
					return this->container.cend();
				}

			[[nodiscard]]

			auto begin() const noexcept
				{
					return this->container.cbegin();
				}

			/*	Access
			*/
			[[nodiscard]]

			UInt32 Count() const noexcept
				{
					return static_cast< UInt32 >
							(
								this->container.size()
							);
				}

			/*	Generate
			*/
			template< X_material T = Material,
					  typename ...Args >

			T* Generate( Args&& ...args )
				{
					T* material =
						Material::reflection.Generate< T >(
							std::forward< Args >( args )... );

					this->container.emplace_back( material );

					return material;
				}

			Material* Add( const stl::string& fileName );

			/*	Request
			*/
			Material* Request( const stl::string& name );

			/*	Get
			*/
			Material* Get( const stl::string& name ) const;

			/*	Delete
			*/
			Void Clear()
				{
					this->container.clear();
				}

			Void Remove( const stl::string& name );

			Void Destroy( Material* Material );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( MaterialManagement );

			~MaterialManagement() noexcept;

			MaterialManagement() = default;

		private:
			/*	Material container
			*/
			stl::list< Material* > container;
		};


	/*
	 @	MaterialManagement singleton shortcut
	 */
	extern FC_API MaterialManagement& s_MaterialMgmt;
}



namespace fce
{
	/*
	 @	Generate
	 */
	template< std::derived_from< Material > T,
			  typename ...Args >

	FC_INLINE T* Material::Generate( Args&& ...args )
		{
			return s_MaterialMgmt.Generate< T >
					(
						std::forward< Args >( args )...
					);
		}

	/*
	 @	Destroy
	 */
	FC_INLINE Void Material::Destroy( Material* const material )
		{
			s_MaterialMgmt.Destroy( material );
		}
}



namespace fce::material
{
	/*
	 @	Set & Get Texture
	 */
	FC_INLINE auto* Texture::GetTexture( this auto& self ) noexcept
		{
			return &std::forward_like< decltype( self ) >
					(
						*self.texture
					);
		}

	FC_INLINE Void Texture::SetTexture( Texture2* const texture ) NOEXCEPT
		{
			FC_ASSERT( texture );

			this->texture = texture;
		}
}