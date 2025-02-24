/*
**
**	PrettyFunction.
**
		Copyright (c) WangFeng since 2020...
*/

namespace fce
{
	/*
	 !	Pretty function
	 */
	class PrettyFunction final
		{
			/*	Flags
			*/
			#if _MSC_VER || __MINGW32__ || __MINGW64__

				inline static constexpr char END_FLAG { '>' };

				inline static constexpr char START_FLAG[] = "RawName<";

			#else

				inline static constexpr char END_FLAG { ']' };

				inline static constexpr char START_FLAG[] = "= ";

			#endif

		public:
			/*
			 @	class name
			 */
			template< class T >

			FC_INLINE static stl::string className() noexcept
				{
					constexpr std::string_view rawName
						{
							RawName< T >()
						};

					if constexpr( is_template_v< T > )
						{
							return FormatName( rawName );
						}
					else
						{
							return stl::string( rawName );
						}
				}

		protected:
			/*
			 @	Raw name
			 */
			template< class T >

			static consteval std::string_view RawName() noexcept
				{
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
							sv.find( START_FLAG )
						  + sizeof( START_FLAG ) - 1
						};

					if constexpr( is_template_v< T >
							   || std::is_arithmetic_v< T > )
						{
							return sv.substr( start, end - start );
						}
					else
						{
							constexpr std::size_t blank
								{
									sv.find( ' ', start )
								};

							FC_ASSERT_LOG( blank < end
										&& sv[ end - 1 ] != ' '
										&& sv.rfind( ' ', end ) == blank,
										"class name format is not match" );

							constexpr std::size_t start2
								{
									blank + 1
								};

							return sv.substr( start2, end - start2 );
						}
				}

			/*
			 @	Format name
			 */
			static stl::string FormatName( const std::string_view& ) noexcept;
		};
}