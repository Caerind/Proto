#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/Core/World.hpp>

namespace en
{

class ImGuiEntityBrowser : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiEntityBrowser);
	~ImGuiEntityBrowser();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	virtual void Display();

	// TODO : Have a GetCurrentWorld more global to the engine ?
	void SetCurrentWorld(World* world);
	World* GetCurrentWorld();
	const World* GetCurrentWorld() const;

private:
	World* mCurrentWorld;
	std::vector<entt::entity> mSelectedEntities;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
