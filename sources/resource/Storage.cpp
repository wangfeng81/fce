/*
**
**	Storage.
**
		待解问题：

			AssetManagement::save 需要修改

			Storage 架构调整，需要 local pack net 三个类，然后包含 std obb exe这些
			GetPath()需要放到 local pack net 类统一调用 等等

		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/Application.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "storage/Std_Storage.cxx"
#elif defined  FC_ANDROID
	#include "storage/Std_Storage.cxx"
	#include "storage/Obb_Storage.cxx"
#elif defined  FC_LINUX
	#include "storage/Std_Storage.cxx"
#elif defined  FC_IOS
	#include "storage/Std_Storage.cxx"
#elif defined  FC_MAC
	#include "storage/Std_Storage.cxx"
#endif


/**
 !	inner::storage
 */
namespace fce::inner::storage
{
	/*
	 @	Message
	 */
	FC_INLINE Message::Message( const EVENT e,
									  Path&& path ) noexcept
		:
			e( e ),
			path( std::move( path ) ),
			result( RESULT::SUCCESS )
		{
		}


	/*
	 @	Msg_Async
	 */
	FC_INLINE Msg_Async::Msg_Async( const EVENT e,
										  Path&& path,
										  Delegate&& callback ) noexcept
		:
			Message( e, std::move( path ) ),
			callback( std::move( callback ) )
		{
		}

	/*
	 @	Msg_Load
	 */
	FC_INLINE Msg_Load::Msg_Load( Path&& path,
								  Delegate&& callback ) noexcept
		:
			Msg_Async( EVENT::LOAD,
					   std::move( path ),
					   std::move( callback ) )
		{
		}

	/*
	 @	Msg_Save
	 */
	FC_INLINE Msg_Save::Msg_Save( Path&& path,
								  Buffer&& data,
								  Delegate&& callback ) noexcept
		:
			Msg_Async( EVENT::SAVE,
					   std::move( path ),
					   std::move( callback ) ),
			data( std::move( data ) )
		{
		}

	/*
	 @	Msg_Copy
	 */
	FC_INLINE Msg_Copy::Msg_Copy( Path&& src,
								  Path&& dst,
								  Delegate&& callback ) noexcept
		:
			Msg_Async( EVENT::COPY,
					   std::move( src ),
					   std::move( callback ) ),
			dstPath( std::move( dst ) )
		{
		}

	/*
	 @	Msg_Rename
	 */
	FC_INLINE Msg_Rename::Msg_Rename( Path&& src,
									  Path&& dst,
									  Delegate&& callback ) noexcept
		:
			Msg_Async( EVENT::RENAME,
					   std::move( src ),
					   std::move( callback ) ),
			dstPath( std::move( dst ) )
		{
		}

	/*
	 @	Msg_Remove
	 */
	FC_INLINE Msg_Remove::Msg_Remove( Path&& path,
									  Delegate&& callback ) noexcept
		:
			Msg_Async( EVENT::REMOVE,
					   std::move( path ),
					   std::move( callback ) )
		{
		}


	/*
	 @	Msg_AsyOp
	 */
	FC_INLINE Msg_AsyOp::Msg_AsyOp( const EVENT e,
										  Path&& path ) noexcept
		:
			Message( e, std::move( path ) ),
			asyObj( this )
		{
		}

	/*
	 @	AsyOp_Load
	 */
	FC_INLINE AsyOp_Load::AsyOp_Load( Path&& path ) noexcept
		:
			Msg_AsyOp( EVENT::ASY_OP_LOAD,
					   std::move( path ) )
		{
		}

	/*
	 @	AsyOp_Save
	 */
	FC_INLINE AsyOp_Save::AsyOp_Save( Path&& path,
									  Buffer&& data ) noexcept
		:
			Msg_AsyOp( EVENT::ASY_OP_SAVE,
					   std::move( path ) ),
			data( std::move( data ) )
		{
		}

	/*
	 @	AsyOp_Copy
	 */
	FC_INLINE AsyOp_Copy::AsyOp_Copy( Path&& src,
									  Path&& dst ) noexcept
		:
			Msg_AsyOp( EVENT::ASY_OP_COPY,
					   std::move( src ) ),
			dstPath( std::move( dst ) )
		{
		}

	/*
	 @	AsyOp_Rename
	 */
	FC_INLINE AsyOp_Rename::AsyOp_Rename( Path&& src,
										  Path&& dst ) noexcept
		:
			Msg_AsyOp( EVENT::ASY_OP_RENAME,
					   std::move( src ) ),
			dstPath( std::move( dst ) )
		{
		}

	/*
	 @	AsyOp_Remove
	 */
	FC_INLINE AsyOp_Remove::AsyOp_Remove( Path&& path ) noexcept
		:
			Msg_AsyOp( EVENT::ASY_OP_REMOVE,
					   std::move( path ) )
		{
		}
}



/**
 !	Storage
 */
namespace fce
{
	/*
	 @	构造
	 */
	Storage::Storage( const Path& workspace )
		:
			workspace( workspace )
		{
		// 这里需要避免 charset 转换
		}


	/*
	 @	Get full path
	 */
	Path Storage::GetPath( const Path& relative ) const
		{
			//if( 先查找打包表内 是否存在，则直接匹配 )
			//	{
			//		stl::unordered_map<const Path&, const Path>;
			//	}

			return this->workspace / relative;
		}
}



/**
 !	StorageManagement
 */
namespace fce
{
	/*
	 @	构造
	 */
	StorageManagement::StorageManagement()
		:
			jobThread( JobThread::Generate( Process ) )
		{
		}


	/*
	 @	检测 是否存在
	 */
	Bool StorageManagement::Exists( const Path& name ) const noexcept
		{
			// 考虑从 Version 获取
			return this->local.Exists( this->local.GetPath( name ) );
		}


	/*
	 @	加载 整个文件 到 Buffer
	 */
	RESULT StorageManagement::Load( const Path& name,
										  Buffer& buf ) const
		{
			// 当前游标 设置为起始
			buf.SeekBeg();

			return this->local.Load( this->local.GetPath( name ), buf );
		}


	/*
	 @	Async callback => Load
	 */
	Void StorageManagement::LoadAsync( Path path,
									   Delegate callback )
		{
			this->jobThread->Post< Msg_Load >
				(
					std::move( path ),
					std::move( callback )
				);
		}

	/*
	 @	Async callback => Save
	 */
	Void StorageManagement::SaveAsync( Path path,
									   Buffer data,
									   Delegate callback )
		{
			this->jobThread->Post< Msg_Save >
				(
					std::move( path ),
					std::move( data ),
					std::move( callback )
				);
		}

	/*
	 @	Async callback => Copy
	 */
	Void StorageManagement::CopyAsync( Path src,
									   Path dst,
									   Delegate callback )
		{
			this->jobThread->Post< Msg_Copy >
				(
					std::move( src ),
					std::move( dst ),
					std::move( callback )
				);
		}

	/*
	 @	Async callback => Rename
	 */
	Void StorageManagement::RenameAsync( Path src,
										 Path dst,
										 Delegate callback )
		{
			this->jobThread->Post< Msg_Rename >
				(
					std::move( src ),
					std::move( dst ),
					std::move( callback )
				);
		}

	/*
	 @	Async callback => Remove
	 */
	Void StorageManagement::RemoveAsync( Path path,
										 Delegate callback )
		{
			this->jobThread->Post< Msg_Remove >
				(
					std::move( path ),
					std::move( callback )
				);
		}


	/*
	 @	Async operation => Load
	 */
	AsyncOperation< Storage::AsyOp_Load >
		
	StorageManagement::LoadAsyOp( Path path )
		{
			auto msg = this->jobThread->Post< AsyOp_Load >
						(
							std::move( path )
						);

			return msg->asyObj;
		}

	/*
	 @	Async operation => Save
	 */
	AsyncOperation< Storage::AsyOp_Save >
		
	StorageManagement::SaveAsyOp( Path path,
								  Buffer data )
		{
			auto msg = this->jobThread->Post< AsyOp_Save >
						(
							std::move( path ),
							std::move( data )
						);

			return msg->asyObj;
		}

	/*
	 @	Async operation => Copy
	 */
	AsyncOperation< Storage::AsyOp_Copy >
		
	StorageManagement::CopyAsyOp( Path src,
								  Path dst )
		{
			auto msg = this->jobThread->Post< AsyOp_Copy >
						(
							std::move( src ),
							std::move( dst )
						);

			return msg->asyObj;
		}

	/*
	 @	Async operation => Rename
	 */
	AsyncOperation< Storage::AsyOp_Rename >
		
	StorageManagement::RenameAsyOp( Path src,
									Path dst )
		{
			auto msg = this->jobThread->Post< AsyOp_Rename >
						(
							std::move( src ),
							std::move( dst )
						);

			return msg->asyObj;
		}

	/*
	 @	Async operation => Remove
	 */
	AsyncOperation< Storage::AsyOp_Remove >
		
	StorageManagement::RemoveAsyOp( Path path )
		{
			auto msg = this->jobThread->Post< AsyOp_Remove >
						(
							std::move( path )
						);

			return msg->asyObj;
		}


	/*
	 @	分发处理 异步请求
	 */
	Bool StorageManagement::Process( Object& obj )
		{
			// 获取 消息
			Message& msg{ static_cast< Message& >( obj ) };

			// 处理 结果
			RESULT& result{ const_cast< RESULT& >( msg.result ) };

			// 分发 消息
			switch( msg.e )
				{
					[[likely]]	case EVENT::LOAD:
									{
										result = s_StorageMgmt.on_Load< Msg_Load >( msg );
									}
									break;

								case EVENT::SAVE:
									{
										result = s_StorageMgmt.on_Save< Msg_Save >( msg );
									}
									break;

								case EVENT::COPY:
									{
										result = s_StorageMgmt.on_Copy< Msg_Copy >( msg );
									}
									break;

								case EVENT::RENAME:
									{
										result = s_StorageMgmt.on_Rename< Msg_Rename >( msg );
									}
									break;

								case EVENT::REMOVE:
									{
										result = s_StorageMgmt.on_Remove< Msg_Remove >( msg );
									}
									break;

								// Async operation
								case EVENT::ASY_OP_LOAD:
									{
										result = s_StorageMgmt.on_Load< AsyOp_Load >( msg );
									}
									break;

								case EVENT::ASY_OP_SAVE:
									{
										result = s_StorageMgmt.on_Save< AsyOp_Save >( msg );
									}
									break;

								case EVENT::ASY_OP_COPY:
									{
										result = s_StorageMgmt.on_Copy< AsyOp_Copy >( msg );
									}
									break;

								case EVENT::ASY_OP_RENAME:
									{
										result = s_StorageMgmt.on_Rename< AsyOp_Rename >( msg );
									}
									break;

								case EVENT::ASY_OP_REMOVE:
									{
										result = s_StorageMgmt.on_Remove< AsyOp_Remove >( msg );
									}
									break;

					default: std::unreachable();
				}

			// Async operation 模式
			if( msg.e >= EVENT::ASY_OP_LOAD )
				{
					// 将 msg 通过 promise 传给调用者
					Msg_AsyOp& msgAsyOp
						{
							static_cast< Msg_AsyOp& >( msg )
						};

					AsyncObject& asyObj{ msgAsyOp.asyObj };

					// 设置 进度
					asyObj.SetProgress( 1.0f );

					// 完成 操作
					asyObj.Complete();

					// 保留 msg 由 asyOp 析构时销毁
					return false;
				}
			// Delegate callback 模式
			else if( static_cast< Msg_Async& >( msg ).callback )
				{
					// 将 msg 通过 app 传回主线程
					s_App.Post( Application::EVENT::STORAGE,
								&obj );

					// 保留 msg 由 s_App 回调后销毁
					return false;
				}
			// 当 callback 为空时，如果发生了错误 则抛异常
			else if( result != RESULT::SUCCESS )
				{
					throw result;
				}

			// 线程立即销毁 msg
			return true;
		}


	/*
	 @	On load
	 */
	template< X_object T >

	FC_INLINE RESULT StorageManagement::on_Load( Message& obj )
		{
			T& msg{ static_cast< T& >( obj ) };

			return this->Load( msg.path, msg.data );
		}

	/*
	 @	On save
	 */
	template< X_object T >

	FC_INLINE RESULT StorageManagement::on_Save( Message& obj )
		{
			T& msg{ static_cast< T& >( obj ) };

			return this->Save( msg.path, msg.data );
		}

	/*
	 @	On copy
	 */
	template< X_object T >

	FC_INLINE RESULT StorageManagement::on_Copy( Message& obj )
		{
			T& msg{ static_cast< T& >( obj ) };

			return this->Copy( msg.path, msg.dstPath );
		}

	/*
	 @	On rename
	 */
	template< X_object T >

	FC_INLINE RESULT StorageManagement::on_Rename( Message& obj )
		{
			T& msg{ static_cast< T& >( obj ) };

			return this->Rename( msg.path, msg.dstPath );
		}

	/*
	 @	On remove
	 */
	template< X_object T >

	FC_INLINE RESULT StorageManagement::on_Remove( Message& obj )
		{
			T& msg{ static_cast< T& >( obj ) };

			return this->Remove( msg.path );
		}
}