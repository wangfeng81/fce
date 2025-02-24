/*
**
**	Dynamic link library.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../common/Utility.h"

/**
 !	Cross platform
 */
#ifdef FC_WINDOWS
	#include "windows/Win_DynLib.h"
#elif defined  FC_ANDROID
	#include "linux/Linux_DynLib.h"
#elif defined  FC_LINUX
	#include "linux/Linux_DynLib.h"
#elif defined  FC_IOS
	#include "linux/Linux_DynLib.h"
#elif defined  FC_MAC
	#include "linux/Linux_DynLib.h"
#endif


namespace fce
{
	/*
	 !	DynLib
	 */
	class DynLib final
		{
		public:
			/*	Implementation
			*/
			using Impl = inner::dynLib::Impl;

			Impl impl;

		public:
			/*	Constructor & Destructor
			*/
			~DynLib() = default;

			DynLib( const Char* const dllName ) noexcept;

			DynLib( const stl::string& dllName ) noexcept;

			/*	Get native handle
			*/
			[[nodiscard]] const auto& GetHandle() const noexcept;

			/*	Is valid
			*/
			[[nodiscard]] explicit operator Bool() const noexcept;

			/*	Query function
			*/
			template< typename Func = Handle >

			[[nodiscard]] Func Query( const Char* const funcName ) const noexcept;

			template< typename Func = Handle >

			[[nodiscard]] Func Query( const stl::string& funcName ) const noexcept;

			/*	Query function directly
			*/
			template< typename Func >

			[[nodiscard]] static Func Query( const Char* const dllName,
											 const Char* const funcName ) noexcept;

			[[nodiscard]] static Handle Query( const Char* const dllName,
											   const Char* const funcName ) noexcept;

			template< typename Func = Handle >

			[[nodiscard]] static Func Query( const stl::string& dllName,
											 const stl::string& funcName ) noexcept;

			/*	Unload
			*/
			Void Unload() noexcept;

		private:
			/*	Constructor
			*/
			DynLib( const auto ) noexcept;

			/*	Load
			*/
			static auto Load( const Char* const ) noexcept;
		};
}



/**
 !	class DynLib
 */
namespace fce
{
	/*
	 @	Get native handle
	 */
	FC_INLINE const auto& DynLib::GetHandle() const noexcept
		{
			return this->impl.GetHandle();
		}

	/*
	 @	Check whether the DynLib is valid
	 */
	FC_INLINE DynLib::operator Bool() const noexcept
		{
			return this->GetHandle();
		}

	/*
	 @	Unload library
	 */
	FC_INLINE Void DynLib::Unload() noexcept
		{
			FC_ASSERT( *this );

			Impl::Unload( this->impl );
		}

	/*
	 @	Load library
	 */
	FC_INLINE auto DynLib::Load( const Char* const dllName ) noexcept
		{
			FC_ASSERT( dllName );

			return Impl::Load( dllName );
		}

	/*
	 @	Query function
	 */
	template< typename Func >

	FC_INLINE Func DynLib::Query( const Char* const funcName ) const noexcept
		{
			FC_ASSERT( *this && funcName );

			return reinterpret_cast< Func >
					(
						Impl::Query( this->impl, funcName )
					);
		}

	template< typename Func >

	FC_INLINE Func DynLib::Query( const stl::string& funcName ) const noexcept
		{
			return this->Query< Func >( funcName.c_str() );
		}

	/*
	 @	Query function	directly ( Syntax sugar )
	 */
	template< typename Func >

	FC_INLINE Func DynLib::Query( const Char* const dllName,
								  const Char* const funcName ) noexcept
		{
			return reinterpret_cast< Func >
					(
						Query( dllName, funcName )
					);
		}

	template< typename Func >

	FC_INLINE Func DynLib::Query( const stl::string& dllName,
								  const stl::string& funcName ) noexcept
		{
			return Query< Func >( dllName.c_str(),
								  funcName.c_str() );
		}
}