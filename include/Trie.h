#ifndef TRIE_H
#define TRIE_H

#include <cstddef>
#include <memory>
#include <vector>
#include <boost/dynamic_bitset.hpp>
template<typename ValueType>
class Trie {
    int offset;
    size_t dimension;
    std::vector<std::unique_ptr<Trie<ValueType>>> subTries;
    ValueType value;

    Trie<ValueType>* getSubTrie(size_t index) {
        if(subTries.empty())
            return nullptr;
        return this->subTries[index - offset].get();
    }

    Trie<ValueType>* getOrCreateSubTrie(size_t index) {
        if(subTries.empty()) {
            subTries = std::vector<std::unique_ptr<Trie<ValueType>>>(dimension - offset, nullptr);
        }

        auto subTrie = getSubTrie(index);
        
    }

public:
    Trie(size_t dimension)
        : offset(0), dimension(dimension) {}
    Trie(int offset, size_t dimension)
        : offset(offset), dimension(dimension) {}
    
    ValueType associate(const boost::dynamic_bitset<size_t>& key, size_t nextBit, ValueType value) {
        nextBit = key.find_next(nextBit);
        if(nextBit == boost::dynamic_bitset<>::npos) {
            ValueType oldValue = this->value;
            this->value = value;
            return oldValue;
        }


    }


};

#endif
