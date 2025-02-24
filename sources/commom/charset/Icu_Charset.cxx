/*
**
**	Icu_Charset.
**
		ICU implementation

	Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::charset
{
	/*
	 !	icu dll names
	 */
	const char* const icuDllName =

		#ifdef FC_WINDOWS
			"icu.dll"
		#elif defined  FC_ANDROID
			"/system/lib/libicuuc.so"
		#elif defined  FC_LINUX
			"/system/lib/libicuuc.so"
		#elif defined  FC_IOS
			"libicuuc.so"
		#elif defined  FC_MAC
			"libicuuc.so"
		#else
			"icu"
		#endif
	;

	/*
	 @	ucnv_close
	 */
	void ( * ucnv_close )( UConverter* );

	/*
	 @	ucnv_getDefaultName
	 */
	const char * ( * ucnv_getDefaultName )( void );

	/*
	 @	ucnv_open
	 */
	UConverter * ( * ucnv_open )( const char*, UErrorCode* );

	/*
	 @	ucnv_convertEx
	 */
	void ( * ucnv_convertEx )( UConverter*,  UConverter*,
									 char**, const char*,
							   const char**, const char*,
							   char16_t*,		   char16_t**,
							   char16_t**,	 const char16_t*,
							   Int8, Int8,   UErrorCode* );
}



namespace fce::inner::charset
{
	/*
	 @	构造
	 */
	FC_INLINE Icu_Charset::Icu_Charset() noexcept
		:
			cnvtList{ nullptr }
		{
		}


	/*
	 @	关闭 icu 转换器列表
	 */
	FC_INLINE Void Icu_Charset::Release() noexcept
		{
			// 获取数量 并 遍历
			UInt8 count
				{
					UInt8( TYPE::MAXIMUM )
				};

			while( count )
				{
					// 获取 UConverter
					UConverter* cnvt
						{
							this->cnvtList[ --count ]
						};

					// 如果 不为空 则关闭
					if( cnvt )
						{
							ucnv_close( cnvt );
						}
				}
		}


	/*
	 @	打开 icu 转换器列表
	 */
	FC_INLINE UErrorCode Icu_Charset::Initialize() noexcept
		{
			// 加载 icu 动态库 API
			if( Load() )
				{
					// 转换器 名字表
					const char* names[]
						{
							"UTF-8",
							"UTF16_PlatformEndian",
							"UTF32_PlatformEndian",

							// 本地 多字节
							ucnv_getDefaultName(),

							// 宽字符
							sizeof( wchar_t ) == 2
							? "UTF16_PlatformEndian"
							: "UTF32_PlatformEndian"
						};

					// 循环开启 icu 转换器列表
					UInt8 count{ UInt8( TYPE::MAXIMUM ) };

					while( count )
						{
							--count;

							// 打开 UConverter
							UErrorCode errorCode{ U_ZERO_ERROR };

							this->cnvtList[ count ] = ucnv_open( names[ count ],
																&errorCode );

							// 检测是否 open 成功
							if( errorCode > U_ZERO_ERROR )
								{
									return errorCode;
								}
						}

					return U_ZERO_ERROR;
				}

			return UErrorCode( DWord_MAX );
		}


	/*
	 @	加载 icu 库 获取 api
	 */
	FC_INLINE Bool Icu_Charset::Load() noexcept
		{
			// Load icu dynLib
			const DynLib icuLib( icuDllName );

			if( icuLib )
				{
					// 通过获取 ucnv_convertEx 试探版本
					stl::string ver( "ucnv_convertEx" );

					ucnv_convertEx = decltype( ucnv_convertEx )
									 ( Query( icuLib, ver ) );

					// 获取 icu 函数
					ucnv_open = icuLib.Query< decltype( ucnv_open ) >
									   ( "ucnv_open" + ver );

					ucnv_close = icuLib.Query< decltype( ucnv_close ) >
										( "ucnv_close" + ver );

					ucnv_getDefaultName = icuLib.Query< decltype( ucnv_getDefaultName ) >
												 ( "ucnv_getDefaultName" + ver );
				}

			// 检测是否 获取成功
			return ucnv_open
				&& ucnv_close
				&& ucnv_convertEx
				&& ucnv_getDefaultName;
		}


	/*
	 @	通过查询一个函数 获取 icu版本
	 */
	Handle Icu_Charset::Query( const DynLib& icuLib,
									 stl::string& ver ) noexcept
		{
			// 先试 原名
			if( Handle func{ icuLib.Query( ver ) } )
				{
					return ver = "", func;
				}

			// 版本试探
			for( Byte c = 56; c < Byte_MAX; ++c )
				{
					stl::string number( '_' + std::to_string( c ) );

					if( auto func{ icuLib.Query( ver + number ) } )
						{
							return ver.assign( std::move( number ) ), func;
						};
				}

			// 查找失败
			return nullptr;
		}


	/*
	 @	转换 字符串编码
	 */
	UInt Icu_Charset::Convert( UConverter* const srcCnvt,
							   UConverter* const dstCnvt,
							   const Void* const src,
							   const Void* const srcLimit,
									 Void* const dst,
									 Void* const dstLimit )
		{
			const char* src2{ ( char* )src };
				  char* dst2{ ( char* )dst };

			UErrorCode errorCode{ U_ZERO_ERROR };

			ucnv_convertEx( dstCnvt, srcCnvt,
							&dst2, ( char* )dstLimit,
							&src2, ( char* )srcLimit,
							NULL, NULL, NULL, NULL,
							true, true, &errorCode );

			// 转换失败返回 零长
			return errorCode <= U_ZERO_ERROR
				 ? dst2 - static_cast< char* >( dst )
				 : 0;
		}
}