/*
**
**	Win_DynLib.
**
		Windows platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::win
{
	/*
	 !	Win DynLib
	 */
	class Win_DynLib final
		{
			friend DynLib;

		public:
			/*	Get HMODULE
			*/
			[[nodiscard]] operator const HMODULE() const noexcept;

			[[nodiscard]] const HMODULE& GetHandle() const noexcept;

			/*	Query function
			*/
			[[nodiscard]] static auto Query( const HMODULE,
												   LPCSTR ) noexcept;

			/*	Load & Unload
			*/
			static BOOL Unload( HMODULE ) noexcept;

			static HMODULE Load( LPCSTR ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Win_DynLib );

			~Win_DynLib() = default;

			Win_DynLib( const HMODULE ) noexcept;

		private:
			/*	Library handle
			*/
			HMODULE hModule;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::dynLib
{
	/*
	 @	DynLib win impl
	 */
	using Impl = win::Win_DynLib;
}



/*
 !	Implementation
 */
namespace fce::inner::win
{
	/*
	 @	Get HMODULE
	 */
	FC_INLINE Win_DynLib::operator const HMODULE() const noexcept
		{
			return this->hModule;
		}

	/*
	 @	Get HMODULE
	 */
	FC_INLINE const HMODULE& Win_DynLib::GetHandle() const noexcept
		{
			return this->hModule;
		}

	/*
	 @	Unload library
	 */
	FC_INLINE BOOL Win_DynLib::Unload( HMODULE hModule ) noexcept
		{
			return FreeLibrary( hModule );
		}

	/*
	 @	Load library
	 */
	FC_INLINE HMODULE Win_DynLib::Load( LPCSTR dllName ) noexcept
		{
			return LoadLibraryA( dllName );
		}

	/*
	 @	Query function directly
	 */
	FC_INLINE auto Win_DynLib::Query( const HMODULE hModule,
											LPCSTR funcName ) noexcept
		{
			return GetProcAddress( hModule, funcName );
		}
}