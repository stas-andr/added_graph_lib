#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"

int main()
{
    Simple_window win(Point(0,0),1200,700,"Paint tree");
    Tree *tree = new Tree(7,50);
    BinaryTree btree(tree,Point(600,100));
    win.attach(btree);
    win.wait_for_button();
}


