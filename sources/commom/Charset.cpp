/*
**
**	Charset.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/platform/DynLib.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "charset/Icu_Charset.cxx"
#elif defined  FC_ANDROID
	#include "charset/Icu_Charset.cxx"
#elif defined  FC_LINUX
	#include "charset/Std_Charset.cxx"
#elif defined  FC_IOS
	#include "charset/Std_Charset.cxx"
#elif defined  FC_MAC
	#include "charset/Std_Charset.cxx"
#endif


namespace fce
{
	/*
	 @	构造
	 */
	Charset::Charset() noexcept
		{
			// 初始化
			const RESULT res
				{
					this->Initialize()
				};

			if( res != RESULT::SUCCESS )
				{
					std::quick_exit( int( res ) );
				}
		}

	/*
	 @	析构
	 */
	Charset::~Charset() noexcept
		{
			this->Release();
		}
}



namespace fce
{
	/*
	 @	释放 ( 原生调用 )
	 */
	FC_INLINE Void Charset::Release() noexcept
		{
			this->impl.Release();
		}

	/*
	 @	初始化 ( 原生调用 )
	 */
	FC_INLINE RESULT Charset::Initialize() noexcept
		{
			return RESULT( this->impl.Initialize() );
		}
}