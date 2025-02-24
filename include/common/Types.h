/*
**
**	Base types.
**
		Copyright (c) WangFeng since 2006...
*/

#pragma once

#include "../platform/Platform.h"


namespace fce
{
	/*	Common
	 */
	using Void		= void;
	using Bool		= bool;

	using Handle	= void*;

	/*	Char
	 */
	using Char		= char;
	using WChar		= wchar_t;

	using Char8		= char8_t;
	using Char16	= char16_t;
	using Char32	= char32_t;

	/*	Float
	 */
	using Float32	= float;
	using Float64	= double;

	using Float		= Float32;

	/*	Integer
	 */
	using Int8		= int8_t;
	using Int16		= int16_t;
	using Int32		= int32_t;
	using Int64		= int64_t;

	using UInt8		= uint8_t;
	using UInt16	= uint16_t;
	using UInt32	= uint32_t;
	using UInt64	= uint64_t;

	/*	Data
	 */
	using Byte		= UInt8;
	using Word		= UInt16;
	using DWord		= UInt32;
	using QWord		= UInt64;


	/*	Variable length integer
	 */
	#ifdef FC_64_BIT
		using Int	= Int64;
		using UInt	= UInt64;
	#elif FC_32_BIT
		using Int	= Int32;
		using UInt	= UInt32;
	#endif


	/*	Unicode or MBCS
	 */
	#ifdef UNICODE
		using TChar = WChar;
		#define TXT( text ) L##text
	#else
		using TChar = Char;
		#define TXT( text ) text
	#endif
}



namespace fce
{
	/*	Type range
	 */
	constexpr Int8		Int8_MIN			= INT8_MIN;
	constexpr Int16		Int16_MIN			= INT16_MIN;
	constexpr Int32		Int32_MIN			= INT32_MIN;
	constexpr Int64		Int64_MIN			= INT64_MIN;

	constexpr Int8		Int8_MAX			= INT8_MAX;
	constexpr Int16		Int16_MAX			= INT16_MAX;
	constexpr Int32		Int32_MAX			= INT32_MAX;
	constexpr Int64		Int64_MAX			= INT64_MAX;

	constexpr UInt8		UInt8_MAX			= UINT8_MAX;
	constexpr UInt16	UInt16_MAX			= UINT16_MAX;
	constexpr UInt32	UInt32_MAX			= UINT32_MAX;
	constexpr UInt64	UInt64_MAX			= UINT64_MAX;

	constexpr Byte		Byte_MAX			= UInt8_MAX;
	constexpr Word		Word_MAX			= UInt16_MAX;
	constexpr DWord		DWord_MAX			= UInt32_MAX;
	constexpr QWord		QWord_MAX			= UInt64_MAX;

	constexpr Float32	Float32_MIN			= FLT_MIN;
	constexpr Float32	Float32_MAX			= FLT_MAX;
	constexpr Float64	Float64_MIN			= DBL_MIN;
	constexpr Float64	Float64_MAX			= DBL_MAX;

	constexpr Float32	Float32_EPSILON		= FLT_EPSILON;
	constexpr Float64	Float64_EPSILON		= DBL_EPSILON;

	constexpr Float32	Float32_DIG			= FLT_DIG;
	constexpr Float64	Float64_DIG			= DBL_DIG;

	constexpr Float32	Float32_PRECISION	= 1e-6F;
	constexpr Float64	Float64_PRECISION	= 1e-15;

	constexpr Float		Float_MIN			= Float32_MIN;
	constexpr Float		Float_MAX			= Float32_MAX;
	constexpr Float		Float_EPSILON		= Float32_EPSILON;
	constexpr Float		Float_PRECISION		= Float32_PRECISION;


	/*	Float constant
	 */
	constexpr Float		Float_ZERO			= Float( 0 );
	constexpr Float		Float_ONE			= Float( 1 );
	constexpr Float		Float_TWO			= Float( 2 );
}



namespace fce
{
	/*	TYPE
	 */
	enum struct TYPE
		:
			Byte
		{
			BOOL,

			CHAR,
			WCHAR,
			CHAR8,
			CHAR16,
			CHAR32,

			INT8,			UINT8,
			INT16,			UINT16,
			INT32,			UINT32,
			INT64,			UINT64,
			FLOAT32,		FLOAT64,

			VECTOR2_B,
			VECTOR3_B,
			VECTOR4_B,

			VECTOR2_I8,		VECTOR2_U8,
			VECTOR2_I16,	VECTOR2_U16,
			VECTOR2_I32,	VECTOR2_U32,
			VECTOR2_I64,	VECTOR2_U64,
			VECTOR2_F32,	VECTOR2_F64,

			VECTOR3_I8,		VECTOR3_U8,
			VECTOR3_I16,	VECTOR3_U16,
			VECTOR3_I32,	VECTOR3_U32,
			VECTOR3_I64,	VECTOR3_U64,
			VECTOR3_F32,	VECTOR3_F64,

			VECTOR4_I8,		VECTOR4_U8,
			VECTOR4_I16,	VECTOR4_U16,
			VECTOR4_I32,	VECTOR4_U32,
			VECTOR4_I64,	VECTOR4_U64,
			VECTOR4_F32,	VECTOR4_F64,

			MATRIX_2X2_F32,	MATRIX_2X2_F64,
			MATRIX_2X3_F32,	MATRIX_2X3_F64,
			MATRIX_2X4_F32,	MATRIX_2X4_F64,

			MATRIX_3X2_F32,	MATRIX_3X2_F64,
			MATRIX_3X3_F32,	MATRIX_3X3_F64,
			MATRIX_3X4_F32,	MATRIX_3X4_F64,

			MATRIX_4X2_F32,	MATRIX_4X2_F64,
			MATRIX_4X3_F32,	MATRIX_4X3_F64,
			MATRIX_4X4_F32,	MATRIX_4X4_F64,

			MAXIMUM,

			MAXIMUM_BASE	= VECTOR2_B,

			VECTOR2			= VECTOR2_F32,
			VECTOR3			= VECTOR3_F32,
			VECTOR4			= VECTOR4_F32,

			MATRIX2			= MATRIX_2X2_F32,
			MATRIX3			= MATRIX_3X3_F32,
			MATRIX4			= MATRIX_4X4_F32,

			MATRIX_2X3		= MATRIX_2X3_F32,
			MATRIX_2X4		= MATRIX_2X4_F32,
			MATRIX_3X2		= MATRIX_3X2_F32,
			MATRIX_3X4		= MATRIX_3X4_F32,
			MATRIX_4X2		= MATRIX_4X2_F32,
			MATRIX_4X3		= MATRIX_4X3_F32,
		};


	/*
	 !	Size of type
	 */
	struct FC_API final
		{
			/*
			 @	sizeOf[ TYPE ]
			 */
			constexpr UInt8 operator [] ( const TYPE type ) const
				{
					assert( type < TYPE::MAXIMUM );

					return SIZE[ UInt8( type ) ];
				}

		private:
			/*	Type size list
			 */
			static constexpr UInt8 SIZE[]
				{
					sizeof( Bool	),
					
					sizeof( Char	),
					sizeof( WChar	),
					sizeof( Char8	),
					sizeof( Char16	),
					sizeof( Char32	),	

					sizeof( Int8	),	sizeof( UInt8	),
					sizeof( Int16	),	sizeof( UInt16	),
					sizeof( Int32	),	sizeof( UInt32	),
					sizeof( Int64	),	sizeof( UInt64	),
					sizeof( Float32	),	sizeof( Float64	),

					sizeof( Bool	) * 2,
					sizeof( Bool	) * 3,
					sizeof( Bool	) * 4,

					sizeof( Int8	) * 2,	sizeof( UInt8	) * 2,
					sizeof( Int16	) * 2,	sizeof( UInt16	) * 2,
					sizeof( Int32	) * 2,	sizeof( UInt32	) * 2,
					sizeof( Int64	) * 2,	sizeof( UInt64	) * 2,
					sizeof( Float32	) * 2,	sizeof( Float64	) * 2,

					sizeof( Int8	) * 3,	sizeof( UInt8	) * 3,
					sizeof( Int16	) * 3,	sizeof( UInt16	) * 3,
					sizeof( Int32	) * 3,	sizeof( UInt32	) * 3,
					sizeof( Int64	) * 3,	sizeof( UInt64	) * 3,
					sizeof( Float32	) * 3,	sizeof( Float64	) * 3,

					sizeof( Int8	) * 4,	sizeof( UInt8	) * 4,
					sizeof( Int16	) * 4,	sizeof( UInt16	) * 4,
					sizeof( Int32	) * 4,	sizeof( UInt32	) * 4,
					sizeof( Int64	) * 4,	sizeof( UInt64	) * 4,
					sizeof( Float32	) * 4,	sizeof( Float64	) * 4,

					sizeof( Float32	) * 4,	sizeof( Float64	) * 4,
					sizeof( Float32	) * 6,	sizeof( Float64	) * 6,
					sizeof( Float32	) * 8,	sizeof( Float64	) * 8,

					sizeof( Float32 ) * 6,	sizeof( Float64 ) * 6,
					sizeof( Float32 ) * 9,	sizeof( Float64 ) * 9,
					sizeof( Float32 ) * 12,	sizeof( Float64 ) * 12,

					sizeof( Float32 ) * 8,	sizeof( Float64 ) * 8,
					sizeof( Float32 ) * 12,	sizeof( Float64 ) * 12,
					sizeof( Float32 ) * 16,	sizeof( Float64 ) * 16
				};
		}
	/*
	 @	Instance
	 */
	constexpr sizeOf;
}



namespace fce
{
	 /*
	  !	RESULT

			Only reasonable errors
			that need to be handled at runtime are returned

			Other design errors
			are handled with assertions during debugging
	  */
	enum struct RESULT
		:
			Word
		{
			SUCCESS,
			FAILURE,
			UNKNOWN,
			EXCEPTION,

			NULL_POINTER,
			DATA_ERROR,

			OBJECT_NOT_FOUND,
			OBJECT_ALREADY_EXISTS,

			NOT_ENOUGH_STORAGE,

			MAXIMUM
		};


	/*
	 !	Path
	 */
	using Path = std::filesystem::path;
}



namespace fce
{
	 /*
	  !	COMPARE
	  */
	enum struct COMPARE
		:
			Byte
		{
			NEVER,
			ALWAYS,

			LESS,
			EQUAL,
			GREATER,

			NOT_EQUAL,
			LESS_EQUAL,
			GREATER_EQUAL,

			MAXIMUM
		};
}



/**
 !	Meta
 */
namespace fce
{
	/*	concept		Template object
	 */
	template< class T >

	static constexpr bool is_template_v = false;

	template< template< typename... >
			  class T, typename ...Args >

	static constexpr bool is_template_v
						  < T< Args... > > = true;

	/*	concept		Template
	 */
	template< template< typename... >
			  class T, class Derived >

	static constexpr bool is_template_of_v = false;

	template< template< typename... >
			  class T, typename ...Args >

	static constexpr bool is_template_of_v
						  < T, T< Args... > > = true;

	template< class Derived,
			  template< typename... > class T  >

	concept Template = is_template_of_v< T,
										 std::remove_cvref_t< Derived > >;

	/*	concept		Assignable from R to T
	 */
	template< class L, class R >

	concept Assignable_from = std::is_assignable< L, R >::value;

	/*	concept		Const of T
	 */
	template< class T >

	concept Const_of = std::is_const< std::remove_pointer_t<
									  std::remove_reference_t< T > > >::value;

	/*	concept		Any of T
	 */
	template< class T, class... Types >

	concept Any_of = std::disjunction< std::is_same< T, Types >... >::value;

	/*	concept		Enum
	 */
	template< typename T >

	concept Enum_value = std::is_enum< std::remove_cvref_t< T > >::value;

	/*	concept		Signed integer
	 */
	template< typename T >

	concept Signed_integer = Any_of< std::remove_cvref_t< T >,
									 Int8, Int16, Int32, Int64,
									 long >;

	/*	concept		Unsigned integer
	 */
	template< typename T >

	concept Unsigned_integer = Any_of< std::remove_cvref_t< T >,
									   UInt8, UInt16, UInt32, UInt64,
									   unsigned long >;

	/*	concept		Integer
	 */
	template< typename T >

	concept Integer = Signed_integer< T >
				   || Unsigned_integer< T >;

	/*	concept		Floating point
	 */
	template< typename T >

	concept Floating_point = std::floating_point< std::remove_cvref_t< T > >;

	/*	concept		Real number
	 */
	template< typename T >

	concept Real_number = Integer< T >
					   || Floating_point< T >;

	/*	concept		UTF char
	 */
	template< typename T >

	concept UTF_char = Any_of< std::remove_cvref_t< T >,
							   Char8, Char16, Char32 >;

	/*	concept		Character
	 */
	template< typename T >

	concept Character = Any_of< std::remove_cvref_t< T >,
								Char, WChar, Char8, Char16, Char32 >;

	/*	concept		C_string_pointer
	 */
	template< typename T >

	concept C_string_pointer = Any_of< std::remove_cvref_t< T >,
									   Char*,
									   WChar*,
									   Char8*,
									   Char16*,
									   Char32*,
									   const Char*,
									   const WChar*,
									   const Char8*,
									   const Char16*,
									   const Char32* >;

	/*	concept		C_string_array
	 */
	template< typename T >

	concept C_string_array = std::is_array< std::remove_cvref_t< T > >::value
						  && Character< std::ranges::range_value_t< T > >;

	/*	concept		C_string
	 */
	template< typename T >

	concept C_string = C_string_array< T >
					|| C_string_pointer< T >;

	/*	concept		Arithmetic
	 */
	template< typename T >

	concept Arithmetic = std::is_arithmetic< std::remove_cvref_t< T > >::value
					  || Enum_value< T >
					  || Character< T >;

	/*	Does Byte order requires conversion
	 */
	template< Arithmetic T >

	static constexpr bool endian_requires_conversion = std::endian::native
													!= std::endian::little
													&& sizeof( T ) > 1;

	/*	concept		C fixed_array
	 */
	template< class T >

	concept C_fixed_array = std::is_array< std::remove_cvref_t< T > >::value;

	/*	concept		Stl fixed_array
	 */
	template< class T >

	concept Stl_fixed_array = std::same_as< std::remove_cvref_t< T >,
											std::array< typename T::value_type,
														std::tuple_size< T >::value > >;

	/*	concept		Fixed array
	 */
	template< class T >

	concept Fixed_array = C_fixed_array< T >
					   || Stl_fixed_array< T >;

	/*	concept		Array_object
	 */
	template< class T >

	concept Array_object = Fixed_array< T >
						|| Template< T, std::vector >;


	/*	concept		Stl_sequence_container
	 */
	template< class T >

	concept Stl_sequence_container = Stl_fixed_array< T >
								  || Template< T, std::vector >
								  || Template< T, std::deque >
								  || Template< T, std::list >
								  || Template< T, std::forward_list >;

	/*	concept		Stl_associative_container
	 */
	template< class T >

	concept Stl_associative_container = Template< T, std::set >
									 || Template< T, std::map >
									 || Template< T, std::multiset >
									 || Template< T, std::multimap >;

	/*	concept		Stl_unordered_container
	 */
	template< class T >

	concept Stl_unordered_container = Template< T, std::unordered_set >
								   || Template< T, std::unordered_map >
								   || Template< T, std::unordered_multiset >
								   || Template< T, std::unordered_multimap >;

	/*	concept		Stl_adapter_container
	 */
	template< class T >

	concept Stl_adapter_container = Template< T, std::stack >
								 || Template< T, std::queue >
								 || Template< T, std::priority_queue >;

	/*	concept		Stl_container
	 */
	template< class T >

	concept Stl_container = Stl_sequence_container< T >
						 || Stl_associative_container< T >
						 || Stl_unordered_container< T >
						 || Stl_adapter_container< T >;

	/*	concept		container object
	 */
	template< class T >

	concept Container_object = C_fixed_array< T >
							|| Stl_container< T >;

	/*	concept		Container with resize
	 */
	template< class T >

	concept Container_with_resize = Container_object< T > &&

									requires( T& c )
										{
											c.resize( 0 );
										};

	/*	concept		Container with reserve
	 */
	template< class T >

	concept Container_with_reserve = Container_object< T > &&

									requires( T& c )
										{
											c.reserve( 0 );
										};

	/*	concept		Container with emplace
	 */
	template< class T >

	concept Container_with_emplace = Container_object< T > &&

									 requires( T& c )
										{
											c.emplace();
										};


	/*	concept		Stl_pair
	 */
	template< class T >

	concept Stl_pair = Template< T, std::pair >;

	/*	concept		Stl_tuple
	 */
	template< class T >

	concept Stl_tuple = Template< T, std::tuple >;

	/*	concept		Stl_pointer
	 */
	template< class T >

	concept Stl_pointer = Template< T, std::unique_ptr >
					   || Template< T, std::shared_ptr >
					   || Template< T, std::weak_ptr >;

	/*	concept		Stl_string
	 */
	template< class T >

	concept Stl_string = Template< T, std::basic_string >;

	template< typename C, class T >

	concept Stl_string_t = std::same_as< std::remove_cvref_t< T >,
										 std::basic_string< C, typename std::remove_cvref_t< T >::traits_type,
															   typename std::remove_cvref_t< T >::allocator_type > >;

	template< class T >

	concept Stl_mstring = Stl_string_t< Char, T >;

	template< class T >

	concept Stl_wstring = Stl_string_t< WChar, T >;

	template< class T >

	concept Stl_u8string = Stl_string_t< Char8, T >;

	template< class T >

	concept Stl_u16string = Stl_string_t< Char16, T >;

	template< class T >

	concept Stl_u32string = Stl_string_t< Char32, T >;

	/*	concept		Stl_string_view
	 */
	template< typename T >

	concept Stl_string_view = Template< T, std::basic_string_view >;

	/*	concept		Cpp_string
	 */
	template< typename T >

	concept Cpp_string = Stl_string< T >
					  || Stl_string_view< T >;


	/*	concept		Character string
	 */
	template< typename T >

	concept Character_string = C_string< T >
							|| Cpp_string< T >;


	/*	concept		Pointer object
	 */
	template< class T >

	concept Pointer_object = std::is_pointer<
							 std::remove_reference_t< T > >::value
						&& ! C_string_pointer< T >
						&& ! C_fixed_array< T >;

	/*
	 !	Get the index of the first element of this type in the tuple

			return count of elements if not exist
	 */
	template< typename T,
			  Stl_tuple Tuple >

	struct Tuple_element_index
		{
			static constexpr std::size_t value{ 0 };
		};

	template< typename T,
			  typename ...Types >

	struct Tuple_element_index< T,
								std::tuple< T, Types... > >
		{
			static constexpr std::size_t value{ 0 };
		};

	template< typename T,
			  typename U,
			  typename ...Types >

	struct Tuple_element_index< T,
								std::tuple< U, Types... > >
		{
			static constexpr std::size_t value
				{
					1 + Tuple_element_index< T,
											 std::tuple< Types... > >::value
				};
		};


	/*
	 !	Get the type of the tuple element by index
	 */
	template< std::size_t I,
			  Stl_tuple Tuple >
	struct Tuple_element_type;

	template< typename T,
			  typename ...Types >

	struct Tuple_element_type< 0,
							   std::tuple< T, Types... > >
		{
			using type = T;
		};

	template< std::size_t I,
			  typename T,
			  typename ...Types >

	struct Tuple_element_type< I,
							   std::tuple< T, Types... > >
		{
			using type = Tuple_element_type< I - 1,
											 std::tuple< Types... > >::type;
		};


	/*
	 !	Log2 complie-time integer constant
	 */
	template< std::size_t N >

	struct Log2
		{
			static constexpr std::size_t value
				{
					1 + Log2< ( N >> 1 ) >::value
				};
		};

	template<>

	struct Log2< 1 >
		{
			static constexpr std::size_t value{ 0 };
		};


	/*
	 !	Detect whether the equals operator exists Detect
	 */
	template< typename L,
			  typename R = L >

	class Has_equals_operator final
		{
			/*
			 @	Test failed
			 */
			template< typename T >

			static Void Match( ... );

			/*
			 @	Test succeeded
			 */
			template< typename T >

			static auto Match( std::nullptr_t )
				->
				decltype( std::declval< L >()
					   == std::declval< T >() );

			/*	Matching type
			*/
			using Type = decltype( Match< R >( nullptr ) );

		public:
			/*
			 @	Result value
			 */
			static constexpr Bool value = std::convertible_to
											   < Type, Bool >;
		};


	/*
	 !	Invoke the equals operator when it exists
	 */
	template< typename L, typename R >

	FC_INLINE Bool Equal( const L& a, const R& b )
		{
			/*	Conditional compile of ==
			*/
			if constexpr( Has_equals_operator< L, R >::value )
				{
					return a == b;
				}
			/*	Conditional compile of memcmp
			*/
			else if constexpr( std::same_as< L, R > )
				{
					return ! memcmp( &a, &b, sizeof( L ) );
				}
			/*	Conditional compile of false
			*/
			else
				{
					return false;
				}
		}


	/*
	 !	Deduce const type by self
	 */
	#define DEDUCE_CONST_BY_SELF( T )									\
																		\
			std::conditional_t											\
			<															\
				Const_of< decltype( self ) >,							\
				const T*, T*											\
			>


	/*
	 !	Is invocable the member function

			Unlike the concept,
			it can have "friend" permission
	 */
	#define IS_INVOCABLE_MEMBER( FUNCTION )								\
																		\
			template< class T,											\
					  typename ...Args >								\
																		\
			class Is_invocable_member_##FUNCTION final					\
				{														\
					/*
					 @	Test failed
					 */													\
					template< typename U >								\
																		\
					static Void Match( ... );							\
																		\
					/*
					 @	Test succeeded
					 */													\
					template< typename U >								\
																		\
					static auto Match( U* p )							\
						->												\
						decltype( p->##FUNCTION							\
									(									\
										std::declval< Args >()...		\
									),									\
								  std::true_type() );					\
																		\
					/*	Matching type
					*/													\
					using Type = decltype( Match< T >( nullptr ) );		\
																		\
				public:													\
					/*
					 @	Result value
					 */													\
					static constexpr Bool value = std::is_same			\
													<					\
														Type,			\
														std::true_type	\
													>					\
													::value;			\
				};
}



/**
 !	Predefines
 */
namespace fce
{
	/*	Engine
	 */
	class Engine;
	class Object;
	class ObjectEx;

	/*	Math
	 */
	struct AABox3;

	struct Vector3;
	struct Vector4;

	struct Affine2;
	struct Affine3;
	struct Matrix2;
	struct Matrix3;
	struct Matrix4;
	struct Matrix3X2;
	struct Matrix4X3;

	struct Quaternion;

	/*	Input
	 */
	class Ime;
	class Mouse;
	class Keyboard;

	/*	Platform
	 */
	class DynLib;
	class Window;
	class Assertion;
	class Clipboard;
	class Application;

	/*	Graphics
	 */
	class Image;
	class XImage;
	class XModel3D;
	class Graphics;

	class IndexSet;
	class VertexSet;

	class Shader;
	class Sampler;
	class Texture2;
	class Material;
	class RenderPass;
	class FrameBuffer;
	class IndexBuffer;
	class VertexBuffer;
	class UniformBuffer;

	class Descriptor;
	class ColorBlend;
	class Multisample;
	class DepthStencil;
	class Rasterization;
	class DescriptorSet;
	class DescriptorPool;
	class RenderPipeline;

	/*	Component
	 */
	class Entity;
	class Component;

	class Camera;

	/*	Common
	 */
	class Crc32;
	class Timer;
	class Buffer;
	class String;
	class Charset;
	class Director;
	class Singleton;

	class JobThread;
	class AsyncObject;

	/*	Management
	 */
	class JobManagement;
	class TimerManagement;
	class AssetManagement;
	class MemoryManagement;
	class VersionManagement;
	class StorageManagement;
	class MaterialManagement;

	/*	Scene
	 */
	class Scene;
	class Renderer;
	class SceneGroup;


	/*	Scene node
	 */
	namespace inner::scene
		{
			class Node;
		}

	/*	Window surface
	 */
	namespace inner::window
		{
			class Surface;
		}

	/*	Render pipeline
	 */
	namespace inner::renderPipeline
		{
			struct Info;
		}

	/*	Gui
	 */
	namespace gui
		{
			class Picture;
		}
}



/*
 @	Disable default copy & move constructor
 */
#define DISABLE_COPY_MOVE( T )		\
									\
		T( const T&& ) = delete;

#define DISABLE_COPY( T )			\
									\
		T( const T& ) = delete;

#define DISABLE_MOVE( T )			\
									\
		T( T&& ) = delete;