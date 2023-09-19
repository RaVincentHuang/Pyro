#ifndef VERTICAL_MAP_H
#define VERTICAL_MAP_H

#include "Trie.h"
#include "Vertical.h"
#include <any>
#include <cstddef>
#include <memory>

template<typename ValueType>
class VerticalMap {
private:
    const Schema& schema;
    std::unique_ptr<Trie<ValueType>> trie;
    size_t _size = 0;

    VerticalMap(const Schema& schema)
        : schema(schema) {}
    
public:
    size_t size() { return _size; }

    bool empty() { return _size == 0; }

    bool contain(const std::shared_ptr<Vertical>& key) {
        return get(key) != nullptr;
    }

    bool contain(const BitSet& key) {
        return get(key) != nullptr;
    }

    ValueType get(const std::shared_ptr<Vertical>& key) {
        return trie->getValue(key->getColumnIndices(), 0);
    }

    ValueType get(const BitSet& key) {
        return trie->getValue(key, 0);
    }

    ValueType put(const std::shared_ptr<Vertical> key, ValueType value) {
        ValueType oldValue = trie->associate(key->getColumnIndices(), 0, value);
        
    }


};

#endif