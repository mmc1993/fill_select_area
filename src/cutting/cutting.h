#pragma once

#include "../base.h"
#include "../math/vec2.h"
#include "../math/math.h"

class Cutting {
public:
    //  拆分闭合路径
    math::Pointss CutClosePaths(const math::Points & points);
    //  拆分凹多边形
    math::Pointss CutPolygons(const math::Pointss & pointss);
    //  拆分凹多边形
    math::Pointss CutPolygons(const math::Points & points);

private:
    //  拆分闭合路径
    struct CutClosePath {
        static math::Pointss Cut(math::Points points);

        static size_t NewClose(
            math::Points & points,
            math::Pointss & pointss,
            const Vec2 & point, size_t beg, size_t end);

        static bool CheckCross(
            const math::Points & points,
            size_t idx, const Vec2 & a, const Vec2 & b, 
            size_t * crossA, size_t * crossB, Vec2 * crossP);
    };

    //  拆分凹多边形
    struct CutPolygon {
        static math::Pointss Cut(const math::Pointss & pointss);
        static math::Pointss Cut(const math::Points & points);

        static float CheckOrder(
            const math::Points & points);

        static float CheckOrder(
            const Vec2 & p0, 
            const Vec2 & p1, 
            const Vec2 & p2);
        
        static bool CheckCross(
            const math::Points & points, 
            const Vec2 & a, 
            const Vec2 & b, 
            Vec2   * crossP,
            size_t * crossA, 
            size_t * crossB);

        static void NewPolygon(
            const math::Points & points, 
            const Vec2 & point,
            const size_t crossA,
            const size_t crossB,
            const size_t startA,
            const size_t startB,
            math::Pointss & pointss);

        static void CutImpl(const math::Points & points, math::Pointss & pointss);
    };
};
