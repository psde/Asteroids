#pragma once

namespace Math
{
	class vec2
	{
	public:
		float x;
		float y;

		vec2()
			: vec2(0.f)
		{ }

		template<class T>
		vec2(T v)
			: vec2(static_cast<float>(v), static_cast<float>(v))
		{ }

		template<class T1, class T2>
		vec2(T1 x, T2 y)
			: x(static_cast<float>(x))
			, y(static_cast<float>(y))
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
	
	inline vec2 operator+(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x + b.x, a.y + b.y);
	}

	template <typename T>
	vec2 operator-(T const& s, vec2 const& v)
	{
		return vec2(s - v.x, s - v.y);
	}

	inline vec2 operator-(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x - b.x, a.y - b.y);
	}

	template <typename T>
	vec2 operator*(T const& s, vec2 const& v)
	{
		return vec2(s * v.x, s * v.y);
	}

	inline vec2 operator*(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x * b.x, a.y * b.y);
	}

	template <typename T>
	vec2 operator/(T const& s, vec2 const& v)
	{
		return vec2(s / v.x, s / v.y);
	}

	inline vec2 operator/(vec2 const& a, vec2 const& b)
	{
		return vec2(a.x / b.x, a.y / b.y);
	}
}