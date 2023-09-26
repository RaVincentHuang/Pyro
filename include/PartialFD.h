#ifndef PARTIAL_FD_H
#define PARTIAL_FD_H

#include "Vertical.h"
#include "Column.h"
#include <memory>
#include <utility>

class PartialFD {
    double error;
    std::shared_ptr<Vertical> lhs;
    std::shared_ptr<Column> rhs;
    double score;

public:
    PartialFD(std::shared_ptr<Vertical> lhs, std::shared_ptr<Column> rhs, double error, double score)
        : error(error), lhs(std::move(lhs)), rhs(std::move(rhs)), score(score) {}

    std::string toString() {
        return lhs->toString() + " -> " + rhs->toString();// TODO Add the error and score
    }

    double getError() const {
        return error;
    }

    int getArity() {
        return lhs->getColumns().size();
    }

    
};

#endif