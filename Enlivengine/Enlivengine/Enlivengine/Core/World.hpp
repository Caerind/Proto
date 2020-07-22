#pragma once

#include <Enlivengine/Graphics/View.hpp>
#include <Enlivengine/Core/EntityManager.hpp>

namespace en
{

class World
{
public:
	World();

	EntityManager& GetEntityManager();
	const EntityManager& GetEntityManager() const;

	View& GetGameView();
	const View& GetGameView() const;

#ifdef ENLIVE_DEBUG
	View& GetFreeCamView();
	const View& GetFreeCamView() const;
#endif // ENLIVE_DEBUG

private:
	EntityManager mEntityManager;

	View mGameView;

#ifdef ENLIVE_DEBUG
	View mFreeCamView;
#endif // ENLIVE_DEBUG

	ENLIVE_META_CLASS();
};

} // namespace en

ENLIVE_META_CLASS_BEGIN(en::World)
	ENLIVE_META_CLASS_MEMBER("EntityManager", &en::World::mEntityManager)
ENLIVE_META_CLASS_END()