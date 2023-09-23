#ifndef PLI_CACHE_H
#define PLI_CACHE_H

#include "RelationalData.h"
#include "Vertical.h"
#include "VerticalMap.h"
// #include "ColumnData.h"
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <functional>
#include <memory>
#include <set>
#include <utility>
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
    explicit PLI(ValueId num);
    explicit PLI(const std::vector<ValueId>& data);
    PLI(std::vector<std::set<size_t>> clusters, 
        std::vector<ValueId> probingTableCache, 
        double entropy, long nep, size_t size, size_t relationalSize, size_t originRelationalSize)
        : clusters(std::move(clusters)), probingTableCache(std::move(probingTableCache)),
            entropy(entropy), nep(nep), size(size), relationalSize(relationalSize),
            originRelationalSize(originRelationalSize) {}
    
    friend PLI operator*(PLI& lhs, PLI& rhs);

    std::vector<ValueId>& getProbTable();
    size_t getClusterSize(ValueId probingTableValueId);
    std::shared_ptr<PLI> probAll(std::shared_ptr<Vertical> probingColumns, 
        const RelationalData& relationalData);

    std::shared_ptr<PLI> intersect(std::shared_ptr<PLI> that);
    
    double calculateStripped(boost::dynamic_bitset<size_t> indices, 
        double startValue, std::function<double(double, double)> update);

    size_t getSize() const { return size; }
    // const std::vector<ValueId>& 
};


class PLICache {
    RelationalData& relationalData;
    VerticalMap<std::shared_ptr<PLI>> index;

public:
    explicit PLICache(RelationalData& relationData);
    std::shared_ptr<PLI> get(const std::shared_ptr<Vertical>& vertical);
    std::shared_ptr<PLI> getOrCreateFor(const std::shared_ptr<Vertical>& vertical);
};

struct PLIRank {
    std::shared_ptr<Vertical> vertical;
    std::shared_ptr<PLI> pli;
    int addedArity;

    PLIRank(std::shared_ptr<Vertical> vertical, std::shared_ptr<PLI> pli, int initialArity)
        : vertical(std::move(vertical)), pli(std::move(pli)), addedArity(initialArity) {}
};

#endif