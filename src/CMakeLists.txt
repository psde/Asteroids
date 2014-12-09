# Graphics sources
list(APPEND SRC_GRAPHICS
	Graphics/Graphics.h
	Graphics/Graphics.cpp
)
SOURCE_GROUP("Graphics" FILES ${SRC_GRAPHICS})

# Shader sources
list(APPEND SRC_SHADER
	Graphics/Shader/Globals.h
	Graphics/Shader/Program.cpp
	Graphics/Shader/Program.h
	Graphics/Shader/Shader.h
	Graphics/Shader/Shader.cpp
	Graphics/Shader/UniformAssigner.h
	Graphics/Shader/UniformAssigner.cpp 
)
SOURCE_GROUP("Graphics\\Shader" FILES ${SRC_SHADER})

# Geometry sources
list(APPEND SRC_GEOMETRY
	Graphics/Geometry/Mesh.h
	Graphics/Geometry/Mesh.cpp
)
SOURCE_GROUP("Graphics\\Geometry" FILES ${SRC_GEOMETRY})

# Window sources
list(APPEND SRC_WINDOW
	Graphics/Window/Window.h
	Graphics/Window/Window.cpp
)
SOURCE_GROUP("Graphics\\Window" FILES ${SRC_WINDOW})

# Math sources
list(APPEND SRC_MATH
	Math/Math.h
	Math/Vector2.h
	Math/Vector4.h
)
SOURCE_GROUP("Math" FILES ${SRC_MATH})

# Game sources
list(APPEND SRC_GAME
	Game/Asteroid.h
	Game/Asteroid.cpp
	Game/FontRenderer.h
	Game/FontRenderer.cpp
	Game/Game.h
	Game/Game.cpp
	Game/GameObject.h
	Game/GameObject.cpp
	Game/ParticleEmitter.h
	Game/ParticleEmitter.cpp
	Game/Projectile.h
	Game/Projectile.cpp
	Game/Ship.h
	Game/Ship.cpp
	Game/UFO.h
	Game/UFO.cpp
)
SOURCE_GROUP("Game" FILES ${SRC_GAME})

# Components sources
list(APPEND SRC_COMPONENTS
	Game/Components/ColliderComponent.h
	Game/Components/ColliderComponent.cpp
	Game/Components/PhysicsComponent.h
	Game/Components/PhysicsComponent.cpp
	Game/Components/PhysicsRungeKutta.h
	Game/Components/PhysicsRungeKutta.cpp
	Game/Components/PhysicsEuler.h
	Game/Components/PhysicsEuler.cpp
)
SOURCE_GROUP("Game\\Components" FILES ${SRC_COMPONENTS})

# Source files
list(APPEND SRC_SOURCE
	main.cpp
)
SOURCE_GROUP("Source" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_GRAPHICS} ${SRC_SHADER} ${SRC_GEOMETRY} ${SRC_WINDOW} ${SRC_MATH} ${SRC_GAME} ${SRC_COMPONENTS} ${SRC_SOURCE})

add_definitions("-DGLEW_STATIC")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} ${GLFW3_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARIES})