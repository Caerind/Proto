#include <Enlivengine/Application/Application.hpp>

#include <Enlivengine/System/String.hpp>
#include <Enlivengine/System/ParserXml.hpp>

#ifdef ENLIVE_DEBUG
#include <Enlivengine/Graphics/DebugDraw.hpp>
#endif // ENLIVE_DEBUG

#ifdef ENLIVE_ENABLE_IMGUI
#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/Tools/ImGuiConsole.hpp>
#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>
#include <Enlivengine/Tools/ImGuiLogger.hpp>
#include <Enlivengine/Tools/ImGuiProfiler.hpp>
#include <Enlivengine/Tools/ImGuiDemoWindow.hpp>
#include <Enlivengine/Tools/ImGuiResourceBrowser.hpp>
#include <Enlivengine/Tools/ImGuiAnimationEditor.hpp>
#include <Enlivengine/Tools/ImGuiInputEditor.hpp>
#include <Enlivengine/Tools/ImGuiMemoryDebugger.hpp>
#endif // ENLIVE_ENABLE_IMGUI

// Resources
#include <Enlivengine/Map/Tileset.hpp>
#include <Enlivengine/Map/Map.hpp>
#include <Enlivengine/Graphics/Animation.hpp>
#include <Enlivengine/Graphics/AnimationStateMachine.hpp>

namespace en
{

Application::Application()
	: mStates(*this)
	, mWindow(sf::VideoMode(1024, 768), "")
	, mFps(0)
	, mRunning(true)
	, mInitialized(false)
	, mTotalFrames(0)
	, mTotalDuration()
{
}

Application::~Application()
{
	Stop();
}

bool Application::Initialize()
{
	mInitialized = true;

	mWindowClosedSlot.Connect(mWindow.onWindowClosed, [this](const en::Window*) { Stop(); });

	RegisterTools();

	LoadResources();

	return true;
}

Window& Application::GetWindow()
{
	return mWindow;
}

void Application::Stop()
{
	if (mRunning)
	{
		ENLIVE_PROFILE_FUNCTION();

		onApplicationStopped(this);

		AudioSystem::GetInstance().Stop();
		AudioSystem::GetInstance().Clear();

#ifdef ENLIVE_ENABLE_IMGUI
		//ImGuiResourceBrowser::GetInstance().SaveResourceInfosToFile(PathManager::GetInstance().GetAssetsPath() + "resources.xml");
#endif // ENLIVE_ENABLE_IMGUI

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::GetInstance().Shutdown();
#endif // ENLIVE_ENABLE_IMGUI

		if (mWindow.isOpen())
		{
			mWindow.close();
		}

		mRunning = false;

		//std::exit(EXIT_SUCCESS);
	}
}

bool Application::IsRunning() const
{
	return mRunning;
}

void Application::PopState()
{
	mStates.popState();
}

void Application::ClearStates()
{
	mStates.clearStates();
}

U32 Application::GetFPS() const
{
	return mFps;
}

U32 Application::GetTotalFrames() const
{
	return mTotalFrames;
}

Time Application::GetTotalDuration() const
{
	return mTotalDuration.GetElapsedTime();
}

bool Application::LoadResources()
{
	enAssert(mInitialized);

#ifdef ENLIVE_ENABLE_IMGUI
	return ImGuiResourceBrowser::GetInstance().LoadResourceInfosFromFile(PathManager::GetInstance().GetAssetsPath() + "resources.xml");
#else
	ParserXml xml;
	if (!xml.LoadFromFile(PathManager::GetInstance().GetAssetsPath() + "resources.xml"))
	{
		enLogError(en::LogChannel::Application, "Can't open resources file at {}", filename.c_str());
		return false;
	}

	if (xml.ReadNode("Resources"))
	{
		U32 resourceCount = 0;
		xml.GetAttribute("resourceCount", resourceCount);
		if (resourceCount > 0)
		{
			if (xml.ReadNode("Resource"))
			{
				do
				{
					std::string identifierStr;
					xml.GetAttribute("identifier", identifierStr);
					std::string filenameStr;
					xml.GetAttribute("filename", filenameStr);
					I32 typeInt;
					xml.GetAttribute("type", typeInt);
					ResourceID resourceID;
					LoadResource(typeInt, identifierStr, filenameStr, resourceID);
				} while (xml.NextSibling("Resource"));
				xml.CloseNode();
			}
		}
		xml.CloseNode();
	}
	else
	{
		enLogError(en::LogChannel::Application, "Invalid resources file at {}", filename.c_str());
		return false;
	}
	return true;
#endif // ENLIVE_ENABLE_IMGUI
}

bool Application::LoadResource(I32 type, const std::string& identifier, const std::string& filename, ResourceID& resourceID)
{
#ifdef ENLIVE_ENABLE_IMGUI
	static_assert(-1 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Unknown));
	static_assert(0 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Font));
	static_assert(1 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Texture));
	static_assert(2 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Tileset));
	static_assert(3 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Map));
	static_assert(4 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Animation));
	static_assert(5 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::AnimationStateMachine));
	static_assert(6 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Music));
	static_assert(7 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Sound));
	static_assert(8 == static_cast<I32>(ImGuiResourceBrowser::ResourceInfo::Type::Count));
#endif // ENLIVE_ENABLE_IMGUI

	ResourceManager& resourceManager = ResourceManager::GetInstance();
	std::string resourceFilename = en::PathManager::GetInstance().GetAssetsPath() + filename;
	switch (type)
	{
	case -1: // ResourceInfo::Type::Unknown
	{
		resourceID = InvalidResourceID;
		enLogError(en::LogChannel::Global, "Unknown resource type for resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
		return false;
	}
	case 0: // ResourceInfo::Type::Font
	{
		FontPtr fontPtr = resourceManager.Create<Font>(identifier.c_str(), FontLoader::FromFile(resourceFilename));
		if (!fontPtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = fontPtr.GetID();
		}
		break;
	}
	case 1: // ResourceInfo::Type::Texture
	{
		TexturePtr texturePtr = resourceManager.Create<Texture>(identifier.c_str(), TextureLoader::FromFile(resourceFilename));
		if (!texturePtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = texturePtr.GetID();
		}
		break;
	}
	case 2: // ResourceInfo::Type::Tileset
	{
		tmx::TilesetPtr tilesetPtr = resourceManager.Create<tmx::Tileset>(identifier.c_str(), tmx::TilesetLoader::FromFile(resourceFilename));
		if (!tilesetPtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = tilesetPtr.GetID();
		}
		break;
	}
	case 3: // ResourceInfo::Type::Map
	{
		tmx::MapPtr mapPtr = resourceManager.Create<tmx::Map>(identifier.c_str(), tmx::MapLoader::FromFile(resourceFilename));
		if (!mapPtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = mapPtr.GetID();
		}
		break;
	}
	case 4: // ResourceInfo::Type::Animation
	{
		AnimationPtr animPtr = resourceManager.Create<Animation>(identifier.c_str(), AnimationLoader::FromFile(resourceFilename));
		if (!animPtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = animPtr.GetID();
		}
		break;
	}
	case 5: // ResourceInfo::Type::AnimationStateMachine
	{
		AnimationStateMachinePtr animPtr = resourceManager.Create<AnimationStateMachine>(identifier.c_str(), AnimationStateMachineLoader::FromFile(resourceFilename));
		if (!animPtr.IsValid())
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = animPtr.GetID();
		}
		break;
	}
	case 6: // ResourceInfo::Type::Music
	{
		MusicID musicID = AudioSystem::GetInstance().PrepareMusic(identifier.c_str(), resourceFilename);
		if (musicID == InvalidMusicID)
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = musicID;
		}
		break;
	}
	case 7: // ResourceInfo::Type::Sound
	{
		SoundID soundID = AudioSystem::GetInstance().PrepareSound(identifier.c_str(), resourceFilename);
		if (soundID == InvalidSoundID)
		{
			resourceID = InvalidResourceID;
			enLogWarning(en::LogChannel::Global, "Can't load resource : {}, {}", identifier.c_str(), resourceFilename.c_str());
			return false;
		}
		else
		{
			resourceID = soundID;
		}
		break;
	}
	default: enAssert(false); return false;
	}
	return true;
}

void Application::Run()
{
	constexpr Time TimeUpdateFPS = Time::Seconds(0.5f);
	constexpr Time TimePerFrame = Time::Seconds(1.0f / 60.0f);
	Time accumulator;
	Time fpsAccumulator;
	U32 framesFps = 0;
	Clock clock;

	// Running loop
	while (mRunning)
	{
#ifdef ENLIVE_ENABLE_PROFILE
		Profiler::GetInstance().StartFrame(mTotalFrames);
#endif // ENLIVE_ENABLE_PROFILE

		{
			ENLIVE_PROFILE_SCOPE(MainFrame);

			// Time
			Time dt = clock.Restart();

			// Usefull when using Breakpoints on Debugging
#ifdef ENLIVE_DEBUG
			if (dt > Time::Second())
			{
				dt = TimePerFrame;
			}
#endif // ENLIVE_DEBUG

			accumulator += dt;
			fpsAccumulator += dt;

			Events();
			if (!mRunning)
			{
#ifdef ENLIVE_ENABLE_PROFILE
				Profiler::GetInstance().EndFrame();
#endif // ENLIVE_ENABLE_PROFILE
				return;
			}

			// Fixed time 60 FPS
			while (accumulator >= TimePerFrame)
			{
				accumulator -= TimePerFrame;

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiToolManager::GetInstance().Update(mWindow, TimePerFrame);
#endif // ENLIVE_ENABLE_IMGUI

				PreUpdate();
				Update(TimePerFrame);
				PostUpdate();

#ifdef ENLIVE_ENABLE_IMGUI
				ImGuiToolManager::GetInstance().Render();
#endif // ENLIVE_ENABLE_IMGUI
			}

			Render();

			// FPS
			framesFps++;
			if (fpsAccumulator > TimeUpdateFPS)
			{
				mFps = static_cast<U32>(framesFps / TimeUpdateFPS.AsSeconds());
				fpsAccumulator = Time::Zero();
				framesFps = 0;

#ifdef ENLIVE_DEBUG
				mWindow.setTitle("FPS : " + ToString(mFps));
#endif // ENLIVE_DEBUG
			}

			mTotalFrames++;

			// Stop ?
			if (!mWindow.isOpen() || mStates.getStateCount() == 0)
			{
				Stop();
			}
		}

#ifdef ENLIVE_ENABLE_PROFILE
		Profiler::GetInstance().EndFrame();
#endif // ENLIVE_ENABLE_PROFILE
	}
}

void Application::Events()
{
	ENLIVE_PROFILE_FUNCTION();

	sf::Event event;
	while (mWindow.pollEvent(event) && mRunning)
	{
		mActionSystem.AddEvent(event);

		// Might have used Signal of the Window, but clearer like this
		if (event.type == sf::Event::GainedFocus)
		{
			AudioSystem::GetInstance().Play();
		}
		if (event.type == sf::Event::LostFocus)
		{
			AudioSystem::GetInstance().Pause();
		}

#ifdef ENLIVE_ENABLE_IMGUI
		ImGuiToolManager::GetInstance().HandleEvent(event);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P && event.key.control == true)
		{
#ifdef ENLIVE_ENABLE_PROFILE
			if (ImGuiProfiler::GetInstance().CanCurrentFrameBeCaptured())
			{
				ImGuiProfiler::GetInstance().CaptureCurrentFrameAndOpenProfiler();
			}
			else
			{
				ImGuiProfiler::GetInstance().SetEnabled(true);
			}
#endif // ENLIVE_ENABLE_PROFILE
		}
#endif	

		mStates.handleEvent(event);
	}
}

void Application::PreUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
	AudioSystem::GetInstance().Update();
	mActionSystem.Update();
}

void Application::Update(Time dt)
{
	ENLIVE_PROFILE_FUNCTION();
	mStates.update(dt);
}

void Application::PostUpdate()
{
	ENLIVE_PROFILE_FUNCTION();
}

void Application::Render()
{
	ENLIVE_PROFILE_FUNCTION();

	mWindow.clear();

	mStates.render(mWindow.getHandle());

#ifdef ENLIVE_DEBUG
	DebugDraw::render(mWindow.getHandle());
#endif // ENLIVE_DEBUG

#ifdef ENLIVE_ENABLE_IMGUI
	ImGuiToolManager::GetInstance().RenderFrame(mWindow);
#endif // ENLIVE_ENABLE_IMGUI

	mWindow.display();
}

void Application::RegisterTools()
{
#ifdef ENLIVE_ENABLE_IMGUI
	ImGuiToolManager::GetInstance().Initialize(mWindow);

	// Main
#ifdef ENLIVE_ENABLE_LOG
	ImGuiLogger::GetInstance().Register();
#endif // ENLIVE_ENABLE_LOG
	ImGuiInputEditor::GetInstance().Register();
	ImGuiResourceBrowser::GetInstance().Register();
    ImGuiAnimationEditor::GetInstance().Register();
	ImGuiConsole::GetInstance().Register();
	ImGuiDemoWindow::GetInstance().Register();
	// Engine
	ImGuiEntityBrowser::GetInstance().Register();
#ifdef ENLIVE_ENABLE_DEBUG_MEMORY
	ImGuiMemoryDebugger::GetInstance().Register();
#endif // ENLIVE_ENABLE_DEBUG_MEMORY
#ifdef ENLIVE_ENABLE_PROFILE
	ImGuiProfiler::GetInstance().Register();
#endif // ENLIVE_ENABLE_PROFILE

	// Game
	// ...
#endif // ENLIVE_ENABLE_IMGUI
}

ScreenshotSystem& Application::GetScreenshotSystem()
{
	return mScreenshotSystem;
}

ActionSystem& Application::GetActionSystem()
{
	return mActionSystem;
}

} // namespace en
