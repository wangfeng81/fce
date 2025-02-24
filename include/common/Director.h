/*
**
**	Director.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "Utility.h"


namespace fce
{
	/*
	 !	Stage base
	 */
	class Stage
		{
			friend Director;

		protected:
			/*	Constructor & Destructor
			*/
			Stage() = default;

			virtual ~Stage() = default;

			/*	Enter & Leave
			*/
			virtual Void Enter() {};
			virtual Void Leave() {};
		};


	/*	concept		Derived stage
	 */
	template< class T >

	concept X_stage = std::derived_from< T, Stage >;
}



namespace fce
{
	/*
	 !	Director
	 */
	class Director
		{
		public:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( Director );

			Director();

			/*	Get current stage index
			*/
			DWord GetCurrentIndex() const
				{
					return this->index;
				}

			Stage* GetCurrentStage() const
				{
					return this->stage;
				}

			/*	Get stage
			*/
			template< X_stage T >

			T* GetStage()
				{
					return static_cast< T* >(
							this->GetStage( typeid( T ) ) );
				}

			template< X_stage T >

			const T* GetStage() const
				{
					return static_cast< T* >(
							this->GetStage( typeid( T ) ) );
				}

			/*	Switch the stage
			*/
			Void Switch( Stage* next );

			Void Switch( DWord index );

		protected:
			/*	Get component
			*/
			Stage* GetStage( const std::type_info& ti ) const;

		private:
			/*	Current stage
			*/
			Stage* stage;

			DWord index;

			/*	Stage container
			*/
			stl::vector< Stage* > listStages;
		};


	/*
	 @	Director singleton shortcut
	 */
	extern FC_API Director& s_Director;
}