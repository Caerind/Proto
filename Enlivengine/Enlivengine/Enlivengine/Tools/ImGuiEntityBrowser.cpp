#include <Enlivengine/Tools/ImGuiEntityBrowser.hpp>

#ifdef ENLIVE_ENABLE_IMGUI

#include <imgui/imgui.h>

#include <Enlivengine/Core/Components.hpp>

namespace en
{

ImGuiEntityBrowser::ImGuiEntityBrowser()
	: ImGuiTool()
	, mCurrentWorld(nullptr)
{
}

ImGuiEntityBrowser::~ImGuiEntityBrowser()
{
}

ImGuiToolTab ImGuiEntityBrowser::GetTab() const
{
	return ImGuiToolTab::Engine;
}

const char* ImGuiEntityBrowser::GetName() const
{
	return ICON_FA_USER_COG " EntityBrowser";
}

void ImGuiEntityBrowser::Display()
{
	if (mCurrentWorld != nullptr)
	{
		static constexpr char unknownEntityName[] = "<Unknown>";

		static constexpr U32 entityNameMaxSize = 255;
		static char entityNameInput[entityNameMaxSize] = "";

		EntityManager& entityManager = mCurrentWorld->GetEntityManager();
		bool button = ImGui::Button("New Entity");
		ImGui::SameLine();
		bool input = ImGui::InputText("", entityNameInput, entityNameMaxSize, ImGuiInputTextFlags_EnterReturnsTrue);
		if (button || input)
		{
			Entity newEntity = entityManager.CreateEntity();
			if (strlen(entityNameInput) > 0)
			{
				if (newEntity.IsValid())
				{
					newEntity.Add<NameComponent>(entityNameInput);

					mSelectedEntities.push_back(newEntity.GetEntity());
				}
#ifdef ENLIVE_COMPILER_MSVC
				strcpy_s(entityNameInput, "");
#else
				strcpy(entityNameInput, "");
#endif // ENLIVE_COMPILER_MSVC
			}
		}

		entityManager.Each([this, &entityManager](auto entityEntt)
		{
			en::Entity entity(entityManager, entityEntt);
			if (entity.IsValid())
			{
				ImGui::PushID(entity.GetID());
				if (ImGui::Button("x"))
				{
					entityManager.DestroyEntity(entity);
				}
				else
				{
					ImGui::SameLine();
					const char* entityName = entity.GetName();
					if (entityName == nullptr || strlen(entityName) == 0)
					{
						entityName = unknownEntityName;
					}
					if (ImGui::Button(entityName))
					{
						const auto& selectEnt = entity.GetEntity();
						bool alreadySelected = false;
						for (const auto& ent : mSelectedEntities)
						{
							if (ent == selectEnt)
							{
								alreadySelected = true;
								break;
							}
						}
						if (!alreadySelected)
						{
							mSelectedEntities.push_back(selectEnt);
						}
					}
				}
				ImGui::PopID();
			}
		});

		en::U32 selectedEntities = static_cast<U32>(mSelectedEntities.size());
		for (en::U32 i = 0; i < selectedEntities; )
		{
			Entity entity(mCurrentWorld->GetEntityManager(), mSelectedEntities[i]);
			bool selected = true;
			if (entity.IsValid())
			{
				const char* entityName = entity.GetName();
				if (entityName == nullptr || strlen(entityName) == 0)
				{
					entityName = unknownEntityName;
				}
				if (ImGui::Begin(entityName, &selected))
				{
					CustomObjectEditor<en::Entity>::ImGuiEditor(entity, nullptr);
					ImGui::End();
				}
				else
				{
					ImGui::End();
					selected = false;
				}
			}
			else
			{
				selected = false;
			}

			if (!selected)
			{
				mSelectedEntities.erase(mSelectedEntities.begin() + i);
				selectedEntities--;
			}
			else
			{
				i++;
			}
		}

	}
	else
	{
		ImGui::Text("No CurrentWorld for EntityBrowser");
	}
}

bool ImGuiEntityBrowser::IsSelected(const Entity& entity) const
{
	if (entity.IsValid() && mCurrentWorld != nullptr && &entity.GetWorld() == mCurrentWorld)
	{
		const auto& entityToTest = entity.GetEntity();
		for (const auto& ent : mSelectedEntities)
		{
			if (ent == entityToTest)
			{
				return true;
			}
		}
	}
	return false;
}

void ImGuiEntityBrowser::SetCurrentWorld(World* world)
{
	mCurrentWorld = world;
	mSelectedEntities.clear();
}

World* ImGuiEntityBrowser::GetCurrentWorld()
{
	return mCurrentWorld;
}

const World* ImGuiEntityBrowser::GetCurrentWorld() const
{
	return mCurrentWorld;
}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI
