set(SRC_PROTO
	Blueprint.cpp
	Blueprint.hpp
	BlueprintEditor.cpp
	BlueprintEditor.hpp
	ColorGradient.hpp
	Components.cpp
	Components.hpp
	DataFile.cpp
	DataFile.hpp
	DataFileSpecialization.hpp
	GameSingleton.cpp
	GameSingleton.hpp
	GameState.cpp
	GameState.hpp
	main.cpp
	Meta.cpp
	Meta.hpp
	ObjectEditor.cpp
	ObjectEditor.hpp
	ObjectEditorSpecialization.hpp
)
source_group("" FILES ${SRC_PROTO})

add_executable(Proto ${SRC_PROTO})
target_link_libraries(Proto PUBLIC Enlivengine)