#include "cutting.h"

math::Pointss Cutting::CutClosePaths(const math::Points & points)
{
    return CutClosePath::Cut(points);
}

math::Pointss Cutting::CutPolygons(const math::Pointss & pointss)
{
    return CutPolygon::Cut(pointss);
}

math::Pointss Cutting::CutPolygons(const math::Points & points)
{
    return CutPolygon::Cut(points);
}

math::Pointss Cutting::CutClosePath::Cut(math::Points points)
{
    math::Pointss result;
    while (points.size() >= 3)
    {
        Vec2 crossP;
        size_t crossA = 0;
        size_t crossB = 0;
        for (auto i = 2; i != points.size();)
        {
            auto & a = points.at(INDEX(i    , points.size()));
            auto & b = points.at(INDEX(i + 1, points.size()));
            if (CheckCross(points, i, a, b, &crossA, &crossB, &crossP))
            {
                i = NewClose(points, result, crossP, crossB, i + 1);
            }
            else { ++i; }
        }
        if (crossA == 0 && crossB == 0) { break; }
    }
    return result;
}

size_t Cutting::CutClosePath::NewClose(math::Points & points, math::Pointss & pointss, const Vec2 & point, size_t beg, size_t end)
{
    math::Points result{ point };
    std::copy(points.begin() + beg, 
              points.begin() + end, 
              std::back_inserter(result));
    points.erase(points.begin() + beg, 
                 points.begin() + end);
    points.insert(points.begin() + beg, point);
    pointss.push_back(result);
    return beg;
}

bool Cutting::CutClosePath::CheckCross(const math::Points & points, size_t idx, const Vec2 & a, const Vec2 & b, size_t * crossA, size_t * crossB, Vec2 * crossP)
{
    if (idx >= 2)
    {
        auto distance = std::numeric_limits<float>::max();
        for (auto i = 0; i != idx - 1; ++i)
        {
            auto outA = 0.0f;
            auto outB = 0.0f;
            auto & c = points.at(INDEX(i    , points.size()));
            auto & d = points.at(INDEX(i + 1, points.size()));
            if (math::IsCrossSegment(a, b, c, d, &outA, &outB) && outA < distance)
            {
                distance = outA;
                *crossP = a.Lerp(b, outA);
                *crossA = INDEX(i    , points.size());
                *crossB = INDEX(i + 1, points.size());
            }
        }
        return distance < std::numeric_limits<float>::max();
    }
    return false;
}

math::Pointss Cutting::CutPolygon::Cut(const math::Pointss & pointss)
{
    math::Pointss result;
    for (auto & points : pointss)
    {
        CutImpl(points, result);
    }
    return result;
}

math::Pointss Cutting::CutPolygon::Cut(const math::Points & points)
{
    math::Pointss result;
    CutImpl(points, result);
    return result;
}

float Cutting::CutPolygon::CheckOrder(const math::Points & points)
{
    auto beg = 0;
    for (auto i = 0; i != points.size(); ++i)
    {
        if (points.at(i).x < points.at(beg).x)
        {
            beg = i;
        }
    }

    auto & a = points.at(beg                                          );
    auto & b = points.at(INDEX(beg + 1                , points.size()));
    auto & c = points.at(INDEX(beg + points.size() - 1, points.size()));
    return (b - a).Cross(c - a) >= 0 ? 1.0f : -1.0f;
}

float Cutting::CutPolygon::CheckOrder(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2)
{
    return (p1 - p0).Cross(p2 - p1);
}

void Cutting::CutPolygon::CheckCross(const math::Points & points, const Vec2 & a, const Vec2 & b, Vec2 * crossP, size_t * crossA, size_t * crossB)
{
    auto distance = std::numeric_limits<float>::max();
    for (auto i = 0; i != points.size(); ++i)
    {
        float outA, outB;
        auto & c = points.at(INDEX(i    , points.size()));
        auto & d = points.at(INDEX(i + 1, points.size()));
        if (math::IsCrossLine(a, b, c, d, &outA, &outB))
        {
            if (outB >= 0.0f && outB <= 1.0f &&
                outA > 1.0f && outA < distance)
            {
                distance = outA;
                *crossP = a.Lerp(b, outA);
                *crossA = INDEX(i    , points.size());
                *crossB = INDEX(i + 1, points.size());
            }
        }
    }
    assert(distance != std::numeric_limits<float>::max());
}

void Cutting::CutPolygon::NewPolygon(const math::Points & points, const Vec2 & point, const size_t crossA, const size_t crossB, const size_t startA, const size_t startB, math::Pointss & pointss)
{
    math::Points points0{ point };
    for (auto i = crossB; i != startB; i = INDEX(i + 1, points.size()))
    {
        points0.push_back(points.at(i));
    }
    CutImpl(points0, pointss);

    math::Points points1{ point };
    for (auto i = startB; i != crossB; i = INDEX(i + 1, points.size()))
    {
        points1.push_back(points.at(i));
    }
    CutImpl(points1, pointss);
}

void Cutting::CutPolygon::CutImpl(const math::Points & points, math::Pointss & pointss)
{
    Vec2 crossP;
    size_t crossA = 0;
    size_t crossB = 0;
    auto polOrder = CheckOrder(points);
    for (auto i = 0; i != points.size(); ++i)
    {
        auto & a = points.at(INDEX(i, points.size()));
        auto & b = points.at(INDEX(i + 1, points.size()));
        auto & c = points.at(INDEX(i + 2, points.size()));
        auto n = std::abs(CheckOrder(a, b, c)) > 0.1f
                        ? CheckOrder(a, b, c) : 0.0f;
        if (n * polOrder < 0)
        {
            CheckCross(points, a, b, &crossP, &crossA, &crossB);
            NewPolygon(points, 
                crossP, crossA, crossB, 
                INDEX(i    , points.size()),
                INDEX(i + 1, points.size()), pointss);
            return;
        }
    }
    pointss.push_back(points);
}
