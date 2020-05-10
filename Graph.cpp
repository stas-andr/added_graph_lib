#include "Graph.h"
#include<map>

namespace Graph_lib {

void Shape::draw_lines() const
{
	if (color().visibility() && 1<points.size())	// draw sole pixel?
		for (unsigned int i=1; i<points.size(); ++i)
			fl_line(points[i-1].x,points[i-1].y,points[i].x,points[i].y);
}

void Shape::draw() const
{
	Fl_Color oldc = fl_color();
	// there is no good portable way of retrieving the current style
	fl_color(lcolor.as_int());
	fl_line_style(ls.style(),ls.width());
	draw_lines();
	fl_color(oldc);	// reset color (to pevious) and style (to default)
	fl_line_style(0);
}


// does two lines (p1,p2) and (p3,p4) intersect?
// if se return the distance of the intersect point as distances from p1
inline pair<double,double> line_intersect(Point p1, Point p2, Point p3, Point p4, bool& parallel) 
{
    double x1 = p1.x;
    double x2 = p2.x;
	double x3 = p3.x;
	double x4 = p4.x;
	double y1 = p1.y;
	double y2 = p2.y;
	double y3 = p3.y;
	double y4 = p4.y;

	double denom = ((y4 - y3)*(x2-x1) - (x4-x3)*(y2-y1));
	if (denom == 0){
		parallel= true;
		return pair<double,double>(0,0);
	}
	parallel = false;
	return pair<double,double>( ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3))/denom,
								((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3))/denom);
}


//intersection between two line segments
//Returns true if the two segments intersect,
//in which case intersection is set to the point of intersection
bool line_segment_intersect(Point p1, Point p2, Point p3, Point p4, Point& intersection){
   bool parallel;
   pair<double,double> u = line_intersect(p1,p2,p3,p4,parallel);
   if (parallel || u.first < 0 || u.first > 1 || u.second < 0 || u.second > 1) return false;
   intersection.x = p1.x + u.first*(p2.x - p1.x);
   intersection.y = p1.y + u.first*(p2.y - p1.y);
   return true;
} 

void Polygon::add(Point p)
{
	int np = number_of_points();

	if (1<np) {	// check that thenew line isn't parallel to the previous one
		if (p==point(np-1)) error("polygon point equal to previous point");
		bool parallel;
		line_intersect(point(np-1),p,point(np-2),point(np-1),parallel);
		if (parallel)
			error("two polygon points lie in a straight line");
	}

	for (int i = 1; i<np-1; ++i) {	// check that new segment doesn't interset and old point
		Point ignore(0,0);
		if (line_segment_intersect(point(np-1),p,point(i-1),point(i),ignore))
			error("intersect in polygon");
	}
	

	Closed_polyline::add(p);
}


void Polygon::draw_lines() const
{
		if (number_of_points() < 3) error("less than 3 points in a Polygon");
		Closed_polyline::draw_lines();
}

void Open_polyline::draw_lines() const
{
		if (fill_color().visibility()) {
			fl_color(fill_color().as_int());
			fl_begin_complex_polygon();
			for(int i=0; i<number_of_points(); ++i){
				fl_vertex(point(i).x, point(i).y);
			}
			fl_end_complex_polygon();
			fl_color(color().as_int());	// reset color
		}
		
		if (color().visibility())
			Shape::draw_lines();
}


void Closed_polyline::draw_lines() const
{
	Open_polyline::draw_lines();
		
	if (color().visibility())	// draw closing line:
		fl_line(point(number_of_points()-1).x,point(number_of_points()-1).y,point(0).x,point(0).y);
}
void Shape::move(int dx, int dy)
{
	for (unsigned int i = 0; i<points.size(); ++i) {
		points[i].x+=dx;
		points[i].y+=dy;
	}
}

void Lines::draw_lines() const
{
//	if (number_of_points()%2==1) error("odd number of points in set of lines");
	if (color().visibility())
		for (int i=1; i<number_of_points(); i+=2)
			fl_line(point(i-1).x,point(i-1).y,point(i).x,point(i).y);
}

void Text::draw_lines() const
{
	int ofnt = fl_font();
	int osz = fl_size();
	fl_font(fnt.as_int(),fnt_sz);
	fl_draw(lab.c_str(), point(0).x, point(0).y);
	fl_font(ofnt,osz);
}

Function::Function(Fct f, double r1, double r2, Point xy, int count, double xscale, double yscale)
// graph f(x) for x in [r1:r2) using count line segments with (0,0) displayed at xy
// x coordinates are scaled by xscale and y coordinates scaled by yscale
{
	if (r2-r1<=0) error("bad graphing range");
	if (count<=0) error("non-positive graphing count");
	double dist = (r2-r1)/count;
	double r = r1;
	for (int i = 0; i<count; ++i) {
		add(Point(xy.x+int(r*xscale),xy.y-int(f(r)*yscale)));
		r += dist;
	}
}

void Rectangle::draw_lines() const
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
        fl_rectf(point(0).x,point(0).y,w,h);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {	// edge on top of fill
		fl_color(color().as_int());
        fl_rect(point(0).x,point(0).y,w,h);
    }
}

Point Rectangle::n() const
{
    return Point(this->point(0).x+w/2.,this->point(0).y);
}
Point Rectangle::s() const
{
    return Point(this->point(0).x+w/2.,this->point(0).y+h);
}
Point Rectangle::wc() const
{
    return Point(this->point(0).x,this->point(0).y+h/2.);
}
Point Rectangle::e() const
{
    return Point(this->point(0).x+w,this->point(0).y+h/2.);
}
Point Rectangle::center() const
{
    return Point(this->point(0).x+w/2.,this->point(0).y+h/2.);
}
Point Rectangle::nw() const
{
    return Point(this->point(0).x,this->point(0).y);
}
Point Rectangle::ne() const
{
    return Point(this->point(0).x+w,this->point(0).y);
}
Point Rectangle::se() const
{
    return Point(this->point(0).x+w,this->point(0).y+h);
}
Point Rectangle::sw() const
{
    return Point(this->point(0).x,this->point(0).y+h);
}


Axis::Axis(Orientation d, Point xy, int length, int n, string lab)
	:label(Point(0,0),lab)
{
	if (length<0) error("bad axis length");
	switch (d){
	case Axis::x:
		{	Shape::add(xy);	// axis line
			Shape::add(Point(xy.x+length,xy.y));	// axis line
			if (1<n) {
				int dist = length/n;
				int x = xy.x+dist;
				for (int i = 0; i<n; ++i) {
					notches.add(Point(x,xy.y),Point(x,xy.y-5));
				x += dist;
			}
		}
		// label under the line
		label.move(length/3,xy.y+20);
		break;
	}
	case Axis::y:
		{	Shape::add(xy);	// a y-axis goes up
			Shape::add(Point(xy.x,xy.y-length));
			if (1<n) {
			int dist = length/n;
			int y = xy.y-dist;
			for (int i = 0; i<n; ++i) {
				notches.add(Point(xy.x,y),Point(xy.x+5,y));
				y -= dist;
			}
		}
		// label at top
		label.move(xy.x-10,xy.y-length-10);
		break;
	}
	case Axis::z:
		error("z axis not implemented");
	}
}

void Axis::draw_lines() const
{
	Shape::draw_lines();	// the line
	notches.draw();	// the notches may have a different color from the line
	label.draw();	// the label may have a different color from the line
}


void Axis::set_color(Color c)
{
	Shape::set_color(c);
	notches.set_color(c);
	label.set_color(c);
}

void Axis::move(int dx, int dy)
{
	Shape::move(dx,dy);
	notches.move(dx,dy);
	label.move(dx,dy);
}

void Circle::draw_lines() const
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,r+r-1,r+r-1,0,360);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x,point(0).y,r+r,r+r,0,360);
	}
}


void Ellipse::draw_lines() const
{
	if (fill_color().visibility()) {	// fill
		fl_color(fill_color().as_int());
		fl_pie(point(0).x,point(0).y,w+w-1,h+h-1,0,360);
		fl_color(color().as_int());	// reset color
	}

	if (color().visibility()) {
		fl_color(color().as_int());
		fl_arc(point(0).x,point(0).y,w+w,h+h,0,360);
	}
}
void Arc::draw_lines() const
{
    if (color().visibility()) {
        fl_color(color().as_int());
        fl_arc(point(0).x,point(0).y,w+w,h+h,start_g,end_g);
    }
}

void draw_mark(Point xy, char c)
{
	static const int dx = 4;
	static const int dy = 4;
	string m(1,c);
	fl_draw(m.c_str(),xy.x-dx,xy.y+dy);
}

void Marked_polyline::draw_lines() const
{
	Open_polyline::draw_lines();
	for (int i=0; i<number_of_points(); ++i) 
		draw_mark(point(i),mark[i%mark.size()]);
}
/*
void Marks::draw_lines() const
{
	for (int i=0; i<number_of_points(); ++i) 
		fl_draw(mark.c_str(),point(i).x-4,point(i).y+4);
}
*/


std::map<string,Suffix::Encoding> suffix_map;

int init_suffix_map()
{
	suffix_map["jpg"] = Suffix::jpg;
	suffix_map["JPG"] = Suffix::jpg;
	suffix_map["jpeg"] = Suffix::jpg;
	suffix_map["JPEG"] = Suffix::jpg;
	suffix_map["gif"] = Suffix::gif;
	suffix_map["GIF"] = Suffix::gif;
	suffix_map["bmp"] = Suffix::bmp;
	suffix_map["BMP"] = Suffix::bmp;
	return 0;
}

Suffix::Encoding get_encoding(const string& s)
		// try to deduce type from file name using a lookup table
{
	static int x = init_suffix_map();

	string::const_iterator p = find(s.begin(),s.end(),'.');
	if (p==s.end()) return Suffix::none;	// no suffix

	string suf(p+1,s.end());
	return suffix_map[suf];
}

bool can_open(const string& s)
            // check if a file named s exists and can be opened for reading
{
	ifstream ff(s.c_str());
    return !(!ff);
}


// somewhat overelaborate constructor
// because errors related to image files can be such a pain to debug
Image::Image(Point xy, string s, Suffix::Encoding e)
	:w(0), h(0), fn(xy,"")
{
	add(xy);

	if (!can_open(s)) {
		fn.set_label("cannot open \""+s+'\"');
		p = new Bad_image(30,20);	// the "error image"
		return;
	}

	if (e == Suffix::none) e = get_encoding(s);
	
	switch(e) {
    case Suffix::jpg:
        p = new Fl_JPEG_Image(s.c_str());
        break;
    case Suffix::gif:
        p = new Fl_GIF_Image(s.c_str());
        break;

	default:	// Unsupported image encoding
		fn.set_label("unsupported file type \""+s+'\"');
		p = new Bad_image(30,20);	// the "error image"
	}
}

void Image::draw_lines() const
{
	if (fn.label()!="") fn.draw_lines();

	if (w&&h)
		p->draw(point(0).x,point(0).y,w,h,cx,cy);
	else
        p->draw(point(0).x,point(0).y);
}

void Arrow::draw_lines() const
{
    Line::draw_lines();
    int wa=5,la=15;
    int xf = this->point(1).x;
    int yf = this->point(1).y;
    double cos_alpha = this->findcos();
    double sin_alpha = this->findsin();
    Polygon poly;
    poly.add(this->point(1));
    poly.add(Point(xf-la*cos_alpha + wa*sin_alpha,yf - la*sin_alpha - wa*cos_alpha));
    poly.add(Point(xf-la*cos_alpha - wa*sin_alpha,yf - la*sin_alpha + wa*cos_alpha));
    poly.set_fill_color(this->color());
    poly.draw_lines();
}

float Myline::findcos() const
{
    Point start = this->point(0);
    Point end = this->point(1);
    float x = end.x-start.x;
    float y = end.y-start.y;
    float res = x/sqrt(x*x+y*y);
    return res;
}

float Myline::findsin() const
{
    Point start = this->point(0);
    Point end = this->point(1);
    float x = end.x-start.x;
    float y = end.y-start.y;
    float res = y/sqrt(x*x+y*y);
    return res;
}

void Box::draw_lines() const
{
    Text text_title(Point(this->point(0).x+w/5.,this->point(0).y+h*5/6.),title);
    Point p1(this->point(0).x+r,this->point(0).y+r);
    Point p2(this->point(0).x+r,this->point(0).y+h-r);
    Point p3(this->point(0).x+w-r,this->point(0).y+h-r);
    Point p4(this->point(0).x+w-r,this->point(0).y+r);

    Arc a1(p1, r, r, 90, 180);
    Arc a2(p2, r, r, 180, 270);
    Arc a3(p3, r, r, 270, 360);
    Arc a4(p4, r, r, 0,90);

    Line l1(Point(p1.x-r,p1.y),Point(p2.x-r,p2.y));
    Line l2(Point(p2.x,p2.y+r),Point(p3.x,p3.y+r));
    Line l3(Point(p3.x+r,p3.y),Point(p4.x+r,p4.y));
    Line l4(Point(p4.x,p4.y-r),Point(p1.x,p1.y-r));

    a1.draw_lines();
    a2.draw_lines();
    a3.draw_lines();
    a4.draw_lines();
    l1.draw();
    l2.draw();
    l3.draw();
    l4.draw();
    text_title.set_font_size(h*1/2.);
    text_title.draw_lines();
}
Point Box::n()
{
    return Point(this->point(0).x+w/2.,this->point(0).y);
}
Point Box::s()
{
    return Point(this->point(0).x+w/2.,this->point(0).y+h);
}
Point Box::wc()
{
    return Point(this->point(0).x,this->point(0).y+h/2.);
}
Point Box::e()
{
    return Point(this->point(0).x+w,this->point(0).y+h/2.);
}
Point Box::center()
{
    return Point(this->point(0).x+w/2.,this->point(0).y+h/2.);
}
Point Box::nw()
{
    return Point(this->point(0).x,this->point(0).y);
}
Point Box::ne()
{
    return Point(this->point(0).x+w,this->point(0).y);
}
Point Box::se()
{
    return Point(this->point(0).x+w,this->point(0).y+h);
}
Point Box::sw()
{
    return Point(this->point(0).x,this->point(0).y+h);
}

void Regular_hexagon::draw_lines() const
{
        Polygon poly;
        int angle_hex=(1-2./cnt_side)*180;
        const double PI = 3.141593;
        const double len_side = 2*radius*sin(PI/cnt_side);
        Point start_point(this->point(0).x-radius/2.,this->point(0).y-radius*cos((90-angle_hex/2.)*PI/180.));
        poly.add(start_point);
        int angle = 0;
        for(int i=0; i<cnt_side-1; ++i){
            poly.add(Point(poly.point(i).x+radius*cos(angle*PI/180.),poly.point(i).y+radius*sin(angle*PI/180.)));
            angle+=180-angle_hex;
        }
        poly.draw_lines();
}

void Right_triangle::draw_lines() const
{
    Point p1(peak.x+cathet1,peak.y);
    Point p2(peak.x,peak.y+cathet2);
    Polygon triangle;
    triangle.add(peak);
    triangle.add(p1);
    triangle.add(p2);
    triangle.draw_lines();
}

void Striped_rectangle::draw_lines() const
{
    Rectangle::draw_lines();
    Lines ls;
    int widht_lines=10;
    for(int y = this->n().y;y<this->s().y;y+=widht_lines)
        ls.add(Point(this->wc().x,y),Point(this->e().x,y));
    ls.set_style(Line_style(Line_style::solid,widht_lines));
    ls.set_fill_color(this->color());
    ls.draw_lines();
}



} // Graph
