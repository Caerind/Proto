#pragma once

#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Debugger.hpp>

#ifdef ENLIVE_ENABLE_ASSERT
	#define enAssert(expr) \
		if (expr) {} \
		else \
		{ \
			LogError(en::LogChannel::All, 10, "Assertion failed!\nExpr : %s\nFile : %s\nLine : %d\n", #expr, __FILE__, __LINE__); \
			::en::Debugger::Break(); \
		}
#else
	#define enAssert(expr) 
#endif