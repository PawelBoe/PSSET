//
// Created by Pawel Boening on 31.08.18.
//

#ifndef PSSET_SPARSE_MAP_H
#define PSSET_SPARSE_MAP_H


#include "sparse_set.h"

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
