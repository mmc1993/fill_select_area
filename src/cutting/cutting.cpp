#include "cutting.h"

const std::vector<math::Points> & Cutting::ClosePath::Cut(const math::Points & points)
{
    _result.clear();
    _points = points;
    for (auto i = 0; i != _points.size(); )
    {
        Vec2 crossP;
        size_t crossA, crossB;
        auto & a = _points.at(INDEX(i    , _points.size()));
        auto & b = _points.at(INDEX(i + 1, _points.size()));
        if (CheckCross(i, a, b, &crossA, &crossB, &crossP))
        {
            i = NewPath(crossP, INDEX(crossA + 1, _points.size()), i + 1);
        }
        else { ++i; }
    }
    return _result;
}

size_t Cutting::ClosePath::NewPath(const Vec2 & point, size_t beg, size_t end)
{
    math::Points path{ point };
    std::copy(std::next(_points.begin(), beg),
              std::next(_points.begin(), end),
              std::back_inserter(path));
    _points.erase(std::next(_points.begin(), beg),
                  std::next(_points.begin(), end));
    _points.insert(std::next(_points.begin(), beg), point);
    _result.push_back(std::move(path));
    return beg;
}

bool Cutting::ClosePath::CheckCross(size_t idx, const Vec2 & a, const Vec2 & b, size_t * crossA, size_t * crossB, Vec2 * crossP)
{
    auto isCross = false;

    if (idx >= 2)
    {
        idx -= 1;
        float outA = 0;
        float outB = 0;
        float distance = std::numeric_limits<float>::max();
        for (auto i = 0; i != idx; ++i)
        {
            auto & c = _points.at(INDEX(i    , _points.size()));
            auto & d = _points.at(INDEX(i + 1, _points.size()));
            if (math::IsCrossSegment(a, b, c, d, &outA, &outB))
            {
                if (outA < distance)
                {
                    isCross = true;
                    distance = outA;
                    *crossP = a.Lerp(b, outA);
                    *crossA = INDEX(i    , _points.size());
                    *crossB = INDEX(i + 1, _points.size());
                }
            }
        }
    }
    return isCross;
}



void Cutting::SetPoints(const math::Points & points)
{
    _closePathResult.clear();
    _triangleResult.clear();
    _polygonResult.clear();
    ClosePath closePath;
    Triangle triangle;
    Polygon polygon;

    _closePathResult = closePath.Cut(points);
    for (auto & path : _closePathResult)
    {
        auto result = polygon.Cut(path);
        std::copy(result.begin(), result.end(), 
           std::back_inserter(_polygonResult));
    }

    for (auto & polygon : _polygonResult)
    {
        auto result = triangle.Cut(polygon);
        std::copy(result.begin(), result.end(),
          std::back_inserter(_triangleResult));
    }
}

const std::vector<math::Points> & Cutting::CutClosePaths()
{
    return _closePathResult;
}

const std::vector<math::Points> & Cutting::CutTriangles()
{
    return _triangleResult;
}

const std::vector<math::Points> & Cutting::CutPolygons()
{
    return _polygonResult;
}

const std::vector<math::Points> & Cutting::Polygon::Cut(const math::Points & points)
{
    _result.clear();
    CutImpl(points);
    return _result;
}

float Cutting::Polygon::CheckOrder(const math::Points & points)
{
    auto order0 = 0;    //  Ë³Ê±Õë
    auto order1 = 0;    //  ÄæÊ±Õë
    for (auto i = 0; i != points.size(); ++i)
    {
        auto & a = points.at(INDEX(i    , points.size()));
        auto & b = points.at(INDEX(i + 1, points.size()));
        auto & c = points.at(INDEX(i + 2, points.size()));
        auto order = CheckOrder(a, b, c);
        if (order >= 0.0f ||std::abs(order) < 0.1f)
        {
            ++order0;
        }
        else
        {
            ++order1;
        }
    }
    return order0 > order1 ? 1.0f : -1.0f;
}

float Cutting::Polygon::CheckOrder(const Vec2 & p0, const Vec2 & p1, const Vec2 & p2)
{
    return (p1 - p0).Cross(p2 - p1);
}

void Cutting::Polygon::CheckCross(const math::Points & points, const Vec2 & a, const Vec2 & b, size_t * crossA, size_t * crossB, Vec2 * crossP)
{
    float outA, outB;
    auto distance = std::numeric_limits<float>::max();
    for (auto i = 0; i != points.size(); ++i)
    {
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

void Cutting::Polygon::NewPolygon(const math::Points & points, 
                                  const Vec2 & point, 
                                  size_t crossA, 
                                  size_t crossB, 
                                  size_t originA,
                                  size_t originB)
{
    math::Points points0{ point };
    math::Points points1{ point };
    for (auto i = crossB; i != originB; i = INDEX(i + 1, points.size()))
    {
        points0.push_back(points.at(i));
    }
    CutImpl(points0);

    for (auto i = originA; i != crossB; i = INDEX(i + 1, points.size()))
    {
        points1.push_back(points.at(i));
    }
    CutImpl(points1);
}

void Cutting::Polygon::CutImpl(const math::Points & points)
{
    Vec2 crossP;
    size_t crossA = 0;
    size_t crossB = 0;
    auto order = CheckOrder(points);
    for (auto i = 2; i != points.size(); ++i)
    {
        auto & a = points.at(INDEX(i - 1, points.size()));
        auto & b = points.at(INDEX(i    , points.size()));
        auto & c = points.at(INDEX(i + 1, points.size()));
        auto n = std::abs(CheckOrder(a, b, c)) > 0.1f
                        ? CheckOrder(a, b, c) : 0.0f;
        if (n * order < 0)
        {
            CheckCross(points, a, b, &crossA, &crossB, &crossP);
            NewPolygon(points, crossP, 
                       crossA, crossB, 
                       INDEX(i    , points.size()),
                       INDEX(i + 1, points.size()));
            return;
        }
    }
    _result.push_back(points);
}

const std::vector<math::Points>& Cutting::Triangle::Cut(const math::Points & points)
{
    Vec2 origin;
    for (auto i = 0; i != points.size(); ++i)
    {
        origin += points.at(i);
    }
    origin.x = origin.x / points.size();
    origin.y = origin.y / points.size();

    _result.clear();
    for (auto i = 0; i != points.size(); ++i)
    {
        auto & a = points.at(INDEX(i    , points.size()));
        auto & b = points.at(INDEX(i + 1, points.size()));
        _result.push_back({ origin, a, b });
    }
    return _result;
}
