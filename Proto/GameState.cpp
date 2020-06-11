#include "GameState.hpp"

#include <Enlivengine/System/Profiler.hpp>
#include <Enlivengine/Application/Application.hpp>
#include <Enlivengine/Graphics/LinearColor.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>
#include <Enlivengine/System/ClassManager.hpp>

#include <imgui/imgui.h>

GameState::GameState(en::StateManager& manager)
	: en::State(manager)
	, mAs("mAs")
	, mAaa("mAaa")
{
	en::ImGuiBlueprintEditor::GetInstance().Register();
	en::ImGuiBlueprintEditor::GetInstance().Initialize();

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
	mTemperatureGradient[0.0f] = sf::Color(0, 0, 255);
	mTemperatureGradient[0.25f] = sf::Color(0, 255, 255);
	mTemperatureGradient[0.5f] = sf::Color(0, 255, 0);
	mTemperatureGradient[0.75f] = sf::Color(255, 255, 0);
	mTemperatureGradient[1.0f] = sf::Color(255, 0, 0);

	mWaterGradient[0.0f] = sf::Color(63, 72, 204);
	mWaterGradient[0.5f] = sf::Color(0, 162, 232);
	mWaterGradient[1.0f] = sf::Color(153, 217, 234);
	mMapImage.create(1024, 768);
	UpdateMap();

	mDisplay = 0;
	mTexture.loadFromImage(mMapImage);

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

	return false;
}

bool GameState::update(en::Time dt)
{
	ENLIVE_PROFILE_FUNCTION();

	if (!en::Application::GetInstance().IsRunning())
	{
		return false;
	}

	if (ImGui::Begin("ObjectEditor"))
	{
		if (ImGui::Button("Save As"))
		{
			DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Serialize(mAs, "As");
			file.SaveToFile("DataFileGame.xml");
		}
		ImGui::SameLine(); 
		if (ImGui::Button("Load As"))
		{
			mAs.DeleteAll();
			mAs.Clear();

			DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Deserialize(mAs, "As");
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear As"))
		{
			mAs.DeleteAll();
			mAs.Clear();
		}
		ObjectEditor::ImGuiEditor(mAs, "As");

		if (ImGui::Button("Save A"))
		{
			DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Serialize(mAaa, "A");
			file.SaveToFile("DataFileGame.xml");
		}
		ImGui::SameLine();
		if (ImGui::Button("Load A"))
		{
			mAaa.DeleteAll();
			mAaa.Clear();

			DataFile file;
			file.LoadFromFile("DataFileGame.xml");
			file.Deserialize(mAaa, "A");
		}
		ImGui::SameLine();
		if (ImGui::Button("Clear A"))
		{
			mAaa.DeleteAll();
			mAaa.Clear();
		}
		ObjectEditor::ImGuiEditor(mAaa, "A");

		if (mTestFactory != nullptr)
		{
			ObjectEditor::ImGuiEditor(mTestFactory, "TestFactory");
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

	return false;
}

void GameState::render(sf::RenderTarget& target)
{
	ENLIVE_PROFILE_FUNCTION();
	sf::Sprite sprite(mTexture);
	target.draw(sprite);
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
			mTemperatureImage.setPixel((unsigned int)x, (unsigned int)y, mTemperatureGradient.GetSampleColor(temperature));
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

			sf::Color color;
			if (elevation < 0.35f)
			{
				const en::F32 waterDeepFactor = elevation / 0.35f;
				color = mWaterGradient.GetSampleColor(waterDeepFactor);
			}
			else if (elevation < 0.45f)
			{
				en::U8 hum = humidity * humidity * humidity * 200;
				color = sf::Color(232, 211, 160);
				color.r -= hum;
				color.g -= hum;
				color.b -= hum;
			}
			else if (elevation > 0.85f)
			{
				color = sf::Color::White;
			}
			else if (elevation > 0.80f)
			{
				color = sf::Color(97, 97, 97);
			}
			else if (elevation > 0.70f)
			{
				color = sf::Color(87, 44, 0);
			}
			else
			{
				en::F32 invTemp = 1.0f - temperature;
				en::U8 temp = invTemp * invTemp * invTemp * 200;
				color = sf::Color(46, 122, 79); // Moderate
				color.r += temp;
				color.g += temp;
				color.b += temp;
			}

			mMapImage.setPixel(x, y, color);
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
