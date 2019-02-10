//
// Created by Pawel Boening on 01.09.18.
//

#include "catch.hpp"

#include "sparse_set.h"
#include "sparse_map.h"
#include "sparse_factory.h"

#include <cstdint>

typedef uint32_t EntityIndex;
typedef uint8_t EntityVersion;
typedef uint32_t EntityId;

class Entity
{
public:
    struct Hash
    {
        unsigned int operator()(Entity const& e) const
        {
            return e.index();
        }
    };

    Entity()
            : _id(EntityId(0) << 8U | EntityId(0))
    {

    }

    Entity(EntityIndex index, EntityVersion version)
            : _id(EntityId(index) << 8U | EntityId(version))
    {

    }

    EntityId id() const
    {
        return _id;
    }

    EntityIndex index() const
    {
        return EntityIndex(_id >> 8U);
    }

    EntityVersion version() const
    {
        return EntityVersion(_id & 0xFFU);
    }

    bool operator==(const Entity &rhs) const
    {
        return _id == rhs._id;
    }

    bool operator!=(const Entity &rhs) const
    {
        return !(rhs == *this);
    }

private:
    EntityId _id;
};



TEST_CASE( "sparse_set creation and deletion of 1M entities", "[sparse_set]")
{
    psset::sparse_set<Entity, Entity::Hash> sset;
    int amount = 1000000;

    for (int i = 0; i < amount; ++i) {
        Entity e(static_cast<EntityIndex>(i), 0);
        sset.add(e);
    }

    REQUIRE( sset.size() == amount );

    for (int i = 0; i < amount; ++i) {
        Entity e(static_cast<EntityIndex>(i), 0);
        REQUIRE( sset.search(e) < sset.size() );
        sset.remove(e);
        REQUIRE( sset.search(e) >= sset.size() );
    }
}

TEST_CASE( "sparse_set mixed creation and deletion of 10k entities", "[sparse_set]")
{
    psset::sparse_set<Entity, Entity::Hash> sset;

    std::vector<int> entity_amounts = {3000, 7000, 100, 666, 1000, 10000};

    for (int amount : entity_amounts) {
        for (int i = 0; i < amount; ++i) {
            Entity e(static_cast<EntityIndex>(i), 0);
            sset.add(e);
        }

        REQUIRE( sset.size() == amount );

        for (int i = 0; i < amount; ++i) {
            Entity e(static_cast<EntityIndex>(i), 0);
            REQUIRE( sset.search(e) < sset.size() );
            sset.remove(e);
            REQUIRE( sset.search(e) >= sset.size() );
        }
    }
}

TEST_CASE( "sparse_map creation and deletion of 1M entities", "[sparse_map]")
{
    psset::sparse_map<Entity, int, Entity::Hash> smap;
    int amount = 1000000;

    for (int i = 0; i < amount; ++i) {
        Entity e(static_cast<EntityIndex>(i), 0);
        smap.add(e, i);
    }

    REQUIRE( smap.size() == amount );

    for (int i = 0; i < amount; ++i) {
        Entity e(static_cast<EntityIndex>(i), 0);
        REQUIRE( smap.search(e) < smap.size() );
        smap.remove(e);
        REQUIRE( smap.search(e) >= smap.size() );
    }
}

TEST_CASE( "sparse_map mixed creation and deletion of 10k entities", "[sparse_map]")
{
    psset::sparse_map<Entity, int, Entity::Hash> smap;

    std::vector<int> entity_amounts = {3000, 7000, 100, 666, 1000, 10000};

    for (int amount : entity_amounts) {
        for (int i = 0; i < amount; ++i) {
            Entity e(static_cast<EntityIndex>(i), 0);
            smap.add(e, i);
        }

        REQUIRE( smap.size() == amount );

        for (int i = 0; i < amount; ++i) {
            Entity e(static_cast<EntityIndex>(i), 0);
            REQUIRE( smap.search(e) < smap.size() );
            smap.remove(e);
            REQUIRE( smap.search(e) >= smap.size() );
        }
    }
}

TEST_CASE( "sparse_factor mixed creation and deletion of 10k entities", "[sparse_factory]")
{
    using EntityFactory = psset::sparse_factory<Entity>;
    EntityFactory sfactory;

    std::vector<int> entity_amounts = {3000, 7000, 100, 666, 1000, 10000};

    for (int amount : entity_amounts) {
        std::vector<EntityFactory::ValueId> valueIds;
        valueIds.reserve(amount);
        for (int i = 0; i < amount; ++i) {
            valueIds.push_back(sfactory.create());
        }

        REQUIRE(valueIds.size() == amount);

        for (auto id : valueIds) {
            REQUIRE(sfactory.exists(static_cast<EntityFactory::ValueId >(id)));
            sfactory.remove(static_cast<EntityFactory::ValueId >(id));
            REQUIRE(!sfactory.exists(static_cast<EntityFactory::ValueId >(id)));
        }
    }
}
