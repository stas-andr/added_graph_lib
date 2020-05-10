#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"
#include <wait.h>
#include <unistd.h>

double calc(int x)
{
    int a=150,b=100;
    return b*sqrt(1-pow(x,2)/pow(a,2));
}

int main()
{
    Simple_window win(Point(0,0),1100,500,"My window");
    Ellipse e(Point(350,300),150,100);
    int dx = 5,curx = 145;
    int x_start=350;
    int y_start=300;
    int is_positiv=1;
    Axis x(Axis::x,Point(100,300),500,25,"x");
    Axis y(Axis::y,Point(350,500),400,20,"y");
    Mark f1(e.focus1(),'X');
    Mark f2(e.focus2(),'X');
    x.label.move(400,0);
    win.attach(e);
    win.attach(x);
    win.attach(y);
    win.attach(f1);
    win.attach(f2);
    while(true){
    if(curx+dx>150){
        is_positiv=-1;
        dx=-dx;
    }
    if(curx+dx<-150){
        is_positiv=1;
        dx=-dx;
    }
    Point p1((curx+=dx)+x_start, is_positiv*calc(curx)+y_start);
    Line pf1(p1,e.focus1());
    Line pf2(p1,e.focus2());
    Mark p(p1,'*');
    win.attach(p);
    win.attach(pf1);
    win.attach(pf2);
    win.detach(p);
    win.detach(pf1);
    win.detach(pf2);
    }

}
