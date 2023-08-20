#include "PLICache.h"
#include "RelationalData.h"
#include <__algorithm/remove_if.h>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

PLI::PLI(const std::vector<ValueId>& data) {
    std::unordered_map<ValueId, std::vector<size_t>> positions;

    for(auto iter = data.begin(); iter != data.end(); iter++) {
        auto pos = std::distance(data.begin(), iter);
        auto value = *iter;

        if(positions.count(value)) {
            auto& tmp = positions.at(value);
            tmp.push_back(pos);
        } else {
            positions.at(value) = { static_cast<size_t>(pos) };
        }
    }

    positions.erase(std::remove_if(positions.begin(), 
        positions.end(), std::not_fn([](std::vector<ValueId> x) {
            return x.size() > 1;
    })), positions.end());

    double keyGap = 0;
    nep = 0;
    size = 0;

    nullCluster = nullptr;

    for(auto& [value, pos]: positions) {
        keyGap += pos.size() * log(pos.size());
        nep += pos.size() * (pos.size() - 1) / 2;
        size += pos.size();

        std::unordered_set<ValueId> cluster;
        cluster.insert(pos.begin(), pos.end());
        this->clusters.push_back(cluster);
        
        if(value == nullValueId) {
            nullCluster = std::addressof(*clusters.rbegin());
        }
    }

    this->entropy = log(data.size()) - keyGap / data.size();
    relationalSize = data.size();
    originRelationalSize = data.size();   
}