#ifndef TRIE_H
#define TRIE_H

#include <boost/dynamic_bitset/dynamic_bitset.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <type_traits>
#include <vector>
#include <boost/dynamic_bitset.hpp>

using BitSet = boost::dynamic_bitset<size_t>;

// template<typename ValueType>
//     concept shared_ptr = std::is_;

template<typename ValueType> // need to be std::shared_ptr
// TODO use concept
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

        Trie<ValueType>* subTrie = getSubTrie(index);
        if(subTrie == nullptr) {
            subTries.at(index - offset) = std::make_unique<Trie<ValueType>>(index + 1, dimension);
            subTrie = subTries.at(index - offset).get();
        }
        return subTrie;
    }

    void traverseEntries(BitSet& subsetKey, std::function<void(BitSet&, ValueType)> collector) {
        if(value != nullptr) {
            collector(BitSet(subsetKey), value);
        }

        for(size_t i = offset; i < dimension; i++) {
            Trie<ValueType>* subTrie = getSubTrie(i);
            if(subTrie != nullptr) {
                subsetKey.set(i);
                subTrie->traverseEntries(subsetKey, collector);
                subsetKey.reset(i);
            }
        }
    }

    bool isEmpty() {
        if(value != nullptr)
            return false;
        if(subTries.empty())
            return true;

        for(auto subTrie : this->subTries) {
            if(subTrie != nullptr)
                return false;
        }
        return true;
    }

public:
    Trie(size_t dimension)
        : offset(0), dimension(dimension) {}
    Trie(int offset, size_t dimension)
        : offset(offset), dimension(dimension) {}
    
    ValueType associate(const BitSet& key, size_t nextBit, ValueType value) {
        nextBit = key.find_next(nextBit);
        if(nextBit == BitSet::npos) {
            ValueType oldValue = this->value;
            this->value = value;
            return oldValue;
        }

        return getOrCreateSubTrie(nextBit)->associate(key, nextBit + 1, value);
    }

    ValueType get(const BitSet& key, size_t nextBit) {
        nextBit = key.find_next(nextBit);
        if(nextBit == BitSet::npos) {
            return value;
        }

        Trie<ValueType>* subTrie = getSubTrie(nextBit);
        if(subTrie == nullptr)
            return nullptr;

        return subTrie->get(key, nextBit + 1);
    }

    ValueType remove(const BitSet& key, size_t nextBit) {
        nextBit = key.find_next(nextBit);
        if(nextBit == BitSet::npos) {
            ValueType removedValue = value;
            value = nullptr;
            return removedValue;
        }

        Trie<ValueType>* subTrie = getSubTrie(nextBit);
        if(subTrie == nullptr)
            return nullptr;

        ValueType removedValue = subTrie->remove(key, nextBit + 1);
        if(subTrie->isEmpty()) {
            this->subTries[nextBit - this->offset] = nullptr;
        }

        return removedValue;
    }

    bool collectSubsetKeys(BitSet& key, size_t nextBit, BitSet& subsetKey, std::function<bool(BitSet&, ValueType)> collector) {
        if(value != nullptr && !collector(BitSet(subsetKey), value)) {
            return false;
        }

        nextBit = key.find_next(nextBit);
        while (nextBit != BitSet::npos) {
            Trie<ValueType>* subTrie = getSubTrie(nextBit);
            if(subTrie != nullptr) {
                subsetKey.set(nextBit);
                if(!collectSubsetKeys(key, nextBit + 1, subsetKey, collector))
                    return false;
                subsetKey.reset(nextBit);
            }

            nextBit = key.find_next(nextBit + 1);
        }

        return true;
    }

    bool collectSupersetKeys(BitSet& key, size_t nextBit, BitSet& superKey, std::function<bool(BitSet&, ValueType)> collector) {
        nextBit = nextBit == BitSet::npos ? nextBit : key.find_next(nextBit);

        if(nextBit == BitSet::npos) {
            if(value != nullptr && !collector(BitSet(superKey), value))
                return false;

            for(size_t i = offset; i < dimension; i++) {
                Trie<ValueType>* subTrie = getSubTrie(i);
                if(subTrie != nullptr) {
                    superKey.set(i);
                    if(!subTrie->collectSupersetKeys(key, nextBit, superKey, collector))
                        return false;
                    superKey.reset(i);
                }
            }
        } else {
            for(size_t i = offset; i < nextBit; i++) {
                Trie<ValueType>* subTrie = getSubTrie(i);
                if(subTrie != nullptr) {
                    superKey.set(i);
                    if(!subTrie->collectSupersetKeys(key, nextBit, superKey, collector))
                        return false;
                    superKey.reset(i);
                }
            }

            Trie<ValueType>* subTrie = getSubTrie(nextBit);
            if(subTrie != nullptr) {
                superKey.set(nextBit);
                if(!subTrie->collectSupersetKeys(key, nextBit + 1, superKey, collector))
                    return false;
                superKey.reset(nextBit);
            }
        }

        return true;
    }

    bool collectRestrictedSupersetKeys(BitSet& key, BitSet& blackList, size_t nextBit, BitSet& superKey, std::function<bool(BitSet&, ValueType)> collector) {
        nextBit = nextBit == BitSet::npos ? nextBit : key.find_next(nextBit);

        if(nextBit == BitSet::npos) {
            if(value != nullptr && !collector(BitSet(superKey), value))
                return false;

            for(size_t i = offset; i < dimension; i++) {
                if(blackList.test(i))
                    continue;
                Trie<ValueType>* subTrie = getSubTrie(i);
                if(subTrie != nullptr) {
                    superKey.set(i);
                    if(!subTrie->collectRestrictedSupersetKeys(key, blackList, nextBit, superKey, collector))
                        return false;
                    superKey.reset(i);
                }
            }
        } else {
            for(size_t i = offset; i < nextBit; i++) {
                if(blackList.test(i))
                    continue;
                Trie<ValueType>* subTrie = getSubTrie(i);
                if(subTrie != nullptr) {
                    superKey.set(i);
                    if(!subTrie->collectRestrictedSupersetKeys(key, blackList, nextBit, superKey, collector))
                        return false;
                    superKey.reset(i);
                }
            }

            Trie<ValueType>* subTrie = getSubTrie(nextBit);
            if(subTrie != nullptr) {
                superKey.set(nextBit);
                if(!subTrie->collectRestrictedSupersetKeys(key, blackList, nextBit + 1, superKey, collector))
                    return false;
                superKey.reset(nextBit);
            }
        }

        return true;
    }

};

#endif
