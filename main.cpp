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

template<class T>
struct Less{bool operator()(const T& a, const T& b) {return a < b; } };


//comp - comporator pointing on which one's function for compare we should use
template<class T, class comp = Less<T>>
class priority_queue {

    //Crucial variables
    T* root;
    map<T, set<size_t> > accord;
    size_t size;
    size_t capacity;

    //Constants for allocation of memory(yep it sucks)
    static const size_t eps = 5;
    static const size_t delta = 1000;

    //Get index of next or previous vertex
    inline size_t parent(size_t i) { return i / 2; }
    inline size_t left_child(size_t i) { return 2 * i; }
    inline size_t right_child(size_t i) { return 2 * i + 1; }


    void remove_by_index(size_t v) {
        root[v] = get_max() + 1;
        sift_up(v);
        extract_root();
    }

    void exchange(size_t first, size_t second) {
        accord[root[first]].erase(first);
        accord[root[second]].erase(second);
        accord[root[first]].insert(second);
        accord[root[second]].insert(first);
    }

    void sift_up(size_t v) {
        while (v > 1 && comp(root[parent(v)], root[v])) {
            exchange(parent(v), v);
            swap(root[parent(v)], root[v]);
        }
    }

    void sift_down(size_t v) {
        size_t temp = v;
        if (left_child(v) <= size && !comp(root[left_child(v)], root[temp]))
            temp = left_child(v);
        if (right_child(v) <= size && !comp(root[right_child(v)], root[temp]))
            temp = right_child(v);
        if (v != temp) {
            exchange(temp, v);
            swap(root[temp], root[v]);
            sift_down(temp);
        }
    }

public:


    priority_queue() : root(nullptr), size(0) {}

    priority_queue(size_t N) : size(N), root(new T[N + eps]), capacity(N + eps) {}

    ~priority_queue() {
        for (size_t i = 1; i <= size; ++i)
            root[i]->~T();
        delete [] root;
        root = nullptr;
    }
    priority_queue(const priority_queue& obj) : size(obj.size()), root(new T[obj.size() + eps]), capacity() {
        for (size_t i = 1; i <= size; ++i)
            root[i] = obj.root[i];
    }

    priority_queue & operator=(const priority_queue& obj) {
        if (root != obj.root) {
            size = obj.size();
            capacity = obj.size() + eps;
            T* tmp = new T[obj.size() + eps];
            for (size_t i = 1; i <= size; ++i)
                tmp[i] = root[i], root[i]->~T();
            delete [] root;
            root = tmp;
        }
        return *this;
    }

    inline T get_max() const { return root[1]; }

    void insert(const T& obj) {
            if (size == capacity) {
                capacity += delta;
                T* tmp = new T[capacity];
                for (size_t i = 1; i <= size; ++i)
                    tmp[i] = root[i], root[i]->~T();
                delete [] root;
                root = tmp;
            }
            root[++size] = obj;
            accord[obj].insert(size);
            sift_up(size);
    }

    T extract_root() {
        try {
            if (!size)
                throw;
            T res = root[1];
            exchange(1, size);
            root[1] = root[size--];
            sift_down(1);
            return res;
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain any element" << endl;
        }
    }

    void clear() {
        try {
            if (!size)
                throw;
            for (size_t i = 1; i <= size; ++i)
                root[i]->~T();
            delete[] root;
            accord.clear();
            root = nullptr;
            size = capacity = 0;
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain any element" << endl;
        }
    }

    void remove(const T& obj) {
        try {
            if (accord[obj].empty())
                throw;
            remove_by_index(*accord[obj].begin());
            //accord[obj].erase(accord[obj].begin());
        }
        catch (...) {
            cout << "Error:" << " priority_queue don't contain an element" << endl;
        }
    }

};

int main() {
    return 0;
}