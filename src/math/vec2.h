#pragma once

#include <cmath>

class Vec2 {
public:
	float x, y;

	Vec2(float _x = 0, float _y = 0)
	{
        x = _x; y = _y;
    }

    bool operator ==(const Vec2 & v) const
    {
        return std::abs(x - v.x) <= FLT_EPSILON
            && std::abs(y - v.y) <= FLT_EPSILON;
    }

    bool operator !=(const Vec2 & v) const
    {
        return !(*this == v);
    }

    Vec2 & operator +=(const Vec2 & v)
    {
        return (*this = *this + v);
    }

    Vec2 & operator -=(const Vec2 & v)
    {
        return (*this = *this - v); 
    }

	Vec2 operator +(const Vec2 & v) const
	{
		return { x + v.x, y + v.y };
	}

	Vec2 operator -(const Vec2 & v) const
	{
		return { x - v.x, y - v.y };
	}

	Vec2 operator *(const float v) const
	{
		return { x * v, y * v };
	}

    Vec2 operator *(const Vec2 & v) const
    {
        return { x * v.x, y * v.y };
    }

	Vec2 operator /(const float v) const
	{
		return { x / v, y / v };
	}

	float Dot(const Vec2 & v) const
	{
		return x * v.x + y * v.y;
	}

	float Cross(const Vec2 & v) const
	{
		return x * v.y - y * v.x;
	}

	float LengthSqrt() const
	{
		return Dot(*this);
	}

	float Length() const
	{
		return std::sqrt(LengthSqrt());
	}

	const Vec2 & Normal()
	{
		return (*this = GetNormal());
	}

	Vec2 GetNormal() const
	{
		Vec2 ret;
		auto len = Length();
		if (len != 0)
		{
			auto v = 1.0f / len;
			ret.x = x * v;
			ret.y = y * v;
		}
		return ret;
	}

    Vec2 Lerp(const Vec2 & v, float f) const
    {
        return *this + (v - *this) * f;
    }

    float Unlerp(const Vec2 & v, const Vec2 & m) const
    {
        return (m - *this).Length() / (v - *this).Length();
    }
};
