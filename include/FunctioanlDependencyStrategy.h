#ifndef FD_DEPENDENCY_STRATEGY_H
#define FD_DEPENDENCY_STRATEGY_H

#include "DependencyCandidate.h"
#include "PartialFD.h"
#include "Vertical.h"
#include <cstddef>
#include <memory>
class SearchSpace;
// TODO derive by basic class.
class FunctionalDependencyStrategy {
    double maxDependencyError;
    double minNonDependencyError;

public:
    void ensureInitialized(const SearchSpace& searchSpace);
    DependencyCandidate createDependencyCandidate(const std::shared_ptr<Vertical>& candidate);
    double calculateError(const std::shared_ptr<Vertical>& candidate);
    void registerDependency(const std::shared_ptr<Vertical>& vertical, double error, std::vector<PartialFD>& discoveryUnit);
    bool shouldResample(const std::shared_ptr<Vertical>& vertical, double boostFactor);
    bool isIrrelevantColumn(size_t columnIndex);
    bool isIrrelevantColumn(const std::shared_ptr<Column>& column) {
        return isIrrelevantColumn(column->index);
    }
    int getNumIrrelevantColumns();
    std::shared_ptr<Vertical> getIrrelevantColumns();
};

#endif