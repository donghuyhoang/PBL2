#ifndef HASHTABLE_H
#define HASHTABLE_H
#include <algorithm>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "MyVector.h"

using namespace std;

template<typename T>
class HashEntry {
public:
    string key;
    T value;
    HashEntry* next;
    HashEntry(const string& k, const T& v) : key(k), value(v), next(nullptr) {}
};

template<typename T>
class HashTable {
private:
    HashEntry<T>** table;
    int capacity;
    int size;

    int hashFunction(const string& key) {
        int hash = 0;
        for (char c : key) hash = (hash * 31 + c) % capacity;
        return hash;
    }

public:
    HashTable(int cap = 100) : capacity(cap), size(0) {
        table = new HashEntry<T>*[capacity]();
    }

    ~HashTable() {
        clear();
        delete[] table;
    }

    bool insert(const string& key, const T& value) {
        int index = hashFunction(key);
        HashEntry<T>* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) {
                entry->value = value;
                return true;
            }
            entry = entry->next;
        }
        HashEntry<T>* newEntry = new HashEntry<T>(key, value);
        newEntry->next = table[index];
        table[index] = newEntry;
        size++;
        return true;
    }

    T* searchByString(const string& key) {
        int index = hashFunction(key);
        HashEntry<T>* entry = table[index];
        while (entry != nullptr) {
            if (entry->key == key) return &(entry->value);
            entry = entry->next;
        }
        return nullptr;
    }

    bool remove(const string& key) {
        int index = hashFunction(key);
        HashEntry<T>* entry = table[index];
        HashEntry<T>* prev = nullptr;
        while (entry != nullptr) {
            if (entry->key == key) {
                if (prev == nullptr) table[index] = entry->next;
                else prev->next = entry->next;
                delete entry;
                size--;
                return true;
            }
            prev = entry;
            entry = entry->next;
        }
        return false;
    }

    MyVector<T> getAllElements() {
        MyVector<T> result;
        for (int i = 0; i < capacity; i++) {
            HashEntry<T>* entry = table[i];
            while (entry != nullptr) {
                result.push_back(entry->value);
                entry = entry->next;
            }
        }
        // Sắp xếp an toàn (tránh crash nếu key không có số)
        sort(result.begin(), result.end(), [](const T& a, const T& b) {
            try {
                string k1 = a.getKey(); string k2 = b.getKey();
                if (k1.length() > 2 && k2.length() > 2 && isdigit(k1[2]) && isdigit(k2[2])) {
                    return stoi(k1.substr(2)) < stoi(k2.substr(2));
                }
                return k1 < k2;
            } catch (...) { return false; }
        });
        return result;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }
    bool contains(const string& key) { return searchByString(key) != nullptr; }

    void clear() {
        for (int i = 0; i < capacity; i++) {
            HashEntry<T>* entry = table[i];
            while (entry != nullptr) {
                HashEntry<T>* temp = entry;
                entry = entry->next;
                delete temp;
            }
            table[i] = nullptr;
        }
        size = 0;
    }
};
#endif