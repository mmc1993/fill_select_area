#pragma once

#include "../base.h"
#include "../math/vec2.h"
#include "../math/math.h"

class Cutting {
public:
    class ClosePath {
    public:
        const std::vector<math::Points> & Cut(const math::Points & points);

    private:
        size_t NewPath(const Vec2 & point, size_t beg, size_t end);

        bool CheckCross(size_t idx,
                        const Vec2 & a,
                        const Vec2 & b,
                        size_t * crossA,
                        size_t * crossB,
                        Vec2 * crossP);

    private:
        math::Points _points;

        std::vector<math::Points> _result;
    };

    class Triangle {
    public:
        const std::vector<math::Points> & Cut(const math::Points & points);

    private:
        std::vector<math::Points> _result;
    };

    class Polygon {
    public:
        const std::vector<math::Points> & Cut(const math::Points & points);

    private:
        float CheckOrder(const math::Points & points);

        float CheckOrder(const Vec2 & p0, 
                         const Vec2 & p1, 
                         const Vec2 & p2);

        void CheckCross(const math::Points & points,
                        const Vec2 & a,
                        const Vec2 & b,
                        size_t * crossA,
                        size_t * crossB,
                        Vec2 * crossP);

        void NewPolygon(const math::Points & points,
                        const Vec2 & point,
                        size_t crossA,
                        size_t crossB,
                        size_t originA,
                        size_t priginB);

        void CutImpl(const math::Points & points);

    private:
        std::vector<math::Points> _result;

    };

public:
    void SetPoints(const math::Points & point);
    
    //  ÇÐ¸î±ÕºÏÂ·¾¶
    const std::vector<math::Points> & CutClosePaths();

    //  ÇÐ¸îÍ¹¶à±ßÐÎ
    const std::vector<math::Points> & CutPolygons();

    //  ÇÐ¸îÈý½ÇÍø
    const std::vector<math::Points> & CutTriangles();

private:
    std::vector<math::Points> _closePathResult;
    std::vector<math::Points> _triangleResult;
    std::vector<math::Points> _polygonResult;
};
