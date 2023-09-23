#ifndef VERTICAL_MAP_H
#define VERTICAL_MAP_H

#include "Trie.h"
#include "Vertical.h"
#include "RelationalData.h"
#include <any>
#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

template<typename ValueType>
class VerticalMap {
private:
    const Schema& schema;
    std::unique_ptr<Trie<ValueType>> trie;
    size_t _size = 0;
 
public:
    explicit VerticalMap(const Schema& schema)
        : schema(schema) {}
        
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

    ValueType put(const std::shared_ptr<Vertical>& key, ValueType value) {
        ValueType oldValue = trie->associate(key->getColumnIndices(), 0, value);
        if(oldValue == nullptr) 
            _size++;
        return oldValue;
    }

    ValueType remove(const std::shared_ptr<Vertical>& key) {
        ValueType removedValue = trie->remove(key->getColumnIndices(), 0);
        if(removedValue != nullptr)
            _size --;
        return removedValue;
    }

    ValueType remove(const BitSet& key) {
        ValueType removedValue = trie->remove(key, 0);
        if(removedValue != nullptr)
            _size --;
        return removedValue;
    }

        std::vector<std::shared_ptr<Vertical>> getSubsetKeys(const std::shared_ptr<Vertical>& vertical) {
        std::vector<std::shared_ptr<Vertical>> subsetKeys;
        trie->collectSubsetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&subsetKeys, this](BitSet& key, ValueType value) -> bool {
                subsetKeys.push_back(schema.getVertical(key));
                return true;
            }
        );
    }

    std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> getSubsetTuples(const std::shared_ptr<Vertical>& vertical) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSubsetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this](BitSet& key, ValueType value) -> bool {
                tuples.push_back(std::make_tuple(key, value));
                return true;
            }
        );
    }

    std::optional<std::tuple<std::shared_ptr<Vertical>, ValueType>> getAnySubsetTuple(const std::shared_ptr<Vertical>& vertical) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSubsetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this, vertical](BitSet& key, ValueType value) -> bool {
                tuples.push_back(std::make_tuple(schema.getVertical(key), value));
                return false;
            }
        );
        return tuples.empty() ? std::nullopt : tuples.at(0);
    }

    std::optional<std::tuple<std::shared_ptr<Vertical>, ValueType>> getAnySubsetTuple(const std::shared_ptr<Vertical>& vertical, std::function<bool(const std::shared_ptr<Vertical>, ValueType)> condition) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSubsetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this, &condition, vertical](BitSet& key, ValueType value) -> bool {
                auto kv = schema.getVertical(key);
                if(condition(kv, value)) {
                    tuples.push_back(std::make_tuple(vertical, value));
                    return false;
                } else {
                    return true;
                }
            }
        );
        return tuples.empty() ? std::nullopt : tuples.at(0);
    }

    std::vector<std::shared_ptr<Vertical>> getSupersetTuples(const std::shared_ptr<Vertical>& vertical) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSupersetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this](BitSet& key, ValueType value) -> bool {
                tuples.push_back(std::make_tuple(schema.getVertical(key), value));
                return true;
            }
        );
        return tuples;
    }

    std::optional<std::tuple<std::shared_ptr<Vertical>, ValueType>> getAnySupersetTuple(const std::shared_ptr<Vertical>& vertical) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSupersetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this, vertical](BitSet& key, ValueType value) -> bool {
                tuples.push_back(std::make_tuple(schema.getVertical(key), value));
                return false;
            }
        );
        return tuples.empty() ? std::nullopt : tuples.at(0);
    }

    std::optional<std::tuple<std::shared_ptr<Vertical>, ValueType>> getAnySupersetTuple(const std::shared_ptr<Vertical>& vertical, std::function<bool(const std::shared_ptr<Vertical>, ValueType)> condition) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectSupersetKeys(
            vertical->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this, &condition, vertical](BitSet& key, ValueType value) -> bool {
                auto kv = schema.getVertical(key);
                if(condition(kv, value)) {
                    tuples.push_back(std::make_tuple(vertical, value));
                    return false;
                } else {
                    return true;
                }
            }
        );
        return tuples.empty() ? std::nullopt : tuples.at(0);
    }

    std::vector<std::shared_ptr<Vertical>> getRestrictedSupersetTuples(const std::shared_ptr<Vertical>& vertical, const std::shared_ptr<Vertical>& exclusion) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples;
        trie->collectRestrictedSupersetKeys(
            vertical->getColumnIndices(),
            exclusion->getColumnIndices(),
            0,
            BitSet(schema.columns.size()),
            [&tuples, this](BitSet& key, ValueType value)  {
                tuples.push_back(std::make_tuple(schema.getVertical(key), value));
            }
        );
        return tuples;
    }

    bool removeSupersetTuples(const std::shared_ptr<Vertical>& key) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples = getSupersetTuples(key);
        for(auto [key, value] : tuples) {
            remove(key);
        }

        return !tuples.empty();
    }

    bool removeSubsetTuples(const std::shared_ptr<Vertical>& key) {
        std::vector<std::tuple<std::shared_ptr<Vertical>, ValueType>> tuples = getSubsetTuples(key);
        for(auto [key, value] : tuples) {
            remove(key);
        }

        return !tuples.empty();
    }


};

#endif