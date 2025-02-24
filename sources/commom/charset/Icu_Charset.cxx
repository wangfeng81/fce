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
	 @	����
	 */
	FC_INLINE Icu_Charset::Icu_Charset() noexcept
		:
			cnvtList{ nullptr }
		{
		}


	/*
	 @	�ر� icu ת�����б�
	 */
	FC_INLINE Void Icu_Charset::Release() noexcept
		{
			// ��ȡ���� �� ����
			UInt8 count
				{
					UInt8( TYPE::MAXIMUM )
				};

			while( count )
				{
					// ��ȡ UConverter
					UConverter* cnvt
						{
							this->cnvtList[ --count ]
						};

					// ��� ��Ϊ�� ��ر�
					if( cnvt )
						{
							ucnv_close( cnvt );
						}
				}
		}


	/*
	 @	�� icu ת�����б�
	 */
	FC_INLINE UErrorCode Icu_Charset::Initialize() noexcept
		{
			// ���� icu ��̬�� API
			if( Load() )
				{
					// ת���� ���ֱ�
					const char* names[]
						{
							"UTF-8",
							"UTF16_PlatformEndian",
							"UTF32_PlatformEndian",

							// ���� ���ֽ�
							ucnv_getDefaultName(),

							// ���ַ�
							sizeof( wchar_t ) == 2
							? "UTF16_PlatformEndian"
							: "UTF32_PlatformEndian"
						};

					// ѭ������ icu ת�����б�
					UInt8 count{ UInt8( TYPE::MAXIMUM ) };

					while( count )
						{
							--count;

							// �� UConverter
							UErrorCode errorCode{ U_ZERO_ERROR };

							this->cnvtList[ count ] = ucnv_open( names[ count ],
																&errorCode );

							// ����Ƿ� open �ɹ�
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
	 @	���� icu �� ��ȡ api
	 */
	FC_INLINE Bool Icu_Charset::Load() noexcept
		{
			// Load icu dynLib
			const DynLib icuLib( icuDllName );

			if( icuLib )
				{
					// ͨ����ȡ ucnv_convertEx ��̽�汾
					stl::string ver( "ucnv_convertEx" );

					ucnv_convertEx = decltype( ucnv_convertEx )
									 ( Query( icuLib, ver ) );

					// ��ȡ icu ����
					ucnv_open = icuLib.Query< decltype( ucnv_open ) >
									   ( "ucnv_open" + ver );

					ucnv_close = icuLib.Query< decltype( ucnv_close ) >
										( "ucnv_close" + ver );

					ucnv_getDefaultName = icuLib.Query< decltype( ucnv_getDefaultName ) >
												 ( "ucnv_getDefaultName" + ver );
				}

			// ����Ƿ� ��ȡ�ɹ�
			return ucnv_open
				&& ucnv_close
				&& ucnv_convertEx
				&& ucnv_getDefaultName;
		}


	/*
	 @	ͨ����ѯһ������ ��ȡ icu�汾
	 */
	Handle Icu_Charset::Query( const DynLib& icuLib,
									 stl::string& ver ) noexcept
		{
			// ���� ԭ��
			if( Handle func{ icuLib.Query( ver ) } )
				{
					return ver = "", func;
				}

			// �汾��̽
			for( Byte c = 56; c < Byte_MAX; ++c )
				{
					stl::string number( '_' + std::to_string( c ) );

					if( auto func{ icuLib.Query( ver + number ) } )
						{
							return ver.assign( std::move( number ) ), func;
						};
				}

			// ����ʧ��
			return nullptr;
		}


	/*
	 @	ת�� �ַ�������
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

			// ת��ʧ�ܷ��� �㳤
			return errorCode <= U_ZERO_ERROR
				 ? dst2 - static_cast< char* >( dst )
				 : 0;
		}
}