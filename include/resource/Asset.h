/*
**
**	Asset.
**
		32-bit and 64 bit share the same set of assets
		need to unify variable length data type within assets

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../resource/XImage.h"
#include "../resource/XModel3D.h"


/*
 !	Asset inner kit
 */
namespace fce::inner::asset
{
	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			LOAD,
			SAVE,
			UNLOAD,
			REMOVE,
			RENAME,

			UNLOAD_POINTER,

			SYNC_ADD,
			SYNC_LOAD,
			SYNC_RELOAD,
			SYNC_GET_NAME
		};

	/*	Message
	 */
	class FC_API Message
		:
		public Object
		{
			POLYMORPHIC_POOL( Message,
							  MemoryManagement );

		public:
			/*	EVENT
			*/
			const EVENT e;

			/*	RESULT
			*/
			RESULT result;

		protected:
			/*	Constructor & Destructor
			*/
			~Message() = default;

			Message( const EVENT ) noexcept;
		};

	/*	Delegate handler

			When the job is completed,
			it's called in the main thread
	*/
	using Delegate = fce::Delegate< Void, const Message& >;

	/*	Message async
	 */
	class FC_API Msg_Async
		:
		public Message
		{
			POLYMORPHIC_POOL( Msg_Async,
							  MemoryManagement );

			friend AssetManagement;
			friend Application;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Async() = default;

			Msg_Async( const EVENT,
					   Delegate&& ) noexcept;

		private:
			/*	callback
			*/
			Delegate callback;
		};

	/*	Msg save raw data
	 */
	class FC_API Msg_Save final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Save,
							  MemoryManagement );

		public:
			/*	path
			*/
			const Path path;

			/*	Data
			*/
			Buffer data;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Save() = default;

			Msg_Save( Path&&,
					  Delegate&&,
					  Buffer&& ) noexcept;
		};

	/*	Msg remove
	 */
	class FC_API Msg_Remove final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Remove,
							  MemoryManagement );

			friend AssetManagement;

			/*	Implemented function
			*/
			using Imp = RESULT ( AssetManagement::* )
							   ( const Path& );
		public:
			/*	path
			*/
			const Path path;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Remove() = default;

			Msg_Remove( Path&&,
						Delegate&&,
						Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Msg rename
	 */
	class FC_API Msg_Rename final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Rename,
							  MemoryManagement );

			friend AssetManagement;

			/*	Implemented function
			*/
			using Imp = RESULT ( AssetManagement::* )
							   ( const Path&, const Path& );

		public:
			/*	Name
			*/
			const Path oldName;

			const Path newName;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Rename() = default;

			Msg_Rename( Path&&,
						Path&&,
						Delegate&&,
						Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Msg load
	 */
	class FC_API Msg_Load final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Load,
							  MemoryManagement );

			friend AssetManagement;

			/*	Implemented function
			*/
			using Imp = Handle ( AssetManagement::* )
							   ( const Path& );

		public:
			/*	path
			*/
			const Path path;

			/*	Get asset
			*/
			template< class T >

			T* Get() const
				{
					return static_cast< T* >( asset );
				}

		protected:
			/*	Constructor & Destructor
			*/
			Msg_Load( Path&&,
					  Delegate&&,
					  Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;

			/*	asset
			*/
			Handle asset;
		};

	/*	Msg unload with path
	 */
	class FC_API Msg_Unload final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Unload,
							  MemoryManagement );

			friend AssetManagement;

			/*	Implemented function
			*/
			using Imp = Void ( AssetManagement::* )
							 ( const Path& );

		public:
			/*	path
			*/
			const Path path;

		protected:
			/*	Constructor & Destructor
			*/
			Msg_Unload( Path&&,
						Delegate&&,
						Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Msg unload with pointer
	 */
	class FC_API Msg_Unload_Pointer final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Unload_Pointer,
							  MemoryManagement );

			friend AssetManagement;

			/*	Implemented function
			*/
			using Imp = Void ( AssetManagement::* )
							 ( Handle const );

		protected:
			/*	Constructor & Destructor
			*/
			Msg_Unload_Pointer( Handle const,
								Delegate&&,
								Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;

			/*	asset
			*/
			Handle asset;
		};

	/*	Message sync
	 */
	class FC_API Msg_Sync
		:
		public Message
		{
			POLYMORPHIC_POOL( Msg_Sync,
							  MemoryManagement );

		protected:
			/*	path
			*/
			const Path& path;

			/*	Promise
			*/
			using Promise = std::promise< Handle >;

			/*	Constructor & Destructor
			*/
			~Msg_Sync() = default;

			Msg_Sync( const EVENT,
					  const Path&,
					  Promise* const ) noexcept;

		protected:
			/*	promise
			*/
			Promise* promise;
		};

	/*	Message add sync
	 */
	class FC_API Msg_Sync_Add final
		:
		public Msg_Sync
		{
			POLYMORPHIC_POOL( Msg_Sync_Add,
							  MemoryManagement );

			friend AssetManagement;

		protected:
			/*	Imp
			*/
			using Imp = fce::Delegate< Handle,
									   const Path& >;

			/*	Constructor & Destructor
			*/
			~Msg_Sync_Add() = default;

			Msg_Sync_Add( const Path&,
						  Promise* const,
						  Imp&& ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Message load sync
	 */
	class FC_API Msg_Sync_Load final
		:
		public Msg_Sync
		{
			POLYMORPHIC_POOL( Msg_Sync_Load,
							  MemoryManagement );

			friend AssetManagement;

		protected:
			/*	Imp
			*/
			using Imp = Handle ( AssetManagement::* )
							   ( const Path& );

			/*	Constructor & Destructor
			*/
			~Msg_Sync_Load() = default;

			Msg_Sync_Load( const Path&,
						   Promise* const,
						   Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Message reload sync
	 */
	class FC_API Msg_Sync_Reload final
		:
		public Msg_Sync
		{
			POLYMORPHIC_POOL( Msg_Sync_Reload,
							  MemoryManagement );

			friend AssetManagement;

		protected:
			/*	Imp
			*/
			using Imp = Handle ( AssetManagement::* )
							   ( const Path& );

			/*	Constructor & Destructor
			*/
			~Msg_Sync_Reload() = default;

			Msg_Sync_Reload( const Path&,
							 Promise* const,
							 Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;
		};

	/*	Message get path sync
	 */
	class FC_API Msg_Sync_Get_Path final
		:
		public Message
		{
			POLYMORPHIC_POOL( Msg_Sync_Get_Path,
							  MemoryManagement );

			friend AssetManagement;

		protected:
			/*	Promise
			*/
			using Promise = std::promise< const Path* >;

			/*	Imp
			*/
			using Imp = const Path* ( AssetManagement::* )
									( Handle const );

			/*	Constructor & Destructor
			*/
			~Msg_Sync_Get_Path() = default;

			Msg_Sync_Get_Path( Promise* const,
							   Handle const,
							   Imp ) noexcept;

		private:
			/*	Imp
			*/
			Imp imp;

			/*	asset
			*/
			Handle asset;

			/*	promise
			*/
			Promise* promise;
		};
}



namespace fce
{
	/*
	 !	Asset
	 */
	class FC_API Asset
		{
			/*	Allocated by asset pool
			*/
			ASSET_POOL( AssetManagement,
						Asset, Path );

			/*	EVENT
			*/
			using EVENT = inner::asset::EVENT;

		public:
			/*	Delegate
			*/
			using Delegate = inner::asset::Delegate;

			/*	Asset message
			*/
			using Message = inner::asset::Message;

			using Msg_Async = inner::asset::Msg_Async;

			using Msg_Save = inner::asset::Msg_Save;
			using Msg_Load = inner::asset::Msg_Load;
			using Msg_Unload = inner::asset::Msg_Unload;
			using Msg_Remove = inner::asset::Msg_Remove;
			using Msg_Rename = inner::asset::Msg_Rename;

			using Msg_Unload_Pointer = inner::asset::Msg_Unload_Pointer;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Asset );

			virtual ~Asset() = default;

			Asset() = default;

			/*	Load a dependent asset
			*/
			template< class T >

			static T* LoadAsset( const Path& ) noexcept;

			/*	Add a new asset
			*/
			template< class T,
					  typename ...Args >

			static T* AddAsset( const Path&, Args&&... ) noexcept;
		};
}



namespace fce
{
	/*
	 !	Asset management

		Delegate

			Void ( Message & )
	 */
	class FC_API AssetManagement final
		{
			/*	Friends
			*/
			friend Engine;
			friend Application;

			friend Asset;
			friend RenderPipeline;
			friend StandardMaterial;
			friend Shader::Info;

			friend class Fbx;
			friend inner::graphics::Impl;

			/*	EVENT
			*/
			using EVENT = Asset::EVENT;

			/*	Delegate
			*/
			using Delegate = Asset::Delegate;

			/*	Message
			*/
			using Message = Asset::Message;

			/*	Msg async
			*/
			using Msg_Async = Asset::Msg_Async;

			using Msg_Save = Asset::Msg_Save;
			using Msg_Load = Asset::Msg_Load;
			using Msg_Unload = Asset::Msg_Unload;
			using Msg_Remove = Asset::Msg_Remove;
			using Msg_Rename = Asset::Msg_Rename;
			using Msg_Unload_Pointer = Asset::Msg_Unload_Pointer;

			/*	Msg sync
			*/
			using Msg_Sync = inner::asset::Msg_Sync;

			using Msg_Sync_Add = inner::asset::Msg_Sync_Add;
			using Msg_Sync_Load = inner::asset::Msg_Sync_Load;
			using Msg_Sync_Reload = inner::asset::Msg_Sync_Reload;
			using Msg_Sync_Get_Path = inner::asset::Msg_Sync_Get_Path;

			/*	Asset container
			*/
			template< class T >

			using AssetPool = AssetPool< AssetManagement, T, Path >;

		public:
			/*	Get asset path
			*/
			template< class T >

			[[nodiscard]] const Path* QueryPath( const T* const ) const noexcept;

			/*	Handle asset in memory

					( Reload & Rebuild ) are non-thread-safe
			*/
			template< class T = Buffer >

			T* Load( const Path& ) noexcept;

			template< class T = Buffer >

			T* Reload( const Path& ) noexcept;

			template< class T = Buffer >

			T* Reload( T* const ) noexcept;

			template< class T = Buffer,
					  typename ...Args >

			T* Add( const Path&, Args&&... ) noexcept;

			template< class T,
					  typename ...Args >

			T* Rebuild( T*, Args&&... ) noexcept;

			template< class T = Buffer >

			Void LoadAsync( Path,
							Delegate = nullptr ) noexcept;

			template< class T = Buffer >

			Void UnloadAsync( Path,
							  Delegate = nullptr ) noexcept;

			template< class T >

			Void UnloadAsync( T* const,
							  Delegate = nullptr ) noexcept;

			/*	Handle raw storage of asset
			*/
			Void SaveAsync( Path, Buffer,
							Delegate = nullptr ) noexcept;

			template< class T = Buffer >

			Void RemoveAsync( Path,
							  Delegate = nullptr ) noexcept;

			template< class T = Buffer >

			Void RenameAsync( Path src,
							  Path dst,
							  Delegate = nullptr ) noexcept;

			/*	Reload all
			*/
			template< class T = Buffer >

			Bool ReloadAll() noexcept;

			/*	Load asset ( only for thread )
			*/
			template< class T >

			T* LoadAsset( const Path& ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( AssetManagement );

			~AssetManagement() = default;

			AssetManagement() noexcept;

			/*	Clear assets
			*/
			Void Clear() noexcept;

			/*	Response event
			*/
			RESULT on_Save( Message& ) noexcept;

			RESULT on_Load( Message& ) noexcept;

			RESULT on_Unload( Message& ) noexcept;

			RESULT on_Remove( Message& ) noexcept;

			RESULT on_Rename( Message& ) noexcept;

			RESULT on_Load_Raw_Data( Message& ) noexcept;

			RESULT on_Unload_Pointer( Message& ) noexcept;

			Void on_Sync_Add( Message& ) noexcept;

			Void on_Sync_Load( Message& ) noexcept;

			Void on_Sync_Reload( Message& ) noexcept;

			Void on_Sync_QueryPath( Message& ) noexcept;

			/*	Process message queue
			*/
			static Bool Process( Object& ) noexcept;

			/*	Process asset in a sub thread
			*/
			template< class T,
					  typename ...Args >

			T* AddAsset( const Path&,
						 Args&&... ) noexcept;

			template< class T >

			T* ReloadAsset( const Path& ) noexcept;

			template< class T >

			Void UnloadAsset( const Path& ) noexcept;

			template< class T >

			Void UnloadAsset( const T* const ) noexcept;

			template< class T >

			T* QueryAsset( const Path& ) noexcept;

			template< class T >

			T* QueryAsset( const DWord ) noexcept;

			/*	Implementation
			*/
			template< class T,
					  typename ...Args >

			Handle Add_Imp( const Path&,
							Args&&... ) noexcept;

			template< class T >

			Handle Load_Imp( const Path& ) noexcept;

			template< class T >

			Void Unload_Imp( const Path& ) noexcept;

			template< class T >

			Void Unload_Imp( Handle const ) noexcept;

			template< class T >

			Handle Reload_Imp( const Path& ) noexcept;

			template< class T >

			RESULT Remove_Imp( const Path& ) noexcept;

			template< class T >

			RESULT Rename_Imp( const Path&,
							   const Path& ) noexcept;

			template< class T >

			const Path* QueryPath_Imp( Handle const ) noexcept;

		private:
			/*	JobThread
			*/
			JobThread * const jobThread;

			/*	Common asset containers
			*/
			template< typename T >

			static AssetPool< T > assetPool;
		};

	/*
	 @	Instances
	 */
	template< typename T >

	AssetManagement::AssetPool< T > AssetManagement::assetPool;


	/*
	 @	AssetManagement singleton shortcut
	 */
	extern FC_API AssetManagement& s_AssetMgmt;
}



/**
 !	class Asset
 */
namespace fce
{
	/*
	 @	Load a dependent asset
	 */
	template< class T >

	FC_INLINE T* Asset::LoadAsset( const Path& path ) noexcept
		{
			return s_AssetMgmt.LoadAsset< T >( path );
		}

	/*
	 @	Add a new asset with a unique path
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE T* Asset::AddAsset( const Path& path,
								  Args&& ...args ) noexcept
		{
			return s_AssetMgmt.AddAsset< T >
						(
							path,
							std::forward< Args >( args )...
						);
		}
}



/**
 !	class AssetManagement
 */
namespace fce
{
	/*
	 @	Sync => Get a asset path
	 */
	template< class T >

	const Path* AssetManagement::QueryPath( const T* const asset ) const noexcept
		{
			Msg_Sync_Get_Path::Promise promise;

			auto future = promise.get_future();

			this->jobThread->Post< Msg_Sync_Get_Path >
				(
					&promise, asset,
					&AssetManagement::QueryPath_Imp< T >
				);

			return std::move( future.get() );
		}

	/*
	 @	Sync => Load a asset
	 */
	template< class T >

	T* AssetManagement::Load( const Path& path ) noexcept
		{
			Msg_Sync_Load::Promise promise;

			auto future = promise.get_future();

			this->jobThread->Post< Msg_Sync_Load >
				(
					path, &promise,
					&AssetManagement::Load_Imp< T >
				);

			return static_cast< T* >( future.get() );
		}

	/*
	 @	Sync => Reload a asset

				Non thread safety
	 */
	template< class T >

	T* AssetManagement::Reload( const Path& path ) noexcept
		{
			Msg_Sync_Reload::Promise promise;

			auto future = promise.get_future();

			this->jobThread->Post< Msg_Sync_Reload >
				(
					path, &promise,
					&AssetManagement::Reload_Imp< T >
				);

			return static_cast< T* >( future.get() );
		}

	template< class T >

	T* AssetManagement::Reload( T* const asset ) noexcept
		{
			if( const Path* path{ s_AssetMgmt.QueryPath( asset ) } )
				{
					return this->Reload< T >( *path );
				}

			return nullptr;
		}

	/*
	 @	Sync => Add a new asset
	 */
	template< class T,
			  typename ...Args >

	T* AssetManagement::Add( const Path& path,
							 Args&& ...args ) noexcept
		{
			auto imp = [ &args... ]( const Path& path )
				{
					return s_AssetMgmt.Add_Imp< T >
							(
								path, args...
							);
				};

			Msg_Sync_Add::Promise promise;

			auto future = promise.get_future();

			this->jobThread->Post< Msg_Sync_Add >
				(
					path, &promise, imp
				);

			return static_cast< T* >( future.get() );
		}

	/*
	 @	Sync => Rebuild a asset in its original memory
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE T* AssetManagement::Rebuild( T* asset,
										   Args&& ...args ) noexcept
		{
			return assetPool< T >.Rebuild( asset,
										   std::forward< Args >( args )... );
		}

	/*
	 @	Async => Load a asset
	 */
	template< class T >

	FC_INLINE Void AssetManagement::LoadAsync( Path path,
											   Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Load >
				(
					std::move( path ),
					std::move( callback ),
					&AssetManagement::Load_Imp< T >
				);
		}

	/*
	 @	Async => Unload a asset with path
	 */
	template< class T >

	FC_INLINE Void AssetManagement::UnloadAsync( Path path,
												 Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Unload >
				(
					std::move( path ),
					std::move( callback ),
					Msg_Unload::Imp( &AssetManagement::Unload_Imp< T > )
				);
		}

	/*
	 @	Async => Unload a asset with pointer
	 */
	template< class T >

	FC_INLINE Void AssetManagement::UnloadAsync( T* const asset,
												 Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Unload_Pointer >
				(
					asset,
					std::move( callback ),
					Msg_Unload_Pointer::Imp( &AssetManagement::Unload_Imp< T > )
				);
		}

	/*
	 @	Async => Remove a asset
	 */
	template< class T >

	FC_INLINE Void AssetManagement::RemoveAsync( Path path,
												 Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Remove >
				(
					std::move( path ),
					std::move( callback ),
					&AssetManagement::Remove_Imp< T >
				);
		}

	/*
	 @	Async => ( Rename | Move ) a asset
	 */
	template< class T >

	FC_INLINE Void AssetManagement::RenameAsync( Path oldName,
												 Path newName,
												 Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Rename >
				(
					std::move( oldName ),
					std::move( newName ),
					std::move( callback ),
					&AssetManagement::Rename_Imp< T >
				);
		}

	/*
	 @	Thread => Add a new asset
	 */
	template< class T,
			  typename ...Args >

	FC_INLINE T* AssetManagement::AddAsset( const Path& path,
											Args&& ...args ) noexcept
		{
			return static_cast< T* >
					(
						this->Add_Imp< T >
							(
								path,
								std::forward< Args >( args )...
							)
					);
		}

	/*
	 @	Thread => Load a asset
	 */
	template< class T >

	FC_INLINE T* AssetManagement::LoadAsset( const Path& path ) noexcept
		{
			return static_cast< T* >
					(
						this->Load_Imp< T >( path )
					);
		}

	/*
	 @	Thread => Reload a asset
	 */
	template< class T >

	FC_INLINE T* AssetManagement::ReloadAsset( const Path& path ) noexcept
		{
			return static_cast< T* >
					(
						this->Reload_Imp< T >( path )
					);
		}

	/*
	 @	Thread => Unload a asset by path
	 */
	template< class T >

	FC_INLINE Void AssetManagement::UnloadAsset( const Path& path ) noexcept
		{
			assetPool< T >.Destroy( path );
		}

	/*
	 @	Thread => Unload a asset by pointer
	 */
	template< class T >

	FC_INLINE Void AssetManagement::UnloadAsset( const T* const asset ) noexcept
		{
			assetPool< T >.Destroy( asset );
		}

	/*
	 @	Thread => Query a asset by path
	 */
	template< class T >

	FC_INLINE T* AssetManagement::QueryAsset( const Path& path ) noexcept
		{
			return assetPool< T >.Find( path );
		}

	/*
	 @	Thread => Query a asset by hash
	 */
	template< class T >

	FC_INLINE T* AssetManagement::QueryAsset( const DWord key ) noexcept
		{
			return assetPool< T >.Find( key );
		}

	/*
	 @	Imp => Unload a asset by path
	 */
	template< class T >

	Void AssetManagement::Unload_Imp( const Path& path ) noexcept
		{
			assetPool< T >.Destroy( path );
		}

	/*
	 @	Imp => Unload a asset by pointer
	 */
	template< class T >

	Void AssetManagement::Unload_Imp( Handle const asset ) noexcept
		{
			assetPool< T >.Destroy
				(
					static_cast< T* const >( asset )
				);
		}

	/*
	 @	Imp => Add a new asset
	 */
	template< class T,
			  typename ...Args >

	Handle AssetManagement::Add_Imp( const Path& path,
									 Args&& ...args ) noexcept
		{
			return assetPool< T >.Generate
					(
						path,
						std::forward< Args >( args )...
					);
		}

	/*
	 @	Imp => Load a asset
	 */
	template< class T >

	Handle AssetManagement::Load_Imp( const Path& path ) noexcept
		{
			if( T* asset = assetPool< T >.Find( path ) )
				{
					return asset;
				}
			else
				{
					Buffer buf;

					RESULT res = s_StorageMgmt.Load( path, buf );

					if( res == RESULT::SUCCESS )
						{
							return this->Add_Imp< T >
									(
										path,
										std::move( buf )
									);
						}
				}

			return nullptr;
		}

	/*
	 @	Imp => Reload a asset
	 */
	template< class T >

	Handle AssetManagement::Reload_Imp( const Path& path ) noexcept
		{
			Buffer buf;

			RESULT res = s_StorageMgmt.Load( path, buf );

			if( res == RESULT::SUCCESS )
				{
					if( T* asset = assetPool< T >.Find( path ) )
						{
							return this->Rebuild
									(
										asset,
										std::move( buf )
									);
						}
					else
						{
							return this->Add_Imp< T >
									(
										path,
										std::move( buf )
									);
						}
				}

			return nullptr;
		}

	/*
	 @	Imp => Remove a asset
	 */
	template< class T >

	RESULT AssetManagement::Remove_Imp( const Path& path ) noexcept
		{
			this->UnloadAsset< T >( path );

			return s_StorageMgmt.Remove( path );
		}

	/*
	 @	Imp => Rename a asset
	 */
	template< class T >

	RESULT AssetManagement::Rename_Imp( const Path& src,
										const Path& dst ) noexcept
		{
			auto& pool = assetPool< T >;

			if( pool.Exists( dst ) )
				{
					return RESULT::OBJECT_ALREADY_EXISTS;
				}

			RESULT res = s_StorageMgmt.Rename( src, dst );

			if( res == RESULT::SUCCESS )
				{
					pool.Rename( src, dst );
				}

			return res;
		}

	/*
	 @	Imp => Get a asset path by pointer
	 */
	template< class T >

	const Path* AssetManagement::QueryPath_Imp( Handle const asset ) noexcept
		{
			return assetPool< T >.Query
					(
						static_cast< T* const >( asset )
					);
		}

	/*
	 @	Reload all assets of T
	 */
	template< class T >

	Bool AssetManagement::ReloadAll() noexcept
		{
			for( const auto& p : assetPool< T > )
				{
					if( ! this->Reload< T >( p.first ) )
						{
							return false;
						}
				}

			return true;
		}
}



/**
 !	Asset ( Syntax sugar )
 */
namespace fce
{
	ASSET_SYNTAX_SUGAR_IMPL( Shader );

	ASSET_SYNTAX_SUGAR_IMPL( Texture2 );

	ASSET_SYNTAX_SUGAR_IMPL( RenderPass );

	ASSET_SYNTAX_SUGAR_IMPL( IndexBuffer );

	ASSET_SYNTAX_SUGAR_IMPL( VertexBuffer );

	ASSET_SYNTAX_SUGAR_IMPL( RenderPipeline );
}