#include "Vertical.h"
#include "RelationalData.h"
#include "Trie.h"
#include <cstddef>

bool Vertical::contains(shared_ptr<Vertical> that) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = that->getColumnIndices();
    
    return thatIndices.is_subset_of(thisIndices);
}

bool Vertical::intersects(shared_ptr<Vertical> that) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = that->getColumnIndices();

    return thatIndices.intersects(thisIndices);
}

shared_ptr<Vertical> Vertical::combine(shared_ptr<Vertical> that) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = that->getColumnIndices();

    return this->getSchema().getVertical(thisIndices | thatIndices);
}

shared_ptr<Vertical> Vertical::intersect(shared_ptr<Vertical> that) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = that->getColumnIndices();

    return this->getSchema().getVertical(thisIndices & thatIndices);
}

shared_ptr<Vertical> Vertical::without(shared_ptr<Vertical> that) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = that->getColumnIndices();

    return this->getSchema().getVertical(thisIndices - thatIndices);
}

shared_ptr<Vertical> Vertical::invert() {
    auto thisIndices = boost::dynamic_bitset<size_t>(this->getColumnIndices());
    thisIndices.flip(0, this->getSchema().columns.size());
    return this->getSchema().getVertical(thisIndices);
}

shared_ptr<Vertical> Vertical::invert(shared_ptr<Vertical> scope) {
    auto thisIndices = this->getColumnIndices();
    auto thatIndices = scope->getColumnIndices();

    return this->getSchema().getVertical(thisIndices ^ thatIndices);
}

int Vertical::getArity() {
    return this->getColumnIndices().size();
}

std::vector<shared_ptr<Column>> Vertical::getColumns() { 
    auto columnIndices = this->getColumnIndices();
    auto schema = this->getSchema();
    std::vector<std::shared_ptr<Column>> columns;
    for(auto index = columnIndices.find_first(); 
            index != boost::dynamic_bitset<size_t>::npos;
            index = columnIndices.find_next(index + 1)) {
        columns.push_back(schema.columns.at(index));
    }
    return columns;
}

shared_ptr<Vertical> Vertical::getColumn(size_t n) {
    auto columnIndices = this->getColumnIndices();
    auto columnIndex = columnIndices.find_first();
    for(size_t i = 0; i< n; i++) {
        columnIndex = columnIndices.find_next(columnIndex + 1);
    }
    return this->getSchema().columns.at(columnIndex);
}

std::vector<shared_ptr<Vertical>> Vertical::getParents() {
    if(this->getArity() < 2) 
        return {};
    std::vector<shared_ptr<Vertical>> parents;
    auto columnIndices = boost::dynamic_bitset<size_t>(this->getColumnIndices());
    for(auto columnIndex = columnIndices.find_first();
            columnIndex != boost::dynamic_bitset<size_t>::npos;
            columnIndex = columnIndices.find_next(columnIndex + 1)) {
        columnIndices.reset(columnIndex);
        parents.push_back(this->getSchema().getVertical(columnIndices));
        columnIndices.set(columnIndex);
    }
    return parents;
}
