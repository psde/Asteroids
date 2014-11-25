#pragma once

#include <GL/glew.h>

#include "Math/Math.h"

namespace Graphics
{
	class UniformAssigner
	{
	private:
		GLuint _location;

	public:
		explicit UniformAssigner(GLuint location);

		//UniformAssigner& operator=(const Math::mat4 &val);

		UniformAssigner& operator=(const Math::vec2 &val);

		UniformAssigner& operator=(const float &val);
	};
}