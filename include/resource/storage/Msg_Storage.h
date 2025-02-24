/*
**
**	Storage messages.
**
		Copyright (c) WangFeng since 2000...
*/

#pragma once

#include "../../common/Archive.h"
#include "../../common/Job.h"


/*
 !	Storage inner kit
 */
namespace fce::inner::storage
{
	/*	EVENT
	 */
	enum struct EVENT
		:
			Byte
		{
			LOAD,
			SAVE,
			COPY,
			RENAME,
			REMOVE,

			ASY_OP_LOAD,
			ASY_OP_SAVE,
			ASY_OP_COPY,
			ASY_OP_RENAME,
			ASY_OP_REMOVE,
		};


	/*
	 !	Message
	 */
	class FC_API Message
		:
		public Object
		{
			POLYMORPHIC_POOL( Message,
							  MemoryManagement );

		public:
			/*	EVENT
			*/
			const EVENT e;

			/*	path
			*/
			const Path path;

			/*	RESULT
			*/
			const RESULT result;

		protected:
			/*	Constructor & Destructor
			*/
			~Message() override = default;

			Message( const EVENT,
						   Path&& ) noexcept;
		};


	/*	Delegate handler

			When the job is completed,
			it's called in the main thread
	*/
	using Delegate = fce::Delegate< Void,
									const Message& >;


	/*
	 !	Message async
	 */
	class FC_API Msg_Async
		:
		public Message
		{
			friend Application;

			friend StorageManagement;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Async() override = default;

			Msg_Async( const EVENT,
							 Path&&,
							 Delegate&& ) noexcept;

		private:
			/*	callback
			*/
			Delegate callback;
		};

	/*
	 !	Msg load
	 */
	class FC_API Msg_Load final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Load,
							  MemoryManagement );

		public:
			/*	Data
			*/
			mutable Buffer data;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Load() override = default;

			Msg_Load( Path&&,
					  Delegate&& ) noexcept;
		};

	/*
	 !	Msg save
	 */
	class FC_API Msg_Save final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Save,
							  MemoryManagement );

		public:
			/*	Data
			*/
			mutable Buffer data;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Save() override = default;

			Msg_Save( Path&&,
					  Buffer&&,
					  Delegate&& ) noexcept;
		};

	/*
	 !	Msg copy
	 */
	class FC_API Msg_Copy final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Copy,
							  MemoryManagement );

		public:
			/*	Destination path
			*/
			const Path dstPath;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Copy() override = default;

			Msg_Copy( Path&& src,
					  Path&& dst,
					  Delegate&& ) noexcept;
		};

	/*
	 !	Msg rename
	 */
	class FC_API Msg_Rename final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Rename,
							  MemoryManagement );

		public:
			/*	Destination path
			*/
			const Path dstPath;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Rename() override = default;

			Msg_Rename( Path&& src,
						Path&& dst,
						Delegate&& ) noexcept;
		};

	/*
	 !	Msg remove
	 */
	class FC_API Msg_Remove final
		:
		public Msg_Async
		{
			POLYMORPHIC_POOL( Msg_Remove,
							  MemoryManagement );

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_Remove() override = default;

			Msg_Remove( Path&&,
						Delegate&& ) noexcept;
		};


	/*
	 !	Async operation
	 */
	class FC_API Msg_AsyOp
		:
		public Message
		{
			friend StorageManagement;

		protected:
			/*	Constructor & Destructor
			*/
			~Msg_AsyOp() override = default;

			Msg_AsyOp( const EVENT,
							 Path&& ) noexcept;

		private:
			/*	Async object
			*/
			AsyncObject asyObj;
		};

	/*
	 !	AsyOp load
	 */
	class FC_API AsyOp_Load final
		:
		public Msg_AsyOp
		{
			POLYMORPHIC_POOL( AsyOp_Load,
							  MemoryManagement );

		public:
			/*	Data
			*/
			mutable Buffer data;

		protected:
			/*	Constructor & Destructor
			*/
			~AsyOp_Load() override = default;

			AsyOp_Load( Path&& ) noexcept;
		};

	/*
	 !	AsyOp save
	 */
	class FC_API AsyOp_Save final
		:
		public Msg_AsyOp
		{
			POLYMORPHIC_POOL( AsyOp_Save,
							  MemoryManagement );

		public:
			/*	Data
			*/
			mutable Buffer data;

		protected:
			/*	Constructor & Destructor
			*/
			~AsyOp_Save() override = default;

			AsyOp_Save( Path&&,
						Buffer&& ) noexcept;
		};

	/*
	 !	AsyOp copy
	 */
	class FC_API AsyOp_Copy final
		:
		public Msg_AsyOp
		{
			POLYMORPHIC_POOL( AsyOp_Copy,
							  MemoryManagement );

		public:
			/*	Destination path
			*/
			const Path dstPath;

		protected:
			/*	Constructor & Destructor
			*/
			~AsyOp_Copy() override = default;

			AsyOp_Copy( Path&& src,
						Path&& dst ) noexcept;
		};

	/*
	 !	AsyOp rename
	 */
	class FC_API AsyOp_Rename final
		:
		public Msg_AsyOp
		{
			POLYMORPHIC_POOL( AsyOp_Rename,
							  MemoryManagement );

		public:
			/*	Destination path
			*/
			const Path dstPath;

		protected:
			/*	Constructor & Destructor
			*/
			~AsyOp_Rename() override = default;

			AsyOp_Rename( Path&& src,
						  Path&& dst ) noexcept;
		};

	/*
	 !	AsyOp remove
	 */
	class FC_API AsyOp_Remove final
		:
		public Msg_AsyOp
		{
			POLYMORPHIC_POOL( AsyOp_Remove,
							  MemoryManagement );

		protected:
			/*	Constructor & Destructor
			*/
			~AsyOp_Remove() override = default;

			AsyOp_Remove( Path&& ) noexcept;
		};
}