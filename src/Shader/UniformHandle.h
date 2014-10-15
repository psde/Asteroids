#pragma once

#include <GL/glew.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Shader
{
	class UniformHandle
	{
	private:
		GLint *_location;

	public:
		UniformHandle(GLint *location)
			: _location(location)
		{
		}

		void update(const glm::mat4 &val)
		{
			glUniformMatrix4fv(*_location, 1, GL_FALSE, glm::value_ptr(val));
		}

		void update(const float &val)
		{
			glUniform1f(*_location, val);
		}
	};
}