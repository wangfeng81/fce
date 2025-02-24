/*
**
**	Utility.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "String.h"


namespace fce::stl
{
	/*
	 @	remove_all_t
	 */
	template< class T >

	using remove_all_t = std::remove_cv_t<
						 std::remove_pointer_t<
						 std::remove_reference_t< T > > >;

	/*
	 @	std::distance
	 */
	FC_INLINE [[nodiscard]] constexpr auto distance( const std::ranges::range auto& container ) NOEXCEPT
		{
			return std::distance( container.begin(),
								  container.end() );
		}

	/*
	 @	std::remove
	 */
	FC_INLINE [[nodiscard]] constexpr auto remove( std::ranges::range auto&& container,
																	  auto&& value ) NOEXCEPT
		{
			return std::remove( container.begin(),
								container.end(), value );
		}

	/*
	 @	std::erase
	 */
	FC_INLINE [[nodiscard]] constexpr auto erase( std::ranges::range auto&& container,
																	 auto&& value ) NOEXCEPT
		{
			return std::erase( container.begin(),
							   container.end(), value );
		}

	/*
	 @	std::find
	 */
	FC_INLINE [[nodiscard]] constexpr auto find( std::ranges::range auto&& container,
																	auto&& value ) NOEXCEPT
		{
			return std::find( container.begin(),
							  container.end(), value );
		}

	/*
	 @	std::find_if
	 */
	FC_INLINE [[nodiscard]] constexpr auto find_if( std::ranges::range auto&& container,
																	   auto&& func ) NOEXCEPT
		{
			return std::find_if( container.begin(),
								 container.end(), func );
		}

	/*
	 @	std::sort
	 */
	FC_INLINE constexpr auto sort( std::ranges::range auto&& container,
													  auto&& func ) NOEXCEPT
		{
			return std::sort( container.begin(),
							  container.end(), func );
		}

	/*
	 @	for_each	std::tuple
	 */
	FC_INLINE constexpr auto for_each( Stl_tuple auto&& tp,
												 auto&& func ) NOEXCEPT
		{
			auto f = [ &func ]( auto&&... args ) NOEXCEPT
				{
					return ( func( args ), ... );
				};

			return std::apply( f, tp );
		}

	/*
	 @	std::transform ex
	 */
	template< std::ranges::sized_range Src,
			  std::ranges::sized_range Dst,
			  typename Func >

	FC_INLINE void transform( const Src& src,
									Dst& dst,
							  const Func& func ) NOEXCEPT
		{
			dst.resize( std::ranges::size( src ) );

			std::transform( src.begin(),
							src.end(),
							dst.begin(),
							func );
		}

	/*
	 @	std::transform imp
	 */
	template< std::ranges::range Src,
			  std::ranges::range Dst >

	FC_INLINE void transform_imp( const Src& src,
										Dst& dst ) NOEXCEPT
		{
			if constexpr( Template< Dst, std::vector > )
				{
					dst.reserve( std::ranges::size( src ) );
				}

			for( const auto& val : src )
				{
					if constexpr( Template< Dst, std::forward_list > )
						{
							dst.emplace_front( val );
						}
					else if constexpr( Stl_sequence_container< Dst > )
						{
							dst.emplace_back( val );
						}
					else
						{
							dst.emplace( val );
						}
				}
		}

	template< std::ranges::range Src,
			  std::ranges::range Dst,
			  typename Func >

	FC_INLINE void transform_imp( const Src& src,
										Dst& dst,
								  const Func& func ) NOEXCEPT
		{
			if constexpr( Template< Dst, std::vector > )
				{
					dst.reserve( std::ranges::size( src ) );
				}

			for( const auto& val : src )
				{
					if constexpr( Template< Dst, std::forward_list > )
						{
							dst.emplace_front( func( val ) );
						}
					else if constexpr( Stl_sequence_container< Dst > )
						{
							dst.emplace_back( func( val ) );
						}
					else
						{
							dst.emplace( func( val ) );
						}
				}
		}

	/*
	 @	std::transform ex
	 */
	template< Stl_container T >

	FC_INLINE auto transform( const std::ranges::range auto& src ) NOEXCEPT
		{
			std::remove_cvref_t< T > dst;

			transform_imp( src, dst );

			return std::move( dst );
		}

	template< Stl_container T >

	FC_INLINE auto transform( const std::ranges::range auto& src,
							  const auto& func ) NOEXCEPT
		{
			std::remove_cvref_t< T > dst;

			transform_imp( src, dst, func );

			return std::move( dst );
		}

	template< template< typename... > class T = vector >

	FC_INLINE auto transform( const std::ranges::range auto& src,
							  const auto& func ) NOEXCEPT
		{
			using Value = std::remove_cvref_t
						  < decltype( func( *src.begin() ) ) >;

			return transform< T< Value > >( src, func );
		}
}



namespace fce
{
	/*
	 @	Floating-point comparisons
	 */
	FC_INLINE Bool Equal   ( Float32 a, Float32 b, Float32 eps = Float32_EPSILON ) noexcept { return abs( a - b ) <  eps; }

	FC_INLINE Bool Equal   ( Float64 a, Float64 b, Float64 eps = Float64_EPSILON ) noexcept { return abs( a - b ) <  eps; }

	FC_INLINE Bool NotEqual( Float32 a, Float32 b, Float32 eps = Float32_EPSILON ) noexcept { return abs( a - b ) >= eps; }

	FC_INLINE Bool NotEqual( Float64 a, Float64 b, Float64 eps = Float64_EPSILON ) noexcept { return abs( a - b ) >= eps; }
}



/*
 @	Convert enum => impl value
 */
#define CONVERT_IMPL_ENUM_VALUE( T )											\
																				\
		FC_INLINE [[nodiscard]] static CONSTEXPR auto Convert( const T type )	\
			{																	\
				FC_ASSERT( type < T::MAXIMUM );									\
																				\
				return Impl::##T##_LIST[ UInt8( type ) ];						\
			}

/*
 @	Combine enum => impl value
 */
#define COMBINE_IMPL_ENUM_VALUE( T )											\
																				\
		[[nodiscard]] static DWord Combine( const stl::vector< T >& arrTypes )	\
			{																	\
				DWord bits{ 0 };												\
																				\
				for( const T type : arrTypes )									\
					{															\
						bits |= Convert( type );								\
					}															\
																				\
				return bits;													\
			}



/*
 @	Forward function decl
 */
#define FORWARD_FUNCTION_DECL( F )												\
																				\
		template< typename ...Args >											\
																				\
		decltype( auto ) F( Args&&... ) NOEXCEPT;

#define FORWARD_FUNCTION_STATIC_DECL( F )										\
																				\
		template< typename ...Args >											\
																				\
		static decltype( auto ) F( Args&&... ) NOEXCEPT;



/*
 @	Asset Forward function impl
 */
#define ASSET_FORWARD_FUNCTION_IMPL( T, F )										\
																				\
		template< typename ...Args >											\
																				\
		FC_INLINE decltype( auto ) T::F( Args&& ...args ) NOEXCEPT				\
			{																	\
				return fce::s_AssetMgmt.F< T >									\
						(														\
							this,												\
							std::forward< Args >( args )...						\
						);														\
			}

#define ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, F )								\
																				\
		template< typename ...Args >											\
																				\
		FC_INLINE decltype( auto ) T::F( Args&& ...args ) NOEXCEPT				\
			{																	\
				return fce::s_AssetMgmt.F< T >									\
						(														\
							std::forward< Args >( args )...						\
						);														\
			}

/*
 @	Asset decl ( Syntax sugar )
 */
#define ASSET_SYNTAX_SUGAR_DECL													\
																				\
		FORWARD_FUNCTION_STATIC_DECL( Add )										\
																				\
		FORWARD_FUNCTION_STATIC_DECL( Load )									\
																				\
		FORWARD_FUNCTION_STATIC_DECL( Reload )									\
																				\
		FORWARD_FUNCTION_STATIC_DECL( LoadAsset )								\
																				\
		FORWARD_FUNCTION_STATIC_DECL( LoadAsync )								\
																				\
		FORWARD_FUNCTION_STATIC_DECL( UnloadAsync )								\
																				\
		FORWARD_FUNCTION_STATIC_DECL( RemoveAsync )								\
																				\
		FORWARD_FUNCTION_STATIC_DECL( RenameAsync )

/*
 @	Asset impl ( Syntax sugar )
 */
#define ASSET_SYNTAX_SUGAR_IMPL( T )											\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, Add )							\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, Load )							\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, Reload )							\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, LoadAsset )						\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, LoadAsync )						\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, UnloadAsync )					\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, RemoveAsync )					\
																				\
		ASSET_FORWARD_FUNCTION_STATIC_IMPL( T, RenameAsync )



/**
 !	Includes
 */
#include "utility/Enum.h"
#include "utility/Color.h"
#include "utility/Crc32.h"
#include "utility/Delegate.h"
#include "utility/PrettyFunction.h"