/*
**
**	Version management.
**
		Copyright (c) WangFeng since 1999...
*/

#pragma once

#include "Asset.h"


namespace fce
{
	/*
	 !	Version
	 */
	class Version final
		{
			friend VersionManagement;

		public:
			/*	Access
			*/
			DWord crc32;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Version );

			~Version();

			Version();
		};



	/*
	 !	VersionManagement
	 */
	class VersionManagement final
		{
			/*	Friends
			*/
			friend Engine;

		public:
			/*	Version root path
			*/
			const stl::string path;
			const stl::string url;

			/*	
			*/
			Bool Unload( const stl::string& name );

			const Version* Load( const stl::string& name );

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( VersionManagement );

			~VersionManagement();

			VersionManagement();

		private:
			/*	Version container
			*/
			stl::unordered_map< stl::string, Version* > container;
		};


	/*
	 !	VersionManagement singleton shortcut.
	*/
	extern FC_API VersionManagement& s_VersionMgmt;
}