#pragma once

#include <Enlivengine/System/Assert.hpp>
#include <Enlivengine/System/PrimitiveTypes.hpp>
#include <SFML/Graphics/Color.hpp>
#include <map>

class ColorGradient
{
public:
	ColorGradient() : mColors()
	{
	}

	sf::Color& operator[](en::F32 position)
	{
		assert(position >= 0.0f && position <= 1.0f);
		return mColors[position];
	}

	const sf::Color& operator[](en::F32 position) const
	{
		assert(position >= 0.0f && position <= 1.0f);
		return mColors.at(position);
	}

	sf::Color GetSampleColor(en::F32 position) const
	{
		// Make sure the positions 0 and 1 are set
		assert(mColors.count(0.0f) && mColors.count(1.0f));

		// Find first entry >= position, return color if entry == position
		auto nextColor = mColors.lower_bound(position);
		if (nextColor->first == position)
			return nextColor->second;

		// Find first entry < position
		auto prevColor = std::prev(nextColor);

		// Interpolate color between 2 entries
		const en::F32 interpolation = (position - prevColor->first) / (nextColor->first - prevColor->first);
		return BlendColors(prevColor->second, nextColor->second, interpolation);
	}

	static sf::Color BlendColors(const sf::Color& firstColor, const sf::Color& secondColor, en::F32 interpolation)
	{
		assert(interpolation >= 0.f && interpolation <= 1.f);

		const en::F32 firstPart = 1.f - interpolation;

		return sf::Color(
			static_cast<sf::Uint8>(firstPart * firstColor.r + interpolation * secondColor.r),
			static_cast<sf::Uint8>(firstPart * firstColor.g + interpolation * secondColor.g),
			static_cast<sf::Uint8>(firstPart * firstColor.b + interpolation * secondColor.b),
			static_cast<sf::Uint8>(firstPart * firstColor.a + interpolation * secondColor.a));
	}

private:
	std::map<en::F32, sf::Color> mColors;
};