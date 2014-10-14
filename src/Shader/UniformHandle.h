#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Shader
{
	template<typename T>
	class UniformHandle
	{
		friend class Program;
	private:
		GLuint *_location;
		UniformHandle(GLuint *location)
			: _location(location)
		{
		}

	public:
		UniformHandle &operator=(const T &rhs)
		{
			set(rhs);
			return *this;
		}

		void set(const glm::mat4 &val)
		{
			glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(val));
		}

		void set(const float &val)
		{
			glUniform1f(0, val);
		}
	};
}