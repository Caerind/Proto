#pragma once

#include <Enlivengine/Application/StateManager.hpp>

#include <SFML/Graphics.hpp>
#include <Enlivengine/Math/Noise.hpp>
#include <Enlivengine/Graphics/ColorGradient.hpp>

#include <Enlivengine/Core/Components.hpp>
#include <Enlivengine/Core/EntityManager.hpp>
#include <Enlivengine/Core/Entity.hpp>
#include <Enlivengine/Core/ObjectEditor.hpp>
#include <Enlivengine/Core/DataFile.hpp>

#include "Meta.hpp"
#include "Components.hpp"

class GameState : public en::State
{
public:
	GameState(en::StateManager& manager);
	~GameState();

	bool handleEvent(const sf::Event& event);

	bool update(en::Time dt);

	void render(sf::RenderTarget& target);

	void UpdateTemperature();
	void UpdateMap();

	static bool ImGuiNoiseWindow(const char* title, en::Noise& noise);
	static void NoiseToImage(en::Noise& noise, sf::Image& image);

private:
	en::Noise mElevationNoise;
	sf::Image mElevationImage;

	en::Noise mHumidityNoise;
	sf::Image mHumidityImage;

	en::Noise mTemperatureNoise;
	sf::Image mTemperatureImage;
	en::ColorGradient mTemperatureGradient;

	sf::Image mMapImage;
	en::ColorGradient mWaterGradient;

	int mDisplay;
	sf::Texture mTexture;

	en::Array<TestClassA*> mAs;
	en::Array<Aaa*> mAaa;

	Aaa* mTestFactory;

	en::EntityManager mManager;
	en::Entity mEntity;
};