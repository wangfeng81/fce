/*
**
**	Entity.
**
		Copyright (c) WangFeng since 2012...
*/

#include "../../include/common/Entity.h"


namespace fce
{
	/*
	 @	����
	 */
	Entity::~Entity() noexcept
		{
			this->ClearComponents();
		}

	/*
	 @	���� Buffer
	 */
	Entity::Entity( const Buffer& buf )
		{
			this->DeserializeComponents( buf );
		}

	/*
	 @	���� move
	 */
	Entity::Entity( Entity&& rv ) noexcept
		{
			this->MoveComponents( std::move( rv ) );
		}

	/*
	 @	���� copy
	 */
	Entity::Entity( const Entity& entity ) NOEXCEPT
		{
			this->CopyComponents( entity );
		}

	/*
	 @	��ֵ move
	 */
	Entity& Entity::operator = ( Entity&& entity ) noexcept
		{
			this->MoveComponents( std::move( entity ) );

			return *this;
		}

	/*
	 @	��ֵ copy
	 */
	Entity& Entity::operator = ( const Entity& entity ) NOEXCEPT
		{
			FC_ASSERT( typeid( *this )
					== typeid( entity ) );

			this->ClearComponents();

			this->CopyComponents( entity );

			return *this;
		}

	/*
	 @	�ƶ� �������
	 */
	Void Entity::MoveComponents( Entity&& entity ) noexcept
		{
			try{
					Buffer buf;

					entity.SerializeComponents( buf );

					entity.ClearComponents();

					this->DeserializeComponents( buf.SeekBeg() );
				}
			catch( ... )
				{
				}
		}

	/*
	 @	���� �������
	 */
	Void Entity::CopyComponents( const Entity& entity ) NOEXCEPT
		{
			Buffer buf;

			entity.SerializeComponents( buf );

			this->DeserializeComponents( buf.SeekBeg() );
		}
}



namespace fce
{
	/*
	 @	�����л� ������� bin
	 */
	const Buffer& Entity::DeserializeComponents( const Buffer& buf )
		{
			// ��ȡ ������������� �б�ռ�
			UInt32 count;

			buf >> count;

			this->arrComponents.reserve( count );

			// �����½� �������
			while( count-- )
				{
					this->ReflectComponent( buf );
				}

			return buf;
		}

	/*
	 !	�༭�� ģʽ
	 */
	#ifdef FC_EDITOR

		/*
		 @	���л� xml
		 */
		Void Entity::SerializeComponents( Xml::Serialization& ser ) const NOEXCEPT
			{
				ser.Serialize( this->arrComponents,
							   Component::ARCHIVE_NAME );
			}

		/*
		 @	�����л� xml
		 */
		Void Entity::DeserializeComponents( const Xml::Deserialization& des ) NOEXCEPT
			{
				// ������� lamble
				auto f = [ this ]( const Xml::Deserialization& deser,
								   const std::size_t index ) NOEXCEPT
					{
						// ��ȡ className
						stl::string className;

						deser.Deserialize( className, "className" );

						// �������
						this->arrComponents.emplace_back
							(
								Component::reflectionXml.Reflect( className.c_str(),
																  *this, deser )
							);
					};

				// ���� group �� ����
				des.Travel( f, Component::ARCHIVE_NAME );
			}

	#endif
}



namespace fce
{
	/*
	 @	�� type_info ������� �� ֱ�ӷ���ָ��
	 */
	Component* Entity::FindComponent( const std::type_info& ti ) const noexcept
		{
			for( Component* const c : this->arrComponents )
				{
					if( ti == typeid( *c ) )
						{
							return c;
						}
				}

			return nullptr;
		}

	/*
	 @	�Ż� �� class hash �Ƚϣ���Ϊ��Ҫ�Ĳ��ҷ�ʽ
	 */
	Component* Entity::FindComponent( const DWord classHash ) const noexcept
		{
			for( Component* const c : this->arrComponents )
				{
					if( c->classHash() == classHash )
						{
							return c;
						}
				}

			return nullptr;
		}
}



namespace fce
{
	/*
	 @	��ѯ��� ������ class hash
	 */
	Entity::Iterator Entity::QueryComponent( const DWord classHash ) const noexcept
		{
			auto f = [ classHash ] ( const Component* const c ) noexcept
				{
					return c->classHash() == classHash;
				};

			return stl::find_if( this->arrComponents, f );
		}

	/*
	 @	��ѯ��� ������ with type_info
	 */
	Entity::Iterator Entity::QueryComponent( const std::type_info& ti ) const noexcept
		{
			auto f = [ &ti ] ( const Component* const c ) noexcept
				{
					return ti == typeid( *c );
				};

			return stl::find_if( this->arrComponents, f );
		}

	/*
	 @	��ѯ Entity ��һ������ ������
	 */
	Entity::Iterator Entity::QueryDependency( const Entity* const entity ) const noexcept
		{
			auto f = [ entity ] ( const Component* const c ) noexcept
				{
					return c->DependentOn( entity );
				};

			return stl::find_if( this->arrComponents, f );
		}

	/*
	 @	��ѯ Component ��һ������ ������
	 */
	Entity::Iterator Entity::QueryDependency( const Component* const cmpt ) const noexcept
		{
			auto f = [ cmpt ] ( const Component* const c ) noexcept
				{
					return c->DependentOn( cmpt );
				};

			return stl::find_if( this->arrComponents, f );
		}
}



namespace fce
{
	/*
	 @	��ȡ cmpt Ҫ�������б�
	 */
	Entity::ComponentGroup Entity::GetRequirements( const Component& cmpt ) const noexcept
		{
			ComponentGroup arrRequirements;

			for( Component* const c : this->arrComponents )
				{
					if( cmpt.DependentOn( c ) )
						{
							arrRequirements.emplace_back( c );
						}
				}

			return std::move( arrRequirements );
		}


	/*
	 @	��ȡ ���� entity ���������
	 */
	Entity::ComponentGroup Entity::GetDependencies( const Entity* const entity ) const noexcept
		{
			ComponentGroup arrDependencies;

			for( Component* const c : this->arrComponents )
				{
					if( c->DependentOn( entity ) )
						{
							arrDependencies.emplace_back( c );
						}
				}

			return std::move( arrDependencies );
		}

	/*
	 @	��ȡ ���� cmpt ���������
	 */
	Entity::ComponentGroup Entity::GetDependencies( const Component* const cmpt ) const noexcept
		{
			ComponentGroup arrDependencies;

			for( Component* const c : this->arrComponents )
				{
					if( c->DependentOn( cmpt ) )
						{
							arrDependencies.emplace_back( c );
						}
				}

			return std::move( arrDependencies );
		}
}



namespace fce
{
	/*
	 @	�Ƴ���� by ָ��
	 */
	Void Entity::RemoveComponent( Component* const cmpt ) NOEXCEPT
		{
			// ��ѯ���
			const Iterator p
				{
					this->QueryComponent( cmpt )
				};

			// �Ƴ����
			this->RemoveComponent( p );
		}


	/*
	 @	�Ƴ���� by ������
	 */
	Void Entity::RemoveComponent( const Iterator p ) NOEXCEPT
		{
			Component* const cmpt{ *p };

			// ������ �����������
			FC_ASSERT( ! this->hasDependency( cmpt ) );

			// �Ƴ� �ڵ�
			this->arrComponents.erase( p );

			// ���� ���
			this->DestroyComponent( cmpt );
		}


	/*
	 @	������

			ֻ�����û��������ϵ�����
			���ڴ������������ �޷�ɾ��
			���� transform
	 */
	Void Entity::RemoveComponents() noexcept
		{
			// ɾ���޾����������������������ɾ��
			UInt size{ this->arrComponents.size() };

			while( size-- )
				{
					Component* const cmpt
						{
							this->arrComponents[ size ]
						};

					if( ! this->hasDependency( cmpt ) )
						{
							this->RemoveComponent( cmpt );
						}
				}
		}


	/*
	 @	��� �������

			ȷ����Ϣ�ص�ʱ ��ʹ���б�������޸� Ҳ����������
	 */
	Void Entity::ClearComponents() noexcept
		{
			// �������� ��Ϊ�������������ǰ��
			while( ! this->arrComponents.empty() )
				{
					// ��ȡ β�����
					Component* const cmpt
						{
							this->arrComponents.back()
						};

					// ���� β�����
					this->arrComponents.pop_back();

					// ���� �������
					this->DestroyComponent( cmpt );
				}
		}


	/*
	 @	�������
	 */
	Void Entity::DestroyComponent( Component* const cmpt ) NOEXCEPT
		{
			FC_ASSERT( cmpt );

			// �ȴ��� �¼�

			// ������ ���
			Component::reflection.Destroy( cmpt );
		}
}



namespace fce
{
	/*
	 @	�������������

			���й����� ��̬���ɾ�����
			���ܴ��� ������˳��

			����Ϊ��Ч��
			Ҫ�����������������
			��Ϊ���󲿷����ԭ����������
			���ʱ�Է������ȷ��� �����������
	 */
	Void Entity::SortComponents() noexcept
		{
			// ���������� ����������ǰ
			auto f = [] ( const Component* const cmpt1,
						  const Component* const cmpt2 ) noexcept
				{
					return cmpt2->DependentOn( cmpt1 );
				};

			stl::sort( this->arrComponents, f );
		}


	/*
	 @	������ ���б�
	 */
	Void Entity::AddComponent( Component* const cmpt ) NOEXCEPT
		{
			// �ȼ�� ����Ƿ�Ϊ�գ���Ϊ���� ����ʧ��
			if( cmpt )
				{
					this->arrComponents.emplace_back( cmpt );

					// ���� �¼�
				}
		}


	/*
	 @	�½��������	�������� buf
	 */
	Component* Entity::ReflectComponent( const Buffer& buf )
		{
			// ���� �������
			Component* const cmpt
				{
					// �Ȼ�ȡ class hash code ( DWord )
					Component::reflection.Reflect( buf.Get< DWord >(),
												  *this, buf )
				};

			// ���� ����б�
			this->AddComponent( cmpt );

			return cmpt;
		}


	/*
	 @	���� ���
	 */
	Component* Entity::CopyComponent( const Component* const src ) NOEXCEPT
		{
			FC_ASSERT( src );

			// ���л� ������ hash code
			Buffer buf;

			src->Serialize( buf );

			// �����½��� ����б�
			return this->ReflectComponent( buf.SeekBeg() );
		}


	/*
	 @	������� ������
	 */
	Void Entity::InsertComponent( const Component* const dependency,
										Component* const cmpt ) NOEXCEPT
		{
			// �ȼ�� ����Ƿ�Ϊ�գ���Ϊ���� ����ʧ��
			if( cmpt )
				{
					// ���� ����
					const auto p
						{
							this->QueryComponent( dependency )
						};

					// ���뵽 ���������֮ǰ
					this->arrComponents.emplace( p, cmpt );

					// ���� �¼�
				}
		}
}