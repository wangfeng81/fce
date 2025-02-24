/*
**
**	Scene.
**

		�������̼߳��أ�����s_AssetMgmt

		Copyright (c) WangFeng since 2006...
*/

#include "../../include/resource/Asset.h"


/**
 !	Scene
 */
namespace fce
{
	/*
	 @	����
	 */
	Scene::~Scene() noexcept
		{
			this->ClearNodes();
		}

	/*
	 @	���� Buffer
	 */
	Scene::Scene( const Buffer& buf )
		{
			this->DeserializeNodes( buf );

			this->DeserializeComponents( buf );
		}

	/*
	 @	�����л� root nodes
	 */
	Void Scene::DeserializeNodes( const Buffer& buf )
		{
			UInt32 count;

			buf >> count;

			while( count-- )
				{
					this->NewNode( buf );
				}
		}

	/*
	 @	���л�
	 */
	Buffer& Scene::operator >> ( Buffer& buf ) const NOEXCEPT
		{
			this->SerializeNodes( buf );

			return this->SerializeComponents( buf );
		}
}



namespace fce
{
	/*
	 @	��� node ��
	 */
	Void Scene::ClearNodes() noexcept
		{
			for( Node* const node : this->listNodes )
				{
					Node::pool.Destroy( node );
				}

			this->listNodes.clear();
		}

	/*
	 @	Push node
	 */
	Void Scene::PushNode( Node* const node ) noexcept
		{
			node->parent = nullptr;

			this->listNodes.emplace_back( node );
		}

	/*
	 @	Erase node
	 */
	Void Scene::EraseNode( Node* const node ) NOEXCEPT
		{
			const auto p{ this->QueryChild( node ) };

			FC_ASSERT( p != this->end() );

			this->listNodes.erase( p );
		}

	/*
	 @	�Ƴ� node
	 */
	Void Scene::RemoveNode( Node* const node ) NOEXCEPT
		{
			this->EraseNode( node );

			Node::pool.Destroy( node );
		}

	/*
	 @	��ȡ node λ��
	 */
	UInt Scene::GetIndex( const Node* const node ) const NOEXCEPT
		{
			const auto p{ this->QueryChild( node ) };

			FC_ASSERT( p != this->end() );

			return std::distance( this->begin(), p );
		}

	/*
	 @	Render
	 */
	Void Scene::Render( inner::window::Surface& surface ) const noexcept
		{
			// ��Ҫ���� ���߳�
			for( const Camera* const camera : this->arrCameras )
				{
					camera->Render( this );
				}
		}
}



namespace fce
{
	/*
	 @	Move up
	 */
	Void Scene::MoveUp( Node* const node ) NOEXCEPT
		{
			auto p{ this->QueryChild( node ) };

			FC_ASSERT( p != this->end() );

			if( p != this->begin() )
				{
					auto p2{ p };

					std::swap( *p, *--p2 );
				}
		}

	/*
	 @	Move down
	 */
	Void Scene::MoveDown( Node* const node ) NOEXCEPT
		{
			const auto p{ this->QueryChild( node ) };

			FC_ASSERT( p != this->end() );

			auto p2{ p };

			if( ++p2 != this->end() )
				{
					std::swap( *p, *p2 );
				}
		}

	/*
	 @	Move end
	 */
	Void Scene::MoveEnd( Node* const node ) NOEXCEPT
		{
			this->EraseNode( node );

			this->listNodes.emplace_back( node );
		}

	/*
	 @	Move begin
	 */
	Void Scene::MoveBegin( Node* const node ) NOEXCEPT
		{
			this->EraseNode( node );

			this->listNodes.emplace_front( node );
		}

	/*
	 @	Move node
	 */
	Void Scene::MoveNode( const Node* const dst,
								Node* const src ) NOEXCEPT
		{
			const auto p{ this->QueryChild( dst ) };

			FC_ASSERT( p != this->end() );

			this->EraseNode( src );

			this->listNodes.insert( p, src );
		}
}



namespace fce
{
	/*
	 !	Editor mode
	 */
	#ifdef FC_EDITOR

		/*
		 @	���� xml
		 */
		Scene::Scene( const Xml::Deserialization& des ) NOEXCEPT
			{
				*this << des;
			}

		/*
		 @	Serialize xml
		 */
		Void Scene::operator >> ( Xml::Serialization& ser ) const NOEXCEPT
			{
				this->SerializeComponents( ser );

				this->SerializeNodes( ser );
			}

		/*
		 @	Deserialize xml
		 */
		Void Scene::operator << ( const Xml::Deserialization& des ) NOEXCEPT
			{
				this->DeserializeNodes( des );

				this->DeserializeComponents( des );
			}

		/*
		 @	Serialize nodes xml
		 */
		Void Scene::SerializeNodes( Xml::Serialization& ser ) const NOEXCEPT
			{
				ser.Serialize( this->listNodes, "Node" );
			}

		/*
		 @	Deserialize nodes xml
		 */
		Void Scene::DeserializeNodes( const Xml::Deserialization& des ) NOEXCEPT
			{
				auto f = [ this ]( const Xml::Deserialization& deser,
								   const std::size_t index ) NOEXCEPT
					{
						this->NewNode( deser );
					};

				des.Travel( f, "Node" );
			}

	#endif
}



/**
 !	SceneGroup
 */
namespace fce
{
	/*
	 @	����
	 */
	SceneGroup::SceneGroup( inner::window::Surface& surface ) noexcept
		:
			surface( surface )
		{
		}

	/*
	 @	ͨ�� Node ���� Scene
	 */
	Scene* SceneGroup::Find( const Node* const node ) const noexcept
		{
			for( Scene* const scene : this->arrScenes )
				{
					if( scene->hasNode< false >( node ) )
						{
							return scene;
						}
				}

			return nullptr;
		}

	/*
	 @	Render
	 */
	Void SceneGroup::Render() const noexcept
		{
			for( const Scene* const scene : this->arrScenes )
				{
					scene->Render( this->surface );
				}
		}
}



namespace fce
{
	/*
	 @	��� �����б�
	 */
	Void SceneGroup::Clear() noexcept
		{
			//for( Scene* const scene : this->arrScenes )
			//	{
			//		s_AssetMgmt.UnloadAsync( scene );
			//	}

			this->arrScenes.clear();
		}

	/*
	 @	�Ƴ� Scene
	 */
	Void SceneGroup::Remove( Scene* const scene ) NOEXCEPT
		{
			s_AssetMgmt.UnloadAsync( scene );

			// ���б��Ƴ�
			const auto p
				{
					stl::find( this->arrScenes, scene )
				};

			FC_ASSERT( p != this->end() );

			this->arrScenes.erase( p );
		}
}



namespace fce
{
	/*
	 @	����һ�� �³���
	 */
	Scene* SceneGroup::New( const Path& path ) NOEXCEPT
		{
			Scene* const scene
				{
					s_AssetMgmt.Add< Scene >( path )
				};

			FC_ASSERT( scene );

			return this->arrScenes.emplace_back( scene );
		}

	/*
	 @	���� Scene ͬ��
	 */
	Scene* SceneGroup::Load( const Path& path ) NOEXCEPT
		{
			// �������̼߳��أ�����s_AssetMgmt
			Scene* const scene
				{
					s_AssetMgmt.Load< Scene >( path )
				};

			if( scene )
				{
					this->arrScenes.emplace_back( scene );
				}

			return scene;
		}

	/*
	 @	���� Scene �첽
	 */
	Void SceneGroup::LoadAsync( const Path& path,
								Delegate callback ) NOEXCEPT
		{
			// ��Դ�ص�
			auto f = [ this, callback ]( const Asset::Message& msg ) NOEXCEPT
				{
					const Asset::Msg_Load& msgLoad
						{
							static_cast< const Asset::Msg_Load& >( msg )
						};

					Scene* const scene
						{
							msgLoad.Get< Scene >()
						};

					if( scene )
						{
							this->arrScenes.emplace_back( scene );
						}

					// �û��ص�
					if( callback )
						{
							callback( scene );
						}
				};

			// ���� ����
			s_AssetMgmt.LoadAsync< Scene >( path, f );
		}
}