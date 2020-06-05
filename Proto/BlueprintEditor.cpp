#include "BlueprintEditor.hpp"

#ifdef ENLIVE_ENABLE_IMGUI

namespace en
{

ImGuiBlueprintEditor::ImGuiBlueprintEditor()
	: ImGuiTool()
	, mEditorContext(nullptr)
{
}

ImGuiBlueprintEditor::~ImGuiBlueprintEditor()
{
	if (mEditorContext != nullptr)
	{
		ax::NodeEditor::DestroyEditor(mEditorContext);
	}
}

ImGuiToolTab ImGuiBlueprintEditor::GetTab() const
{
	return ImGuiToolTab::Main;
}

const char* ImGuiBlueprintEditor::GetName() const
{
	return ICON_FA_SITEMAP " BlueprintEditor";
}

void ImGuiBlueprintEditor::Display()
{
	if (IsInitialized())
	{
		ax::NodeEditor::SetCurrentEditor(mEditorContext);
		ImGui::Columns(2, "blueprintEditorColumns");
		LeftPanel();
		ImGui::NextColumn();
		NodeEditor();
		ImGui::NextColumn();
		ImGui::Columns(1);
		ax::NodeEditor::SetCurrentEditor(nullptr);
	}
	else
	{
		BlueprintList();
	}
}

bool ImGuiBlueprintEditor::Initialize()
{
	if (IsInitialized())
	{
		Uninitialize();
	}

	ax::NodeEditor::Config config;
	config.SettingsFile = "NoiseEditor.json";
	mEditorContext = ax::NodeEditor::CreateEditor(&config);

	mVisible = true;

	return true;
}

void ImGuiBlueprintEditor::Uninitialize()
{
	if (IsInitialized())
	{
		// TODO : Save ?

		ax::NodeEditor::DestroyEditor(mEditorContext);

		mVisible = false;
	}
}

bool ImGuiBlueprintEditor::IsInitialized() const
{
	return mEditorContext != nullptr;
}

void ImGuiBlueprintEditor::LeftPanel()
{
	static int initialLeftColSize = true;
	if (initialLeftColSize)
	{
		ImGui::SetColumnWidth(0, 240.0f);
		initialLeftColSize = false;
	}

	if (ImGui::Button("New constant"))
	{
		mBlocks.emplace_back(new ConstantBlock());
	}
	if (ImGui::Button("New add"))
	{
		mBlocks.emplace_back(new AddBlock());
	}
}

void ImGuiBlueprintEditor::NodeEditor()
{
	ax::NodeEditor::Begin("NoiseEditor", ImVec2(0.0f, 0.0f));

	static constexpr float padding = 12.0f;
	static constexpr float rounding = 10.0f;

	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBg, ImColor(229, 229, 229, 200));
	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_NodeBorder, ImColor(125, 125, 125, 200));
	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_PinRect, ImColor(229, 229, 229, 60));
	ax::NodeEditor::PushStyleColor(ax::NodeEditor::StyleColor_PinRectBorder, ImColor(125, 125, 125, 60));

	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodePadding, ImVec4(0, 0, 0, 0));
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_NodeRounding, rounding);
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_SourceDirection, ImVec2(0.0f, 1.0f));
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_TargetDirection, ImVec2(0.0f, -1.0f));
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_LinkStrength, 0.0f);
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinBorderWidth, 1.0f);
	ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinRadius, 6.0f);

	const en::U32 blockCount = static_cast<en::U32>(mBlocks.size());
	for (en::U32 i = 0; i < blockCount; ++i)
	{
		ax::NodeEditor::BeginNode(i);

		ImGui::Text(mBlocks[i]->GetName());

		switch (mBlocks[i]->GetTypeID())
		{
		case ConstantBlock::GetStaticTypeID():
		{
			if (ConstantBlock* block = static_cast<ConstantBlock*>(mBlocks[i]))
			{
				ImGui::InputFloat("", &block->constant);
			}
			break;
		}
		case AddBlock::GetStaticTypeID():
		{
			if (AddBlock* block = static_cast<AddBlock*>(mBlocks[i]))
			{
			}
			break;
		}
		default: assert(false); break;
		}

		const en::U32 inputCount = mBlocks[i]->GetInputCount();
		if (inputCount > 0)
		{
			ImRect inputsRect;
			for (en::U32 j = 0; j < inputCount; ++j)
			{
				ImGui::Dummy(ImVec2(padding, padding));
				inputsRect = ImRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax());
				inputsRect.Min.y -= padding;
				inputsRect.Max.y -= padding;

				ax::NodeEditor::PushStyleVar(ax::NodeEditor::StyleVar_PinCorners, 15);
				ax::NodeEditor::BeginPin((j + 1) * blockCount + i, ax::NodeEditor::PinKind::Input);
				ax::NodeEditor::PinPivotRect(inputsRect.GetCenter(), inputsRect.GetCenter());
				ax::NodeEditor::PinRect(inputsRect.GetTL(), inputsRect.GetBR());
				ax::NodeEditor::EndPin();
				ax::NodeEditor::PopStyleVar(1);
			}
		}

		// TODO

		ax::NodeEditor::EndNode(); 
	}

	ax::NodeEditor::PopStyleVar(7);
	ax::NodeEditor::PopStyleColor(4);

	ax::NodeEditor::End();
}

void ImGuiBlueprintEditor::BlueprintList()
{

}

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI