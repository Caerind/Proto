#pragma once

#include <Enlivengine/Application/StateManager.hpp>

#include <Enlivengine/Math/Noise.hpp>
#include <SFML/Graphics.hpp>

#include "ColorGradient.hpp"
#include "NoiseEditor.hpp"

class GameState : public en::State
{
public:
	GameState(en::StateManager& manager);

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
	ColorGradient mTemperatureGradient;

	sf::Image mMapImage;
	ColorGradient mWaterGradient;

	int mDisplay;
	sf::Texture mTexture;
};