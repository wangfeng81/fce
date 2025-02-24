/*
**
**	Component.
**
		Copyright (c) WangFeng since 2012...
*/

namespace fce
{
	/*
	 !	Component

			If multiple inheritance is required
			it must be the first base class

			Because the dynamic_cast has been deprecated
			use static_cast instead to improve efficiency
	 */
	class FC_API Component
		:
		public ObjectEx
		{
			/*	Friends
			*/
			friend Entity;

		protected:
			/*	Reflection base
			*/
			REFLECTION_BASE( Component, Entity& );

			/*	Constructor & Destructor
			*/
			using ObjectEx::ObjectEx;

			Component() = default;

			Component( Entity& ) noexcept {}

			~Component() override = default;

			/*	Dependent on ( Syntax sugar )
			*/
			[[nodiscard]] Bool DependentOn( this const auto&, const Entity& ) noexcept;

			[[nodiscard]] Bool DependentOn( this const auto&, const Component& ) noexcept;

			/*	Dependent on entity & component
			*/
			[[nodiscard]] virtual Bool DependentOn( const Entity* const ) const noexcept;

			[[nodiscard]] virtual Bool DependentOn( const Component* const ) const noexcept;

		private:
			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "Component";
		};


	/*	concept		Derived component
	 */
	template< class T >

	concept X_component = std::derived_from< T, Component >;
}



/*
 @	Component macros
 */
#define COMPONENT( T )								\
													\
		friend Entity;								\
													\
		OBJECT_EX( T )


#define COMPONENT_ARCHIVE( T, P, ... )				\
													\
		friend Entity;								\
													\
		XML_CONSTRUCTOR_ENTITY( T, P )				\
													\
		REFLECTION_ARCHIVE( T, P, __VA_ARGS__ )