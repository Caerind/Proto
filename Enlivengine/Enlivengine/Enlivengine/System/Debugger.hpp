#pragma once

// TODO : Breakpoints with features : Togglables/Conditions/Actions ?
// TODO : DataBreakpoints ?
// TODO : Callstack ?

namespace en
{
	
class Debugger
{
public:
	Debugger() = delete;
	
	static bool IsPresent();
	static void Break();
};
	
} // namespace en