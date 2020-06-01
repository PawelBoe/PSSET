//
// Created by Pawel Boening on 14.09.18.
//
#include <iostream>

#include "psset.h"


typedef uint16_t EntityIndex;
typedef uint16_t EntityVersion;
typedef uint32_t EntityId;


class Entity
{
public:
    Entity() = default;

    Entity(EntityIndex index, EntityVersion version)
            : _id(EntityId(index) << 16U | EntityId(version))
    {

    }

    bool operator==(const Entity& rhs)
    {
        return id() == rhs.id();
    }

    EntityId id() const
    {
        return _id;
    }

    EntityIndex index() const
    {
        return EntityIndex(_id >> 16U);
    }

    EntityVersion version() const
    {
        return EntityVersion(_id & 0xFFFFU);
    }

private:
    EntityId _id;
};

struct EntityHash
{
    unsigned int operator()(Entity const& e) const
    {
        return e.index();
    }
};

struct UIntHash
{
    unsigned int operator()(int const& e) const
    {
        return e;
    }
};

template <typename T>
void print_dense_set(T &t)
{
    for (auto const i : t)
        std::cout << i.index() << " ";
    std::cout << std::endl;
}

template <typename T>
void print_dense_map(T &t)
{
    for (auto i : t) {
        std::cout << "(" << i.key << ", ";
        std::cout << i.value.index() << ") ";
    }
    std::cout << std::endl;
}

int main(int argc, char *argv[])
{
    Entity e0(0,0);
    Entity e1(1,0);
    Entity e2(2,0);
    Entity e3(3,0);
    Entity e4(4,0);
    Entity e5(5,0);
    Entity e6(6,0);
    Entity e7(7,0);
    Entity e8(8,0);
    Entity e9(9,0);
    Entity e10(10,0);
    Entity e15(15,0);
    Entity e16(16,0);

    psset::sparse_set<Entity, EntityHash> set_1;

    set_1.add(e2);
    set_1.add(e5);
    set_1.add(e1);
    set_1.add(e4);
    set_1.add(e10);
    set_1.resize(15);
    set_1.remove(e1);
    set_1.add(e6);
    set_1.add(e7);
    set_1.add(e8);
    set_1.add(e9);
    set_1.add(e1);
    set_1.add(e0);
    set_1.add(e3);
    set_1.add(e16);

    print_dense_set(set_1);

    psset::sparse_map<unsigned int, Entity, UIntHash> map_1;
    map_1.add(2, e2);
    map_1.add(5, e5);
    map_1.add(1, e1);
    map_1.add(4, e4);
    map_1.add(10, e10);
    map_1.resize(15);
    map_1.remove(1);
    map_1.add(6, e6);
    map_1.add(7, e7);
    map_1.add(8, e8);
    map_1.add(9, e9);
    map_1.add(1, e1);
    map_1.add(0, e0);
    map_1.add(3, e3);
    map_1.add(42, e16);

    print_dense_map(map_1);
}
