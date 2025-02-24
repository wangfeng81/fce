/*
**
**	Director.
**
		Copyright (c) WangFeng since 2000...
*/

#include "../../include/common/Director.h"


namespace fce
{
	/*
	 @	Constructor
	 */
	Director::Director()
		:
			stage( nullptr ),
			index( -1 )
		{
		}


	/*
	 @	Switch
	 */
	Void Director::Switch( Stage* next )
		{
			if( this->stage )
				this->stage->Leave();

			if( next )
				next->Enter();

			this->stage = next;
		}


	/*
	 @	Switch
	 */
	Void Director::Switch( DWord index )
		{
			FC_ASSERT_LOG( index < this->listStages.size(),
						   "Invalid stage index...");

			this->Switch( this->listStages[ index ] );

			this->index = index;
		}


	/*
	 @	Get component
	 */
	Stage* Director::GetStage( const std::type_info& ti ) const
		{
			for( Stage* stage : this->listStages )
				{
					if( typeid( *stage ) == ti )
						return stage;
				}

			return nullptr;
		}
}