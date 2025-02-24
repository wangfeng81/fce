/*
**
**	Net_Storage.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce::inner::storage
{
	/*
	 !	Net_Storage
	 */
	class Net_Storage final
		:
		public Storage
		{
			/*	Friends
			*/
			friend StorageManagement;

		private:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Net_Storage );

			~Net_Storage() override = default;

			Net_Storage();
		};


	/*
	 @	Net storage implementation
	 */
	using Net = Net_Storage;


	/*
	 @	Constructor
	 */
	FC_INLINE Net_Storage::Net_Storage()
		:
			Storage( "" )
		{
		}
}