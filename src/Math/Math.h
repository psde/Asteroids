#pragma once

#include <cmath>

#include "Vector2.h"
#include "Vector4.h"

namespace glm
{
	template <typename T>
	vec2 rotate(vec2 const &v, T const& angle)
	{
		vec2 Result;
		T const Cos(cos(angle));
		T const Sin(sin(angle));

		Result.x = v.x * Cos - v.y * Sin;
		Result.y = v.x * Sin + v.y * Cos;
		return Result;
	}


	template <typename T>
	T pi()
	{
		return 3.14159265359;
	}

	vec2 normalize(vec2 const &x)
	{
		float sqr = x.x * x.x + x.y * x.y;
		return x * (1.f / (float)std::sqrt(sqr));
	}

	float length(vec2 const& v)
	{
		float sqr = v.x * v.x + v.y * v.y;
		return (float)std::sqrt(sqr);
	}
}