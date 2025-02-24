/*
**
**	Enum.
**
		Copyright (c) WangFeng since 2006...
*/

namespace fce
{
	/*
	 !	Enum

			ordered from 0 to N
	 */
	template< Enum_value T,
			  std::underlying_type_t< T > N
			= std::underlying_type_t< T >( T::MAXIMUM ) >

	class Enum final
		{
			using Value_t = std::underlying_type_t< T >;

		public:
			/*
			 @	Get name
			 */
			static const stl::string& GetName( const T value ) NOEXCEPT
				{
					FC_ASSERT( Value_t( value ) < N );

					static const stl::string* arrNames
						{
							GetArray( std::make_index_sequence< N >{} )
						};

					return arrNames[ Value_t( value ) ];
				}

			/*
			 @	Get value
			 */
			static T GetValue( const Character_string_ex auto& name ) NOEXCEPT
				{
					for( Value_t i{ 0 }; i < N; ++i )
						{
							if( GetName( T( i ) ) == name )
								{
									return T( i );
								}
						}

					return T( N );
				}

			/*
			 @	Get name ( Syntax sugar )
			 */
			static const Char* Get( const T value ) noexcept
				{
					if( Value_t( value ) < N )
						{
							return GetName( value ).c_str();
						}

					return nullptr;
				}

			/*
			 @	Get value ( Syntax sugar )
			 */
			static T Get( const Character_string_ex auto& name ) NOEXCEPT
				{
					return GetValue( name );
				}

		private:
			/*
			 @	Get name
			 */
			template< T >

			static consteval std::string_view GetName() noexcept
				{
					#if _MSC_VER || __MINGW32__ || __MINGW64__

						constexpr char END_FLAG { '>' };

						constexpr char START_FLAG { ':' };

					#else

						//constexpr char END_FLAG { ']' };

						//constexpr char START_FLAG[] = "= ";

					#endif

					constexpr std::string_view sv
						{
							std::source_location::current()
												 .function_name()
						};

					constexpr std::size_t end
						{
							sv.rfind( END_FLAG )
						};

					constexpr std::size_t start
						{
							sv.rfind( START_FLAG, end ) + 1
						};

					return sv.substr( start, end - start );
				}

			/*
			 @	Get array
			 */
			template< std::size_t ...I >

			static const stl::string* GetArray( const std::index_sequence< I... > ) noexcept
				{
					static const stl::string arrNames[]
						{
							stl::string( GetName< T( I ) >() )...
						};

					return arrNames;
				}
		};
}