#include <Enlivengine/Core/World.hpp>

namespace en
{

World::World()
	: mEntityManager(*this)
{
}

EntityManager& World::GetEntityManager()
{
	return mEntityManager;
}

const EntityManager& World::GetEntityManager() const
{
	return mEntityManager;
}

View& World::GetGameView()
{
	return mGameView;
}

const View& World::GetGameView() const
{
	return mGameView;
}

#ifdef ENLIVE_DEBUG
View& World::GetFreeCamView()
{
	return mFreeCamView;
}

const View& World::GetFreeCamView() const
{
	return mFreeCamView;
}
#endif // ENLIVE_DEBUG

} // namespace en