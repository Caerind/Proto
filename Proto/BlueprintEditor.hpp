#pragma once

#include <Enlivengine/System/PrimitiveTypes.hpp>

#if defined(ENLIVE_ENABLE_IMGUI)

#include <Enlivengine/Application/ImGuiToolManager.hpp>
#include <Enlivengine/Math/Noise.hpp>

#include <vector>
#include <string>

#include <imgui-node-editor/imgui_node_editor.h>
#include <imgui/imgui_internal.h>

#include "Blueprint.hpp"

namespace en
{

class ImGuiBlueprintEditor : public ImGuiTool
{
	ENLIVE_SINGLETON(ImGuiBlueprintEditor);
	~ImGuiBlueprintEditor();

public:
	virtual ImGuiToolTab GetTab() const;
	virtual const char* GetName() const;

	void Display();

	bool Initialize();
	void Uninitialize();
	bool IsInitialized() const;

public:
	enum class PinType
	{
		Bool,
		Int,
		Float
	};

	enum class PinKind
	{
		Input,
		Output
	};

	enum class NodeType
	{
		Blueprint,
		Comment
	};

	struct Node;

	struct Pin
	{
		ax::NodeEditor::PinId id;
		std::string name;
		Node* node;
		PinType type;
		PinKind kind;
	};

	struct Node
	{
		ax::NodeEditor::NodeId id;
		std::string name;
		std::vector<Pin> inputs;
		std::vector<Pin> outputs;
		ImColor color;
		NodeType type;
		ImVec2 size;
	};

	struct Link
	{
		ax::NodeEditor::LinkId id;
		ax::NodeEditor::PinId start;
		ax::NodeEditor::PinId end;
		ImColor color;
	};

private:
	void LeftPanel();

	void NodeEditor();

	void BlueprintList();

private:
	ax::NodeEditor::EditorContext* mEditorContext;

	std::vector<Block*> mBlocks;

	std::vector<Node> mNodes;
	std::vector<Link> mLinks;
};

} // namespace en

#endif // ENLIVE_ENABLE_IMGUI