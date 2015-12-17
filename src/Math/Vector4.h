#pragma once

#include "Vector2.h"

namespace Math
{
	class vec4
	{
	public:
		float x;
		float y;
		float z;
		float w;

		vec4()
			: vec4(0.f)
		{ }

		template<class T>
		vec4(T v)
			: vec4(v, v, v, v)
		{ }

		vec4(vec2 a, vec2 b)
			: vec4(a.x, a.y, b.x, b.y)
		{ }

		template<class T1, class T2, class T3, class T4>
		vec4(T1 x, T2 y, T3 z, T4 w)
			: x(static_cast<float>(x))
			, y(static_cast<float>(y))
			, z(static_cast<float>(z))
			, w(static_cast<float>(w))
		{ }
	};

	inline vec4 operator+(vec4 const& a, vec4 const& b)
	{
		return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
	}
}
