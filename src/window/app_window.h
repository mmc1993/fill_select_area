#pragma once

#include "window.h"
#include "../math/vec2.h"
#include "../cutting/cutting.h"

class AppWindow: public Window {
public:
    struct Point {
        Vec2 v;
        COLORREF c;
        Point(const Vec2 v, const COLORREF & c)
        {
            this->v = v;
            this->c = c;
        }
    };

    struct Points {
        math::Points ps;
        COLORREF c;
        Points(const math::Points ps, const COLORREF & c)
        {
            this->ps = ps;
            this->c = c;
        }
    };

    enum ShowTypeEnum {
        kCLOSE_PATH,
        kTRIANGLE,
        kPOLYGON,
    };

public:
	AppWindow();
	~AppWindow();

protected:
	virtual void OnMessage(UINT uint, WPARAM wparam, LPARAM lparam) override;

private:
	void Update();
    void AddPoint(const Vec2 & pt, COLORREF color);
    void RunCutting();

    COLORREF NewColor();

private:
    Cutting _cutting;

    std::vector<Points> _closePaths;
    std::vector<Points> _triangles;
    std::vector<Points> _polygons;
    std::vector<Point> _points;

    ShowTypeEnum _showType;
};