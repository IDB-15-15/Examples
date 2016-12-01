#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <list>
using namespace std;
template <class T>
class my_allocator
{
    
public:
    typedef size_t    size_type;
    typedef T*        pointer;
    typedef const T*  const_pointer;
    typedef T&        reference;
    typedef const T&  const_reference;
    typedef T         value_type;
    my_allocator() {}
    my_allocator(const my_allocator&) {}
    T* allocate(size_t n, const void * = 0)
    {
        
        T* t = (T*) malloc(n * sizeof(T));
        std::cout<< " used  my_allocator to allocate   at address "<< t << std::endl;
        return t;
    }
    void deallocate(void* p, size_t)
    {
        
        std::cout << "  used my_allocator to deallocate at address "<< p << std::endl;
        free(p);
    }
    
    T* address(T& x) const
    {
        return &x;
    }
    void  construct(T* p, const T& val)
    {
        cout<<"construct "<<p<<endl;
        new (p) T(val);
    }
    void destroy(T* p)
    {
        cout<<"destruct "<<p<<endl;
        p->~T();
    }
    size_t max_size() const
    {
        return std::numeric_limits<size_t>::max();
    }
};
int main()
{
    // использование своего аллокатора
    my_allocator<int> alloc;
    int* a=alloc.allocate(100);
    alloc.construct(a,100);
    alloc.destroy(a);
    alloc.deallocate(a,100);
    //использование своего аллокатора в std::vector
    vector<int,my_allocator<int> > v;
    v.push_back(42);
    v.push_back(56);
    v.push_back(11);
    v.push_back(22);
    v.push_back(33);
    v.push_back(44);
    cout<<endl<<endl;
    for (int i=0;i<v.size();i++)
    {
        cout<<v[i]<<' ';
    }
    cout<<endl;
    cout<<"size v1= "<<v.size()<<endl;
    cout<<endl<<endl;
    vector<string,my_allocator<string> > v2;
    v2.push_back("pen");
    v2.push_back("pineapple");
    v2.push_back("apple");
    v2.push_back("pen ");
    cout<<endl<<endl;
    for (int i=0;i<v2.size();i++)
    {
        cout<<v2[i]<<' ';
    }
    cout<<endl<<endl;
    cout<<"size v2= "<<v2.size()<<endl;
    cout<<endl<<endl;
    return 0;
}

