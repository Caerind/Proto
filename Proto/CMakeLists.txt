set(SRC_PROTO
	Components.hpp
	GameSingleton.cpp
	GameSingleton.hpp
	GameState.cpp
	GameState.hpp
	main.cpp
	Meta.cpp
	Meta.hpp
)
source_group("" FILES ${SRC_PROTO})

add_executable(Proto ${SRC_PROTO})
target_link_libraries(Proto PUBLIC Enlivengine)