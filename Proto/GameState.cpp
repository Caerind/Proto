#include "GameState.hpp"

#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Math/Color.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/System/ClassManager.hpp>
#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>
#include <Enlivengine/Core/Universe.hpp>

#include <imgui/imgui.h>

GameState::GameState(en::StateManager& manager)
	: en::State(manager)
	, mAs("mAs")
	, mAaa("mAaa")
{
	Vector2Test v1;
	v1.x = -10;
	v1.y = +10;
	Vector2Test v2;
	v2.x = +10;
	v2.y = -10;

	TestClassA* mA1 = enNew(TestClassA, "mA1");
	mA1->SetA(123);
	mA1->SetB(-456);
	mA1->SetC(7.89f);
	mA1->SetD(true);
	mA1->SetE(MyEnum::B);
	mA1->SetF(en::Time::Seconds(23.0f));
	mA1->GetG().SetDebugMemoryContext("mA1::g");
	mA1->AddG(7);
	mA1->AddG(20);
	mA1->SetH(v1);
	mA1->SetI("TestI1");
	mA1->SetJ(0, v1);
	mA1->SetJ(1, v2);
	TestClassA* mA2 = enNew(TestClassA, "mA2");
	mA2->SetA(789);
	mA2->SetB(-654);
	mA2->SetC(1.23f);
	mA2->SetD(false);
	mA2->SetE(MyEnum::C);
	mA2->SetF(en::Time::Milliseconds(10));
	mA2->GetG().SetDebugMemoryContext("mA2::g");
	mA2->AddG(1);
	mA2->AddG(8);
	mA2->AddG(123);
	mA2->SetH(v2);
	mA2->SetI("TestI2");
	mA2->SetJ(0, v1);
	mA2->SetJ(1, v2);
	mAs.Add(mA1);
	mAs.Add(mA2);
	mAs.Add(nullptr);

	Aaa* mA = enNew(Aaa, "mA");
	mA->a = 1;
	Bbb* mB = enNew(Bbb, "mB");
	mB->a = 2;
	mB->b = true;
	Ccc* mC = enNew(Ccc, "mC");
	mC->a = 3;
	mC->c = 'c';
	mAaa.Add(mA);
	mAaa.Add(mB);
	mAaa.Add(mC);

	mTestFactory = (Aaa*)en::ClassManager::CreateClassFromHash(en::TypeInfo<Bbb>::GetHash());

	en::DataFile file;
	file.LoadFromFile("DataFileWorld.xml");
	file.Deserialize(mWorld, "World");
#ifdef ENLIVE_DEBUG
	mWorld.GetFreeCamView().setCenter(getApplication().GetWindow().getMainView().getSize() * 0.5f);
	mWorld.GetFreeCamView().setSize(getApplication().GetWindow().getMainView().getSize());
#endif // ENLIVE_DEBUG
	en::Universe::GetInstance().SetCurrentWorld(&mWorld);

	mElevationNoise.SetNoiseType(en::Noise::NoiseType::SimplexFractal);
	mElevationImage.create(1024, 768);
	NoiseToImage(mElevationNoise, mElevationImage);

	mHumidityNoise.SetNoiseType(en::Noise::NoiseType::PerlinFractal);
	mHumidityImage.create(1024, 768);
	NoiseToImage(mHumidityNoise, mHumidityImage);

	mTemperatureNoise.SetNoiseType(en::Noise::NoiseType::CubicFractal);
	mTemperatureNoise.SetFractalOctaves(1);
	mTemperatureNoise.SetFractalGain(5.0f);
	mTemperatureImage.create(1024, 768);
	//NoiseToImage(mTemperatureNoise, mTemperatureImage);
	mTemperatureGradient[0.0f] = en::Color(0, 0, 255);
	mTemperatureGradient[0.25f] = en::Color(0, 255, 255);
	mTemperatureGradient[0.5f] = en::Color(0, 255, 0);
	mTemperatureGradient[0.75f] = en::Color(255, 255, 0);
	mTemperatureGradient[1.0f] = en::Color(255, 0, 0);

	mWaterGradient[0.0f] = en::Color(63, 72, 204);
	mWaterGradient[0.5f] = en::Color(0, 162, 232);
	mWaterGradient[1.0f] = en::Color(153, 217, 234);
	mMapImage.create(1024, 768);
	UpdateMap();

	mDisplay = 0;
	mTexture.loadFromImage(mMapImage);
}

GameState::~GameState()
{
	mAs.DeleteAll();
	mAaa.DeleteAll();
	enDelete(Aaa, mTestFactory);
}

bool GameState::handleEvent(const sf::Event& event)
{
	ENLIVE_PROFILE_FUNCTION();

	if (event.type == sf::Event::KeyPressed)
	{
		if (event.key.control == true && event.key.code == sf::Keyboard::S)
		{
			en::DataFile file;
			file.CreateEmptyFile();
			file.Serialize(mWorld, "World");
			file.SaveToFile("DataFileWorld.xml");
		}
	}

	return false;
}

bool GameState::update(en::Time dt)
{
	ENLIVE_PROFILE_FUNCTION();

	if (!en::Application::GetInstance().IsRunning())
	{
		return false;
	}

#ifdef ENLIVE_DEBUG
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
	{
		constexpr en::F32 speed = 400.0f;
		en::F32 speedMulti = (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl)) ? 2.0f : 1.0f;
		en::F32 speedDiv = (sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt)) ? 0.5f : 1.0f;
		en::Vector2f velocity(en::Vector2f::zero);
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			velocity.y -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		{
			velocity.y += 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			velocity.x -= 1.0f;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			velocity.x += 1.0f;
		}
		if (velocity.getSquaredLength() > 0.01f)
		{
			velocity.normalize();
		}
		velocity *= speed * speedMulti * speedDiv * dt.AsSeconds();
		mWorld.GetFreeCamView().move(velocity);
	}
#endif // ENLIVE_DEBUG

	/*
	int i = 0;
	mManager.Each([&i, this](auto ent)
	{
		en::Entity entity(mManager, ent);
		enLogInfo(en::LogChannel::System, "TestEntity {} : {}", i, entity.GetName()); ++i; }
	);

	int j = 0;
	auto view = mManager.View<PositionComponent>();
	for (auto ent : view)
	{
		en::Entity entity(mManager, ent);
		enLogInfo(en::LogChannel::System, "TestPositionEntity {}", j); ++j;
	}
	*/

	/*
	if (ImGui::Begin("ObjectEditor"))
	{
		if (ImGui::Button("Save As"))
		{
			en::DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Serialize(mAs, "As");
			file.SaveToFile("DataFileGame.xml");
		}
		ImGui::SameLine(); 
		if (ImGui::Button("Load As"))
		{
			mAs.DeleteAll();
			mAs.Clear();

			en::DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Deserialize(mAs, "As");
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear As"))
		{
			mAs.DeleteAll();
			mAs.Clear();
		}
		en::ObjectEditor::ImGuiEditor(mAs, "As");

		if (ImGui::Button("Save A"))
		{
			en::DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Serialize(mAaa, "A");
			file.SaveToFile("DataFileGame.xml");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load A"))
		{
			mAaa.DeleteAll();
			mAaa.Clear();

			en::DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Deserialize(mAaa, "A");
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear A"))
		{
			mAaa.DeleteAll();
			mAaa.Clear();
		}
		en::ObjectEditor::ImGuiEditor(mAaa, "A");
		
		en::ObjectEditor::ImGuiEditor(mWorld.GetEntityManager(), "EntityManager");

		if (ImGui::Button("SerializeEntities")) // TEMP
		{
			en::DataFile file;
			file.CreateEmptyFile();
			file.Serialize(mWorld, "World");
			file.SaveToFile("DataFileWorld.xml");
		}


		if (mTestFactory != nullptr)
		{
			en::ObjectEditor::ImGuiEditor(mTestFactory, "TestFactory");
			if (ImGui::Button("Remove"))
			{
				enDelete(Aaa, mTestFactory);
			}
		}
		else
		{
			// TODO : Make this more automatic 
			if (ImGui::Button("Create Aaa"))
			{
				mTestFactory = (Aaa*)en::ClassManager::CreateClassFromHash(en::TypeInfo<Aaa>::GetHash());
			}
			if (ImGui::Button("Create Bbb"))
			{
				mTestFactory = (Aaa*)en::ClassManager::CreateClassFromHash(en::TypeInfo<Bbb>::GetHash());
			}
			if (ImGui::Button("Create Ccc"))
			{
				mTestFactory = (Aaa*)en::ClassManager::CreateClassFromHash(en::TypeInfo<Ccc>::GetHash());
			}
		}
		ImGui::End();
	}
	*/

	/*
	ImGui::Begin("Noise");
	static const char* strings[] = { "Map", "Elevation", "Humidity", "Temperature" };
	if (ImGui::Combo("", &mDisplay, strings, IM_ARRAYSIZE(strings)))
	{
		switch (mDisplay)
		{
		case 0: UpdateMap(); mTexture.loadFromImage(mMapImage); break;
		case 1: NoiseToImage(mElevationNoise, mElevationImage); mTexture.loadFromImage(mElevationImage); break;
		case 2: NoiseToImage(mHumidityNoise, mHumidityImage); mTexture.loadFromImage(mHumidityImage); break;
		case 3: UpdateTemperature(); mTexture.loadFromImage(mTemperatureImage); break;
		}	
	}
	ImGui::End();

	bool modified = false;
	if (ImGuiNoiseWindow("Elevation", mElevationNoise))
	{
		if (mDisplay == 1)
		{
			NoiseToImage(mElevationNoise, mElevationImage);
			mTexture.loadFromImage(mElevationImage);
		}
		modified = true;
	}
	if (ImGuiNoiseWindow("Humidity", mHumidityNoise))
	{
		if (mDisplay == 2)
		{
			NoiseToImage(mHumidityNoise, mHumidityImage);
			mTexture.loadFromImage(mHumidityImage);
		}
		modified = true;
	}
	if (ImGuiNoiseWindow("Temperature", mTemperatureNoise))
	{
		if (mDisplay == 3)
		{
			UpdateTemperature();
			mTexture.loadFromImage(mTemperatureImage);
		}
		modified = true;
	}
	if (modified && mDisplay == 0)
	{
		UpdateMap();
		mTexture.loadFromImage(mMapImage);
	}
	*/

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();

#ifdef ENLIVE_DEBUG
	bool editor = true;
	if (editor)
	{
		getApplication().GetWindow().setView(mWorld.GetFreeCamView());
	}
	else
	{
		getApplication().GetWindow().setView(mWorld.GetGameView());
	}
#else
	getApplication().GetWindow().setView(mWorld.GetGameView());
#endif // ENLIVE_DEBUG

	sf::Sprite sprite(mTexture);
	target.draw(sprite);

#ifdef ENLIVE_DEBUG
	static bool dotInit = false;
	static sf::CircleShape dotTransform;
	if (!dotInit)
	{
		static constexpr float radius = 2.0f;
		dotTransform.setFillColor(sf::Color::Red);
		dotTransform.setRadius(radius);
		dotTransform.setOrigin(sf::Vector2f(radius, radius));
		dotInit = true;
	}
#endif // ENLIVE_DEBUG

	{
		ENLIVE_PROFILE_SCOPE(RenderSort);
		mWorld.GetEntityManager().GetRegistry().sort<en::TransformComponent>([](const auto& lhs, const auto& rhs)
		{
			const auto& pL = lhs.transform.GetPosition();
			const auto& pR = rhs.transform.GetPosition();
			if (pL.z == pR.z)
			{
				return pL.y < pR.y;
			}
			return pL.z < pR.z;
		});
	}

	{
		ENLIVE_PROFILE_SCOPE(RenderSystem);
		auto transformView = mWorld.GetEntityManager().View<en::TransformComponent, en::RenderableComponent>();
		for (auto entt : transformView)
		{
			en::Entity entity(mWorld, entt);
			if (entity.IsValid())
			{
				sf::RenderStates states;
				states.transform = en::toSF(entity.Get<en::TransformComponent>().transform.GetMatrix());

				if (entity.Has<en::SpriteComponent>())
				{
					entity.Get<en::SpriteComponent>().sprite.Render(target, states);
				}
				if (entity.Has<en::TextComponent>())
				{
					entity.Get<en::TextComponent>().text.Render(target, states);
				}

#ifdef ENLIVE_DEBUG
				if (en::ImGuiEntityBrowser::GetInstance().IsSelected(entity))
				{
					sf::RenderStates states;
					states.transform = en::toSF(entity.Get<en::TransformComponent>().transform.GetMatrix());
					target.draw(dotTransform, states);
				}
#endif // ENLIVE_DEBUG
			}
		}
	}

	getApplication().GetWindow().applyMainView();
}

void GameState::UpdateTemperature()
{
	const en::F32 width = static_cast<en::F32>(mTemperatureImage.getSize().x);
	const en::F32 height = static_cast<en::F32>(mTemperatureImage.getSize().y);
	for (en::F32 x = 0; x < width; x += 1.0f)
	{
		for (en::F32 y = 0; y < height; y += 1.0f)
		{
			const en::F32 temperature = (mTemperatureNoise.Get(x, y) * 0.5f + 0.5f);
			mTemperatureImage.setPixel((unsigned int)x, (unsigned int)y, en::toSF(mTemperatureGradient.GetSampleColor(temperature)));
		}
	}
}

void GameState::UpdateMap()
{
	for (en::U32 x = 0; x < 1024; ++x)
	{
		for (en::U32 y = 0; y < 768; ++y)
		{
			const en::F32 xf = static_cast<en::F32>(x);
			const en::F32 yf = static_cast<en::F32>(y);
			en::F32 elevation = mElevationNoise.Get(xf, yf) * 0.5f + 0.5f; // [0, 1]
			en::F32 humidity = mHumidityNoise.Get(xf, yf) * 0.5f + 0.5f; // [0, 1]
			en::F32 temperature = mTemperatureNoise.Get(xf, yf) * 0.5f + 0.5f; // [0, 1]

			en::Color color;
			if (elevation < 0.35f)
			{
				const en::F32 waterDeepFactor = elevation / 0.35f;
				color = mWaterGradient.GetSampleColor(waterDeepFactor);
			}
			else if (elevation < 0.45f)
			{
				const en::U8 hum = humidity * humidity * humidity * 200;
				color = en::Color(232, 211, 160);
				color.r -= hum;
				color.g -= hum;
				color.b -= hum;
			}
			else if (elevation > 0.85f)
			{
				color = en::Color::White;
			}
			else if (elevation > 0.80f)
			{
				color = en::Color(97, 97, 97);
			}
			else if (elevation > 0.70f)
			{
				color = en::Color(87, 44, 0);
			}
			else
			{
				const en::F32 invTemp = 1.0f - temperature;
				const en::U8 temp = invTemp * invTemp * invTemp * 200;
				color = en::Color(46, 122, 79); // Moderate
				color.r += temp;
				color.g += temp;
				color.b += temp;
			}

			mMapImage.setPixel(x, y, en::toSF(color));
		}
	}
}

bool GameState::ImGuiNoiseWindow(const char* title, en::Noise& noise)
{
	bool modified = false;

	ImGui::Begin(title);

	static const char* noiseTypeStrings[] = { "Value", "ValueFractal", "Perlin", "PerlinFractal", "Simplex", "SimplexFractal", "WhiteNoise", "Cubic", "CubicFractal" };
	int noiseTypeIndex = (int)noise.GetNoiseType();
	if (ImGui::Combo("Noise Type", &noiseTypeIndex, noiseTypeStrings, IM_ARRAYSIZE(noiseTypeStrings)))
	{
		noise.SetNoiseType(static_cast<en::Noise::NoiseType>(noiseTypeIndex));
		modified = true;
	}

	int seed = (int)noise.GetSeed();
	if (ImGui::InputInt("Seed", &seed))
	{
		noise.SetSeed(static_cast<en::I32>(seed));
		modified = true;
	}

	float frequency = (float)noise.GetFrequency();
	if (ImGui::SliderFloat("Frequency", &frequency, 0.01f, 10.0f))
	{
		noise.SetFrequency(frequency);
		modified = true;
	}

	if (noise.GetNoiseType() == en::Noise::NoiseType::Value
		|| noise.GetNoiseType() == en::Noise::NoiseType::ValueFractal
		|| noise.GetNoiseType() == en::Noise::NoiseType::Perlin
		|| noise.GetNoiseType() == en::Noise::NoiseType::PerlinFractal)
	{
		static const char* interpolationStrings[] = { "Linear", "Hermite", "Quintic" };
		int interpolationIndex = (int)noise.GetInterpolation();
		if (ImGui::Combo("Interpolation", &interpolationIndex, interpolationStrings, IM_ARRAYSIZE(interpolationStrings)))
		{
			noise.SetInterpolation(static_cast<en::Noise::Interpolation>(interpolationIndex));
			modified = true;
		}
	}

	if (noise.GetNoiseType() == en::Noise::NoiseType::ValueFractal
		|| noise.GetNoiseType() == en::Noise::NoiseType::PerlinFractal
		|| noise.GetNoiseType() == en::Noise::NoiseType::SimplexFractal
		|| noise.GetNoiseType() == en::Noise::NoiseType::CubicFractal)
	{
		static const char* fractalTypeStrings[] = { "FBM", "Billow", "RigidMulti" };
		int fractalTypeIndex = (int)noise.GetFractalType();
		if (ImGui::Combo("Fractal Type", &fractalTypeIndex, fractalTypeStrings, IM_ARRAYSIZE(fractalTypeStrings)))
		{
			noise.SetFractalType(static_cast<en::Noise::FractalType>(fractalTypeIndex));
			modified = true;
		}

		int octaves = (int)noise.GetFractalOctaves();
		if (ImGui::InputInt("Octaves", &octaves))
		{
			noise.SetFractalOctaves(static_cast<en::I32>(octaves));
			modified = true;
		}

		float lacunarity = (float)noise.GetFractalLacunarity();
		if (ImGui::SliderFloat("Lacunarity", &lacunarity, 0.0f, 10.0f))
		{
			noise.SetFractalLacunarity(lacunarity);
			modified = true;
		}

		float gain = (float)noise.GetFractalGain();
		if (ImGui::SliderFloat("Gain", &gain, 0.0f, 10.0f))
		{
			noise.SetFractalGain(gain);
			modified = true;
		}
	}

	ImGui::End();

	return modified;
}

void GameState::NoiseToImage(en::Noise& noise, sf::Image& image)
{
	const en::F32 width = static_cast<en::F32>(image.getSize().x);
	const en::F32 height = static_cast<en::F32>(image.getSize().y);
	for (en::F32 x = 0; x < width; x += 1.0f)
	{
		for (en::F32 y = 0; y < height; y += 1.0f)
		{
			const en::F32 noiseValue = noise.Get(x, y) * 0.5f + 0.5f;
			const en::U8 grey = noiseValue * 255;
			sf::Color color;
			color.r = grey;
			color.g = grey;
			color.b = grey;
			image.setPixel((unsigned int)x, (unsigned int)y, color);
		}
	}
}
