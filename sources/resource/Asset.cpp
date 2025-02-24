/*
**
**	Asset management.
**
		���ڹ����쳣���������ͷ��ѷ�����Դ����������ڴ�й¶
		������һ���쳣 ���ǲ�Ӧ�ó��ֵ� ������״��
		���Ҵ� �ڴ�ط�����ڴ� �ڳ����˳�ʱ�������ͷ�
		����ֻҪ��֤������ Ҳ�ǿ��Ե�
		�����˴������쳣����ɱ� ��ʵ���޶�������

		ע�⣺
		rebuild ���ͨ�� ���½�һ�� obj��
		Ȼ��ͨ�� �ƶ���ֵ ���� �ؽ���
		�������� �ؽ�ʧ�ܺ󣬵��»��������ݣ������

		reload ��һ������ rebuild ��
		���� textrue2d ��������Ҫ ���¿��� image buffer��
		����ᵼ�� ��������


		Generate_with_result ȱ��������
		�� obj ����Ϊ ���ģ�� ʱ�������ƥ�� RESULT ����
		���´���󶨵� Generate_with_result
		asset obj ԭ����Ӧ��Ҫ�� ���� ��һ������Ϊ RESULT


	ע�⣺add_imp û���ж� �Ƿ��Ѿ�������Դ�����ܻ�� load ��ͻ



	Ŀǰ��������������Դ�ļ��� ������Դ��ͷλ�ã�
	�����Ϳ��Դ��� �������أ��ļ���ȡ �� ��Դ���� �ֱ������߳�






	1.ͬһ�� asset �Ĳ�����Ҫ �������� �Ľ��У����ܱ�֤��ȷ
	  ����ͬ asset �Ĳ��� ����� �������У�����֮�䲻Ӱ��

	2.load unload reload remove rename��ֻ�� �ظ� load reload ��Ӱ��
	  ͨ��һ�� std::set<std::string> ��¼ ǰһ������

	  ��ȡ�� һ��asset����ʱ��
	  ÿ��load reload ��¼ asset �� name �� set
	  ÿ�� �������� ���set ɾ�� asset �� name

	3. ������Դ ������ڸ�����
	   ���� ������������ ��Դ���ִ洢�� ���ݿ�ͷ
	   ���ļ������߳�ʱ Ԥ�ȼ���

	4. ��������ָ�� ���ü�������������Ҫ�����ڴ�ʱ�������ͷż���Ϊ 0 ��asset
	   ������Ϊ 0 ʱ��ʾû����ʹ��

	5. ��ʱ�����ķ� ������Դ����Ҫ���� ���������ݷ��� ���̻��ڴ� ĳ���ط����棬�Է���ʧ�ؽ���Ҫ
		���߼򵥴������ϲ㷢һ�� ��ʧ��Ϣ�������ؽ�

	6. ���� ���� static �Զ��� ����Դ����Ҫ���Ƿ���һ�������� ���� ��ǰ�ͷţ�����ͨ�� ģ�庯��

	7. ��ʱ��Դ���Խ� ��������洢������������� lamble ����������ʧʱ���� ���ڻָ�

	8. Reload Ϊ��ʱ����Ҫ����

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
	 @	����
	 */
	AssetManagement::AssetManagement() noexcept
		:
			jobThread( JobThread::Generate( Process ) )
		{
		}


	/*
	 @	�첽 => ���� ��Դ
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
	 @	�ַ����� �첽����
	 */
	Bool AssetManagement::Process( Object& obj ) noexcept
		{
			// ��ȡ Asset��Ϣ
			Message& msg
				{
					static_cast< Message& >( obj )
				};

			// ������
			RESULT& result{ msg.result };

			// �ַ���Ϣ
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

								// ͬ������ ֱ�ӷ���
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

			// ���� �첽������msg ͨ�� post �������߳�
			if( static_cast< Msg_Async& >( msg ).callback )
				{
					s_App.Post( Application::EVENT::ASSET,
								&obj );

					return false;
				}

			// �����ִ
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

			// ���� asset ���洢
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

			// �Ƴ� asset �Ĵ洢
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

			// ���� asset
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

			// ж�� asset
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

			// ж�� asset
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

			// �½� asset
			Handle asset{ msg.imp( msg.path ) };

			// ���� promise
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

			// ���� asset
			Handle asset{ ( this->*msg.imp )( msg.path ) };

			// ���� promise
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

			// ���¼��� asset
			Handle asset{ ( this->*msg.imp )( msg.path ) };

			// ���� promise
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

			// ��ѯ ����
			const Path* path{ ( this->*msg.imp )( msg.asset ) };

			// ���� promise
			msg.promise->set_value( path );
		}


	/*
	 @	ͬ�� => ��� ��Ҫ��ǰ�ͷŵ���Դ
	 */
	Void AssetManagement::Clear() noexcept
		{
			assetPool< Scene >.DestructAll();

			// �ȴ� ������ ͼ����Դ
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