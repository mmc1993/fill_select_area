#pragma once

#include "../base.h"
#include "vec2.h"

namespace math {

using Points = std::vector<Vec2>;
using Pointss = std::vector<Points>;

static bool OnLine(const Vec2 & p, const Vec2 & a, const Vec2 & b)
{
    return p.x >= std::min(a.x, b.x)
        && p.x <= std::max(a.x, b.x)
        && p.y >= std::min(a.y, b.y)
        && p.y <= std::max(a.y, b.y)
        && (p - a).Cross(b - p) == 0.0f;
}

static bool OnLine(const Vec2& p, const Points & points)
{
    auto size = points.size();
    for (auto i = 0; i != size; ++i)
    {
        auto & a = points.at(INDEX(i    , size));
        auto & b = points.at(INDEX(i + 1, size));
        if (OnLine(p, a, b)) { return true; }
    }
    return false;
}

//  直线交点
static bool IsCrossLine(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d, float * crossA, float * crossB)
{
    assert(crossA != nullptr);
    assert(crossB != nullptr);
    auto cross = (b - a).Cross(d - c);
    if (cross != 0.0)
    {
        *crossA = (d - c).Cross(a - c) / cross;
        *crossB = (b - a).Cross(a - c) / cross;
        return true;
    }
    return false;
}

//  线段相交
static bool IsCrossSegment(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d)
{
    auto ab = b - a; 
    auto cd = d - c;
    return ab.Cross(c - a) * ab.Cross(d - a) <= 0
        && cd.Cross(a - c) * cd.Cross(b - c) <= 0;
}

//  线段交点
static bool IsCrossSegment(const Vec2 & a, const Vec2 & b, const Vec2 & c, const Vec2 & d, float * crossA, float * crossB)
{
    if (IsCrossLine(a, b, c, d, crossA, crossB))
    {
        return *crossA >= 0.0f && *crossA <= 1.0f
            && *crossB >= 0.0f && *crossB <= 1.0f;
    }
    return false;
}

template <class T>
static T Clamp(const T & val, const T & max, const T & min)
{
    if (val > max) return max;
    if (val < min) return min;
    return val;
}

}

