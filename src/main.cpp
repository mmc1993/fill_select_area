
#include "window/app_window.h"

int main()
{
    AppWindow app;
    app.Create("xxx");
    app.Size(1024, 800);
    app.Move(100, 100);
    app.Loop();
    return 0;
}