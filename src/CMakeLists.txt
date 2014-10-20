# Shader sources
list(APPEND SRC_SHADER
	Shader/Globals.h
	Shader/Shader.h
	Shader/Shader.cpp
	Shader/Program.cpp
	Shader/Program.h
	Shader/UniformAssigner.h
	Shader/UniformAssigner.cpp 
)
SOURCE_GROUP("Shader" FILES ${SRC_SHADER})

# Geometry sources
list(APPEND SRC_GEOMETRY
	Geometry/Mesh.h
	Geometry/Mesh.cpp
)
SOURCE_GROUP("Geometry" FILES ${SRC_GEOMETRY})

# Game sources
list(APPEND SRC_GAME
	Game/Asteroid.h
	Game/Asteroid.cpp
	Game/PhysicsObject.h
	Game/PhysicsObject.cpp
	Game/FontRenderer.h
	Game/FontRenderer.cpp
	Game/Ship.h
	Game/Ship.cpp
)
SOURCE_GROUP("Game" FILES ${SRC_GAME})

# Source files
list(APPEND SRC_SOURCE
	main.cpp
)
SOURCE_GROUP("Source" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_SHADER} ${SRC_GEOMETRY} ${SRC_GAME} ${SRC_SOURCE})

add_definitions("-DGLEW_STATIC -DGLM_FORCE_RADIANS")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})