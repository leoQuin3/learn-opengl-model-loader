add_executable(model_loader_main
	main.cpp
	"shader.cpp"
	"mesh.cpp"
	"model.cpp"
	"stb_image.cpp"
	"camera.cpp"
	"mesh.h"
	"model.h"
	"stb_image.h"
	"camera.h"
)

add_compile_definitions(PROJECT_ROOT_DIR="${CMAKE_SOURCE_DIR}")

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

target_link_libraries(model_loader_main
	PRIVATE glad::glad
	PRIVATE glfw
	PRIVATE glm::glm
	PRIVATE assimp::assimp
)