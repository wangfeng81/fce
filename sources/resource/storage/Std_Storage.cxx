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
	 @	构造
	 */
	Std_Storage::Std_Storage()
		:
			Storage( L"D:/Workspace/" )
		{
		}


	/*
	 @	检测 是否存在
	 */
	Bool Std_Storage::Exists( const Path& path ) const noexcept
		{
			std::error_code ec;

			return std::filesystem::exists( path, ec );
		}


	/*
	 @	获取 可用空间大小
	 */
	UInt64 Std_Storage::GetAvailableSpace() const noexcept
		{
			std::error_code ec;

			auto si{ std::filesystem::space( this->workspace, ec ) };

			return ec.default_error_condition().value()
				 ? 0 : si.available;
		}


	/*
	 @	获取 文件大小
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
	 @	加载 整个文件 到 Buffer
	 */
	RESULT Std_Storage::Load( const Path& path,
							  Buffer& data ) const
		{
			// 获取文件大小，最大只加载 4G
			const UInt32 size
				{
					static_cast< UInt32 >
						(
							this->GetSize( path )
						)
				};

			// 设置缓冲尺寸
			data.Resize( size );

			// 读取文件
			return this->Load( path, data.Data(), size );
		}


	/*
	 @	加载 整个文件 到 Buffer
	 */
	RESULT Std_Storage::Load( const Path& path,
									Void* data,
							  const UInt size ) const
		{
			// 打开输入文件
			std::ifstream ifs( path,
							   std::ios::in | std::ios::binary );

			// 是否打开成功
			if( ifs.is_open() )
				{
					// 读取文件
					ifs.read( ( char* )data, size );

					// 是否全部读入
					if( ifs.good() && ifs.gcount() == size )
						{
							return RESULT::SUCCESS;
						}
				}
			// 如果文件不存在
			else if( ! this->Exists( path ) )
				{
					return RESULT::OBJECT_NOT_FOUND;
				}

			return RESULT::FAILURE;
		}


	/*
	 @	保存 数据 到 存储器
	 */
	RESULT Std_Storage::Save( const Path& path,
							  const Void* data,
							  const UInt size )
		{
			// 打开输出文件
			std::ofstream ofs( path,
							   std::ios::out | std::ios::binary );

			// 是否打开成功
			if( ofs.is_open() )
				{
					// 写入文件
					ofs.write( ( const char* )data, size );

					// 是否全部写入
					if( ofs.good() && ofs.tellp() == size )
						{
							return RESULT::SUCCESS;
						}
				}

			return RESULT::FAILURE;
		}


	/*
	 @	拷贝 （文件夹 | 文件）（递归）
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
	 @	改名 | 移动 （文件夹 | 文件）
	 */
	RESULT Std_Storage::Rename( const Path& src,
								const Path& dst )
		{
			std::error_code ec;

			std::filesystem::rename( src, dst, ec );

			return on_Error( ec );
		}


	/*
	 @	删除 （文件夹 | 文件）（递归）
	 */
	RESULT Std_Storage::Remove( const Path& path )
		{
			std::error_code ec;

			std::filesystem::remove_all( path, ec );

			return on_Error( ec );
		}


	/*
	 @	创建 文件夹（递归）
	 */
	RESULT Std_Storage::MakeFolder( const Path& path )
		{
			std::error_code ec;

			std::filesystem::create_directories( path, ec );

			return on_Error( ec );
		}


	/*
	 @	处理 错误码
	 */
	RESULT Std_Storage::on_Error( const std::error_code& ec ) noexcept
		{
			auto error_cond{ ec.default_error_condition() };

			// 匹配转换 error code
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

			// 未定义枚举 统一 FAILURE
			return RESULT::FAILURE;
		}
}