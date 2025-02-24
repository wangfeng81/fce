/*
**
**	DynLib.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/platform/DynLib.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_DynLib.cxx"
#elif defined  FC_ANDROID
	#include "linux/Linux_DynLib.cxx"
#elif defined  FC_LINUX
	#include "linux/Linux_DynLib.cxx"
#elif defined  FC_IOS
	#include "linux/Linux_DynLib.cxx"
#elif defined  FC_MAC
	#include "linux/Linux_DynLib.cxx"
#endif


namespace fce
{
	/*
	 @	����	handle
	 */
	DynLib::DynLib( const auto handle ) noexcept
		:
			impl( handle )
		{
		}

	/*
	 @	����	dllName
	 */
	DynLib::DynLib( const Char* const dllName ) noexcept
		:
			DynLib( Load( dllName ) )
		{
		}

	DynLib::DynLib( const stl::string& dllName ) noexcept
		:
			DynLib( dllName.c_str() )
		{
		}


	/*
	 @	ֱ�Ӳ�ѯ function
	 */
	Handle DynLib::Query( const Char* const dllName,
						  const Char* const funcName ) noexcept
		{
			FC_ASSERT( funcName );

			// �ȼ��� DynLib
			if( const auto handle{ Load( dllName ) } )
				{
					// ��ѯ function
					return Impl::Query( handle, funcName );
				}

			return nullptr;
		}
}