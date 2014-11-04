#pragma once

#include "Vector2.h"

namespace glm
{
	class vec4
	{
	public:
		union {
			float x;
			float y;
			float z;
			float w;
		};

		vec4()
			: vec4(0.f)
		{ }

		vec4(float v)
			: vec4(v, v, v, v)
		{ }

		vec4(vec2 a, vec2 b)
			: vec4(a.x, a.y, b.x, b.y)
		{ }

		vec4(float x, float y, float z, float w)
			: x(x)
			, y(y)
			, z(z)
			, w(w)
		{ }
	};

	vec4 operator+(vec4 const& a, vec4 const& b)
	{
		return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
}