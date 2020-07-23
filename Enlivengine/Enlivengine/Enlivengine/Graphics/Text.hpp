#pragma once

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>

namespace en
{

class Text
{
public:
	Text();

	void SetFont(const Font& font);
	const Font* GetFont() const;

	void SetFontID(const ResourceID& fontID);
	ResourceID GetFontID() const;

	void SetString(const sf::String& string);
	const sf::String& GetString() const;

	void SetCharacterSize(U32 characterSize);
	U32 GetCharacterSize() const;

	void SetLineSpacing(F32 lineSpacingFactor);
	F32 GetLineSpacing() const;

	void SetLetterSpacing(F32 letterSpacingFactor);
	F32 GetLetterSpacing() const;

	void SetStyle(U32 style);
	U32 GetStyle() const;

	void SetFillColor(Color color);
	Color GetFillColor() const;

	void SetOutlineColor(Color color);
	Color GetOutlineColor() const;

	void SetOutlineThickness(F32 thickness);
	F32 GetOutlineThickness() const;

	void SetOrigin(const Vector2f& origin);
	void SetOrigin(F32 x, F32 y);
	const Vector2f& GetOrigin() const;

	void Render(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text mText;
	const Font* mFont;
	Vector2f mOrigin;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Text)
ENLIVE_META_CLASS_END()