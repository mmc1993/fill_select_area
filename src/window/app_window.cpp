#include "app_window.h"

AppWindow::AppWindow()
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::OnMessage(UINT uint, WPARAM wparam, LPARAM lparam)
{
	switch (uint)
	{
	case WM_CREATE:
		{
			SetTimer(GetHwnd(), 1001, 33, nullptr);
		}
		break;
    case WM_KEYUP:
        {
            switch (wparam)
            {
            case '1': _showType = ShowTypeEnum::kCLOSE_PATH; break; 
            case '2': _showType = ShowTypeEnum::kTRIANGLE; break;
            case '3': _showType = ShowTypeEnum::kPOLYGON; break;
            }
        }
        break;
	case WM_TIMER:
		{
			if (wparam == 1001) 
			{ 
                DCClear();
				Update();
                DCDraw();
            }
		}
		break;
    case WM_LBUTTONDOWN:
        {
            AddPoint({ 
                (float)GET_X_LPARAM(lparam),
                (float)GET_Y_LPARAM(lparam) }, 
                RGB(255, 0, 0));
        }
        break;
    case WM_RBUTTONDOWN:
        {
            RunCutting();
        }
        break;
    case WM_MBUTTONDOWN:
        {
            _closePaths.clear();
            _triangles.clear();
            _polygons.clear();
            _points.clear();
        }
        break;
	}
}

void AppWindow::Update()
{
    if (_closePaths.empty() && _triangles.empty() && _polygons.empty())
    {
        for (auto i = (size_t)1; i < _points.size(); ++i)
        {
            auto & a = _points.at(INDEX(i - 1, _points.size()));
            auto & b = _points.at(INDEX(i, _points.size()));
            DCLine(a.v, b.v, RGB(255, 255, 255));
        }
    }

    switch (_showType)
    {
    case ShowTypeEnum::kCLOSE_PATH:
        {
            for (auto & path : _closePaths)
            {
                DCPolygon(path.ps, path.c);
            }
        }
        break;
    case ShowTypeEnum::kTRIANGLE:
        {
            for (auto & triangle : _triangles)
            {
                auto r = ((triangle.c >> 0x00) & 0x0000ff) / 2;
                auto g = ((triangle.c >> 0x08) & 0x0000ff) / 2;
                auto b = ((triangle.c >> 0x10) & 0x0000ff) / 2;
                DCFillPolygon(triangle.ps, RGB(r, g, b));
                DCPolygon(triangle.ps, triangle.c);
            }
        }
        break;
    case ShowTypeEnum::kPOLYGON:
        {
            for (auto & polygon: _polygons)
            {
                DCFillPolygon(polygon.ps, polygon.c);
            }
        }
        break;
    }
    
    for (auto & point : _points)
    {
        DCPoint(point.v, 5, point.c);
    }
}

void AppWindow::AddPoint(const Vec2 & pt, COLORREF color)
{
    _points.push_back({ pt, color });
}

void AppWindow::RunCutting()
{
    if (_points.size() > 2)
    {
        math::Points points;

        std::transform(_points.begin(), _points.end(), std::back_inserter(points),
                       [](const Point & point) { return point.v; });

        for (auto & closePath : _cutting.CutClosePaths(points))
        {
            _closePaths.push_back({ closePath, NewColor() });

            for (auto & polygon : _cutting.CutPolygons(closePath))
            {
                _polygons.push_back({ polygon, NewColor() });
            }
        }
    }
}

COLORREF AppWindow::NewColor()
{
    auto r = rand() % 256;
    auto g = rand() % 256;
    auto b = rand() % 256;
    auto h = (r + g + b) / 3;
    if (h < 50)
    {
        r = r + (255 - r) / 2;
        g = r + (255 - g) / 2;
        b = r + (255 - b) / 2;
    }
    return RGB(r, g, b);
}
