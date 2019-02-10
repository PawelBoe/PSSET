//
// Created by Pawel Boening on 2019-02-10.
//

#ifndef PPHY_SPARSEIDFACTORY_H
#define PPHY_SPARSEIDFACTORY_H


#include "sparse_map.h"
#include <vector>

namespace psset
{
    template<typename Value>
    class sparse_factory
    {
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


#endif //PPHY_SPARSEIDFACTORY_H
