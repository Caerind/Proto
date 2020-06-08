#include <Enlivengine/System/Preprocessor.hpp>
#include <Enlivengine/System/PlatformDetection.hpp>
#include <Enlivengine/System/CompilerDetection.hpp>
#include <Enlivengine/System/CompilerTraits.hpp>
#include <Enlivengine/System/ByteUnits.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Endianness.hpp>
#include <Enlivengine/System/Debugger.hpp>
#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/System/Time.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/TypeInfo.hpp>

#include <Box2D/Box2D.h>

#include <SFML/Graphics.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <iostream>

enum class SomeEnum
{
	A,
	B,
	C
};

enum class FlagEnum
{
	A = 0x01,
	B = 0x02,
	C = 0x04,
	D = 0x08
};

void MetaEnumTest()
{
	std::cout << "-- EnumTest --" << std::endl;
	std::cout << std::endl;

	// SomeEnum
	std::cout << en::Meta::GetEnumTypeName<SomeEnum>() << std::endl;
	std::cout << en::Meta::GetEnumName(SomeEnum::A) << std::endl;
	for (const auto& element : en::Meta::GetEnumNames<SomeEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Meta::GetEnumName(en::Meta::EnumCast<SomeEnum>(1)) << std::endl;
	std::cout << en::Meta::GetEnumCount<SomeEnum>() << std::endl;

	std::cout << std::endl;

	// FlagEnum
	std::cout << en::Meta::GetEnumTypeName<FlagEnum>() << std::endl;
	std::cout << en::Meta::GetEnumName(FlagEnum::A) << std::endl;
	for (const auto& element : en::Meta::GetEnumNames<FlagEnum>())
	{
		std::cout << element << std::endl;
	}
	std::cout << en::Meta::GetEnumName(en::Meta::EnumCast<FlagEnum>(1)) << std::endl;
	std::cout << en::Meta::GetEnumCount<FlagEnum>() << std::endl;

	std::cout << std::endl;
	std::cout << "-- -- --" << std::endl;
}


int main()
{
#ifdef ENLIVE_ENABLE_LOG
	en::LogManager::GetInstance().Initialize();
#endif // ENLIVE_ENABLE_LOG

	LogInfo(en::LogChannel::System, 9, "ENLIVE_FUNCTION : %s", ENLIVE_FUNCTION);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_NAME : %s", ENLIVE_COMPILER_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_VERSION : %s", ENLIVE_STRINGIFY(ENLIVE_COMPILER_VERSION));
	LogInfo(en::LogChannel::System, 9, "ENLIVE_COMPILER_STRING : %s", ENLIVE_COMPILER_STRING);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_NAME : %s", ENLIVE_PLATFORM_NAME);
	LogInfo(en::LogChannel::System, 9, "ENLIVE_PLATFORM_DESCRIPTION : %s", ENLIVE_PLATFORM_DESCRIPTION);

	/*
	LogInfo(en::LogChannel::System, 9, "A: %s", en::TypeInfo<A>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<A>: %s", en::TypeInfo<B<A>>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<en::U32>: %s", en::TypeInfo<B<en::U32>>::GetName());
	LogInfo(en::LogChannel::System, 9, "B<C>: %s", en::TypeInfo<B<C>>::GetName());
	*/

	MetaEnumTest();

	return 0;
}
