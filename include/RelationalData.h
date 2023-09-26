#ifndef RELATIONAL_DATA_H
#define RELATIONAL_DATA_H

#include "Column.h"
#include <boost/dynamic_bitset.hpp>
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

using ValueId = long;
constexpr ValueId nullValueId = -1;
constexpr ValueId singletonValueId = 0;


struct Schema {
private:
    class EmptyVertical: public Vertical {
        boost::dynamic_bitset<size_t> columnIndices;
        const Schema& schema;
    public:
        explicit EmptyVertical(const Schema& schema);
        const boost::dynamic_bitset<size_t>& getColumnIndices() override;
        const Schema& getSchema() override;
    };
public:
    std::vector<std::shared_ptr<Column>> columns;
    std::shared_ptr<Vertical> getVertical(const boost::dynamic_bitset<size_t>& bitset) const;
};

struct ColumnData;
class RelationalData {
public:
    Schema schema;
    std::vector<ColumnData> columns;

    static RelationalData reader(std::string_view path);
};


#endif