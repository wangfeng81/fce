/*
**
**	Delegate.
**
		support all function
		and delegate chain self-deleting

			Delegate => Chain => Group

			Invoker | Filter

	Copyright (c) WangFeng since 2006...
*/

#pragma push_macro("new")
#undef new

	#include "delegate/FunctionSlot.h"
	#include "delegate/SingleSlot.h"
	#include "delegate/SlotChain.h"
	#include "delegate/SlotGroup.h"

#pragma pop_macro("new")