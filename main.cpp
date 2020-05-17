#include "Graph.h"
#include "Simple_window.h"
#include "Window.h"

class myIterator{
public:
    virtual double* next()=0;
    size_t size;
};
class Vector_iterator: public myIterator{
public:
    Vector_iterator(vector<double> &vector):myvector(vector){size = vector.size();}
    double * next() override;
private:
    vector<double> &myvector;
};
class List_iterator: public myIterator{
public:
    List_iterator(list<double> &list):mylist(list){size = list.size();}
    double * next() override;
private:
    list<double> mylist;
};
void print(myIterator& it)
{
    for(size_t i=0;i<it.size;i++)
        cout<<*it.next()<<endl;
}
int main()
{
    vector<double> v = {1.2,2,3,4};
    list<double> l = {1,2,3,4};
    Vector_iterator vec_it(v);
    List_iterator list_it(l);
    print(vec_it);
}



double *Vector_iterator::next()
{
    static int current = 0;
    if(myvector.empty())
        return nullptr;
    else if(current<myvector.size())
        return &myvector[current++];
}

double *List_iterator::next()
{
    static int current = 0;
    if(mylist.empty())
        return nullptr;
    else if(current<mylist.size())
    {
        auto it = mylist.begin();
        advance(it,current);
        current++;
        return &(*it);
    }
}
