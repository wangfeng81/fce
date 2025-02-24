/*
**
**	Push constant.
**
		Copyright (c) WangFeng since 2016...
*/

#pragma once

#include "Shader.h"


/**
 !	PushConstant inner kit
 */
namespace fce::inner::pushConstant
{
	/*	STAGE
	 */
	using STAGE = Shader::TYPE;

	/*
	 !	Info
	 */
	struct FC_API Info final
		{
			/*	Model
			*/
			UInt16 size;

			/*	Archive
			*/
			OBJECT_ARCHIVE( Info,
							ARCHIVE_UNIT( size ),
							ARCHIVE_INFO( arrStages, "Stage" ) )

		public:
			/*	Access
			*/
			[[nodiscard]] auto end() const noexcept;

			[[nodiscard]] auto begin() const noexcept;

			[[nodiscard]] UInt32 Count() const noexcept;

			[[nodiscard]] auto GetStage() const;

			[[nodiscard]] STAGE operator [] ( const UInt ) const;

		public:
			/*	Constructor
			*/
			Info( const UInt16 size						= 0,
				  const std::initializer_list< STAGE >& = {} ) noexcept;

		private:
			/*	Stage list
			*/
			stl::vector< STAGE > arrStages;
		};
}



namespace fce
{
	/*
	 !	PushConstant
	 */
	class FC_API PushConstant final
		{
			/*	Friends
			*/
			friend RenderPipeline;

		public:
			/*	Using inner kit
			*/
			using Info = inner::pushConstant::Info;

			using STAGE = inner::pushConstant::STAGE;

		protected:
			/*	Constructor & Destructor
			*/
			DISABLE_COPY_MOVE( PushConstant );

			~PushConstant() = default;

			PushConstant() = default;
		};
}



/**
 !	PushConstant inner kit
 */
namespace fce::inner::pushConstant
{
	/*
	 @	Access
	 */
	FC_INLINE auto Info::end() const noexcept
		{
			return this->arrStages.cend();
		}

	FC_INLINE auto Info::begin() const noexcept
		{
			return this->arrStages.cbegin();
		}

	FC_INLINE UInt32 Info::Count() const noexcept
		{
			return static_cast< UInt32 >
					(
						this->arrStages.size()
					);
		}

	FC_INLINE auto Info::GetStage() const
		{
			return Shader::Combine( this->arrStages );
		}

	FC_INLINE STAGE Info::operator [] ( const UInt idx ) const
		{
			return this->arrStages[ idx ];
		}
}