#pragma once

/*

#include <Enlivengine/Config.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <Enlivengine/System/TypeTraits.hpp>

static constexpr en::U32 Attribute_None = 0;
static constexpr en::U32 Attribute_Transient = (1 << 0);
static constexpr en::U32 Attribute_CustomSerialization = (1 << 1);
static constexpr en::U32 Attribute_ToolOnly = (1 << 2);
static constexpr en::U32 Attribute_Grayed = (1 << 3);
static constexpr en::U32 Attribute_Hidden = (1 << 4);

template <typename Class, typename T>
class MetaDataMember
{
public:
	using MemberPtrT = T Class::*;

	constexpr MetaDataMember(const char* name, MemberPtrT ptr, en::U32 attributes = Attribute_None)
		: mName(name)
		, mPtr(ptr)
		, mAttributes(attributes)
	{
	}

	constexpr const char* GetName() const { return mName; }
	constexpr MemberPtrT GetPtr() const { return mPtr; }
	constexpr en::U32 GetAttributes() const { return mAttributes; }

	const T& Get(const Class& obj) const 
	{ 
		assert(mPtr != nullptr);
		return obj.*mPtr;
	}

	template <typename V, typename = std::enable_if_t<std::is_constructible_v<T, V>>>
	void Set(Class& obj, V&& value) const
	{
		assert(mPtr != nullptr);
		obj.*mPtr = value;
	}

private:
	const char* mName;
	MemberPtrT mPtr;
	en::U32 mAttributes;
};

template <typename T, typename TupleType>
class 
	*/


#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/System/Time.hpp>
#include <iostream>
#include <array>
#include <vector>

#include "DataFile.hpp"
#include "ObjectEditor.hpp"


enum class MyEnum
{
	A,
	B,
	C
};

struct Vector2Test
{
	int x;
	int y;
};
ENLIVE_META_CLASS_BEGIN(Vector2Test)
	ENLIVE_META_CLASS_MEMBER("x", &Vector2Test::x),
	ENLIVE_META_CLASS_MEMBER("y", &Vector2Test::y)
ENLIVE_META_CLASS_END()

class TestClassA
{
public:
	void SetA(en::U32 v) { a = v; }
	void SetB(en::F32 v) { b = v; }
	void SetC(en::I32 v) { c = v; }
	void SetD(bool v) { d = v; }
	void SetE(MyEnum v) { e = v; }
	void SetF(en::Time v) { f = v; }
	void AddG(en::U32 v) { g.Add(v); }
	void SetH(const Vector2Test& v) { h = v; }
	void SetI(const std::string& v) { i = v; }
	void SetJ(en::U32 index, const Vector2Test& v) { j[index] = v; }

	en::U32 GetA() const { return a; }
	en::F32 GetB() const { return b; }
	en::I32 GetC() const { return c; }
	bool GetD() const { return d; }
	MyEnum GetE() const { return e; }
	en::Time GetF() const { return f; }
	en::Array<en::U32>& GetG() { return g; }
	const Vector2Test& GetH() const { return h; }
	const std::string& GetI() const { return i; }
	const std::array<Vector2Test, 2> GetJ() const { return j; }

private:
	en::U32 a;
	en::F32 b;
	en::I32 c;
	bool d;
	MyEnum e;
	en::Time f;
	en::Array<en::U32> g;
	Vector2Test h;
	std::string i;
	std::array<Vector2Test, 2> j;

	ENLIVE_META_CLASS()
};

// This is the default behavior for ImGui edition
/*
template <>
struct CustomImGuiEditor<TestClassA>
{
	static constexpr bool value = true;
	static bool ImGuiEditor(TestClassA& object, const char* name)
	{
		return ObjectEditor::ImGuiEditor_Registered(object, name);
	}
};
*/

// This is the default behavior for Serialization
/*
template <>
struct CustomSerialization<TestClassA>
{
	static constexpr bool value = true;
	static bool Serialize(DataFile& dataFile, const TestClassA& object, const char* name) const
	{
		return dataFile.Serialize_Registered(object, name);
	}
	static bool Deserialize(DataFile& dataFile, TestClassA& object, const char* name)
	{
		return dataFile.Deserialize_Registered(object, name);
	}
};
*/

static_assert(!en::Traits::IsPolymorphic<TestClassA>::value); // This is used to ensure we don't had any virtual to TestClassA

ENLIVE_META_CLASS_BEGIN(TestClassA)
	ENLIVE_META_CLASS_MEMBER("a", &TestClassA::a),
	ENLIVE_META_CLASS_MEMBER("b", &TestClassA::b),
	ENLIVE_META_CLASS_MEMBER("c", &TestClassA::c),
	ENLIVE_META_CLASS_MEMBER("d", &TestClassA::d),
	ENLIVE_META_CLASS_MEMBER("e", &TestClassA::e),
	ENLIVE_META_CLASS_MEMBER("f", &TestClassA::f),
	ENLIVE_META_CLASS_MEMBER("g", &TestClassA::g),
	ENLIVE_META_CLASS_MEMBER("h", &TestClassA::h),
	ENLIVE_META_CLASS_MEMBER("i", &TestClassA::i),
	ENLIVE_META_CLASS_MEMBER("j", &TestClassA::j)
ENLIVE_META_CLASS_END()


struct Aaa
{
	int a;

	ENLIVE_META_CLASS()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_IMGUI_EDITOR()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_SERIALIZATION()
};
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_IMGUI_EDITOR(Aaa)
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_SERIALIZATION(Aaa)
ENLIVE_META_CLASS_BEGIN(Aaa)
	ENLIVE_META_CLASS_MEMBER("a", &Aaa::a)
ENLIVE_META_CLASS_END()

struct Bbb : public Aaa
{
	bool b;

	ENLIVE_META_CLASS()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_IMGUI_EDITOR()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_SERIALIZATION()
};
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_IMGUI_EDITOR(Bbb)
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_SERIALIZATION(Bbb)
ENLIVE_META_CLASS_BEGIN(Bbb)
	ENLIVE_META_CLASS_MEMBER("a", &Bbb::a),
	ENLIVE_META_CLASS_MEMBER("b", &Bbb::b)
ENLIVE_META_CLASS_END()

struct Ccc : public Aaa
{
	char c;

	ENLIVE_META_CLASS()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_IMGUI_EDITOR()
	ENLIVE_META_CLASS_DEFAULT_VIRTUAL_SERIALIZATION()
};
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_IMGUI_EDITOR(Ccc)
ENLIVE_META_CLASS_DEFAULT_TRAITS_VIRTUAL_SERIALIZATION(Ccc)
ENLIVE_META_CLASS_BEGIN(Ccc)
	ENLIVE_META_CLASS_MEMBER("a", &Ccc::a),
	ENLIVE_META_CLASS_MEMBER("c", &Ccc::c)
ENLIVE_META_CLASS_END()