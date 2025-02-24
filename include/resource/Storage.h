/*
**
**	Storage.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "storage/Msg_Storage.h"


namespace fce
{
	/*
	 !	Storage
	 */
	class Storage
		{
			/*	Friends
			*/
			friend StorageManagement;

		public:
			/*	EVENT
			*/
			using EVENT = inner::storage
							   ::EVENT;

			/*	Delegate
			*/
			using Delegate = inner::storage
								  ::Delegate;

			/*	Message
			*/
			using Message = inner::storage
								 ::Message;

			using Msg_Async = inner::storage
								   ::Msg_Async;

			using Msg_AsyOp = inner::storage
								   ::Msg_AsyOp;

			/*	Async message
			*/
			using Msg_Load = inner::storage::Msg_Load;
			using Msg_Save = inner::storage::Msg_Save;
			using Msg_Copy = inner::storage::Msg_Copy;
			using Msg_Rename = inner::storage::Msg_Rename;
			using Msg_Remove = inner::storage::Msg_Remove;

			/*	AsyOp message
			*/
			using AsyOp_Load = inner::storage::AsyOp_Load;
			using AsyOp_Save = inner::storage::AsyOp_Save;
			using AsyOp_Copy = inner::storage::AsyOp_Copy;
			using AsyOp_Rename = inner::storage::AsyOp_Rename;
			using AsyOp_Remove = inner::storage::AsyOp_Remove;

		public:
			/*	Get full path
			*/
			[[nodiscard]] Path GetPath( const Path& relative ) const;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Storage );

			virtual ~Storage() = default;

			Storage( const Path& );

		protected:
			/*	Workspace
			*/
			const Path workspace;
		};
}


/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "storage/Std_Storage.h"
	#include "storage/Exe_Storage.h"
#elif defined  FC_ANDROID
	#include "storage/Std_Storage.h"
	#include "storage/Obb_Storage.h"
#elif defined  FC_LINUX
	#include "storage/Std_Storage.h"
#elif defined  FC_IOS
	#include "storage/Std_Storage.h"
#elif defined  FC_MAC
	#include "storage/Std_Storage.h"
#endif

#include "storage/Net_Storage.h"


namespace fce
{
	/*
	 !	Storage management
	 */
	class StorageManagement final
		{
			/*	Friends
			*/
			friend Engine;

			/*	EVENT
			*/
			using EVENT = Storage::EVENT;

			/*	Delegate
			*/
			using Delegate = Storage::Delegate;

			// Message
			using Message = Storage::Message;

			using Msg_Async = Storage::Msg_Async;
			using Msg_AsyOp = Storage::Msg_AsyOp;

			/*	Async message
			*/
			using Msg_Load = Storage::Msg_Load;
			using Msg_Save = Storage::Msg_Save;
			using Msg_Copy = Storage::Msg_Copy;
			using Msg_Rename = Storage::Msg_Rename;
			using Msg_Remove = Storage::Msg_Remove;

			/*	AsyOp message
			*/
			using AsyOp_Load = Storage::AsyOp_Load;
			using AsyOp_Save = Storage::AsyOp_Save;
			using AsyOp_Copy = Storage::AsyOp_Copy;
			using AsyOp_Rename = Storage::AsyOp_Rename;
			using AsyOp_Remove = Storage::AsyOp_Remove;

		public:
			/*	Net storage
			*/
			inner::storage::Net net;

			/*	Pack storage
			*/
			inner::storage::Pack pack;

			/*	Local storage
			*/
			inner::storage::Local local;

		public:
			/*	Load the entire file into the buffer

					The current cursor of the buffer
					will not be modified
			*/
			RESULT Load( const Path&,
							   Buffer& ) const;

			/*	Save the buffer to the storage
			*/
			RESULT Save( const Path&,
						 const Buffer& );

			RESULT Save( const Path&,
						 const Void*,
						 const UInt size );

			/*	Copy folders & files recursively
			*/
			RESULT Copy( const Path& src,
						 const Path& dst );

			/*	Modify path of the folder | file
			*/
			RESULT Rename( const Path& src,
						   const Path& dst );

			/*	Remove folders & files recursively
			*/
			RESULT Remove( const Path& );

			/*	Make folders recursively
			*/
			RESULT MakeFolder( const Path& );

			/*	Async callback

					If the delegate callback is nullptr
					throw an exception when an error occurs
			*/
			Void LoadAsync( Path,
							Delegate );

			Void SaveAsync( Path,
							Buffer,
							Delegate = nullptr );

			Void CopyAsync( Path src,
							Path dst,
							Delegate = nullptr );

			Void RenameAsync( Path src,
							  Path dst,
							  Delegate = nullptr );

			Void RemoveAsync( Path,
							  Delegate = nullptr );

			/*	Async operation
			*/
			[[nodiscard]] AsyncOperation< AsyOp_Load > LoadAsyOp( Path );

			[[nodiscard]] AsyncOperation< AsyOp_Save > SaveAsyOp( Path,
																  Buffer );

			[[nodiscard]] AsyncOperation< AsyOp_Copy > CopyAsyOp( Path src,
																  Path dst );

			[[nodiscard]] AsyncOperation< AsyOp_Rename > RenameAsyOp( Path src,
																	  Path dst );

			[[nodiscard]] AsyncOperation< AsyOp_Remove > RemoveAsyOp( Path );

			/*	Detect whether the folder | file exists
			*/
			[[nodiscard]] Bool Exists( const Path& ) const noexcept;

			/*	Get available space
			*/
			[[nodiscard]] UInt64 GetAvailableSpace() const noexcept;

			/*	Get file size
			*/
			[[nodiscard]] UInt64 GetSize( const Path& ) const noexcept;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( StorageManagement );

			~StorageManagement() = default;

			StorageManagement();

			/*	Response event
			*/
			template< X_object T >

			RESULT on_Load( Message& );

			template< X_object T >

			RESULT on_Save( Message& );

			template< X_object T >

			RESULT on_Copy( Message& );

			template< X_object T >

			RESULT on_Rename( Message& );

			template< X_object T >

			RESULT on_Remove( Message& );

			/*	Process message queue
			*/
			static Bool Process( Object& );

		private:
			/*	JobThread
			*/
			JobThread * const jobThread;
		};


	/*
	 @	StorageManagement singleton shortcut
	 */
	extern FC_API StorageManagement& s_StorageMgmt;
}



/**
 !	class StorageManagement
 */
namespace fce
{
	/*
	 @	Get file size
	 */
	FC_INLINE UInt64 StorageManagement::GetSize( const Path& name ) const noexcept
		{
			return this->local.GetSize( this->local.GetPath( name ) );
		}

	/*
	 @	Get available space
	 */
	FC_INLINE UInt64 StorageManagement::GetAvailableSpace() const noexcept
		{
			return this->local.GetAvailableSpace();
		}

	/*
	 @	Save the buffer to the storage
	 */
	FC_INLINE RESULT StorageManagement::Save( const Path& name,
											  const Buffer& buf )
		{
			return this->Save( name, buf.Data(), buf.Size() );
		}

	/*
	 @	Save
	 */
	FC_INLINE RESULT StorageManagement::Save( const Path& name,
											  const Void* data,
											  const UInt size )
		{
			return this->local.Save( this->local.GetPath( name ),
									 data, size );
		}

	/*
	 @	Copy
	 */
	FC_INLINE RESULT StorageManagement::Copy( const Path& src,
											  const Path& dst )
		{
			return this->local.Copy( this->local.GetPath( src ),
									 this->local.GetPath( dst ) );
		}

	/*
	 @	Rename
	 */
	FC_INLINE RESULT StorageManagement::Rename( const Path& src,
												const Path& dst )
		{
			return this->local.Rename( this->local.GetPath( src ),
									   this->local.GetPath( dst ) );
		}

	/*
	 @	Remove
	 */
	FC_INLINE RESULT StorageManagement::Remove( const Path& name )
		{
			return this->local.Remove( this->local.GetPath( name ) );
		}

	/*
	 @	Make folder
	 */
	FC_INLINE RESULT StorageManagement::MakeFolder( const Path& name )
		{
			return this->local.MakeFolder( this->local.GetPath( name ) );
		}
}



/**
 !	class Buffer load & save ( Syntax sugar )
 */
namespace fce
{
	/*
	 @	Load
	 */
	FC_INLINE RESULT Buffer::Load( const Path& name ) noexcept
		{
			return s_StorageMgmt.Load( name, *this );
		}


	/*
	 @	Save
	 */
	FC_INLINE RESULT Buffer::Save( const Path& name ) const noexcept
		{
			return s_StorageMgmt.Save( name, *this );
		}
}