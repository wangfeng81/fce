/*
**
**	Canvas.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../resource/Storage.h"

#include "Node.h"


/**
 !	Canvas inner kit
 */
namespace fce::inner::canvas
{
	/*	Info
	 */
	class FC_API Info final
		{
			/*	Friends
			*/
			friend CanvasManagement;

		public:
			/*	Path
			*/
			Path path;

			/*	Canvas
			*/
			Canvas* canvas;

			/*	Progress
			*/
			Float32 progress;

		protected:
			/*	Constructor & Destructor
			*/
			Info() = default;

			~Info() = default;
		};
}


namespace fce
{
	/*
	 !	Canvas
	 */
	class FC_API Canvas final
		:
		public Entity
		//public ComponentEx
		{
			/*	Friends
			*/
			friend Transform;

			friend CanvasManagement;

			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Canvas, Path );

		public:
			/*	Node
			*/
			using Node = Transform;

			/*	Info
			*/
			using Info = inner::canvas::Info;

		public:
			/*	Iterator
			*/
			auto end() const noexcept;

			auto begin() const noexcept;

			/*	Count of nodes
			*/
			template< Bool RECURSIVE = false >

			UInt NodeCount() const noexcept;

			UInt NodeCount( const Bool ) const noexcept;

			/*	Add spatial node
			*/
			Node* AddPrefab( const Buffer& );

			Node* AddNode( const Buffer& );

			Node* AddNode( const Vector3& pos = Vector3::ZERO,
						   const Vector3& scale = Vector3::ONE,
						   const Quaternion& = Quaternion::IDENTITY );

			/*	Remove spatial node
			*/
			Bool RemoveNode( Node* );

			Bool RemoveNode( Entity* );

			Bool RemoveNode( SpatialComponent* );

			Void ClearNodes();

			/*	Check entity exists
			*/
			Bool Exists( const Entity* ) const;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Canvas );

			~Canvas();

			Canvas() = default;

			Canvas( const Buffer& );

			/*	Initialize
			*/
			Bool Initialize( const Buffer& );

			/*	Add & Remove root entity
			*/
			Void AddRoot( Entity* );

			Bool RemoveRoot( Entity* );

			/*	Exists root entity
			*/
			Bool ExistsRoot( const Entity* ) const;

			/*	Get root
			*/
			static Entity* GetRoot( const Entity* );

			/*	Entities serialize & deserialize
			*/
			Void SerializeEntities( Buffer& ) const;

			Void DeserializeEntities( const Buffer& );

		private:
			/*	Node list
			*/
			stl::list< Node* > listNodes;
		};
}



namespace fce
{
	/*
	 !	Canvas management
	 */
	class FC_API CanvasManagement final
		{
			/*	Friends
			*/
			friend Engine;

			friend Canvas;
			friend Transform;

			/*	Info
			*/
			using Info = Canvas::Info;

		public:
			/*	Delegate
			*/
			using Delegate = Delegate< Void, Canvas* >;

		public:
			/*	Iterator
			*/
			auto end() const;

			auto begin() const;

			/*	Count canvass
			*/
			UInt CountCanvass() const;

			/*	Get canvas by index
			*/
			Canvas* GetCanvas( const UInt );

			const Canvas* GetCanvas( const UInt ) const;

			Canvas& operator [] ( const UInt );

			const Canvas& operator [] ( const UInt ) const;

			/*	Create a new canvas
			*/
			Canvas* CreateCanvas( const Path& );

			/*	Save a canvas
			*/
			RESULT SaveCanvas( const Path&,
							  const Canvas* );

			/*	Load main canvas
			*/
			Canvas* LoadCanvas( const Path& );

			Void LoadCanvas( Path, Delegate );

			/*	Add additional canvas
			*/
			Canvas* AddCanvas( const Path& );

			Void AddCanvas( Path, Delegate );

			/*	Unload canvas
			*/
			Void UnloadCanvas( Canvas* );

			Void UnloadCanvas( const UInt index );

			/*	Find the canvas that the entity belongs to
			*/
			Canvas* FindCanvas( const Entity* );

			const Canvas* FindCanvas( const Entity* ) const;

			/*	Find the spatial entity by ID component
			*/
			template< class Key >

			Entity* FindEntity( const Key& );

			template< class Key >

			const Entity* FindEntity( const Key& ) const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( CanvasManagement );

			~CanvasManagement() = default;

			CanvasManagement() = default;

			/*	Clear canvass
			*/
			Void ClearCanvass();

			/*	Add & Remove list entity
			*/
			Void Add( Entity* );

			Bool Remove( Entity* );

			/*	Get canvas by spatial entity
			*/
			Canvas* GetCanvas( const Entity* ) const;

		private:
			/*	Canvas list
			*/
			stl::vector< Canvas* > listCanvass;

			/*	Entity pool
			*/
			ObjectPool< Entity, CanvasManagement > pool;
		};


	/*
	 @	CanvasManagement singleton shortcut.
	 */
	extern FC_API CanvasManagement& s_CanvasMgmt;
}



/**
 !	class Canvas
 */
namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto Canvas::end() const noexcept
		{
			return this->listNodes.cend();
		}

	FC_INLINE auto Canvas::begin() const noexcept
		{
			return this->listNodes.cbegin();
		}

	/*
	 @	Add root entity
	 */
	FC_INLINE Void Canvas::AddRoot( Entity* root )
		{
			//this->listNodes.emplace_back( root );
		}

	/*
	 @	Check whether the entity exists in the canvas
	 */
	FC_INLINE Bool Canvas::Exists( const Entity* entity ) const
		{
			return this->ExistsRoot( GetRoot( entity ) );
		}

	/*
	 @	Count of nodes
	 */
	template< Bool RECURSIVE >

	UInt Canvas::NodeCount() const noexcept
		{
			UInt count = this->listNodes.size();

			if constexpr( RECURSIVE )
				{
					for( const Node* child : this->listNodes )
						{
							count += child->ChildCount< true >();
						}
				}

			return count;
		}

	FC_INLINE UInt Canvas::NodeCount( const Bool recursive ) const noexcept
		{
			return recursive ? this->NodeCount< true >()
							 : this->NodeCount< false >();
		}
}



/**
 !	class CanvasManagement
 */
namespace fce
{
	/*
	 @	Iterator
	 */
	FC_INLINE auto CanvasManagement::end() const
		{
			return this->listCanvass.cend();
		}

	FC_INLINE auto CanvasManagement::begin() const
		{
			return this->listCanvass.cbegin();
		}

	/*
	 @	Count of canvass
	 */
	FC_INLINE UInt CanvasManagement::CountCanvass() const
		{
			return this->listCanvass.size();
		}

	/*
	 @	Add root entity
	 */
	FC_INLINE Void CanvasManagement::Add( Entity* entity )
		{
			this->listCanvass.front()->AddRoot( entity );
		};

	/*
	 @	Unload canvas by index
	 */
	FC_INLINE Void CanvasManagement::UnloadCanvas( const UInt index )
		{
			this->UnloadCanvas( this->listCanvass[ index ] );
		}

	/*
	 @	Get canvas by index
	 */
	FC_INLINE Canvas* CanvasManagement::GetCanvas( const UInt idx )
		{
			return this->listCanvass[ idx ];
		}

	FC_INLINE const Canvas* CanvasManagement::GetCanvas( const UInt idx ) const
		{
			return this->listCanvass[ idx ];
		}

	FC_INLINE Canvas& CanvasManagement::operator [] ( const UInt idx )
		{
			return *this->listCanvass[ idx ];
		}

	FC_INLINE const Canvas& CanvasManagement::operator [] ( const UInt idx ) const
		{
			return *this->listCanvass[ idx ];
		}

	/*
	 @	Find the canvas which the entity belongs to
	 */
	FC_INLINE Canvas* CanvasManagement::FindCanvas( const Entity* entity )
		{
			return this->GetCanvas( entity );
		}

	FC_INLINE const Canvas* CanvasManagement::FindCanvas( const Entity* entity ) const
		{
			return this->GetCanvas( entity );
		}

	/*
	 @	Find spatial entity by ID component
	 */
	template< class Key >

	FC_INLINE Entity* CanvasManagement::FindEntity( const Key& key )
		{
			return ID< Key >::Find( key );
		}

	template< class Key >

	FC_INLINE const Entity* CanvasManagement::FindEntity( const Key& key ) const
		{
			return ID< Key >::Find( key );
		}
}