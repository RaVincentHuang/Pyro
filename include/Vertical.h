#ifndef VERTICAL_H
#define VERTICAL_H
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

using std::shared_ptr;

struct Schema;
class Column;

class Vertical {
public:
    virtual const boost::dynamic_bitset<size_t>& getColumnIndices() = 0;
    virtual const Schema& getSchema() = 0;
    virtual bool contains(shared_ptr<Vertical> that);
    virtual bool intersects(shared_ptr<Vertical> that);
    virtual shared_ptr<Vertical> combine(shared_ptr<Vertical> that);
    virtual shared_ptr<Vertical> intersect(shared_ptr<Vertical> that);
    virtual shared_ptr<Vertical> without(shared_ptr<Vertical> that);
    virtual shared_ptr<Vertical> invert();
    virtual shared_ptr<Vertical> invert(shared_ptr<Vertical> scope);
    virtual int getArity();
    virtual std::vector<shared_ptr<Column>> getColumns();
    virtual shared_ptr<Vertical> getColumn(size_t n);
    virtual std::vector<shared_ptr<Vertical>> getParents();
    virtual std::string toString() = 0;
};

#endif