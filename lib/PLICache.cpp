#include "PLICache.h"
#include "Common.h"
#include "RelationalData.h"
#include "ColumnData.h"
#include "Trie.h"
#include "Vertical.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <set>
#include <vector>
#include <random>

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

    double keyGap = 0;
    nep = 0;
    size = 0;

    // TODO Add the null != null situation. 
    // nullCluster = nullptr;

    for(auto& [value, pos]: positions) {
        if(pos.size() > 1) {
            keyGap += pos.size() * log(pos.size());
            nep += pos.size() * (pos.size() - 1) / 2;
            size += pos.size();
            std::set<size_t> cluster;
            cluster.insert(pos.begin(), pos.end());
            this->clusters.push_back(cluster);
        }

        // TODO Add the null != null situation. 
        // if(value == nullValueId) {
        //     nullCluster = std::addressof(*clusters.rbegin());
        // }
    }

    std::sort(clusters.begin(), clusters.end(), [](std::set<size_t> x, std::set<size_t> y) -> bool {
        return (*x.begin()) < (*y.begin());
    });

    this->entropy = log(data.size()) - keyGap / data.size();
    relationalSize = data.size();
    originRelationalSize = data.size();   
}

std::vector<ValueId>& PLI::getProbTable() {
    // Optimize  If need to cache the PLI prob table, uncomment it!
    // if(!probingTableCache.empty())
    //     return probingTableCache;
    probingTableCache = std::vector<ValueId>(originRelationalSize);

    ValueId nextId = singletonValueId;
    probingTableCache[singletonValueId] = 0;
    for(auto& cluster : this->clusters) {
        auto valueId = nextId++;
        for(auto& pos : cluster) {
            probingTableCache[pos] = valueId;
        }
    }
    return probingTableCache;
}

PLI operator*(PLI& lhs, PLI& rhs) {
    if(lhs.size > rhs.size)
        return rhs * lhs;
    
    std::vector<std::set<size_t>> newClusters;
    size_t newSize = 0;
    double newKeyGap = 0;
    long newNep = 0;


    auto& rhsProbingTable = rhs.getProbTable();
    std::unordered_map<ValueId, std::set<size_t>> partialIndex;

    for(auto& cluster: lhs.clusters) {
        for(auto position : cluster) { 
            auto rhsProbTableValueId = rhsProbingTable[position];
            if(rhsProbTableValueId == singletonValueId)
                continue;

            if(partialIndex.count(rhsProbTableValueId)) {
                auto& newCluster = partialIndex.at(rhsProbTableValueId);
                newCluster.insert(position);
            } else {
                partialIndex[rhsProbTableValueId] = {position};
            }
        }

        for(auto& [valueId, cluster]: partialIndex) {
            if(cluster.size() == 1)
                continue;

            newSize += cluster.size();
            newKeyGap += cluster.size() * log(cluster.size());
            newNep = cluster.size() * (cluster.size() - 1) / 2;
            newClusters.push_back(cluster);
        }
        partialIndex.clear();
    }
    double newEntropy = log(lhs.relationalSize) - newKeyGap / lhs.relationalSize;

    std::sort(newClusters.begin(), newClusters.end(), [](std::set<size_t> x, std::set<size_t> y) -> bool {
        return (*x.begin()) < (*y.begin());
    });

    return {newClusters, {}, newEntropy, 
        newNep, newSize, lhs.relationalSize, lhs.relationalSize};
}

std::shared_ptr<PLI> PLI::intersect(std::shared_ptr<PLI> that) {
    auto ans = (*this) * (*that);
    return std::make_shared<PLI>(ans);
}

size_t PLI::getClusterSize(ValueId probingTableValueId) {
    switch (probingTableValueId) {
        case singletonValueId:
            return 1;
        // TODO impl the null != null 
        default: // DEBUG panic could occur here! they took a bold risk.
            return clusters.at(probingTableValueId - 1).size();
    }
}

std::shared_ptr<PLI> PLI::probAll(std::shared_ptr<Vertical> probingColumns, 
        const RelationalData& relationalData) {
    
    std::vector<std::set<size_t>> newClusters;
    size_t newSize = 0;
    double newKeyGap = 0;
    long newNep = 0;

    std::unordered_map<std::vector<ValueId>, std::set<size_t>, 
        boost::hash<std::vector<ValueId>>> partialIndex;
    
    std::vector<ValueId> probe;

    for(auto cluster : this->clusters) {
        for(auto position: cluster) {
            if(!takeProbe(position, relationalData, probingColumns, probe)) {
                probe.clear();
                continue;
            }

            if(partialIndex.count(probe)) {
                auto& newCluster = partialIndex.at(probe);
                newCluster.insert(position);
            } else {
                partialIndex[probe] = {position};
            }
            probe.clear();
        }

        for(auto [probe, newCluster] : partialIndex) {
            if(newCluster.size() == 1)
                continue;

            newSize += newCluster.size();
            newKeyGap += newCluster.size() * log(newCluster.size());
            newNep = newCluster.size() * (newCluster.size() - 1) / 2;
            newClusters.push_back(newCluster);
        }

        partialIndex.clear();
    }

    double newEntropy = log(this->relationalSize) - newKeyGap / this->relationalSize;

    std::sort(newClusters.begin(), newClusters.end(), [](std::set<size_t> x, std::set<size_t> y) -> bool {
        return (*x.begin()) < (*y.begin());
    });

    return std::make_shared<PLI>(newClusters, std::vector<ValueId>({}), newEntropy, 
        newNep, newSize, this->relationalSize, this->relationalSize);

}

bool PLI::takeProbe(size_t position, const RelationalData& relationalData, 
        std::shared_ptr<Vertical> probingColumns, std::vector<ValueId>& probe) {
    auto probingIndices = probingColumns->getColumnIndices();
    for(auto index = probingIndices.find_first();
            index != boost::dynamic_bitset<size_t>::npos;
            index = probingIndices.find_next(index + 1)) {
        auto value = relationalData.columns.at(index).probingTable.at(position);
        if(value == singletonValueId)
            return false;
        probe.push_back(value);
    }
    return true;
}

double PLI::calculateStripped(boost::dynamic_bitset<size_t> indices, 
        double startValue, std::function<double(double, double)> update) {
    double accu = startValue;
    for(auto cluster : this->clusters) {
        boost::dynamic_bitset<size_t> clusterIndices;
        for(auto item : cluster) {
            clusterIndices.set(item);
        }
        auto intersectionSize = (clusterIndices & indices).size();
        auto newClusterSize = cluster.size() - intersectionSize;
        if(newClusterSize > 0)
            accu = update(accu, newClusterSize);
    }
    return accu;
}

PLICache::PLICache(RelationalData& relationData)
    :  relationalData(relationData), index(VerticalMap<std::shared_ptr<PLI>>(relationData.schema)) {
    for(const auto& column : relationData.schema.columns) {
        auto pli = relationData.columns.at(column->index).pli;
        index.put(column, pli);
    }
}   

std::shared_ptr<PLI> PLICache::get(const std::shared_ptr<Vertical>& vertical) {
    return index.get(vertical);
}

std::shared_ptr<PLI> PLICache::getOrCreateFor(const std::shared_ptr<Vertical>& vertical) {
    auto pli = get(vertical);
    if(pli != nullptr)
        return pli;

    auto subTuples = index.getSubsetTuples(vertical);

    std::list<PLIRank> ranks;

    for(auto [subVertical, subPLI] : subTuples) {
        PLIRank pliRank = {subVertical, subPLI, subVertical->getArity()};
        ranks.push_back(pliRank);
    }

    auto& smallestPliRank = *ranks.begin();
    for(auto& rank : ranks) {
        if(smallestPliRank.pli->getSize() > rank.pli->getSize() ||
            (smallestPliRank.pli->getSize() == rank.pli->getSize() && smallestPliRank.addedArity < rank.addedArity))
            smallestPliRank = rank;
    }

    std::vector<PLIRank> operands;
    auto cover = BitSet(relationalData.schema.columns.size());
    auto coverTester = BitSet(relationalData.schema.columns.size());
    
    // FIXME  if (smallestPliRank != null)
    operands.push_back(smallestPliRank);
    cover = cover | smallestPliRank.vertical->getColumnIndices();

    while (cover.count() < vertical->getArity() && !ranks.empty()) {
        auto& bestRank = *ranks.begin();
        for(auto iter = ranks.begin(); iter != ranks.end();) {
            auto& rank = *iter;
            coverTester.clear();
            coverTester = coverTester | rank.vertical->getColumnIndices();
            coverTester = coverTester - cover;
            rank.addedArity = coverTester.count();
            if(rank.addedArity < 2) {
                iter = ranks.erase(iter);
                continue;
            }

            if(bestRank.addedArity < rank.addedArity 
                || (bestRank.addedArity == rank.addedArity && bestRank.pli->getSize() > rank.pli->getSize())) {
                bestRank = rank;        
            }

            ++iter;
        }

        operands.push_back(bestRank);
        cover = cover | bestRank.vertical->getColumnIndices();
    }

    for(const auto& column : vertical->getColumns()) {
        if(!cover.test(column->index)) {
            auto columnData = relationalData.columns.at(column->index);
            operands.emplace_back(column, columnData.pli, 1);
        }
    }

    std::sort(operands.begin(), operands.end(), [](const PLIRank& x, const PLIRank& y) -> bool {
        return x.pli->getSize() < y.pli->getSize();
    });

    std::random_device rd;
    std::mt19937_64 engine(rd());
    std::uniform_real_distribution<double> distrib(0.0, 1.0);

    if(operands.size() >= naryIntersectionSize) {
        auto basePliRank = operands.at(0);
        pli = basePliRank.pli->probAll(vertical->without(basePliRank.vertical), relationalData);
        if(distrib(engine) < cachingProbability) {
            index.put(vertical, pli);
        }
    } else {
        std::shared_ptr<Vertical> currentVertical = nullptr;
        for(auto operand : operands) {
            if(pli == nullptr) {
                currentVertical = operand.vertical;
                pli = operand.pli;
            } else {
                currentVertical = currentVertical->combine(operand.vertical);
                pli = pli->intersect(operand.pli);
                if(distrib(engine) < cachingProbability) {
                    index.put(vertical, pli);
                }
            }
        }
    }

    return pli;
}

 