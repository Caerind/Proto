#include <Enlivengine/Graphics/Text.hpp>

namespace en
{

Text::Text()
	: mText()
	, mFont(nullptr)
{
}

void Text::SetFont(const Font& font)
{
	mText.setFont(font);
	mFont = &font;
}

const Font* Text::GetFont() const
{
	return mFont;
}

void Text::SetFontID(const ResourceID& fontID)
{
	FontPtr fontPtr = ResourceManager::GetInstance().Get<Font>(fontID);
	if (fontPtr.IsValid())
	{
		SetFont(fontPtr.Get());
	}
	else
	{
		enAssert(false);
		mFont = nullptr;
	}
}

ResourceID Text::GetFontID() const
{
	if (mFont != nullptr)
	{
		return mFont->GetID();
	}
	else
	{
		return InvalidResourceID;
	}
}

void Text::SetString(const sf::String& string)
{
	mText.setString(string);
}

const sf::String& Text::GetString() const
{
	return mText.getString();
}

void Text::SetCharacterSize(U32 characterSize)
{
	mText.setCharacterSize(static_cast<sf::Uint32>(characterSize));
}

U32 Text::GetCharacterSize() const
{
	return static_cast<U32>(mText.getCharacterSize());
}

void Text::SetLineSpacing(F32 lineSpacingFactor)
{
	mText.setLineSpacing(static_cast<float>(lineSpacingFactor));
}

F32 Text::GetLineSpacing() const
{
	return static_cast<F32>(mText.getLineSpacing());
}

void Text::SetLetterSpacing(F32 letterSpacingFactor)
{
	mText.setLetterSpacing(static_cast<float>(letterSpacingFactor));
}

F32 Text::GetLetterSpacing() const
{
	return static_cast<F32>(mText.getLetterSpacing());
}

void Text::SetStyle(U32 style)
{
	mText.setStyle(static_cast<sf::Uint32>(style));
}

U32 Text::GetStyle() const
{
	return static_cast<U32>(mText.getStyle());
}

void Text::SetFillColor(Color color)
{
	mText.setFillColor(toSF(color));
}

Color Text::GetFillColor() const
{
	return toEN(mText.getFillColor());
}

void Text::SetOutlineColor(Color color)
{
	mText.setOutlineColor(toSF(color));
}

Color Text::GetOutlineColor() const
{
	return toEN(mText.getOutlineColor());
}

void Text::SetOutlineThickness(F32 thickness)
{
	mText.setOutlineThickness(static_cast<float>(thickness));
}

F32 Text::GetOutlineThickness() const
{
	return static_cast<F32>(mText.getOutlineThickness());
}

void Text::SetOrigin(const Vector2f& origin)
{
	mOrigin = origin;
}

void Text::SetOrigin(F32 x, F32 y)
{
	mOrigin.set(x, y);
}

const Vector2f& Text::GetOrigin() const
{
	return mOrigin;
}

void Text::Render(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mFont != nullptr)
	{
		states.transform.translate(-mOrigin.x, -mOrigin.y);
		target.draw(mText, states);
	}
}

} // namespace en
