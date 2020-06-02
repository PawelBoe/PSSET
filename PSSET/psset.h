/*
MIT License

Copyright (c) 2018 Pawel Böning

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
//
// Created by Pawel Boening on 31.08.18.
//

#ifndef PSSET_SPARSE_MAP_H
#define PSSET_SPARSE_MAP_H



namespace psset
{
    template <typename K, typename  V>
    struct KeyValue
    {
        K key;
        V value;

        template <typename H>
        struct KeyHash
        {
            unsigned int operator()(KeyValue<K, V> const& e) const
            {
                return _hash(e.key);
            }

            H _hash;
        };
    };

    template <typename K, typename V>
    KeyValue<K, V> make_keyvalue(K &key, V &value)
    {
        return {key, value};
    }

    template <typename Key, typename Value, typename Hash>
    class sparse_map
    {

    public:
        explicit sparse_map(unsigned int cap = 0);

        void resize(unsigned int new_cap);
        void add(Key k, Value v);
        void remove(Key k);
        unsigned int search(Key k) const;
        Value& at(Key k);
        const Value& at(Key k) const;
        void clear();

        unsigned int size() const;
        KeyValue<Key, Value>* data();
        const KeyValue<Key, Value>* data() const;

        using iterator = KeyValue<Key, Value>*;
        iterator begin();
        const iterator begin() const;
        iterator end();
        const iterator end() const;

    private:
        sparse_set<KeyValue<Key, Value>,  typename KeyValue<Key, Value>::template KeyHash<Hash>> _sset;
    };

    template<typename Key, typename Value, typename Hash>
    sparse_map<Key, Value, Hash>::sparse_map(unsigned int cap) : _sset(cap)
    {
    }

    template<typename Key, typename Value, typename Hash>
    void sparse_map<Key, Value, Hash>::resize(unsigned int new_cap)
    {
        return _sset.resize(new_cap);
    }

    template<typename Key, typename Value, typename Hash>
    void sparse_map<Key, Value, Hash>::add(Key k, Value v)
    {
        auto p = make_keyvalue(k, v);
        return _sset.add(p);
    }

    template<typename Key, typename Value, typename Hash>
    void sparse_map<Key, Value, Hash>::remove(Key k)
    {
        Value v;
        auto p = make_keyvalue(k, v);
        return _sset.remove(p);
    }

    template<typename Key, typename Value, typename Hash>
    unsigned int sparse_map<Key, Value, Hash>::search(Key k) const
    {
        Value v;
        auto p = make_keyvalue(k, v);
        return _sset.search(p);
    }

    template<typename Key, typename Value, typename Hash>
    Value &sparse_map<Key, Value, Hash>::at(Key k)
    {
        auto idx = search(k);

        if (idx >= size())
            throw std::out_of_range("key not found in smap.");

        return _sset.data()[idx].value;
    }

    template<typename Key, typename Value, typename Hash>
    const Value &sparse_map<Key, Value, Hash>::at(Key k) const
    {
        auto idx = search(k);

        if (idx >= size())
            throw std::out_of_range("key not found in smap.");

        return _sset.data()[idx].value;
    }
    template<typename Key, typename Value, typename Hash>
    void sparse_map<Key, Value, Hash>::clear()
    {
        _sset.clear();
    }

    template<typename Key, typename Value, typename Hash>
    unsigned int sparse_map<Key, Value, Hash>::size() const
    {
        return _sset.size();
    }

    template<typename Key, typename Value, typename Hash>
    KeyValue<Key, Value> *sparse_map<Key, Value, Hash>::data() // not allowed to change result of hash function
    {
        return _sset.data();
    }

    template<typename Key, typename Value, typename Hash>
    const KeyValue<Key, Value> *sparse_map<Key, Value, Hash>::data() const // not allowed to change result of hash function
    {
        return _sset.data();
    }

    template<typename Key, typename Value, typename Hash>
    typename sparse_map<Key, Value, Hash>::iterator sparse_map<Key, Value, Hash>::begin()
    {
        return _sset.begin();
    }

    template<typename Key, typename Value, typename Hash>
    const typename sparse_map<Key, Value, Hash>::iterator sparse_map<Key, Value, Hash>::begin() const
    {
        return _sset.begin();
    }

    template<typename Key, typename Value, typename Hash>
    typename sparse_map<Key, Value, Hash>::iterator sparse_map<Key, Value, Hash>::end()
    {
        return _sset.end();
    }

    template<typename Key, typename Value, typename Hash>
    const typename sparse_map<Key, Value, Hash>::iterator sparse_map<Key, Value, Hash>::end() const
    {
        return _sset.end();
    }


}


#endif //PSSET_SPARSE_MAP_H
//
// Created by Pawel Boening on 2019-02-10.
//

#ifndef PSSET_SPARSE_FACTORY_H
#define PSSET_SPARSE_FACTORY_H


#include <vector>

namespace psset
{
    template<typename Value>
    class sparse_factory
    {
    public:
        using ValueId = unsigned int;

        struct ValueIdHash
        {
            unsigned int operator()(ValueId const &e) const
            {
                return e;
            }
        };

    public:
        ValueId create();
        const Value &at(ValueId p) const;
        Value &at(ValueId p);
        bool exists(ValueId p) const;
        void remove(ValueId p);

        using iterator = psset::KeyValue<ValueId, Value> *;
        iterator begin();
        iterator end();

    private:
        ValueId _index_counter = 0;
        psset::sparse_map<ValueId, Value, ValueIdHash> _used;
        std::vector<ValueId> _unused;
    };

    template<typename Value>
    typename sparse_factory<Value>::ValueId sparse_factory<Value>::create()
    {
        ValueId value_id;

        if (_unused.empty())
        {
            value_id = _index_counter++;
            _used.add(value_id, Value());
        } else
        {
            value_id = _unused.back();
            _unused.pop_back();
            _used.add(value_id, Value());
        }

        return value_id;
    }

    template<typename Value>
    const Value &sparse_factory<Value>::at(sparse_factory::ValueId p) const
    {
        return _used.at(p);
    }

    template<typename Value>
    Value &sparse_factory<Value>::at(sparse_factory::ValueId p)
    {
        return _used.at(p);
    }

    template<typename Value>
    bool sparse_factory<Value>::exists(sparse_factory::ValueId p) const
    {
        auto idx = _used.search(p);
        return !(idx >= _used.size());
    }

    template<typename Value>
    void sparse_factory<Value>::remove(sparse_factory::ValueId p)
    {
        if (exists(p))
        {
            _used.remove(p);
            _unused.push_back(p);
        }
    }

    template<typename Value>
    typename sparse_factory<Value>::iterator sparse_factory<Value>::begin()
    {
        return _used.begin();
    }

    template<typename Value>
    typename sparse_factory<Value>::iterator sparse_factory<Value>::end()
    {
        return _used.end();
    }

}


#endif //PSSET_SPARSE_FACTORY_H
