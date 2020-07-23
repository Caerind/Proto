#pragma once

#include <SFML/Graphics/Vertex.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <Enlivengine/System/Meta.hpp>
#include <Enlivengine/Math/Rect.hpp>
#include <Enlivengine/Graphics/SFMLResources.hpp>
#include <Enlivengine/Graphics/SFMLWrapper.hpp>

// TODO : Color ?
// TODO : Origin ?

namespace en
{

class Sprite
{
public:
	Sprite();
	Sprite(const Texture& texture);
	Sprite(const Texture& texture, const Recti& textureRect);
	Sprite(const ResourceID& textureID);
	Sprite(const ResourceID& textureID, const Recti& textureRect);

	void SetTexture(const Texture& texture, bool resetRect = false);
	const Texture* GetTexture() const;

	void SetTextureID(const ResourceID& textureID, bool resetRect = false);
	ResourceID GetTextureID() const;

	void SetTextureRect(const Recti& textureRect);
	const Recti& GetTextureRect() const;

	void SetOrigin(const Vector2f& origin);
	void SetOrigin(F32 x, F32 y);
	const Vector2f& GetOrigin() const;

	Rectf GetBounds() const;

	void Render(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	void UpdatePositions();
	void UpdateTexCoords();

private:
	sf::Vertex mVertices[4];
	const Texture* mTexture;
	Recti mTextureRect;
	Vector2f mOrigin;
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::Sprite)
ENLIVE_META_CLASS_END()