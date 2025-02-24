/*
**
**	Movement.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../common/Timer.h"
#include "../input/Keyboard.h"


namespace fce
{
	/*
	 !	Displacement
	 */
	class FC_API Displacement final
		{

		public:
			/*	Initial velocity
			*/
			Float initialVelocity;

			/*	Acceleration
			*/
			Float acceleration;

			/*	Common
			*/
			Void Update();

		protected:
			/*	Deserialize
			*/
			Displacement( Float v0 = Float_ZERO,
						  Float a = Float_ZERO )
				:
					initialVelocity( v0 ),
					acceleration( a )
				{
				}

		private:
			/*	Start time
			*/
			using Duration = std::chrono::duration< Float32 >;

			std::chrono::time_point
			< std::chrono::steady_clock, Duration > startTime;
		};


	/*
	 !	Movement
	 */
	class FC_API Movement final
		:
		public Scene::NodeComponent
		{
			COMPONENT_ARCHIVE( Movement,
							   NodeComponent );

		public:
			/*	Common
			*/
			Void Update();

		protected:
			/*	Constructor & Destructor
			*/
			~Movement() noexcept;

			Movement( Entity& entity );

			Movement( Entity& entity,
					  const Buffer& buf );

			/*	OnKeyboard
			*/
			Bool OnKeyboard( const Keyboard::Message& msg );

		private:
			Bool down;
			Keyboard::KEY key;

			/*	Trigger time
			*/
			using Duration = std::chrono::duration< Float32 >;

			std::chrono::time_point
			< std::chrono::steady_clock, Duration > startTime;

			Duration duration;
		};
}