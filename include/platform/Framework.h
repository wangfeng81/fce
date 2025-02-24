/*
**
**	Framework.
**
		Application entry point

	Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../common/Entity.h"


namespace fce
{
	/*
	 !	Framework
	 */
	class Framework
		:
			public Component
		{
			/*	Friends
			*/
			friend Application;

		public:
			/*	Interfaces
			*/
			virtual Bool on_Low_Memory() NOEXCEPT;

			virtual Bool on_Exception( std::exception_ptr ) NOEXCEPT;

		protected:
			/*	Interfaces
			*/
			virtual Bool on_Quit() NOEXCEPT;

			virtual Void on_Frame() NOEXCEPT;

			virtual Void on_Destroy() NOEXCEPT;

			virtual Bool on_Create( const WChar* const cmdLine ) NOEXCEPT = 0;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Framework );

			~Framework() noexcept override;

			Framework( Application& ) noexcept;

		private:
			/*	Dependent
			*/
			Bool DependentOn( const Component* const ) const noexcept override;
		};


	/*	concept		Derived framework
	 */
	template< class T >

	concept X_framework = std::derived_from< T, Framework >;
}



/**
 !	class Framework
 */
namespace fce
{
	FC_INLINE Void Framework::on_Destroy() NOEXCEPT
		{
		}

	FC_INLINE Void Framework::on_Frame() NOEXCEPT
		{
		}

	FC_INLINE Bool Framework::on_Quit() NOEXCEPT
		{
			return true;
		}

	FC_INLINE Bool Framework::on_Low_Memory() NOEXCEPT
		{
			return false;
		}

	FC_INLINE Bool Framework::on_Exception( std::exception_ptr ) NOEXCEPT
		{
			return true;
		}
}



/*
 @	Inject the framework
 */
#define FC_INJECT_FRAMEWORK( FRAMEWORK )					\
															\
		extern "C" fce::Framework* fce::InjectFramework()	\
			{												\
				return s_App.NewComponent< FRAMEWORK >();	\
			}