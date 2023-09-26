#ifndef DEPENDNECY_CANDIDATE_H
#define DEPENDNECY_CANDIDATE_H

#include "Common.h"
#include "ConfidenceInterval.h"
#include "Trie.h"
#include "Vertical.h"
#include <memory>
#include <utility>


struct DependencyCandidate {
    std::shared_ptr<Vertical> vertical;
    ConfidenceInterval error;
    bool _isExact;
    
    DependencyCandidate(std::shared_ptr<Vertical> vertical, ConfidenceInterval error, bool isExact)
        : vertical(std::move(vertical)), error(error), _isExact(isExact) {}

    int compareTo(const DependencyCandidate& that) {
        // Primarily order by the error.
        int result = compare(error.getMean(), that.error.getMean());
        if (result != 0) return result;

        // Use the arity to break ties.
        result = compare(that.vertical->getArity(), vertical->getArity());
        if (result != 0) return result;

        // Finally, apply a lexicographical comparison to remove duplicates.
        auto thisColumns = vertical->getColumnIndices();
        auto thatColumns = that.vertical->getColumnIndices();
        for (auto a = thisColumns.find_next(0), b = thatColumns.find_next(0);
             a != BitSet::npos;
             a = thisColumns.find_next(a + 1), b = thatColumns.find_next(b + 1)) {
            if (a < b) return -1;
            else if (a > b) return 1;
        }
        return 0;
    }

    bool operator==(const DependencyCandidate& that) const {
        return vertical == that.vertical && error == that.error && _isExact == that._isExact;
    }

    bool isExact() const {
        return _isExact && error.isPoint();
    }

};


#endif