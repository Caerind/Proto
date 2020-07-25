#include <Enlivengine/Core/World.hpp>

namespace en
{

World::World()
	: mEntityManager(*this)
	, mGameView()
#ifdef ENLIVE_DEBUG
	, mFreeCamView()
#endif // ENLIVE_DEBUG
	, mPlaying(false)
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

void World::Play()
{
	mPlaying = true;
}

void World::Pause()
{
	mPlaying = false;
}

bool World::IsPlaying() const
{
	return mPlaying;
}

void World::SetIDGenerator(U32 idGenerator)
{
	mIDGenerator = idGenerator;
}

U32 World::GetIDGenerator() const
{
	return mIDGenerator;
}

U32 World::CreateID()
{
	return mIDGenerator++;
}

} // namespace en