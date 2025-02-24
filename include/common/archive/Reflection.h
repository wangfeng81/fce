/*
**
**	Reflection.
**
		If the constructor throws an exception
		it will return a null pointer

	Copyright (c) WangFeng since 2012...
*/


namespace fce
{
	/*
	 !	Reflection
	 */
	template< class Base,
			  typename ...Args >

	class Reflection final
		{
			/*	Friends
			*/
			friend typename Base;

			/*	Singleton instance
			*/
			SINGLETON_INSTANCE( Reflection );

			/*	Kit
			*/
			using PolymorphicPool = PolymorphicPool< ObjectEx >;

			using Creator = Base * ( * ) ( Args&&... ) noexcept;

		public:
			/*
			 @	Register with class type
			 */
			template< class T >

			requires requires{ T::CLASS_NAME; }

			FC_INLINE DWord Register() NOEXCEPT
				{
					return this->Register< T >
								(
									T::CLASS_NAME
								);
				}

			/*
			 @	Register with class hash
			 */
			template< class T >

			FC_INLINE DWord Register( const DWord hash ) NOEXCEPT
				{
					const auto p
						{
							this->mapCreators.try_emplace
								(
									hash,
									Create< T >
								)
						};

					FC_ASSERT_LOG( p.second,
								  "The class has Registered..." );

					return hash;
				}

			/*
			 @	Register with class name
			 */
			template< class T >

			FC_INLINE DWord Register( const stl::string& name ) NOEXCEPT
				{
					return this->Register< T >
								(
									Crc32( name )
								);
				}

		public:
			/*
			 @	Reflect with class hash
			 */
			FC_INLINE [[nodiscard]] Base* Reflect( const DWord hash,
												   Args&& ...args )
				{
					const Creator creator
						{
							this->GetCreator( hash )
						};

					return creator( std::forward< Args >( args )... );
				}

			/*
			 @	Reflect with class name
			 */
			FC_INLINE [[nodiscard]] Base* Reflect( const Char* const name,
												   Args&& ...args )
				{
					return this->Reflect( Crc32( name ),
										  std::forward< Args >( args )... );
				}

		public:
			/*
			 @	Generate
			 */
			template< std::derived_from< Base > T,
					  typename ...ArgsX >

			FC_INLINE [[nodiscard]] T* Generate( ArgsX&& ...argsX ) noexcept
				{
					return PolymorphicPool::Generate< T >
							(
								std::forward< ArgsX >( argsX )...
							);
				}

			/*
			 @	Rebuild
			 */
			template< std::derived_from< Base > T,
					  typename ...ArgsX >

			FC_INLINE [[nodiscard]] T* Rebuild( T* const obj,
												ArgsX&& ...argsX ) NOEXCEPT
				{
					return PolymorphicPool::Rebuild< T >
							(
								obj,
								std::forward< ArgsX >( argsX )...
							);
				}

			/*
			 @	Destroy
			 */
			FC_INLINE Void Destroy( Base* obj ) NOEXCEPT
				{
					PolymorphicPool::Destroy( obj );
				}

		protected:
			/*
			 @	Constructor & Destructor
			 */
			DISABLE_COPY_MOVE( Reflection );

			~Reflection() = default;

			Reflection() = default;

			/*
			 @	Create
			 */
			template< std::derived_from< Base > T >

			FC_INLINE static Base* Create( Args&& ...args ) noexcept
				{
					return PolymorphicPool::Generate< T >
							(
								std::forward< Args >( args )...
							);
				}

			/*
			 @	Get creator
			 */
			FC_INLINE const Creator GetCreator( const DWord hash ) const
				{
					const auto p
						{
							this->mapCreators.find( hash )
						};

					FC_ASSERT_LOG( p != this->mapCreators.end(),
								  "The class hasn't registered..." );

					return p->second;
				}

		private:
			/*	Associate creator with the class hash
			*/
			stl::unordered_map< DWord, Creator > mapCreators;
		};
}



/*
 @	ObjectEx pool
 */
#define OBJECT_EX( T )																\
																					\
		POLYMORPHIC_POOL( T, fce::ObjectEx );

/*
 @	Reflection decl
 */
#define REFLECTION_DECL( U, ... )													\
																					\
		using Reflection##U = fce::Reflection< __VA_ARGS__ >;						\
																					\
		inline static auto& reflection##U											\
			{																		\
				Reflection##U::Instance()											\
			};

/*
 @	Reflection base
 */
#define REFLECTION_BASE( T, ... )													\
																					\
		OBJECT_EX( T )																\
																					\
		XML_ARCHIVE_DECLARATION														\
																					\
		XML_REFLECTION( T, __VA_ARGS__ )											\
																					\
		REFLECTION_DECL( , T, __VA_ARGS__, const Buffer& )

/*
 @	Reflection object
 */
#define REFLECTION( T )																\
																					\
		OBJECT_EX( T )																\
																					\
		friend Reflection;															\
																					\
		friend class ReflectionXml;													\
																					\
		inline static const fce::stl::string CLASS_NAME								\
			{																		\
				PrettyFunction::className< T >()									\
			};																		\
																					\
		inline static const fce::DWord CLASS_HASH									\
			{																		\
				reflection.Register< T >( XML_REGISTER( T ) )						\
			};																		\
																					\
		[[nodiscard]] fce::DWord classHash() const noexcept override				\
			{																		\
				return CLASS_HASH;													\
			}																		\
																					\
		[[nodiscard]] const fce::stl::string& className() const noexcept override	\
			{																		\
				return CLASS_NAME;													\
			}



/*
 @	Reflection archive
 */
#define REFLECTION_ARCHIVE( T, P, ... )												\
																					\
		REFLECTION( T )																\
																					\
		DERIVED_ARCHIVE( T, P, __VA_ARGS__ )


/*
 @	Reflection archive
 */
#define REFLECTION_BASE_ARCHIVE( T, P, ... )										\
																					\
		REFLECTION_BASE( T )														\
																					\
		DERIVED_ARCHIVE( T, P, __VA_ARGS__ )