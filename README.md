# PSSET - Pawel's sparse set

This repository contains an implementation of a sparse set
and a sparse map that was inspired by
[this](https://programmingpraxis.com/2012/03/09/sparse-sets/)
article. The two data structures allow for the following
time complexities:

| Operation   |      Complexity      |
|----------|:-------------:|
| Insert Element |  O(1) |
| Delete Element |    O(1)   |
| Search Element | O(1) |
| Clear Container | O(1) |
| Resize Container | O(n) |

Regarding its space complexity, it is always O(MAX), where
MAX is the biggest possible key in the current allocation.
The internal size is managed dynamically, meaning that once
the capacity is exhausted the internal size is doubled and
all content is moved over to the new memory block.

## Installation
Just clone the repository and put the `\PSSET` folder wherever
you see fit. Include `sset.h` or `smap.h` and you can start!
It is best that you integrate the library in your own build
system.

## Example
### Sparse Set
```
struct UIntHash
{
    unsigned int operator()(int const& e) const
    {
        return e;
    }
};

psset::sparse_set<int, UIntHash> sset;

// sset.add(VALUE);
sset.add(42);
sset.remove(42);

[...]
```

### Sparse Map
```
struct UIntHash
{
    unsigned int operator()(int const& e) const
    {
        return e;
    }
};

psset::sparse_map<unsigned int, int, UIntHash> smap;

// smap.add(KEY, VALUE);
smap.add(1, 42);
smap.add(2, 13);
smap.remove(1);
smap.remove(2);

[...]
```
