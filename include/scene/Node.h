/*
**
**	Scene node.
**
		Copyright (c) WangFeng since 2012...
*/

#pragma once

#include "../common/Tree.h"
#include "../common/Entity.h"

#include "Transform.h"
#include "NodeComponent.h"


/**
 !	Scene node inner kit
 */
namespace fce::inner::scene
{
	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			SCALE,
			ROTATION,
			TRANSLATION,
			COMBINATION,

			ADD_CHILD,
			MOVE_CHILD,
			REMOVE_CHILD,

			MAXIMUM
		};

	/*	Invoker chain wrapper
	 */
	using DelegateHandler = InvokerChainWrapper
							<
								EVENT, class Node&
							>;
}


namespace fce::inner::scene
{
	class FC_API Node final
		:
		public Entity
		,
		public TreeNode< Node >
		,
		public DelegateHandler
		{
			/*	Friends
			*/
			friend Scene;

			/*	Allocated by object pool
			*/
			using TreeNode = TreeNode< Node >;

			OBJECT_POOL( Node, TreeNode );

		public:
			/*	Using inner kit
			*/
			using EVENT = EVENT;

			/*	Local transform
			*/
			Transform transform;

			/*	Query by ID
			*/
			QUERY_ID( Node )

		public:
			/*	Access
			*/
			using TreeNode::end;

			using TreeNode::begin;

			using TreeNode::GetIndex;

			using TreeNode::AddChild;

			using TreeNode::NewChild;

			using TreeNode::SetParent;

			using TreeNode::RemoveChild;

			using TreeNode::operator [];

			/*	Assignment
			*/
			Node& operator = ( Node&& ) noexcept;

			Node& operator = ( const Node& ) NOEXCEPT;

			/*	Serialize
			*/
			Buffer& operator >> ( Buffer& ) const NOEXCEPT;

			/*	Update
			*/
			Void Update( const EVENT = EVENT::COMBINATION ) noexcept;

		public:
			/*	Move child
			*/
			using TreeNode::MoveUp;

			using TreeNode::MoveDown;

			using TreeNode::MoveEnd;

			using TreeNode::MoveBegin;

			/*	Move node
			*/
			Void MoveUp() noexcept;

			Void MoveDown() noexcept;

			Void MoveEnd() noexcept;

			Void MoveBegin() noexcept;

			Void SetParent( Node* const ) noexcept;

		public:
			/*	Destroy
			*/
			Void Destroy() noexcept;

			/*	Remove child by NodeComponent
			*/
			Void RemoveChild( NodeComponent& ) NOEXCEPT;

			Void RemoveChild( NodeComponent* const ) NOEXCEPT;

		public:
			/*	Add child with component
			*/
			template< X_component T, typename ...Args >

			T& AddChild( Args&& ...args ) NOEXCEPT;

			/*	New child with component
			*/
			template< X_component T, typename ...Args >

			T* NewChild( Args&&... ) NOEXCEPT;

			template< X_component T, typename ...Args >

			T* NewChild( const Transform&,
						 Args&&... ) NOEXCEPT;

			template< X_component T, typename ...Args >

			T* NewChild( const Vector3& pos,
						 const Vector3& scale,
						 const Quaternion& orien,
						 Args&&... ) NOEXCEPT;

		public:
			/*	Gets
			*/
			[[nodiscard]] UInt GetIndex() const noexcept;

			template< Bool RECURSIVE = true >

			[[nodiscard]] auto& GetScene( this auto& ) noexcept;

			/*
			 @	Query component by Name
			 */
			template< X_component T,
					  Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto& GetChild( this auto&,
										  const Key& ) NOEXCEPT;

			template< X_component T,
					  Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto* FindChild( this auto&,
										   const Key& ) noexcept;

			/*
			 @	Query node by Name
			 */
			template< Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto& GetChild( this auto&,
										  const Key& ) NOEXCEPT;

			template< Bool RECURSIVE = true, class Key >

			[[nodiscard]] auto* FindChild( this auto&,
										   const Key& ) noexcept;

		public:
			/*	Set position
			*/
			Void SetX( const Float ) noexcept;

			Void SetY( const Float ) noexcept;

			Void SetZ( const Float ) noexcept;

			Void SetPosition( const Float x,
							  const Float y ) noexcept;

			Void SetPosition( const Float x,
							  const Float y,
							  const Float z ) noexcept;

			Void SetPosition( const Vector2& ) noexcept;

			Void SetPosition( const Vector3& ) noexcept;

			/*	Set scale
			*/
			Void SetScaleX( const Float ) noexcept;

			Void SetScaleY( const Float ) noexcept;

			Void SetScaleZ( const Float ) noexcept;

			Void SetScale( const Float ) noexcept;

			Void SetScale( const Float x,
						   const Float y ) noexcept;

			Void SetScale( const Float x,
						   const Float y,
						   const Float z ) noexcept;

			Void SetScale( const Vector2& ) noexcept;

			Void SetScale( const Vector3& ) noexcept;

			/*	Set orientation
			*/
			Void SetAngleX( const Float ) noexcept;

			Void SetAngleY( const Float ) noexcept;

			Void SetAngleZ( const Float ) noexcept;

			Void SetAngles( const Float xAngle,
							const Float yAngle,
							const Float zAngle ) noexcept;

			Void SetAngles( const Vector3& angles ) noexcept;

			Void SetOrientation( const Quaternion& ) noexcept;

			Void SetOrientation( const Vector3& axis,
								 const Float angle ) noexcept;

			Void SetOrientation( const Vector3& xAxis,
								 const Vector3& yAxis,
								 const Vector3& zAxis ) noexcept;

			/*	Set transform
			*/
			Void SetTransform( const Transform& ) noexcept;

			Void SetTransform( const Vector3& pos,
							   const Vector3& scale,
							   const Quaternion& ) noexcept;

		public:
			/*	Pitch & Yaw & Roll
			*/
			Void Yaw( const Float angle ) noexcept;

			Void Roll( const Float angle ) noexcept;

			Void Pitch( const Float angle ) noexcept;

			/*	Translate
			*/
			Void Translate( const Float x,
							const Float y ) noexcept;

			Void Translate( const Float x,
							const Float y,
							const Float z ) noexcept;

			Void Translate( const Vector2& ) noexcept;

			Void Translate( const Vector3& ) noexcept;

			/*	Rotate
			*/
			Void RotateX( const Float angle ) noexcept;

			Void RotateY( const Float angle ) noexcept;

			Void RotateZ( const Float angle ) noexcept;

			Void Rotate( const Float xAngle,
						 const Float yAngle,
						 const Float zAngle ) noexcept;

			Void Rotate( const Vector3& axis,
						 const Float angle ) noexcept;

			Void Rotate( const Quaternion& ) noexcept;

		public:
			/*	Set global space
			*/
			Void SetGlobalScale( const Vector3& ) noexcept;

			Void SetGlobalPosition( const Vector3& ) noexcept;

			Void SetGlobalOrientation( const Quaternion& ) noexcept;

			Void SetGlobalScale( const Float x,
								 const Float y,
								 const Float z ) noexcept;

			Void SetGlobalPosition( const Float x,
									const Float y,
									const Float z ) noexcept;

			Void SetGlobalOrientation( const Float xAngle,
									   const Float yAngle,
									   const Float zAngle ) noexcept;

			/*	Set global transform
			*/
			Void SetGlobalTransform( const Transform& ) noexcept;

			Void SetGlobalTransform( const Vector3& pos,
									 const Vector3& scale,
									 const Quaternion& ) noexcept;

		public:
			/*	Angles
			*/
			[[nodiscard]] Float AngleX() const NOEXCEPT;

			[[nodiscard]] Float AngleY() const NOEXCEPT;

			[[nodiscard]] Float AngleZ() const NOEXCEPT;

			/*	Axles
			*/
			[[nodiscard]] Matrix3 Axes() const noexcept;

			[[nodiscard]] Vector3 AxisX() const noexcept;

			[[nodiscard]] Vector3 AxisY() const noexcept;

			[[nodiscard]] Vector3 AxisZ() const noexcept;

			/*	Local space
			*/
			[[nodiscard]] const Vector3& Scale() const noexcept;

			[[nodiscard]] const Vector3& Position() const noexcept;

			[[nodiscard]] const Quaternion& Orientation() const noexcept;

			/*	Global space
			*/
			[[nodiscard]] const Vector3& GlobalScale() const noexcept;

			[[nodiscard]] const Vector3& GlobalPosition() const noexcept;

			[[nodiscard]] const Quaternion& GlobalOrientation() const noexcept;

			[[nodiscard]] const Transform& GlobalTransform() const noexcept;

			[[nodiscard]] const Affine3& GlobalMatrix() const noexcept;

			/*	Convert Local <=> Global
			*/
			[[nodiscard]] Vector3 PosToLocal( const Vector3& ) const noexcept;

			[[nodiscard]] Vector3 PosToGlobal( const Vector3& ) const noexcept;

			[[nodiscard]] Vector3 ScaleToLocal( const Vector3& ) const noexcept;

			[[nodiscard]] Vector3 ScaleToGlobal( const Vector3& ) const noexcept;

			[[nodiscard]] Quaternion OrienToLocal( const Quaternion& ) const noexcept;

			[[nodiscard]] Quaternion OrienToGlobal( const Quaternion& ) const noexcept;

		protected:
			/*	Constructor & Destructor
			*/
			~Node() = default;

			Node( Node* const,
				  const Buffer& );

			Node( Node* const,
				  Node&& ) noexcept;

			Node( Node* const,
				  const Node& ) NOEXCEPT;

			Node( Node* const ) noexcept;

			Node( Node* const,
				  const Transform& ) noexcept;

			Node( Node* const,
				  const Vector3&,
				  const Vector3&	= Vector3::ONE,
				  const Quaternion& = Quaternion::IDENTITY ) noexcept;

			/*	Editor mode
			*/
			#ifdef FC_EDITOR

				XML_ARCHIVE_DECLARATION

				Node( Node* const,
					  const Xml::Deserialization& ) NOEXCEPT;

				Void operator >> ( Xml::Serialization& ) const NOEXCEPT;

				Void operator << ( const Xml::Deserialization& ) NOEXCEPT;

			#endif

		private:
			/*	Updates
			*/
			Void UpdateSub( const EVENT ) noexcept;

			Void UpdateRoot( const EVENT ) noexcept;

			Void UpdateCommon( const EVENT ) noexcept;

			Void UpdateGlobal( const EVENT ) noexcept;

			Void UpdateGlobalSub( const EVENT ) noexcept;

		private:
			/*	Global transform
			*/
			Affine3 globalMatrix;

			Transform globalTransform;

			/*	Archive name
			*/
			static constexpr inline char ARCHIVE_NAME[] = "Node";
		};
}



/*
 !	Implementation
 */
namespace fce
{
	/*
	 @	SceneNode
	 */
	using SceneNode = inner::scene::Node;
}