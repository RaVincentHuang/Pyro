#ifndef COLUMN_COMBINE_H
#define COLUMN_COMBINE_H

#include "Vertical.h"
#include <cstddef>


class ColumnCombine: public Vertical {
    const Schema& schema;
    boost::dynamic_bitset<size_t> indices;
public:
    ColumnCombine(const Schema& schema, boost::dynamic_bitset<size_t> bitset)
        : schema(schema), indices(bitset) {}
    const boost::dynamic_bitset<size_t>& getColumnIndices() override;
    const Schema& getSchema() override;
};

#endif