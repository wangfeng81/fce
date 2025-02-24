/*
**
**	Exe_Storage.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::storage
{
	/*
	 !	Exe_Storage
	 */
	class Exe_Storage final
		:
		public Storage
		{
			/*	Friends
			*/
			friend StorageManagement;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Exe_Storage );

			~Exe_Storage() override = default;

			Exe_Storage();
		};


	/*
	 @	Pack storage implementation
	 */
	using Pack = Exe_Storage;


	/*
	 @	Constructor
	 */
	FC_INLINE Exe_Storage::Exe_Storage()
		:
			Storage( "" )
		{
		}
}