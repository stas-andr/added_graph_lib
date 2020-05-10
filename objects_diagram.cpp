#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"
#define PI 3.14159265

const double rtog = PI / 180;

double mcos(double grad)
{
    return cos(grad*rtog);
}
double msin(double grad)
{
    return sin(grad*rtog);
}
void draw_figures(Simple_window &win, Point start, int max_cnt_side, int min_cnt_side,double side)
{
    using namespace Graph_lib;
    double correct = side/max_cnt_side;
    for(int cnt_side=max_cnt_side; cnt_side>min_cnt_side && side>0;cnt_side--){
        Polygon *poly = new Polygon;
        poly->add(Point(start.x+(max_cnt_side-cnt_side)*10,start.y+(max_cnt_side-cnt_side)*10));
        //poly->add(Point(startx,starty));
        int angle = 0;
        for(int i=0; i<cnt_side-1; ++i){
            poly->add(Point(poly->point(i).x+side*mcos(angle),poly->point(i).y+side*msin(angle)));
            angle+=(2./cnt_side*180);
        }
        win.attach(*poly);
        side-=correct;
    }
    win.wait_for_button();
}
int main()
{
    Simple_window win(Point(0,0),1100,500,"My window");
    Box window(Point(30,0),200,50,10,"Window");
    Box s_window(Point(0,100),260,50,10,"Simple_window");
    Arrow simtowin(s_window.n(),window.s());

    Box line_s(Point(260,0),200,50,10,"Line_style");
    Box color(Point(520,0),200,50,10,"Color");
    Box shape(Point(300,100),150,50,10,"Shape");
    Box point(Point(490,100),150,50,10,"Point");

    Box line(Point(0,250),120,50,10,"Line");
    Box lines(Point(140,250),130,50,10,"Lines");
    Box polygon(Point(300,250),150,50,10,"Polygon");
    Box axis(Point(480,250),120,50,10,"Axis");
    Box rectangle(Point(630,250),180,50,10,"Rectangle");
    Box text(Point(840,250),120,50,10,"Text");
    Box image(Point(990,250),120,50,10,"Image");

    Arrow linetos(line.n(),shape.sw());
    Arrow linestos(lines.n(),shape.s());
    Arrow poltos(polygon.n(),shape.s());
    Arrow axtos(axis.n(),shape.s());
    Arrow rectos(rectangle.n(),shape.s());
    Arrow ttos(text.n(),shape.se());
    Arrow itos(image.n(),shape.se());

    win.attach(window);
    win.attach(simtowin);
    win.attach(s_window);
    win.attach(line_s);
    win.attach(color);
    win.attach(shape);
    win.attach(point);
    win.attach(line);
    win.attach(lines);
    win.attach(polygon);
    win.attach(axis);
    win.attach(rectangle);
    win.attach(text);
    win.attach(image);

    win.attach(linetos);
    win.attach(linestos);
    win.attach(poltos);
    win.attach(axtos);
    win.attach(rectos);
    win.attach(ttos);
    win.attach(itos);


    win.wait_for_button();
}
