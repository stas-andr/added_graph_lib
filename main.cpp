#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"

int main()
{
    Simple_window win(Point(0,0),1100,500,"My window");
    Striped_polygon poly;
    poly.add(Point(100,100));
    poly.add(Point(500,200));
    poly.add(Point(400,300));
    poly.add(Point(300,500));
    win.attach(poly);
    win.wait_for_button();
}



