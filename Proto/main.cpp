#include <Enlivengine/System/Log.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Application/Window.hpp>
#include <Enlivengine/Graphics/View.hpp>

#include "GameState.hpp"
#include "Meta.hpp"

#include <Enlivengine/System/String.hpp>

int main(int argc, char** argv)
{
	{
		// TODO : Do something about type info for pointers
		printf("%s\n", en::TypeInfo<en::Array<en::U32>>::GetName());
		printf("%s\n", en::TypeInfo<en::Array<en::U32*>>::GetName());

		printf("%s\n", en::Meta::ClassFactory::IsRegistered<TestClassA>() ? "true" : "false");

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
	}

#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	if (en::MemoryAllocator::GetDebugAllocator().GetBlockCount() > 0)
	{
		const auto& blocks = en::MemoryAllocator::GetDebugAllocator().GetBlocks();
		for (const auto& block : blocks)
		{
			printf("%s (%p): %d from %s:%d\n", block.context, block.ptr, block.size, block.GetFile().c_str(), block.line);
		}
	}
#endif // ENLIVE_ENABLE_DEBUG_MEMORY

	return 0;
}