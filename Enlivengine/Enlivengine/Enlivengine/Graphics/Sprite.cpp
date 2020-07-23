#include <Enlivengine/Graphics/Sprite.hpp>

namespace en
{

Sprite::Sprite()
	: mTexture(nullptr)
	, mTextureRect()
{
}

Sprite::Sprite(const Texture& texture)
	: mTexture(nullptr)
	, mTextureRect()
{
	SetTexture(texture);
}

Sprite::Sprite(const Texture& texture, const Recti& textureRect)
	: mTexture(nullptr)
	, mTextureRect()
{
	SetTexture(texture);
	SetTextureRect(textureRect);
}

Sprite::Sprite(const ResourceID& textureID)
	: mTexture(nullptr)
	, mTextureRect()
{
	SetTextureID(textureID);
}

Sprite::Sprite(const ResourceID& textureID, const Recti& textureRect)
	: mTexture(nullptr)
	, mTextureRect()
{
	SetTextureID(textureID);
	SetTextureRect(textureRect);
}

void Sprite::SetTexture(const Texture& texture, bool resetRect)
{
	if (resetRect || (!mTexture && (mTextureRect == Recti())))
	{
		SetTextureRect(Recti(0, 0, texture.getSize().x, texture.getSize().y));
	}
	mTexture = &texture;
}

const Texture* Sprite::GetTexture() const
{
	return mTexture;
}

void Sprite::SetTextureID(const ResourceID& textureID, bool resetRect)
{
	TexturePtr texturePtr = ResourceManager::GetInstance().Get<Texture>(textureID);
	if (texturePtr.IsValid())
	{
		SetTexture(texturePtr.Get(), resetRect);
	}
	else
	{
		enAssert(false);
		mTexture = nullptr;
	}
}

ResourceID Sprite::GetTextureID() const
{
	if (mTexture != nullptr)
	{
		return mTexture->GetID();
	}
	else
	{
		return InvalidResourceID;
	}
}

void Sprite::SetTextureRect(const Recti& textureRect)
{
	if (mTextureRect != textureRect)
	{
		mTextureRect = textureRect;
		UpdatePositions();
		UpdateTexCoords();
	}
}

const Recti& Sprite::GetTextureRect() const
{
	return mTextureRect;
}

void Sprite::SetOrigin(const Vector2f& origin)
{
	mOrigin = origin;
}

void Sprite::SetOrigin(F32 x, F32 y)
{
	mOrigin.set(x, y);
}

const Vector2f& Sprite::GetOrigin() const
{
	return mOrigin;
}

Rectf Sprite::GetBounds() const
{
	const F32 x = static_cast<F32>(Math::Abs(mTextureRect.width()));
	const F32 y = static_cast<F32>(Math::Abs(mTextureRect.height()));
	return Rectf(0.0f, 0.0f, x, y);
}

void Sprite::Render(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (mTexture != nullptr)
	{
		states.texture = mTexture;
		states.transform.translate(-mOrigin.x, -mOrigin.y);
		target.draw(mVertices, 4, sf::PrimitiveType::TriangleStrip, states);
	}
}

void Sprite::UpdatePositions()
{
	const Rectf bounds = GetBounds();
	mVertices[0].position = sf::Vector2f(0.0f, 0.0f);
	mVertices[1].position = sf::Vector2f(0.0f, bounds.height());
	mVertices[2].position = sf::Vector2f(bounds.width(), 0.0f);
	mVertices[3].position = sf::Vector2f(bounds.width(), bounds.height());
}

void Sprite::UpdateTexCoords()
{
	const F32 left = static_cast<F32>(mTextureRect.left());
	const F32 right = left + mTextureRect.width();
	const F32 top = static_cast<F32>(mTextureRect.top());
	const F32 bottom = top + mTextureRect.height();
	mVertices[0].texCoords = sf::Vector2f(left, top);
	mVertices[1].texCoords = sf::Vector2f(left, bottom);
	mVertices[2].texCoords = sf::Vector2f(right, top);
	mVertices[3].texCoords = sf::Vector2f(right, bottom);
}

} // namespace en
