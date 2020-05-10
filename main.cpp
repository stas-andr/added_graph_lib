#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"
#include <wait.h>
#include <unistd.h>

struct Face:Circle{
    using Circle::Circle;
    void draw_lines() const override
    {
        Circle::draw_lines();
    }
    virtual void draw_hat() const;
};
struct Smiley:Face{
    using Face::Face;
    void draw_lines() const override;
};
struct Frowny:Face{
    using Face::Face;
    void draw_lines() const override;
};
void Face::draw_hat() const
{
    Point pl(this->center().x-this->radius(),this->center().y-this->radius());
    Point pr(this->center().x+this->radius(),this->center().y-this->radius());
    Line l(pl,pr);
    Rectangle rect(Point(pl.x+this->radius()/2.,pl.y-this->radius()/2.),this->radius(),this->radius()/2);
    l.draw();
    rect.draw_lines();
}


int main()
{
    Simple_window win(Point(0,0),1100,500,"My window");
    Striped_rectangle rect(Point(100,100),400,200);
    win.attach(rect);
    win.wait_for_button();
}

void Smiley::draw_lines() const
{
    Face::draw_lines();
    Face::draw_hat();
    int rad_smile = this->radius();
    int center_x = this->point(0).x+rad_smile;
    int center_y = this->point(0).y+rad_smile;
    Circle left_eye(Point(center_x-rad_smile/2.,center_y),rad_smile/10.);
    Circle right_eye(Point(center_x+rad_smile/2.,center_y),rad_smile/10.);
    Arc smile(Point(center_x,center_y+2/3.*rad_smile),1/3.*rad_smile,1/4.*rad_smile,-180,0);
    left_eye.draw_lines();
    right_eye.draw_lines();
    smile.draw_lines();
}
void Frowny::draw_lines() const
{
    Face::draw_lines();
    int rad_smile = this->radius();
    int center_x = this->point(0).x+rad_smile;
    int center_y = this->point(0).y+rad_smile;
    Circle left_eye(Point(center_x-rad_smile/2.,center_y),rad_smile/10.);
    Circle right_eye(Point(center_x+rad_smile/2.,center_y),rad_smile/10.);
    Line mouth(Point(center_x-rad_smile/2.,center_y+2/3.*rad_smile),Point(center_x+rad_smile/2.,center_y+2/3.*rad_smile));
    Line brow_left(Point(center_x-rad_smile/2.,center_y-rad_smile/2.),Point(center_x-rad_smile/4.,center_y));
    Line brow_right(Point(center_x+rad_smile/2.,center_y-rad_smile/2.),Point(center_x+rad_smile/4.,center_y));
    left_eye.draw_lines();
    right_eye.draw_lines();
    brow_left.draw();
    brow_right.draw();
    mouth.draw();
}


