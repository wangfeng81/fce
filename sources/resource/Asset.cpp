/*
**
**	Asset management.
**
		对于构建异常，需自行释放已分配资源，否则存在内存泄露
		但鉴于一般异常 都是不应该出现的 不合理状况
		并且从 内存池分配的内存 在程序退出时可自行释放
		所以只要保证不崩溃 也是可以的
		避免了大量的异常处理成本 其实并无多少意义

		注意：
		rebuild 最好通过 先新建一个 obj，
		然后通过 移动赋值 进行 重建，
		这样避免 重建失败后，导致坏掉的数据，会崩溃

		reload 不一定调用 rebuild ，
		比如 textrue2d 可能是需要 重新拷贝 image buffer，
		否则会导致 关联错误


		Generate_with_result 缺陷隐患：
		当 obj 构造为 变参模板 时，会错误匹配 RESULT 类型
		导致错误绑定到 Generate_with_result
		asset obj 原则上应该要求 构造 第一个参数为 RESULT


	注意：add_imp 没有判断 是否已经存在资源，可能会和 load 冲突



	目前决定，将依赖资源文件名 放在资源开头位置，
	这样就可以处理 依赖加载，文件读取 和 资源解析 分别两个线程






	1.同一个 asset 的操作需要 线性有序 的进行，才能保证正确
	  而不同 asset 的操作 则可以 并发进行，互相之间不影响

	2.load unload reload remove rename，只有 重复 load reload 有影响
	  通过一个 std::set<std::string> 记录 前一个操作

	  当取出 一个asset操作时，
	  每次load reload 记录 asset 的 name 到 set
	  每次 其他操作 则从set 删除 asset 的 name

	3. 依赖资源 处理存在复杂性
	   考虑 将所有依赖的 资源名字存储在 数据开头
	   在文件加载线程时 预先加载

	4. 增加智能指针 引用计数，这样在需要整理内存时，优先释放计数为 0 的asset
	   当计数为 0 时表示没有在使用

	5. 临时创建的非 磁盘资源，需要考虑 将创建数据放入 磁盘或内存 某个地方缓存，以防丢失重建需要
		或者简单处理，给上层发一个 丢失消息，自行重建

	6. 对于 额外 static 自定义 的资源，需要考虑放入一个表里面 可以 提前释放，考虑通过 模板函数

	7. 临时资源可以将 参数打包存储下来（比如放入 lamble 函数），丢失时可以 用于恢复

	8. Reload 为空时，需要处理

		Copyright (c) WangFeng since 2006...
*/

#include "../../include/platform/Application.h"


/**
 !	inner::asset
 */
namespace fce::inner::asset
{
	/*
	 @	Message
	 */
	Message::Message( const EVENT e ) noexcept
		:
			e( e ),
			result( RESULT::SUCCESS )
		{
		}

	/*
	 @	Msg_Async
	 */
	Msg_Async::Msg_Async( const EVENT e,
						  Delegate&& callback ) noexcept
		:
			Message( e ),
			callback( std::move( callback ) )
		{
		}

	/*
	 @	Msg_Save
	 */
	Msg_Save::Msg_Save( Path&& path,
						Delegate&& callback,
						Buffer&& data ) noexcept
		:
			Msg_Async( EVENT::SAVE,
					   std::move( callback ) ),
			path( std::move( path ) ),
			data( std::move( data ) )
		{
		}

	/*
	 @	Msg_Remove
	 */
	Msg_Remove::Msg_Remove( Path&& path,
							Delegate&& callback,
							Imp imp ) noexcept
		:
			Msg_Async( EVENT::REMOVE,
					   std::move( callback ) ),
			path( std::move( path ) ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Rename
	 */
	Msg_Rename::Msg_Rename( Path&& oldName,
							Path&& newName,
							Delegate&& callback,
							Imp imp ) noexcept
		:
			Msg_Async( EVENT::RENAME,
					   std::move( callback ) ),
			oldName( std::move( oldName ) ),
			newName( std::move( newName ) ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Load
	 */
	Msg_Load::Msg_Load( Path&& path,
						Delegate&& callback,
						Imp imp ) noexcept
		:
			Msg_Async( EVENT::LOAD,
					   std::move( callback ) ),
			path( std::move( path ) ),
			asset( nullptr ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Unload
	 */
	Msg_Unload::Msg_Unload( Path&& path,
							Delegate&& callback,
							Imp imp ) noexcept
		:
			Msg_Async( EVENT::UNLOAD,
					   std::move( callback ) ),
			path( std::move( path ) ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Unload_Pointer
	 */
	Msg_Unload_Pointer::Msg_Unload_Pointer( Handle const asset,
											Delegate&& callback,
											Imp imp ) noexcept
		:
			Msg_Async( EVENT::UNLOAD_POINTER,
					   std::move( callback ) ),
			asset( asset ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Sync
	 */
	Msg_Sync::Msg_Sync( const EVENT e,
						const Path& path,
						Promise* const promise ) noexcept
		:
			Message( e ),
			path( path ),
			promise( promise )
		{
		}

	/*
	 @	Msg_Sync_Add
	 */
	Msg_Sync_Add::Msg_Sync_Add( const Path& path,
								Promise* const promise,
								Imp&& imp ) noexcept
		:
			Msg_Sync( EVENT::SYNC_ADD,
					  path, promise ),
			imp( std::move( imp ) )
		{
		}

	/*
	 @	Msg_Sync_Load
	 */
	Msg_Sync_Load::Msg_Sync_Load( const Path& path,
								  Promise* const promise,
								  Imp imp ) noexcept
		:
			Msg_Sync( EVENT::SYNC_LOAD,
					  path, promise ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Sync_Reload
	 */
	Msg_Sync_Reload::Msg_Sync_Reload( const Path& path,
									  Promise* const promise,
									  Imp imp ) noexcept
		:
			Msg_Sync( EVENT::SYNC_RELOAD,
					  path, promise ),
			imp( imp )
		{
		}

	/*
	 @	Msg_Sync_Get_Path
	 */
	Msg_Sync_Get_Path::Msg_Sync_Get_Path( Promise* const promise,
										  Handle const asset,
										  Imp imp ) noexcept
		:
			Message( EVENT::SYNC_GET_NAME ),
			promise( promise ),
			asset( asset ),
			imp( imp )
		{
		}
}



/**
 !	AssetManagement
 */
namespace fce
{
	/*
	 @	构造
	 */
	AssetManagement::AssetManagement() noexcept
		:
			jobThread( JobThread::Generate( Process ) )
		{
		}


	/*
	 @	异步 => 保存 资源
	 */
	Void AssetManagement::SaveAsync( Path path,
									 Buffer data,
									 Delegate callback ) noexcept
		{
			this->jobThread->Post< Msg_Save >
				(
					std::move( path ),
					std::move( callback ),
					std::move( data )
				);
		}


	/*
	 @	分发处理 异步请求
	 */
	Bool AssetManagement::Process( Object& obj ) noexcept
		{
			// 获取 Asset消息
			Message& msg
				{
					static_cast< Message& >( obj )
				};

			// 处理结果
			RESULT& result{ msg.result };

			// 分发消息
			switch( msg.e )
				{
					[[likely]]	case EVENT::LOAD:
									{
										result = s_AssetMgmt.on_Load( msg );
									}
									break;

								case EVENT::SAVE:
									{
										result = s_AssetMgmt.on_Save( msg );
									}
									break;

								case EVENT::UNLOAD:
									{
										result = s_AssetMgmt.on_Unload( msg );
									}
									break;

								case EVENT::REMOVE:
									{
										result = s_AssetMgmt.on_Remove( msg );
									}
									break;

								case EVENT::RENAME:
									{
										result = s_AssetMgmt.on_Rename( msg );
									}
									break;

								case EVENT::UNLOAD_POINTER:
									{
										result = s_AssetMgmt.on_Unload_Pointer( msg );
									}
									break;

								// 同步操作 直接返回
								case EVENT::SYNC_ADD:
									{
										s_AssetMgmt.on_Sync_Add( msg );
									}
									return true;

								case EVENT::SYNC_LOAD:
									{
										s_AssetMgmt.on_Sync_Load( msg );
									}
									return true;

								case EVENT::SYNC_RELOAD:
									{
										s_AssetMgmt.on_Sync_Reload( msg );
									}
									return true;

								case EVENT::SYNC_GET_NAME:
									{
										s_AssetMgmt.on_Sync_QueryPath( msg );
									}
									return true;
				}

			// 用于 异步函数，msg 通过 post 传回主线程
			if( static_cast< Msg_Async& >( msg ).callback )
				{
					s_App.Post( Application::EVENT::ASSET,
								&obj );

					return false;
				}

			// 无需回执
			return true;
		}


	/*
	 @	On save
	 */
	FC_INLINE RESULT AssetManagement::on_Save( Message& obj ) noexcept
		{
			Msg_Save& msg
				{
					static_cast< Msg_Save& >( obj )
				};

			// 保存 asset 到存储
			return s_StorageMgmt.Save( msg.path, msg.data );
		}


	/*
	 @	On remove
	 */
	FC_INLINE RESULT AssetManagement::on_Remove( Message& obj ) noexcept
		{
			Msg_Remove& msg
				{
					static_cast< Msg_Remove& >( obj )
				};

			// 移除 asset 的存储
			return ( this->*msg.imp )( msg.path );
		}


	/*
	 @	On rename
	 */
	FC_INLINE RESULT AssetManagement::on_Rename( Message& obj ) noexcept
		{
			Msg_Rename& msg
				{
					static_cast< Msg_Rename& >( obj )
				};

			return ( this->*msg.imp )( msg.oldName, msg.newName );
		}


	/*
	 @	On load asset
	 */
	FC_INLINE RESULT AssetManagement::on_Load( Message& obj ) noexcept
		{
			Msg_Load& msg
				{
					static_cast< Msg_Load& >( obj )
				};

			// 加载 asset
			if( msg.asset = ( this->*msg.imp )( msg.path ) )
				{
					return RESULT::SUCCESS;
				}

			return RESULT::FAILURE;
		}


	/*
	 @	On unload asset with path
	 */
	FC_INLINE RESULT AssetManagement::on_Unload( Message& obj ) noexcept
		{
			Msg_Unload& msg
				{
					static_cast< Msg_Unload& >( obj )
				};

			// 卸载 asset
			( this->*msg.imp )( msg.path );

			return RESULT::SUCCESS;
		}


	/*
	 @	On unload asset with pointer
	 */
	FC_INLINE RESULT AssetManagement::on_Unload_Pointer( Message& obj ) noexcept
		{
			Msg_Unload_Pointer& msg
				{
					static_cast< Msg_Unload_Pointer& >( obj )
				};

			// 卸载 asset
			( this->*msg.imp )( msg.asset );

			return RESULT::SUCCESS;
		}


	/*
	 @	sync => On add
	 */
	FC_INLINE Void AssetManagement::on_Sync_Add( Message& obj ) noexcept
		{
			Msg_Sync_Add& msg
				{
					static_cast< Msg_Sync_Add& >( obj )
				};

			// 新建 asset
			Handle asset{ msg.imp( msg.path ) };

			// 设置 promise
			msg.promise->set_value( asset );
		}


	/*
	 @	sync => On load
	 */
	FC_INLINE Void AssetManagement::on_Sync_Load( Message& obj ) noexcept
		{
			Msg_Sync_Load& msg
				{
					static_cast< Msg_Sync_Load& >( obj )
				};

			// 加载 asset
			Handle asset{ ( this->*msg.imp )( msg.path ) };

			// 设置 promise
			msg.promise->set_value( asset );
		}


	/*
	 @	sync => On reload
	 */
	FC_INLINE Void AssetManagement::on_Sync_Reload( Message& obj ) noexcept
		{
			Msg_Sync_Reload& msg
				{
					static_cast< Msg_Sync_Reload& >( obj )
				};

			// 重新加载 asset
			Handle asset{ ( this->*msg.imp )( msg.path ) };

			// 设置 promise
			msg.promise->set_value( asset );
		}


	/*
	 @	sync => On get path
	 */
	FC_INLINE Void AssetManagement::on_Sync_QueryPath( Message& obj ) noexcept
		{
			Msg_Sync_Get_Path& msg
				{
					static_cast< Msg_Sync_Get_Path& >( obj )
				};

			// 查询 名字
			const Path* path{ ( this->*msg.imp )( msg.asset ) };

			// 设置 promise
			msg.promise->set_value( path );
		}


	/*
	 @	同步 => 清空 需要提前释放的资源
	 */
	Void AssetManagement::Clear() noexcept
		{
			assetPool< Scene >.DestructAll();

			// 等待 并清理 图形资源
			s_Graphics.WaitIdle();

			assetPool< RenderPipeline >.DestructAll();
			assetPool< VertexBuffer >.DestructAll();
			assetPool< IndexBuffer >.DestructAll();
			assetPool< RenderPass >.DestructAll();
			assetPool< Texture2 >.DestructAll();
			assetPool< Sampler >.DestructAll();
			assetPool< Shader >.DestructAll();
		}
}