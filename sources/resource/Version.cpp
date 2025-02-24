/*
**
**	Version management.
**
		Copyright (c) WangFeng since 1999...
*/

#include "../../include/resource/Version.h"


namespace fce
{
	/*
	 @	Destructor
	 */
	Version::~Version()
		{
		}


	/*
	 @	Constructor
	 */
	Version::Version()
		{
		}



	/*
	 @	Destructor
	 */
	VersionManagement::~VersionManagement()
		{
		}


	/*
	 @	Constructor
	 */
	VersionManagement::VersionManagement()
		{
		}


	/*
	 !	Unload Version
	*/
	Bool VersionManagement::Unload( const stl::string& name )
		{

			return false;
		}


	/*
	 !	Load Version
	*/
	const Version* VersionManagement::Load( const stl::string& name )
		{

			return nullptr;
		}
}