/*
**
**	Linux_DynLib.
**
		Linux platform implementation

	Copyright (c) WangFeng since 2000...
*/

namespace fce::inner::linux
{
	/*
	 !	Linux DynLib
	 */
	class Linux_DynLib final
		{
			friend DynLib;

		public:
			/*	Get Handle
			*/
			[[nodiscard]] operator const Handle() const noexcept;

			[[nodiscard]] const Handle& GetHandle() const noexcept;

			/*	Query function
			*/
			[[nodiscard]] static auto Query( const Handle,
											 const char* const ) noexcept;

			/*	Load & Unload
			*/
			static int Unload( Handle ) noexcept;

			static Handle Load( const char* const ) noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Linux_DynLib );

			~Linux_DynLib() = default;

			Linux_DynLib( const Handle ) noexcept;

		private:
			/*	Library handle
			*/
			Handle handle;
		};
}



/*
 !	Implementation
 */
namespace fce::inner::dynLib
{
	/*
	 @	DynLib linux impl
	 */
	using Impl = linux::Linux_DynLib;
}



/*
 !	Implementation
 */
namespace fce::inner::linux
{
	/*
	 @	Get Handle
	 */
	FC_INLINE Linux_DynLib::operator const Handle() const noexcept
		{
			return this->handle;
		}

	/*
	 @	Get Handle
	 */
	FC_INLINE const Handle& Linux_DynLib::GetHandle() const noexcept
		{
			return this->handle;
		}

	/*
	 @	Unload library
	 */
	FC_INLINE int Linux_DynLib::Unload( Handle handle ) noexcept
		{
			return dlclose( handle );
		}

	/*
	 @	Load library
	 */
	FC_INLINE Handle Linux_DynLib::Load( const char* const dllName ) noexcept
		{
			return dlopen( dllName, RTLD_NOW );
		}

	/*
	 @	Query function
	 */
	FC_INLINE auto Linux_DynLib::Query( const Handle handle,
										const char* const funcName ) noexcept
		{
			return dlsym( handle, funcName );
		}
}