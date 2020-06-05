#pragma once

#include <Enlivengine/Config.hpp>

#ifdef ENLIVE_ENABLE_METADATA

#include <magic_enum/magic_enum.hpp>
#include <MetaStuff/Meta.h>

#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/Hash.hpp>
#include <Enlivengine/System/TypeTraits.hpp>

namespace en
{

namespace Meta
{

// Enums
static_assert(magic_enum::is_magic_enum_supported);
template <typename E> constexpr auto GetEnumTypeName() { return magic_enum::enum_type_name<E>(); }
template <typename E> constexpr auto GetEnumName(E value) { return magic_enum::enum_name(value); }
template <typename E> constexpr auto GetEnumNames() { return magic_enum::enum_names<E>(); }
template <typename E, typename T> constexpr auto EnumCast(T value) { return magic_enum::enum_cast<E>(value).value(); }
template <typename E> constexpr en::U32 GetEnumCount() { return static_cast<en::U32>(magic_enum::enum_count<E>()); }

// Classes
#define ENLIVE_META_CLASS_FRIEND() template <typename T> friend auto meta::registerMembers(); // https://stackoverflow.com/questions/922545/how-to-allow-template-function-to-have-friend-like-access
#define ENLIVE_META_CLASS_BEGIN(className) ENLIVE_DEFINE_TYPE_TRAITS_NAME(className) \
	namespace meta { \
		template <> inline auto registerName<className>() { return #className; } \
		template <> inline auto registerMembers<className>() { return members(
#define ENLIVE_META_CLASS_MEMBER(name, ptr) member(name, ptr)
#define ENLIVE_META_CLASS_END() ); } }

template <typename T> constexpr bool IsRegistered() { return meta::isRegistered<T>(); }
template <typename T> using EnableIfRegistered = en::Traits::EnableIf<IsRegistered<T>()>;
template <typename T> using EnableIfNotRegistered = en::Traits::EnableIf<!IsRegistered<T>()>;

template <typename T> constexpr auto GetClassName() { return meta::getName<T>(); }
template <typename T> constexpr auto GetClassMembers() { return meta::getMembers<T>(); }
template <typename T> constexpr en::U32 GetClassMemberCount() { return static_cast<en::U32>(meta::getMemberCount<T>()); }

template <typename T, typename F, typename = EnableIfRegistered<T>::type> 
void ForAllMembers(F&& f)
{
	meta::doForAllMembers<T>(f);
}
template <typename T, typename F, typename = EnableIfNotRegistered<T>::type, typename = void>
void ForAllMembers(F&& f)
{
	meta::doForAllMembers<T>(f);
}

template <typename T, typename MemberType, typename F>
void ForMember(const char* name, F&& f)
{
	meta::doForMember<T, MemberType>(name, f); // TODO : This internally use strcmp, maybe we could use hash instead ?
}

template <typename MemberType, typename T>
MemberType GetMemberValue(T& obj, const char* name)
{
	return meta::getMemberValue<MemberType>(obj, name); // TODO : This internally use strcmp, maybe we could use hash instead ?
}

template <typename MemberType, typename T, typename V>
void SetMemberValue(T& obj, const char* name, V&& value)
{
	return meta::setMemberValue<MemberType>(obj, name, value);
}

template <typename T>
U32 GetClassMembersHash()
{
	if constexpr (IsRegistered<T>())
	{
		U32 hash = 89;
		en::Meta::ForAllMembers<T>(
			[&hash](const auto& member)
			{
				hash = Hash::Combine32(hash, Hash::Meow32(member.getName()));
			}
		);
		return hash;
	}
	else
	{
		return 0;
	}
}

} // namespace Meta

} // namespace en

#endif // ENLIVE_ENABLE_METADATA
