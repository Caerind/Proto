#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Graphics/View.hpp>

#include "GameState.hpp"

#include <Enlivengine/System/Meta.hpp>
#include <iostream>

enum class MyEnum
{
	A,
	B,
	C
};

class TestClassA
{
public:
	void SetA(en::U32 v) { a = v; }
	void SetB(en::F32 v) { b = v; }
	void SetC(en::I32 v) { c = v; }
	void SetD(bool v) { d = v; }
	void SetE(MyEnum v) { e = v; }

	en::U32 GetA() const { return a; }
	en::F32 GetB() const { return b; }
	en::I32 GetC() const { return c; }
	bool GetD() const { return d; }
	MyEnum GetE() const { return e; }

private:
	en::U32 a;
	en::F32 b;
	en::I32 c;
	bool d;
	MyEnum e;
};

ENLIVE_META_CLASS_BEGIN(TestClassA)
	ENLIVE_META_CLASS_MEMBER("a", &TestClassA::GetA, &TestClassA::SetA),
	ENLIVE_META_CLASS_MEMBER("b", &TestClassA::GetB, &TestClassA::SetB),
	ENLIVE_META_CLASS_MEMBER("c", &TestClassA::GetC, &TestClassA::SetC),
	ENLIVE_META_CLASS_MEMBER("d", &TestClassA::GetD, &TestClassA::SetD),
	ENLIVE_META_CLASS_MEMBER("e", &TestClassA::GetE, &TestClassA::SetE)
ENLIVE_META_CLASS_END()

template <typename T, typename = en::Meta::EnableIfRegistered<T>::type>
void Serialize(const T& obj)
{
	en::Meta::ForAllMembers<T>(
		[&obj](const auto& member)
		{
			if (member.canGetConstRef())
			{
				Serialize(member.get(obj));
			}
			else if (member.hasGetter())
			{
				auto copy = member.getCopy(obj);
				Serialize(copy);
			}
		}
	);
}

template <typename T, typename = en::Meta::EnableIfNotRegistered<T>::type, typename = void>
void Serialize(const T& obj)
{
	SerializeBasic(obj);
}

template <typename T> 
void SerializeBasic(const T& obj) 
{ 
	std::cout << obj << std::endl; 
}

template <>
void SerializeBasic<MyEnum>(const MyEnum& obj)
{
	std::cout << en::Meta::GetEnumName(obj) << std::endl;
}

template <>
void SerializeBasic<bool>(const bool& obj)
{
	std::cout << (obj ? "true" : "false") << std::endl;
}

void MetaTest()
{
	// MetaClass
	std::cout << en::Meta::GetClassName<TestClassA>() << ":\n";
	en::Meta::ForAllMembers<TestClassA>(
		[] (const auto& member)
		{
			std::cout << " - " << member.getName() << '\n';
		}
	);
	std::cout << std::endl;

	// TestClassA1
	TestClassA testClassA1;
	testClassA1.SetA(123);
	testClassA1.SetB(-456);
	testClassA1.SetC(7.89f);
	testClassA1.SetD(true);
	testClassA1.SetE(MyEnum::B);
	Serialize(testClassA1);
	std::cout << std::endl;

	// TestClassA2
	TestClassA testClassA2;
	testClassA2.SetA(789);
	testClassA2.SetB(-654);
	testClassA2.SetC(1.23f);
	testClassA2.SetD(false);
	testClassA2.SetE(MyEnum::C);
	Serialize(testClassA2);
	std::cout << std::endl;
}

int main(int argc, char** argv)
{

	MetaTest();

	if (argc >= 1)
	{
		en::PathManager::GetInstance().SetExecutablePath(argv[0]);
	}

	en::Application::GetInstance().Initialize();

	en::Application& app = en::Application::GetInstance();
	app.GetWindow().create(sf::VideoMode(1024, 768), "Proto", sf::Style::Titlebar | sf::Style::Close);
	app.GetWindow().getMainView().setCenter({ 512.0f, 384.0f });
	app.GetWindow().getMainView().setSize({ 1024.0f, 768.0f });
	en::PathManager::GetInstance().SetScreenshotPath("Screenshots/");

	app.Start<GameState>();

	return 0;
}