/*
**
**	Component system.
**
		Copyright (c) WangFeng since 2018...
*/

#pragma once

#include "../common/Entity.h"


namespace fce::component
{
	/*
	 !	System
	 */
	template< class T >

	class System
		{
		public:

		protected:
			/*	Constructor
			*/
			System() noexcept
				{
				}

			/*	Destructor
			*/
			virtual ~System() = default;

		private:
			/*	Object
			*/
			T obj;
		};
}



/**
 !	Alias
 */
namespace fce
{
	/*
	 @	Component system alias
	 */
	using ComponentSystem = component::System;
}