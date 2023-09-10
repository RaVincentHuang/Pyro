#ifndef COLUMN_H
#define COLUMN_H
#include "Vertical.h"
#include <cstddef>

struct Schema;

class Column : public Vertical {
    const Schema& schema;
    boost::dynamic_bitset<size_t> indices;
public:
    std::string_view name;
    size_t index;
    Column(const Schema& schema, std::string_view name, size_t index)
        : schema(schema), name(name), index(index) {}
    const boost::dynamic_bitset<size_t>& getColumnIndices() override;
    const Schema& getSchema() override;

};

#endif