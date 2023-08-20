#ifndef PLI_CACHE_H
#define PLI_CACHE_H

#include "RelationalData.h"
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <unordered_set>
#include <vector>
class PLI {
    std::vector<std::unordered_set<ValueId>> clusters;
    std::unordered_set<ValueId>* nullCluster;
    std::vector<ValueId> probingTableCache;

    double entropy;
    long nep;
    size_t size, relationalSize, originRelationalSize;
public:
    PLI(ValueId num);
    PLI(const std::vector<ValueId>& data);
    friend PLI operator*(const PLI& lhs, const PLI& rhs);
};


class PLICache {
    
};

#endif