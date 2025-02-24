/*
**
**	Movement.
**
		Copyright (c) WangFeng since 2006...
*/

#include "../../include/scene/Scene.h"


namespace fce
{
	/*	Destructor
	*/
	Movement::~Movement() noexcept
		{
			s_Keyboard.RemoveHandler(
				this, &Movement::OnKeyboard );
		}

	/*	Deserialize
	*/
	Movement::Movement( Entity& entity,
						const Buffer& buf )
		:
			Movement( entity )
		{
		}

	/*	Constructor
	*/
	Movement::Movement( Entity& entity )
		:
			down(false),
			NodeComponent( entity )
		{
			s_Keyboard.AddHandler( this,
								   &Movement::OnKeyboard );
		}
}



namespace fce
{
	/*
	 @	Update
	 */
	Void Movement::Update()
		{
			if( this->down )
				{
				const float speed = 180.1f;
				const float speed2 = 0.1f;

				auto now{ std::chrono::steady_clock::now() };
				duration = now - startTime;
				startTime = now;

				auto s1{ 20.1f };
				auto s2{ 0.2f };

				switch( this->key )
					{
						case	Keyboard::KEY::_W:
							this->node.Translate( this->node.AxisZ()* s1);
							break;

						case	Keyboard::KEY::_S:
							this->node.Translate( -this->node.AxisZ() * s1 );
							break;

						case	Keyboard::KEY::_A:
							this->node.Translate( this->node.AxisX() * s1 );
							break;

						case	Keyboard::KEY::_D:
							this->node.Translate( -this->node.AxisX() * s1 );
							break;


						case	Keyboard::KEY::PRIOR:
							this->node.Translate( this->node.AxisY() * s1 );
							break;

						case	Keyboard::KEY::NEXT:
							this->node.Translate( -this->node.AxisY() * s1 );
							break;




						case	Keyboard::KEY::UP:
							this->node.Pitch( s2 );
							break;

						case	Keyboard::KEY::DOWN:
							this->node.Pitch( -s2 );
							break;

						case	Keyboard::KEY::LEFT:
							this->node.Yaw( s2 );
							break;

						case	Keyboard::KEY::RIGHT:
							this->node.Yaw( -s2 );
							break;

						case	Keyboard::KEY::HOME:
							this->node.Roll( s2 );
							break;

						case	Keyboard::KEY::END:
							this->node.Roll( -s2 );
							break;
					}
				}
		};

	/*
	 @	OnKeyboard.
	 */
	Bool Movement::OnKeyboard( const Keyboard::Message& msg )
		{
			this->down = Bool( msg.e );

			if( this->down )
				{
					this->key = msg.key;

					auto now{ std::chrono::steady_clock::now() };
					duration = now - startTime;
					startTime = now;
				}


			return false;
		}
}