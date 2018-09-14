//
// Created by Pawel Boening on 31.08.18.
//

#ifndef PSSET_SPARSE_SET_H
#define PSSET_SPARSE_SET_H


#include <cmath>
#include <climits>
#include <cstring>
#include <algorithm>

namespace psset
{

    template <typename T, typename Hash>
    class sparse_set
    {
    public:
        explicit sparse_set(unsigned int cap = 0);
        ~sparse_set();

        void resize(unsigned int new_cap);
        void add(T x);
        void remove(T x);
        unsigned int search(T x) const;
        void clear();

        unsigned int size() const;
        T* data();
        const T* data() const;

        using iterator = T*;
        iterator begin();
        const iterator begin() const;
        iterator end();
        const iterator end() const;

    private:
        Hash _hash;
        unsigned int _n;
        unsigned int _capacity;
        unsigned int* _sparse;
        T* _dense;
    };

    template<typename T, typename Hash>
    sparse_set<T, Hash>::sparse_set(unsigned int cap)
    {
        _n = 0;
        _capacity = cap;

        _sparse = new unsigned int[_capacity];
        for (int i = 0; i < _capacity; i++)
        {
            _sparse[i] = UINT_MAX;
        }
        _dense = new T[_capacity];
    }

    template<typename T, typename Hash>
    sparse_set<T, Hash>::~sparse_set()
    {
        delete [] _sparse;
        delete [] _dense;
    }

    template<typename T, typename Hash>
    void sparse_set<T, Hash>::resize(unsigned int new_cap)
    {
        auto * new_sparse = new unsigned int[new_cap];
        for (int i = 0; i < new_cap; i++)
        {
            new_sparse[i] = UINT_MAX;
        }
        T* new_dense = new T[new_cap];

        unsigned int min_cap = std::min(_capacity, new_cap);
        std::copy(_sparse, _sparse + min_cap, new_sparse);
        std::copy(_dense, _dense + min_cap, new_dense);

        delete [] _sparse;
        delete [] _dense;

        if (_n > new_cap)
            _n = new_cap;
        _capacity = new_cap;
        _sparse = new_sparse;
        _dense = new_dense;
    }

    template<typename T, typename Hash>
    void sparse_set<T, Hash>::add(T x)
    {
        unsigned int val = _hash(x);

        if (val >= _capacity) {
            resize(static_cast<unsigned int>(std::pow(2, std::ceil(std::log2(val + 1)))));
        }

        if (search(x) != UINT_MAX)
            return;

        _dense[_n] = x;
        _sparse[val] = _n;
        _n++;
    }

    template<typename T, typename Hash>
    void sparse_set<T, Hash>::remove(T x)
    {
        unsigned int val = _hash(x);

        if (search(x) == UINT_MAX)
            return;

        _dense[_sparse[val]] = _dense[_n - 1];
        _sparse[_hash(_dense[_n - 1])] = _sparse[val];
        _sparse[val] = UINT_MAX;

        _n--;
    }

    template<typename T, typename Hash>
    unsigned int sparse_set<T, Hash>::search(T x) const
    {
        unsigned int val = _hash(x);

        if (val >= _capacity)
            return UINT_MAX;

        if (_sparse[val] < _n)
            return _sparse[val];

        return UINT_MAX;
    }

    template<typename T, typename Hash>
    void sparse_set<T, Hash>::clear()
    {
        _n = 0;
    }

    template<typename T, typename Hash>
    unsigned int sparse_set<T, Hash>::size() const
    {
        return _n;
    }

    template<typename T, typename Hash>
    T *sparse_set<T, Hash>::data() // not allowed to change result of hash function
    {
        return _dense;
    }

    template<typename T, typename Hash>
    const T *sparse_set<T, Hash>::data() const // not allowed to change result of hash function
    {
        return _dense;
    }

    template<typename T, typename Hash>
    typename sparse_set<T, Hash>::iterator sparse_set<T, Hash>::begin()
    {
        return &_dense[0];
    }

    template<typename T, typename Hash>
    const typename sparse_set<T, Hash>::iterator sparse_set<T, Hash>::begin() const
    {
        return &_dense[0];
    }

    template<typename T, typename Hash>
    typename sparse_set<T, Hash>::iterator sparse_set<T, Hash>::end()
    {
        return &_dense[size()];
    }

    template<typename T, typename Hash>
    const typename sparse_set<T, Hash>::iterator sparse_set<T, Hash>::end() const
    {
        return &_dense[size()];
    }

}


#endif //PSSET_SPARSE_SET_H
