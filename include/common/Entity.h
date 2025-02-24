/*
**
**	Entity.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "Archive.h"


/**
 !	Entity inner kit
 */
//namespace fce::inner::entity
//{
//	/*	EVENT
//	 */
//	enum struct EVENT
//		:
//			Byte
//		{
//			ADD_COMPONENT,
//			REMOVE_COMPONENT,
//
//			DESTROY,
//
//			MAXIMUM
//		};
//
//	/*
//	 !	Message
//	 */
//	struct FC_API Message final
//		{
//			/*	EVENT
//			*/
//			EVENT e;
//
//			/*	Component
//			*/
//			Component* component;
//
//			/*	Source entity
//			*/
//			Entity& entity;
//
//		public:
//			/*	Constructor
//			*/
//			Message( Entity& );
//
//			/*	Dispatch
//			*/
//			Void Dispatch() const;
//		};
//
//	/*	Invoker chain wrapper
//	 */
//	using DelegateHandler = InvokerChainWrapper
//							<
//								EVENT, Transform&
//							>;
//}


#include "entity/Component.h"


namespace fce
{
	/*
	 !	Entity
	 */
	class FC_API Entity
		{
			/*	Component container
			*/
			using ComponentGroup = stl::vector< Component* >;

			using Iterator = ComponentGroup::const_iterator;

		public:
			/*	Assignment
			*/
			Entity& operator = ( Entity&& ) noexcept;

			Entity& operator = ( const Entity& ) NOEXCEPT;

			/*	Serialize
			*/
			virtual Buffer& operator >> ( Buffer& ) const NOEXCEPT;

		public:
			/*	Access
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt CountComponents() const noexcept;

			[[nodiscard]] auto& operator [] ( this auto&,
											  const UInt ) NOEXCEPT;

			/*	Get component
			*/
			template< X_component >

			[[nodiscard]] auto& GetComponent( this auto&,
											  const UInt ) NOEXCEPT;

			template< X_component >

			[[nodiscard]] auto& GetComponent( this auto& ) NOEXCEPT;

			/*	Find component
			*/
			template< X_component >

			[[nodiscard]] auto* FindComponent( this auto& ) noexcept;

			template< X_component T >

			requires requires{ T::CLASS_HASH; }

			[[nodiscard]] auto* FindComponent( this auto& ) noexcept;

			/*	Has component
			*/
			template< X_component >

			[[nodiscard]] Bool hasComponent() const noexcept;

			[[nodiscard]] Bool hasComponent( const Component& ) const noexcept;

			[[nodiscard]] Bool hasComponent( const Component* const ) const noexcept;

			/*	Has dependency
			*/
			[[nodiscard]] Bool hasDependency( const Entity& ) const noexcept;

			[[nodiscard]] Bool hasDependency( const Component& ) const noexcept;

			[[nodiscard]] Bool hasDependency( const Entity* const ) const noexcept;

			[[nodiscard]] Bool hasDependency( const Component* const ) const noexcept;

			/*	Get the dependent components
			*/
			[[nodiscard]] ComponentGroup GetDependencies( const Entity& ) const noexcept;

			[[nodiscard]] ComponentGroup GetDependencies( const Component& ) const noexcept;

			[[nodiscard]] ComponentGroup GetDependencies( const Entity* const ) const noexcept;

			[[nodiscard]] ComponentGroup GetDependencies( const Component* const ) const noexcept;

			/*	Get the requirement components
			*/
			[[nodiscard]] ComponentGroup GetRequirements( const Component& ) const noexcept;

			[[nodiscard]] ComponentGroup GetRequirements( const Component* const ) const NOEXCEPT;

		public:
			/*	Add component
			*/
			template< X_component T,
					  typename ...Args >

			T& AddComponent( this auto&,
							 Args&&... ) NOEXCEPT;

			/*	Request component
			*/
			template< X_component T >

			T& RequestComponent( this auto& ) NOEXCEPT;

		public:
			/*	New component
			*/
			template< X_component T,
					  typename ...Args >

			T* NewComponent( this auto&,
							 Args&& ...args ) NOEXCEPT;

			/*	New component with entity
			*/
			template< X_component T,
					  typename ...Args >

			T* NewComponent( this auto& self,
							 Args&& ...args ) NOEXCEPT

			requires requires
				{
					T( std::forward< Args >( args )... );
				};

		public:
			/*	Remove component
			*/
			template< X_component >

			Void RemoveComponent() NOEXCEPT;

			Void RemoveComponent( Component& ) NOEXCEPT;

			Void RemoveComponent( Component* const ) NOEXCEPT;

		public:
			/*	Update component
			*/
			template< X_component T,
					  typename ...Args >

			T* UpdateComponent( this auto&,
								Args&&... ) NOEXCEPT;

			template< X_component T,
					  typename ...Args >

			T* UpdateComponent( this auto&,
								T* const, Args&&... ) NOEXCEPT;

			template< X_component T,
					  typename ...Args >

			T* UpdateComponent( this auto&,
								T* const, Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); };

		public:
			/*	Copy component
			*/
			Component& CopyComponent( const Component& ) NOEXCEPT;

			Component* CopyComponent( const Component* const ) NOEXCEPT;

		public:
			/*	Sort components
			*/
			Void SortComponents() noexcept;

			/*	Remove components
			*/
			Void RemoveComponents() noexcept;

			/*	Insert component
			*/
			template< X_component T,
					  typename ...Args >

			T* InsertComponent( this auto&,
								Component* const,
								Args&&... ) NOEXCEPT;

		public:
			/*	Has name
			*/
			template< class T = stl::string >

			Bool hasName() NOEXCEPT;

			/*	Remove name
			*/
			template< class T = stl::string >

			Void RemoveName() NOEXCEPT;

			/*	Get name
			*/
			template< class T = stl::string >

			auto& GetName( this auto& ) NOEXCEPT;

			/*	Add name component
			*/
			template< class T = stl::string,
					  typename ...Args >

			auto& AddName( Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); };

			/*	Set name component
			*/
			template< class T = stl::string,
					  typename ...Args >

			auto& SetName( Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); };

		public:
			/*	Has id
			*/
			template< class T = stl::string >

			Bool hasID() NOEXCEPT;

			/*	Remove id
			*/
			template< class T = stl::string >

			Void RemoveID() NOEXCEPT;

			/*	Get id
			*/
			template< class T = stl::string >

			auto& GetID( this auto& ) NOEXCEPT;

			/*	Add id component
			*/
			template< class T = stl::string,
					  typename ...Args >

			auto& AddID( Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); };

			/*	Set id component
			*/
			template< class T = stl::string,
					  typename ...Args >

			auto& SetID( Args&& ...args ) NOEXCEPT

			requires requires{ T( std::forward< Args >( args )... ); };

		protected:
			/*	Constructor & Destructor
			*/
			virtual ~Entity() noexcept;

			Entity() = default;

			Entity( Entity&& ) noexcept;

			Entity( const Entity& ) NOEXCEPT;

			explicit Entity( const Buffer& );

			/*	Clear & Copy & Move all components
			*/
			Void ClearComponents() noexcept;

			Void MoveComponents( Entity&& ) noexcept;

			Void CopyComponents( const Entity& ) NOEXCEPT;

			/*	Serialize & Deserialize
			*/
			virtual const Buffer& operator << ( const Buffer& );

			const Buffer& DeserializeComponents( const Buffer& );

			Buffer& SerializeComponents( Buffer& ) const NOEXCEPT;

			/*	Editor mode
			*/
			#ifdef FC_EDITOR

				XML_ARCHIVE_DECLARATION

				virtual Void operator >> ( Xml::Serialization& ) const NOEXCEPT;

				virtual Void operator << ( const Xml::Deserialization& ) NOEXCEPT;

				Void SerializeComponents( Xml::Serialization& ) const NOEXCEPT;

				Void DeserializeComponents( const Xml::Deserialization& ) NOEXCEPT;

			#endif

		private:
			/*	Process component
			*/
			Component* ReflectComponent( const Buffer& );

			Void AddComponent( Component* const ) NOEXCEPT;

			Void RemoveComponent( const Iterator ) NOEXCEPT;

			Void DestroyComponent( Component* const ) NOEXCEPT;

			Void InsertComponent( const Component* const,
										Component* const ) NOEXCEPT;

			template< X_component = Component >

			[[nodiscard]] auto* castComponent( this auto&,
											   Component* const ) noexcept;

			[[nodiscard]] Component* FindComponent( const DWord ) const noexcept;

			[[nodiscard]] Component* FindComponent( const std::type_info& ) const noexcept;

			/*	Query component
			*/
			template< X_component T >

			requires requires{ T::CLASS_HASH; }

			[[nodiscard]] Iterator QueryComponent() const noexcept;

			template< X_component >

			[[nodiscard]] Iterator QueryComponent() const noexcept;

			[[nodiscard]] Iterator QueryComponent( const DWord ) const noexcept;

			[[nodiscard]] Iterator QueryComponent( const std::type_info& ) const noexcept;

			[[nodiscard]] Iterator QueryComponent( const Component* const ) const noexcept;

			/*	Query dependency
			*/
			[[nodiscard]] Iterator QueryDependency( const Entity* const ) const noexcept;

			[[nodiscard]] Iterator QueryDependency( const Component* const ) const noexcept;

		private:
			/*	Component group
			*/
			ComponentGroup arrComponents;

			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "Entity";
		};


	/*	concept		Derived entity
	 */
	template< class T >

	concept X_entity = std::derived_from< T, Entity >;
}


/**
 !	Includes
 */
#include "entity/Identification.h"
#include "entity/ComponentEx.h"
#include "entity/Container.h"


/**
 !	Inlines
 */
#include "entity/Entity.inl"
#include "entity/Component.inl"