/*
**
**	Obb_Storage.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce::inner::storage
{
	/*
	 !	Obb_Storage
	 */
	class Obb_Storage final
		:
		public Storage
		{
			/*	Friends
			*/
			friend StorageManagement;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Obb_Storage );

			~Obb_Storage() override = default;

			Obb_Storage();
		};


	/*
	 @	Pack storage implementation
	 */
	using Pack = Obb_Storage;


	/*
	 @	Constructor
	 */
	Obb_Storage::Obb_Storage()
		:
			Storage( "" )
		{
		}
}