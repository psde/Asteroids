#include "UniformAssigner.h"

namespace Shader
{
	UniformAssigner::UniformAssigner(GLuint location) : _location(location){}

	/*UniformAssigner& UniformAssigner::operator=(const glm::mat4 &val)
	{
		glUniformMatrix4fv(_location, 1, GL_FALSE, glm::value_ptr(val));
		return *this;
	}*/

	UniformAssigner& UniformAssigner::operator=(const glm::vec2 &val)
	{
		glUniform2f(_location, val.x, val.y);
		return *this;
	}

	UniformAssigner& UniformAssigner::operator=(const float &val)
	{
		glUniform1f(_location, val);
		return *this;
	}
}