#ifndef MYVECTOR_H
#define MYVECTOR_H
// #include <cstddef>
// #include <stdexcept>
// //#include <algorithm>
using namespace std;

template<typename T>
class MyVector {
private:
    T* data;
    size_t sz;
    size_t capacity;
public:
    MyVector() : data(nullptr), sz(0), capacity(0) {}
    
    MyVector(size_t size, const T& val_default = T()) : sz(size), capacity(size) {
        data = new T[capacity];
        for (size_t i = 0; i < sz; i++) data[i] = val_default;
    }
    
    // Copy constructor
    MyVector(const MyVector& other) : sz(other.sz), capacity(other.capacity) {
        data = new T[capacity];
        for (size_t i = 0; i < sz; i++) {
            data[i] = other.data[i];
        }
    }
    
    // Assignment operator
    MyVector& operator=(const MyVector& other) {
        if (this != &other) {
            delete[] data;
            sz = other.sz;
            capacity = other.capacity;
            data = new T[capacity];
            for (size_t i = 0; i < sz; i++) {
                data[i] = other.data[i];
            }
        }
        return *this;
    }

    ~MyVector() {
        delete[] data;
    }

    void resize(size_t newCap) {
        if (newCap == capacity) return;

        T* newData = new T[newCap];
        size_t copySize = (newCap < sz) ? newCap : sz;

        for (size_t i = 0; i < copySize; i++) {
            newData[i] = data[i];
        }
        for (size_t i = copySize; i < newCap; i++) {
            newData[i] = T();
        }

        delete[] data;
        data = newData;
        capacity = newCap;
        sz = newCap;
    }

    void reserve(size_t newCap) {
        if (newCap <= capacity) return;
        T* newData = new T[newCap];
        for (size_t i = 0; i < sz; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        capacity = newCap;
    }

    size_t size() const { return sz; }
    bool empty() const { return sz == 0; }

    void push_back(const T& value) {
        if (sz == capacity) {
            reserve(capacity == 0 ? 1 : capacity * 2);
        }
        data[sz++] = value;
    }

    T& operator[](size_t index) {
        if (index >= sz) throw out_of_range("Index out of range");
        return data[index];
    }

    const T& operator[](size_t index) const {
        if (index >= sz) throw out_of_range("Index out of range");
        return data[index];
    }

    T& back() { 
        if (sz == 0) throw out_of_range("Vector is empty");
        return data[sz - 1]; 
    }
    
    const T& back() const { 
        if (sz == 0) throw out_of_range("Vector is empty");
        return data[sz - 1]; 
    }

    T* begin() { return data; }
    T* end() { return data + sz; }
    const T* begin() const { return data; }
    const T* end() const { return data + sz; }
    
    // Thêm method erase
    void erase(T* position) {
        if (position < data || position >= data + sz) {
            throw out_of_range("Iterator out of range");
        }
        
        size_t index = position - data;
        for (size_t i = index; i < sz - 1; i++) {
            data[i] = data[i + 1];
        }
        sz--;
    }
};
#endif // MYVECTOR_H