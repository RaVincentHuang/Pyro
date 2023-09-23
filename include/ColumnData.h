#ifndef COLUMN_DATA_H
#define COLUMN_DATA_H

#include "RelationalData.h"
#include "PLICache.h"
#include <memory>

struct ColumnData {
    std::shared_ptr<Column> attribute;
    std::vector<ValueId> probingTable;
    std::shared_ptr<PLI> pli;
};

#endif