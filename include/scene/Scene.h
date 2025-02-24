/*
**
**	Scene.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "Camera.h"


namespace fce
{
	/*
	 !	Scene
	 */
	class FC_API Scene final
		:
		public Entity
		{
			/*	Friends
			*/
			friend Camera;

			friend Renderer;

			friend SceneNode;

			friend SceneGroup;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Scene, Path );

		public:
			/*	Using inner kit
			*/
			using Node = SceneNode;

			using NodeComponent = SceneNodeComponent;

		public:
			/*	Iterator
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto rend() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] auto rbegin() const noexcept;

			[[nodiscard]] auto& operator [] ( this auto&,
											  const UInt ) NOEXCEPT;

			/*	Count of nodes
			*/
			template< Bool RECURSIVE = false >

			[[nodiscard]] UInt CountNodes() const noexcept;

			/*	Has node
			*/
			template< Bool RECURSIVE = false >

			[[nodiscard]] Bool hasNode( const Node& ) const noexcept;

			template< Bool RECURSIVE = false >

			[[nodiscard]] Bool hasNode( const Node* ) const noexcept;

			 /*	Get node index
			 */
			[[nodiscard]] UInt GetIndex( const Node& ) const NOEXCEPT;

			[[nodiscard]] UInt GetIndex( const Node* const ) const NOEXCEPT;

		public:
			/*
			 @	Query node by Name
			 */
			template< Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto& GetNode( this auto&,
										 const Key& ) NOEXCEPT;

			template< Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto* FindNode( this auto&,
										  const Key& ) noexcept;

			/*
			 @	Query component by Name
			 */
			template< X_component T,
					  Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto& GetNode( this auto&,
										 const Key& ) NOEXCEPT;

			template< X_component T,
					  Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto* FindNode( this auto&,
										  const Key& ) noexcept;

		public:
			/*	Move node ( Syntax sugar )
			*/
			Void MoveUp( Node& ) NOEXCEPT;

			Void MoveDown( Node& ) NOEXCEPT;

			Void MoveEnd( Node& ) NOEXCEPT;

			Void MoveBegin( Node& ) NOEXCEPT;

			Void MoveNode( const Node& dst,
						   		 Node& src ) NOEXCEPT;

			/*	Move node
			*/
			Void MoveUp( Node* const ) NOEXCEPT;

			Void MoveDown( Node* const ) NOEXCEPT;

			Void MoveEnd( Node* const ) NOEXCEPT;

			Void MoveBegin( Node* const ) NOEXCEPT;

			Void MoveNode( const Node* const dst,
								 Node* const src ) NOEXCEPT;

		public:
			/*	Remove node
			*/
			Void RemoveNode( Node& ) NOEXCEPT;

			Void RemoveNode( Node* const ) NOEXCEPT;

			/*	Remove node by NodeComponent
			*/
			Void RemoveNode( NodeComponent& ) NOEXCEPT;

			Void RemoveNode( NodeComponent* const ) NOEXCEPT;

		public:
			/*	Add node
			*/
			template< typename ...Args >

			Node& AddNode( Args&& ...args ) NOEXCEPT

			requires requires{ Node( nullptr,
									 std::forward< Args >( args )... ); };

			/*	New node
			*/
			template< typename ...Args >

			Node* NewNode( Args&& ...args ) noexcept

			requires requires{ Node( nullptr,
									 std::forward< Args >( args )... ); };

		public:
			/*	Add node with component
			*/
			template< X_component T, typename ...Args >

			T& AddNode( Args&&... ) NOEXCEPT;

			/*	New node with component
			*/
			template< X_component T, typename ...Args >

			T* NewNode( Args&&... ) NOEXCEPT;

			template< X_component T, typename ...Args >

			T* NewNode( const Transform&,
						Args&&... ) NOEXCEPT;

			template< X_component T, typename ...Args >

			T* NewNode( const Vector3& pos,
						const Vector3& scale,
						const Quaternion& orien,
						Args&&... ) NOEXCEPT;

		public:
			/*	Clear nodes
			*/
			Void ClearNodes() noexcept;

			/*	Sort nodes
			*/
			Void SortNodes( auto&& compare ) NOEXCEPT;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Travel
			*/
			template< Bool RECURSIVE = false >

			auto Travel( this auto&& self, auto&& func ) NOEXCEPT;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Scene );

			~Scene() noexcept;

			Scene() = default;

			explicit Scene( const Buffer& );

			/*	Push & Erase & Find node
			*/
			Void PushNode( Node* const ) noexcept;

			Void EraseNode( Node* const ) NOEXCEPT;

			auto QueryChild( this auto&,
							 const Node* const ) noexcept;

			/*	Add & Remove camera
			*/
			Void Add( Camera* const ) noexcept;

			Void Remove( Camera* const ) noexcept;

			/*	Add & Remove renderer
			*/
			Void Add( Renderer* const ) noexcept;

			Void Remove( Renderer* const ) noexcept;

			/*	Render
			*/
			Void Render( inner::window::Surface& ) const noexcept;

		protected:
			/*	Archive
			*/
			Void DeserializeNodes( const Buffer& );

			Void SerializeNodes( Buffer& ) const NOEXCEPT;

			/*	Editor mode
			*/
			#ifdef FC_EDITOR

				XML_ARCHIVE_DECLARATION

				Scene( const Xml::Deserialization& ) NOEXCEPT;

				Void operator >> ( Xml::Serialization& ) const NOEXCEPT;

				Void operator << ( const Xml::Deserialization& ) NOEXCEPT;

				Void SerializeNodes( Xml::Serialization& ) const NOEXCEPT;

				Void DeserializeNodes( const Xml::Deserialization& ) NOEXCEPT;

			#endif

		private:
			/*	Node list
			*/
			stl::list< Node* > listNodes;

			/*	Camera array
			*/
			stl::vector< Camera* > arrCameras;

			/*	Renderer array
			*/
			stl::list< Renderer* > listRenderers;

			/*	Scene pool
			*/
			static ObjectPool< Scene, SceneGroup > pool;

			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "Scene";
		};
}



namespace fce
{
	/*
	 !	Scene group
	 */
	class FC_API SceneGroup final
		{
			/*	Friends
			*/
			friend Window;

			friend Scene;

			friend SceneNode;

		public:
			/*	Using inner kit
			*/
			using Node = SceneNode;

			using Delegate = Delegate< Void, Scene* >;

			/*	Attached surface
			*/
			inner::window::Surface& surface;

		public:
			/*	Iterator
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto rend() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] auto rbegin() const noexcept;

			/*	Access
			*/
			[[nodiscard]] UInt Count() const noexcept;

			[[nodiscard]] auto& operator [] ( this auto&,
											  const UInt ) NOEXCEPT;

		public:
			/*	Remove scene
			*/
			Void Clear() noexcept;

			Void Remove( Scene& ) NOEXCEPT;

			Void Remove( const UInt ) NOEXCEPT;

			Void Remove( Scene* const ) NOEXCEPT;

			/*	New & Load scene
			*/
			Scene* New( const Path& ) NOEXCEPT;

			Scene* Load( const Path& ) NOEXCEPT;

			Void LoadAsync( const Path&,
							Delegate = nullptr ) NOEXCEPT;

		public:
			/*	Find the spatial entity by ID component
			*/
			template< class Key >

			Entity* FindEntity( const Key& );

			template< class Key >

			const Entity* FindEntity( const Key& ) const;

			/*	Get scene by node
			*/
			template< Bool RECURSIVE = true >

			[[nodiscard]] Scene& Get( const Node& ) const noexcept;

			template< Bool RECURSIVE = true >

			[[nodiscard]] Scene& Get( const Node* ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( SceneGroup );

			~SceneGroup() = default;

			SceneGroup( inner::window::Surface& ) noexcept;

			/*	Render
			*/
			Void Render() const noexcept;

			/*	Find scene by node
			*/
			[[nodiscard]] Scene* Find( const Node* const ) const noexcept;

		private:
			/*	Scene list
			*/
			stl::vector< Scene* > arrScenes;
		};


	/*
	 @	SceneGroup singleton shortcut
	 */
	extern FC_API SceneGroup& s_SceneGroup;
}


/**
 !	Components
 */
#include "Movement.h"


/**
 !	Inlines
 */
#include "Node.inl"
#include "Scene.inl"