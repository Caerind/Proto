set(SRC_PROTO
	Blueprint.cpp
	Blueprint.hpp
	ColorGradient.hpp
	GameSingleton.cpp
	GameSingleton.hpp
	GameState.cpp
	GameState.hpp
	main.cpp
	NoiseEditor.cpp
	NoiseEditor.hpp
)
source_group("" FILES ${SRC_PROTO})

add_executable(Proto ${SRC_PROTO})
target_link_libraries(Proto PUBLIC Enlivengine)