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
                return e & 0x00000000FFFFFFFF;
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
        const iterator begin();
        const iterator end();

    private:
        ValueId _inc_version(ValueId &e) const;

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

        if (idx >= _used.size())
            return false;

        return _used.data()[idx].key == p;
    }

    template<typename Value>
    void sparse_factory<Value>::remove(sparse_factory::ValueId p)
    {
        if (exists(p))
        {
            _used.remove(p);
            _unused.push_back(_inc_version(p));
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

    template<typename Value>
    const typename sparse_factory<Value>::iterator sparse_factory<Value>::begin()
    {
        return _used.begin();
    }

    template<typename Value>
    const typename sparse_factory<Value>::iterator sparse_factory<Value>::end()
    {
        return _used.end();
    }

    template<typename Value>
    ValueId sparse_factory<Value>::_inc_version(sparse_factory::ValueId p) const
    {
        return (p & 0x00000000FFFFFFFF) | ((p + 0x0000000100000000) & 0xFFFFFFFF00000000);
    }

}


#endif //PSSET_SPARSE_FACTORY_H
