/*
NOT WORKING YET
*/
#include <iostream>
#include <new>
#include <cstddef>
#include <set>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

#define sz(c) int((c).size())

template<class T>
struct Less{bool operator()(const T& a, const T& b) {return a < b; } };


//comp - comporator pointing on which one's function for compare we should use
template<class T, class comp = Less<T>>
class priority_queue {

    //Crucial variables
    T* root;
    map<T, set<size_t> > accord;
    size_t __size;
    size_t __capacity;

    //Constants for allocation of memory(yep it sucks)
    static const size_t eps = 5;
    //static const size_t delta = 1000;

    //Get index of next or previous vertex
    inline size_t parent(size_t i) { return i / 2; }
    inline size_t left_child(size_t i) { return 2 * i; }
    inline size_t right_child(size_t i) { return 2 * i + 1; }

    void exchange(size_t first, size_t second) {
        accord[root[first]].erase(first);
        accord[root[second]].erase(second);
        accord[root[first]].insert(second);
        accord[root[second]].insert(first);
    }

    void sift_up(size_t v, comp less) {
        while (v > 1 && less(root[parent(v)], root[v])) {
            exchange(parent(v), v);
            swap(root[parent(v)], root[v]);
            v = parent(v);
        }
    }

    void sift_down(size_t v, comp less) {
        size_t temp = v;
        if (left_child(v) <= __size && !less(root[left_child(v)], root[temp]))
            temp = left_child(v);
        if (right_child(v) <= __size && !less(root[right_child(v)], root[temp]))
            temp = right_child(v);
        if (v != temp) {
            exchange(temp, v);
            swap(root[temp], root[v]);
            sift_down(temp, comp());
        }
    }

public:


    priority_queue() : root(nullptr), __size(0), __capacity(0) {}

    priority_queue(size_t N) : __size(N), root(new T[N + eps]), __capacity(N + eps) {}

    ~priority_queue() {
        for (size_t i = 1; i <= __size; ++i)
            (root + i)->~T();
        delete [] root;
        root = nullptr;
    }
    priority_queue(const priority_queue& obj) : __size(obj.size()), root(new T[obj.size() + eps]), __capacity(obj.size() + eps) {
        for (size_t i = 1; i <= __size; ++i)
            root[i] = obj.root[i];
    }

    priority_queue & operator=(const priority_queue& obj) {
        if (root != obj.root) {
            __size = obj.size();
            __capacity = __size + eps;
            T* tmp = new T[__capacity];
            for (size_t i = 1; i <= __size; ++i)
                tmp[i] = root[i], root[i]->~T();
            delete [] root;
            root = tmp;
        }
        return *this;
    }

    inline T get_root() const { return root[1]; }

    void insert(const T& obj) {
        if (__size == __capacity) {
            __capacity ^= __capacity << 1;
            T* tmp = new T[__capacity];
            for (size_t i = 1; i <= __size; ++i)
                tmp[i] = root[i], (root + i)->~T();
            delete [] root;
            root = tmp;
        }
        root[++__size] = obj;
        accord[obj].insert(__size);
        sift_up(__size, comp());
    }

    T extract_root() {
        try {
            if (!__size)
                throw;
            T res = root[1];
            exchange(1, __size);
            root[1] = root[__size--];
            sift_down(1, comp());
            return res;
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain any element" << endl;
        }
    }

    void clear() {
        try {
            if (!__size)
                throw;
            for (size_t i = 1; i <= __size; ++i)
                root[i]->~T();
            delete[] root;
            accord.clear();
            root = nullptr;
            __size = __capacity = 0;
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain any element" << endl;
        }
    }

    void remove_by_index(size_t v) {
        root[v] = get_root() + 1;
        sift_up(v, comp());
        extract_root();
    }

    void remove(const T& obj) {
        try {
            if (accord[obj].empty())
                throw;
            int t = sz(accord[obj]);
            remove_by_index(*accord[obj].begin());
            //accord[obj].erase(accord[obj].begin());
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain an element" << endl;
        }
    }

    size_t size() const { return __size; }

};

int main() {
    priority_queue<int> a;
    for(int i = 0; i < 10; i++)
        a.insert(i);
    cout << a.size() << "hui" << endl;
    cout << a.get_root() << endl;
    //for(int i = 0; i < 10; i++)
    //    cout << a.extract_root() << endl;
    a.remove(9);
    cout << a.get_root() << endl;
    return 0;
}
