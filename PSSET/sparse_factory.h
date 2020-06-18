//
// Created by Pawel Boening on 2019-02-10.
//

#ifndef PSSET_SPARSE_FACTORY_H
#define PSSET_SPARSE_FACTORY_H


#include "sparse_map.h"
#include <vector>

namespace psset
{
    template<typename Value>
    class sparse_factory
    {
    public:
        using ValueId = unsigned long;

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
    };

    template<typename Value>
    typename sparse_factory<Value>::ValueId sparse_factory<Value>::create()
    {
        ValueId value_id = _index_counter++;

        _used.add(value_id, Value());

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
        _used.remove(p);
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
