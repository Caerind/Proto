#include <Enlivengine/System/Debugger.hpp>

#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/PlatformTraits.hpp>
#include <debug_break/debugbreak.h>

#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
#include <Windows.h>
#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)

namespace en
{

bool Debugger::IsPresent()
{
	#if defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
	return IsDebuggerPresent();
	#else
	return false;
	#endif // defined(ENLIVE_PLATFORM_WINDOWS) && defined(ENLIVE_COMPILER_MSVC)
}

void Debugger::Break()
{
	if (IsPresent())
	{
		debug_break();
	}
}
	
} // namespace en
