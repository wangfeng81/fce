/*
**
**	Std_Storage.
**
		Cpp std implementation

	Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::storage
{
	/*
	 @	����
	 */
	Std_Storage::Std_Storage()
		:
			Storage( L"D:/Workspace/" )
		{
		}


	/*
	 @	��� �Ƿ����
	 */
	Bool Std_Storage::Exists( const Path& path ) const noexcept
		{
			std::error_code ec;

			return std::filesystem::exists( path, ec );
		}


	/*
	 @	��ȡ ���ÿռ��С
	 */
	UInt64 Std_Storage::GetAvailableSpace() const noexcept
		{
			std::error_code ec;

			auto si{ std::filesystem::space( this->workspace, ec ) };

			return ec.default_error_condition().value()
				 ? 0 : si.available;
		}


	/*
	 @	��ȡ �ļ���С
	 */
	UInt64 Std_Storage::GetSize( const Path& path ) const noexcept
		{
			std::error_code ec;

			const UInt64 size
				{
					std::filesystem::file_size( path, ec )
				};

			return ec.default_error_condition().value()
				 ? 0 : size;
		}


	/*
	 @	���� �����ļ� �� Buffer
	 */
	RESULT Std_Storage::Load( const Path& path,
							  Buffer& data ) const
		{
			// ��ȡ�ļ���С�����ֻ���� 4G
			const UInt32 size
				{
					static_cast< UInt32 >
						(
							this->GetSize( path )
						)
				};

			// ���û���ߴ�
			data.Resize( size );

			// ��ȡ�ļ�
			return this->Load( path, data.Data(), size );
		}


	/*
	 @	���� �����ļ� �� Buffer
	 */
	RESULT Std_Storage::Load( const Path& path,
									Void* data,
							  const UInt size ) const
		{
			// �������ļ�
			std::ifstream ifs( path,
							   std::ios::in | std::ios::binary );

			// �Ƿ�򿪳ɹ�
			if( ifs.is_open() )
				{
					// ��ȡ�ļ�
					ifs.read( ( char* )data, size );

					// �Ƿ�ȫ������
					if( ifs.good() && ifs.gcount() == size )
						{
							return RESULT::SUCCESS;
						}
				}
			// ����ļ�������
			else if( ! this->Exists( path ) )
				{
					return RESULT::OBJECT_NOT_FOUND;
				}

			return RESULT::FAILURE;
		}


	/*
	 @	���� ���� �� �洢��
	 */
	RESULT Std_Storage::Save( const Path& path,
							  const Void* data,
							  const UInt size )
		{
			// ������ļ�
			std::ofstream ofs( path,
							   std::ios::out | std::ios::binary );

			// �Ƿ�򿪳ɹ�
			if( ofs.is_open() )
				{
					// д���ļ�
					ofs.write( ( const char* )data, size );

					// �Ƿ�ȫ��д��
					if( ofs.good() && ofs.tellp() == size )
						{
							return RESULT::SUCCESS;
						}
				}

			return RESULT::FAILURE;
		}


	/*
	 @	���� ���ļ��� | �ļ������ݹ飩
	 */
	RESULT Std_Storage::Copy( const Path& src,
							  const Path& dst )
		{
			std::error_code ec;

			std::filesystem::copy( src, dst,
								   std::filesystem::copy_options::update_existing,
								   ec );

			return on_Error( ec );
		}


	/*
	 @	���� | �ƶ� ���ļ��� | �ļ���
	 */
	RESULT Std_Storage::Rename( const Path& src,
								const Path& dst )
		{
			std::error_code ec;

			std::filesystem::rename( src, dst, ec );

			return on_Error( ec );
		}


	/*
	 @	ɾ�� ���ļ��� | �ļ������ݹ飩
	 */
	RESULT Std_Storage::Remove( const Path& path )
		{
			std::error_code ec;

			std::filesystem::remove_all( path, ec );

			return on_Error( ec );
		}


	/*
	 @	���� �ļ��У��ݹ飩
	 */
	RESULT Std_Storage::MakeFolder( const Path& path )
		{
			std::error_code ec;

			std::filesystem::create_directories( path, ec );

			return on_Error( ec );
		}


	/*
	 @	���� ������
	 */
	RESULT Std_Storage::on_Error( const std::error_code& ec ) noexcept
		{
			auto error_cond{ ec.default_error_condition() };

			// ƥ��ת�� error code
			switch( std::errc( error_cond.value() ) )
				{
					[[likely]]		case std::errc( 0 ):
										{
											return RESULT::SUCCESS;
										}

									case std::errc::file_exists:
										{
											return RESULT::OBJECT_ALREADY_EXISTS;
										}

									case std::errc::no_such_file_or_directory:
										{
											return RESULT::OBJECT_NOT_FOUND;
										}

									case std::errc::no_space_on_device:
										{
											return RESULT::NOT_ENOUGH_STORAGE;
										}

					[[unlikely]]	default:
										{
											FC_LOG( "File system error::%s",
													error_cond.message() );
										}
				}

			// δ����ö�� ͳһ FAILURE
			return RESULT::FAILURE;
		}
}