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
	 @	析构
	 */
	Entity::~Entity() noexcept
		{
			this->ClearComponents();
		}

	/*
	 @	构造 Buffer
	 */
	Entity::Entity( const Buffer& buf )
		{
			this->DeserializeComponents( buf );
		}

	/*
	 @	构造 move
	 */
	Entity::Entity( Entity&& rv ) noexcept
		{
			this->MoveComponents( std::move( rv ) );
		}

	/*
	 @	构造 copy
	 */
	Entity::Entity( const Entity& entity ) NOEXCEPT
		{
			this->CopyComponents( entity );
		}

	/*
	 @	赋值 move
	 */
	Entity& Entity::operator = ( Entity&& entity ) noexcept
		{
			this->MoveComponents( std::move( entity ) );

			return *this;
		}

	/*
	 @	赋值 copy
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
	 @	移动 所有组件
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
	 @	拷贝 所有组件
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
	 @	反序列化 所有组件 bin
	 */
	const Buffer& Entity::DeserializeComponents( const Buffer& buf )
		{
			// 获取 组件数量，分配 列表空间
			UInt32 count;

			buf >> count;

			this->arrComponents.reserve( count );

			// 反射新建 所有组件
			while( count-- )
				{
					this->ReflectComponent( buf );
				}

			return buf;
		}

	/*
	 !	编辑器 模式
	 */
	#ifdef FC_EDITOR

		/*
		 @	序列化 xml
		 */
		Void Entity::SerializeComponents( Xml::Serialization& ser ) const NOEXCEPT
			{
				ser.Serialize( this->arrComponents,
							   Component::ARCHIVE_NAME );
			}

		/*
		 @	反序列化 xml
		 */
		Void Entity::DeserializeComponents( const Xml::Deserialization& des ) NOEXCEPT
			{
				// 遍历组件 lamble
				auto f = [ this ]( const Xml::Deserialization& deser,
								   const std::size_t index ) NOEXCEPT
					{
						// 获取 className
						stl::string className;

						deser.Deserialize( className, "className" );

						// 反射添加
						this->arrComponents.emplace_back
							(
								Component::reflectionXml.Reflect( className.c_str(),
																  *this, deser )
							);
					};

				// 查找 group 并 遍历
				des.Travel( f, Component::ARCHIVE_NAME );
			}

	#endif
}



namespace fce
{
	/*
	 @	用 type_info 查找组件 并 直接返回指针
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
	 @	优化 用 class hash 比较，作为主要的查找方式
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
	 @	查询组件 迭代器 class hash
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
	 @	查询组件 迭代器 with type_info
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
	 @	查询 Entity 第一个依赖 迭代器
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
	 @	查询 Component 第一个依赖 迭代器
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
	 @	获取 cmpt 要求的组件列表
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
	 @	获取 依赖 entity 的所有组件
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
	 @	获取 依赖 cmpt 的所有组件
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
	 @	移除组件 by 指针
	 */
	Void Entity::RemoveComponent( Component* const cmpt ) NOEXCEPT
		{
			// 查询组件
			const Iterator p
				{
					this->QueryComponent( cmpt )
				};

			// 移除组件
			this->RemoveComponent( p );
		}


	/*
	 @	移除组件 by 迭代器
	 */
	Void Entity::RemoveComponent( const Iterator p ) NOEXCEPT
		{
			Component* const cmpt{ *p };

			// 不能有 依赖它的组件
			FC_ASSERT( ! this->hasDependency( cmpt ) );

			// 移除 节点
			this->arrComponents.erase( p );

			// 销毁 组件
			this->DestroyComponent( cmpt );
		}


	/*
	 @	清空组件

			只能清空没有依赖关系的组件
			对于存在依赖的组件 无法删除
			比如 transform
	 */
	Void Entity::RemoveComponents() noexcept
		{
			// 删除无绝对依赖的所有组件，反向删除
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
	 @	清空 所有组件

			确保消息回调时 即使对列表进行了修改 也能正常运行
	 */
	Void Entity::ClearComponents() noexcept
		{
			// 反序销毁 因为被依赖的组件在前面
			while( ! this->arrComponents.empty() )
				{
					// 获取 尾部组件
					Component* const cmpt
						{
							this->arrComponents.back()
						};

					// 弹出 尾部组件
					this->arrComponents.pop_back();

					// 销毁 组件对象
					this->DestroyComponent( cmpt );
				}
		}


	/*
	 @	销毁组件
	 */
	Void Entity::DestroyComponent( Component* const cmpt ) NOEXCEPT
		{
			FC_ASSERT( cmpt );

			// 先触发 事件

			// 再销毁 组件
			Component::reflection.Destroy( cmpt );
		}
}



namespace fce
{
	/*
	 @	按依赖排序组件

			运行过程中 动态添加删除组件
			可能打乱 其依赖顺序

			另外为了效率
			要求组件单独处理排序
			因为绝大部分组件原生就是有序
			添加时自发会优先放入 被依赖的组件
	 */
	Void Entity::SortComponents() noexcept
		{
			// 按依赖排序 被依赖的在前
			auto f = [] ( const Component* const cmpt1,
						  const Component* const cmpt2 ) noexcept
				{
					return cmpt2->DependentOn( cmpt1 );
				};

			stl::sort( this->arrComponents, f );
		}


	/*
	 @	添加组件 到列表
	 */
	Void Entity::AddComponent( Component* const cmpt ) NOEXCEPT
		{
			// 先检测 组件是否为空，因为可能 生成失败
			if( cmpt )
				{
					this->arrComponents.emplace_back( cmpt );

					// 触发 事件
				}
		}


	/*
	 @	新建反射组件	带类名的 buf
	 */
	Component* Entity::ReflectComponent( const Buffer& buf )
		{
			// 反射 生成组件
			Component* const cmpt
				{
					// 先获取 class hash code ( DWord )
					Component::reflection.Reflect( buf.Get< DWord >(),
												  *this, buf )
				};

			// 加入 组件列表
			this->AddComponent( cmpt );

			return cmpt;
		}


	/*
	 @	拷贝 组件
	 */
	Component* Entity::CopyComponent( const Component* const src ) NOEXCEPT
		{
			FC_ASSERT( src );

			// 序列化 带类名 hash code
			Buffer buf;

			src->Serialize( buf );

			// 反射新建到 组件列表
			return this->ReflectComponent( buf.SeekBeg() );
		}


	/*
	 @	插入组件 并排序
	 */
	Void Entity::InsertComponent( const Component* const dependency,
										Component* const cmpt ) NOEXCEPT
		{
			// 先检测 组件是否为空，因为可能 生成失败
			if( cmpt )
				{
					// 查找 依赖
					const auto p
						{
							this->QueryComponent( dependency )
						};

					// 插入到 依赖的组件之前
					this->arrComponents.emplace( p, cmpt );

					// 触发 事件
				}
		}
}