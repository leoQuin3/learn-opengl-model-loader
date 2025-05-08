add_executable(model_loader_main
	main.cpp
	"shader.cpp"
	"stb_image.h"
 "Mesh.h" "Mesh.cpp")

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