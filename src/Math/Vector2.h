#pragma once

namespace glm
{
	class vec2
	{
	public:
		union {
			float x;
			float y;
		};

		vec2()
			: vec2(0.f)
		{ }

		vec2(float v)
			: vec2(v, v)
		{ }

		vec2(float x, float y)
			: x(x)
			, y(y)
		{ }

		vec2 operator-() const {
			return vec2(-x, -y);
		}

		vec2& operator= (vec2 const & v)
		{
			x = v.x;
			y = v.y;
			return *this;
		}

		vec2 & operator+= (float s)
		{
			x += s;
			y += s;
			return *this;
		}

		vec2 & operator+= (vec2 const & v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		vec2 & operator-= (float s)
		{
			x -= s;
			y -= s;
			return *this;
		}

		vec2 & operator-= (vec2 const & v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

		vec2 & operator*= (float s)
		{
			x *= s;
			y *= s;
			return *this;
		}

		vec2 & operator*= (vec2 const & v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}

		vec2 & operator/= (float s)
		{
			x /= s;
			y /= s;
			return *this;
		}

		vec2 & operator/= (vec2 const & v)
		{
			x *= v.x;
			y *= v.y;
			return *this;
		}
	};

	template <typename T>
	vec2 operator+(T const& s, vec2 const& v)
	{
		return vec2(s + v.x, s + v.y);
	}	
	
	vec2 operator+(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x + b.x, a.y + b.y);
	}

	template <typename T>
	vec2 operator-(T const& s, vec2 const& v)
	{
		return vec2(s - v.x, s - v.y);
	}

	vec2 operator-(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x - b.x, a.y - b.y);
	}

	template <typename T>
	vec2 operator*(T const& s, vec2 const& v)
	{
		return vec2(s * v.x, s * v.y);
	}

	vec2 operator*(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x * b.x, a.y * b.y);
	}

	template <typename T>
	vec2 operator/(T const& s, vec2 const& v)
	{
		return vec2(s / v.x, s / v.y);
	}

	vec2 operator/(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x / b.x, a.y / b.y);
	}
}