set(SRC_PROTO
	ColorGradient.hpp
	ComponentManager.cpp
	ComponentManager.hpp
	Components.hpp
	CoreComponents.hpp
	DataFile.cpp
	DataFile.hpp
	DataFileSpecialization.hpp
	Entity.cpp
	Entity.hpp
	EntityManager.cpp
	EntityManager.hpp
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