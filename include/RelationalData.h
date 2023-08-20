#ifndef RELATIONAL_DATA_H
#define RELATIONAL_DATA_H
#include <string_view>
#include <vector>

using ValueId = long;
constexpr ValueId nullValueId = -1;
constexpr ValueId singletonValueId = 0;

struct Attribute {
    std::string_view name;
    ValueId index;
};

struct Column {
    Attribute& attribute;
    std::vector<ValueId> data;
};

class RelationalData {
public:
    std::vector<Attribute> schema;
    std::vector<Column> columns;

    static RelationalData reader(std::string_view path);
};


#endif