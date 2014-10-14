# Shader sources
list(APPEND SRC_SHADER
	Shader/Shader.h
	Shader/Program.cpp
	Shader/Program.h
	Shader/UniformHandle.h
	Shader/UniformHandle.cpp
)
SOURCE_GROUP("Shader" FILES ${SRC_SHADER})

# Geometry sources
list(APPEND SRC_GEOMETRY
	Geometry/Mesh.h
	Geometry/Mesh.cpp
)
SOURCE_GROUP("Geometry" FILES ${SRC_GEOMETRY})

# Source files
list(APPEND SRC_SOURCE
	main.cpp
)
SOURCE_GROUP("Source" FILES ${SRC_SOURCE})

list(APPEND SOURCES ${SRC_SHADER} ${SRC_GEOMETRY} ${SRC_SOURCE})

# Stuff
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries (${PROJECT_NAME} glfw GLEW ${OPENGL_LIBRARIES})