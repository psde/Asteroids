#include "UniformAssigner.h"

namespace Graphics
{
	UniformAssigner::UniformAssigner(GLuint location) : m_location(location){}

	/*UniformAssigner& UniformAssigner::operator=(const Math::mat4 &val)
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, Math::value_ptr(val));
		return *this;
	}*/

	UniformAssigner& UniformAssigner::operator=(const Math::vec2 &val)
	{
		glUniform2f(m_location, val.x, val.y);
		return *this;
	}

	UniformAssigner& UniformAssigner::operator=(const float &val)
	{
		glUniform1f(m_location, val);
		return *this;
	}
}
