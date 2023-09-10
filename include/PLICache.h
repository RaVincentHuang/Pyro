#ifndef PLI_CACHE_H
#define PLI_CACHE_H

#include "RelationalData.h"
#include "Vertical.h"
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <set>
#include <vector>
class PLI {
    std::vector<std::set<size_t>> clusters;
    // Optimize We use the set, which is rbtree impl the cluster, while the original code use the vector.
    // replace to vec could reduce the time of the build of the PLI.
    
    // TODO Add the null != null situation. 
    // std::set<size_t>* nullCluster;
    std::vector<ValueId> probingTableCache;

    double entropy;
    long nep;
    size_t size, relationalSize, originRelationalSize;

    bool takeProbe(size_t position, const RelationalData& relationalData, 
        std::shared_ptr<Vertical> probingColumns, std::vector<ValueId>& probe);

public:
    PLI(ValueId num);
    PLI(const std::vector<ValueId>& data);
    PLI(std::vector<std::set<size_t>> clusters, 
        std::vector<ValueId> probingTableCache, 
        double entropy, long nep, size_t size, size_t relationalSize, size_t originRelationalSize)
        : clusters(clusters), probingTableCache(probingTableCache),
            entropy(entropy), nep(nep), size(size), relationalSize(relationalSize),
            originRelationalSize(originRelationalSize) {}
    
    friend PLI operator*(PLI& lhs, PLI& rhs);

    std::vector<ValueId>& getProbTable();
    size_t getClusterSize(ValueId probingTableValueId);
    PLI probAll(std::shared_ptr<Vertical> probingColumns, 
        const RelationalData& relationalData);
    
    double calculateStripped(boost::dynamic_bitset<size_t> indices, 
        double startValue, std::function<double(double, double)> update);
    // const std::vector<ValueId>& 
};


class PLICache {
    RelationalData& relationalData;
    


};

#endif